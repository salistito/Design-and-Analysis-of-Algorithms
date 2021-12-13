# str -> list
def suffix_array(text):
    # suffix_array que contiene el texto envés de los indices
    #sa = sorted([texto[i:] for i in range(len(texto))])

    # diccionario de sufijos
    dic = {i: text[i:] for i in range(len(text))}

    # Ahora con el diccionario se creará una lista donde estén las "tuplas" key-value del diccionario ordenadas lexicográficamente
    suffix_array = [index for index, _ in sorted(dic.items(), key=lambda x: x[1])] # Se ordenar el diccionario según el orden lexicográfico de sus valores (key=dic.values())
    
    # debug del suffix array y a que sufijos corresponden los indices
    #print(suffix_array)
    #for i in suffix_array:
        #print(i, ":", text[i:])
    return suffix_array

