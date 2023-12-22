#ifndef DATA_STRUCTURES_2_WET_2_AVLTREE_H
#define DATA_STRUCTURES_2_WET_2_AVLTREE_H

#include <iostream>
#include <memory>

#define SPACE 10

template <class T, class Cond>
class Node {
public:
    T value;
    double subTree;
    std::shared_ptr<Node<T, Cond>> left;
    std::shared_ptr<Node<T, Cond>> right;

    Node(): value() {
        left = nullptr;
        right = nullptr;
        subTree = 0;
    }

    Node(T t) : value(t){
        left = nullptr;
        right = nullptr;
        subTree = 0;
    }

    int get_subTree() const{
        return subTree;
    }

    void set_subTree(int set_val){
        subTree = set_val;
    }
};

template <class T, class Cond>
class AVLTree {
private:

public:
    std::shared_ptr<Node<T, Cond>> r;

    AVLTree() {
        r = nullptr;
    }

    void fix(std::shared_ptr<Node<T, Cond>> root) {
        root->right ? root->right->subTree = 1 + ((root->right->right) ? root->right->right->subTree : 0) + (root->right->left ? root->right->left->subTree : 0) : 0;
        root->left ? root->left->subTree = 1 + ((root->left->left) ? root->left->left->subTree : 0) + (root->left->right ? root->left->right->subTree : 0) : 0;
    }

    double getSum(std::shared_ptr<Node<T, Cond>> root, Node<T, Cond> val) {
        std::shared_ptr<Node<T, Cond>> current = root;
        double sum = 0;
        /* loop down to find the node */
        Cond c = Cond();
        while (current != nullptr) {
            if (c(val.value, current->value)) { //val < root -> value >> left turn
                sum += current->subTree;
                current = current->left;
            }
                // If the key is greater than the root's key, then it lies in right subtree
            else if (c(current->value, val.value)) { //val > root -> value >> right turn
                sum += current->subTree;
                current = current->right;
            }
                // if key is same as root's key, then This is the node to be deleted
            else {
                sum += current->subTree;
                return sum;
            }
        }

        return sum;
    }

    int height(std::shared_ptr<Node<T, Cond>> t_node) const {
        if (t_node == nullptr)
            return -1;

        int lheight = height(t_node->left);
        int rheight = height(t_node->right);

        if (lheight > rheight)
            return (lheight + 1);
        else return (rheight + 1);
    }

    int getBalanceFactor(std::shared_ptr<Node<T, Cond>> t_node) const {
        if (t_node == nullptr)
            return -1;
        return height(t_node->left) - height(t_node->right);
    }

    std::shared_ptr<Node<T, Cond>> rightRotate(std::shared_ptr<Node<T, Cond>> y) {
        std::shared_ptr<Node<T, Cond>> x = y->left;
        std::shared_ptr<Node<T, Cond>> T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        return x;
    }

    std::shared_ptr<Node<T, Cond>> leftRotate(std::shared_ptr<Node<T, Cond>> x) {
        std::shared_ptr<Node<T, Cond>> y = x->right;
        std::shared_ptr<Node<T, Cond>> T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        return y;
    }

