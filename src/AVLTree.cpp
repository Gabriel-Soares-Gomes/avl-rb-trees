#include "../include/AVLTree.h"
#include <iostream>
#include <stdexcept>
#include <string>


AVLTree::AVLTree() : root{ nullptr } {}


AVLNode* AVLTree::get_root() const {
    return root;
}


int AVLTree::get_height(AVLNode* node) const{
    if(node == nullptr) return 0;
    return node->get_height();
}


int AVLTree::get_balance(AVLNode* node) const {
    if(node == nullptr) return 0;
    return (get_height(node->get_left())) - (get_height(node->get_right())); 
}


int AVLTree::get_bigger(int a, int b) const {
    return (a > b) ? a : b;
}


AVLNode* AVLTree::rotate_right(AVLNode* node){

    AVLNode* new_parent{ node->get_left() };
    AVLNode* grandson{ new_parent->get_right() };

    new_parent->set_right(node);
    node->set_left(grandson);
    
    node->set_height(1 + get_bigger(get_height(node->get_left()), get_height(node->get_right())));
    new_parent->set_height(1 + get_bigger(get_height(new_parent->get_left()), get_height(new_parent->get_right())));

    return new_parent;
}


AVLNode* AVLTree::rotate_left(AVLNode* node){

    AVLNode* new_parent{ node->get_right() };
    AVLNode* grandson{ new_parent->get_left() };

    new_parent->set_left(node);
    node->set_right(grandson);

    node->set_height(1 + get_bigger(get_height(node->get_left()), get_height(node->get_right())));

    new_parent->set_height(1 + get_bigger(get_height(new_parent->get_left()), get_height(new_parent->get_right())));

    return new_parent;
}


AVLNode* AVLTree::recursive_insert(AVLNode* node, int id){
    if(node == nullptr) return new AVLNode(id);

    if(id < node->get_id()){
        node->set_left(recursive_insert(node->get_left(), id));
    }
    else if(id > node->get_id()){
        node->set_right(recursive_insert(node->get_right(), id));
    }
    else{
        throw std::invalid_argument("Erro: Já existe encomenda com o ID: " + std::to_string(node->get_id()));
    }

    node->set_height(1 + get_bigger(get_height(node->get_left()), get_height(node->get_right())));

    int balance{ get_balance(node) };

    if(balance > 1){
        if(id < node->get_left()->get_id()){
            return rotate_right(node);
        }
        else if(id > node->get_left()->get_id()){
            node->set_left(rotate_left(node->get_left()));
            return rotate_right(node);
        }
    }
    else if(balance < -1){
        if(id > node->get_right()->get_id()){
            return rotate_left(node);
        }
        else if(id < node->get_right()->get_id()){
            node->set_right(rotate_right(node->get_right()));
            return rotate_left(node);
        }
    }

    return node;
    
}


void AVLTree::insert(int id){
    root = recursive_insert(root, id);
}


AVLNode* AVLTree::search(int id) const {
    AVLNode* current{ root };

    while( current != nullptr ){

        int current_id{ current->get_id()};
        if (id == current_id) return current;
        if(id < current_id) current = current->get_left();
        else if (id > current_id) current = current->get_right();
    }
    
    return nullptr;
}


void AVLTree::inorder(AVLNode* node) const {

    if(node == nullptr) return;

    inorder(node->get_left());
    std::cout << node->get_id() << " ";
    inorder(node->get_right());

}

AVLNode* AVLTree::find_min(AVLNode* node) const {
    AVLNode* current{ node };

    while(current->get_left() != nullptr){
        current = current->get_left();
    }

    return current;
}


AVLNode* AVLTree::recursive_remove(AVLNode* node, int id){
    if(node == nullptr) return node;

    if(id < node->get_id()){
        node->set_left(recursive_remove(node->get_left(), id));
    }
    else if(id > node->get_id()){
        node->set_right(recursive_remove(node->get_right(), id));
    }
    else{
        if(node->get_left() == nullptr || node->get_right() == nullptr){
            AVLNode* temp{ node->get_left() ? node->get_left() : node->get_right() };

            if(temp == nullptr){
                temp = node;
                node = nullptr;
            }
            else{
                *node = *temp;
            }

            delete temp;
        }
        else{
            AVLNode* temp{ find_min(node->get_right()) };
            node->set_id(temp->get_id());
            node->set_right(recursive_remove(node->get_right(), temp->get_id()));
        }
    }

    if(node == nullptr) return node;

    node->set_height(1 + get_bigger(get_height(node->get_left()), get_height(node->get_right())));

    int balance{ get_balance(node) };

    if(balance > 1){
        if(get_balance(node->get_left()) >= 0){
            return rotate_right(node);
        }
        else{
            node->set_left(rotate_left(node->get_left()));
            return rotate_right(node);
        }
    }
    else if(balance < -1){
        if(get_balance(node->get_right()) <= 0){
            return rotate_left(node);
        }
        else{
            node->set_right(rotate_right(node->get_right()));
            return rotate_left(node);
        }
    }

    return node;
}
