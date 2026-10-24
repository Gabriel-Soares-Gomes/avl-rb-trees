#ifndef AVLTREE_H
#define AVLTREE_H

#include "AVLNode.h"

class AVLTree {
    private:
        AVLNode* root;

        int get_height(AVLNode* node) const;
        int get_balance(AVLNode* node) const;
        int get_bigger(int a, int b) const;
        AVLNode* find_min(AVLNode* node) const;
        AVLNode* recursive_insert(AVLNode* node, int id);
        AVLNode* recursive_remove(AVLNode* node, int id);

    public:
        AVLTree();
        AVLNode* get_root() const;
        AVLNode* rotate_right(AVLNode* node);
        AVLNode* rotate_left(AVLNode* node);
        int height() const;
        void insert(int id);
        void remove(int id);
        AVLNode* search(int id) const;
        void inorder(AVLNode* node) const;

};

#endif