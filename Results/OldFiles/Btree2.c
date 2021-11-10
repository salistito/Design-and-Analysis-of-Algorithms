#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX2 256

typedef struct Btree256Node {
  int vals[MAX2];          // Array of values
  int count;              // value counter
  struct Btree256Node *C[MAX2+1];  // Array of node pointers
  struct Btree256Node *parent;    // We keep trace of the parent node
}Btree256Node;


Btree256Node *createBTree256Node(int val) {
  Btree256Node *node;
  node = malloc(sizeof(Btree256Node));
  node->vals[0] = val;
  node->count = 1;
  return node;
}


void BTree256_split(Btree256Node **node, Btree256Node** root){
  Btree256Node* p = *node;
  int median = p->vals[(int) floor(MAX2/2)];
  // We create a new root. 
  Btree256Node* right_son = createBTree256Node(median);

  // We save all the values greater than the median
  // into a its new right node
  int i = floor(MAX2/2);
  int k = 0;
  // For each position (except the last one)
  for (i; i < MAX2 ; i++){
    // If the value stored in i is greater, we move the value and the children
    // to the next position
    if (p->vals[i] > median){
      right_son->vals[k] = p->vals[i];
      p->vals[i] = NULL;
      right_son->C[k] = p->C[i];
      p->C[i] = (Btree256Node*) NULL;
      p->count--;
      right_son->count++;
      k++;

    // If the value is the median, we just erase it
    } else if (p->vals[i] == median){
      p->vals[i] = NULL;
      p->count--;
    }
  }

  // We add the las son into the new right node
  right_son->count--;
  right_son->C[k] = p->C[MAX2];
  p->C[MAX2] = NULL;

  // If we are the main root of the tree
  if (p->parent == NULL){
    Btree256Node* new_root = createBTree256Node(median);
    // We change the root of the tree.
    new_root->C[0] = p;
    new_root->C[1] = right_son;
    p->parent = new_root;
    right_son->parent = new_root;
    *root = new_root;
    return;
  }

  // If we are not the root, we are in the recursive case
  // We have to add the value into the parent
  Btree256Node* father = p->parent;
  Btree256Node* grand = father->parent;
  int j = 0;

  while (j < father->count){
    if (father->vals[j] > median) break;
    j++;
  }

  int aux_j = j;
  // We do the same on the next values of the node.
  while (j <= father->count){
    int e = father->vals[j];
    Btree256Node* aux = father->C[j+1];
    right_son->parent = father;
    father->vals[j] = median;
    father->C[j+1] = right_son;
    right_son = aux;
    median = e;
    j++;
  }

  p->parent = father;
  father->C[aux_j] = p;
  father->count++;
  father->parent = grand;

  // If after adding the value into the father's node, the father
  // is now full, we must plit recursively.
  if (father->count == MAX2){
    BTree256_split(&father, root);
  }

  return;
}


void BTree256Insert(Btree256Node **node, int val, Btree256Node **root) {
  Btree256Node* p = *node;
  // If the root is NULL, we just create it
  if (p == NULL) {
    *node = createBTree256Node(val);
    return;

  // If the root is not null, we proceed
  } else {
    // We must look for the position where we must insert
    int i = 0;
    while(i < p->count){
      // If the value in i is greater than val, and there is a child
      // We insert recursively in that child
      if(p->vals[i] > val && p->C[i] != NULL){
        BTree256Insert(&(p->C[i]), val, root);
        return;

      // If the value i is greater than val, and theres not a child, we found it.
      } else if (p->vals[i] > val && p->C[i] == NULL){
        break;
      }
      i++;
    }

    // If the last value is still smaller, and it theres a children on
    // its right, we go recursively in the last children
    if (p->vals[i] < val && p->C[i] != NULL){
      BTree256Insert(&(p->C[i]), val, root);
      return;
    }
    
    // Now that we have the right position, we move all the values to
    // the right and insert the main val into the node
    while(i <= p->count){
      int e = p->vals[i];
      p->vals[i] = val;
      val = e;
      i++;
    }
    p->count ++;

    // If the node is now full, we split it
    if(p->count == MAX2){
      BTree256_split(&p, root);
    }
  }
}


void BTree256_insert(Btree256Node **node, int val){
  // To avoid loosing trace of the main root, we just give it to the 
  // insert and split functions.
  BTree256Insert(node, val, node);
}


Btree256Node* BTree256_find(Btree256Node **node, int val) {
  Btree256Node *a = *node;
  // If the node is empty, the value doest exist in the node
  if (a == NULL){
    return NULL;
  
  // If the node is not empty,
  } else{
    // We set a counter for the possible possition of the value
    // The first iteration will be with a value of 0
    int i = 0;
    while (i < a->count){
      // If the value stored in i is greater, we search recursively on the child
      if (val < a->vals[i]){
        return BTree256_find(&(a->C[i]), val);
      // if we founded the right value, we just return
      } else if (a->vals[i] == val){
        return a;
      }
      i++;
    }
    // We look in the last child
    return BTree256_find(&(a->C[i]), val);
  }
}

/*
void inorden(Btree256Node **node) {
  Btree256Node *a = *node;
  if (a != NULL) {
    printf("[");
    for (int i = 0; i<a->count ; i++)
      printf("%d ", a->vals[i]);
    printf("]");
    printf("\n");
    int i = 0;
    printf("(");
    while (a->C[i] != NULL){
      inorden(&(a->C[i]));
      i++;
    }
    printf(")");
  }
}
*/
