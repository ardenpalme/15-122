#include <string>

/* Client-Supplied Definitions */
typedef void* tree_entry_t;
typedef uint64_t tree_key_t;
tree_key_t entry_key(tree_entry_t *entry);
int key_compare(tree_key_t k1, tree_key_t k2);

typedef struct _beer_t
{
    uint64_t batch_id;
    std::string beer_style;
    std::string sku;
    std::string location;
} beer_t;

/* Library Defintions */
typedef struct _tree_t
{
    int height;
    tree_entry_t data;
    struct _tree_t *left;
    struct _tree_t *right;
} tree_t;

tree_t *avl_tree_insert(tree_t *tree, tree_entry_t entry);
tree_entry_t avl_tree_lookup(tree_t *tree, tree_entry_t data);
void avl_tree_print(tree_t *tree, int depth, int thrsh);