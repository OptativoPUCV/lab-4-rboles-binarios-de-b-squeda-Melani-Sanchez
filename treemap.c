#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * newNode = createTreeNode(key, value);
    if (tree->root == NULL) {
        tree->root = newNode;
        return;
    }
    
    TreeNode * aux = tree->root;
    while (1) {
        if (tree->lower_than(key, aux->pair->key)) {
            if (aux->left == NULL) {
                aux->left = newNode;
                newNode->parent = aux;
                tree->current = newNode;
                return;
            }
            aux = aux->left;
        }
        else if (tree->lower_than(aux->pair->key, key)) {
            if (aux->right == NULL) {
                aux->right = newNode;
                newNode->parent = aux;
                tree->current = newNode;
                return;
            }
            aux = aux->right;
        }
        else {
            free(newNode);
            return;
        }
    }
}

TreeNode * minimum(TreeNode * x) {
    while (x->left != NULL) {
        x = x->left;
    }
    
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;
    
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;
        }
        else if (node->parent->left == node) {
            node->parent->left = NULL;
        }
        else node->parent->right = NULL;
        free(node);
        return;
    }
        
    else if (node->left == NULL || node->right == NULL) {
        TreeNode *hijo = (node->left == NULL) ? node->right : node->left;

        if (node->parent == NULL) tree->root = hijo;
        else if (node->parent->left == node) node->parent->left = hijo;
        else node->parent->right = hijo;

        hijo->parent = node->parent;
        free(node);
        return;
    }
    TreeNode *min = minimum(node->right);
    node->pair = min->pair;
    removeNode(tree, min);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    while (aux != NULL) {
        if (tree->lower_than(key, aux->pair->key)) {
            aux = aux->left;
        }
        else if (tree->lower_than(aux->pair->key, key)) {
            aux = aux->right;
        }
        else if (is_equal(tree, key, aux->pair->key)) {
            tree->current = aux;
            return aux->pair;
        }
    }
    
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    TreeNode *upper = NULL;
    
    while (aux != NULL) {
        if (tree->lower_than(aux->pair->key, key)) aux = aux->right;
        else {
            upper = aux;
            aux = aux->left;
        }
    }
    if (upper != NULL) {
        tree->current = upper;
        return upper->pair;
    }
    
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode *aux = tree->root;
    while (aux->left != NULL) {
        aux = aux->left;
    }
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    if (tree->current->right != NULL) {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    
    TreeNode *aux = tree->current;
    while (aux->parent != NULL && aux->parent->right == aux) {
        aux = aux->parent;
    }
    
    if (aux->parent == NULL) return NULL;
    tree->current = aux->parent;
    return tree->current->pair;
}
