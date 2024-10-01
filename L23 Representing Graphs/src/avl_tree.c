#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "avl_tree.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* ===== HELPER FUNCTIONS - BEGIN ==== */

int tree_height(tree_t *tree)
{
    return (tree != NULL) ? tree->height : 0;
}

void fix_height(tree_t *tree)
//@requires tree != NULL
//@requires is_specified_height(tree->left)
//@requires is_specified_height(tree->right)
{
    tree->height = MAX(tree_height(tree->left), tree_height(tree->right)) + 1;
}

tree_t *rotate_tree_left(tree_t *tree)
//@requires (tree != NULL) && (tree->right != NULL)
//@ensures is_specified_height(tree)
{
    tree_t *ret;

    ret = tree->right;
    tree->right = tree->right->left;
    ret->left = tree;

    // operation to ensure that the height member field is properly reflective of 
    // graph topology
    fix_height(ret);
    fix_height(tree);

    return ret;
}

tree_t *rotate_tree_right(tree_t *tree)
//@requires (tree != NULL) && (tree->left != NULL)
//@ensures is_specified_height(tree)
{
    tree_t *ret;

    if(tree == NULL) return NULL;

    ret = tree->left;
    tree->left = tree->left->right;
    ret->right = tree;

    fix_height(ret);
    fix_height(tree);

    return ret;
}

tree_t *avl_rebalance_left(tree_t *tree)
//@requires tree != NULL && tree->left != NULL
//@ensures is_avl(\result)
{
    if(abs(tree_height(tree->right) - tree_height(tree->left)) == 2){
        if(tree_height(tree->left->left) < tree_height(tree->left->right)){
            tree->left = rotate_tree_left(tree->left);
            tree = rotate_tree_right(tree);
        }else{
            tree = rotate_tree_right(tree);
        }
    }else{
        // rotation already incorporates height fix
        fix_height(tree); 
    }
    return tree;
}

tree_t *avl_rebalance_right(tree_t *tree)
//@requires tree != NULL && tree->right != NULL
//@ensures is_avl(\result)
{
    if(abs(tree_height(tree->right) - tree_height(tree->left)) == 2){
        if(tree_height(tree->right->right) < tree_height(tree->right->left)){
            tree->right = rotate_tree_right(tree->right); 
            tree = rotate_tree_left(tree);
        }else{
            tree = rotate_tree_left(tree);
        }
    }else{
        // rotation already incorporates height fix
        fix_height(tree); 
    }
    return tree;
}

/* ===== HELPER FUNCTIONS - END ==== */

tree_t *avl_tree_insert(tree_t *tree, 
                        tree_entry_t entry, 
                        entry_key_func_t entry_key_func, 
                        key_cmp_func_t key_cmp_func)
//@requires is_avl(tree)
//@ensures is_avl(\result)
{
    if(tree == NULL) {
        tree_t *ret = (tree_t*)calloc(1,sizeof(tree_t));
        ret->data = entry;
        ret->height = 1;
        return ret;
    }

    tree_key_t key1 = (*entry_key_func)(entry);
    tree_key_t key2 = (*entry_key_func)(tree->data);
    int cmp = (*key_cmp_func)(key1, key2);
    if(cmp == 0){
        tree->data = entry; // only changing stored data at that key location, valid dictionary operation
    }else if(cmp < 0){
        // update parent links recursively up the tree
        tree->left = avl_tree_insert(tree->left, entry, entry_key_func, key_cmp_func);
        tree = avl_rebalance_left(tree);
    }else if(cmp > 0){
        tree->right = avl_tree_insert(tree->right, entry, entry_key_func, key_cmp_func);
        tree = avl_rebalance_right(tree);
    }

    return tree;
}

tree_entry_t avl_tree_lookup(tree_t *tree, 
                             tree_entry_t entry, 
                             entry_key_func_t entry_key_func, 
                             key_cmp_func_t key_cmp_func)
//@requires is_avl(tree)
//@ensures \result == NULL || 
//         key_compare(key_entry(\result), key_entry(entry)) == 0
{
    if(tree == NULL) return NULL;

    tree_key_t key1 = (*entry_key_func)(entry);
    tree_key_t key2 = (*entry_key_func)(tree->data);
    int cmp = (*key_cmp_func)(key1, key2);
    if(cmp == 0){
        return tree->data;
    }else if(cmp < 0){
        return avl_tree_lookup(tree->left, entry, entry_key_func, key_cmp_func);
    }else{
        //@assert(cmp > 0)
        return avl_tree_lookup(tree->right, entry, entry_key_func, key_cmp_func);
    }
}

void avl_tree_print(tree_t *tree, int depth, int thrsh, print_func_t print_func)
{
    if(depth >= thrsh) return;

    for(int i=0; i<depth; i++) printf("|"); 
    printf("[%d]", tree->height);
    (*print_func)(tree->data);

    if(tree->right != NULL){
        avl_tree_print(tree->right,depth++, thrsh, print_func);
    }
    if(tree->left != NULL){
        avl_tree_print(tree->left, depth++, thrsh, print_func);
    }
}