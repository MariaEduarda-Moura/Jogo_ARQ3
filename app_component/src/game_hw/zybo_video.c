#include "zybo_video.h"

#include "platform_config.h"
#include "xaxivdma.h"
#include "xil_cache.h"
#include "xil_printf.h"

#include "../display_ctrl/display_ctrl.h"

static DisplayCtrl g_display;
static XAxiVdma g_vdma;
static uint8_t g_frame[ZYBO_FRAME_SIZE] __attribute__((aligned(0x20)));
static uint8_t *g_frames[DISPLAY_NUM_FRAMES] = { g_frame, g_frame, g_frame };
static int g_initialized = 0;

static XAxiVdma_Config *zybo_vdma_lookup(uint32_t id_or_base)
{
    XAxiVdma_Config *cfg = XAxiVdma_LookupConfig(id_or_base);
    if (cfg != NULL)
        return cfg;

#ifdef XPAR_XAXIVDMA_NUM_INSTANCES
    extern XAxiVdma_Config XAxiVdma_ConfigTable[];
    int i;
    for (i = 0; i < XPAR_XAXIVDMA_NUM_INSTANCES; i++) {
        if (XAxiVdma_ConfigTable[i].BaseAddress == id_or_base)
            return &XAxiVdma_ConfigTable[i];
    }
#endif
    return NULL;
}

int zybo_video_init(void)
{
    XAxiVdma_Config *vdma_cfg;
    int status;
    u32 dynclk_addr = 0U;

    if (g_initialized)
        return 0;

    vdma_cfg = zybo_vdma_lookup(ZYBO_VDMA_BASEADDR);
    if (vdma_cfg == NULL) {
        xil_printf("HDMI init error: VDMA not found at 0x%08lx\r\n", (unsigned long)ZYBO_VDMA_BASEADDR);
        return -1;
    }

    status = XAxiVdma_CfgInitialize(&g_vdma, vdma_cfg, vdma_cfg->BaseAddress);
    if (status != XST_SUCCESS) {
        xil_printf("HDMI init error: XAxiVdma_CfgInitialize failed (%d)\r\n", status);
        return -1;
    }

#if ZYBO_HAS_AXI_DYNCLK
    dynclk_addr = XPAR_AXI_DYNCLK_0_BASEADDR;
#endif

    status = DisplayInitialize(&g_display, &g_vdma, ZYBO_VTC_BASEADDR, dynclk_addr, g_frames, ZYBO_HDMI_STRIDE);
    if (status != XST_SUCCESS) {
        xil_printf("HDMI init error: DisplayInitialize failed (%d)\r\n", status);
        return -1;
    }

    status = DisplaySetMode(&g_display, &VMODE_640x480);
    if (status != XST_SUCCESS) {
        xil_printf("HDMI init error: DisplaySetMode failed (%d)\r\n", status);
        return -1;
    }

    status = DisplayStart(&g_display);
    if (status != XST_SUCCESS) {
        xil_printf("HDMI init error: DisplayStart failed (%d)\r\n", status);
        return -1;
    }

    g_initialized = 1;
    return 0;
}

void zybo_video_fill_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t i;
    for (i = 0; i < ZYBO_FRAME_SIZE; i += 3) {
        g_frame[i + 0] = b;
        g_frame[i + 1] = g;
        g_frame[i + 2] = r;
    }
}

uint8_t *zybo_video_framebuffer(void)
{
    return g_frame;
}

void zybo_video_flush(void)
{
    Xil_DCacheFlushRange((UINTPTR)g_frame, ZYBO_FRAME_SIZE);
}
