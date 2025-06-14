/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if( tree == NULL ) return false;
  if( tree->key == key ){
     *value = tree->value;
     return true;
  }
  return ( bst_search(tree->left,key,value) || bst_search(tree->right,key,value) );
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // vytvareni noveho prvku
  if( (*tree) == NULL ){
    *tree = (bst_node_t*)malloc(sizeof(bst_node_t));
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = (*tree)->right = NULL;
    return;
  }
  //update stavajiciho itemu
  if( (*tree)->key == key ) (*tree)->value = value;
  //klic je mensi nez klic rootu: a leva
  else if( key < (*tree)->key ) bst_insert( &(*tree)->left, key, value );
  //jinak: a prava
  else bst_insert( &(*tree)->right, key, value );
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  // vymena a uvolneni pokud v pravo uz nic neni
  if( (*tree)->right == NULL ){
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    bst_node_t *tmp = *tree;
    *tree = (*tree)->left;
    free(tmp);
    return;
  }
  // pokracovat do praveho syna
  bst_replace_by_rightmost(target,&(*tree)->right);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  // neexistuje, navrat zpet
  if(*tree == NULL ) return;
  // existuje, mazu ho
  if( (*tree)->key == key ){
    bst_node_t *tmp = *tree;
    // prava cast neexistuje, prirazuju levou
    if(  !(*tree)->right ){
      *tree = (*tree)->left;
      free(tmp);
    }
    // leva cast neexistuje, prirazju pravou
    else if( !(*tree)->left && (*tree)->right ){
       *tree = (*tree)->right;
       free(tmp);
    }
    // existuje leva i prava, vymenuju podle zadani
    else{
      bst_replace_by_rightmost(tmp, &(*tree)->left );
    }
    return;
  }
  // klid je mensi: a leva
  else if( key < (*tree)->key ) bst_delete( &(*tree)->left,key );
  // jinak: a prava
  else bst_delete( &(*tree)->right,key );
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  // neni co uvolnovat
  if( *tree == NULL ) return;
  // doleva
  bst_dispose( &(*tree)->left );
  // doprava
  bst_dispose( &(*tree)->right );
  // uvolnit se
  free(*tree);
  // nastavit se na NULL
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if(tree == NULL) return;
  bst_add_node_to_items(tree,items);
  bst_preorder(tree->left,items);
  bst_preorder(tree->right,items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if(tree == NULL) return;
  bst_inorder(tree->left,items);
  bst_add_node_to_items(tree,items);
  bst_inorder(tree->right,items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if(tree == NULL) return;
  bst_postorder(tree->left,items);
  bst_postorder(tree->right,items);
  bst_add_node_to_items(tree,items);
}
