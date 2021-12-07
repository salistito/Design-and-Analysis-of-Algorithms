// Implementación de un Btree
#include <stdio.h>
#include <stdlib.h>
#define B256 256

// un nodo de un Btree256
typedef struct Btree256Node {
    unsigned int keys[B256], count;
    struct Btree256Node *child[B256 + 1];
    struct Btree256Node* parent;
} Btree256Node;

Btree256Node* createBTree256Node(unsigned int x){
    Btree256Node* tnode = (Btree256Node*)malloc(sizeof(Btree256Node));
    tnode->keys[0] = x;
    tnode->count = 1;
    return tnode;
}

void BTree256_split(Btree256Node** pnode, Btree256Node** root){
    Btree256Node* node = *pnode;                                                             
    unsigned int median = node->keys[(node->count)/2];
    //partimos node y creamos el que va a la derecha
    Btree256Node* new_node = createBTree256Node(0);
    //muevo todos los mayores a la mediana al nodo nuevo
    int n=0;
    for (int i=(node->count)/2; i<B256;i++) {
        //printf(" i= %d ", i);
        if(node->keys[i] == median) {
            node->keys[i] = NULL;
            node->count--;
        }else{
            new_node->keys[n] = node->keys[i];
            node->keys[i] = NULL;
            new_node->child[n] = node->child[i];
            node->child[i] = (Btree256Node*)NULL;
            node->count--;
            new_node->count++;
            n++;
        }
        if(i==B256-1){
            new_node->count--;
            new_node->child[n]= node->child[i+1];
            node->child[i+1] = NULL;
        }
    }
    //printf("Nodo derecho sin problemas\n");
    int first = 1;
    //si estoy en la raiz tengo que crear un nuevo nodo raiz
    if(node->parent == NULL){
        Btree256Node* new_root = createBTree256Node(median);
        new_root->child[0] = node;
        new_root->child[1] = new_node;
        node->parent = new_root;
        new_node->parent = new_root;
        *root = new_root;
    }
    //sino meto las cosas en el padre

    else{
        Btree256Node* parent = node->parent;
        //printf("Parent count = %d", parent->count );
        //printf("Parent key = %d", parent->keys[0] );
        //Btree256Node* grandparent = parent->parent;
        for(int i=0; i< parent->count;i++){
            //aqui lo voy a insertar
            if(parent->keys[i] >= median){
                unsigned int val = parent->keys[i];
                Btree256Node* aux = parent->child[i+1];
                new_node->parent = parent;
                parent->keys[i] = median;
                parent->child[i+1] = new_node;
                new_node=aux;
                median=val;
                if (first){
                    parent->child[i] = node;
                    first=0;
                }
            }
        }
        new_node->parent = parent;
        parent->keys[parent->count] = median;
        parent->child[parent->count+1] = new_node;
        parent->count++;

        if(parent->count == B256){
            BTree256_split(&parent, root);
        }
    }
    return;
}



void BTree256Insert(Btree256Node **pnode, unsigned int x, Btree256Node **root) {
  Btree256Node* node= *pnode;
  if (node== NULL) {
    *pnode = createBTree256Node(x);
    return;} 
  int n = 0;
  while(n< node->count){
      if(node->keys[n] > x && node->child[n] == NULL){break;}
      else if(node->keys[n] > x && node->child[n] != NULL){
          BTree256Insert(&(node->child[n]), x, root);
          return;} 
          n++;
  }
  if (node->keys[n] < x && node->child[n] != NULL){
    BTree256Insert(&(node->child[n]), x, root);
    return;
  }
  while(n <= node->count){
    unsigned int val = node->keys[n];
    node->keys[n] = x;
    x = val;
    n++;
  }
  node->count ++;
  if(node->count == B256){
     BTree256_split(&node, root);
  }
}

void BTree256_insert(Btree256Node** root, unsigned int x){
    BTree256Insert(root,x,root);
}


Btree256Node* BTree256_find(Btree256Node** pnode, unsigned int x) {
    Btree256Node* node = *pnode;
    if (node == NULL){
        return NULL;
    }else{
        for(int i=0; i< node->count;i++ ){
            if(x == node->keys[i]){return node;}
            if(x < node->keys[i]){
                return BTree256_find(&(node->child[i]),x);
            }
        }
        return BTree256_find(&(node->child[node->count]), x);
    }
}

void Btree256_inorder(Btree256Node** node) {
  Btree256Node* a = *node;
  if (a != NULL) {
    printf("[");
    for (int i = 0; i<a->count ; i++)
      printf("%d ", a->keys[i]);
    printf("]");
    printf("\n");
    int i = 0;
    printf("(");
    while (a->child[i] != NULL){
      Btree256_inorder(&(a->child[i]));
      i++;
    }
    printf(")");
  }
}

