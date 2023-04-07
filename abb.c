#include "abb.h"
#include "definiciones.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

///////////////////////// ESTRUCTURAS DE DATOS

struct celda {
    tipoelem info;
    struct celda *izq, *der;
};

/////////////////////////// INICIO PARTE MODIFICABLE

/*Extraer la clave de una celda */
tipoclave _clave_elem(tipoelem *E) {
    return E->lexema;
}

/* Esta funcion puente nos permite modificar el tipo de
 * de datos del TAD sin tener que cambiar todas las
 * comparaciones del resto de la biblioteca y en su lugar
 * cambiando solo esta. */
int _comparar_claves(tipoclave cl1, tipoclave cl2) {
    return strcmp(cl1,cl2)==0 ? 0 : strcmp(cl1,cl2)>0 ? 1 : -1;
}

/* Si tipoelem tiene alguna estructura que necesite
 * destruirse ha de hacerse aqui. El uso de esta funcion
 * permite hacer mas eficiente la destruccion del arbol.*/
void _destruir_elem(tipoelem *E) {
    if(E != NULL){
        if (E->lexema != NULL) {//lexeme memory release
            free((E)->lexema);
            E->lexema = NULL;
        }
    }

}

/////////////////////////// FIN PARTE MODIFICABLE
/////////////////////////////////////////////////////////////

//OPERACIONES DE CREACIÓN Y DESTRUCCIÓN

void crear(abb *A) {
    *A = NULL;
}


void destruir(abb *A) {
    if (*A != NULL) {
        destruir(&(*A)->izq);
        destruir(&(*A)->der);
        _destruir_elem(&((*A)->info));
    }
    free(*A);
    *A = NULL;
}


/*
void destruir(abb *A) {
    if (es_vacio(*A)) {
        A = (abb) malloc(sizeof (struct celda));
        (*A)->info.lex = (char)malloc(sizeof(char) (strlen(E.lex) + 1));
        strcpy((*A)->info.lex, E.lex);
        if (*A) {
            (*A)->info.lex[strlen(E.lex)] = '\0';
            (*A)->info.codigo = E.;
            (*A)->der = NULL;
            (*A)->der = NULL;
        }
    return;
    }
}
 */



//OPERACIONES DE INFORMACIÓN

unsigned es_vacio(abb A) {
    return A == NULL;
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

void leer(abb A, tipoelem *E) {
    *E = A->info;
}
// Función privada para comparar las claves

int _comparar_clave_elem(tipoclave cl, tipoelem E) {
    return _comparar_claves(cl, _clave_elem(&E));
}
//Función privada para informar si una clave está en el árbol

unsigned _es_miembro_clave(abb A, tipoclave cl) {
    if (es_vacio(A)) {
        return 0;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { //cl == A->info
        return 1;
    }
    if (comp > 0) { //cl > A->info
        return _es_miembro_clave(der(A), cl);
    }
    //cl < A->info
    return _es_miembro_clave(izq(A), cl);
}

//Funciones públicas

unsigned es_miembro(abb A, tipoelem E) {
    return _es_miembro_clave(A, _clave_elem(&E));
}

void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (es_vacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { // cl == A->info
        *nodo = A->info;
    } else if (comp < 0) { // cl < A->info
        buscar_nodo(A->izq, cl, nodo);
    } else { // cl > A->info
        buscar_nodo(A->der, cl, nodo);
    }
}

int buscar_nodoComando(abb A, char *comando) {
    if (es_vacio(A)) {
        return 0;
    }
    int comp = strcmp(comando, A->info.lexema);

    if (comp == 0) { // cl == A->info Son iguale sy por lo tanto se devuelve el código
        return A->info.type;
    } else if (comp < 0) { // cl < A->info
        return buscar_nodoComando(A->izq, comando);
    } else { // cl > A->info
        return buscar_nodoComando(A->der, comando);
    }
}

//OPERACIONES DE MODIFICACIÓN

/* Funcion recursiva para insertar un nuevo nodo
   en el arbol. Se presupone que no existe un nodo
   con la misma clave en el arbol. */
void insertar(abb *A, tipoelem E) {
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info.lexema = (char*)malloc(sizeof(char)*(strlen(E.lexema) + 1));
        strcpy((*A)->info.lexema, E.lexema);
        if (*A) {
            (*A)->info.lexema[strlen(E.lexema)] = '\0';
            (*A)->izq = NULL;
            (*A)->der = NULL;
        }
        return;

    }
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp > 0) {
        insertar(&(*A)->der, E);
    } else {
        insertar(&(*A)->izq, E);
    }
}

