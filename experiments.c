#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "ABB.c"
#include "AVL.c"
#include "Btree16.c"
#include "Btree256.c"
#include "Btree4096.c"
//#include "Btree1.c"
//#include "Btree2.c"
//#include "Btree3.c"
#include "splayTree.c"



#define n 1000000 // cantidad de operaciones de la secuencia
#define interval 1000 // intervalo de medición de tiempos (cada 1000 operaciones)
int operations[n]; // arreglo con la secuencia de operaciones
int operations_size = (sizeof(operations)/sizeof(*operations));

double start_time, end_time, elapsed_cpu_time;
// Función para obtener tiempos de CPU (user time)
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

// Función que retorna un unsigned int aleatorio asegurando que no está insertado en el árbol (es decir que no esté repetido)
unsigned int random_not_inserted(AVLNode** root, unsigned int max_value) {
    unsigned int random_uint;
    do {
        random_uint = rand() % max_value;
    } while(AVL_find(root, random_uint)!=NULL);
    return random_uint;
}

unsigned int nearest_value(float probabilities_array[], float value, unsigned int size){
    for (int i=0; i<size;i++) {
        if (probabilities_array[i] >= value) {
            return i;
        }
    }
    return -1;
}

/*
Idea para generar la secuencia de operaciones:
Se generarán números aleatorios del 1 al 6.
Además pi=1/2 -> 3/6, pbe=1/3 -> 2/6 y pbi=1/6 -> 1/6
Por lo tanto:
    Si sale 1 2 3 corresponde a una operación pi (Inserción)
    Si sale 4 5 corresponde a una operación pbe (Búsqueda exitosa)
    Si sale 6 corresponde a una operación pbi (Búsqueda infructuosa)
*/
void operations_sequence() {
    int intentos = 0;
    int cantidad_pi = 0;
    int cantidad_pbe = 0;
    int cantidad_pbi = 0;
    printf("Generando números aleatorios del 1 al 6 que representan a las operaciones de la secuencia... \n");
    do {
        for(int i=0;i<operations_size;i++){ // generar números aleatorios del 1 al 6 que representan a las operaciones de la secuencia
            operations[i] = (rand() % 6) + 1;
        }
        intentos++;
        printf("Intentos para generar una secuencia valida: %d\n", intentos);
    } while(operations[0]>=4); // Asegurar que la primera operación (operations[0]) sea una inserción
    printf("Números generados! \n");
    printf("Calculando la cantidad de operaciones pi, pbe y pbi...\n");
    for(int i = 0;i<operations_size;i++){ // calcular la cantidad de operaciones pi, pbe y pbi
        if (operations[i]<=3){
            cantidad_pi++;
        }
        else if (operations[i]<= 5){
            cantidad_pbe++;
        }
        else{
            cantidad_pbi++;
        }
    }
    printf("Cantidad de pi (Inserciones): %d \n", cantidad_pi);
    printf("Cantidad de pbe (Búsquedas exitosas): %d \n", cantidad_pbe);
    printf("Cantidad de pbi (Búsquedas infructuosa): %d \n", cantidad_pbi);
    printf("Total: %d \n", cantidad_pi+cantidad_pbi+cantidad_pbe);
}

