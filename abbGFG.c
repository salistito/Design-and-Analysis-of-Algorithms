// C program to demonstrate insert
// operation in binary
// search tree.
#include <stdio.h>
#include <stdlib.h>
 
struct nodexd {
    int key;
    struct nodexd *left, *right;
};
 
// A utility function to create a new BST node
struct nodexd* newNodexd(int item)
{
    struct nodexd* temp
        = (struct nodexd*)malloc(sizeof(struct nodexd));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}
 
// A utility function to do inorder traversal of BST
void inorderxd(struct nodexd* root)
{
    if (root != NULL) {
        inorderxd(root->left);
        printf("%d \n", root->key);
        inorderxd(root->right);
    }
}
 
/* A utility function to insert
   a new node with given key in
 * BST */
struct nodexd* insertxd(struct nodexd* node, int key)
{
    /* If the tree is empty, return a new node */
    if (node == NULL)
        return newNodexd(key);
 
    /* Otherwise, recur down the tree */
    if (key < node->key)
        node->left = insertxd(node->left, key);
    else if (key > node->key)
        node->right = insertxd(node->right, key);
 
    /* return the (unchanged) node pointer */
    return node;
}

// C function to search a given key in a given BST
struct nodexd* searchxd(struct nodexd* root, int key)
{
    // Base Cases: root is null or key is present at root
    if (root == NULL || root->key == key)
       return root;
    
    // Key is greater than root's key
    if (root->key < key)
       return searchxd(root->right, key);
 
    // Key is smaller than root's key
    return searchxd(root->left, key);
}
/*
// Driver Code
int main()
{
    struct node* root = NULL;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 20);
    insert(root, 40);
    insert(root, 70);
    insert(root, 60);
    insert(root, 80);
 
    // print inoder traversal of the BST
    inorder(root);
 
    return 0;
}
*/