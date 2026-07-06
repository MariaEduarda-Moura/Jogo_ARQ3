#include "display_ctrl/display_ctrl.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "sprites.h" // Inclui o arquivo gerado com os pixels das motos
#include "xgpio.h" 

/* Configurações de Endereço Base */
#define DYNCLK_BASEADDR XPAR_AXI_DYNCLK_0_BASEADDR
#define VDMA_BASEADDR   XPAR_AXI_VDMA_0_BASEADDR
#define VTC_BASEADDR    XPAR_V_TC_OUT_BASEADDR


/* Resolução Nativa do Monitor (HDMI) */
#define DEMO_WIDTH      1920
#define DEMO_HEIGHT     1080
#define DEMO_STRIDE     (DEMO_WIDTH * 3) 
#define DEMO_MAX_FRAME  (DEMO_WIDTH * DEMO_HEIGHT * 3)
#define BORDER_PIXELS   20

/* Dimensões da Arena de Jogo do Tron */
#define GAME_WIDTH      800
#define GAME_HEIGHT     600
#define GAME_STRIDE     (GAME_WIDTH * 3)

/*Botões*/
#define GPIO_INPUTS_DEVICE_ID   XPAR_AXI_GPIO_0_BASEADDR 

#define BUTTONS_CHANNEL         1 // Canal 1 do axi_gpio_0 (btns)
#define SWITCHES_CHANNEL        2 // Canal 2 do axi_gpio_0 (sws)

/* Alinhamento de memória necessário para o VDMA */
u8 frameBuf[DEMO_MAX_FRAME] __attribute__((aligned(0x20)));
u8 *pFrames[1];

// Desenha a borda da arena centralizada na tela 1080p
void draw_board_border() {
    u32 x_offset = (DEMO_WIDTH - GAME_WIDTH) / 2; 
    u32 y_offset = (DEMO_HEIGHT - GAME_HEIGHT) / 2;
    
    for (u32 y = 0; y < GAME_HEIGHT; y++) {
        for (u32 x = 0; x < GAME_WIDTH; x++) {
            u32 y_center = y + y_offset;
            u32 x_center = x + x_offset;
            u32 idx = (y_center * DEMO_STRIDE) + (x_center * 3);

            // Condição corrigida das bordas (parênteses ajustados)
            if (y < BORDER_PIXELS || y >= (GAME_HEIGHT - BORDER_PIXELS) ||
                x < BORDER_PIXELS || x >= (GAME_WIDTH - BORDER_PIXELS)) {
                
                frameBuf[idx + 0] = 255; // Azul (Componente B)
                frameBuf[idx + 1] = 255; // Verde (Componente G) -> Cria Ciano brilhante
                frameBuf[idx + 2] = 0;   // Vermelho (Componente R)
            } else {
                // Fundo escuro do jogo (Estilo grade clássica)
                frameBuf[idx + 0] = 20;  // Azul escuro
                frameBuf[idx + 1] = 0;
                frameBuf[idx + 2] = 0;
            }
        }
    }
}

// Desenha uma moto na tela baseado no ID do Spriteheet
// x_game e y_game são coordenadas de dentro do espaço 800x600
void draw_moto_sprite(u32 x_game, u32 y_game, MotoSpriteID id) {
    u32 x_offset = (DEMO_WIDTH - GAME_WIDTH) / 2;
    u32 y_offset = (DEMO_HEIGHT - GAME_HEIGHT) / 2;

    // Calcula onde começa o sprite horizontalmente na nossa folha customizada
    u32 sprite_x_inicial_na_folha = id * MOTO_SPRITE_W;

    for (u32 row = 0; row < MOTO_SPRITE_H; row++) {
        for (u32 col = 0; col < MOTO_SPRITE_W; col++) {
            
            // Coordenadas absolutas na tela HDMI (1920x1080)
            u32 screen_x = x_game + col + x_offset;
            u32 screen_y = y_game + row + y_offset;

            // Proteção contra estouro de tela
            if (screen_x >= DEMO_WIDTH || screen_y >= DEMO_HEIGHT) continue;

            // Índice do pixel no buffer de destino (HDMI)
            u32 dest_idx = (screen_y * DEMO_STRIDE) + (screen_x * 3);

            // Índice do pixel no array de origem (sprites.h)
            u32 folha_x = sprite_x_inicial_na_folha + col;
            u32 src_idx = (row * (MOTO_SPRITE_W * TOTAL_MOTOS) * 3) + (folha_x * 3);

            // Copia os bytes de cor BGR diretos da memória
            frameBuf[dest_idx + 0] = fileira_motos_data[src_idx + 0]; // B
            frameBuf[dest_idx + 1] = fileira_motos_data[src_idx + 1]; // G
            frameBuf[dest_idx + 2] = fileira_motos_data[src_idx + 2]; // R
        }
    }
}

