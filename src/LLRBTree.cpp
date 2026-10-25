#include "../include/LLRBTree.h"
#include <stdexcept>
#include <iostream>

LLRBTree::LLRBTree() : root{ nullptr }, rotations_count{ 0 }, flip_colors_count{ 0 }, balance_count{ 0 } {}

LLRBTree::~LLRBTree() {
    destroy(root);
}

void LLRBTree::destroy(LLRBNode* node) {
    if (node == nullptr) return;
    destroy(node->get_left());
    destroy(node->get_right());
    delete node;
}

void LLRBTree::clear() {
    destroy(root);
    root = nullptr;
    reset_stats();
}

void LLRBTree::reset_stats() {
    rotations_count = 0;
    flip_colors_count = 0;
    balance_count = 0;
}

int LLRBTree::get_rotations() const {
    return rotations_count;
}

int LLRBTree::get_flip_colors() const {
    return flip_colors_count;
}

int LLRBTree::get_balance_count() const {
    return balance_count;
}

LLRBNode* LLRBTree::get_root() const {
    return root;
}

bool LLRBTree::is_empty() const {
    return root == nullptr;
}

int LLRBTree::get_height(LLRBNode* node) const {
    if (node == nullptr) return 0;
    int left_h = get_height(node->get_left());
    int right_h = get_height(node->get_right());
    return 1 + (left_h > right_h ? left_h : right_h);
}

int LLRBTree::height() const {
    return get_height(root);
}

int LLRBTree::recursive_count_nodes(LLRBNode* node) const {
    if (node == nullptr) return 0;
    return 1 + recursive_count_nodes(node->get_left()) + recursive_count_nodes(node->get_right());
}

int LLRBTree::count_nodes() const {
    return recursive_count_nodes(root);
}

bool LLRBTree::is_red(LLRBNode* node) const {
    if (node == nullptr) return false;
    return node->get_color() == RED;
}

LLRBNode* LLRBTree::rotate_left(LLRBNode* node) {
    rotations_count++;
    LLRBNode* x = node->get_right();
    node->set_right(x->get_left());
    x->set_left(node);
    x->set_color(node->get_color());
    node->set_color(RED);
    return x;
}

LLRBNode* LLRBTree::rotate_right(LLRBNode* node) {
    rotations_count++;
    LLRBNode* x = node->get_left();
    node->set_left(x->get_right());
    x->set_right(node);
    x->set_color(node->get_color());
    node->set_color(RED);
    return x;
}

void LLRBTree::flip_colors(LLRBNode* node) {
    flip_colors_count++;
    node->set_color(node->get_color() == RED ? BLACK : RED);
    if (node->get_left() != nullptr) {
        node->get_left()->set_color(node->get_left()->get_color() == RED ? BLACK : RED);
    }
    if (node->get_right() != nullptr) {
        node->get_right()->set_color(node->get_right()->get_color() == RED ? BLACK : RED);
    }
}

LLRBNode* LLRBTree::balance(LLRBNode* node) {
    bool did_balance = false;

    if (is_red(node->get_right()) && !is_red(node->get_left())) {
        node = rotate_left(node);
        did_balance = true;
    }

    if (is_red(node->get_left()) && is_red(node->get_left()->get_left())) {
        node = rotate_right(node);
        did_balance = true;
    }

    if (is_red(node->get_left()) && is_red(node->get_right())) {
        flip_colors(node);
        did_balance = true;
    }

    if (did_balance) {
        balance_count++;
    }

    return node;
}

LLRBNode* LLRBTree::recursive_insert(LLRBNode* node, int id) {
    if (node == nullptr) {
        return new LLRBNode(id, RED);
    }

    if (id < node->get_id()) {
        node->set_left(recursive_insert(node->get_left(), id));
    } else if (id > node->get_id()) {
        node->set_right(recursive_insert(node->get_right(), id));
    } else {
        throw std::invalid_argument("Erro: Já existe encomenda com o ID: " + std::to_string(id));
    }

    return balance(node);
}

void LLRBTree::insert(int id) {
    root = recursive_insert(root, id);
    root->set_color(BLACK);
}

LLRBNode* LLRBTree::move_red_left(LLRBNode* node) {
    flip_colors(node);
    if (node->get_right() != nullptr && is_red(node->get_right()->get_left())) {
        node->set_right(rotate_right(node->get_right()));
        node = rotate_left(node);
        flip_colors(node);
    }
    return node;
}