    std::shared_ptr<Node<T, Cond>> insert(std::shared_ptr<Node<T, Cond>> root, Node<T, Cond> new_node) {
        if (r == nullptr) {
            std::shared_ptr<Node<T, Cond>> ptr = std::shared_ptr<Node<T, Cond>>(new Node<T, Cond>(new_node));
            r = ptr;
            return r;
        }

        if (root == nullptr) {
            std::shared_ptr<Node<T, Cond>> ptr = std::shared_ptr<Node<T, Cond>>(new Node<T, Cond>(new_node));
            root = ptr;

            double sum = getSum(r, root->value);
            if (sum == 0)
                root->subTree = sum;
            else
                root->subTree = -sum;
            return root;
        }

        Cond c = Cond();
        if (c(new_node.value, root->value)) { //new_node -> value < root -> value
            //root->subTree += 1;
            root->left = insert(root->left, new_node);

        }
        else if (c(root->value, new_node.value)) { //new_node -> value > root -> value
            //root->subTree += 1;
            root->right = insert(root->right, new_node);
        }
        else {
            // duplicate values, new_node -> value = root -> value
            return root;
        }


        int bf = getBalanceFactor(root);
        // Left Left Case
        if (bf > 1 && c(new_node.value, root->left->value)) { //bf > 1 && new_node -> value < root -> left -> value
            std::shared_ptr<Node<T, Cond>> y = root;
            std::shared_ptr<Node<T, Cond>> x = y->left;
            std::shared_ptr<Node<T, Cond>> T2 = x->right;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += x->subTree;
            x->subTree += y->subTree;
            y->subTree -= x->subTree;
            root = rightRotate(root);

            return root;
        }

        // Right Right Case
        if (bf < -1 && c(root->right->value, new_node.value)) { //bf < -1 && new_node -> value > root -> right -> value
            std::shared_ptr<Node<T, Cond>> x = root;
            std::shared_ptr<Node<T, Cond>> y = x->right;
            std::shared_ptr<Node<T, Cond>> T2 = y->left;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += y->subTree;
            y->subTree += x->subTree;
            x->subTree -= y->subTree;

            root = leftRotate(root);

            return root;
        }

        // Left Right Case
        if (bf > 1 && c(root->left->value, new_node.value)) { //bf > 1 && new_node -> value > root -> left -> value
            std::shared_ptr<Node<T, Cond>> x = root->left;
            std::shared_ptr<Node<T, Cond>> y = x->right;
            std::shared_ptr<Node<T, Cond>> T2 = y->left;
            if (T2)
                T2->subTree += y->subTree;
            y->subTree += x->subTree;
            x->subTree -= y->subTree;

            root->left = leftRotate(root->left);

            y = root;
            x = y->left;
            T2 = x->right;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += x->subTree;
            x->subTree += y->subTree;
            y->subTree -= x->subTree;
            root = rightRotate(root);

            return root;
        }

        // Right Left Case
        if (bf < -1 && c(new_node.value, root->right->value)) {//bf < -1 && new_node -> value < root -> right -> value
            std::shared_ptr<Node<T, Cond>> y = root->right;
            std::shared_ptr<Node<T, Cond>> x = y->left;
            std::shared_ptr<Node<T, Cond>> T2 = x->right;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += x->subTree;
            //x->subTree -= y->subTree;
            x->subTree += y->subTree;
            //y->subTree += x->subTree;
            y->subTree -= x->subTree;

            root->right = rightRotate(root->right);

            x = root;
            y = x->right;
            T2 = y->left;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += y->subTree;
            y->subTree += x->subTree;
            x->subTree -= y->subTree;

            root = leftRotate(root);

            return root;
        }

        /* return the (unchanged) node pointer */
        return root;
    }

