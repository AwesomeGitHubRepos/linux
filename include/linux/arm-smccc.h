/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2015, Linaro Limited
 */
#ifndef __LINUX_ARM_SMCCC_H
#define __LINUX_ARM_SMCCC_H

#include <linux/args.h>
#include <linux/init.h>

#ifndef __ASSEMBLY__
#include <linux/uuid.h>
#endif

#include <uapi/linux/const.h>

/*
 * This file provides common defines for ARM SMC Calling Convention as
 * specified in
 * https://developer.arm.com/docs/den0028/latest
 *
 * This code is up-to-date with version DEN 0028 C
 */

#define ARM_SMCCC_STD_CALL	        _AC(0,U)
#define ARM_SMCCC_FAST_CALL	        _AC(1,U)
#define ARM_SMCCC_TYPE_SHIFT		31

#define ARM_SMCCC_SMC_32		0
#define ARM_SMCCC_SMC_64		1
#define ARM_SMCCC_CALL_CONV_SHIFT	30

#define ARM_SMCCC_OWNER_MASK		0x3F
#define ARM_SMCCC_OWNER_SHIFT		24

#define ARM_SMCCC_FUNC_MASK		0xFFFF

#define ARM_SMCCC_IS_FAST_CALL(smc_val)	\
	((smc_val) & (ARM_SMCCC_FAST_CALL << ARM_SMCCC_TYPE_SHIFT))
#define ARM_SMCCC_IS_64(smc_val) \
	((smc_val) & (ARM_SMCCC_SMC_64 << ARM_SMCCC_CALL_CONV_SHIFT))
#define ARM_SMCCC_FUNC_NUM(smc_val)	((smc_val) & ARM_SMCCC_FUNC_MASK)
#define ARM_SMCCC_OWNER_NUM(smc_val) \
	(((smc_val) >> ARM_SMCCC_OWNER_SHIFT) & ARM_SMCCC_OWNER_MASK)

#define ARM_SMCCC_CALL_VAL(type, calling_convention, owner, func_num) \
	(((type) << ARM_SMCCC_TYPE_SHIFT) | \
	((calling_convention) << ARM_SMCCC_CALL_CONV_SHIFT) | \
	(((owner) & ARM_SMCCC_OWNER_MASK) << ARM_SMCCC_OWNER_SHIFT) | \
	((func_num) & ARM_SMCCC_FUNC_MASK))

#define ARM_SMCCC_OWNER_ARCH		0
#define ARM_SMCCC_OWNER_CPU		1
#define ARM_SMCCC_OWNER_SIP		2
#define ARM_SMCCC_OWNER_OEM		3
#define ARM_SMCCC_OWNER_STANDARD	4
#define ARM_SMCCC_OWNER_STANDARD_HYP	5
#define ARM_SMCCC_OWNER_VENDOR_HYP	6
#define ARM_SMCCC_OWNER_TRUSTED_APP	48
#define ARM_SMCCC_OWNER_TRUSTED_APP_END	49
#define ARM_SMCCC_OWNER_TRUSTED_OS	50
#define ARM_SMCCC_OWNER_TRUSTED_OS_END	63

#define ARM_SMCCC_FUNC_QUERY_CALL_UID  0xff01

#define ARM_SMCCC_QUIRK_NONE		0
#define ARM_SMCCC_QUIRK_QCOM_A6		1 /* Save/restore register a6 */

#define ARM_SMCCC_VERSION_1_0		0x10000
#define ARM_SMCCC_VERSION_1_1		0x10001
#define ARM_SMCCC_VERSION_1_2		0x10002
#define ARM_SMCCC_VERSION_1_3		0x10003

#define ARM_SMCCC_1_3_SVE_HINT		0x10000
#define ARM_SMCCC_CALL_HINTS		ARM_SMCCC_1_3_SVE_HINT


#define ARM_SMCCC_VERSION_FUNC_ID					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   0, 0)

#define ARM_SMCCC_ARCH_FEATURES_FUNC_ID					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   0, 1)

#define ARM_SMCCC_ARCH_SOC_ID						\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   0, 2)

