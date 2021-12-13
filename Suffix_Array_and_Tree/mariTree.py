import suffix_array as sa
import sys

# Construir el arreglo de sufijos para el texto de un archivo
file = open(sys.argv[1],'r') # Abrir el archivo de texto
print("Leyendo el archivo: "+ sys.argv[1])
text = file.read()
suffix_array = sa.suffix_array(text) # Generar arreglo de sufijos del archivo
tree_suffix_array = [] # arreglo de sufijos pero obtenido desde el árbol

class Node:
    def __init__(self, index_or_char='', common_len=0):
        self.index_or_char = index_or_char  # Si es una hoja contiene un indice del arreglo de sufijos y si es un nodo interno un caracter
        self.common_len = common_len  # cantidad de letras en común de las palabras que nacen desde un nodo interno
        self.children = []  # lista de hijos
        self.weight = 0  # cantidad de nodos hojas que cuelgan del nodo
        self.accumulated_len = 0  # cantidad acumulada de la cantidad de letras en común de las palabras que nacen desde un nodo interno
        self.depth = 0  # profundidad a la cual se ubica el nodo en el árbol
        self.is_root = False  # valor booleano para verificar si es raíz
        self.is_leaf = True  # valor booleano para verificar si es hoja
    
    # Función para insertar sufijos al PatriciaTree
    def insert(self, index):
        palabra = text[index:] # palabra que se va a insertar
        node = self.search(palabra, mode="insert") # realizar búsqueda para insertar
        # print("Nodo en el cual se está intentando insertar: ", node.index_or_char, ",", node.common_len,",", node.weight)

        if node.children==[]: # La primera vez cuando root es null
            # i) inserción normal en nodo interno -> añadirlo como hoja
            new_node=Node(index)
            new_node.depth+=1
            node.children.append(new_node)
            node.is_leaf = False
            return

        # Todos los demás casos -> 3 casos: i) inserción normal en nodo, ii) inserción que genera un nodo interno y iii) inserción con split
        for child in node.children:
            if child.is_leaf: # Iterar sobre los hijos para chequear posibles coincidencias
                #print("Se esta recorriendo: ", child.index_or_char, ",", child.common_len,",", child.weight, "profundidad:", child.depth)
                
                count = 0
                if not node.is_leaf:
                    count += node.depth

                leaf_palabra = text[child.index_or_char:]
                largo_min = min(len(leaf_palabra), len(palabra))
                for i in range(node.depth, largo_min): # +1 pq range se come el último
                    if(leaf_palabra[i] == palabra[i]):
                        count += 1
                    else:
                        break

                if node.is_root and child.is_leaf and text[child.index_or_char+node.depth]==palabra[node.depth]: # una hoja se tranforma en nodo interno
                    # Crear hojas que serán los hijos
                    leaf1 = Node(child.index_or_char)
                    leaf2 = Node(index)
                    leaf1.depth=child.depth+1
                    leaf2.depth=child.depth+1
                    child.children.append(leaf1)
                    child.children.append(leaf2)
                    child.weight += 2
                    # Cambiar estado de la hoja a un nodo interno
                    child.index_or_char = palabra[node.accumulated_len]
                    child.is_leaf = False
                    # Actualizar cantidad de letras en comun
                    child.common_len = count-node.accumulated_len
                    child.accumulated_len = child.common_len + node.accumulated_len
                    return

                elif not node.is_root and child.is_leaf and text[child.index_or_char+node.depth:child.index_or_char+node.depth+node.common_len]==palabra[node.depth:node.depth+node.common_len]: #una hoja se tranforma en nodo
                    if count != node.accumulated_len:
                        # Crear hojas que serán los hijos
                        leaf1 = Node(child.index_or_char)
                        leaf2 = Node(index)
                        leaf1.depth=child.depth+1
                        leaf2.depth=child.depth+1
                        child.children.append(leaf1)
                        child.children.append(leaf2)
                        child.weight += 2
                        # Cambiar estado de la hoja a un nodo interno
                        child.index_or_char = palabra[node.accumulated_len]
                        child.is_leaf = False
                        # Actualizar cantidad de letras en comun
                        child.common_len = count-node.accumulated_len
                        child.accumulated_len = child.common_len + node.accumulated_len
                        return

                elif count < node.accumulated_len: # Caso díficil, tienen letras en común pero no son todas -> split del nodo
                    # Nuevos parametros para el nodo que sube en el split (modificar nodo actual)
                    new_node_common_len = node.accumulated_len-count  
                    new_node_weight = node.weight
                    new_node_acummulated_len = node.accumulated_len-new_node_common_len
                    new_node_depth = node.depth

                    # Crear hoja
                    leaf = Node(index)
                    leaf.depth=node.depth+1
                    
                    # Nuevos parametros para el nodo que baja en el split (se crea un nodo nuevo con ellos)
                    changed_node_letter = text[child.index_or_char+count]
                    changed_node_common_len = node.common_len-new_node_common_len
                    changed_node_childs = node.children
                    for child in changed_node_childs:
                        child.update_depth()
                    changed_node_weight = node.weight-1

                    # Crear nodo que baja
                    changed_node = Node(changed_node_letter, new_node_common_len)
                    changed_node.children = changed_node_childs
                    changed_node.weight = changed_node_weight
                    changed_node.depth = node.depth+1
                    changed_node.is_leaf = False

                    # Modificar nodo que se tenía
                    node.common_len = changed_node_common_len
                    node.children = [changed_node, leaf]
                    node.weight = new_node_weight
                    node.accumulated_len = new_node_acummulated_len
                    node.depth = new_node_depth
                    return

        # Si no entra en ningun caso -> i) inserción normal en nodo
        new_node=Node(index)
        new_node.depth=node.depth+1
        node.children.append(new_node)
        return

    # Función que actualiza las profundidades recursivamente
    def update_depth(self):
        if self.is_leaf:
            self.depth+=1
        else:
            self.depth+=1
            for child in self.children:
                child.update_depth()

    # Funciín que inicializa el PatriciaTree insertandoles todos los sufijos del suffix array
    def build_trie(self):
        for index in suffix_array:
            self.insert(index)

    # Función para visualizar los nodos del PatriciaTree
    def print_PatriciaTree(self):
        for child in self.children:
            if child.is_leaf:
                print("Hoja: ", child.index_or_char, ",", child.common_len,",", child.weight, "depth:", child.depth, "accumulated_len:", child.accumulated_len)
            else:
                print("Nodo: ", child.index_or_char, ",", child.common_len,",",child.weight, "depth:", child.depth, "accumulated_len:", child.accumulated_len)
                child.print_PatriciaTree()

    # Función que imprime solo las hojas del PatriciaTree
    def print_leaves(self):
        if self.is_leaf: # Si es hoja
            print(self.index_or_char) # Imprimir
            return
        else: # Si es un nodo interno
            for child in self.children: # Llamada recursiva a los hijos
                child.print_leaves()

    # Obtiene los indices insertados en las hojas del PatriciaTree en orden de izquierda a derecha, obteniendo así el suffix array original
    def get_suffix_array(self):
        if self.is_leaf: # Si es hoja
            tree_suffix_array.append(self.index_or_char) # guardar en el array el indice
            return
        else: # Si es un nodo interno
            for child in self.children: # Llamada recursiva a los hijos
                child.get_suffix_array()
    
    # Función que devuelve las palabras recomendadas a partir de un nodo
    def get_words(self, words=[], word_counter=0):
        nodes_and_weights = {}
        heavist_childs = []
        for child in self.children:
            #print("Hoja: ", child.index_or_char, ",", child.common_len, ",", child.weight)
            if word_counter == 3:
                break

            elif child.is_leaf:
                words.append(text[child.index_or_char:])
                word_counter+=1

            else: # es un nodo interno
                nodes_and_weights[child] = child.weight

        # Si aún no se tienen 3 palabras para recomendar
        if word_counter<3:
            # Ordenar la lista de los nodos internos más pesados para recomendar esas palabras
            heavist_childs = [node for node, _ in sorted(nodes_and_weights.items(), key=lambda x: x[1], reverse=True)][:3-word_counter]

        if word_counter<3 and len(nodes_and_weights)==0: # estoy en el root o en el caso donde solo tengo 2 hojas o 1 hoja
            return words

        # Caso en que se tienen menos de 3 palabras recomendadas pero es posible búscar en el resto de nodos internos
        elif word_counter<3 and len(nodes_and_weights)>= 3-word_counter:
            heavist_childs = [node for node, _ in sorted(nodes_and_weights.items(), key=lambda x: x[1], reverse=True)][:3-word_counter]
            for i in range(len(heavist_childs)):
                words = heavist_childs[i].get_words(words, 2)

        # Caso en que se tenía 1 palabra recomendada y solo un nodos interno
        elif word_counter==1 and len(nodes_and_weights)==1:
            words = heavist_childs[0].get_words(words, 1) # retorna 2 palabras

        # Caso en que se tenían 0 palabras recomendadas y dos nodos internos
        elif word_counter==0 and len(nodes_and_weights)==2:
            words = heavist_childs[0].get_words(words, 1) # retorna 2 palabras
            words = heavist_childs[1].get_words(words, 2) # retorna 2 palabras
        
        return words

    # Función que retorna la primera hoja que encuentre
    def getFirstChildren(self):
        if type(self.index_or_char) == int:
            return self.index_or_char
        else:
            #no estamos en un hijo normal
            for hijos in self.children:
                return hijos.getFirstChildren()

    # Función de búsqueda del PatriciaTree, devuelve el nodo más cercano a la ocurrencia de la palabra
    # es utilizado para el autocompletado y para realizar la inserción adecuadamente
    def search(self, palabra, mode=""):
        if mode=="insert":
            self.weight+=1
        if self.is_leaf:
            return self
        elif self.is_root:
            for hijo in self.children:
                if type(hijo.index_or_char) == str:
                    # estamos en un nodo interno
                    if hijo.index_or_char == palabra[0]:
                        return hijo.search(palabra, mode)
                elif type(hijo.index_or_char) == int:
                    # estamos en una hoja
                    if text[hijo.index_or_char:]==palabra:
                        return self
                    if text[hijo.index_or_char] == palabra[0]:
                        return self
            #print("no se encontró palabras y estamos en root, el resultado será basura")
            return self

        else:
            if self.children == []:
                return self
            else:
                largo = self.common_len
                if largo>=len(palabra):
                    return self
                elif largo==len(palabra):
                    for hijo in self.children:
                        if type(hijo.index_or_char) == int:
                            # estamos en una hoja
                            if text[hijo.index_or_char:]==palabra:
                                #print("palabras iguales")
                                return hijo
                    return self
                
                else:
                    # buscar en los hijos
                    #print("vamos a buscar en los hijos ya que ",largo,"<",len(palabra))
                    hijo= None
                    for h in self.children:
                        if type(h.index_or_char) == int:
                            hijo = h

                    if hijo == None:
                        #print("no existe ningun hijo con indice, todos son nodos internos")
                        indice = None
                        for chi in self.children:
                            #print("chi.index",chi.index_or_char)
                            #print("common len",palabra[self.common_len])
                            if chi.index_or_char == palabra[self.common_len]:
                                indice = chi.getFirstChildren()
                                comun = 0
                                texto1= palabra
                                texto2= text[indice:]
                                i = 0
                                while i < min(len(texto1),len(texto2)):
                                    if texto1[i] == texto2[i]:
                                        comun +=1
                                    else:
                                        break
                                    i+=1

                                if comun >= self.common_len:
                                    nueva_palabra = palabra[largo:]
                                    return chi.search(nueva_palabra, mode)

                        return self

                    else:
                        indice = hijo.index_or_char
                        i = 0
                        for i in range(0,largo):
                            if palabra[i] != text[indice]:
                                return self
                            indice += 1
                        nueva_palabra = palabra[largo:]
                        for hijo in self.children:
                            if hijo.index_or_char == nueva_palabra[0]:
                                return hijo.search(nueva_palabra, mode)
                        return self

    def searchBool(self, palabra):
        nodo = self.search(palabra)
        if type(nodo.index_or_char) == int:
            indice = nodo.index_or_char
            palabra_a_comparar = text[indice:]
            if palabra_a_comparar == palabra:
                return True
            else:
                #print("palabras diferentes")
                return False
        else:
            for hijo in nodo.children:
                if type(hijo.index_or_char) == int:
                    indice = hijo.index_or_char
                    palabra_a_comparar = text[indice:]
                    #print("palabra a comparar:",palabra_a_comparar)
                    if palabra_a_comparar == palabra:
                        return True
                    
            return False

    # Función que printea un nodo, útil para realizar debugging
    def print_search(self, node):
            if node.is_leaf:
                print("Hoja:", node.index_or_char, ",", node.common_len,",", node.weight)
                print("palabra:", text[node.index_or_char:])
            else:
                print("Nodo: ", node.index_or_char, ",", node.common_len,",", node.weight)
                for child in node.children:
                    child.print_search(child)
                    

