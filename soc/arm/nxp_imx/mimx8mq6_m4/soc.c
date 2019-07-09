/*
 * Copyright (c) 2019, Prevas A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <init.h>
#include <soc.h>

/* OSC 27M configuration */
const osc_config_t g_osc27MConfig = {
    .oscMode = kOSC_OscMode,
    .oscDiv  = 1U,
};

/* OSC 25M configuration */
const osc_config_t g_osc25MConfig = {
    .oscMode = kOSC_OscMode,
    .oscDiv  = 1U,
};

/* AUDIO PLL1 configuration */
const ccm_analog_frac_pll_config_t g_audioPll1Config = {
    .refSel      = kANALOG_PllRefOsc25M, /*!< PLL reference OSC25M */
    .refDiv      = 5U,                   /*!< PLL input = 25 / 5 = 5M */
    .fractionDiv = 0U,
    .intDiv      = 64U, /*!< Integer and fractional Divider output  = 5 * (1 + 64) * 8 = 2600MHZ */
    .outDiv      = 4U,  /*!< Pll out frequency  = 2600 / 4 = 650MHZ */
};

/* AUDIO PLL2 configuration */
const ccm_analog_frac_pll_config_t g_audioPll2Config = {
    .refSel      = kANALOG_PllRefOsc25M, /*!< PLL reference OSC25M */
    .refDiv      = 5U,                   /*!< PLL input = 25 / 5 = 5M */
    .fractionDiv = 0U,
    .intDiv      = 64U, /*!< Integer and fractional Divider output  = 5 * (1 + 64) * 8 = 2600MHZ */
    .outDiv      = 4U,  /*!< Pll out frequency  = 2600 / 4 = 650MHZ */
};

/* VIDEO PLL1 configuration */
const ccm_analog_frac_pll_config_t g_videoPll1Config = {
    .refSel      = kANALOG_PllRefOsc25M, /*!< PLL reference OSC25M */
    .refDiv      = 5U,                   /*!< PLL input = 25 / 5 = 5M */
    .fractionDiv = 0U,
    .intDiv      = 64U, /*!< Integer and fractional Divider output  = 5 * (1 + 64) * 8 = 2600MHZ */
    .outDiv      = 4U,  /*!< Pll out frequency  = 2600 / 4 = 650MHZ */
};

/* SYSTEM PLL1 configuration */
const ccm_analog_sscg_pll_config_t g_sysPll1Config = {
    .refSel       = kANALOG_PllRefOsc25M, /*!< PLL reference OSC25M */
    .refDiv1      = 1U,                   /*!< PLL1 input = 25 / 1 = 25MHZ */
    .loopDivider1 = 32U,                  /*!< PLL1 output  = 25 * 32 * 2 = 1600MHZ */
    .refDiv2      = 24U,                  /*!< PLL2 input  = 1600 / 24 = 66.66MHZ */
    .loopDivider2 = 12U,                  /*!< PLL2 output  = 12 * 66.66 * 2 = 1600MHZ */
    .outDiv       = 1U,                   /*!< PLL output = 1600 / 2 / 1 = 800MHZ */
};

/* SYSTEM PLL2 configuration */
const ccm_analog_sscg_pll_config_t g_sysPll2Config = {
    .refSel       = kANALOG_PllRefOsc25M, /*!< PLL reference OSC25M */
    .refDiv1      = 1U,                   /*!< PLL1 input = 25 / 1 = 25MHZ */
    .loopDivider1 = 32U,                  /*!< PLL1 output  = 25 * 32 * 2 = 1600MHZ */
    .refDiv2      = 16U,                  /*!< PLL2 input  = 1600 / 16 = 100MHZ */
    .loopDivider2 = 10U,                  /*!< PLL2 output  = 10 * 100 * 2 = 2000MHZ */
    .outDiv       = 1U,                   /*!< PLL output = 2000 / 2 / 1 = 1000MHZ */
};