#define ARM_SMCCC_ARCH_WORKAROUND_1					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   0, 0x8000)

#define ARM_SMCCC_ARCH_WORKAROUND_2					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   0, 0x7fff)

#define ARM_SMCCC_ARCH_WORKAROUND_3					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   0, 0x3fff)

#define ARM_SMCCC_VENDOR_HYP_CALL_UID_FUNC_ID				\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_FUNC_QUERY_CALL_UID)

/* KVM UID value: 28b46fb6-2ec5-11e9-a9ca-4b564d003a74 */
#define ARM_SMCCC_VENDOR_HYP_UID_KVM UUID_INIT(\
	0x28b46fb6, 0x2ec5, 0x11e9, \
	0xa9, 0xca, 0x4b, 0x56, \
	0x4d, 0x00, 0x3a, 0x74)

/* KVM "vendor specific" services */
#define ARM_SMCCC_KVM_FUNC_FEATURES		0
#define ARM_SMCCC_KVM_FUNC_PTP			1
/* Start of pKVM hypercall range */
#define ARM_SMCCC_KVM_FUNC_HYP_MEMINFO		2
#define ARM_SMCCC_KVM_FUNC_MEM_SHARE		3
#define ARM_SMCCC_KVM_FUNC_MEM_UNSHARE		4
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_5		5
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_6		6
#define ARM_SMCCC_KVM_FUNC_MMIO_GUARD		7
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_8		8
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_9		9
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_10		10
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_11		11
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_12		12
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_13		13
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_14		14
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_15		15
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_16		16
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_17		17
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_18		18
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_19		19
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_20		20
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_21		21
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_22		22
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_23		23
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_24		24
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_25		25
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_26		26
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_27		27
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_28		28
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_29		29
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_30		30
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_31		31
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_32		32
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_33		33
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_34		34
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_35		35
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_36		36
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_37		37
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_38		38
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_39		39
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_40		40
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_41		41
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_42		42
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_43		43
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_44		44
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_45		45
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_46		46
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_47		47
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_48		48
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_49		49
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_50		50
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_51		51
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_52		52
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_53		53
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_54		54
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_55		55
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_56		56
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_57		57
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_58		58
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_59		59
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_60		60
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_61		61
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_62		62
#define ARM_SMCCC_KVM_FUNC_PKVM_RESV_63		63
/* End of pKVM hypercall range */
#define ARM_SMCCC_KVM_FUNC_DISCOVER_IMPL_VER	64
#define ARM_SMCCC_KVM_FUNC_DISCOVER_IMPL_CPUS	65

#define ARM_SMCCC_KVM_FUNC_FEATURES_2		127
#define ARM_SMCCC_KVM_NUM_FUNCS			128

#define ARM_SMCCC_VENDOR_HYP_KVM_FEATURES_FUNC_ID			\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_FEATURES)

#define SMCCC_ARCH_WORKAROUND_RET_UNAFFECTED	1

/*
 * ptp_kvm is a feature used for time sync between vm and host.
 * ptp_kvm module in guest kernel will get service from host using
 * this hypercall ID.
 */
#define ARM_SMCCC_VENDOR_HYP_KVM_PTP_FUNC_ID				\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_32,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_PTP)

#define ARM_SMCCC_VENDOR_HYP_KVM_HYP_MEMINFO_FUNC_ID			\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_64,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_HYP_MEMINFO)

#define ARM_SMCCC_VENDOR_HYP_KVM_MEM_SHARE_FUNC_ID			\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_64,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_MEM_SHARE)

#define ARM_SMCCC_VENDOR_HYP_KVM_MEM_UNSHARE_FUNC_ID			\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_64,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_MEM_UNSHARE)

#define ARM_SMCCC_VENDOR_HYP_KVM_MMIO_GUARD_FUNC_ID			\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_64,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_MMIO_GUARD)

#define ARM_SMCCC_VENDOR_HYP_KVM_DISCOVER_IMPL_VER_FUNC_ID		\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_64,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_DISCOVER_IMPL_VER)

