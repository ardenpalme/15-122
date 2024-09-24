#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <iostream>

#include <avl_tree.hpp>

/* ==== CLIENT-PROVIDED FUNCTIONS - BEGIN ==== */

tree_key_t entry_key(tree_entry_t entry)
{
    beer_t *beer = (beer_t *)entry;
    return beer->batch_id;
}

int key_compare(tree_key_t k1, tree_key_t k2)
{
    if(k1 == k2) return 0;
    if(k1 > k2) return 1;
    if(k1 < k2) return -1;
}

/* ==== CLIENT-PROVIDED FUNCTIONS - END ==== */

/* ===== CONDITIONS - BEGIN ====== */

bool is_ordered(tree_t *tree, tree_entry_t lo, tree_entry_t hi) 
{
    if(tree == NULL) return true; // a NULL tree is a vaild empty tree
    
    int k = entry_key(tree->data);

    /* Pool the recursive case conditions together so that the return value propagates
     * from the leaf nodes */

    /* check bounds for local key on with per-tree scope, 
        this fundamental operation needs to take place */
    return ((lo == NULL) || key_compare(entry_key(lo), k) < 0) && // localized node check operation
           ((hi == NULL) || key_compare(k, entry_key(hi)) < 0) &&

           // left and right trees can be NULL for base case to be reached
           is_ordered(tree->left, lo, tree->data) && // graph traversal
           is_ordered(tree->right, tree->data, hi);
}

/* Is the height set in the tree member field reflective of the actual tree topology */
bool is_specified_height(tree_t *tree)
{
    if(tree == NULL) return true;

    int left_height = 0;
    int right_height = 0;
    if(tree->left != NULL) left_height = tree->left->height;
    if(tree->right != NULL) right_height = tree->right->height;

    /* Add the localized check in the pooled recursive check so that the
     * localized check is performed across all sub-graph visits */
    return (tree->height == std::max(left_height, right_height) + 1) &&
           is_specified_height(tree->right) &&
           is_specified_height(tree->left);
}

/* NOTE: the is_func() checks are part of pre/post conditions, and should not have any themselves */
bool is_balanced(tree_t *tree)
{
    /* We're going to be visiting NULL trees, even if we've already 
     * accounted for their height computation */
    if(tree == NULL) return true;

    int left_height = 0;
    int right_height = 0;
    if(tree->left != NULL) left_height = tree->left->height;
    if(tree->right != NULL) right_height = tree->right->height;

    /* Consider the heights of the sub-trees are those proper too? 
     * Requires recursive graph traversal */
    return (abs(left_height - right_height) <= 1) &&
            is_balanced(tree->left) &&
            is_balanced(tree->right);
}

/* Performs data validation check across all nodes in tree */
bool is_tree(tree_t *tree)
{
    if(tree == NULL) return true;
    return (tree->data != NULL) && // localized node check
           is_tree(tree->right) && // graph traversal
           is_tree(tree->left);
}

bool is_avl_tree(tree_t *tree)
{
    return is_tree(tree) && // data validation 
           is_ordered(tree, NULL, NULL) && // key-ordering invariant
           is_specified_height(tree) && // height validation 
           is_balanced(tree);
}

/* ===== CONDITIONS - END ====== */



/* ===== HELPER FUNCTIONS - BEGIN ==== */

/* This function could be used to set the heights of all nodes in tree, 
 * but it's overkill in conjunction with the post-condition
int get_height(tree_t *tree)
{
    if(tree == NULL) return 0;
    if(tree->left == NULL && tree->right == NULL) return 1;

    return MAX(get_height(tree->left), get_height(tree->right)) + 1;
}

void fix_height(tree_t *tree)
//@requires tree != NULL
//@requires is_specified_height(tree->left)
//@requires is_specified_height(tree->right)
{
    tree->height = get_height(tree);
}
*/

int tree_height(tree_t *tree)
{
    return (tree != NULL) ? tree->height : 0;
}

void fix_height(tree_t *tree)
//@requires tree != NULL
//@requires is_specified_height(tree->left)
//@requires is_specified_height(tree->right)
{
    tree->height = std::max(tree_height(tree->left), tree_height(tree->right)) + 1;
}

/* Go for the most specific pre and post conditions possible */
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
            beer_t *tmp =(beer_t *)tree;
            tree = rotate_tree_left(tree);
        }
    }else{
        // rotation already incorporates height fix
        fix_height(tree); 
    }
    return tree;
}

/* ===== HELPER FUNCTIONS - END ==== */

tree_t *avl_tree_insert(tree_t *tree, tree_entry_t entry)
//@requires is_avl(tree)
//@ensures is_avl(\result)
{
    if(tree == NULL) {
        tree_t *ret = new tree_t();
        ret->data = entry;
        ret->height = 1;
        return ret;
    }

    int cmp = key_compare(entry_key(entry), entry_key(tree->data));
    if(cmp == 0){
        tree->data = entry; // only changing stored data at that key location, valid dictionary operation
    }else if(cmp < 0){
        // update parent links recursively up the tree
        tree->left = avl_tree_insert(tree->left,entry);
        tree = avl_rebalance_left(tree);
    }else if(cmp > 0){
        tree->right = avl_tree_insert(tree->right,entry);
        tree = avl_rebalance_right(tree);
    }

    return tree;
}

tree_entry_t avl_tree_lookup(tree_t *tree, tree_entry_t entry)
//@requires is_avl(tree)
//@ensures \result == NULL || 
//         key_compare(key_entry(\result), key_entry(entry)) == 0
{
    if(tree == NULL) return NULL;

    int cmp = key_compare(entry_key(entry), entry_key(tree->data));
    if(cmp == 0){
        return tree->data;
    }else if(cmp < 0){
        return avl_tree_lookup(tree->left, entry);
    }else{
        //@assert(cmp > 0)
        return avl_tree_lookup(tree->right, entry);
    }
}

void avl_tree_print(tree_t *tree, int depth, int thrsh)
{
    if(depth == thrsh) return;
    beer_t *tmp = (beer_t*)tree->data;

    for(int i=0; i<depth; i++) std::cout<< "|"; 
    std::cout << "[" << tree->height << "] " << tmp->batch_id << std::endl;

    if(tree->right != NULL){
        //std::cout << "R ";
        avl_tree_print(tree->right,depth++, thrsh);
    }
    if(tree->left != NULL){
        //std::cout << "L ";
        avl_tree_print(tree->left, depth++, thrsh);
    }
}