/*
* Funcion recursiva para insertar un nuevo nodo de comando
*/
void insertarComando(abb *A, char *nombre, int id, void *funcion){
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info.lexema = (char*)malloc(sizeof(char)*(strlen(nombre) + 1));
        strncpy((*A)->info.lexema, nombre, strlen(nombre)+1);
        if (*A) {
            (*A)->info.lexema[strlen(nombre)] = '\0';
            (*A)->info.type = id;
            (*A)->info.data.func = funcion;
            (*A)->info.initVal = 1;
            (*A)->izq = NULL;
            (*A)->der = NULL;
        }
        return;

    }
    int comp = strcmp(nombre, (*A)->info.lexema);
    if (comp > 0) {
        insertarComando(&(*A)->der, nombre, id, funcion);
    } else {
        insertarComando(&(*A)->izq, nombre, id, funcion);
    }
    
}

void insertarVar(abb *A, char *nombre, int id, double valor){
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info.lexema = (char*)malloc(sizeof(char)*(strlen(nombre) + 1));
        strncpy((*A)->info.lexema, nombre, strlen(nombre)+1);
        if (*A) {
            (*A)->info.lexema[strlen(nombre)] = '\0';
            (*A)->info.type = id;
            (*A)->info.data.val = valor;
            (*A)->info.initVal = 1;
            (*A)->izq = NULL;
            (*A)->der = NULL;
        }
        return;

    }
    int comp = strcmp(nombre, (*A)->info.lexema);
    if (!comp && (*A)->info.type == ID_VAR) {
        (*A)->info.data.val = valor;
        (*A)->info.initVal = 1;
        
    }else if( comp < 0){
        insertarVar(&(*A)->der, nombre, id, valor);
    } else if (comp > 0) {
        insertarVar(&(*A)->izq, nombre, id, valor);
    }
    
}

/* Funcion privada que devuelve mínimo de subárbol dcho */
tipoelem _suprimir_min(abb * A) {//Se devuelve el elemento más a la izquierda
    abb aux;
    tipoelem ele;
    if (es_vacio((*A)->izq)) {//Si izquierda vacía, se devuelve valor nodo actual A
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq); //se vuelve a buscar mínimo rama izquierda
    }
}

/* Funcion que permite eliminar un nodo del arbol */
void suprimir(abb *A, tipoelem E) {
    abb aux;
    if (es_vacio(*A)) {
        return;
    }

    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp < 0) { //if (E < (*A)->info) {
        suprimir(&(*A)->izq, E);
    } else if (comp > 0) { //(E > (*A)->info) {
        suprimir(&(*A)->der, E);
    } else if (es_vacio((*A)->izq) && es_vacio((*A)->der)) {
        _destruir_elem(&((*A)->info));
        free(*A);
        *A = NULL;
    } else if (es_vacio((*A)->izq)) { // pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        _destruir_elem(&aux->info);
        free(aux);
    } else if (es_vacio((*A)->der)) { //pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        _destruir_elem(&aux->info);
        free(aux);
    } else { //ni derecha ni izquierda esta vacio, busco mínimo subárbol derecho
        _destruir_elem(&(*A)->info); //elimino la info pero no libero el nodo,
        //pues en su sitio voy a poner el mínimo del subárbol derecho
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}

/* Funcion privada para pasar la clave y no tener que
   extraerla del nodo en las llamadas recursivas.*/
void _modificar(abb A, tipoclave cl, tipoelem nodo) {
    if (es_vacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);
    if (comp == 0) {
        A->info = nodo;
    } else if (comp < 0) {
        _modificar(A->izq, cl, nodo);
    } else {
        _modificar(A->der, cl, nodo);
    }
}


/* Permite modificar el nodo extrayendo del mismo la clave */
void modificar(abb A, tipoelem nodo) {
    tipoclave cl = _clave_elem(&nodo);
    _modificar(A, cl, nodo);
}

/*
 * Funcion auxiliar recursiva de print de la tabla
 */
void _printTabla(abb *A){

    if (!es_vacio(*A)) {
        if (&(*A)->izq != NULL) {
            _printTabla(&(*A)->izq);
            printf("\nLexema: %14s", (*A)->info.lexema);
            printf("%15s: %-3d", "Codigo", (*A)->info.type);
            printf("%18s: %d", "Inicializado", (*A)->info.initVal);
            if ((*A)->info.type == ID_CONST || (*A)->info.type == ID_VAR) {
                printf("%13s: %-7f", "Valor", (*A)->info.data.val);
            } else {
                printf("%15s: %-3s", "Funcion", (*A)->info.lexema);
            }
        }



        if (&(*A)->der != NULL) {
            _printTabla(&(*A)->der);
        }
    }

}
