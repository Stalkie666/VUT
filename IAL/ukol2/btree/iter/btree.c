/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t * tmp = tree;
  // pruchod stromem dokud nenajdu prvek nebo nenarazim na NULL
  while(1){
    if( tmp == NULL ) break;
    if( tmp->key == key ){
      *value = tmp->value;
      return true;
    }
    else{
      if( key < tmp->key ) tmp = tmp->left;
      else tmp = tmp->right;
    }
  }
  return false;
}


// Pomocna funkce k vytvoreni noveho node - vytvoreno ciste z duvodu zprehledneni kodu, prosim nepenalizovat
bst_node_t * allocNode(char key, int value){
  bst_node_t * retVal = (bst_node_t*)malloc(sizeof(bst_node_t));
  retVal->key = key;
  retVal->value = value;
  retVal->left = retVal->right = NULL;
  return retVal;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if( (*tree) == NULL ){
    // uziti pomocne funkce z duvou opisovani kodu - nijak to nezlehcuje zadani, ale prinasi to prehlednost
    *tree = allocNode(key,value);
    return;
  }
  bst_node_t * tmp = *tree;
  while(1){
    // key nalezen, prepsat hodnotu
    if( tmp->key == key ){
      tmp->value = value;
      return;
    }
    // klic je mensi nez root, jdu doleva
    else if(key < tmp->key){
      // leva null, vytvoreni novaho potomka
      if(tmp->left == NULL){
        // uziti pomocne funkce z duvou opisovani kodu - nijak to nezlehcuje zadani, ale prinasi to prehlednost
        tmp->left = allocNode(key,value);
        return;
      }
      // presun do leveho potomka
      else tmp = tmp->left;
    }
    // klic vetsi nez root, jdu doprava
    else{
      //prava null, vytvoreni noveho potomka
      if( tmp->right == NULL ){
        // uziti pomocne funkce z duvou opisovani kodu - nijak to nezlehcuje zadani, ale prinasi to prehlednost
        tmp->right = allocNode(key,value);
        return;
      }
      // presun do leveho potomka
      else tmp = tmp->right;
    }
  }

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) { 
  // kontrola jestle nebudu presouvat uz root
  if( (*tree)->right == NULL ){
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    bst_node_t *tmp = *tree;
    *tree = (*tree)->left;
    free(tmp);
    return;
  }

  bst_node_t * tmp = *tree;
  //proskakani do nejvice praveho
  while(1){
    // nalezeni nejvice praveho a prekopirovani dat, nutno delat z urovne o jedna vyssi
    if( tmp->right->right == NULL ){
      target->key = tmp->right->key;
      target->value = tmp->right->value;
      // frm == for remove
      bst_node_t * frm = tmp->right;
      tmp->right = tmp->right->left;
      free(frm);
      return;
    }
    tmp = tmp->right;
  }
  
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  // kontrola prazdneho stromu
  if(*tree == NULL) return;
  // kontrola jestli neni mazany prvek uz na vrchu
  if( (*tree)->key == key ){
    // prevek nema potomky
    if( !(*tree)->left && !(*tree)->right ){
      free(*tree);
      *tree = NULL;
      return;
    }
    // ma leveho
    else if( !(*tree)->right ){
      bst_node_t * tmp = (*tree)->left;
      (*tree)->key = tmp->key;
      (*tree)->value = tmp->value;
      (*tree)->left = tmp->left;
      (*tree)->right = tmp->right;
      free(tmp);
      return;
    }
    // ma praveho
    else if( !(*tree)->left){
      bst_node_t * tmp = (*tree)->right;
      (*tree)->key = tmp->key;
      (*tree)->value = tmp->value;
      (*tree)->left = tmp->left;
      (*tree)->right = tmp->right;
      free(tmp);
      return;
    }
    // ma oba
    else{
      bst_replace_by_rightmost(*tree,&(*tree)->left);
      return;
    }
  }


  // to same co predtim, jen trochu upravene
  bst_node_t * previous = *tree;
  bst_node_t * current;
  if(key < previous->key)current = previous->left;
  else current = previous->right;

  while(1){
    // prvek nenalezen
    if(current == NULL) return;
    // prvek nalezen
    if(current->key == key){
      // bezdetny
      if( !current->left && !current->right){
        if( previous->left->key == current->key ) previous->left = NULL;
        else previous->right = NULL;
        free(current);
        return;
      }
      // ma jen leveho potomka
      else if(!current->right){
        bst_node_t * tmp = current->left;
        current->key = tmp->key;
        current->value = tmp->value;
        current->left = tmp->left;
        current->right = tmp->right;
        free(tmp);
        return;
      }
      // jen praveho potomka
      else if(!current->left){
        bst_node_t * tmp = current->right;
        current->key = tmp->key;
        current->value = tmp->value;
        current->left = tmp->left;
        current->right = tmp->right;
        free(tmp);
        return;
      }
      // ma oba potomky
      else{
        bst_replace_by_rightmost(current,&(current)->left);
      }
    }
    // klic je mensi nez root
    else if(key < current->key){
      previous = current;
      current = current->left;
    }
    // klic je vesi nez root
    else{
      previous = current;
      current = current->right;
    }
  }
  
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  // tree je prazdny
  if(*tree == NULL) return;

  // priprava zasobniku
  stack_bst_t tmpStack;
  stack_bst_t * stack = &tmpStack;
  stack_bst_init(stack);

  bst_node_t * current = *tree;
  bst_node_t * tmp;

  // opsano z prednasky
  while(1){
    if( !current->left && !current->right ){
      free(current);
      if( stack_bst_empty(stack) ) break;
      else current = stack_bst_pop(stack);
    }
    else if(!current->left){
      stack_bst_push(stack,current);
      tmp = current;
      current = current->right;
      tmp->right = NULL;
    }
    else{
      stack_bst_push(stack,current);
      tmp = current;
      current = current->left;
      tmp->left = NULL;
    }
  }
  // nastaveni stromu na init
  *tree = NULL;
}


