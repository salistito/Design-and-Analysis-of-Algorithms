#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define max_char 256 

int cmp(const void *a, const void *b) {
   const char *ia = *(const char **) a;
   const char *ib = *(const char **) b;
   return strcmp(ia, ib);
} 


char **create_array(char* text){
    int size_of_array = strlen (text);

    char ** arreglo = malloc(size_of_array * sizeof(char *));

    for(int i=0; i <= size_of_array; i++){
        arreglo[i] = text+i;
    }
    qsort(arreglo, size_of_array, sizeof(char *), cmp);
    return arreglo;
}

int match(char* sa_word, char* cadena) {
    int largo = strlen(cadena);
    for(int i=0; i<largo; i++) {
        if(sa_word[i]!=cadena[i])
            return 0;
    }
    return 1;
}


int main(int argc, char* argv[]) {
    //abrimos el archivo y leemos su contenido
    FILE * file = fopen(argv[1], "r");
    if (!file)
        exit(EXIT_FAILURE); 
    struct stat sb;
    if (stat(argv[1], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    char* file_contents = malloc(sb.st_size);
    fread(file_contents, sb.st_size, 1, file);
    int len_file = strlen(file_contents);

    //Creamos el arreglo de sufijos
    char **suffix_array = create_array(file_contents);

    //----------------------------------------------

    while(1){
        char cadena[max_char];
        int largo_cadena;
        //Preguntamos palabra buscada
        printf("Cadena a buscar: \n");
        if(scanf("%10[0-9a-zA-Z ]", &cadena) !=1)
            break;
        
        //printf ("el valor es: %s\n",&cadena);
        //largo_cadena = strlen(cadena);
        //printf ("el largo de la cadena es: %d\n",largo_cadena);

        // Para terminar el programa escribir Fin
        if(strcmp(cadena, "Fin") == 0){break;}

        int l = 0;
        int r = len_file;
        while (l<r){
            int mid =(int)(l+r)/2;
            if(strcmp(suffix_array[mid], cadena)<0){l=mid+1;}
            
            else{r=mid;}
        }
        
        int first = l;
        while (first>0 && match(suffix_array[first-1], cadena)==1){
            first--;
        }

        int last = r;
        while(match(suffix_array[last], cadena)==1){
            last ++;
        }

        int ocurrencias = last-first;
        printf("Ocurrencias encontradas: %d\n", ocurrencias);
        //cadena[0] = 's';
        //break;
    }

    fclose(file);
    free(file_contents);
    return 0;
}