/* SYSTEM PLL3 configuration */
const ccm_analog_sscg_pll_config_t g_sysPll3Config = {
    .refSel       = kANALOG_PllRefOsc25M, /*!< PLL reference OSC25M */
    .refDiv1      = 1U,                   /*!< PLL1 input = 25 / 1 = 25MHZ */
    .loopDivider1 = 32U,                  /*!< PLL1 output  = 25 * 32 * 2 = 1600MHZ */
    .refDiv2      = 16U,                  /*!< PLL2 input  = 1600 / 16 = 100MHZ */
    .loopDivider2 = 10U,                  /*!< PLL2 output  = 10 * 100 * 2 = 2000MHZ */
    .outDiv       = 1U,                   /*!< PLL output = 2000 / 2 / 1 = 1000MHZ */
};

void SOC_clkInit(void)
{
    /* OSC configuration */
    CLOCK_InitOSC25M(&g_osc25MConfig);
    CLOCK_InitOSC27M(&g_osc27MConfig);

    /* The following steps just show how to configure the PLL clock sources using the clock driver on M4 core side .
     * Please note that the ROM has already configured the SYSTEM PLL1 to 800Mhz when power up the SOC, meanwhile A core
     * would enable the Div output for SYSTEM PLL1 & PLL2 by U-Boot.
     * Therefore, there is no need to configure the system PLL again on M4 side, otherwise it would have a risk to make
     * the SOC hang.
     */

    /* switch AHB NOC root to 25M first in order to configure the SYSTEM PLL1. */
    //    CLOCK_SetRootMux(kCLOCK_RootAhb, kCLOCK_AhbRootmuxOsc25m);
    //    CLOCK_SetRootMux(kCLOCK_RootNoc, kCLOCK_NocRootmuxOsc25m);
    /* switch AXI root to 25M first in order to configure the SYSTEM PLL2. */
    //    CLOCK_SetRootMux(kCLOCK_RootAxi, kCLOCK_AxiRootmuxOsc25m);

    //    CLOCK_InitSysPll1(&g_sysPll1Config); /* init SYSTEM PLL1 run at 800MHZ */
    //    CLOCK_InitSysPll2(&g_sysPll2Config); /* init SYSTEM PLL2 run at 1000MHZ */
    //    CLOCK_InitSysPll3(&g_sysPll3Config); /* init SYSTEM PLL3 run at 1000MHZ */
    //
    //    CLOCK_InitAudioPll1(&g_audioPll1Config); /* init AUDIO PLL1 run at 650MHZ */
    //    CLOCK_InitAudioPll2(&g_audioPll2Config); /* init AUDIO PLL2 run at 650MHZ */
    //    CLOCK_InitVideoPll1(&g_videoPll1Config); /* init VIDEO PLL1 run at 650MHZ */

    CLOCK_SetRootDivider(kCLOCK_RootM4, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootM4, kCLOCK_M4RootmuxSysPll1Div3); /* switch cortex-m4 to SYSTEM PLL1 DIV3 */
    //    CLOCK_SetRootMux(kCLOCK_RootNoc, kCLOCK_NocRootmuxSysPll1);   /* change back to SYSTEM PLL1*/

    CLOCK_SetRootDivider(kCLOCK_RootAhb, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootAhb, kCLOCK_AhbRootmuxSysPll1Div6); /* switch AHB to SYSTEM PLL1 DIV6 = 133MHZ */

    CLOCK_SetRootDivider(kCLOCK_RootAxi, 3U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootAxi, kCLOCK_AxiRootmuxSysPll1); /* switch AXI to SYSTEM PLL1 = 266MHZ */

    CLOCK_SetRootMux(kCLOCK_RootUart2, kCLOCK_UartRootmuxSysPll1Div10); /* Set UART source to SysPLL1 Div10 80MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootUart2, 1U, 1U);                     /* Set root clock to 80MHZ/ 1= 80MHZ */

    CLOCK_EnableClock(kCLOCK_Rdc); /* Enable RDC clock */

    /* The purpose to enable the following modules clock is to make sure the M4 core could work normally when A53 core
     * enters the low power status.*/
    // CLOCK_EnableClock(kCLOCK_Sim_m);
    // CLOCK_EnableClock(kCLOCK_Sim_main);
    // CLOCK_EnableClock(kCLOCK_Sim_s);
    // CLOCK_EnableClock(kCLOCK_Sim_wakeup);
    // CLOCK_EnableClock(kCLOCK_Debug);
    // CLOCK_EnableClock(kCLOCK_Dram);
    // CLOCK_EnableClock(kCLOCK_Sec_Debug);

    /* Disable unused PLL */
    CLOCK_DeinitSysPll3();
    CLOCK_DeinitVideoPll1();
    CLOCK_DeinitAudioPll1();
    CLOCK_DeinitAudioPll2();
    /* Update core clock */
    SystemCoreClockUpdate();

}