void generate_random_values(unsigned int** cache) {
    unsigned int max_32bits = 4294967295; // 2^32 - 1
    int tree_size = 0; // tamaño actual de los árboles, siempre se parte de un árbol vacío con una inserción (Enunciado)
    int operations_index = 0; // indice para iterar sobre la lista de valores para las operaciones
    unsigned int random_uint; // unsigned int con un valor aleatorio para los distintos nodos de los árboles

    // Variables para experimentos crecientes
    double ponderation;
    double factor01 = 0.1;
    double factor05 = 0.5;
    double k01 = factor01*tree_size;
    double k05 = factor05*tree_size;
    unsigned int random_between_0andk = 0; // número aleatorio entre 0 y k

    // Variables para experimentos sesgados
    float peso_random; // peso aleatorio que se eligirá del arreglo de pesos
    float P_x = 0; // Sumatoria de p(x)=x -> Probabilad total cuando p(x)=x
    float P_sqrt = 0; // Sumatoria de p(x)=x -> Probabilad total cuando p(x)=sqrt(x)
    float P_ln = 0; // Sumatoria de p(x)=x -> Probabilad total cuando p(x)=ln(x)

    unsigned int* aleatorio_insertions = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que se insertarán en los árboles para el experimento aleatorio
    unsigned int* aleatorio_values_for_operations = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que tendrán los nodos con los que se harán las distintas operaciones

    unsigned int* creciente01_insertions = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que se insertarán en los árboles para el experimento creciente01
    unsigned int* creciente01_values_for_operations = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que tendrán los nodos con los que se harán las distintas operaciones

    unsigned int* creciente05_insertions = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que se insertarán en los árboles para el experimento creciente05
    unsigned int* creciente05_values_for_operations = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que tendrán los nodos con los que se harán las distintas operaciones

    unsigned int* sesgado_x_insertions = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que se insertarán en los árboles para el experimento sesgado_x
    unsigned int* sesgado_x_values_for_operations = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que tendrán los nodos con los que se harán las distintas operaciones
    float* sesgado_x_probabilities = (float*)malloc(sizeof(float)*n);

    unsigned int* sesgado_sqrt_insertions = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que se insertarán en los árboles para el experimento sesgado_sqrt
    unsigned int* sesgado_sqrt_values_for_operations = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que tendrán los nodos con los que se harán las distintas operaciones
    float* sesgado_sqrt_probabilities = (float*)malloc(sizeof(float)*n);

    unsigned int* sesgado_ln_insertions = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que se insertarán en los árboles para el experimento sesgado_ln
    unsigned int* sesgado_ln_values_for_operations = (unsigned int*)malloc(sizeof(unsigned int)*n); // arreglo con los valores que tendrán los nodos con los que se harán las distintas operaciones
    float* sesgado_ln_probabilities = (float*)malloc(sizeof(float)*n);

    //----------------------------------------------------------------------------------------------------------
    printf("Precomputando valores aleatorios para realizar las futuras operaciones de inserción/búsqueda...\n");
    start_time = get_cpu_time();
    AVLNode* setup_tree = NULL; // árbol para hacer el setup y precomputar los valores que se usarán para todos los experimentos
    for(int i = 0;i<operations_size;i++){ // iterar sobre las operaciones una vez para precomputar valores aleatorios de inserción/búsqueda
        random_uint = random_not_inserted(&setup_tree, max_32bits); // buscar un número aleatorio que no haya sido insertado
        if (operations[i]<=3){
            // la operación es un pi (Inserción)
            AVL_insert(&setup_tree, random_uint); // guardarlo en el árbol del setup para marcarlo como valor ya generado para una futura inserción

            // Experimento Aleatorio
            aleatorio_insertions[tree_size] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores marcados como futuras inserciones
            aleatorio_values_for_operations[operations_index] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones

            // Experimento Creciente01 k=0.1*m
            // buscar un número aleatorio entre 0 y k "piso" + m, que no esté insertado
            ponderation = k01/max_32bits;
            random_between_0andk = (unsigned int)(random_uint*ponderation);
            creciente01_insertions[tree_size] = random_between_0andk+tree_size; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores marcados como futuras inserciones
            creciente01_values_for_operations[operations_index] = random_between_0andk+tree_size; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones

            // Experimento Creciente05 k=0.5*m
            // buscar un número aleatorio entre 0 y k "piso" + m, que no esté insertado
            ponderation = k05/max_32bits;
            random_between_0andk = (unsigned int)(random_uint*ponderation);
            creciente05_insertions[tree_size] = random_between_0andk+tree_size; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores marcados como futuras inserciones
            creciente05_values_for_operations[operations_index] = random_between_0andk+tree_size; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones
            
            // Experimento Sesgado con p(x)=x
            sesgado_x_insertions[tree_size] = random_uint;
            sesgado_x_values_for_operations[operations_index] = random_uint;
            P_x+=random_uint; // El peso del elemento es p(x)=x=random_uint
            sesgado_x_probabilities[tree_size]=P_x; // Agregar peso al arreglo que contiene los pesos de cada elemento insertado

            // Experimento Sesgado con p(x)=sqrt(x)
            sesgado_sqrt_insertions[tree_size] = random_uint;
            sesgado_sqrt_values_for_operations[operations_index] = random_uint;
            P_sqrt+=sqrt(random_uint); // El peso del elemento es p(x)=x=random_uint
            sesgado_sqrt_probabilities[tree_size]=P_sqrt; // Agregar peso al arreglo que contiene los pesos de cada elemento insertado

            // Experimento Sesgado con p(x)=ln(x)
            sesgado_ln_insertions[tree_size] = random_uint;
            sesgado_ln_values_for_operations[operations_index] = random_uint;
            P_ln+=log(random_uint); // El peso del elemento es p(x)=x=random_uint
            sesgado_ln_probabilities[tree_size]=P_ln; // Agregar peso al arreglo que contiene los pesos de cada elemento insertado
            
            // Avanzar un paso los iteradores y actualizar los k
            tree_size++; // se aumenta en uno la cantidad de elementos que tendrá el árbol
            operations_index++; // avanzar iterador
            k01 = factor01*tree_size; // 0.1*m
            k05 = factor05*tree_size; // 0.5*m
        }
        else if (operations[i]<= 5){
            // la operación es un pbe (Búsqueda exitosa)
            int indice_random = rand() % tree_size; // indice aleatorio entre 0 y el tamaño del árbol (para buscar un valor en el arreglo de elementos que se insertarán, cumpliendo así una búsqueda exitosa)

            // Experimento Aleatorio
            random_uint = aleatorio_insertions[indice_random]; // acceder a la lista de valores que se insertarán y obtener el valor en ese indice
            aleatorio_values_for_operations[operations_index] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones

            // Experimento Creciente01 k=0.1*m
            random_uint = creciente01_insertions[indice_random]; // acceder a la lista de valores que se insertarán y obtener el valor en ese indice
            creciente01_values_for_operations[operations_index] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones

            // Experimento Creciente05 k=0.5*m
            random_uint = creciente05_insertions[indice_random]; // acceder a la lista de valores que se insertarán y obtener el valor en ese indice
            creciente05_values_for_operations[operations_index] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones
            
            // Experimento Sesgado con p(x)=x
            peso_random = P_x*(indice_random/tree_size);
            random_uint = sesgado_x_insertions[nearest_value(sesgado_x_probabilities,peso_random,tree_size)];
            sesgado_x_values_for_operations[operations_index] = random_uint;

            // Experimento Sesgado con p(x)=sqrt(x)
            peso_random = (P_sqrt/tree_size)*indice_random;
            random_uint = sesgado_sqrt_insertions[nearest_value(sesgado_sqrt_probabilities,peso_random,tree_size)];
            sesgado_sqrt_values_for_operations[operations_index] = random_uint;

            // Experimento Sesgado con p(x)=ln(x)
            peso_random = (P_ln/tree_size)*indice_random;
            random_uint = sesgado_ln_insertions[nearest_value(sesgado_ln_probabilities,peso_random,tree_size)];
            sesgado_ln_values_for_operations[operations_index] = random_uint;

            // Avanzar un paso el iterador
            operations_index++;
        }
        else{
            // la operación es un pbi (Búsqueda infructuosa)
            // Experimento Aleatorio
            aleatorio_values_for_operations[operations_index] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones

            // Experimento Creciente01 k=0.1*m
            creciente01_values_for_operations[operations_index] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones

            // Experimento Creciente05 k=0.5*m
            creciente05_values_for_operations[operations_index] = random_uint; // añadirlo a un arreglo para facilitar el acceso (tiempo constante) a los valores generados para las distintas operaciones
            
            // Experimento Sesgado con p(x)=x
            sesgado_x_values_for_operations[operations_index] = random_uint;

            // Experimento Sesgado con p(x)=sqrt(x)
            sesgado_sqrt_values_for_operations[operations_index] = random_uint;

            // Experimento Sesgado con p(x)=ln(x)
            sesgado_ln_values_for_operations[operations_index] = random_uint;

            // Avanzar un paso el iterador
            operations_index++;
        }
    }
    end_time = get_cpu_time();
    elapsed_cpu_time = end_time-start_time;
    printf("El tiempo de precomputo para los experimentos fue: %lf\n", elapsed_cpu_time);
    //AVL_preorder(&setup_tree);

    // Liberar memoria para los arreglos utilizados
    free(aleatorio_insertions);
    free(creciente01_insertions);
    free(creciente05_insertions);
    free(sesgado_x_insertions);
    free(sesgado_sqrt_insertions);
    free(sesgado_ln_insertions);
    free(sesgado_x_probabilities);
    free(sesgado_sqrt_probabilities);
    free(sesgado_ln_probabilities);

    // Guardar los valores generados para las operaciones de los distintos experimentos
    cache[0] = aleatorio_values_for_operations;
    cache[1] = creciente01_values_for_operations;
    cache[2] = creciente05_values_for_operations;
    cache[3] = sesgado_x_values_for_operations;
    cache[4] = sesgado_sqrt_values_for_operations;
    cache[5] = sesgado_ln_values_for_operations;
}

