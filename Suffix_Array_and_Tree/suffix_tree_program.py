import sys
import suffix_array as sa
import stackoverflow as so

# Construir el arreglo de sufijos para el texto de un archivo
file = open(sys.argv[1],'r') # Abrir el archivo de texto
text = file.read()
suffix_array = sa.suffix_array(text) # Generar arreglo de sufijos del archivo
#print(suffix_array)

root = so.Trie()
words = []
for i in suffix_array:
    words.append(text[i:])
root.build_trie(words)

"""
words = ['a', 'banana']
for word in  words:
    print(word, root.search(word)
"""

# Programa que entra en un loop en el que pide al usuario una cadena a autocompletar
while True:
    # Almacenar la cadena ingresada por el usuario
    partial_word = input('Ingrese una palabra para autocompletar: ')

    # Se decidió aribtrariamente que la cadena "Fin" corresponderá a la condición de termino del programa
    if partial_word == "Fin":
        break
    
    auto_completed_words = root.auto_complete(partial_word)
    if len(auto_completed_words)==0:
        print("no existen palabras para autocompletar en base al prefijo ingresado")
    else:
        print("sugerencias encontradas:")
        for word in auto_completed_words:
            print(word)
