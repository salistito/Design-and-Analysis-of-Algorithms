#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
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

    int size_of_array = strlen (file_contents);

    char * arreglo[size_of_array];
    int arreglo_indices[size_of_array];

    for(int i=0; i <= size_of_array; i++){
        arreglo[i] = file_contents+i;
        //printf("%d",i);
        arreglo_indices[i] = i;
        //printf("%s\n", arreglo[i]);
    }

    for (int i = 0; i < size_of_array; i++) {
      for (int j = i+1; j < size_of_array; j++)
         if (strcmp(arreglo[i], arreglo[j]) > 0) {
            char* temp = arreglo[i]; 
            arreglo[i] = arreglo[j]; 
            arreglo[j] = temp; 

            int temp2 = arreglo_indices[i]; 
            arreglo_indices[i] =arreglo_indices[j]; 
            arreglo_indices[j] = temp2; 
         }
     }

    for(int i=0; i <= size_of_array; i++){
        printf("%d . %s\n",arreglo_indices[i] ,arreglo[i]);
    }


    while(1){
        printf("Cadena a buscar: ");
        char * cadena;
        int largo_cadena = strlen(cadena);
        scanf("%s", &cadena);
        if( &cadena == "Fin"){
            break;
        }
        int l = 0;
        int r = size_of_array;
        while (l<r){
            int mid =(int)(l+r)/2;
            if(file_contents[mid] < cadena){
                l=mid++;
            }
            else{
                r=mid;
            }

        int first = l;
        while (first>0){
            for(int n=0; n<= largo_cadena; n++){
                if(sscanf(file_contents[first+n],"%s")==sscanf(cadena[n],"%s")){

                }
            }
            first--;
        }
        int last = r;
        while(file_contents[last] == cadena){
            last ++;
        }
        printf("Ocurrencias encontradas: ", last-first);
        int n=1;
        
        printf("hola %s\n", &cadena);
        break;

    }

    fclose(file);
    free(file_contents);
    return 0;
}

