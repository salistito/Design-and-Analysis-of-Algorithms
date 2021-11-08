// Implementación de un ABB clásico en C
#include <stdio.h>
#include <stdlib.h>

// Nodo de un ABB
typedef struct ABBNode {
    unsigned int value;
    struct ABBNode* left;
    struct ABBNode* right;
} ABBNode;

// Función que inicializa un nuevo ABBNode con el valor dado como parametro y con NULL en los punteros left y right
ABBNode* createABBNode(unsigned int x){
    ABBNode* tABBNode = (ABBNode*)malloc(sizeof(ABBNode));
    tABBNode->value = x;
    tABBNode->left = NULL;
    tABBNode->right = NULL;
    return tABBNode;
}

// Función recursiva que inserta un nodo con el valor dado en el ABB
void ABB_insert(ABBNode** pnode, unsigned int x){
    ABBNode *node = *pnode;
    if (node == NULL) {
        // si el ABB es vacío, crear un nuevo nodo
        *pnode = createABBNode(x);
        return;
    }
    if (x < node->value) {
        // Insertar hacia la izquierda
        ABB_insert(&(node->left), x);
        return;
    }
    else {// (x > node->value)
        // Insertar hacia la derecha
        ABB_insert(&(node->right), x);
        return;
    }
}

// Función recursiva que busca el nodo con el valor dado dentro del ABB
ABBNode* ABB_find(ABBNode** pnode, unsigned int x) {
    ABBNode *node = *pnode;
    if (node == NULL) // si el ABB es vacío, no se encuentra el nodo buscado
        return NULL;
    // si el nodo en que se está tiene exactamente el mismo valor, lo encontramos y retornamos que se encontró
    if (x == node->value)
        return *pnode;

    else if (x < node->value) // búsqueda hacia la izquierda
        return ABB_find(&(node->left), x);

    else if (x > node->value) // búsqueda hacia la derecha
        return ABB_find(&(node->right), x);
}

// Funciones para recorrer el ABB de distintas formas
void ABB_preorder(ABBNode** pnode) {
    ABBNode* root = *pnode;
    if (root != NULL) {
        printf("%d ", root->value);
        ABB_preorder(&(root->left));
        ABB_preorder(&(root->right));
    }
}

void ABB_inorder(ABBNode** pnode) {
    ABBNode* root = *pnode;
    if (root != NULL) {
        ABB_inorder(&(root->left));
        printf("%d ", root->value);
        ABB_inorder(&(root->right));
    }
}
void ABB_postorder(ABBNode** pnode) {
    ABBNode* root = *pnode;
    if (root != NULL) {
        ABB_postorder(&(root->left));
        ABB_postorder(&(root->right));
        printf("%d ", root->value);
    }
}
/*
int main() {
    printf("Generando un ABB...\n");
    ABBNode* root = NULL;
    ABB_insert(&root, 50);
    ABB_insert(&root, 27);
    ABB_insert(&root, 75);
    ABB_insert(&root, 21);
    ABB_insert(&root, 33);
    ABB_insert(&root, 69);
    ABB_insert(&root, 100);
    ABB_insert(&root, 420);

    /* ABB generado con la secuencia
              50
           /     \
          27      75
         /  \    /  \
       21   33  69   100 
                       \
                        420

    printf("Recorriendolo en preorden\n");
    printf("Orden esperado: 50,27,21,33,75,69,100,420\n");
    ABB_preorder(&root);

    printf("\nRecorriendolo en inorden\n");
    printf("Orden esperado: 21,27,33,50,69,75,100,420\n");
    ABB_inorder(&root);

    printf("\nRealizar una busquerda exitosa: Buscando el 420\n");
    ABBNode* tABBNode = ABB_find(&root, 420);
    printf("Valor encontrado:\n");
    ABB_inorder(&tABBNode);

    printf("\nRealizar una busqueda infructuosa: Buscando el 1313\n");
    tABBNode = ABB_find(&root, 1313);
    printf("Valor encontrado:\n");
    ABB_inorder(&tABBNode);
 
    return 0;  
}
*/