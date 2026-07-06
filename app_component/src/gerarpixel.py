from PIL import Image

# 1. Abre a imagem usando o caminho bruto com o 'r'
img = Image.open(r"C:\Users\824749\Downloads\TRON\image\257029.png")

# 2. SEGREDO: Força a imagem a virar RGB para garantir que cada pixel tenha (R, G, B)
img = img.convert("RGB")

# Recorta exatamente a fileira das motos
x_inicial = 4
y_inicial = 542
largura_total = 32 * 10 
altura = 32

fileira = img.crop((x_inicial, y_inicial, x_inicial + largura_total, y_inicial + altura))

# Jeito moderno e seguro recomendado pela própria biblioteca Pillow:
pixels = list(fileira.getdata())

# Gera o arquivo header para o C
with open("sprites.h", "w") as f:
    f.write("#ifndef SPRITES_H\n#define SPRITES_H\n\n")
    f.write('#include "xil_types.h"\n\n')
    f.write("#define MOTO_SPRITE_W  32\n")
    f.write("#define MOTO_SPRITE_H  32\n")
    f.write("#define TOTAL_MOTOS    10\n\n")
    f.write("typedef enum {\n")
    f.write("    MOTO_AZUL_CIMA = 0,\n    MOTO_AZUL_DIREITA,\n    MOTO_AZUL_BAIXO,\n    MOTO_AZUL_ESQUERDA,\n")
    f.write("    MOTO_AMARELA_CIMA,\n    MOTO_AMARELA_DIREITA,\n    MOTO_AMARELA_BAIXO,\n    MOTO_AMARELA_ESQUERDA,\n")
    f.write("    MOTO_EXPLOSAO_1,\n    MOTO_EXPLOSAO_2\n} MotoSpriteID;\n\n")
    f.write("const u8 fileira_motos_data[] = {\n    ")
    
    # Escreve os pixels em formato BGR (padrão VDMA)
    for i, p in enumerate(pixels):
        # Agora p sempre será uma tupla de 3 elementos garantida pelo .convert("RGB")
        f.write(f"0x{p[2]:02X}, 0x{p[1]:02X}, 0x{p[0]:02X}, ")
        if (i + 1) % 12 == 0:
            f.write("\n    ")
            
    f.write("\n};\n\n#endif")

print("Arquivo sprites.h gerado com sucesso!")