void run_experiments(double* experiments_times[], unsigned int experiment_values[]) {
    ABBNode* random_ABB = NULL; // Inicializar un ABB vacío
    AVLNode* random_AVL = NULL; // Inicializar un AVL vacío
    Btree16Node* random_BTree16 = NULL; // Inicializar un BTree16 vacío
    Btree256Node* random_BTree256 = NULL; // Inicializar un BTree256 vacío
    Btree4096Node* random_BTree4096 = NULL; // Inicializar un BTree4096 vacío
    splayNode* random_splayTree = NULL; // Inicializar un splayTree vacío
    unsigned int value;

    int samples = n/interval; // intervalo de medición de tiempos (cada 1000 operaciones)

    double* ABB_times = (double*)malloc(sizeof(double)*(samples));
    double* AVL_times = (double*)malloc(sizeof(double)*(samples));
    double* BTree16_times = (double*)malloc(sizeof(double)*(samples));
    double* BTree256_times = (double*)malloc(sizeof(double)*(samples));
    double* BTree4096_times = (double*)malloc(sizeof(double)*(samples));
    double* splayTree_times = (double*)malloc(sizeof(double)*(samples));
    
    int time_index = 0;
    int inserciones = 0;
    int busquedas_exitosas = 0;
    int busquedas_infructuosas = 0;

    //----------------------------------------------Inicio de las operaciones sobre las estructuras-----------------------------------------------
    //-------------------------------------------------------------------ABB----------------------------------------------------------------------
    printf("Realizando el experimento sobre el ABB...\n");
    start_time = get_cpu_time();
    for(int i=0; i<operations_size;i++) { // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        // Chequear si la cantidad de operaciones hechas es un múltiplo de interval para detener el reloj y guardar los tiempos
        if(i>0 && i%interval==0) { // Se hicieron 1000 operaciones
            end_time = get_cpu_time();
            elapsed_cpu_time = end_time-start_time;
            ABB_times[time_index] = elapsed_cpu_time;
            time_index++;
            start_time = get_cpu_time();
        }
        value = experiment_values[i];
        if (operations[i]<=3){
            // la operación es un pi (Inserción)
            ABB_insert(&random_ABB, value);
            /*
            ABBNode* tABBNode = ABB_find(&random_ABB, value); // DEBUG verificar que efectivamente se hizo la inserción
            if(tABBNode!=NULL && tABBNode->value==value) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                inserciones++;
            */
        }
        else if (operations[i]<= 5){
            // la operación es un pbe (Búsqueda exitosa)
            ABBNode* tABBNode = ABB_find(&random_ABB, value);
            /*
            if(tABBNode!=NULL && tABBNode->value==value) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                busquedas_exitosas++;
            */
        }
        else{
            // la operación es un pbi (Búsqueda infructuosa)
            ABBNode* tABBNode = ABB_find(&random_ABB, value);
            /*
            if(tABBNode==NULL) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                busquedas_infructuosas++;
            */
        }
    }
    end_time = get_cpu_time();
    elapsed_cpu_time = end_time-start_time;
    ABB_times[time_index] = elapsed_cpu_time;
    experiments_times[0] = ABB_times; // guardar los tiempos del experimento para el ABB

    /*
    ABB_preorder(&random_ABB);
    printf("tamaño del ABB (cantidad de inserciones): %d \n", inserciones);
    printf("búsquedas exitosas: %d \n", busquedas_exitosas);
    printf("búsquedas infructuosas: %d \n", busquedas_infructuosas);
    */

    time_index = 0;
    inserciones = 0;
    busquedas_exitosas = 0;
    busquedas_infructuosas = 0;

    //-------------------------------------------------------------------AVL----------------------------------------------------------------------
    printf("Realizando el experimento sobre el AVL...\n");
    start_time = get_cpu_time();
    for(int i=0; i<operations_size;i++) { // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        // Chequear si la cantidad de operaciones hechas es un múltiplo de interval para detener el reloj y guardar los tiempos
        if(i>0 && i%interval==0) { // Se hicieron 1000 operaciones
            end_time = get_cpu_time();
            elapsed_cpu_time = end_time-start_time;
            AVL_times[time_index] = elapsed_cpu_time;
            time_index++;
            start_time = get_cpu_time();
        }
        value = experiment_values[i];
        if (operations[i]<=3){
            // la operación es un pi (Inserción)
            AVL_insert(&random_AVL, value);
            /*
            AVLNode* tAVLNode = AVL_find(&random_AVL, value); // DEBUG verificar que efectivamente se hizo la inserción
            if(tAVLNode!=NULL && tAVLNode->value==value) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                inserciones++;
            */
        }
        else if (operations[i]<= 5){
            // la operación es un pbe (Búsqueda exitosa)
            AVLNode* tAVLNode = AVL_find(&random_AVL, value);
            /*
            if(tAVLNode!=NULL && tAVLNode->value==value) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                busquedas_exitosas++;
            */
        }
        else{
            // la operación es un pbi (Búsqueda infructuosa)
            AVLNode* tAVLNode = AVL_find(&random_AVL, value);
            /*
            if(tAVLNode==NULL) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                busquedas_infructuosas++;
            */
        }
    }
    end_time = get_cpu_time();
    elapsed_cpu_time = end_time-start_time;
    AVL_times[time_index] = elapsed_cpu_time;
    experiments_times[1] = AVL_times; // guardar los tiempos del experimento para el AVL

    /*
    AVL_preorder(&random_AVL);
    printf("tamaño del AVL (cantidad de inserciones): %d \n", inserciones);
    printf("búsquedas exitosas: %d \n", busquedas_exitosas);
    printf("búsquedas infructuosas: %d \n", busquedas_infructuosas);
    */

    time_index = 0;
    inserciones = 0;
    busquedas_exitosas = 0;
    busquedas_infructuosas = 0;

    //---------------------------------------------------------------BTree16----------------------------------------------------------------------
    printf("Realizando el experimento sobre el BTree16...\n");
    start_time = get_cpu_time();
    for(int i=0; i<operations_size;i++) { // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        // Chequear si la cantidad de operaciones hechas es un múltiplo de interval para detener el reloj y guardar los tiempos
        if(i>0 && i%interval==0) { // Se hicieron 1000 operaciones
            end_time = get_cpu_time();
            elapsed_cpu_time = end_time-start_time;
            BTree16_times[time_index] = elapsed_cpu_time;
            time_index++;
            start_time = get_cpu_time();
        }
        value = experiment_values[i];
        if (operations[i]<=3){
            // la operación es un pi (Inserción)
            BTree16_insert(&random_BTree16, value);
        }
        else if (operations[i]<= 5){
            // la operación es un pbe (Búsqueda exitosa)
            BTree16_find(&random_BTree16, value);
        }
        else{
            // la operación es un pbi (Búsqueda infructuosa)
            BTree16_find(&random_BTree16, value);
        }
    }
    end_time = get_cpu_time();
    elapsed_cpu_time = end_time-start_time;
    BTree16_times[time_index] = elapsed_cpu_time;
    experiments_times[2] = BTree16_times; // guardar los tiempos del experimento para el BTree16

    /*
    BTree16_preorder(&random_BTree16);
    printf("tamaño del BTree16 (cantidad de inserciones): %d \n", inserciones);
    printf("búsquedas exitosas: %d \n", busquedas_exitosas);
    printf("búsquedas infructuosas: %d \n", busquedas_infructuosas);
    */

    time_index = 0;
    inserciones = 0;
    busquedas_exitosas = 0;
    busquedas_infructuosas = 0;

    //---------------------------------------------------------------BTree256----------------------------------------------------------------------
    printf("Realizando el experimento sobre el BTree256...\n");
    start_time = get_cpu_time();
    for(int i=0; i<operations_size;i++) { // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        // Chequear si la cantidad de operaciones hechas es un múltiplo de interval para detener el reloj y guardar los tiempos
        if(i>0 && i%interval==0) { // Se hicieron 1000 operaciones
            end_time = get_cpu_time();
            elapsed_cpu_time = end_time-start_time;
            BTree256_times[time_index] = elapsed_cpu_time;
            time_index++;
            start_time = get_cpu_time();
        }
        value = experiment_values[i];
        if (operations[i]<=3){
            // la operación es un pi (Inserción)
            BTree256_insert(&random_BTree256, value);
        }
        else if (operations[i]<= 5){
            // la operación es un pbe (Búsqueda exitosa)
            BTree256_find(&random_BTree256, value);
        }
        else{
            // la operación es un pbi (Búsqueda infructuosa)
            BTree256_find(&random_BTree256, value);
        }
    }
    end_time = get_cpu_time();
    elapsed_cpu_time = end_time-start_time;
    BTree256_times[time_index] = elapsed_cpu_time;
    experiments_times[3] = BTree256_times; // guardar los tiempos del experimento para el BTree256

    /*
    BTree256_preorder(&random_BTree256);
    printf("tamaño del BTree256 (cantidad de inserciones): %d \n", inserciones);
    printf("búsquedas exitosas: %d \n", busquedas_exitosas);
    printf("búsquedas infructuosas: %d \n", busquedas_infructuosas);
    */

    time_index = 0;
    inserciones = 0;
    busquedas_exitosas = 0;
    busquedas_infructuosas = 0;

    //---------------------------------------------------------------BTree4096----------------------------------------------------------------------
    printf("Realizando el experimento sobre el BTree4096...\n");
    start_time = get_cpu_time();
    for(int i=0; i<operations_size;i++) { // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        // Chequear si la cantidad de operaciones hechas es un múltiplo de interval para detener el reloj y guardar los tiempos
        if(i>0 && i%interval==0) { // Se hicieron 1000 operaciones
            end_time = get_cpu_time();
            elapsed_cpu_time = end_time-start_time;
            BTree4096_times[time_index] = elapsed_cpu_time;
            time_index++;
            start_time = get_cpu_time();
        }
        value = experiment_values[i];
        if (operations[i]<=3){
            // la operación es un pi (Inserción)
            BTree4096_insert(&random_BTree4096, value);
        }
        else if (operations[i]<= 5){
            // la operación es un pbe (Búsqueda exitosa)
            BTree4096_find(&random_BTree4096, value);
        }
        else{
            // la operación es un pbi (Búsqueda infructuosa)
            BTree4096_find(&random_BTree4096, value);
        }
    }
    end_time = get_cpu_time();
    elapsed_cpu_time = end_time-start_time;
    BTree4096_times[time_index] = elapsed_cpu_time;
    experiments_times[4] = BTree4096_times; // guardar los tiempos del experimento para el BTree4096

    /*
    BTree4096_preorder(&random_BTree4096);
    printf("tamaño del BTree4096 (cantidad de inserciones): %d \n", inserciones);
    printf("búsquedas exitosas: %d \n", busquedas_exitosas);
    printf("búsquedas infructuosas: %d \n", busquedas_infructuosas);
    */

    time_index = 0;
    inserciones = 0;
    busquedas_exitosas = 0;
    busquedas_infructuosas = 0;

    //----------------------------------------------------------------splayTree-------------------------------------------------------------------
    printf("Realizando el experimento sobre el splayTree...\n");
    start_time = get_cpu_time();
    for(int i=0; i<operations_size;i++) { // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        // Chequear si la cantidad de operaciones hechas es un múltiplo de interval para detener el reloj y guardar los tiempos
        if(i>0 && i%interval==0) { // Se hicieron 1000 operaciones
            end_time = get_cpu_time();
            elapsed_cpu_time = end_time-start_time;
            splayTree_times[time_index] = elapsed_cpu_time;
            time_index++;
            start_time = get_cpu_time();
        }
        value = experiment_values[i];
        if (operations[i]<=3){
            // la operación es un pi (Inserción)
            splay_insert(&random_splayTree, value);
            /*
            splayNode* tsplayNode = splay_find(&random_splayTree, value); // DEBUG verificar que efectivamente se hizo la inserción
            if(tsplayNode->value==value) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                inserciones++;
            */
        }
        else if (operations[i]<= 5){
            // la operación es un pbe (Búsqueda exitosa)
            splayNode* tsplayNode = splay_find(&random_splayTree, value);
            /*
            if(tsplayNode->value==value) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                busquedas_exitosas++;
            */
        }
        else{
            // la operación es un pbi (Búsqueda infructuosa)
            splayNode* tsplayNode = splay_find(&random_splayTree, value);
            /*
            if(tsplayNode->value!=value) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                busquedas_infructuosas++;
            */
        }
    }
    end_time = get_cpu_time();
    elapsed_cpu_time = end_time-start_time;
    splayTree_times[time_index] = elapsed_cpu_time;
    experiments_times[5] = splayTree_times; // guardar los tiempos del experimento para el splayTree

    /*
    splay_preorder(&random_splayTree);
    printf("tamaño del splayTree (cantidad de inserciones): %d \n", inserciones);
    printf("búsquedas exitosas: %d \n", busquedas_exitosas);
    printf("búsquedas infructuosas: %d \n", busquedas_infructuosas);
    */
}

