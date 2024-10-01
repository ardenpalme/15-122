#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

typedef void* tree_entry_t;
typedef void* tree_key_t;

typedef bool (*key_cmp_func_t) (tree_key_t key1, tree_key_t key2);
typedef tree_key_t (*entry_key_func_t) (tree_entry_t entry);
typedef void (*print_func_t) (tree_entry_t entry);

typedef struct _tree_t
{
    int height;
    tree_entry_t data;
    struct _tree_t *left;
    struct _tree_t *right;
} tree_t;

tree_t *avl_tree_insert(tree_t *tree, 
                        tree_entry_t entry, 
                        entry_key_func_t entry_key_func, 
                        key_cmp_func_t key_cmp_func);

tree_entry_t avl_tree_lookup(tree_t *tree, 
                             tree_entry_t data, 
                             entry_key_func_t entry_key_func, 
                             key_cmp_func_t key_cmp_func);

void avl_tree_print(tree_t *tree, int depth, int thrsh, print_func_t print_func);

#endif /* __AVL_TREE_H__ */