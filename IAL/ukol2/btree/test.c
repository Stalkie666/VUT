#include "btree.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int base_data_count = 15;
const char base_keys[] = {'H', 'D', 'L', 'B', 'F', 'J', 'N', 'A',
                          'C', 'E', 'G', 'I', 'K', 'M', 'O'};
const int base_values[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 16};

const int additional_data_count = 6;
const char additional_keys[] = {'S', 'R', 'Q', 'P', 'X', 'Y', 'Z'};
const int additional_values[] = {10, 10, 10, 10, 10, 10};

const int traversal_data_count = 5;
const char traversal_keys[] = {'D', 'B', 'A', 'C', 'E'};
const int traversal_values[] = {1, 2, 3, 4, 5};

void init_test() {
  printf("Binary Search Tree - testing script\n");
  printf("-----------------------------------\n");
  printf("\n");
}

TEST(test_tree_init, "Initialize the tree")
bst_init(&test_tree);
ENDTEST

TEST(test_tree_dispose_empty, "Dispose the tree")
bst_init(&test_tree);
bst_dispose(&test_tree);
ENDTEST

TEST(test_tree_search_empty, "Search in an empty tree (A)")
bst_init(&test_tree);
int result;
assert( bst_search(test_tree, 'A', &result) == false);
ENDTEST

TEST(test_tree_insert_root, "Insert an item (H,1)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
int tmp;
assert( bst_search(test_tree,'H',&tmp) && tmp == 1 );
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search_root, "Search in a single node tree (H)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
int result;
assert( bst_search(test_tree, 'H', &result) && result == 1 );
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_update_root, "Update a node in a single node tree (H,1)->(H,8)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
int result;
assert( bst_search(test_tree, 'H', &result) && result == 1 );
bst_print_tree(test_tree);
bst_insert(&test_tree, 'H', 8);
assert( bst_search(test_tree, 'H', &result) && result == 8 );
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_insert_many, "Insert many values")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search, "Search for an item deeper in the tree (A)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
assert( bst_search(test_tree, 'A', &result) && result == 1);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search_missing, "Search for a missing key (X)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
assert( !bst_search(test_tree, 'X', &result) );
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_leaf, "Delete a leaf node (A)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
int tmp;
assert( bst_search(test_tree,'A',&tmp) && tmp == 1);
bst_delete(&test_tree, 'A');
assert( !bst_search(test_tree,'A',&tmp) );
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_left_subtree, "Delete a node with only left subtree (R)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);
bst_print_tree(test_tree);
int tmp;
assert( bst_search(test_tree,'R',&tmp) && tmp == 10 );
bst_delete(&test_tree, 'R');
assert( !bst_search(test_tree,'R',&tmp));
assert( bst_search(test_tree,'X',&tmp) && tmp == 10);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_right_subtree,
     "Delete a node with only right subtree (X)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);

bst_print_tree(test_tree);
int tmp;
assert( bst_search(test_tree,'X',&tmp) && tmp == 10);
bst_delete(&test_tree, 'X');
assert( !bst_search(test_tree,'X',&tmp));
assert( bst_search(test_tree,'R',&tmp) && tmp == 10);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_both_subtrees, "Delete a node with both subtrees (L)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);

bst_print_tree(test_tree);
int tmp;
assert( bst_search(test_tree,'L',&tmp) && tmp == 12);
bst_delete(&test_tree, 'L');
assert( !bst_search(test_tree,'L',&tmp));
assert( bst_search(test_tree,'R',&tmp) && tmp == 10);
assert( bst_search(test_tree,'H',&tmp) && tmp == 8);
assert( bst_search(test_tree,'O',&tmp) && tmp == 16);
assert( bst_search(test_tree,'Q',&tmp) && tmp == 10);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_missing, "Delete a node that doesn't exist (U)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
int tmp;
assert( !bst_search(test_tree,'U',&tmp));
bst_delete(&test_tree, 'U');
assert( !bst_search(test_tree,'U',&tmp));
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_root, "Delete the root node (H)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
int tmp;
assert( bst_search(test_tree,'H',&tmp) && tmp == 8);
bst_delete(&test_tree, 'H');
assert( !bst_search(test_tree,'H',&tmp));
assert( bst_search(test_tree,'D',&tmp) && tmp == 4);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_dispose_filled, "Dispose the whole tree")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_dispose(&test_tree);
assert( test_tree == NULL );
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_preorder, "Traverse the tree using preorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_preorder(test_tree, test_items);
bst_print_tree(test_tree);
bst_print_items(test_items);
ENDTEST

TEST(test_tree_inorder, "Traverse the tree using inorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_inorder(test_tree, test_items);
bst_print_tree(test_tree);
bst_print_items(test_items);
ENDTEST

TEST(test_tree_postorder, "Traverse the tree using postorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_postorder(test_tree, test_items);
bst_print_tree(test_tree);
bst_print_items(test_items);
ENDTEST

#ifdef EXA

TEST(test_letter_count, "Count letters");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCc_ 123 *");
//letter_count(&test_tree, " _123abBcCc *");
//letter_count(&test_tree, "abdawdgtgcdefadawdghijadawklmnopdsfsgrstuvdawdawwxyz123124");
bst_print_tree(test_tree);
ENDTEST

TEST(test_balance, "Count letters and balance");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCc_ 123 *");
//letter_count(&test_tree, " _123abBcCc *");
//letter_count(&test_tree, "abdawdgtgcdefadawdghijadawklmnopdsfsgrstuvdawdawwxyz123124");
bst_balance(&test_tree);
bst_print_tree(test_tree);
ENDTEST

#endif // EXA

int main(int argc, char *argv[]) {
  init_test();

  test_tree_init();
  test_tree_dispose_empty();
  test_tree_search_empty();
  test_tree_insert_root();
  test_tree_search_root();
  test_tree_update_root();
  test_tree_insert_many();
  test_tree_search();
  test_tree_search_missing();
  test_tree_delete_leaf();
  test_tree_delete_left_subtree();
  test_tree_delete_right_subtree();
  test_tree_delete_both_subtrees();
  test_tree_delete_missing();
  test_tree_delete_root();
  test_tree_dispose_filled();
  test_tree_preorder();
  test_tree_inorder();
  test_tree_postorder();

#ifdef EXA
  test_letter_count();
  test_balance();
#endif // EXA
}
