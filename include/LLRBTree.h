#ifndef LLRBTREE_H
#define LLRBTREE_H

#include "LLRBNode.h"
#include <string>

class LLRBTree {
private:
    LLRBNode* root;
    int rotations_count{0};
    int flip_colors_count{0};
    int balance_count{0};

    bool is_red(LLRBNode* node) const;
    LLRBNode* rotate_left(LLRBNode* h);
    LLRBNode* rotate_right(LLRBNode* h);
    void flip_colors(LLRBNode* h);

    LLRBNode* recursive_insert(LLRBNode* h, int id);
    LLRBNode* recursive_remove(LLRBNode* h, int id);
    LLRBNode* delete_min(LLRBNode* h);
    LLRBNode* move_red_left(LLRBNode* h);
    LLRBNode* move_red_right(LLRBNode* h);
    LLRBNode* balance(LLRBNode* h);
    LLRBNode* find_min(LLRBNode* node) const;

    int get_height(LLRBNode* node) const;
    int recursive_count_nodes(LLRBNode* node) const;
    bool validate_bst(LLRBNode* node, long long min_val, long long max_val) const;
    int check_black_height(LLRBNode* node, bool& is_valid) const;

    void recursive_inorder(LLRBNode* node) const;
    std::string llrb_label(const LLRBNode* node) const;
    void recursive_print(LLRBNode* node, const std::string& prefix, bool is_tail, const std::string& branch_label) const;
    void destroy(LLRBNode* node);

public:
    LLRBTree();
    ~LLRBTree();

    LLRBNode* get_root() const;
    bool is_empty() const;
    int height() const;

    // Métodos principais requisitados
    void insert(int id);
    void remove(int id);
    LLRBNode* search(int id) const;
    void inorder() const;
    void inorder(LLRBNode* node) const;

    void print_tree() const;
    void print_tree(LLRBNode* node) const;

    void clear();
    void reset_stats();
    int get_rotations() const;
    int get_flip_colors() const;
    int get_balance_count() const;
    int count_nodes() const;
};

#endif // LLRBTREE_H
