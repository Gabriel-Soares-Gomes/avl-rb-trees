#include "../include/LLRBNode.h"

LLRBNode::LLRBNode(int id, Color color) {
    this->id = id;
    this->color = color;
    this->left = nullptr;
    this->right = nullptr;
}

int LLRBNode::get_id() const { return id; }
Color LLRBNode::get_color() const { return color; }
LLRBNode* LLRBNode::get_left() const { return left; }
LLRBNode* LLRBNode::get_right() const { return right; }

void LLRBNode::set_id(int id) { this->id = id; }
void LLRBNode::set_color(Color color) { this->color = color; }
void LLRBNode::set_left(LLRBNode* node) { this->left = node; }
void LLRBNode::set_right(LLRBNode* node) { this->right = node; }

bool LLRBNode::is_red() const { return color == RED; }