int main(){
    srand(time(0));
    int num_structures = 6; // los experimentos se realizarán sobre 6 estructuras de datos
    int samples = n/interval; // intervalo de medición de tiempos (cada 1000 operaciones)
    unsigned int* experiment_values[num_structures]; // matriz con los distintos valores para los experimentos de cada estructura
    double* experiments_times[num_structures]; // matriz para guardar el tiempo de los experimentos para cada estructura
    double* ABB_times;
    double* AVL_times;
    double* BTree16_times;
    double* BTree256_times;
    double* BTree4096_times;
    double* splayTree_times;

    operations_sequence(); // Inicializar una secuencia de operaciones valida para todos los experimentos
    generate_random_values(experiment_values); // Generar valores aleatorios para los experimentos de cada estructura

    // Archivos de texto para guardar los tiempos de los experimentos
    //------------------------------------------------------------Aleatorio-----------------------------------------------------------------------
    FILE* expAleatorioABB = fopen("Results/expAleatorioABB.txt", "w"); 
    FILE* expAleatorioAVL = fopen("Results/expAleatorioAVL.txt", "w"); 
    FILE* expAleatorioBTree16 = fopen("Results/expAleatorioBTree16.txt", "w");
    FILE* expAleatorioBTree256 = fopen("Results/expAleatorioBTree256.txt", "w"); 
    FILE* expAleatorioBTree4096 = fopen("Results/expAleatorioBtree4096.txt", "w"); 
    FILE* expAleatorioSplayTree = fopen("Results/expAleatorioSplayTree.txt", "w");
    //------------------------------------------------------Creciente con factor=0.1----------------------------------------------------------------
    FILE* expCreciente01ABB = fopen("Results/expCreciente01ABB.txt", "w"); 
    FILE* expCreciente01AVL = fopen("Results/expCreciente01AVL.txt", "w"); 
    FILE* expCreciente01BTree16 = fopen("Results/expCreciente01BTree16.txt", "w");
    FILE* expCreciente01BTree256 = fopen("Results/expCreciente01BTree256.txt", "w"); 
    FILE* expCreciente01BTree4096 = fopen("Results/expCreciente01Btree4096.txt", "w"); 
    FILE* expCreciente01SplayTree = fopen("Results/expCreciente01SplayTree.txt", "w");
    //------------------------------------------------------Creciente con factor=0.5----------------------------------------------------------------
    FILE* expCreciente05ABB = fopen("Results/expCreciente05ABB.txt", "w"); 
    FILE* expCreciente05AVL = fopen("Results/expCreciente05AVL.txt", "w"); 
    FILE* expCreciente05BTree16 = fopen("Results/expCreciente05BTree16.txt", "w");
    FILE* expCreciente05BTree256 = fopen("Results/expCreciente05BTree256.txt", "w"); 
    FILE* expCreciente05BTree4096 = fopen("Results/expCreciente05Btree4096.txt", "w"); 
    FILE* expCreciente05SplayTree = fopen("Results/expCreciente05SplayTree.txt", "w");
    //------------------------------------------------------Sesgado con p(x)=x---------------------------------------------------------------------
    FILE* expSesgado_x_ABB = fopen("Results/expSesgado_x_ABB.txt", "w"); 
    FILE* expSesgado_x_AVL = fopen("Results/expSesgado_x_AVL.txt", "w"); 
    FILE* expSesgado_x_BTree16 = fopen("Results/expSesgado_x_BTree16.txt", "w");
    FILE* expSesgado_x_BTree256 = fopen("Results/expSesgado_x_BTree256.txt", "w"); 
    FILE* expSesgado_x_BTree4096 = fopen("Results/expSesgado_x_Btree4096.txt", "w"); 
    FILE* expSesgado_x_SplayTree = fopen("Results/expSesgado_x_SplayTree.txt", "w");
    //------------------------------------------------------Sesgado con p(x)=sqrt(x)---------------------------------------------------------------
    FILE* expSesgado_sqrt_ABB = fopen("Results/expSesgado_sqrt_ABB.txt", "w"); 
    FILE* expSesgado_sqrt_AVL = fopen("Results/expSesgado_sqrt_AVL.txt", "w"); 
    FILE* expSesgado_sqrt_BTree16 = fopen("Results/expSesgado_sqrt_BTree16.txt", "w");
    FILE* expSesgado_sqrt_BTree256 = fopen("Results/expSesgado_sqrt_BTree256.txt", "w"); 
    FILE* expSesgado_sqrt_BTree4096 = fopen("Results/expSesgado_sqrt_Btree4096.txt", "w"); 
    FILE* expSesgado_sqrt_SplayTree = fopen("Results/expSesgado_sqrt_SplayTree.txt", "w");
    //------------------------------------------------------Sesgado con p(x)=ln(x)-----------------------------------------------------------------
    FILE* expSesgado_ln_ABB = fopen("Results/expSesgado_ln_ABB.txt", "w"); 
    FILE* expSesgado_ln_AVL = fopen("Results/expSesgado_ln_AVL.txt", "w"); 
    FILE* expSesgado_ln_BTree16 = fopen("Results/expSesgado_ln_BTree16.txt", "w");
    FILE* expSesgado_ln_BTree256 = fopen("Results/expSesgado_ln_BTree256.txt", "w"); 
    FILE* expSesgado_ln_BTree4096 = fopen("Results/expSesgado_ln_Btree4096.txt", "w"); 
    FILE* expSesgado_ln_SplayTree = fopen("Results/expSesgado_ln_SplayTree.txt", "w");
    
    // Correr experimentos
    //------------------------------------------------------------Aleatorio-----------------------------------------------------------------------
    printf("Corriendo el experimento Aleatorio...\n");
    run_experiments(experiments_times, experiment_values[0]); 
    ABB_times = experiments_times[0];
    AVL_times = experiments_times[1];
    BTree16_times = experiments_times[2];
    BTree256_times = experiments_times[3];
    BTree4096_times = experiments_times[4];
    splayTree_times = experiments_times[5];
    // Escribir tiempos en los archivos de cada estructura de datos
    //---------------------------------------------------------------ABB------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expAleatorioABB,"%f", ABB_times[i]);
        fprintf(expAleatorioABB,"%s","\n");  
    }
    //---------------------------------------------------------------AVL------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expAleatorioAVL,"%f", AVL_times[i]);
        fprintf(expAleatorioAVL,"%s","\n");
    }
    //-------------------------------------------------------------BTree16------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expAleatorioBTree16,"%f", BTree16_times[i]);
        fprintf(expAleatorioBTree16,"%s","\n");
    }
    //-------------------------------------------------------------BTree256------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expAleatorioBTree256,"%f", BTree256_times[i]);
        fprintf(expAleatorioBTree256,"%s","\n");
    }
    //-------------------------------------------------------------BTree4096------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expAleatorioBTree4096,"%f", BTree4096_times[i]);
        fprintf(expAleatorioBTree4096,"%s","\n");
    }
    //-------------------------------------------------------------SplayTree------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expAleatorioSplayTree,"%f", splayTree_times[i]);
        fprintf(expAleatorioSplayTree,"%s","\n");
    }
    // Cerrar los archivos de cada estructura de datos
    fclose(expAleatorioABB);
    fclose(expAleatorioAVL);
    fclose(expAleatorioBTree16);
    fclose(expAleatorioBTree256);
    fclose(expAleatorioBTree4096);
    fclose(expAleatorioSplayTree);
    printf("Tiempos del experimento Aleatorio guardados\n");

    // Liberar memoria de los tiempos almacenados
    for(int i=0;i<num_structures;i++){
        free(experiments_times[i]);
    }

    //------------------------------------------------------Creciente con factor=0.1----------------------------------------------------------------
    printf("Corriendo el experimento Creciente con factor=0.1...\n");
    run_experiments(experiments_times, experiment_values[1]); 
    ABB_times = experiments_times[0];
    AVL_times = experiments_times[1];
    BTree16_times = experiments_times[2];
    BTree256_times = experiments_times[3];
    BTree4096_times = experiments_times[4];
    splayTree_times = experiments_times[5];
    // Escribir tiempos en los archivos de cada estructura de datos
    //---------------------------------------------------------------ABB------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente01ABB,"%f", ABB_times[i]);
        fprintf(expCreciente01ABB,"%s","\n");  
    }
    //---------------------------------------------------------------AVL------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente01AVL,"%f", AVL_times[i]);
        fprintf(expCreciente01AVL,"%s","\n");
    }
    //-------------------------------------------------------------BTree16------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente01BTree16,"%f", BTree16_times[i]);
        fprintf(expCreciente01BTree16,"%s","\n");
    }
    //-------------------------------------------------------------BTree256------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente01BTree256,"%f", BTree256_times[i]);
        fprintf(expCreciente01BTree256,"%s","\n");
    }
    //-------------------------------------------------------------BTree4096------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente01BTree4096,"%f", BTree4096_times[i]);
        fprintf(expCreciente01BTree4096,"%s","\n");
    }
    //-------------------------------------------------------------SplayTree------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente01SplayTree,"%f", splayTree_times[i]);
        fprintf(expCreciente01SplayTree,"%s","\n");
    }
    // Cerrar los archivos de cada estructura de datos
    fclose(expCreciente01ABB);
    fclose(expCreciente01AVL);
    fclose(expCreciente01BTree16);
    fclose(expCreciente01BTree256);
    fclose(expCreciente01BTree4096);
    fclose(expCreciente01SplayTree);
    printf("Tiempos del experimento Creciente con factor=0.1 guardados\n");
    
    // Liberar memoria de los tiempos almacenados
    for(int i=0;i<num_structures;i++){
        free(experiments_times[i]);
    }


    //------------------------------------------------------Creciente con factor=0.5----------------------------------------------------------------
    printf("Corriendo el experimento Creciente con factor=0.5...\n");
    run_experiments(experiments_times, experiment_values[2]); 
    ABB_times = experiments_times[0];
    AVL_times = experiments_times[1];
    BTree16_times = experiments_times[2];
    BTree256_times = experiments_times[3];
    BTree4096_times = experiments_times[4];
    splayTree_times = experiments_times[5];
    // Escribir tiempos en los archivos de cada estructura de datos
    //---------------------------------------------------------------ABB------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente05ABB,"%f", ABB_times[i]);
        fprintf(expCreciente05ABB,"%s","\n");  
    }
    //---------------------------------------------------------------AVL------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente05AVL,"%f", AVL_times[i]);
        fprintf(expCreciente05AVL,"%s","\n");
    }
    //-------------------------------------------------------------BTree16------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente05BTree16,"%f", BTree16_times[i]);
        fprintf(expCreciente05BTree16,"%s","\n");
    }
    //-------------------------------------------------------------BTree256------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente05BTree256,"%f", BTree256_times[i]);
        fprintf(expCreciente05BTree256,"%s","\n");
    }
    //-------------------------------------------------------------BTree4096------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente05BTree4096,"%f", BTree4096_times[i]);
        fprintf(expCreciente05BTree4096,"%s","\n");
    }
    //-------------------------------------------------------------SplayTree------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expCreciente05SplayTree,"%f", splayTree_times[i]);
        fprintf(expCreciente05SplayTree,"%s","\n");
    }
    // Cerrar los archivos de cada estructura de datos
    fclose(expCreciente05ABB);
    fclose(expCreciente05AVL);
    fclose(expCreciente05BTree16);
    fclose(expCreciente05BTree256);
    fclose(expCreciente05BTree4096);
    fclose(expCreciente05SplayTree);
    printf("Tiempos del experimento Creciente con factor=0.5 guardados\n");
    
    // Liberar memoria de los tiempos almacenados
    for(int i=0;i<num_structures;i++){
        free(experiments_times[i]);
    }

    //------------------------------------------------------Sesgado con p(x)=x---------------------------------------------------------------------
    printf("Corriendo el experimento Sesgado con p(x)=x...\n");
    run_experiments(experiments_times, experiment_values[3]); 
    ABB_times = experiments_times[0];
    AVL_times = experiments_times[1];
    BTree16_times = experiments_times[2];
    BTree256_times = experiments_times[3];
    BTree4096_times = experiments_times[4];
    splayTree_times = experiments_times[5];
    // Escribir tiempos en los archivos de cada estructura de datos
    //---------------------------------------------------------------ABB------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_x_ABB,"%f", ABB_times[i]);
        fprintf(expSesgado_x_ABB,"%s","\n");  
    }
    //---------------------------------------------------------------AVL------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_x_AVL,"%f", AVL_times[i]);
        fprintf(expSesgado_x_AVL,"%s","\n");
    }
    //-------------------------------------------------------------BTree16------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_x_BTree16,"%f", BTree16_times[i]);
        fprintf(expSesgado_x_BTree16,"%s","\n");
    }
    //-------------------------------------------------------------BTree256------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_x_BTree256,"%f", BTree256_times[i]);
        fprintf(expSesgado_x_BTree256,"%s","\n");
    }
    //-------------------------------------------------------------BTree4096------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_x_BTree4096,"%f", BTree4096_times[i]);
        fprintf(expSesgado_x_BTree4096,"%s","\n");
    }
    //-------------------------------------------------------------SplayTree------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_x_SplayTree,"%f", splayTree_times[i]);
        fprintf(expSesgado_x_SplayTree,"%s","\n");
    }
    // Cerrar los archivos de cada estructura de datos
    fclose(expSesgado_x_ABB);
    fclose(expSesgado_x_AVL);
    fclose(expSesgado_x_BTree16);
    fclose(expSesgado_x_BTree256);
    fclose(expSesgado_x_BTree4096);
    fclose(expSesgado_x_SplayTree);
    printf("Tiempos del experimento Sesgado con p(x)=x guardados\n");
    
    // Liberar memoria de los tiempos almacenados
    for(int i=0;i<num_structures;i++){
        free(experiments_times[i]);
    }

    //------------------------------------------------------Sesgado con p(x)=sqrt(x)---------------------------------------------------------------------
    printf("Corriendo el experimento Sesgado con p(x)=sqrt(x)...\n");
    run_experiments(experiments_times, experiment_values[4]); 
    ABB_times = experiments_times[0];
    AVL_times = experiments_times[1];
    BTree16_times = experiments_times[2];
    BTree256_times = experiments_times[3];
    BTree4096_times = experiments_times[4];
    splayTree_times = experiments_times[5];
    // Escribir tiempos en los archivos de cada estructura de datos
    //---------------------------------------------------------------ABB------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_sqrt_ABB,"%f", ABB_times[i]);
        fprintf(expSesgado_sqrt_ABB,"%s","\n");  
    }
    //---------------------------------------------------------------AVL------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_sqrt_AVL,"%f", AVL_times[i]);
        fprintf(expSesgado_sqrt_AVL,"%s","\n");
    }
    //-------------------------------------------------------------BTree16------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_sqrt_BTree16,"%f", BTree16_times[i]);
        fprintf(expSesgado_sqrt_BTree16,"%s","\n");
    }
    //-------------------------------------------------------------BTree256------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_sqrt_BTree256,"%f", BTree256_times[i]);
        fprintf(expSesgado_sqrt_BTree256,"%s","\n");
    }
    //-------------------------------------------------------------BTree4096------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_sqrt_BTree4096,"%f", BTree4096_times[i]);
        fprintf(expSesgado_sqrt_BTree4096,"%s","\n");
    }
    //-------------------------------------------------------------SplayTree------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_sqrt_SplayTree,"%f", splayTree_times[i]);
        fprintf(expSesgado_sqrt_SplayTree,"%s","\n");
    }
    // Cerrar los archivos de cada estructura de datos
    fclose(expSesgado_sqrt_ABB);
    fclose(expSesgado_sqrt_AVL);
    fclose(expSesgado_sqrt_BTree16);
    fclose(expSesgado_sqrt_BTree256);
    fclose(expSesgado_sqrt_BTree4096);
    fclose(expSesgado_sqrt_SplayTree);
    printf("Tiempos del experimento Sesgado con p(x)=sqrt(x) guardados\n");
    
    // Liberar memoria de los tiempos almacenados
    for(int i=0;i<num_structures;i++){
        free(experiments_times[i]);
    }

    //------------------------------------------------------Sesgado con p(x)=ln(x)---------------------------------------------------------------------
    printf("Corriendo el experimento Sesgado con p(x)=ln(x)...\n");
    run_experiments(experiments_times, experiment_values[5]); 
    ABB_times = experiments_times[0];
    AVL_times = experiments_times[1];
    BTree16_times = experiments_times[2];
    BTree256_times = experiments_times[3];
    BTree4096_times = experiments_times[4];
    splayTree_times = experiments_times[5];
    // Escribir tiempos en los archivos de cada estructura de datos
    //---------------------------------------------------------------ABB------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_ln_ABB,"%f", ABB_times[i]);
        fprintf(expSesgado_ln_ABB,"%s","\n");  
    }
    //---------------------------------------------------------------AVL------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_ln_AVL,"%f", AVL_times[i]);
        fprintf(expSesgado_ln_AVL,"%s","\n");
    }
    //-------------------------------------------------------------BTree16------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_ln_BTree16,"%f", BTree16_times[i]);
        fprintf(expSesgado_ln_BTree16,"%s","\n");
    }
    //-------------------------------------------------------------BTree256------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_ln_BTree256,"%f", BTree256_times[i]);
        fprintf(expSesgado_ln_BTree256,"%s","\n");
    }
    //-------------------------------------------------------------BTree4096------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_ln_BTree4096,"%f", BTree4096_times[i]);
        fprintf(expSesgado_ln_BTree4096,"%s","\n");
    }
    //-------------------------------------------------------------SplayTree------------------------------------------------------------------------------
    for(int i=0;i<samples;i++){
        fprintf(expSesgado_ln_SplayTree,"%f", splayTree_times[i]);
        fprintf(expSesgado_ln_SplayTree,"%s","\n");
    }
    // Cerrar los archivos de cada estructura de datos
    fclose(expSesgado_ln_ABB);
    fclose(expSesgado_ln_AVL);
    fclose(expSesgado_ln_BTree16);
    fclose(expSesgado_ln_BTree256);
    fclose(expSesgado_ln_BTree4096);
    fclose(expSesgado_ln_SplayTree);
    printf("Tiempos del experimento Sesgado con p(x)=ln(x) guardados\n");
    
    // Liberar memoria de los tiempos almacenados
    for(int i=0;i<num_structures;i++){
        free(experiments_times[i]);
    }
    
    // Liberar memoria de la matriz con los distintos valores para los experimentos de cada estructura
    for(int i=0;i<num_structures;i++){
        free(experiment_values[i]);
    }
    
    return 0;
}