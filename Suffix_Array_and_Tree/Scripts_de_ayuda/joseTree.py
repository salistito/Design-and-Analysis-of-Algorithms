import curses
import sys
import copy
from typing import KeysView

# El propósito de este código es implementar un árbol de sufijos
# para la búsqueda de palabras.

# Para eso utilizaremos un árbol patricia, donde cada nodo tiene una
# lista de hijos, un peso y un puntero a la posición en el texto.

# El árbol se construye a partir de un archivo de texto, donde cada
# palabra está separada por un espacio.

# El árbol se construye de la siguiente forma:

# - Se crea un nodo raíz, que tiene una lista de hijos vacía.
# - Se lee el archivo de texto, y se agrega cada sufijo al árbol.

# Para agregar un sufijo a un nodo, se busca el primer caracter de
# la palabra en el nodo. Si no existe, se crea un nodo hoja.

# Si el caracter existe, se busca el siguiente caracter en el nodo.
# Si no existe, se crea un nodo hoja.



with open(sys.argv[1],'r') as file:
# with open("caca.txt",'r') as file:
    data = file.read() 

data = data.lower().replace("\n"," ")

class Node:

    def __init__(self,pos=-1,length=0, accum = 0):
        self.children = []
        self.pos = pos
        self.length = length
        self.weight = 0

    def insert(self,key_pos,i,accum = 0):

        # Caso hoja: sumar primer termino a accum
        if self.weight == 0 and self.pos > -1:
            accum += self.pos
        # Caso nodo: sumar segundo termino a accum
        else:
            accum += self.length

        key_l = len(data)-i

        if self.weight > 0 or self.pos == -1:
            node_l = self.length # Largo de la arista donde se llevara a cabo la comparacion
            j = self.pos # Indice desde el cual se llevara a cabo la comparacion (relativo al texto completo)
        else:
            node_l = len(data) - self.pos
            j = self.pos

        counter = 0

        while(key_l > 0 and node_l > 0):
            if data[i] != data[j]:
                break
            i += 1
            j += 1
            key_l -= 1
            node_l -= 1
            counter += 1

        self.weight += 1
        
        # Caso 1: nodo existente mas corto que nodo a insertar -> se debe descender por algun hijo
        if node_l == 0:
            for child in self.children:
                # Insercion en nodo hoja
                if child.weight == 0:
                    if data[child.pos] == data[i]: 
                        child.insert(key_pos,i,accum)
                        return 0
                # Insercion en nodo interno
                else:
                    if data[child.pos] == data[i]:
                        child.insert(key_pos,i,accum)
                        return 0

            # Si no se encontro un hijo con la misma letra, se crea uno nuevo:

            # Caso primera hoja
            if len(self.children) == 0:
                self.children.append(Node(accum+key_pos,key_pos))
            
            # Caso ya hay hojas
            else:
                k = len(self.children)
                self.children.append(Node(accum+key_pos,key_pos)) # Se agrega nueva hoja
                while k > 0:
                    comp = self.children[k-1].pos
                    if data[self.children[k].pos] > data[comp]:
                        break
                    self.children[k-1], self.children[k] = self.children[k], self.children[k-1]
                    k -= 1    
            return 0   

        # Caso 2: letras distintas
        clone = copy.deepcopy(self)
        
        clone.pos += counter
        clone.weight -= 1
        if len(clone.children) > 0:
            clone.length -= counter
        self.length = counter
        self.weight += 1
        self.children = [clone, Node(i,key_pos)]
        if self.children[0].pos >= len(data) or self.children[1].pos >= len(data):
            print((self.children[0].pos,self.children[1].pos))
            return -1
        if data[self.children[0].pos] > data[self.children[1].pos]:
            self.children[0], self.children[1] = self.children[1],self.children[0]
        return 0

    def printLeaves(self):
        if len(self.children)==0:
            print(self.length)
            return
        else:
            for child in self.children:
                child.printLeaves()
    
    def printHeight(self,h):
        if h==0:
            print((self.pos,self.length))
            #print(self.weight)
            return
        else:
            for child in self.children:
                child.printHeight(h-1)


# get_son(nodo, char)
# Node, char -> Node
def get_son(node: "Node", char: str) -> "Node":
    assert len(char) == 1
    for child in node.children:
        if data[child.pos] == char:
            return child
    return None




if __name__ == "__main__":

    # Inicializacion de arbol
    root = Node()
    # Insercion de nodos
    ultimasLetras = ""
    for i in range(len(data)-1):
        try:
            if data[i] not in [" ","\n",".",","]:
                if root.insert(i,i) != 0:
                    break
                ultimasLetras += data[i]
        except Exception as e:
            print(f"[ERROR] {str(e)}, i = {i}")
            print(f"Ultimas letras: {ultimasLetras[-10:-1]}") 
    
    del ultimasLetras
    
    
    # Inicializamos variables
    text = ""
    autocompletado = ""
    camino = [root]
    step = 0
    lastpoll = 0

    # Inicializamos curses
    stdscr = curses.initscr()
    # stdscr.resize(40,100)
    curses.noecho()
    while True:
        # Recogemos el input del usuario
        inp = stdscr.getch()
        lastpoll = inp
        if inp not in [-1, curses.KEY_BACKSPACE]: 
            next = get_son(camino[-1], chr(inp))
            if next:
                camino.append(next)

        if inp == 8 or inp == 83:
            if len(camino) > 1:
                camino.pop()
        if inp == 27:
            quit()
        
        text = ""
        for nodo in camino:
            if nodo.weight == 0:
                text += data[nodo.pos:-1]
            else:
                for i in range(nodo.length):
                    text += data[nodo.pos+i]

        opciones = camino[-1].children
        opciones.sort(key=lambda x: x.weight, reverse=True)        
        sugerencias = []
        autocompletado = ""
        for nodo in opciones:
            if nodo.weight == 0:
                autocompletado += data[nodo.pos:-1]
            else:
                for i in range(nodo.length):
                    print(data[nodo.pos+i])
                    autocompletado += data[nodo.pos+i]
            if autocompletado != "":
                sugerencias.append(autocompletado)
            autocompletado = ""

        # Limpiamos la pantalla
        stdscr.clear()
        # Mostramos el prompt
        # stdscr.addstr(2, 1, f"> LASTPOLL: {lastpoll}")
        rtext = text 

        MAXWIDTH = stdscr.getmaxyx()[1]
        if len(rtext) > MAXWIDTH-1:
            rtext = "..." + rtext[MAXWIDTH-1:]
        stdscr.addstr(2, 0, f"> {rtext}") # dibujo mi palabra actual
        for i in range(min(3,len(sugerencias))):
            sugerencia = sugerencias[i]
            if len(sugerencia) > MAXWIDTH-1:
                sugerencia = sugerencia[:MAXWIDTH-1] + "..."
            stdscr.addstr(3+i, 0, f"? {sugerencia}")
        