import sys
import suffix_array as sa

# Construir el arreglo de sufijos para el texto de un archivo
file = open(sys.argv[1],'r') # Abrir el archivo de texto
text = file.read()
suffix_array = sa.suffix_array(text) # Generar arreglo de sufijos del archivo

"""
# Función para obtener la linea del texto que contiene la ocurrencia de la cadena que ingresa el usuario
def get_line(text, index):
    left = index
    right = index
    while left > 0: # Retroceder el puntero left hasta llegar al comienzo de la linea de texto (encontrar un "\n" que esté antes de la frase)
        if text[left] == "\n":
            left += 1
            break
        left -= 1 
    while right < len(text)-1: # Avanzar el puntero right hasta llegar al final de la linea de texto (encontrar un "\n" que esté dsp de la frase)
        if text[right] == "\n":
            break
        right += 1
        
    return text[left:right]
"""

# Programa que entra en un loop en el que pide al usuario una cadena a buscar
while True:
    # Almacenar la cadena ingresada por el usuario
    cadena = input('Ingrese una cadena a buscar: ')

    # Se decidió aribtrariamente que la cadena "Fin" corresponderá a la condición de termino del programa
    if cadena == "Fin":
        break
    
    # Realizar una búsqueda binaria sobre el arreglo de sufijos para encontrar los indices que limitan el rango
    # en donde los sufijos pueden coincidir con la cadena ingresada
    l = 0
    r = len(suffix_array)
    while l < r:
        mid = (l+r)//2 
        if text[suffix_array[mid]:] < cadena:
            l = mid+1
        else:
            r = mid

    # Encontrar las posibles ocurrencias de la cadena ingresada
    first = l
    while first > 0 and text[suffix_array[first - 1]: suffix_array[first - 1] + len(cadena)] == cadena:
        first -= 1
    last = r
    while text[suffix_array[last]: suffix_array[last] + len(cadena)] == cadena:
        last += 1

    print("Ocurrencias encontradas: ", last-first)
    #match_index = 1
    for index in suffix_array[first:last]: # indices en el rango first:last
        left = index
        right = index
        while left != 0 and text[left-1] != '\n': # Retroceder el puntero left hasta llegar al comienzo de la linea de texto (encontrar un "\n" que esté antes de la frase)
            left -= 1

        while right != len(text) and text[right] != '\n': # Avanzar el puntero right hasta llegar al final de la linea de texto (encontrar un "\n" que esté dsp de la frase)
            right += 1
        
        print(index,': ', text[left:right])
        #print(match_index,': ', text[left:right])
        #match_index += 1

    """
    Forma anterior
    for index in suffix_array[first:last]: # indices en el rango first:last
        print(index,': ', get_line(text, index))
    """

# Cerrar el archivo de texto
file.close()