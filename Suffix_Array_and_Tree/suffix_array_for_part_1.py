import sys
import suffix_array as sa

# Construir el arreglo de sufijos para el texto de un archivo
file = open(sys.argv[1],'r') # Abrir el archivo de texto
print("Leyendo el archivo: "+ sys.argv[1])
text = file.read()
suffix_array = sa.suffix_array(text) # Generar arreglo de sufijos del archivo
print("Arreglo de sufijos:")
print(suffix_array)
