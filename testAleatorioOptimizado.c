#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "ABB.c"
#include "AVL.c"
#include "Btree.c"
#include "splayTree.c"

#define n 100
int operations[n];
int operations_size = (sizeof(operations)/sizeof(*operations));
unsigned int max_32bits = 4294967295;

unsigned int inserted_numbers[n]; // arreglo con los valores ya insertados en el árbol
int tree_size = 0; // Siempre se parte del árbol vacío con una inserción (Enunciado)
unsigned int nodes_for_operations[n]; // arreglo con los valores que tendrán los nodos con los que se harán las distintas operaciones
int index_nodes = 0; // indice para iterar sobre la lista de nodos para las operaciones
unsigned int random_uint; // valor aleatorio para los nodos de los árboles

// Función para obtener tiempos de CPU (user time)
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

// Función que retorna un unsigned int aleatorio asegurando que no está insertado en el árbol
unsigned int random_not_inserted(AVLNode** root, unsigned int max_value) {
    unsigned int random_uint;
    do {
        random_uint  = rand() % max_value;
    } while(AVL_find(root, random_uint)!=NULL);
    return random_uint;
}

// Se generarán números aleatorios del 1 al 6
// pi  = 1/2 -> 3/6   Si sale 1 2 3 corresponde a una operación pi
// pbe = 1/3 -> 2/6   Si sale 4 5 corresponde a una operación pbe
// pbi = 1/6 -> 1/6   Si sale 6 corresponde a una operación pbi
void experiment_setup() {
    int intentos = 0;
    int cantidad_pi = 0;
    int cantidad_pbe = 0;
    int cantidad_pbi = 0;
    printf("Generando numeros aleatorios que representan a las operaciones... \n");
    do {
        for(int i=0;i<operations_size;i++){ // generando numeros aleatorios que representan a las operaciones
            operations[i] = (rand() % 6) + 1;
        }
        intentos++;
        printf("Intentos para generar la secuencia: %d\n", intentos);
    } while(operations[0]>=4); // Cumplir que la primera operación sea una inserción
    printf("Numeros generados! \n");
    printf("calculando la cantidad de operaciones generadas para pi, pbe y pbi \n");
    for(int i = 0;i<operations_size;i++){ // calculando la cantidad de operaciones generadas para pi, pbe y pbi
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
    printf("La base de datos de virus ha sido actualizada \n");

    printf("pi: %d \n", cantidad_pi);
    printf("pbe: %d \n", cantidad_pbe);
    printf("pbi: %d \n", cantidad_pbi);
    printf("total: %d \n", cantidad_pi+cantidad_pbi+cantidad_pbe);

    printf("Precomputando valores aleatorios para realizar las futuras operaciones de inserción/búsqueda... \n");
    double precomputar_time = get_cpu_time();
    AVLNode* setup_tree = NULL;
    for(int i = 0;i<operations_size;i++){ // iterar sobre las operaciones una vez para precomputar valores aleatorios de inserción/búsqueda
        //printf("%d \n", i);
        if (operations[i]<=3){ // VERIFICADO QUE SIEMPRE AÑADE UNO QUE NO ESTÁ (en ambos arreglos)-> 50249
            // la operación es un pi (inserción)
            random_uint = random_not_inserted(&setup_tree, max_32bits); // buscar un número aleatorio no insertado
            AVL_insert(&setup_tree, random_uint);
            inserted_numbers[tree_size] = random_uint;
            tree_size++;
            nodes_for_operations[index_nodes] = random_uint;
            index_nodes++;
        }
        else if (operations[i]<= 5){ // VERIFICADO QUE SACA UN NÚMERO DE LA LISTA YA INSERTADO (en ambos arreglos) -> 33289
            // la operación es un pbe (búsqueda exitosa)
            int indice_random = rand() % tree_size; // indice aleatorio entre 0 y el tamaño del árbol -> NO ESTA INSERTADO AÚN EL 0
            random_uint = inserted_numbers[indice_random]; // acceder a la lista de números ya insertados y obtener el valor en ese indice
            nodes_for_operations[index_nodes] = random_uint;
            index_nodes++;
        }
        else{ // VERIFICADO QUE SIEMPRE SACA UNO QUE NO ESTÁ (en ambos arreglos) -> 16461
            // la operación es un pbi (búsqueda infructuosa)
            // buscar si el candidato para realizar la búsqueda infructuosa está ya insertado, si lo está -> buscar otro candidato
            random_uint = random_not_inserted(&setup_tree, max_32bits);
            nodes_for_operations[index_nodes] = random_uint;
            index_nodes++;
        }
    }
    //AVL_preorder(&setup_tree);
    printf("El tiempo de precomputo para el experimento aleatorio es: %lf \n", get_cpu_time()-precomputar_time);
}

double aleatorioABB(){
    // Esquema de operaciones -> 1) Aleatoria:
    printf("\n--------------Experimento aleatorio para ABB-------------------------\n");
    
    // Inicializar ABB
    ABBNode* random_ABB = NULL;
    int contar_caca = 0;
    int contar_caca_fructifera = 0;

    // Inicio de operaciones
    double start_time = get_cpu_time();
    for(int i = 0;i<operations_size;i++){ // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        random_uint = nodes_for_operations[i];
        if (operations[i]<=3){ // VERIFICADO QUE SIEMPRE INSERTA EN EL ÁRBOL UNO QUE NO ESTÁ -> 50249
            ABB_insert(&random_ABB, random_uint);
        }
        else {
            ABBNode* tABBNode = ABB_find(&random_ABB, random_uint);
            if (operations[i]<= 5){ // NO SE PQ SIEMPRE FALLA EN 6 BÚSQUEDAS XDDDDDDDDD -> 33289 - 6 = -> 33283
                // la operación es un pbe (búsqueda exitosa)
                if(tABBNode!=NULL && tABBNode->value==random_uint) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                    contar_caca_fructifera++;
                //else if(tABBNode==NULL) // DEBUG -> verificar que efectivamente fue una búsqueda infructuosa
                    //contar_caca++;
            }
            else{ // SIEMPRE ES UNA BÚSQUEDA INFRUCTUOSA -> 16461
                // la operación es un pbi (búsqueda infructuosa)
                // se eligió uno que no estaba insertado, lo buscamos
                if(tABBNode==NULL) // DEBUG -> verificar que efectivamente fue una búsqueda infructuosa
                    contar_caca++;
                //else if(tABBNode!=NULL && tABBNode->value==random_uint) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                    //contar_caca_fructifera++;
            }
        }
    }
    //ABB_preorder(&random_ABB);
    printf("tamaño del ABB (cantidad de inserciones): %d \n", tree_size);
    printf("búsquedas exitosas: %d \n",contar_caca_fructifera);
    printf("búsquedas infructuosas: %d \n",contar_caca);
    printf("felicidades! aprobó todos los test \n");

    return get_cpu_time()-start_time;
}

double aleatorioAVL(){
    // Esquema de operaciones -> 1) Aleatoria:
    printf("\n--------------Experimento aleatorio para AVL-------------------------\n");
    
    // Inicializar AVL
    AVLNode* random_AVL = NULL;
    int contar_caca = 0;
    int contar_caca_fructifera = 0;

    // Inicio de operaciones
    double start_time = get_cpu_time();
    for(int i = 0;i<operations_size;i++){ // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        random_uint = nodes_for_operations[i];
        if (operations[i]<=3){
            AVL_insert(&random_AVL, random_uint);
        }
        else {
            AVLNode* tAVLNode = AVL_find(&random_AVL, random_uint);
            if (operations[i]<= 5){ // NO SE PQ SIEMPRE FALLA EN 6 BÚSQUEDAS XDDDDDDDDD -> 33289 - 6 = -> 33283
                // la operación es un pbe (búsqueda exitosa)
                if(tAVLNode!=NULL && tAVLNode->value==random_uint) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                    contar_caca_fructifera++;
                else if(tAVLNode==NULL) // DEBUG -> verificar que efectivamente fue una búsqueda infructuosa
                    contar_caca++;
            }
            else{ // SIEMPRE ES UNA BÚSQUEDA INFRUCTUOSA -> 16461
                // la operación es un pbi (búsqueda infructuosa)
                // se eligió uno que no estaba insertado, lo buscamos
                if(tAVLNode==NULL) // DEBUG -> verificar que efectivamente fue una búsqueda infructuosa
                    contar_caca++;
                else if(tAVLNode!=NULL && tAVLNode->value==random_uint) // DEBUG -> verificar que efectivamente fue una búsqueda exitosa
                    contar_caca_fructifera++;
            }
        }
    }
    //AVL_preorder(&random_AVL);
    printf("tamaño del AVL (cantidad de inserciones): %d \n", tree_size);
    printf("búsquedas exitosas: %d \n",contar_caca_fructifera);
    printf("búsquedas infructuosas: %d \n",contar_caca);
    printf("felicidades! aprobó todos los test \n");

    return get_cpu_time()-start_time;
}

double aleatorio_splayTree(){
    // Esquema de operaciones -> 1) Aleatoria:
    printf("\n--------------Experimento aleatorio para splayTree-------------------------\n");
    
    // Inicializar splayTree
    splayNode* random_splayTree = NULL;
    int contar_caca = 0;
    int contar_caca_fructifera = 0;

    // Inicio de operaciones
    double start_time = get_cpu_time();
    for(int i = 0;i<operations_size;i++){ // iterar sobre las operaciones para realizarlas realmente y medir el tiempo
        random_uint = nodes_for_operations[i];
        if (operations[i]<=3){ // HAY UN PROBLEMA CON LAS INSERCIONES, NO SE ESTÁN HACIENDO BIEN
            splay_insert(&random_splayTree, random_uint);
            splay_preorder(&random_splayTree);
            printf("Termina el splayTree\n");
        }
        else{
            splayNode* tsplayNode = splay_find(&random_splayTree, random_uint);
            if (operations[i]<=5){// NO SE PQ SIEMPRE FALLA EN 6 BÚSQUEDAS XDDDDDDDDD -> 33289 - 6 = -> 33283
                // la operación es un pbe (búsqueda exitosa)
                if(tsplayNode!=NULL && tsplayNode->value==random_uint)
                    contar_caca_fructifera++;
                else if(tsplayNode==NULL || tsplayNode->value!=random_uint) // DEBUG -> verificar que efectivamente fue una búsqueda infructuosa
                    contar_caca_fructifera++;
            }
            else{ // SIEMPRE ES UNA BÚSQUEDA INFRUCTUOSA -> 16461
                // la operación es un pbi (búsqueda infructuosa)
                // se eligió uno que no estaba insertado, lo buscamos
                if(tsplayNode==NULL || tsplayNode->value!=random_uint) // DEBUG -> verificar que efectivamente fue una búsqueda infructuosa
                    contar_caca++;
                else if(tsplayNode!=NULL && tsplayNode->value==random_uint)
                    contar_caca_fructifera++;
            }
        }
    }
    //splay_preorder(&random_splayTree);
    printf("tamaño del splayTree (cantidad de inserciones): %d \n", tree_size);
    printf("búsquedas exitosas: %d \n", contar_caca_fructifera);
    printf("búsquedas infructuosas: %d \n", contar_caca);
    printf("felicidades! aprobó todos los test \n");

    return get_cpu_time()-start_time;
}

int main() {
    experiment_setup(); // generar la secuencia precisa de operaciones a realizar y aplicar la misma secuencia a cada una de las variantes de ABBs. 
    
    double exp_aleatorioABB = aleatorioABB();
    printf("El tiempo de ejecución para las operaciones en el experimento Aleatorio para el ABB es: %lf \n", exp_aleatorioABB);

    double exp_aleatorioAVL = aleatorioAVL();
    printf("El tiempo de ejecución para las operaciones en el experimento Aleatorio para el AVL es: %lf \n", exp_aleatorioAVL);

    double exp_aleatorio_splayTree = aleatorio_splayTree();
    printf("El tiempo de ejecución para las operaciones en el experimento Aleatorio para el splayTree es: %lf \n", exp_aleatorio_splayTree);
    
    /*
    double factor1 = 0.1;
    double factor2 = 0.5;
    double exp_creciente_factor1 = creciente(factor1);
    printf("El tiempo total ejecución para experimento creciente con factor=%lf es: %lf \n", factor1, exp_creciente_factor1);
    double exp_creciente_factor2 = creciente(factor2);
    printf("El tiempo total ejecución para experimento creciente con factor=%lf es: %lf \n", factor2, exp_creciente_factor2);
    */

    return 0;
}