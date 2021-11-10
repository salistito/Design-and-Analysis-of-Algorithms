import matplotlib.pyplot as plt
import numpy as np
from numpy import loadtxt

def graficar(abb ,avl ,b16 , b256 , b4096 , splay , titulo, log='no'):
    AleatorioABB = []
    AleatorioAVL = []
    AleatorioBTree16 = []
    AleatorioBTree256 = []
    AleatorioBTree4096 = []
    AleatorioSplayTree = []

    lines = loadtxt(abb, delimiter="\n")
    for line in lines:
        AleatorioABB.append(line)
        
    lines = loadtxt(avl , delimiter="\n")
    for line in lines:
        AleatorioAVL.append(line)
        
    lines = loadtxt(b16 , delimiter="\n")
    for line in lines:
        AleatorioBTree16.append(line)

    lines = loadtxt(b256 , delimiter="\n")
    for line in lines:
        AleatorioBTree256.append(line)
        
    lines = loadtxt(b4096 , delimiter="\n")
    for line in lines:
        AleatorioBTree4096.append(line)
        
    lines = loadtxt(splay , delimiter="\n")
    for line in lines:
        AleatorioSplayTree.append(line)

    x = np.arange(0,1000000,1000)

    plt.clf()
    plt.plot(x,AleatorioABB,linewidth=0.5, label='ABB')
    plt.plot(x,AleatorioAVL,linewidth=0.5, label='AVL')
    plt.plot(x,AleatorioBTree16,linewidth=0.5, label='B Tree 16')
    plt.plot(x,AleatorioBTree256,linewidth=0.5, label='B Tree 256')
    plt.plot(x,AleatorioBTree4096,linewidth=0.5, label='B Tree 4096')
    plt.plot(x,AleatorioSplayTree,linewidth=0.5, label='Splay Tree')

    #plt.legend([ABB,AVL,BTree16,BTree256, SplayTree], ['ABB','AVL','BTree16','BTree256', 'SplayTree'])
    plt.grid()
    plt.xlabel('Numero de operación')
    plt.ylabel('Tiempo promedio')
    plt.title(titulo)
    plt.legend(fontsize=10)
    if log =='si':
        plt.yscale('log')
    plt.show()


abb="Results/expAleatorioABB.txt"
avl ="Results/expAleatorioAVL.txt"
b16 ="Results/expAleatorioBTree16.txt"
b256 ="Results/expAleatorioBTree256.txt"
b4096 ="Results/expAleatorioBTree4096.txt"
splay ="Results/expAleatorioSplayTree.txt"

graficar(abb ,avl ,b16 , b256 , b4096 , splay , 'hola', 'si')
