#include "../include/AVLNode.h"

AVLNode::AVLNode(int id){ 
    this->id = id;
    height = 1;
    left = nullptr;
    right = nullptr;
}
int AVLNode::get_id() const { return id; }
int AVLNode::get_height() const { return height; }
AVLNode* AVLNode::get_left() const{ return left; }
AVLNode* AVLNode::get_right() const{ return right; }

void AVLNode::set_height(int height){ this->height = height; }
void AVLNode::set_left(AVLNode* node){ this->left = node; }
void AVLNode::set_right(AVLNode* node){ this->right = node; }