#define ARM_SMCCC_VENDOR_HYP_KVM_DISCOVER_IMPL_CPUS_FUNC_ID		\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,				\
			   ARM_SMCCC_SMC_64,				\
			   ARM_SMCCC_OWNER_VENDOR_HYP,			\
			   ARM_SMCCC_KVM_FUNC_DISCOVER_IMPL_CPUS)

/* ptp_kvm counter type ID */
#define KVM_PTP_VIRT_COUNTER			0
#define KVM_PTP_PHYS_COUNTER			1

/* Paravirtualised time calls (defined by ARM DEN0057A) */
#define ARM_SMCCC_HV_PV_TIME_FEATURES				\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,			\
			   ARM_SMCCC_SMC_64,			\
			   ARM_SMCCC_OWNER_STANDARD_HYP,	\
			   0x20)

#define ARM_SMCCC_HV_PV_TIME_ST					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,			\
			   ARM_SMCCC_SMC_64,			\
			   ARM_SMCCC_OWNER_STANDARD_HYP,	\
			   0x21)

/* TRNG entropy source calls (defined by ARM DEN0098) */
#define ARM_SMCCC_TRNG_VERSION					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,			\
			   ARM_SMCCC_SMC_32,			\
			   ARM_SMCCC_OWNER_STANDARD,		\
			   0x50)

#define ARM_SMCCC_TRNG_FEATURES					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,			\
			   ARM_SMCCC_SMC_32,			\
			   ARM_SMCCC_OWNER_STANDARD,		\
			   0x51)

#define ARM_SMCCC_TRNG_GET_UUID					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,			\
			   ARM_SMCCC_SMC_32,			\
			   ARM_SMCCC_OWNER_STANDARD,		\
			   0x52)

#define ARM_SMCCC_TRNG_RND32					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,			\
			   ARM_SMCCC_SMC_32,			\
			   ARM_SMCCC_OWNER_STANDARD,		\
			   0x53)

#define ARM_SMCCC_TRNG_RND64					\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL,			\
			   ARM_SMCCC_SMC_64,			\
			   ARM_SMCCC_OWNER_STANDARD,		\
			   0x53)

/*
 * Return codes defined in ARM DEN 0070A
 * ARM DEN 0070A is now merged/consolidated into ARM DEN 0028 C
 */
#define SMCCC_RET_SUCCESS			0
#define SMCCC_RET_NOT_SUPPORTED			-1
#define SMCCC_RET_NOT_REQUIRED			-2
#define SMCCC_RET_INVALID_PARAMETER		-3

#ifndef __ASSEMBLY__

#include <linux/linkage.h>
#include <linux/types.h>

enum arm_smccc_conduit {
	SMCCC_CONDUIT_NONE,
	SMCCC_CONDUIT_SMC,
	SMCCC_CONDUIT_HVC,
};

/**
 * arm_smccc_1_1_get_conduit()
 *
 * Returns the conduit to be used for SMCCCv1.1 or later.
 *
 * When SMCCCv1.1 is not present, returns SMCCC_CONDUIT_NONE.
 */
enum arm_smccc_conduit arm_smccc_1_1_get_conduit(void);

/**
 * arm_smccc_get_version()
 *
 * Returns the version to be used for SMCCCv1.1 or later.
 *
 * When SMCCCv1.1 or above is not present, returns SMCCCv1.0, but this
 * does not imply the presence of firmware or a valid conduit. Caller
 * handling SMCCCv1.0 must determine the conduit by other means.
 */
u32 arm_smccc_get_version(void);

void __init arm_smccc_version_init(u32 version, enum arm_smccc_conduit conduit);

/**
 * arm_smccc_get_soc_id_version()
 *
 * Returns the SOC ID version.
 *
 * When ARM_SMCCC_ARCH_SOC_ID is not present, returns SMCCC_RET_NOT_SUPPORTED.
 */
s32 arm_smccc_get_soc_id_version(void);

/**
 * arm_smccc_get_soc_id_revision()
 *
 * Returns the SOC ID revision.
 *
 * When ARM_SMCCC_ARCH_SOC_ID is not present, returns SMCCC_RET_NOT_SUPPORTED.
 */