/*
  DISCLAIMER - Pokud mi tohle chytnete jako plagiat tak se jedna o presny opis ze slidu z prednasek, konkretne 04_Stromy - Slidy 16 az 21
*/

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  // vzato z prednasky
  while( tree != NULL ){
    stack_bst_push(to_visit,tree);
    bst_add_node_to_items(tree,items);
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  // init zasobniku
  stack_bst_t tmpStack;
  stack_bst_t * stack = &tmpStack;
  stack_bst_init(stack);
  // vzato z prednasky
  bst_leftmost_preorder(tree,stack,items);
  while( !stack_bst_empty(stack) ){
    tree = stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right,stack,items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  // vzato z prednasky
  while( tree != NULL ){
    stack_bst_push(to_visit,tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  // init zasobniku
  stack_bst_t tmpStack;
  stack_bst_t * stack = &tmpStack;
  stack_bst_init(stack);
  // vzato z prednasky
  bst_leftmost_inorder(tree,stack);
  while( !stack_bst_empty(stack) ){
    tree = stack_bst_pop(stack);
    bst_add_node_to_items(tree,items);
    bst_leftmost_inorder(tree->right,stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
                              // vzato z prednasky
                              while (tree != NULL){
                                stack_bst_push(to_visit,tree);
                                stack_bool_push(first_visit,tree);
                                tree = tree->left;
                              }
                              
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  // priprava zasobniku a prommene - vzato z prednasky
  bool fromLeft;
  stack_bst_t tmpStack;
  stack_bst_t * stack = &tmpStack;
  stack_bst_init(stack);

  stack_bool_t tmpBool;
  stack_bool_t * boolStack = &tmpBool;
  stack_bool_init(boolStack);
  // vzato z prednasky
  bst_leftmost_postorder(tree,stack,boolStack);
  while( !stack_bst_empty(stack) ){
    tree = stack_bst_top(stack);
    fromLeft = stack_bool_pop(boolStack);
    if( fromLeft ){
      stack_bool_push(boolStack,false);
      bst_leftmost_postorder(tree->right,stack,boolStack);
    }
    else{
      stack_bst_pop(stack);
      bst_add_node_to_items(tree,items);
    }
  }
}
