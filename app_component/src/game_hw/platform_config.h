#ifndef ZYBO_PLATFORM_CONFIG_H
#define ZYBO_PLATFORM_CONFIG_H

#include "xparameters.h"

#ifndef XPAR_AXI_DYNCLK_0_BASEADDR
#if defined(XPAR_AXI_DYNCLK_0_S_AXI_LITE_BASEADDR)
#define XPAR_AXI_DYNCLK_0_BASEADDR XPAR_AXI_DYNCLK_0_S_AXI_LITE_BASEADDR
#define ZYBO_HAS_AXI_DYNCLK 1
#elif defined(XPAR_AXI_DYNCLK_BASEADDR)
#define XPAR_AXI_DYNCLK_0_BASEADDR XPAR_AXI_DYNCLK_BASEADDR
#define ZYBO_HAS_AXI_DYNCLK 1
#else
#define XPAR_AXI_DYNCLK_0_BASEADDR 0x43C00000U
#define ZYBO_HAS_AXI_DYNCLK 0
#warning "Using fallback AXI_DYNCLK base address"
#endif
#else
#define ZYBO_HAS_AXI_DYNCLK 1
#endif

#if defined(XPAR_AXI_VDMA_0_BASEADDR)
#define ZYBO_VDMA_BASEADDR XPAR_AXI_VDMA_0_BASEADDR
#elif defined(XPAR_AXIVDMA_0_BASEADDR)
#define ZYBO_VDMA_BASEADDR XPAR_AXIVDMA_0_BASEADDR
#else
#error "Could not find AXI VDMA base address macro in xparameters.h"
#endif

#if defined(XPAR_V_TC_OUT_BASEADDR)
#define ZYBO_VTC_BASEADDR XPAR_V_TC_OUT_BASEADDR
#elif defined(XPAR_V_TC_OUT_0_BASEADDR)
#define ZYBO_VTC_BASEADDR XPAR_V_TC_OUT_0_BASEADDR
#else
#error "Could not find VTC output base address macro in xparameters.h"
#endif

#if defined(XPAR_AXI_GPIO_BTN_0_BASEADDR)
#define ZYBO_BTN_GPIO_BASEADDR XPAR_AXI_GPIO_BTN_0_BASEADDR
#elif defined(XPAR_AXI_GPIO_0_BASEADDR)
#define ZYBO_BTN_GPIO_BASEADDR XPAR_AXI_GPIO_0_BASEADDR
#else
#define ZYBO_BTN_GPIO_BASEADDR 0x41200000U
#warning "Using fallback GPIO base address for buttons"
#endif
#define ZYBO_BTN_GPIO_CHANNEL 2U

#define ZYBO_HDMI_WIDTH   640U
#define ZYBO_HDMI_HEIGHT  480U
#define ZYBO_HDMI_BPP     3U
#define ZYBO_HDMI_STRIDE  (ZYBO_HDMI_WIDTH * ZYBO_HDMI_BPP)
#define ZYBO_FRAME_SIZE   (ZYBO_HDMI_WIDTH * ZYBO_HDMI_HEIGHT * ZYBO_HDMI_BPP)

#endif /* ZYBO_PLATFORM_CONFIG_H */
