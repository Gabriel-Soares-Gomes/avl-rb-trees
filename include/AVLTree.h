#ifndef AVLTREE_H
#define AVLTREE_H

#include "AVLNode.h"
#include <string>

class AVLTree {
    private:
        AVLNode* root;
        int rotations_count{0};
        int balance_count{0};

        int get_height(AVLNode* node) const;
        int get_balance(AVLNode* node) const;
        int get_bigger(int a, int b) const;
        AVLNode* find_min(AVLNode* node) const;
        AVLNode* recursive_insert(AVLNode* node, int id);
        AVLNode* recursive_remove(AVLNode* node, int id);
        void destroy(AVLNode* node);
        void recursive_print(AVLNode* node, const std::string& prefix, bool is_right) const;
        int recursive_count_nodes(AVLNode* node) const;
        bool validate_avl(AVLNode* node, int& h, long long min_val, long long max_val) const;

    public:
        AVLTree();
        ~AVLTree();

        AVLNode* get_root() const;
        AVLNode* rotate_right(AVLNode* node);
        AVLNode* rotate_left(AVLNode* node);
        int height() const;
        void insert(int id);
        void remove(int id);
        AVLNode* search(int id) const;
        void inorder() const;
        void inorder(AVLNode* node) const;

        std::string avl_label(int id, int height, int balance) const;
        void print_tree() const;
        void print_tree(AVLNode* node) const;

        void clear();
        void reset_stats();
        int get_rotations() const;
        int get_balance_count() const;
        int count_nodes() const;
};

#endif