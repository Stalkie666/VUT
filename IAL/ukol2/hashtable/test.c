#include "hashtable.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define INSERT_TEST_DATA(TABLE)                                                \
  ht_insert_many(TABLE, TEST_DATA, sizeof(TEST_DATA) / sizeof(TEST_DATA[0]));

const ht_item_t TEST_DATA[15] = {
    {"Bitcoin", 53247.71}, {"Ethereum", 3208.67}, {"Binance Coin", 409.15},
    {"Cardano", 1.82},     {"Tether", 0.86},      {"XRP", 0.93},
    {"Solana", 134.50},    {"Polkadot", 34.99},   {"Dogecoin", 0.22},
    {"USD Coin", 0.86},    {"Uniswap", 21.68},    {"Terra", 30.67},
    {"Litecoin", 156.87},  {"Avalanche", 47.03},  {"Chainlink", 21.90}};

void init_test() {
  printf("Hash Table - testing script\n");
  printf("---------------------------\n");
  HT_SIZE = 13;
  printf("\nSetting HT_SIZE to prime number (%i)\n", HT_SIZE);
  printf("\n");
}

TEST(test_table_init, "Initialize the table")
ht_init(test_table);
ENDTEST

TEST(test_search_nonexist, "Search for a non-existing item")
ht_init(test_table);
assert( ht_search(test_table, "Ethereum") == NULL);
ENDTEST

TEST(test_insert_simple, "Insert a new item")
ht_init(test_table);
ht_insert(test_table, "Ethereum", 3208.67);
assert(strcmp(ht_search(test_table,"Ethereum")->key,"Ethereum") == 0 );
ENDTEST

TEST(test_search_exist, "Search for an existing item")
ht_init(test_table);
ht_insert(test_table, "Ethereum", 3208.67);
ht_item_t * test1 = ht_search(test_table, "Ethereum");
assert( strcmp(test1->key,"Ethereum") == 0 );
ENDTEST

TEST(test_insert_many, "Insert many new items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ENDTEST

TEST(test_search_collision, "Search for an item with colliding hash")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_item_t * test1 = ht_search(test_table, "Terra");
assert( strcmp(test1->key,"Terra") == 0 );
ENDTEST

TEST(test_insert_update, "Update an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_insert(test_table, "Ethereum", 12.34);
ht_item_t * test1 = ht_search(test_table, "Ethereum");
assert( fabs(test1->value - 12.34) <= (FLT_EPSILON * 1000) );
ENDTEST

TEST(test_get, "Get an item's value")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
assert( fabs( *ht_get(test_table, "Ethereum") - 3208.67) <= (FLT_EPSILON * 1000) );
ENDTEST

TEST(test_delete, "Delete an item")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete(test_table, "Terra");
assert( ht_search(test_table,"Terra") == NULL );
ENDTEST

TEST(test_delete_all, "Delete all the items")
ht_init(test_table);
INSERT_TEST_DATA(test_table)
ht_delete_all(test_table);
ENDTEST

int main(int argc, char *argv[]) {
  init_uninitialized_item();
  init_test();

  test_table_init();
  test_search_nonexist();
  test_insert_simple();
  test_search_exist();
  test_insert_many();
  test_search_collision();
  test_insert_update();
  test_get();
  test_delete();
  test_delete_all();

  free(uninitialized_item);
}
