// Implementación de un AVL en C
#include<stdio.h>
#include<stdlib.h>
 
// Nodo de un AVL
typedef struct AVLNode {
    unsigned int value;
    struct AVLNode* left;
    struct AVLNode* right;
    unsigned int height;
} AVLNode;
 
 
// A utility function to get the height of the tree
unsigned int height(AVLNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}
 
// A utility function to get maximum of two integers
unsigned int max(unsigned int a, unsigned int b) {
    return (a > b)? a : b;
}
 
// Helper function that allocates a new AVLNode with the given value and NULL left and right pointers.
AVLNode* createAVLNode(unsigned int x) {
    AVLNode* tAVLNode = (AVLNode*)malloc(sizeof(AVLNode));
    tAVLNode->value = x;
    tAVLNode->left = NULL;
    tAVLNode->right = NULL;
    tAVLNode->height = 1;  // new AVLNode is initially added at leaf
    return tAVLNode;
}
 
// A utility function to right rotate subtree rooted with q
void rightRotate(AVLNode** pnode) {
    AVLNode* q = *pnode;
    AVLNode* p = q->left;
    AVLNode* p_right = p->right;
 
    // Perform rotation
    p->right = q;
    q->left = p_right;
 
    // Update heights
    q->height = max(height(q->left), height(q->right))+1;
    p->height = max(height(p->left), height(p->right))+1;
 
    // Return new root
    *pnode = p;
}
 
// A utility function to left rotate subtree rooted with p
void leftRotate(AVLNode** pnode) {
    AVLNode* p = *pnode;
    AVLNode* q = p->right;
    AVLNode* q_left = q->left;
 
    // Perform rotation
    q->left = p;
    p->right = q_left;
 
    //  Update heights
    p->height = max(height(p->left), height(p->right))+1;
    q->height = max(height(q->left), height(q->right))+1;
 
    // Return new root
    *pnode = q;
}
 
// Get Balance factor of AVLNode N
int getBalance(AVLNode* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Recursive function to insert a value in the subtree rooted with node and returns the new root of the subtree.
void AVL_insert(AVLNode** pnode, unsigned int x) {
    AVLNode *node = *pnode;
    // 1.  Perform the normal BST insertion
    if (node == NULL){
        // si el AVL es vacío, crear un nuevo nodo
        *pnode = createAVLNode(x);
        return;
    }
    if (x < node->value) // Insertar hacia la izquierda
        AVL_insert(&(node->left), x);
    else if (x > node->value) // Insertar hacia la derecha
        AVL_insert(&(node->right), x);
    else // Equal keys are not allowed in BST
        return;

    // 2. Update height of this ancestor AVLNode
    node->height = 1 + max(height(node->left), height(node->right));
    
    // 3. Get the balance factor of this ancestor AVLNode to check whether this AVLNode became unbalanced
    int balance = getBalance(node);

    // If this AVLNode becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    if (balance > 1 && x < node->left->value){
        rightRotate(pnode);
        return;
    }
    // Right Right Case
    if (balance < -1 && x > node->right->value){
        leftRotate(pnode);
        return;
    }
    // Left Right Case
    if (balance > 1 && x > node->left->value){
        leftRotate(&(node->left));
        rightRotate(pnode);
        return;
    }
    // Right Left Case
    if (balance < -1 && x < node->right->value){
        rightRotate(&(node->right));
        leftRotate(pnode);
        return;
    }
    return;
}

// Función recursiva que busca el nodo con el valor dado dentro del AVL
AVLNode* AVL_find(AVLNode** pnode, unsigned int x) {
    AVLNode *node = *pnode;
    if (node == NULL) // si el AVL es vacío, no se encuentra el nodo buscado
        return NULL;
    // si el nodo en que se está tiene exactamente el mismo valor, lo encontramos y retornamos el nodo
    if (x == node->value)
        return *pnode;

    else if (x < node->value) // búsqueda hacia la izquierda
        return AVL_find(&(node->left), x);

    else if (x > node->value) // búsqueda hacia la derecha
        return AVL_find(&(node->right), x);
}

// Funciones para recorrer el AVL de distintas formas
void AVL_preorder(AVLNode** pnode) {
    AVLNode* root = *pnode;
    if (root != NULL) {
        printf("%d ", root->value);
        AVL_preorder(&(root->left));
        AVL_preorder(&(root->right));
    }
}
void AVL_inorder(AVLNode** pnode) {
    AVLNode* root = *pnode;
    if (root != NULL) {
        AVL_inorder(&(root->left));
        printf("%d ", root->value);
        AVL_inorder(&(root->right));
    }
}
void AVL_postorder(AVLNode** pnode) {
    AVLNode* root = *pnode;
    if (root != NULL) {
        AVL_postorder(&(root->left));
        AVL_postorder(&(root->right));
        printf("%d ", root->value);
    }
}

/*
int main() {
    printf("Generando un AVL...\n");
    AVLNode* root = NULL;
    AVL_insert(&root, 10);
    AVL_insert(&root, 20);
    AVL_insert(&root, 30);
    AVL_insert(&root, 40);
    AVL_insert(&root, 50);
    AVL_insert(&root, 25);

    /* AVL generado con la secuencia
            30
           /  \
         20   40
        /  \     \
       10  25    50

    printf("Recorriendolo en preorden\n");
    printf("Orden esperado: 30,20,10,25,40,50\n");
    AVL_preorder(&root);

    printf("\nRecorriendolo en inorden\n");
    printf("Orden esperado: 10,20,25,30,40,50\n");
    AVL_inorder(&root);
    
    printf("\nRealizar una busqueda exitosa: Buscando el 50\n");
    AVLNode* tAVLNode = AVL_find(&root, 50);
    printf("Valor encontrado:\n");
    AVL_preorder(&tAVLNode);

    printf("\nRealizar una busqueda infructuosa: Buscando el 69\n");
    tAVLNode = AVL_find(&root, 69);
    printf("Valor encontrado:\n");
    AVL_preorder(&tAVLNode);

  return 0;
}
*/