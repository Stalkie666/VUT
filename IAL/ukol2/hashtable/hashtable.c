/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  // vynulovani pole
  for(int i = 0; i < HT_SIZE; ++i)
    *(*table + i) = NULL;
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int index = get_hash(key);
  // zkousim hledat, pokud najdu vratim odkaz na prvek a jinak NULL
  for(ht_item_t * item = *(*table + index); item != NULL; item = item->next){
    if(strcmp( item->key,key ) == 0) return item;
  }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  // najit prvek, pokud neni vytvorit
  ht_item_t * tmp = ht_search(table,key);
  if( !tmp ){
    //alokace pameti
    tmp = (ht_item_t*)malloc(sizeof(ht_item_t));
    tmp->key =(char*)calloc((strlen(key)+1),sizeof(char));
    //kopirovani dat
    strcpy(tmp->key,key);
    tmp->value = value;
    tmp->next = NULL;
    int index = get_hash(key);
    ht_item_t * item = *(*table + index);
    if( item == NULL) *(*table + index) = tmp;
    else{
      // vlozeni itemu na zacatek seznamu
      tmp->next = item;
      *(*table + index) = tmp;
    }
  }
  else{
    // aktualizace hodnoty
    tmp->value = value;
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  // vracim odkaz na hodnotu
  return &(ht_search(table,key)->value);
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  // ziskat hash... vzato z projektu do IJC a mirne upraveno, fakt nevim sco sem psat
  size_t index = get_hash(key);
  ht_item_t * current = *(*table + index);
  ht_item_t * previous = NULL;

  while( current != NULL ){
    if( strcmp(current->key,key)==0 ){
      if( current == *(*table + index) ){
        *(*table + index) = current->next;
      }
      else{
        previous->next = current->next;
      }
      free(current->key);
      free(current);
      return;
    }
    previous = current;
    current = current->next;
  }
  return;
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  // princip - prolezt cele pole a vsechny polozky spojovych seznamu mazat za sebou
  for(int i = 0; i < HT_SIZE; ++i){
    ht_item_t * item = *(*table + i);

    while(item != NULL){
      ht_item_t * tmp = item;
      item = item->next;
      if( tmp->key != NULL ) free(tmp->key);
      if( tmp != NULL ) free(tmp);
    }
    // prohlasit item pole za prazdny
    *(*table + i) = NULL;
  }
}
