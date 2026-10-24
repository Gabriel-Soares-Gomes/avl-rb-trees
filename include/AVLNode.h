#ifndef AVLNODE_H
#define AVLNODE_H

class AVLNode{
    private:
        int id;
        int height;
        AVLNode* left;
        AVLNode* right;

    public:

        AVLNode(int id);

        int get_id() const;
        int get_height() const;
        AVLNode* get_left() const;
        AVLNode* get_right() const;

        void set_id(int id);
        void set_height(int height);
        void set_left(AVLNode* node);
        void set_right(AVLNode* node);
};

#endif