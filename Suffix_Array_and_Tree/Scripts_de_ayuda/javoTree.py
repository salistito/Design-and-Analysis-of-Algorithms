class PatriciaNode():
    def __init__(self):
        self.word = ""
        self.children = {}
        self.count_sons = 0
        self.most_popular = None

    def pre_count_sons(self):
        if self.children=={}:
            self.count_sons=0
            return 1
        count=0
        for k in self.children.keys():
            sum = self.children[k].pre_count_sons()
            if sum == None:
                sum = 0
            count+=sum
        self.count_sons=count
        return count

    def pre_most_popular(self):
        if self.children=={}:
            self.most_popular = self
            return
        most = None
        for k in self.children.keys():
            self.children[k].pre_most_popular()
            popular = self.children[k]
            if most == None or popular.count_sons > most.count_sons:
                most = popular
        self.most_popular = most
        return

    def autocompletar(self, char):
        return

    def __str__(self, level=0, branch=""):
        ret = "\t"*level+ branch + self.word + "(" + str(self.count_sons) + ")" + "\n"
        for child in self.children.keys():
            ret += self.children[child].__str__(level+1, child)
        return ret

    def __repr__(self):
        return '<>'

class Patricia():
    def __init__(self):
        self.root = PatriciaNode()

    def insert(self, word):
        children = self.root.children
        i = 0
        
        while 1:
            try:
                char= word[i:i+1]
                node = children[char] 
            except KeyError: # cuando no existe llave
                if char == '':
                        return
                else: 
                    new_node = PatriciaNode()
                    new_node.word = word[i+1:]
                    children[char] = new_node
                return

            # existe la llave
            i += 1
            if word.startswith(node.word,i): #nab a
                if len(word[i:]) == len(node.word):
                    return
                else:
                    i += len(node.word)
                    children = node.children
            else:
                ii = i
                j = 0
                while ii != len(word) and j != len(node.word) and \
                      word[ii:ii+1] == node.word[j:j+1]:
                    ii += 1
                    j += 1

                tmpdata = {}
                new_node = PatriciaNode()
                new_node.word=node.word[j+1:]
                new_node.children=node.children
                tmpdata[node.word[j:j+1]] = new_node

                if word[ii:ii+1]!="":
                    new_node = PatriciaNode()
                    new_node.word=word[ii+1:]
                    tmpdata[word[ii:ii+1]] = new_node

                new_node = PatriciaNode()
                new_node.word = node.word[:j]
                new_node.children = tmpdata
                children[word[i-1:i]] = new_node
                return
            
    def pre_count_sons(self):
        self.root.pre_count_sons()

    def pre_most_popular(self):
        self.root.pre_most_popular()

    def search(self, word):
        children = self.root.children
        i = 0
        wordlen = len(word)
        while 1:
            try:
                char= word[i:i+1]
                node = children[char] 
            except KeyError:
                return False
            i += 1
            if word.startswith(node.word,i):
                if len(word[i:]) == len(node.word):
                    return
                else:
                    i += len(node.word)
                    children = node.children
            else:
                return False
    
    def getThreeMost(self,children):

        hijos=[]
        values=[]
        for k in children.keys():
            if k != "$":
                values.append(children[k].count_sons)
                hijos.append(k)
        hijos.sort()
        if len(hijos)>3:
            return hijos[0:3]
        else:
            while len(hijos)<2:
                hijos.append("")
            return hijos


            

    def autocompletar(self,text):
        children = self.root.children
        i = 0
        while 1:
            try:
                char= text[i:i+1] #la letra i
                node = children[char] 

            except KeyError:
                print(">No tengo sugerencia")
                return ["","",""] # no tengo na que sugerir
            i += 1
            if text.startswith(node.word,i): # aca puede ser igual o mas grande
                if len(text[i:]) == len(node.word): #nana.(nana)
                    print(">Tenemos las siguientes 3 sugerencias")
                    a=self.getThreeMost(node.children)
                    return a
                else:
                    #print("debo seguir buscando") #nana.(na)
                    i += len(node.word)
                    children = node.children
            else:
                
                ii = i
                j = 0
                while ii != len(text) and j != len(node.word) and \
                      text[ii:ii+1] == node.word[j:j+1]:
                    ii += 1
                    j += 1
                value = node.word[j:j+1]
                if value == "$":
                    print(">No tengo sugerencia")
                    return ["","",""]
                print(">Te sugiero seguir con la siguiente letra")
                return [value,"",""]
                
    def __str__(self, level=0):
        ret = "\t"*level+ "_(" + str(self.root.count_sons) + ")" + "\n"
        for child in self.root.children.keys():
            ret += self.root.children[child].__str__(level+1, child)
        return ret

    def __repr__(self):
        return '<>'


## Texto de ejemplo
p = Patricia()
words = ['bar$','baz$', 'foo$', 'football$','footballer$', 'footsal$', 'footaa$', 'footballes$']
for x in words:
    p.insert(x)

#p.pre_count_sons()
#p.pre_most_popular()
#print(p.root.most_popular)
print(p)
print(p.search('f'))

print("--------------------")
print(p.autocompletar("footb"))