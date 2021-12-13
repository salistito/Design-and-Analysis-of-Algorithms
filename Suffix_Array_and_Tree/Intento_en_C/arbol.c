#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int cmp(const void *a, const void *b) {
   const char *ia = *(const char **) a;
   const char *ib = *(const char **) b;
   return strcmp(ia, ib);
} 


char * * create_array(char* text){
    int size_of_array = strlen (text);

    char ** arreglo = malloc(size_of_array * sizeof(char *));

    for(int i=0; i <= size_of_array; i++){
        arreglo[i] = text+i;
    }

    qsort(arreglo, size_of_array, sizeof(char *), cmp);

    //for (int i = 0; i < size_of_array; i++) {
    //  for (int j = i+1; j < size_of_array; j++)
    //     if (strcmp(arreglo[i], arreglo[j]) > 0) {
    //        char* temp = arreglo[i]; 
    //        arreglo[i] = arreglo[j]; 
    //        arreglo[j] = temp; 
//
    //        int temp2 = arreglo_indices[i]; 
    //        arreglo_indices[i] =arreglo_indices[j]; 
    //        arreglo_indices[j] = temp2; 
    //     }
    // }
    //for(int i=0; i <= size_of_array; i++){
    //    printf("%d . %s\n",arreglo_indices[i] ,arreglo[i]);
    //}
    return arreglo;
}

typedef struct Node {
    char * letter;
    int common_len;
    int weight;
    struct Node *children[256]; // maxima cantidad segun ASCII
} Node;

//typedef struct Leaf {
//    char * index;
//} Leaf;

Node* createNode(char * l, int x, int w){
    Node* tNode = (Node*)malloc(sizeof(Node));
    tNode->letter = l;
    tNode->common_len = x;
    tNode->weight = w;
    for(int i=0; i<256; i++){
        tNode->children[i] = NULL;
    }
    return tNode;
}

void insert(Node* node, char * word, int depth){

    if(node->children[word[depth]]->letter[depth]==word[depth]){
        
    }



    printf("%s\n", word);

}





Node * build_Tree(char * text){
    Node * root = createNode(NULL, 0, 0);

    char** array = create_array(text);
    for(int i=0; i <= strlen(text); i++){
        insert(root, array[i], 0);
        }

    return root;
}

int main(int argc, char* argv[]){
    FILE * file = fopen(argv[1], "r");
    if (!file)exit(EXIT_FAILURE);
        
    struct stat sb;
    if (stat(argv[1], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    char* file_contents = malloc(sb.st_size);
    fread(file_contents, sb.st_size, 1, file);
    printf("Entrando a la funcion\n");
                
    char * caca = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    printf("%d", caca['A']);

    //Node * root = build_Tree(file_contents);
    //return 0;

    //char** array = create_array(file_contents);
    
    //printf("---------------------------\n");
    //for(int i=0; i <= strlen(file_contents); i++){
    //    printf("%s\n",array[i]);
    //}
}
