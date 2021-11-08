// Implementación de un splayTree en C
#include<stdio.h>
#include<stdlib.h>
 
// Nodo de un splayTree
typedef struct splayNode {
    int value;
    struct splayNode* left;
    struct splayNode* right;
} splayNode;
 
// Función que inicializa un nuevo splayNode con el valor dado como parametro y con NULL en los punteros left y right
splayNode* createSplayNode(int x){
    splayNode* tsplayNode = (splayNode*)malloc(sizeof(splayNode));
    tsplayNode->value = x;
    tsplayNode->left = NULL;
    tsplayNode->right = NULL;
    return tsplayNode;
}
 
// Función auxiliar para rotar hacia la derecha un subárbol
void rightRotation(splayNode** pnode) {
    splayNode* q = *pnode;
    splayNode* p = q->left;
    splayNode* p_right = p->right;

    // hacer la rotación
    p->right = q;
    q->left = p_right;

    // Return new root
    *pnode = p;
}
 
// Función auxiliar para rotar hacia la izquierda un subárbol
void leftRotation(splayNode** pnode) {
    splayNode* p = *pnode;
    splayNode* q = p->right;
    splayNode* q_left = q->left;
 
    // Perform rotation
    q->left = p;
    p->right = q_left;

    // Return new root
    *pnode = q;
}
 
// Función recursiva que busca el nodo con el valor dado dentro del splayTree
// si el valor está presente es movido hacia la raíz
splayNode* splay_find(splayNode** pnode, unsigned int x) {
    splayNode* node = *pnode;
    if (node == NULL) { // si el splayTree es vacío, no se encuentra el nodo buscado
        return NULL;
    }
    // si el nodo en que se está tiene exactamente el mismo valor, lo encontramos y retornamos el nodo
    if (x == node->value)
        return node;

    else if (x < node->value) { // búsqueda hacia la izquierda
        if (node->left == NULL) { // no existe el elemento, devolver el más cercano
             return node;
        }

        if (x < node->left->value) { // caso 2 veces izquierda (Zig-Zig) -> 2 rotaciones a la derecha
            node->left->left = splay_find(&(node->left->left), x); // búsqueda recursiva (subir elemento)
            // equilibrar splay tree
            rightRotation(&node);
            if (node->left != NULL) {
                rightRotation(&node);
            }
            return node;
        }

        else if (x > node->left->value) { // caso izquierda-derecha (Zig-Zag) -> rotación a la izquierda y rotación a la derecha
            node->left->right = splay_find(&(node->left->right), x);
            if (node->left->right != NULL) {
                leftRotation(&(node->left));
            }
            rightRotation(&node);
            return node;

        }
        else if (x == node->left->value) { // nodo de la izquierda es exactamente el valor buscado
            rightRotation(&node);
            return node;
        }

    }
    else {// (x > node->value) -> búsqueda hacia la derecha
        if (node->right == NULL) { // no existe el elemento, devolver el más cercano
            return node;
        }

        if (x < node->right->value) { // caso derecha-izquierda (Zag-Zig) -> rotación a la derecha y rotación a la izquierda
            node->right->left = splay_find(&(node->right->left), x);
            if (node->right->left != NULL){
                rightRotation(&(node->right));
            }
            leftRotation(&node);
            return node;
        }

        else if (x > node->right->value) { // caso 2 veces derecha (Zag-Zag) -> 2 rotaciones a la izquierda
            node->right->right = splay_find(&(node->right->right), x); // búsquerda recursiva (subir elemento)
            // equilibrar splay tree
            leftRotation(&node);
            if (node->right != NULL) {
                leftRotation(&node);
            }
            return node;
        }
        else if (x == node->right->value) { // nodo de la derecha es exactamente el valor buscado
            leftRotation(&node);
            return node;
        }

    }
}

// Función recursiva que inserta un nodo con el valor dado en el splayTree
void normalInsert(splayNode** pnode, unsigned int x){
    splayNode *node = *pnode;
    if(node==NULL){
        *pnode = createSplayNode(x);
        return;
    }

    if (x == node->value) { // ya está insertado un nodo con este valor
        return;
    }

    else if (x < node->value) { // inserción para la izquierda
        normalInsert(&(node->left), x);
        return;
    }
    
    else if (x > node->value){ // inserción para la derecha
        normalInsert(&(node->right), x);
        return;
    }
}

void splay_insert(splayNode** pnode, unsigned int x){
    normalInsert(pnode, x);
    *pnode = splay_find(pnode, x);
    return;
}
 
// Funciones para recorrer el splayTree de distintas formas
void splay_preorder(splayNode** pnode) {
    splayNode* root = *pnode;
    if (root != NULL) {
        printf("%d ", root->value);
        splay_preorder(&(root->left));
        splay_preorder(&(root->right));
    }
}
void splay_inorder(splayNode** pnode) {
    splayNode* root = *pnode;
    if (root != NULL) {
        splay_inorder(&(root->left));
        printf("%d ", root->value);
        splay_inorder(&(root->right));
    }
}
void splay_postorder(splayNode** pnode) {
    splayNode* root = *pnode;
    if (root != NULL) {
        splay_postorder(&(root->left));
        splay_postorder(&(root->right));
        printf("%d ", root->value);
    }
}
/*
int main() {
    printf("Generando un splayTree...\n");
    splayNode* root = NULL;
    splay_insert(&root, 20);
    splay_insert(&root, 30);
    splay_insert(&root, 40);
    splay_insert(&root, 50);
    splay_insert(&root, 200);
    splay_insert(&root, 100);
       splayTree generado con la secuencia
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
    splay_preorder(&root);

    printf("\n\nRecorriendolo en inorden...\n");
    printf("Orden esperado: 20,30,40,50,100,200\n");
    splay_inorder(&root);

    printf("\n\nRealizar una busqueda exitosa: Buscando el 20...\n");
    root = splay_find(&root, 20);
    printf("Valor encontrado: %d\n", root->value);
       splayTree después de la búsqueda
            20
              \
              50
             /  \
            30   100
             \     \
             40    200
    
    printf("Recorriendolo en preorden...\n");
    printf("Orden esperado: 20 50 30 40 100 200\n");
    splay_preorder(&root);

    printf("\n\nRealizar una busqueda infructuosa: Buscando el 69...\n");
    root = splay_find(&root, 69);
    printf("Valor encontrado: %d\n", root->value);
       splayTree después de la búsqueda
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
    splay_preorder(&root);


    printf("\n\nInsertar un elemento\n");
    splay_insert(&root, 25);
    printf("elemento insertado=25\n");
       splayTree después de la inserción
            25     
           /  \
          20  50 
             /  \  
            30  100    
             \    \
             40   200
   
    printf("Recorriendolo en preorden...\n");
    printf("orden esperado 25,20,50,30,40,100,200 \n");
    splay_preorder(&root);

    return 0;
}
*/