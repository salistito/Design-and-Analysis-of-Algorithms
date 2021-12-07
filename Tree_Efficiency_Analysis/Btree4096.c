// Implementación de un Btree
#include <stdio.h>
#include <stdlib.h>
#define B4096 4096

// un nodo de un Btree4096
typedef struct Btree4096Node {
    unsigned int keys[B4096], count;
    struct Btree4096Node *child[B4096 + 1];
    struct Btree4096Node* parent;
} Btree4096Node;

Btree4096Node* createBTree4096Node(unsigned int x){
    Btree4096Node* tnode = (Btree4096Node*)malloc(sizeof(Btree4096Node));
    tnode->keys[0] = x;
    tnode->count = 1;
    return tnode;
}

void BTree4096_split(Btree4096Node** pnode, Btree4096Node** root){
    Btree4096Node* node = *pnode;                                                             
    unsigned int median = node->keys[(node->count)/2];
    //partimos node y creamos el que va a la derecha
    Btree4096Node* new_node = createBTree4096Node(0);
    //muevo todos los mayores a la mediana al nodo nuevo
    int n=0;
    for (int i=(node->count)/2; i<B4096;i++) {
        //printf(" i= %d ", i);
        if(node->keys[i] == median) {
            node->keys[i] = NULL;
            node->count--;
        }else{
            new_node->keys[n] = node->keys[i];
            node->keys[i] = NULL;
            new_node->child[n] = node->child[i];
            node->child[i] = (Btree4096Node*)NULL;
            node->count--;
            new_node->count++;
            n++;
        }
        if(i==B4096-1){
            new_node->count--;
            new_node->child[n]= node->child[i+1];
            node->child[i+1] = NULL;
        }
    }
    //printf("Nodo derecho sin problemas\n");
    int first = 1;
    //si estoy en la raiz tengo que crear un nuevo nodo raiz
    if(node->parent == NULL){
        Btree4096Node* new_root = createBTree4096Node(median);
        new_root->child[0] = node;
        new_root->child[1] = new_node;
        node->parent = new_root;
        new_node->parent = new_root;
        *root = new_root;
    }
    //sino meto las cosas en el padre

    else{
        Btree4096Node* parent = node->parent;
        //printf("Parent count = %d", parent->count );
        //printf("Parent key = %d", parent->keys[0] );
        //Btree4096Node* grandparent = parent->parent;
        for(int i=0; i< parent->count;i++){
            //aqui lo voy a insertar
            if(parent->keys[i] >= median){
                unsigned int val = parent->keys[i];
                Btree4096Node* aux = parent->child[i+1];
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

        if(parent->count == B4096){
            BTree4096_split(&parent, root);
        }
    }
    return;
}



void BTree4096Insert(Btree4096Node **pnode, unsigned int x, Btree4096Node **root) {
  Btree4096Node* node= *pnode;
  if (node== NULL) {
    *pnode = createBTree4096Node(x);
    return;} 
  int n = 0;
  while(n< node->count){
      if(node->keys[n] > x && node->child[n] == NULL){break;}
      else if(node->keys[n] > x && node->child[n] != NULL){
          BTree4096Insert(&(node->child[n]), x, root);
          return;} 
          n++;
  }
  if (node->keys[n] < x && node->child[n] != NULL){
    BTree4096Insert(&(node->child[n]), x, root);
    return;
  }
  while(n <= node->count){
    unsigned int val = node->keys[n];
    node->keys[n] = x;
    x = val;
    n++;
  }
  node->count ++;
  if(node->count == B4096){
     BTree4096_split(&node, root);
  }
}

void BTree4096_insert(Btree4096Node** root, unsigned int x){
    BTree4096Insert(root,x,root);
}


Btree4096Node* BTree4096_find(Btree4096Node** pnode, unsigned int x) {
    Btree4096Node* node = *pnode;
    if (node == NULL){
        return NULL;
    }else{
        for(int i=0; i< node->count;i++ ){
            if(x == node->keys[i]){return node;}
            if(x < node->keys[i]){
                return BTree4096_find(&(node->child[i]),x);
            }
        }
        return BTree4096_find(&(node->child[node->count]), x);
    }
}

void Btree4096_inorder(Btree4096Node** node) {
  Btree4096Node* a = *node;
  if (a != NULL) {
    printf("[");
    for (int i = 0; i<a->count ; i++)
      printf("%d ", a->keys[i]);
    printf("]");
    printf("\n");
    int i = 0;
    printf("(");
    while (a->child[i] != NULL){
      Btree4096_inorder(&(a->child[i]));
      i++;
    }
    printf(")");
  }
}

