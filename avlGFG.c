
// C program to insert a node in AVL tree
#include<stdio.h>
#include<stdlib.h>
 
// An AVL tree node
struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};
 
// A utility function to get maximum of two integers
int max1(int a, int b);
 
// A utility function to get the height of the tree
int height1(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 
// A utility function to get maximum of two integers
int max1(int a, int b)
{
    return (a > b)? a : b;
}
 
/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Node* newNode(int key)
{
    struct Node* node = (struct Node*)
                        malloc(sizeof(struct Node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}
 
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate1(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max1(height1(y->left), height1(y->right))+1;
    x->height = max1(height1(x->left), height1(x->right))+1;
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate1(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max1(height1(x->left), height1(x->right))+1;
    y->height = max1(height1(y->left), height1(y->right))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalance1(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height1(N->left) - height1(N->right);
}
 
// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
struct Node* insert(struct Node* node, int key)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));
 
    if (key < node->key)
        node->left  = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Equal keys are not allowed in BST
        return node;
 
    /* 2. Update height of this ancestor node */
    node->height = 1 + max1(height1(node->left),
                           height1(node->right));
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance1(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate1(node);
 
    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate1(node);
 
    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate1(node->left);
        return rightRotate1(node);
    }
 
    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate1(node->right);
        return leftRotate1(node);
    }
 
    /* return the (unchanged) node pointer */
    return node;
}

// Función recursiva que busca el nodo con el valor dado dentro del AVL
struct Node* AVL_GFG_find(struct Node** pnode, unsigned int x) {
    struct Node* node = *pnode;
    if (node == NULL) // si el AVL es vacío, no se encuentra el nodo buscado
        return NULL;
    // si el nodo en que se está tiene exactamente el mismo valor, lo encontramos y retornamos el nodo
    if (x == node->key)
        return *pnode;

    else if (x < node->key) // búsqueda hacia la izquierda
        return AVL_GFG_find(&(node->left), x);

    else if (x > node->key) // búsqueda hacia la derecha
        return AVL_GFG_find(&(node->right), x);
}
 
// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void preOrder1(struct Node *root)
{
    if(root != NULL)
    {
        printf("%d ", root->key);
        preOrder1(root->left);
        preOrder1(root->right);
    }
}
/*
int main()
{
  struct Node *root = NULL;
 
  root = insert(root, 10);
  root = insert(root, 20);
  root = insert(root, 30);
  root = insert(root, 40);
  root = insert(root, 50);
  root = insert(root, 25);
 
  /* The constructed AVL Tree would be
            30
           /  \
         20   40
        /  \     \
       10  25    50
 
  printf("Preorder traversal of the constructed AVL"
         " tree is \n");
  preOrder(root);
 
  return 0;
}
*/