int main(void) {
    DisplayCtrl dispCtrl;
    XAxiVdma vdma;
    XAxiVdma_Config *vdmaConfig;
    int Status;

    pFrames[0] = frameBuf;

    // Busca e inicializa o hardware de vídeo (VDMA)
    vdmaConfig = XAxiVdma_LookupConfig(VDMA_BASEADDR);
    if (!vdmaConfig) {
        xil_printf("Erro: VDMA nao encontrado no addr 0x%X\r\n", VDMA_BASEADDR);
        return -1;
    }
    
    Status = XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
    if (Status != XST_SUCCESS) {
        xil_printf("Erro na inicializacao do VDMA\r\n");
        return -1;
    }

    // Inicializa o controlador do Display
    Status = DisplayInitialize(&dispCtrl, &vdma, VTC_BASEADDR, DYNCLK_BASEADDR, pFrames, DEMO_STRIDE);
    if (Status != XST_SUCCESS) {
        xil_printf("Erro na inicializacao do Display\r\n");
        return -1;
    }

    DisplaySetMode(&dispCtrl, &VMODE_1920x1080); 
    DisplayStart(&dispCtrl);

    XGpio GpioInputs; // Instância para o bloco 0
        int Status2;

        // 1. Inicializa o bloco de Entradas (axi_gpio_0) 
        Status2 = XGpio_Initialize(&GpioInputs, GPIO_INPUTS_DEVICE_ID);
        if (Status2 != XST_SUCCESS) {
            xil_printf("Erro ao inicializar axi_gpio_0 (Entradas)!\r\n");
            return XST_FAILURE;
        }

        // 3. Configura as Direções (0 = Saída, 1 = Entrada)
        XGpio_SetDataDirection(&GpioInputs, BUTTONS_CHANNEL, 0xFFFFFFFF);  // Canal 1 como Entrada
        XGpio_SetDataDirection(&GpioInputs, SWITCHES_CHANNEL, 0xFFFFFFFF); // Canal 2 como Entrada
    u32 BotoesLidos;
    u32 SwitchesLidas;
    while (1) {
        
        BotoesLidos   = XGpio_DiscreteRead(&GpioInputs, BUTTONS_CHANNEL);
        SwitchesLidas = XGpio_DiscreteRead(&GpioInputs, SWITCHES_CHANNEL);

        printf("botao: %u | switche: %u",BotoesLidos,SwitchesLidas);
    }
/*
        if (BotoesLidos == 0) {
            botao_solto = true;
        } else if (botao_solto) {
            switch(BotoesLidos){
                case KEY_UP:    Player.movX = 0; Player.movY = -1; Player.direcao = DIR_UP;    moveu = true; break;
                case KEY_DOWN:  Player.movX = 0; Player.movY = 1;  Player.direcao = DIR_DOWN;  moveu = true; break;
                case KEY_LEFT:  Player.movX = -1; Player.movY = 0; Player.direcao = DIR_LEFT;  moveu = true; break;
                case KEY_RIGHT: Player.movX = 1; Player.movY = 0;  Player.direcao = DIR_RIGHT; moveu = true; break;
            }
            if (moveu) {
                Move(mapa_atual);
                botao_solto = false;
            }
        }
*/
        
    draw_board_border();

    // 2. Desenha os sprites usando as IDs configuradas!
    // Teste: Desenha a moto azul indo para a direita no meio da arena
    draw_moto_sprite(400, 300, MOTO_AZUL_DIREITA);

    // Teste: Desenha a moto amarela indo para cima um pouco mais para o lado
    draw_moto_sprite(200, 200, MOTO_AMARELA_CIMA);
    
    // Teste: Desenha a explosão clássica perto da borda
    draw_moto_sprite(600, 450, MOTO_EXPLOSAO_1);

    // 3. Importante: Limpa o cache para forçar os dados a saírem da CPU e irem para a RAM física onde o VDMA lê
    Xil_DCacheFlushRange((unsigned int) frameBuf, DEMO_MAX_FRAME);
    
    // Loop infinito mantendo a imagem estática no HDMI
    while (1) {}
    
    return 0;
}