s32 arm_smccc_get_soc_id_revision(void);

#ifndef __ASSEMBLY__

/*
 * Returns whether a specific hypervisor UUID is advertised for the
 * Vendor Specific Hypervisor Service range.
 */
bool arm_smccc_hypervisor_has_uuid(const uuid_t *uuid);

static inline uuid_t smccc_res_to_uuid(u32 r0, u32 r1, u32 r2, u32 r3)
{
	uuid_t uuid = {
		.b = {
			[0]  = (r0 >> 0)  & 0xff,
			[1]  = (r0 >> 8)  & 0xff,
			[2]  = (r0 >> 16) & 0xff,
			[3]  = (r0 >> 24) & 0xff,

			[4]  = (r1 >> 0)  & 0xff,
			[5]  = (r1 >> 8)  & 0xff,
			[6]  = (r1 >> 16) & 0xff,
			[7]  = (r1 >> 24) & 0xff,

			[8]  = (r2 >> 0)  & 0xff,
			[9]  = (r2 >> 8)  & 0xff,
			[10] = (r2 >> 16) & 0xff,
			[11] = (r2 >> 24) & 0xff,

			[12] = (r3 >> 0)  & 0xff,
			[13] = (r3 >> 8)  & 0xff,
			[14] = (r3 >> 16) & 0xff,
			[15] = (r3 >> 24) & 0xff,
		},
	};

	return uuid;
}

static inline u32 smccc_uuid_to_reg(const uuid_t *uuid, int reg)
{
	u32 val = 0;

	val |= (u32)(uuid->b[4 * reg + 0] << 0);
	val |= (u32)(uuid->b[4 * reg + 1] << 8);
	val |= (u32)(uuid->b[4 * reg + 2] << 16);
	val |= (u32)(uuid->b[4 * reg + 3] << 24);

	return val;
}

#endif /* !__ASSEMBLY__ */

/**
 * struct arm_smccc_res - Result from SMC/HVC call
 * @a0-a3 result values from registers 0 to 3
 */
struct arm_smccc_res {
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
};

#ifdef CONFIG_ARM64
/**
 * struct arm_smccc_1_2_regs - Arguments for or Results from SMC/HVC call
 * @a0-a17 argument values from registers 0 to 17
 */
struct arm_smccc_1_2_regs {
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
	unsigned long a4;
	unsigned long a5;
	unsigned long a6;
	unsigned long a7;
	unsigned long a8;
	unsigned long a9;
	unsigned long a10;
	unsigned long a11;
	unsigned long a12;
	unsigned long a13;
	unsigned long a14;
	unsigned long a15;
	unsigned long a16;
	unsigned long a17;
};

/**
 * arm_smccc_1_2_hvc() - make HVC calls
 * @args: arguments passed via struct arm_smccc_1_2_regs
 * @res: result values via struct arm_smccc_1_2_regs
 *
 * This function is used to make HVC calls following SMC Calling Convention
 * v1.2 or above. The content of the supplied param are copied from the
 * structure to registers prior to the HVC instruction. The return values
 * are updated with the content from registers on return from the HVC
 * instruction.
 */
asmlinkage void arm_smccc_1_2_hvc(const struct arm_smccc_1_2_regs *args,
				  struct arm_smccc_1_2_regs *res);

/**
 * arm_smccc_1_2_smc() - make SMC calls
 * @args: arguments passed via struct arm_smccc_1_2_regs
 * @res: result values via struct arm_smccc_1_2_regs
 *
 * This function is used to make SMC calls following SMC Calling Convention
 * v1.2 or above. The content of the supplied param are copied from the
 * structure to registers prior to the SMC instruction. The return values
 * are updated with the content from registers on return from the SMC
 * instruction.
 */
asmlinkage void arm_smccc_1_2_smc(const struct arm_smccc_1_2_regs *args,
				  struct arm_smccc_1_2_regs *res);
#endif

