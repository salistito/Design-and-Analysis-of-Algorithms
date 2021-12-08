import sys

#abrimos el archivo
archivo = open(sys.argv[1],'r')
texto = archivo.read() 

#diccionario de sufijos
dic = {i: texto[i:] for i in range(len(texto))}
arreglo_de_sufijos = [x for x, _ in sorted(dic.items(), key=lambda x: x[1])]

while True:
    cadena = input('Cadena a buscar:')

    if cadena == "Fin":
        break

    l = 0
    r = len(arreglo_de_sufijos)
    while l < r:
        mid = (l+r)//2 
        if texto[arreglo_de_sufijos[mid]:] < cadena:
            l = mid+1
        else:
            r = mid

    first = l
    while first > 0 and texto[arreglo_de_sufijos[first - 1]: arreglo_de_sufijos[first - 1] + len(cadena)] ==cadena :
        first -= 1

    last = r
    while texto[arreglo_de_sufijos[last]: arreglo_de_sufijos[last] + len(cadena)] == cadena :
        last += 1

    print("Ocurrencias encontradas: ",last-first)
    n=1
    for i in arreglo_de_sufijos[first:last]:
        atras=i 
        while texto[atras-1] != '\n' and atras != 0:
            atras-=1

        siguiente=i
        while siguiente != len(texto) and texto[siguiente] != '\n':
            siguiente+=1
        
        print(n,': ',texto[atras:siguiente])
        n+=1

archivo.close()


