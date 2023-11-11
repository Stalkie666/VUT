/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);
    int value;
    char tmp;
    // cist vstup do konce
    while( *input != '\0' ){
        // priradit stavajici char
        tmp = *input;
        // dat na mala pismenka, pokud jsou velka
        if( tmp >= 'A' && tmp <= 'Z' ) tmp += 32; 
        // nastavit na '_' pokud se nejedna o a..zA..Z nebo ' ' 
        if( !((tmp >= 'a' && tmp <= 'z') || (tmp >= 'A' &&  tmp <= 'Z') || tmp == ' ' ) )
            tmp = '_';
        // najit prvek, pokud existuje a zvetsit hodnotu value
        if( bst_search(*tree,tmp,&value) ) ++value;
        // nebo pro novy prvek nastavit value na 1
        else value = 1;
        // vlozit novy prvek nebo upravit hodnotu stavajiciho prvku
        bst_insert(tree,tmp,value);
        // na dalsi char
        input++;
    }
}

// pomozna struktura
typedef struct item{
    char key;
    int value;
}item_t;

// struktura, jsem liny si deklarovat 3 promnene a ty vsude posilat
typedef struct items
{
    size_t alloc_size;
    size_t real_size;
    item_t * array;
}items_array_t;

// inicializace pomocneho seznamu
items_array_t * item_array_init(){
    items_array_t * items = (items_array_t*)malloc(sizeof(items_array_t));
    if( !items ) return NULL;
    items->alloc_size = 8;
    items->real_size = 0;
    items->array = (item_t*)malloc(items->alloc_size * sizeof(item_t));
    if( !items->array ){
        free(items);
        return NULL;
    }
    return items;
}

// uklid, at to nevypada jako na kolejich
void items_array_dispose( items_array_t * arr ){
    free(arr->array);
    free(arr);
}

// pridani itemu do seznamu
void add_item(items_array_t * arr, item_t item){
    if( arr->real_size >= arr->alloc_size ){
        arr->alloc_size *= 2;
        // ano spoleham na to ze realloc neselze,nemel bych to delat, dont judge me, pls
        arr->array = (item_t*)realloc(arr->array,arr->alloc_size*sizeof(item_t));
    }
    arr->array[arr->real_size].key = item.key;
    arr->array[arr->real_size].value = item.value;
    arr->real_size++;
}

// pomocna funkce na ziskani hodnot ze stromu a ulozeni do pole
// diky tomu ze uz puvodni strom byl serazeny tak i ziskany seznam pri pouziti in-order bude serazeny, neni tedy treba seradit
void get_items_from_tree(bst_node_t * tree, items_array_t * items){
    if( tree == NULL ) return;
    get_items_from_tree( tree->left,items);
    item_t new_item;
    new_item.key = tree->key;
    new_item.value = tree->value;
    add_item(items,new_item);
    get_items_from_tree(tree->right,items);
}

//pomocna funkce pro vytvoreni stromu 
// - prakticky prostredni prvek oznacim za node a insertnu ho, 1. polovinu poslu do leva, 2. polovinu do prava
// - ano delano rekurzivne, jinak by to asi ani neslo nebo by to bylo na 50 radku a musel bych si hrat se zasobnikem
void create_new_tree(bst_node_t ** tree, item_t * array, int n_items){
    if( n_items == 0 ) return;
    // zalezi jestli je pocet prvku v poli sudy nebo lichy
    //  **#***
    if( n_items % 2 == 0 ){
        bst_insert(tree, array[(n_items / 2) - 1].key, array[(n_items / 2) - 1].value);
        create_new_tree( &(*tree)->left, array, (n_items / 2) - 1 );
        create_new_tree( &(*tree)->right, array + (n_items / 2), (n_items / 2) );
    }
    // ***#***
    else{
        bst_insert(tree, array[(n_items / 2)].key, array[(n_items / 2)].value);
        create_new_tree( &(*tree)->left, array, (n_items / 2) );
        create_new_tree( &(*tree)->right, array + (n_items / 2) + 1, (n_items / 2) );
    }
}


/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // allokovat pomocne pole
    items_array_t * items = item_array_init();
    if( !items ){
        fprintf(stderr,"Chyba alokace pameti, nelze dale pokracovat.\n");
        return;
    }
    // ziskat prvky
    get_items_from_tree(*tree,items);
    // smazat korektne stary strom
    bst_dispose(tree);
    // vybalancovat novy
    create_new_tree(tree,items->array,items->real_size);
    // uklid - jsme prece slusni kluci
    items_array_dispose(items);
}