/**
 * struct arm_smccc_quirk - Contains quirk information
 * @id: quirk identification
 * @state: quirk specific information
 * @a6: Qualcomm quirk entry for returning post-smc call contents of a6
 */
struct arm_smccc_quirk {
	int	id;
	union {
		unsigned long a6;
	} state;
};

/**
 * __arm_smccc_smc() - make SMC calls
 * @a0-a7: arguments passed in registers 0 to 7
 * @res: result values from registers 0 to 3
 * @quirk: points to an arm_smccc_quirk, or NULL when no quirks are required.
 *
 * This function is used to make SMC calls following SMC Calling Convention.
 * The content of the supplied param are copied to registers 0 to 7 prior
 * to the SMC instruction. The return values are updated with the content
 * from register 0 to 3 on return from the SMC instruction.  An optional
 * quirk structure provides vendor specific behavior.
 */
#ifdef CONFIG_HAVE_ARM_SMCCC
asmlinkage void __arm_smccc_smc(unsigned long a0, unsigned long a1,
			unsigned long a2, unsigned long a3, unsigned long a4,
			unsigned long a5, unsigned long a6, unsigned long a7,
			struct arm_smccc_res *res, struct arm_smccc_quirk *quirk);
#else
static inline void __arm_smccc_smc(unsigned long a0, unsigned long a1,
			unsigned long a2, unsigned long a3, unsigned long a4,
			unsigned long a5, unsigned long a6, unsigned long a7,
			struct arm_smccc_res *res, struct arm_smccc_quirk *quirk)
{
	*res = (struct arm_smccc_res){};
}
#endif

/**
 * __arm_smccc_hvc() - make HVC calls
 * @a0-a7: arguments passed in registers 0 to 7
 * @res: result values from registers 0 to 3
 * @quirk: points to an arm_smccc_quirk, or NULL when no quirks are required.
 *
 * This function is used to make HVC calls following SMC Calling
 * Convention.  The content of the supplied param are copied to registers 0
 * to 7 prior to the HVC instruction. The return values are updated with
 * the content from register 0 to 3 on return from the HVC instruction.  An
 * optional quirk structure provides vendor specific behavior.
 */
asmlinkage void __arm_smccc_hvc(unsigned long a0, unsigned long a1,
			unsigned long a2, unsigned long a3, unsigned long a4,
			unsigned long a5, unsigned long a6, unsigned long a7,
			struct arm_smccc_res *res, struct arm_smccc_quirk *quirk);

#define arm_smccc_smc(...) __arm_smccc_smc(__VA_ARGS__, NULL)

#define arm_smccc_smc_quirk(...) __arm_smccc_smc(__VA_ARGS__)

#define arm_smccc_hvc(...) __arm_smccc_hvc(__VA_ARGS__, NULL)

#define arm_smccc_hvc_quirk(...) __arm_smccc_hvc(__VA_ARGS__)

/* SMCCC v1.1 implementation madness follows */
#ifdef CONFIG_ARM64

#define SMCCC_SMC_INST	"smc	#0"
#define SMCCC_HVC_INST	"hvc	#0"

#elif defined(CONFIG_ARM)
#include <asm/opcodes-sec.h>
#include <asm/opcodes-virt.h>

#define SMCCC_SMC_INST	__SMC(0)
#define SMCCC_HVC_INST	__HVC(0)

#endif

#define __constraint_read_2	"r" (arg0)
#define __constraint_read_3	__constraint_read_2, "r" (arg1)
#define __constraint_read_4	__constraint_read_3, "r" (arg2)
#define __constraint_read_5	__constraint_read_4, "r" (arg3)
#define __constraint_read_6	__constraint_read_5, "r" (arg4)
#define __constraint_read_7	__constraint_read_6, "r" (arg5)
#define __constraint_read_8	__constraint_read_7, "r" (arg6)
#define __constraint_read_9	__constraint_read_8, "r" (arg7)

#define __declare_arg_2(a0, res)					\
	struct arm_smccc_res   *___res = res;				\
	register unsigned long arg0 asm("r0") = (u32)a0