root = Node()
root.is_root = True
root.is_leaf = False
root.build_trie()

# caso trivial 1:
#root.insert2(13)
#root.insert2(5)
#root.insert2(8)
#root.insert2(11)
#root.insert2(3)
#root.insert2(1)

# caso trivial 2:
#root.insert2(12)
#root.insert2(4)
#root.insert2(10)
#root.insert2(2)

# Caso peluo:
#root.insert2(7)
#root.insert2(0)
#root.insert2(9)

# Caso caca cacho
#root.insert(4)
#root.insert(3)
#root.insert(1)
#root.insert(6)
#root.insert(2)
#root.insert(0)
#root.insert(5)
#root.insert(7)
#root.insert(8)
#root.insert(9)

# caso arnolarnobarbolarbosarcangelarcanso:
#root.insert(23) # angelarcanso
#root.insert(31) # anso
#root.insert(10) # arbolarbosarcangelarcanso
#root.insert(15) # arbosarcangelarcanso
#root.insert(20) # arcangelarcanso
#root.insert(28) # arcanso
#root.insert(5)  # arnobarbolarbosarcangelarcanso
#root.insert(0)  # arnolarnobarbolarbosarcangelarcanso
#root.insert(9)  # barbolarbosarcangelarcanso
#root.insert(12)
#root.insert(17)

#root.print_PatriciaTree()
root.get_suffix_array()
print("El suffixArray y el de la Paty son iguales?")
print(suffix_array==tree_suffix_array)
error_counter = 0
if (suffix_array==tree_suffix_array) == False:
    for i in range(len(suffix_array)):
        if suffix_array[i] != tree_suffix_array[i]:
            print("debería ser:", suffix_array[i], "pero fue", tree_suffix_array[i])
            error_counter+=1
    print("cantidad de errores:", error_counter)

# Programa que entra en un loop en el que pide al usuario una cadena a buscar
aux_word = ""
corte = 20
while True:
    # Almacenar la cadena ingresada por el usuario
    partial_word = input('Ingrese una cadena a buscar ("Fin para terminar la ejecución"): ' + aux_word)
    aux_word += partial_word

    # Se decidió aribtrariamente que la cadena "Fin" corresponderá a la condición de termino del programa
    if partial_word == "Fin":
        break
    
    nodo_encontrado = root.search(aux_word)
    print("Podrías estar buscando estas palabras:")
    recommended_words = nodo_encontrado.get_words([], 0)
    for word in recommended_words:
        if len(word)>corte:
            word = word[:corte]
            word+="..."
        print("->", word)
