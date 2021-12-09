def suffix_array(texto):
    # suffix_array que contiene el texto envés de los indices
    #sa = sorted([texto[i:] for i in range(len(texto))])

    # diccionario de sufijos
    dic = {i: texto[i:] for i in range(len(texto))}
    # suffix_array
    suffix_array = [index for index, _ in sorted(dic.items(), key=lambda x: x[1])] # Se ordenar el diccionario según el orden lexicográfico de sus valores (key=dic.values())
    
    """
    # debug del suffix array y a que sufijos corresponden los indices
    print(suffix_array)
    for i in suffix_array:
        print(texto[i:])
    """

    return suffix_array

