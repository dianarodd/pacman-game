#script para generar las frecuencias de notas

def frecuencia_notas(n):
    # Se usa la fórmula para calcular frecuencias en cada semitono:
    # f = 440 * 2^((n - 69)/12)
    return round(440 * 2 ** ((n - 69) / 12))

notas_nombre = ['DO', 'DOS', 'RE', 'RES', 'MI', 'FA', 'FAS', 'SOL', 'SOLS', 'LA', 'LAS', 'SI']

def generar_codigo():
    codigo = ""
    codigo += "// notas.h generado con un programa en python\n"
    #Linea para evitar incluir notas.h mas de una vez
    codigo += "#pragma once\n\n"

    # Cada octava tiene 12 semitonos
    for octava in range(0, 8):
        codigo += f"// Octava {octava+1}\n"
        for i, nota in enumerate(notas_nombre):
            num_midi = 12 + octava * 12 + i
            freq = frecuencia_notas(num_midi)
            codigo += f"const int {nota}{octava+1} = {freq};\n"
        codigo += "\n"  # línea vacía

    # Nota especial de silencio
    codigo += "const int NONE = 0;\n"

    return "\n"+codigo

if __name__ == "__main__":
    contenido = generar_codigo()
    with open("notas.h", "w") as f:
        f.write(contenido)
    print("Archivo notas.h generado correctamente.")