#define __declare_arg_3(a0, a1, res)					\
	typeof(a1) __a1 = a1;						\
	struct arm_smccc_res   *___res = res;				\
	register unsigned long arg0 asm("r0") = (u32)a0;			\
	register typeof(a1) arg1 asm("r1") = __a1

#define __declare_arg_4(a0, a1, a2, res)				\
	typeof(a1) __a1 = a1;						\
	typeof(a2) __a2 = a2;						\
	struct arm_smccc_res   *___res = res;				\
	register unsigned long arg0 asm("r0") = (u32)a0;			\
	register typeof(a1) arg1 asm("r1") = __a1;			\
	register typeof(a2) arg2 asm("r2") = __a2

#define __declare_arg_5(a0, a1, a2, a3, res)				\
	typeof(a1) __a1 = a1;						\
	typeof(a2) __a2 = a2;						\
	typeof(a3) __a3 = a3;						\
	struct arm_smccc_res   *___res = res;				\
	register unsigned long arg0 asm("r0") = (u32)a0;			\
	register typeof(a1) arg1 asm("r1") = __a1;			\
	register typeof(a2) arg2 asm("r2") = __a2;			\
	register typeof(a3) arg3 asm("r3") = __a3

#define __declare_arg_6(a0, a1, a2, a3, a4, res)			\
	typeof(a4) __a4 = a4;						\
	__declare_arg_5(a0, a1, a2, a3, res);				\
	register typeof(a4) arg4 asm("r4") = __a4

#define __declare_arg_7(a0, a1, a2, a3, a4, a5, res)			\
	typeof(a5) __a5 = a5;						\
	__declare_arg_6(a0, a1, a2, a3, a4, res);			\
	register typeof(a5) arg5 asm("r5") = __a5

#define __declare_arg_8(a0, a1, a2, a3, a4, a5, a6, res)		\
	typeof(a6) __a6 = a6;						\
	__declare_arg_7(a0, a1, a2, a3, a4, a5, res);			\
	register typeof(a6) arg6 asm("r6") = __a6

#define __declare_arg_9(a0, a1, a2, a3, a4, a5, a6, a7, res)		\
	typeof(a7) __a7 = a7;						\
	__declare_arg_8(a0, a1, a2, a3, a4, a5, a6, res);		\
	register typeof(a7) arg7 asm("r7") = __a7

/*
 * We have an output list that is not necessarily used, and GCC feels
 * entitled to optimise the whole sequence away. "volatile" is what
 * makes it stick.
 */
#define __arm_smccc_1_1(inst, ...)					\
	do {								\
		register unsigned long r0 asm("r0");			\
		register unsigned long r1 asm("r1");			\
		register unsigned long r2 asm("r2");			\
		register unsigned long r3 asm("r3"); 			\
		CONCATENATE(__declare_arg_,				\
			    COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__);	\
		asm volatile(inst "\n" :				\
			     "=r" (r0), "=r" (r1), "=r" (r2), "=r" (r3)	\
			     : CONCATENATE(__constraint_read_,		\
					   COUNT_ARGS(__VA_ARGS__))	\
			     : "memory");				\
		if (___res)						\
			*___res = (typeof(*___res)){r0, r1, r2, r3};	\
	} while (0)

/*
 * arm_smccc_1_1_smc() - make an SMCCC v1.1 compliant SMC call
 *
 * This is a variadic macro taking one to eight source arguments, and
 * an optional return structure.
 *
 * @a0-a7: arguments passed in registers 0 to 7
 * @res: result values from registers 0 to 3
 *
 * This macro is used to make SMC calls following SMC Calling Convention v1.1.
 * The content of the supplied param are copied to registers 0 to 7 prior
 * to the SMC instruction. The return values are updated with the content
 * from register 0 to 3 on return from the SMC instruction if not NULL.
 */
#define arm_smccc_1_1_smc(...)	__arm_smccc_1_1(SMCCC_SMC_INST, __VA_ARGS__)