/* Initialize MPU, configure non-cacheable memory */
void SOC_InitMemory(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Load$$LR$$LR_cache_region$$Base[];
    extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit[];
    uint32_t cacheStart = (uint32_t)Load$$LR$$LR_cache_region$$Base;
    uint32_t size       = (cacheStart < 0x20000000U) ? (0) : ((uint32_t)Image$$ARM_LIB_STACK$$ZI$$Limit - cacheStart);
#else
    extern uint32_t __CACHE_REGION_START[];
    extern uint32_t __CACHE_REGION_SIZE[];
    uint32_t cacheStart = (uint32_t)__CACHE_REGION_START;
    uint32_t size       = (uint32_t)__CACHE_REGION_SIZE;
#endif
    uint32_t i = 0;
    /* Make sure outstanding transfers are done. */
    __DMB();
    /* Disable the MPU. */
    MPU->CTRL = 0;

    /*
     *  The ARMv7-M default address map define the address space 0x20000000 to 0x3FFFFFFF as SRAM with Normal type, but
     *  there the address space 0x28000000 ~ 0x3FFFFFFF has been physically mapped to smart subsystems, so there need
     *  change the default memory attributes.
     *  Since the base address of MPU region should be multiples of region size, to make it simple, the MPU region 0 set
     *  the all 512M of SRAM space with device attributes, then disable subregion 0 and 1 (address space 0x20000000 ~
     *  0x27FFFFFF) to use the
     *  background memory attributes.
     */

    /* Select Region 0 and set its base address to the M4 code bus start address. */
    MPU->RBAR = (0x20000000U & MPU_RBAR_ADDR_Msk) | MPU_RBAR_VALID_Msk | (0 << MPU_RBAR_REGION_Pos);

    /* Region 0 setting:
     * 1) Disable Instruction Access;
     * 2) AP = 011b, full access;
     * 3) Non-shared device;
     * 4) Region Not Shared;
     * 5) Sub-Region 0,1 Disabled;
     * 6) MPU Protection Region size = 512M byte;
     * 7) Enable Region 0.
     */
    MPU->RASR = (0x1 << MPU_RASR_XN_Pos) | (0x3 << MPU_RASR_AP_Pos) | (0x2 << MPU_RASR_TEX_Pos) |
                (0x3 << MPU_RASR_SRD_Pos) | (28 << MPU_RASR_SIZE_Pos) | MPU_RASR_ENABLE_Msk;

    /*
     *  Non-cacheable area is provided in DDR memory, the DDR region 2MB - 128MB totally 126MB is revserved for CM4
     *  cores. You can put global or static uninitialized variables in NonCacheable section(initialized variables in
     *  NonCacheable.init section) to make them uncacheable. Since the base address of MPU region should be multiples of
     * region size,
     *  to make it simple, the MPU region 1 & 2 set all DDR address space 0x40000000 ~ 0xBFFFFFFF to be non-cacheable).
     *  Then MPU region 3 set the text and data section to be cacheable if the program running on DDR.
     *  The cacheable area base address should be multiples of its size in linker file, they can be modified per your
     * needs.
     */

    /* Select Region 1 and set its base address to the DDR start address. */
    MPU->RBAR = (0x40000000U & MPU_RBAR_ADDR_Msk) | MPU_RBAR_VALID_Msk | (1 << MPU_RBAR_REGION_Pos);

    /* Region 1 setting:
     * 1) Enable Instruction Access;
     * 2) AP = 011b, full access;
     * 3) Shared Device;
     * 4) MPU Protection Region size = 1024M byte;
     * 5) Enable Region 1.
     */
    MPU->RASR = (0x3 << MPU_RASR_AP_Pos) | (0x1 << MPU_RASR_B_Pos) | (29 << MPU_RASR_SIZE_Pos) | MPU_RASR_ENABLE_Msk;

    /* Select Region 2 and set its base address to the DDR start address. */
    MPU->RBAR = (0x80000000U & MPU_RBAR_ADDR_Msk) | MPU_RBAR_VALID_Msk | (2 << MPU_RBAR_REGION_Pos);

    /* Region 2 setting:
     * 1) Enable Instruction Access;
     * 2) AP = 011b, full access;
     * 3) Shared Device;
     * 4) MPU Protection Region size = 1024M byte;
     * 5) Enable Region 2.
     */
    MPU->RASR = (0x3 << MPU_RASR_AP_Pos) | (0x1 << MPU_RASR_B_Pos) | (29 << MPU_RASR_SIZE_Pos) | MPU_RASR_ENABLE_Msk;

    while ((size >> i) > 0x1U)
    {
        i++;
    }

    /* If run on DDR, configure text and data section to be cacheable */
    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert((size & (size - 1)) == 0);
        assert(!(cacheStart % size));
        assert(size == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Select Region 3 and set its base address to the cache able region start address. */
        MPU->RBAR = (cacheStart & MPU_RBAR_ADDR_Msk) | MPU_RBAR_VALID_Msk | (3 << MPU_RBAR_REGION_Pos);

        /* Region 3 setting:
         * 1) Enable Instruction Access;
         * 2) AP = 011b, full access;
         * 3) Outer and inner Cacheable, write and read allocate;
         * 4) Region Not Shared;
         * 5) All Sub-Region Enabled;
         * 6) MPU Protection Region size get from linker file;
         * 7) Enable Region 3.
         */
        MPU->RASR = (0x3 << MPU_RASR_AP_Pos) | (0x1 << MPU_RASR_TEX_Pos) | (0x1 << MPU_RASR_C_Pos) |
                    (0x1 << MPU_RASR_B_Pos) | ((i - 1) << MPU_RASR_SIZE_Pos) | MPU_RASR_ENABLE_Msk;
    }

    /* Enable Privileged default memory map and the MPU. */
    MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
    /* Memory barriers to ensure subsequence data & instruction
     * transfers using updated MPU settings.
     */
    __DSB();
    __ISB();
}
void SOC_RdcInit(void)
{
	/* Move M4 core to specific RDC domain 1 */
	rdc_domain_assignment_t assignment = {0};

	assignment.domainId = M4_DOMAIN_ID;
	RDC_SetMasterDomainAssignment(RDC, kRDC_Master_M4, &assignment);

	CLOCK_EnableClock(kCLOCK_Iomux0);
	CLOCK_EnableClock(kCLOCK_Iomux1);
	CLOCK_EnableClock(kCLOCK_Iomux2);
	CLOCK_EnableClock(kCLOCK_Iomux3);
	CLOCK_EnableClock(kCLOCK_Iomux4);

	CLOCK_ControlGate(kCLOCK_SysPll1Gate, kCLOCK_ClockNeededAll);
	CLOCK_ControlGate(kCLOCK_SysPll2Gate, kCLOCK_ClockNeededAll);
	CLOCK_ControlGate(kCLOCK_SysPll3Gate, kCLOCK_ClockNeededAll);
	CLOCK_ControlGate(kCLOCK_AudioPll1Gate, kCLOCK_ClockNeededAll);
	CLOCK_ControlGate(kCLOCK_AudioPll2Gate, kCLOCK_ClockNeededAll);
	CLOCK_ControlGate(kCLOCK_VideoPll1Gate, kCLOCK_ClockNeededAll);
}

static int nxp_mimx8mq6_init(struct device *arg)
{
	ARG_UNUSED(arg);

	/* SoC specific RDC settings */
	SOC_RdcInit();

	SOC_clkInit();
	SOC_InitMemory();

	return 0;
}

SYS_INIT(nxp_mimx8mq6_init, PRE_KERNEL_1, 0);