    std::shared_ptr<Node<T, Cond>> minValueNode(std::shared_ptr<Node<T, Cond>> node) const {
        std::shared_ptr<Node<T, Cond>> current = node;
        /* loop down to find the leftmost leaf */
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    std::shared_ptr<Node<T, Cond>> maxValueNode(std::shared_ptr<Node<T, Cond>> node) const {
        std::shared_ptr<Node<T, Cond>> current = node;
        /* loop down to find the rightmost leaf */
        while (current->right != nullptr) {
            current = current->right;
        }
        return current;
    }

    std::shared_ptr<Node<T, Cond>> Delete(std::shared_ptr<Node<T, Cond>> root, T val) {
        Cond c = Cond();

        // base case (if node not found)
        if (root == nullptr) {
            return nullptr;
        }

            // If the key to be deleted is smaller than the root's key, then it lies in left subtree
        else if (c(val, root->value)) { //val < root -> value
            //root->subTree -= 1;
            root->left = Delete(root->left, val);
        }
            // If the key to be deleted is greater than the root's key, then it lies in right subtree
        else if (c(root->value, val)) { //val > root -> value
            //root->subTree -= 1;
            root->right = Delete(root->right, val);
        }
            // if key is same as root's key, then This is the node to be deleted
        else {
            // node with only one child or no child
            if (root->left == nullptr) {
                std::shared_ptr<Node<T, Cond>> temp = root->right;

                if (temp != nullptr)
                    temp->subTree += root->subTree;

                if (root->right == nullptr) {
                    //root->value = nullptr;////////////////////////////////////////////////////////////////////////
                    root = nullptr;
                }
                return temp;
            } else if (root->right == nullptr) {
                std::shared_ptr<Node<T, Cond>> temp = root->left;
                temp->subTree += root->subTree;
                return temp;
            } else {
                // node with two children: Get the inorder successor (smallest in the right subtree)
                std::shared_ptr<Node<T, Cond>> temp = minValueNode(root->right);
                double prev = root->subTree;
                double sum = getSum(root, temp->value);
                temp->subTree = sum;

                // Copy the inorder successor's content to this node
                root->value = temp->value;
                // Delete the inorder successor
                //root->subTree-=1;

                root->right->subTree -= (sum-prev);
                root->left->subTree -= (sum-prev);
                root->right = Delete(root->right, temp->value);
            }
        }

        int bf = getBalanceFactor(root);

        // Left Left Imbalance/Case or Right rotation
        if (bf == 2 && getBalanceFactor(root->left) >= 0) {
            std::shared_ptr<Node<T, Cond>> y = root;
            std::shared_ptr<Node<T, Cond>> x = y->left;
            std::shared_ptr<Node<T, Cond>> T2 = x->right;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += x->subTree;
            x->subTree += y->subTree;
            y->subTree -= x->subTree;
            root = rightRotate(root);

            return root;
        }

            // Left Right Imbalance/Case or LR rotation
        else if (bf == 2 && getBalanceFactor(root->left) == -1) {
            std::shared_ptr<Node<T, Cond>> x = root->left;
            std::shared_ptr<Node<T, Cond>> y = x->right;
            std::shared_ptr<Node<T, Cond>> T2 = y->left;
            if (T2)
                T2->subTree += y->subTree;
            y->subTree += x->subTree;
            x->subTree -= y->subTree;

            root->left = leftRotate(root->left);

            y = root;
            x = y->left;
            T2 = x->right;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += x->subTree;
            x->subTree += y->subTree;
            y->subTree -= x->subTree;
            root = rightRotate(root);

            return root;
        }

            // Right Right Imbalance/Case or Left rotation
        else if (bf == -2 && getBalanceFactor(root->right) <= -0) {
            std::shared_ptr<Node<T, Cond>> x = root;
            std::shared_ptr<Node<T, Cond>> y = x->right;
            std::shared_ptr<Node<T, Cond>> T2 = y->left;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += y->subTree;
            y->subTree += x->subTree;
            x->subTree -= y->subTree;

            root = leftRotate(root);

            return root;
        }

            // Right Left Imbalance/Case or RL rotation
        else if (bf == -2 && getBalanceFactor(root->right) == 1) {
            std::shared_ptr<Node<T, Cond>> y = root->right;
            std::shared_ptr<Node<T, Cond>> x = y->left;
            std::shared_ptr<Node<T, Cond>> T2 = x->right;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += x->subTree;
            x->subTree -= y->subTree;
            y->subTree += x->subTree;

            root->right = rightRotate(root->right);

            x = root;
            y = x->right;
            T2 = y->left;
            if (T2) // if exists!!!!!!!!!
                T2->subTree += y->subTree;
            y->subTree += x->subTree;
            x->subTree -= y->subTree;

            root = leftRotate(root);

            return root;
        }

        return root;
    }

/*
    std::shared_ptr<Node<T, Cond>> Delete(std::shared_ptr<Node<T, Cond>> root, T val) {
        Cond c = Cond();

        // base case (if node not found)
        if (root == nullptr) {
            return nullptr;
        }

            // If the key to be deleted is smaller than the root's key,
            // then it lies in left subtree
        else if (c(val, root->value)) { //val < root -> value
            //root->subTree -= 1;
            root->left = Delete(root->left, val);
        }
            // If the key to be deleted is greater than the root's key,
            // then it lies in right subtree
        else if (c(root->value, val)) { //val > root -> value
            //root->subTree -= 1;
            root->right = Delete(root->right, val);
        }
            // if key is same as root's key, then This is the node to be deleted
        else {
            // node with only one child or no child
            if (root->left == nullptr) {
                std::shared_ptr<Node<T, Cond>> temp = root->right;

                if (temp != nullptr)
                    temp->subTree += root->subTree;

                if (root->right == nullptr) {
                    //root->value = nullptr;////////////////////////////////////////////////////////////////////////
                    root = nullptr;
                }
                return temp;
            } else if (root->right == nullptr) {
                std::shared_ptr<Node<T, Cond>> temp = root->left;
                temp->subTree += root->subTree;
                return temp;
            } else {
                // node with two children: Get the inorder successor (smallest in the right subtree)
                std::shared_ptr<Node<T, Cond>> temp = minValueNode(root->right);
                int prev = root->subTree;
                int sum = getSum(root, temp->value);
                temp->subTree = sum;

                // Copy the inorder successor's content to this node
                root->value = temp->value;
                // Delete the inorder successor
                //root->subTree-=1;

                root->right->subTree -= (sum-prev);
                root->left->subTree -= (sum-prev);
                root->right = Delete(root->right, temp->value);
            }
        }

        int bf = getBalanceFactor(root);
        // Left Left Imbalance/Case or Right rotation

        if (bf == 2 && getBalanceFactor(root->left) >= 0) {
            int remember = 0;
            if (root->right != nullptr)
                remember = root->right->subTree;
            root = rightRotate(root);
            root->right->subTree -= root->subTree;
            root->right->subTree += remember;
            fix(root);
            root->subTree = root->left->subTree + root->right->subTree + 1;
            return root;
        }
            // Left Right Imbalance/Case or LR rotation

        else if (bf == 2 && getBalanceFactor(root->left) == -1) {
            root->left = leftRotate(root->left);
            int temp = root->left->subTree;
            root->left->subTree = root->left->left->subTree;
            root->left->left->subTree = temp;

            int remember = 0;
            if (root->right != nullptr)
                remember = root->right->subTree;

            root = rightRotate(root);
            root->right->subTree -= root->subTree;
            fix(root);//root->left->subTree += remember;
            root->subTree = root->left->subTree + root->right->subTree + 1;
            return root;
        }

            // Right Right Imbalance/Case or Left rotation
        else if (bf == -2 && getBalanceFactor(root->right) <= -0) {
            int remember = 0;
            if (root->left != nullptr)
                remember = root->left->subTree;

            root = leftRotate(root);
            root->left->subTree -= root->subTree;
            root->left->subTree += remember;
            fix(root);
            root->subTree = root->left->subTree + root->right->subTree + 1;
            return root;
        }

            // Right Left Imbalance/Case or RL rotation
        else if (bf == -2 && getBalanceFactor(root->right) == 1) {
            root->right = rightRotate(root->right);
            int temp = root->right->subTree;
            root->right->subTree = root->right->right->subTree;
            root->right->right->subTree = temp;

            int remember = 0;
            if (root->left != nullptr)
                remember = root->left->subTree;

            root = leftRotate(root);
            root->left->subTree -= root->subTree;
            root->left->subTree += remember;
            fix(root);
            root->subTree = root->left->subTree + root->right->subTree + 1;
            return root;
        }

        return root;
    }
*/

    std::shared_ptr<Node<T, Cond>> deleteNode(std::shared_ptr<Node<T, Cond>> root, T val) {
        if (root == nullptr)
            return nullptr;

            //tree with onl one node, which we want to remove
        else if (this->height(root) == 0 && r->left == nullptr && r->right == nullptr && root->value == val) {
            //root->value.reset();/////////////////////////////////////////////////////////////////////////////////////////
            r = nullptr;
            return nullptr;
        } else {
            return Delete(root, val);
        }
    }

    void print2D(std::shared_ptr<Node<T, Cond>> root, int space) {
        if (root == nullptr) // Base case  1
            return;

        space += SPACE; // Increase distance between levels   2
        print2D(root->right, space); // Process right child first 3
        std::cout << std::endl;

        for (int i = SPACE; i < space; i++) // 5
            std::cout << " "; // 5.1

        std::cout << root->value << "\n"; // 6
        print2D(root->left, space); // Process left child  7
    }

    //add(i,x)
};


#endif //DATA_STRUCTURES_2_WET_2_AVLTREE_H
