// Implementación de un Btree
#include <stdio.h>
#include <stdlib.h>
#define B16 16

// un nodo de un Btree16
typedef struct Btree16Node {
    int keys[B16], count;
    struct Btree16Node *child[B16 + 1];
    struct Btree16Node* parent;
} Btree16Node;

Btree16Node* createBTree16Node(int x){
    Btree16Node* tnode = (Btree16Node*)malloc(sizeof(Btree16Node));
    tnode->keys[0] = x;
    tnode->count = 1;
    return tnode;
}

void BTree16_split(Btree16Node** pnode, Btree16Node** root){
    Btree16Node* node = *pnode;                                                             
    int median = node->keys[(node->count)/2];
    //partimos node y creamos el que va a la derecha
    Btree16Node* new_node = createBTree16Node(0);
    //muevo todos los mayores a la mediana al nodo nuevo
    int n=0;
    for (int i=(node->count)/2; i<B16;i++) {
        //printf(" i= %d ", i);
        if(node->keys[i] == median) {
            node->keys[i] = NULL;
            node->count--;
        }else{
            new_node->keys[n] = node->keys[i];
            node->keys[i] = NULL;
            new_node->child[n] = node->child[i];
            node->child[i] = (Btree16Node*)NULL;
            node->count--;
            new_node->count++;
            n++;
        }
        if(i==B16-1){
            new_node->count--;
            new_node->child[n]= node->child[i+1];
            node->child[i+1] = NULL;
        }
    }
    printf("Nodo derecho sin problemas\n");
    int first = 1;
    printf("%d\n",node->parent);
    //si estoy en la raiz tengo que crear un nuevo nodo raiz
    if(node->parent == NULL){
        printf("Entro al if\n");
        Btree16Node* new_root = createBTree16Node(median);
        new_root->child[0] = node;
        new_root->child[1] = new_node;
        node->parent = new_root;
        new_node->parent = new_root;
        *root = new_root;
    }
    //sino meto las cosas en el padre
    else{
        printf("Entro al else\n");
        Btree16Node* parent = node->parent;
        printf("ya se preguntó por el parent");
        //printf("Parent count = %d", parent->count);
        //printf("Parent key = %d", parent->keys[0] );
        //Btree16Node* grandparent = parent->parent;
        for(int i=0; i< parent->count;i++){
            //aqui lo voy a insertar
            if(parent->keys[i] >= median){
                int val = parent->keys[i];
                Btree16Node* aux = parent->child[i+1];
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

        if(parent->count == B16){
            BTree16_split(&parent, root);
        }
    }
    return;
}

void BTree16Insert(Btree16Node** pnode, int x, Btree16Node** root){
    printf("VOY A INSERTAR %d \n", x);
    Btree16Node* node = *pnode;
    if(node == NULL){
        *pnode = createBTree16Node(x);
        root = *pnode;
        return;}
    //recorremos el arreglo de las llaves
    for(int i = 0; i< node->count; i++){
        if(node->child[0]!=NULL){
            if(node->keys[i] > x){
                BTree16Insert(&(node->child[i]), x, root);
                return; } 
            else if (node->keys[i] < x && i==node->count-1){
                BTree16Insert(&(node->child[i+1]), x, root);
                return;  }
        }
        //es hoja
        else{
            if(node->keys[i] > x){
                int val = node->keys[i];
                node->keys[i] = x;
                x = val;}}
    }
    node->keys[node->count] = x; 
    node->count++;
    if(node->count == B16){
        printf("VOY A split \n");
        BTree16_split(&node, root);}
    printf("SIN ERROR\n");
    return;
 }


void BTree16_insert(Btree16Node** root, int x){
    BTree16Insert(root,x,root);
}

void Btree16_inorder(Btree16Node** node) {
  Btree16Node* a = *node;
  if (a != NULL) {
    //printf("[");
    for (int i = 0; i<a->count ; i++)
      printf("%d ", a->keys[i]);
    //printf("]");
    //printf("\n");
    int i = 0;
    //printf("(");
    while (a->child[i] != NULL){
      Btree16_inorder(&(a->child[i]));
      i++;
    }
    //printf(")");
  }
}

Btree16Node* BTree16_find(Btree16Node** pnode, int x) {
    Btree16Node* node = *pnode;
    if (node == NULL){
        return NULL;
    }else{
        for(int i=0; i< node->count;i++ ){
            if(x == node->keys[i]){return node;}
            if(x < node->keys[i]){
                return BTree16_find(&(node->child[i]),x);
            }
        }
        return BTree16_find(&(node->child[node->count]), x);
    }
}
/*
int main()
{
    struct Btree16Node *root;
    root = createBTree16Node(20);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 2);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 3);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 5);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 6);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 1);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 4);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 22);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 25);
    Btree16_inorder(&root);
    printf("\n");

    BTree16_insert(&root, 8);
    Btree16_inorder(&root);
    printf("\n");
    BTree16_insert(&root, 10);
    Btree16_inorder(&root);
    printf("\n");

    printf("el 8 está en %d", BTree16_find(&root,8));


    return 0;
}*/