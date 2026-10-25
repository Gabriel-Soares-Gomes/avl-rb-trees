#ifndef LLRBNODE_H
#define LLRBNODE_H

enum Color {
    RED,
    BLACK
};

class LLRBNode {
private:
    int id;
    Color color;
    LLRBNode* left;
    LLRBNode* right;

public:
    LLRBNode(int id, Color color = RED);

    int get_id() const;
    Color get_color() const;
    LLRBNode* get_left() const;
    LLRBNode* get_right() const;

    void set_id(int id);
    void set_color(Color color);
    void set_left(LLRBNode* node);
    void set_right(LLRBNode* node);

    bool is_red() const;
};

#endif
