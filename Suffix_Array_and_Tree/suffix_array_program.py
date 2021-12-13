import sys
import suffix_array as sa

# Construir el arreglo de sufijos para el texto de un archivo
file = open(sys.argv[1],'r') # Abrir el archivo de texto
text = file.read()
suffix_array = sa.suffix_array(text) # Generar arreglo de sufijos del archivo

# Programa que entra en un loop en el que pide al usuario una cadena a buscar
while True:
    # Almacenar la cadena ingresada por el usuario
    cadena = input('Ingrese una cadena a buscar ("Fin para terminar la ejecución"): ')

    # Se decidió aribtrariamente que la cadena "Fin" corresponderá a la condición de termino del programa
    if cadena == "Fin":
        break
    
    # Realizar una búsqueda binaria sobre el arreglo de sufijos para encontrar los indices que limitan el rango
    # en donde los sufijos pueden coincidir con la cadena ingresada
    l = 0 # puntero lo más a la izquierda
    r = len(suffix_array) # puntero lo más a la derecha
    while l < r:
        mid = (l+r)//2 
        if text[suffix_array[mid]:] < cadena:
            l = mid+1 # si la palabra es menor, tengo que acotar moviendo el puntero de la izquierda
        else:
            r = mid # sino, a la derecha

    # Encontrar las posibles ocurrencias de la cadena ingresada
    first = l
    while first > 0 and text[suffix_array[first - 1]: suffix_array[first - 1] + len(cadena)] == cadena:
        first -= 1

    last = r
    while text[suffix_array[last]: suffix_array[last] + len(cadena)] == cadena:
        last += 1

    # A estas alturas first y last denotan los indices de todas las ocurrencias.
    
    # Mostrar la cantidad de ocurrencia y las lineas del texto que contiene las ocurrencias de la cadena que ingresa el usuario
    print("Ocurrencias encontradas: ", last-first)
    
    #match_index = 1 # indice para mostrar el número de la ocurrencia y no la posición de esta

    for index in suffix_array[first:last]: # indices en el rango first:last
        left = index
        right = index
        while left != 0 and text[left-1] != '\n': # Retroceder el puntero left hasta llegar al comienzo de la linea de texto (encontrar un "\n" que esté antes de la frase)
            left -= 1

        while right != len(text) and text[right] != '\n': # Avanzar el puntero right hasta llegar al final de la linea de texto (encontrar un "\n" que esté dsp de la frase)
            right += 1
        
        print(index,': ', text[left:right]) # Mostrar la linea

# Cerrar el archivo de texto
file.close()