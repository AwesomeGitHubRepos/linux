// SPDX-License-Identifier: GPL-2.0+
/*
 * Raspberry Pi voltage sensor driver
 *
 * Based on firmware/raspberrypi.c by Noralf Trønnes
 *
 * Copyright (C) 2018 Stefan Wahren <stefan.wahren@i2se.com>
 */
#include <linux/device.h>
#include <linux/devm-helpers.h>
#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <soc/bcm2835/raspberrypi-firmware.h>

#define UNDERVOLTAGE_STICKY_BIT	BIT(16)

struct rpi_hwmon_data {
	struct device *hwmon_dev;
	struct rpi_firmware *fw;
	u32 last_throttled;
	struct delayed_work get_values_poll_work;
};

static void rpi_firmware_get_throttled(struct rpi_hwmon_data *data)
{
	u32 new_uv, old_uv, value;
	int ret;

	/* Request firmware to clear sticky bits */
	value = 0xffff;

	ret = rpi_firmware_property(data->fw, RPI_FIRMWARE_GET_THROTTLED,
				    &value, sizeof(value));
	if (ret) {
		dev_err_once(data->hwmon_dev, "Failed to get throttled (%d)\n",
			     ret);
		return;
	}

	new_uv = value & UNDERVOLTAGE_STICKY_BIT;
	old_uv = data->last_throttled & UNDERVOLTAGE_STICKY_BIT;
	data->last_throttled = value;

	if (new_uv == old_uv)
		return;

	if (new_uv)
		dev_crit(data->hwmon_dev, "Undervoltage detected!\n");
	else
		dev_info(data->hwmon_dev, "Voltage normalised\n");

	hwmon_notify_event(data->hwmon_dev, hwmon_in, hwmon_in_lcrit_alarm, 0);
}

static void get_values_poll(struct work_struct *work)
{
	struct rpi_hwmon_data *data;

	data = container_of(work, struct rpi_hwmon_data,
			    get_values_poll_work.work);

	rpi_firmware_get_throttled(data);

	/*
	 * We can't run faster than the sticky shift (100ms) since we get
	 * flipping in the sticky bits that are cleared.
	 */
	schedule_delayed_work(&data->get_values_poll_work, 2 * HZ);
}

static int rpi_read(struct device *dev, enum hwmon_sensor_types type,
		    u32 attr, int channel, long *val)
{
	struct rpi_hwmon_data *data = dev_get_drvdata(dev);

	*val = !!(data->last_throttled & UNDERVOLTAGE_STICKY_BIT);
	return 0;
}

static const struct hwmon_channel_info * const rpi_info[] = {
	HWMON_CHANNEL_INFO(in,
			   HWMON_I_LCRIT_ALARM),
	NULL
};

static const struct hwmon_ops rpi_hwmon_ops = {
	.visible = 0444,
	.read = rpi_read,
};

static const struct hwmon_chip_info rpi_chip_info = {
	.ops = &rpi_hwmon_ops,
	.info = rpi_info,
};

static int rpi_hwmon_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct rpi_hwmon_data *data;
	int ret;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	/* Parent driver assure that firmware is correct */
	data->fw = dev_get_drvdata(dev->parent);

	data->hwmon_dev = devm_hwmon_device_register_with_info(dev, "rpi_volt",
							       data,
							       &rpi_chip_info,
							       NULL);
	if (IS_ERR(data->hwmon_dev))
		return PTR_ERR(data->hwmon_dev);

	ret = devm_delayed_work_autocancel(dev, &data->get_values_poll_work,
					   get_values_poll);
	if (ret)
		return ret;
	platform_set_drvdata(pdev, data);

	schedule_delayed_work(&data->get_values_poll_work, 2 * HZ);

	return 0;
}

static int rpi_hwmon_suspend(struct device *dev)
{
	struct rpi_hwmon_data *data = dev_get_drvdata(dev);

	cancel_delayed_work_sync(&data->get_values_poll_work);

	return 0;
}

static int rpi_hwmon_resume(struct device *dev)
{
	struct rpi_hwmon_data *data = dev_get_drvdata(dev);

	get_values_poll(&data->get_values_poll_work.work);

	return 0;
}

static DEFINE_SIMPLE_DEV_PM_OPS(rpi_hwmon_pm_ops, rpi_hwmon_suspend,
				rpi_hwmon_resume);

static struct platform_driver rpi_hwmon_driver = {
	.probe = rpi_hwmon_probe,
	.driver = {
		.name = "raspberrypi-hwmon",
		.pm = pm_ptr(&rpi_hwmon_pm_ops),
	},
};
module_platform_driver(rpi_hwmon_driver);

MODULE_AUTHOR("Stefan Wahren <wahrenst@gmx.net>");
MODULE_DESCRIPTION("Raspberry Pi voltage sensor driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:raspberrypi-hwmon");