LLRBNode* LLRBTree::move_red_right(LLRBNode* node) {
    flip_colors(node);
    if (node->get_left() != nullptr && is_red(node->get_left()->get_left())) {
        node = rotate_right(node);
        flip_colors(node);
    }
    return node;
}

LLRBNode* LLRBTree::find_min(LLRBNode* node) const {
    LLRBNode* current = node;
    while (current->get_left() != nullptr) {
        current = current->get_left();
    }
    return current;
}

LLRBNode* LLRBTree::delete_min(LLRBNode* node) {
    if (node->get_left() == nullptr) {
        LLRBNode* r = node->get_right();
        delete node;
        return r;
    }

    if (!is_red(node->get_left()) && !is_red(node->get_left()->get_left())) {
        node = move_red_left(node);
    }

    node->set_left(delete_min(node->get_left()));
    return balance(node);
}

LLRBNode* LLRBTree::recursive_remove(LLRBNode* node, int id) {
    if (node == nullptr) return nullptr;

    if (id < node->get_id()) {
        if (node->get_left() != nullptr) {
            if (!is_red(node->get_left()) && !is_red(node->get_left()->get_left())) {
                node = move_red_left(node);
            }
            node->set_left(recursive_remove(node->get_left(), id));
        }
    } else {
        if (is_red(node->get_left())) {
            node = rotate_right(node);
        }

        if (id == node->get_id() && (node->get_right() == nullptr)) {
            delete node;
            return nullptr;
        }

        if (node->get_right() != nullptr) {
            if (!is_red(node->get_right()) && !is_red(node->get_right()->get_left())) {
                node = move_red_right(node);
            }

            if (id == node->get_id()) {
                LLRBNode* min_node = find_min(node->get_right());
                node->set_id(min_node->get_id());
                node->set_right(delete_min(node->get_right()));
            } else {
                node->set_right(recursive_remove(node->get_right(), id));
            }
        }
    }

    return balance(node);
}

void LLRBTree::remove(int id) {
    if (root == nullptr || search(id) == nullptr) {
        return;
    }

    // Se ambos os filhos da raiz forem pretos, define a raiz como vermelha temporariamente
    if (!is_red(root->get_left()) && !is_red(root->get_right())) {
        root->set_color(RED);
    }

    root = recursive_remove(root, id);

    if (root != nullptr) {
        root->set_color(BLACK);
    }
}

LLRBNode* LLRBTree::search(int id) const {
    LLRBNode* current = root;
    while (current != nullptr) {
        int current_id = current->get_id();
        if (id == current_id) {
            return current;
        } else if (id < current_id) {
            current = current->get_left();
        } else {
            current = current->get_right();
        }
    }
    return nullptr;
}

void LLRBTree::recursive_inorder(LLRBNode* node) const {
    if (node == nullptr) return;
    recursive_inorder(node->get_left());
    std::cout << node->get_id() << " ";
    recursive_inorder(node->get_right());
}

void LLRBTree::inorder() const {
    recursive_inorder(root);
    std::cout << "\n";
}

void LLRBTree::inorder(LLRBNode* node) const {
    recursive_inorder(node);
}

std::string LLRBTree::llrb_label(const LLRBNode* node) const {
    return std::to_string(node->get_id()) + (node->get_color() == RED ? " (R)" : " (B)");
}

void LLRBTree::recursive_print(LLRBNode* node, const std::string& prefix, bool is_right, const std::string& /*branch_label*/) const {
    if (node == nullptr) return;

    recursive_print(node->get_right(), prefix + (is_right ? "    " : "│   "), true, "");

    std::cout << prefix << (is_right ? "┌── " : "└── ") << llrb_label(node) << "\n";

    recursive_print(node->get_left(), prefix + (is_right ? "│   " : "    "), false, "");
}

void LLRBTree::print_tree() const {
    if (root == nullptr) {
        std::cout << "(Árvore Rubro-Negra vazia)\n";
        return;
    }
    print_tree(root);
}

void LLRBTree::print_tree(LLRBNode* node) const {
    if (node == nullptr) {
        std::cout << "(Subárvore vazia)\n";
        return;
    }
    const std::string indent = "    ";
    recursive_print(node->get_right(), indent, true, "");
    std::cout << llrb_label(node) << "\n";
    recursive_print(node->get_left(), indent, false, "");
    std::cout << "\nLeitura: raiz à esquerda | filhos direitos acima | filhos esquerdos abaixo\n"
              << "Cores: (R) = vermelho | (B) = preto\n";
}
