import sys

#abrimos el archivo
archivo = open(sys.argv[1],'r')
texto = archivo.read() 

sa = sorted([texto[i:] for i in range(len(texto))])

#diccionario de sufijos
dic = {i: texto[i:] for i in range(len(texto))}
arreglo_de_sufijos = [x for x, _ in sorted(dic.items(), key=lambda x: x[1])]
#print(arreglo_de_sufijos)

#for i in arreglo_de_sufijos:
#    print(texto[i:])


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
    i=1
    for i in arreglo_de_sufijos[first:last]:
        print(i,': ', texto[i-3:])
        i+=1


archivo.close()

