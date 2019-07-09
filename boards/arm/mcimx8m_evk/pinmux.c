/*
 * Copyright (c) 2019, Prevas A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <init.h>
#include <fsl_iomuxc.h>

static int mcimx8m_evk_pinmux_init(struct device *dev)
{
	ARG_UNUSED(dev);

	IOMUXC_SetPinMux(IOMUXC_UART2_RXD_UART2_RX, 0U);
	IOMUXC_SetPinConfig(IOMUXC_UART2_RXD_UART2_RX,
			IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
			IOMUXC_SW_PAD_CTL_PAD_RSE(1U) |
			IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
	IOMUXC_SetPinMux(IOMUXC_UART2_TXD_UART2_TX, 0U);
	IOMUXC_SetPinConfig(IOMUXC_UART2_TXD_UART2_TX,
			IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
			IOMUXC_SW_PAD_CTL_PAD_RSE(1U) |
			IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);

	return 0;
}

SYS_INIT(mcimx8m_evk_pinmux_init, PRE_KERNEL_1, 0);
