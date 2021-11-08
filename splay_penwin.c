#include<stdio.h>
#include<stdlib.h>

typedef struct Nodo1
{
    int valor;
    struct Nodo1* izq;
    struct Nodo1* der;
    
}nodo1;

nodo1* crearNodo(int x){
    nodo1* nodoo = (nodo1*) malloc(sizeof(nodo1));
    nodoo->valor = x;
    nodoo->izq=NULL;
    nodoo->der = NULL;
    return nodoo;
}

nodo1* rotarDerecha(nodo1* a){
    nodo1* b;
    b = a->izq;
    a->izq = b->der;
    b->der = a;
    return b;
}

nodo1* rotarIzquierda(nodo1* a){
    nodo1* b;
    b = a->der;
    a-> der = b->izq;
    b->izq = a;
    return b;
}

nodo1* buscar(nodo1* a, int b){
    if(a == NULL){
        return a;
    }
    if(a->valor == b){
        //estamos justo en ese
        return a;
    }

    if(a->valor > b){
        //pa la izquierda
        if (a->izq == NULL){
            return a;
        }

        if (a->izq->valor > b){
            //caso dos veces izq 

            a->izq->izq = buscar(a->izq->izq,b);
            a = rotarDerecha(a);
            if (a->izq != NULL){
                a = rotarDerecha(a);
            }
            return a;
        }

        else if (a->izq->valor< b){
            //caso izq - der

            a->izq->der = buscar(a->izq->der,b);
            if (a->izq->der != NULL){
                a->izq = rotarIzquierda(a->izq);
            }
            a = rotarDerecha(a);
            return a;

        }
        else if (a->izq->valor == b){
            //caso el de la izq es el valor
            a = rotarDerecha(a);
            return a;
        }

    }
    else{
        //pa la derecha
        if (a->der == NULL){
            return a;
        }

        if (a->der->valor > b){
            //caso derecha izquierda 
            a->der->izq = buscar(a->der->izq,b);
            if (a->der->izq != NULL){
                a->der = rotarDerecha(a->der);
            }
            a = rotarIzquierda(a);
            return a;
        }

        else if (a->der->valor< b){
            //caso derecha derecha
            a->der->der = buscar(a->der->der,b);
            a = rotarIzquierda(a);
            if (a->der != NULL){
                a = rotarIzquierda(a);
            }
            return a;
        }
        else if(a->der->valor == b){
            //el segundo es el valor
            a = rotarIzquierda(a);
            return a;
        }

    }


}

nodo1* normalInsert1(nodo1* a, int b){
    if(a==NULL){
        nodo1* x =crearNodo(b);
        a = x;
        return a;
    }

    //ahora, al menos tenemos un elemento xd
    if (a->valor == b){
        return a;
    }

    else if (a->valor > b){
        //pa la izquierda
        a->izq = normalInsert1(a->izq,b);
        return a;
    }
    
    else if (a->valor < b){
        //pa la derecha
        a->der = normalInsert1(a->der,b);
        return a;
    }
}

nodo1* insert(nodo1* a, int b){
    a = normalInsert1(a,b);
    a = buscar(a,b);
    return a;
}

void print1(nodo1* a){
    if (a == NULL){
        return;
    }
    else{
        printf("printing root: %d \n",a->valor);
        if (a->izq != NULL){
            printf("printing izq \n");
        }
        print1(a->izq);
        if (a->der != NULL){
            printf("printing der \n");
        }
        print1(a->der);

        printf("coming out of tree \n");
        return;
    }
}

void splay_preorder1(nodo1** pnode) {
    nodo1* root = *pnode;
    if (root != NULL) {
        printf("%d ", root->valor);
        splay_preorder1(&(root->izq));
        splay_preorder1(&(root->der));
    }
}

void splay_inorder1(nodo1** pnode) {
    nodo1* root = *pnode;
    if (root != NULL) {
        splay_inorder1(&(root->izq));
        printf("%d ", root->valor);
        splay_inorder1(&(root->der));
    }
}
/*
int main()
{
    nodo1* a;

    a = crearNodo(100);
    a->izq = crearNodo(50);
    a->der = crearNodo(200);
    a->izq->izq = crearNodo(40);
    a->izq->izq->izq = crearNodo(30);
    a->izq->izq->izq->izq = crearNodo(20);
    /* splayTree generado con la secuencia
            100
           /   \
         50    200
        / 
       40
      /
     30
    /
   20
    printf("\nRecorriendolo en preorden...\n");
    printf("Orden esperado: 100,50,40,30,20,200\n");
    splay_preorder(&a);

    printf("\n\nRecorriendolo en inorden...\n");
    printf("Orden esperado: 20,30,40,50,100,200\n");
    splay_inorder(&a);

    printf("\n\nRealizar una busqueda exitosa: Buscando el 20...\n");
    a = buscar(a, 20);
    printf("Valor encontrado: %d\n", a->valor);
    /* splayTree después de la búsqueda
            20
              \
              50
             /  \
            30   100
             \     \
             40    200
    printf("Recorriendolo en preorden...\n");
    printf("Orden esperado: 20 50 30 40 100 200\n");
    splay_preorder(&a);

    printf("\n\nRealizar una busqueda infructuosa: Buscando el 69...\n");
    a = buscar(a, 69);
    printf("Valor encontrado: %d\n", a->valor);
    /* splayTree después de la búsqueda
            100
           /   \
         50    200
        / 
       20
        \
        30
         \
         40
    printf("Recorriendolo en preorden...\n");
    printf("Orden esperado: 100 50 20 30 40 200\n");
    splay_preorder(&a);


    printf("\n\nInsertar un elemento\n");
    a = insert(a, 25);
    printf("elemento insertado=25\n");
    /* splayTree después de la inserción
            25     
           /  \
          20  50 
             /  \  
            30  100    
             \    \
             40   200
    printf("Recorriendolo en preorden...\n");
    printf("orden esperado 25,20,50,30,40,100,200 \n");
    splay_preorder(&a);

    return 0;
}
*/