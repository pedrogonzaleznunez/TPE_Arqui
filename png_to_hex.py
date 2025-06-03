from PIL import Image

COLOR_IGNORADO = 0xFFFFFFFF

# Cargar la imagen en modo RGBA
img = Image.open("./resources/character.png").convert("RGBA")
width = height = max(img.size)
pixels = list(img.crop((0, 0, width, height)).getdata())

# Colores base a ignorar (fondos verdes)
ignored_base_colors = [
    (0x9A, 0xE7, 0xD9),  # #9ae7d9
    (0x66, 0xDC, 0xC8),  # #66dcc8
    (0x99, 0xD8, 0xC9),  # #99d8c9
]

# Tolerancia para colores similares
TOLERANCE = 30

# Función para verificar si un color está dentro del rango de tolerancia
def is_ignored_color(r, g, b):
    for ir, ig, ib in ignored_base_colors:
        if abs(r - ir) <= TOLERANCE and abs(g - ig) <= TOLERANCE and abs(b - ib) <= TOLERANCE:
            return True
    return False

# Crear archivo .h
with open("character_data.h", "w") as f:
    f.write(f"#define CHARACTER_WIDTH {width}\n")
    f.write(f"#define CHARACTER_HEIGHT {height}\n")
    f.write("uint32_t characterPixels[] = {\n")

    for y in range(height):
        for x in range(width):
            i = y * width + x
            r, g, b, a = pixels[i]

            if is_ignored_color(r, g, b) or a < 128:
                color = COLOR_IGNORADO # píxel transparente
            else:
                color = (r << 16) | (g << 8) | b

            f.write(f"0x{color:08X},")
        f.write("\n")
    f.write("};\n")

print("✅ Archivo 'character_data.h' generado.")