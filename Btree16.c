// Implementación de un Btree
#include <stdio.h>
#include <stdlib.h>
#define B 4

// un nodo de un Btree
typedef struct NodeBtree16 {
    int keys[B], count;
    struct NodeBtree16 *child[B + 1];
    struct NodeBtree16* parent;
} NodeBtree16;

NodeBtree16* createNodeBTree16(int x){
    NodeBtree16* tnode = (NodeBtree16*)malloc(sizeof(NodeBtree16));
    tnode->keys[0] = x;
    tnode->count = 1;
    return tnode;
}

void splitBTree16(NodeBtree16** pnode,NodeBtree16** root){

    NodeBtree16* node = *pnode;                                                             
    int median = node->keys[(node->count)/2];
    //partimos node y creamos el que va a la derecha
    NodeBtree16* new_node = createNodeBTree16(0);
    //muevo todos los mayores a la mediana al nodo nuevo
    int n=0;
    for (int i=(node->count)/2; i<B;i++) {
        //printf(" i= %d ", i);
        if(node->keys[i] == median) {
            node->keys[i] = NULL;
            node->count--;
        }else{
            new_node->keys[n] = node->keys[i];
            node->keys[i] = NULL;
            new_node->child[n] = node->child[i];
            node->child[i] = (NodeBtree16*)NULL;
            node->count--;
            new_node->count++;
            n++;
        }
        if(i==B-1){
            new_node->count--;
            new_node->child[n]= node->child[i+1];
            node->child[i+1] = NULL;
        }
    }
    printf("Nodo derecho sin problemas\n");
    int first = 1;
    //si estoy en la raiz tengo que crear un nuevo nodo raiz
    if(node->parent == NULL){
        NodeBtree16* new_root = createNodeBTree16(median);
        new_root->child[0] = node;
        new_root->child[1] = new_node;
        node->parent = new_root;
        new_node->parent = new_root;
        *root = new_root;
    }
    //sino meto las cosas en el padre
    
    else{
        
        NodeBtree16* parent = node->parent;
        printf("Parent count = %d", parent->count );
        printf("Parent key = %d", parent->keys[0] );
        //NodeBtree16* grandparent = parent->parent;
        for(int i=0; i< parent->count;i++){
            //aqui lo voy a insertar
            if(parent->keys[i] >= median){
                int val = parent->keys[i];
                NodeBtree16* aux = parent->child[i+1];
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

        if(parent->count == B){
            splitBTree16(&parent, root);
        }
    }
    return;
}

void insertBTree16(NodeBtree16** pnode, int x, NodeBtree16** root){
    printf("VOY A INSERTAR %d \n", x);
    NodeBtree16* node = *pnode;
    if(node == NULL){
        *pnode = createNodeBTree16(x);
        root = *pnode;
        return;}
    //recorremos el arreglo de las llaves
    for(int i = 0; i< node->count; i++){
        if(node->child[0]!=NULL){
            if(node->keys[i] > x){
                insertBTree16(&(node->child[i]), x, root);
                return; } 
            else if (node->keys[i] < x && i==node->count-1){
                insertBTree16(&(node->child[i+1]), x, root);
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
    if(node->count == B){
        printf("VOY A split \n");
        splitBTree16(&node, root);}
    printf("SIN ERROR\n");
    return;
 }


void insert(NodeBtree16** root, int x){
    
    insertBTree16(root,x,root);

}

void inorden(NodeBtree16** node) {
  NodeBtree16* a = *node;
  if (a != NULL) {
    printf("[");
    for (int i = 0; i<a->count ; i++)
      printf("%d ", a->keys[i]);
    printf("]");
    printf("\n");
    int i = 0;
    printf("(");
    while (a->child[i] != NULL){
      inorden(&(a->child[i]));
      i++;
    }
    printf(")");
  }
}

NodeBtree16* find(NodeBtree16** pnode, int x) {
    NodeBtree16* node = *pnode;
    if (node == NULL){
        return NULL;
    }else{
        for(int i=0; i< node->count;i++ ){
            if(x == node->keys[i]){return node;}
            if(x < node->keys[i]){
                return find(&(node->child[i]),x);
            }
        }
        return find(&(node->child[node->count]), x);
    }
}



int main()
{
    struct NodeBtree16 *root;
    root = createNodeBTree16(20);
    inorden(&root);
    printf("\n");
 
    insert(&root, 2);
    inorden(&root);
    printf("\n");

    insert(&root, 3);
    inorden(&root);
    printf("\n");

    insert(&root, 5);
    inorden(&root);
    printf("\n");

    insert(&root, 6);
    inorden(&root);
    printf("\n");

    insert(&root, 1);
    inorden(&root);
    printf("\n");

    insert(&root, 4);
    inorden(&root);
    printf("\n");

    insert(&root, 22);
    inorden(&root);
    printf("\n");

    insert(&root, 25);
    inorden(&root);
    printf("\n");

    insert(&root, 8);
    inorden(&root);
    printf("\n");
    insert(&root, 10);
    inorden(&root);
    printf("\n");

    printf("el 8 está en %d", find(&root,8));


    return 0;
}