/*
 * arm_smccc_1_1_hvc() - make an SMCCC v1.1 compliant HVC call
 *
 * This is a variadic macro taking one to eight source arguments, and
 * an optional return structure.
 *
 * @a0-a7: arguments passed in registers 0 to 7
 * @res: result values from registers 0 to 3
 *
 * This macro is used to make HVC calls following SMC Calling Convention v1.1.
 * The content of the supplied param are copied to registers 0 to 7 prior
 * to the HVC instruction. The return values are updated with the content
 * from register 0 to 3 on return from the HVC instruction if not NULL.
 */
#define arm_smccc_1_1_hvc(...)	__arm_smccc_1_1(SMCCC_HVC_INST, __VA_ARGS__)

/*
 * Like arm_smccc_1_1* but always returns SMCCC_RET_NOT_SUPPORTED.
 * Used when the SMCCC conduit is not defined. The empty asm statement
 * avoids compiler warnings about unused variables.
 */
#define __fail_smccc_1_1(...)						\
	do {								\
		CONCATENATE(__declare_arg_,				\
			    COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__);	\
		asm ("" :						\
		     : CONCATENATE(__constraint_read_,			\
				   COUNT_ARGS(__VA_ARGS__))		\
		     : "memory");					\
		if (___res)						\
			___res->a0 = SMCCC_RET_NOT_SUPPORTED;		\
	} while (0)

/*
 * arm_smccc_1_1_invoke() - make an SMCCC v1.1 compliant call
 *
 * This is a variadic macro taking one to eight source arguments, and
 * an optional return structure.
 *
 * @a0-a7: arguments passed in registers 0 to 7
 * @res: result values from registers 0 to 3
 *
 * This macro will make either an HVC call or an SMC call depending on the
 * current SMCCC conduit. If no valid conduit is available then -1
 * (SMCCC_RET_NOT_SUPPORTED) is returned in @res.a0 (if supplied).
 *
 * The return value also provides the conduit that was used.
 */
#define arm_smccc_1_1_invoke(...) ({					\
		int method = arm_smccc_1_1_get_conduit();		\
		switch (method) {					\
		case SMCCC_CONDUIT_HVC:					\
			arm_smccc_1_1_hvc(__VA_ARGS__);			\
			break;						\
		case SMCCC_CONDUIT_SMC:					\
			arm_smccc_1_1_smc(__VA_ARGS__);			\
			break;						\
		default:						\
			__fail_smccc_1_1(__VA_ARGS__);			\
			method = SMCCC_CONDUIT_NONE;			\
			break;						\
		}							\
		method;							\
	})

#ifdef CONFIG_ARM64

#define __fail_smccc_1_2(___res)					\
	do {								\
		if (___res)						\
			___res->a0 = SMCCC_RET_NOT_SUPPORTED;		\
	} while (0)

/*
 * arm_smccc_1_2_invoke() - make an SMCCC v1.2 compliant call
 *
 * @args: SMC args are in the a0..a17 fields of the arm_smcc_1_2_regs structure
 * @res: result values from registers 0 to 17
 *
 * This macro will make either an HVC call or an SMC call depending on the
 * current SMCCC conduit. If no valid conduit is available then -1
 * (SMCCC_RET_NOT_SUPPORTED) is returned in @res.a0 (if supplied).
 *
 * The return value also provides the conduit that was used.
 */
#define arm_smccc_1_2_invoke(args, res) ({				\
		struct arm_smccc_1_2_regs *__args = args;		\
		struct arm_smccc_1_2_regs *__res = res;			\
		int method = arm_smccc_1_1_get_conduit();		\
		switch (method) {					\
		case SMCCC_CONDUIT_HVC:					\
			arm_smccc_1_2_hvc(__args, __res);		\
			break;						\
		case SMCCC_CONDUIT_SMC:					\
			arm_smccc_1_2_smc(__args, __res);		\
			break;						\
		default:						\
			__fail_smccc_1_2(__res);			\
			method = SMCCC_CONDUIT_NONE;			\
			break;						\
		}							\
		method;							\
	})
#endif /*CONFIG_ARM64*/

#endif /*__ASSEMBLY__*/
#endif /*__LINUX_ARM_SMCCC_H*/
