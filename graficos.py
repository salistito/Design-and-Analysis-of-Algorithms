import matplotlib.pyplot as plt
import numpy as np
from numpy import loadtxt

def graficar(abb, avl, b16, b256, b4096, splay, titulo, log='no'):
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

    x = np.arange(0,1000,1)

    plt.clf()
    plt.plot(x,AleatorioABB,linewidth=0.5, label='ABB')
    plt.plot(x,AleatorioAVL,linewidth=0.5, label='AVL')
    plt.plot(x,AleatorioBTree16,linewidth=0.5, label='BTree 16')
    plt.plot(x,AleatorioBTree256,linewidth=0.5, label='BTree 256')
    plt.plot(x,AleatorioBTree4096,linewidth=0.5, label='BTree 4096')
    plt.plot(x,AleatorioSplayTree,linewidth=0.5, label='Splay Tree')

    #plt.legend([ABB,AVL,BTree16,BTree256, SplayTree], ['ABB','AVL','BTree16','BTree256', 'SplayTree'])
    plt.grid()
    plt.xlabel('Intervalos de 1000 operaciones')
    plt.ylabel('Tiempo[s]')
    plt.title(titulo)
    plt.legend(fontsize=10)
    if log =='si':
        plt.yscale('log')
    plt.show()


expAleatorioABB = "Results/expAleatorioABB.txt"
expAleatorioAVL = "Results/expAleatorioAVL.txt"
expAleatorioBTree16 = "Results/expAleatorioBTree16.txt"
expAleatorioBTree256 = "Results/expAleatorioBTree256.txt"
expAleatorioBTree4096 = "Results/expAleatorioBTree4096.txt"
expAleatorioSplayTree = "Results/expAleatorioSplayTree.txt"

expCreciente01ABB = "Results/expCreciente01ABB.txt"
expCreciente01AVL = "Results/expCreciente01AVL.txt"
expCreciente01BTree16 = "Results/expCreciente01BTree16.txt"
expCreciente01BTree256 = "Results/expCreciente01BTree256.txt"
expCreciente01BTree4096 = "Results/expCreciente01BTree4096.txt"
expCreciente01SplayTree = "Results/expCreciente01SplayTree.txt"

expCreciente05ABB = "Results/expCreciente05ABB.txt"
expCreciente05AVL = "Results/expCreciente05AVL.txt"
expCreciente05BTree16 = "Results/expCreciente05BTree16.txt"
expCreciente05BTree256 = "Results/expCreciente05BTree256.txt"
expCreciente05BTree4096 = "Results/expCreciente05BTree4096.txt"
expCreciente05SplayTree = "Results/expCreciente05SplayTree.txt"

expSesgado_x_ABB = "Results/expSesgado_x_ABB.txt"
expSesgado_x_AVL = "Results/expSesgado_x_AVL.txt"
expSesgado_x_BTree16 = "Results/expSesgado_x_BTree16.txt"
expSesgado_x_BTree256 = "Results/expSesgado_x_BTree256.txt"
expSesgado_x_BTree4096 = "Results/expSesgado_x_BTree4096.txt"
expSesgado_x_SplayTree = "Results/expSesgado_x_SplayTree.txt"

expSesgado_sqrt_ABB = "Results/expSesgado_sqrt_ABB.txt"
expSesgado_sqrt_AVL = "Results/expSesgado_sqrt_AVL.txt"
expSesgado_sqrt_BTree16 = "Results/expSesgado_sqrt_BTree16.txt"
expSesgado_sqrt_BTree256 = "Results/expSesgado_sqrt_BTree256.txt"
expSesgado_sqrt_BTree4096 = "Results/expSesgado_sqrt_BTree4096.txt"
expSesgado_sqrt_SplayTree = "Results/expSesgado_sqrt_SplayTree.txt"

expSesgado_ln_ABB = "Results/expSesgado_ln_ABB.txt"
expSesgado_ln_AVL = "Results/expSesgado_ln_AVL.txt"
expSesgado_ln_BTree16 = "Results/expSesgado_ln_BTree16.txt"
expSesgado_ln_BTree256 = "Results/expSesgado_ln_BTree256.txt"
expSesgado_ln_BTree4096 = "Results/expSesgado_ln_BTree4096.txt"
expSesgado_ln_SplayTree = "Results/expSesgado_ln_SplayTree.txt"

graficar(expAleatorioABB, expAleatorioAVL, expAleatorioBTree16,
         expAleatorioBTree256, expAleatorioBTree4096, expAleatorioSplayTree , 'Experimento Aleatorio')

graficar(expCreciente01ABB, expCreciente01AVL, expCreciente01BTree16,
         expCreciente01BTree256, expCreciente01BTree4096, expCreciente01SplayTree , 'Experimento Creciente con factor=0.1')

graficar(expCreciente05ABB, expCreciente05AVL, expCreciente05BTree16,
         expCreciente05BTree256, expCreciente05BTree4096, expCreciente05SplayTree , 'Experimento Creciente con factor=0.5')

graficar(expSesgado_x_ABB, expSesgado_x_AVL, expSesgado_x_BTree16,
         expSesgado_x_BTree256, expSesgado_x_BTree4096, expSesgado_x_SplayTree , 'Experimento Sesgado con p(x)=x')

graficar(expSesgado_sqrt_ABB, expSesgado_sqrt_AVL, expSesgado_sqrt_BTree16,
         expSesgado_sqrt_BTree256, expSesgado_sqrt_BTree4096, expSesgado_sqrt_SplayTree , 'Experimento Sesgado con p(x)=sqrt(x)')

graficar(expSesgado_ln_ABB, expSesgado_ln_AVL, expSesgado_ln_BTree16,
         expSesgado_ln_BTree256, expSesgado_ln_BTree4096, expSesgado_ln_SplayTree , 'Experimento Sesgado con p(x)=ln(x)')
