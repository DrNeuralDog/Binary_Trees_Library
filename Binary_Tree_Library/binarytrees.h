#ifndef BINARYTREES_H
#define BINARYTREES_H

#include <mutex>
#include <iostream>


namespace BinaryTrees {

/* * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * *
 * *      Description of additional    * *
 * *   functionality required for the  * *
 * *     binary tree library of work   * *
 * * * * * * * * * * * * * * * * * * * * *
   * * * * * * * * * * * * * * * * * * * */
namespace //Local namespace's names
{
typedef bool COLOR;
const bool RED = true;
const bool BLACK = false;
const bool MUTTABLE = true;
const bool IMMUTTABLE = false;
}

namespace ThreadingModel
{

/* * * * * * * * * * * * * * * * * * * * *
 * *    There is no real functionality   * *
 * *  for organizing the interthreading  * *
 * *        work of the program          * *
 * *        - it's just a stub           * *
   * * * * * * * * * * * * * * * * * * * * */
class SingleThreaded
{
protected:
    void lock()
    {

    }

    void unlock()
    {

    }
};

/* * * * * * * * * * * * * * * * * * *
 * *    Functionality of blocking    * *
 * *     resources using mutexes     * *
   * * * * * * * * * * * * * * * * * * */
class ObjectLevelLockable
{
    std::mutex mtx;

protected:
    void lock()
    {
        mtx.lock();
    }

    void unlock()
    {
        mtx.unlock();
    }
};

}


/* * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * *
 * *  Description of specific binary           * *
 * *  trees should be done in accordance       * *
 * *  with the API class of this library.      * *
 * *  These trees must contain the following   * *
 * *  public methods:                          * *
 * *    - search(Node*, int)                   * *
 * *    - getMin(Node*) && getMax(Node*)       * *
 * *    - remove(Node*, int)                   * *
 * *    - printTree(Node*)                     * *
 * *    - deleteTree(Node*&)                   * *
 * *    - copyTree(Node*, Node*)               * *
 * *    - getSizeTree(Node*)                   * *
 * * * * * * * * * * * * * * * * * * * * * * * * *
   * * * * * * * * * * * * * * * * * * * * * * * */
template <typename T> class BinarySearchTree
{
protected:
    class Node
    {
        int key_;
        T* data_;

        Node* left_ = null;
        Node* right_ = null;

    public:
        Node() = delete;
        explicit Node(int key, T* data): key_(key), data_(new T(*data))
        {

        }


        void update(const T data)
        {
            *data_ = data;
        }

        T& operator = (const T data)
        {
            *data_ = data;
        }


        //Setters:
        void set_key(int cur_key)
        {
            key_ = cur_key;
        }
        void set_data(T* data)
        {
            *data_ = *data;
        }
        void set_left(Node* cur_left)
        {
            left_ = cur_left;
        }
        void set_right(Node* cur_right)
        {
            right_ = cur_right;
        }

        //Getters:
        int key()
        {
            return key_;
        }
        const T data()
        {
            return* data_;
        }
        Node* get_left()
        {
            return left_;
        }
        Node* get_right()
        {
            return right_;
        }


protected:
        template <typename U = T>
        void operator delete(void* ptr, U); //! Либо вместо U использовать - typename std::enable_if<!std::is_same<U, BinarySearchTree>::value>::type* = nullptr - это механизм позволяющий контролировать, что объекту, который захочет удалить Node, разрешено это делать - на самом деле не работает в данном случае, так что это просто как вариант действий

        static void destroy(Node* node)
        {
            node->~Node();
            free(node);
        }

        friend class BinarySearchTree;
    };
    static Node *null;


    void insert(Node* __restrict node, int key, T* __restrict data)
    {
        insert_process(node, key, data);
    }

    //Поиск узлов дерева:
    Node* search(Node* node, int key)
    {
        if(node == null) return null;
        if(node->key() == key) return node;

        return (key < node->key()) ? search(node->get_left(), key) : search(node->get_right(), key);
    }

    Node* getMin(Node* node)
    {
        if(node == null) return null;
        if(node->set_left(null)) return node;
        return getMin(node->get_left());
    }

    Node* getMax(Node* node)
    {
        if(node == null) return null;
        if(node->get_right() == null) return node;
        return getMax(node->get_right());
    }

    void remove(Node*& node, int key)
    {
        node = remove_process(node, key);
    }

    //Дополнительный функционал дерева:
    void printTree(Node* node)
    {
        if(node == null) return;
        printTree(node->get_right());
        printTree(node->get_left());
        std::cout << "Node's key: " << node->key() << "  -  Node's value: " << node->data() << "  -  Node's adress: " << node << std::endl;
    }

    void deleteTree(Node*& node)
    {
        if(node == null) return;

        Node* delLeft = node->get_left();
        Node* delRight = node->get_right();

        deleteTree(delLeft);
        deleteTree(delRight);

        Node::destroy(node);
        node = null;
    }

    void copyTree(Node* node_donor, Node* node_recipient)
    {
        if(node_donor == null) return;
        copyTree(node_donor->get_right(), node_recipient->get_right());
        copyTree(node_donor->get_left(), node_recipient->get_left());
        node_recipient->set_key(node_donor->key());
        node_recipient->set_data(node_donor->data());
    }

    int getSizeTree(Node* node)
    {
        int countNodes = 0;
        checkSizeTree_process(node, &countNodes);

        return countNodes;
    }


private:
    void insert_process(Node* __restrict node, int key, T* __restrict data)
    {
        if(key < node->key())
        {
            if(node->get_left() == null) node->set_left(new Node(key, data));
            else insert_process(node->get_left(), key, data);
        }
        else if(key >= node->key())
        {
            if(node->get_right() == null) node->set_right(new Node(key, data));
            else insert_process(node->get_right(), key, data);
        }
    }

    Node* remove_process(Node *node, int key)
    {
        if(node == null) return null;
        else if(key < node->key()) node->set_left(remove_process(node->get_left(), key));
        else if(key > node->key()) node->set_right(remove_process(node->get_right(), key));
        else
        {
            if(node->get_left() == null && node->get_right() == null)
            {
                Node::destroy(node);
                node = null;
            }
            else if(node->get_left() == null || node->get_right() == null)
            {
                Node* temp = node;
                node = (node->get_left() == null) ? node->get_right() : node->get_left();

                Node::destroy(temp);
            }
            else
            {
                Node* maxInLeft = getMax(node->get_left());
                T max_data = maxInLeft->data();

                node->set_key(maxInLeft->key());
                node->set_data(&max_data);
                node->set_left(remove_process(node->get_left(), node->key()));
            }
        }

        return node;
    }

    void checkSizeTree_process(Node *node, int *count)
    {
        if(node == null) return;
        checkSizeTree_process(node->get_left(), count);
        *count += 1;
        checkSizeTree_process(node->get_right(), count);
    }

};
template <typename T> typename BinarySearchTree<T>::Node *BinarySearchTree<T>::null = nullptr;

template <typename T> class BinaryABLTree
{
protected:
    class Node
    {
        int key_;
        T* data_;
        size_t height_ = 0;

        Node* left_ = null;
        Node* right_ = null;


    public:
        Node() = delete;
        explicit Node(int key, T* data): key_(key), data_(new T(*data))
        {

        }


        void update(const T data)
        {
            *data_ = data;
        }

        T& operator = (const T data)
        {
            *data_ = data;
        }

        //Setters:
        void set_key(int cur_key)
        {
            key_ = cur_key;
        }
        void set_data(T* data)
        {
            *data_ = *data;
        }
        void set_left(Node* cur_left)
        {
            left_ = cur_left;
        }
        void set_right(Node* cur_right)
        {
            right_ = cur_right;
        }

        //Getters:
        int key()
        {
            return key_;
        }
        const T data()
        {
            return* data_;
        }
        Node* get_left()
        {
            return left_;
        }
        Node* get_right()
        {
            return right_;
        }


    protected:
        template <typename U = T>
        void operator delete(void* ptr, U); //! Либо вместо U использовать - typename std::enable_if<!std::is_same<U, BinarySearchTree>::value>::type* = nullptr - это механизм позволяющий контролировать, что объекту, который захочет удалить Node, разрешено это делать - на самом деле не работает в данном случае, так что это просто как вариант действий

        static void destroy(Node* node)
        {
            node->~Node();
            free(node);
        }

        friend class BinaryABLTree;
    };
    static Node *null;


    void insert(Node* __restrict node, int key, T* __restrict data)
    {
        insert_process(node, key, data);
    }

    void remove(Node*& node, int key)
    {
        node = remove_process(node, key);
    }

    //Поиск узлов дерева:
    Node* search(Node* node, int key)
    {
        if(node == null) return null;
        if(node->key() == key) return node;

        return (key < node->key()) ? search(node->get_left(), key) : search(node->get_right(), key);
    }

    Node* getMin(Node* node)
    {
        if(node == null) return null;
        if(node->get_left() == null) return node;
        return getMin(node->get_left());
    }
    Node* getMax(Node* node)
    {
        if(node == null) return null;
        if(node->get_right() == null) return node;
        return getMax(node->get_right());
    }

    //Дополнительный функционал дерева:
    void printTree(Node* node)
    {
        if(node == null) return;
        printTree(node->get_right());
        printTree(node->get_left());
        std::cout << "Node's key: " << node->key_ << "  -  Node's value: " << node->data() << "  -  Node's adress: " << node << std::endl;
    }

    void deleteTree(Node*& node)
    {
        if(node == null) return;

        Node* delLeft = node->get_left();
        Node* delRight = node->get_right();

        deleteTree(delLeft);
        deleteTree(delRight);

        Node::destroy(node);
        node = null;
    }

    void copyTree(Node* node_donor, Node* node_recipient)
    {
        if(node_donor == null) return;
        copyTree(node_donor->get_right(), node_recipient->get_right());
        copyTree(node_donor->get_left(), node_recipient->get_left());
        node_recipient->set_key(node_donor->key());
        *node_recipient->set_data(&node_donor->data());
    }

    int getSizeTree(Node* node)
    {
        int countNodes = 0;
        checkSizeTree_process(node, &countNodes);

        return countNodes;
    }


private:
    void insert_process(Node* __restrict node, int key, T* __restrict data)
    {
        if(key < node->key())
        {
            if(node->get_left() == null) node->set_left(new Node(key, data));
            else insert_process(node->get_left(), key, data);
        }
        else if(key >= node->key())
        {
            if(node->get_right() == null) node->set_right(new Node(key, data));
            else insert_process(node->get_right(), key, data);
        }

        updateHeight(node);
        balance(node);
    }

    Node* remove_process(Node* node, int key)
    {
        if(node == null) return null;
        else if(key < node->key()) node->set_left(remove_process(node->get_left(), key));
        else if(key > node->key()) node->set_right(remove_process(node->get_right(), key));
        else
        {
            if(node->get_left() == null && node->get_right() == null)
            {
                Node::destroy(node);
                node = null;
            }
            else if(node->get_left() == null || node->get_right() == null)
            {
                Node* temp = node;
                node = (node->get_left() == null) ? node->get_right() : node->get_left();

                Node::destroy(temp);
            }
            else
            {
                Node* maxInLeft = getMax(node->get_left());
                T max_data = maxInLeft->data();

                node->set_key(maxInLeft->key());
                node->set_data(&max_data);
                node->set_left(remove_process(node->get_left(), node->key()));
            }
        }

        if(node != null)
        {
            updateHeight(node);
            balance(node);
        }

        return node;
    }

    void checkSizeTree_process(Node *node, int *count)
    {
        if(node == null) return;
        checkSizeTree_process(node->get_left(), count);
        *count += 1;
        checkSizeTree_process(node->get_right(), count);
    }


    //Дополнительные методы с базовыми математическими операциями:
    int findMax(int operand_1, int operand_2)
    {
        return (operand_1 >= operand_2) ? operand_1 : operand_2;
    }

    void swap(Node* node_1, Node* node_2)
    {
        int node_1_key = node_1->key();
        T node_1_value = node_1->data();

        node_1->set_key(node_2->key());
        node_2->set_key(node_1_key);

        node_1->set_data(new T(node_2->data()));
        node_2->set_data(&node_1_value);
    }

    int calcNodeHeight(Node* node)
    {
        return (node == null) ? -1 : node->height_;
    }

    void updateHeight(Node* node)
    {
        node->height_ = findMax(calcNodeHeight(node->get_left()), calcNodeHeight(node->get_right())) + 1;
    }

    int calcBalanceValue(Node* node)
    {
        return (node == null) ? 0 : calcNodeHeight(node->get_right()) - calcNodeHeight(node->get_left());
    }


    void rightRotate(Node* node)
    {
        swap(node, node->get_left());

        Node* buffer = node->get_right();
        node->set_right(node->get_left());

        node->set_left(node->get_right()->get_left());

        node->get_right()->set_left(node->get_right()->get_right());
        node->get_right()->set_right(buffer);

        updateHeight(node->get_right());
        updateHeight(node);
    }

    void leftRotate(Node* node)
    {
        swap(node, node->get_right());

        Node* buf_1 = node->get_left();
        node->set_left(node->get_right());

        node->set_right(node->get_left()->get_right());

        node->get_left()->set_right(node->get_left()->get_left());
        node->get_left()->set_left(buf_1);

        updateHeight(node->get_left());
        updateHeight(node);
    }

    void balance(Node* node)
    {
        int balance = calcBalanceValue(node);

        if(balance == -2)
        {
            if(calcBalanceValue(node->get_left()) == 1)
                leftRotate(node->get_left());
            rightRotate(node);
        }
        else if(balance == 2)
        {
            if(calcBalanceValue(node->get_right()) == -1)
                rightRotate(node->get_right());
            leftRotate(node);
        }
    }

};
template <typename T> typename BinaryABLTree<T>::Node* BinaryABLTree<T>::null = nullptr;

template <typename T> class BinaryRedBlackTree
{
protected:
    class Node
    {
        bool color_mutable_;
        bool key_mutable_;
        bool data_mutable_;
        bool relatives_mutable_;

        int key_;
        T* data_;
        COLOR color_ = RED;

        Node* root_ = nullptr;
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        Node* parent_ = nullptr;


    public:
        Node() = delete;
        explicit Node(int key, T* data, COLOR color = RED, bool color_mutable = true, bool key_mutable = true, bool data_mutable = true, bool rel_mutable = true):
            key_(key),
            data_(new T(*data)), color_(color),
            left_(null), right_(null), parent_(null),
            color_mutable_(color_mutable), key_mutable_(key_mutable),
            data_mutable_(data_mutable), relatives_mutable_(rel_mutable)
        {
            root_ = this;
        }


        void update(const T data)
        {
            *data_ = data;
        }

        T& operator = (const T data)
        {
            *data_ = data;
        }


        //Setters:
        void set_color(COLOR cur_color)
        {
            if(color_mutable_)
                color_ = cur_color;
            // else std::cout << "Color is immutable, don't change it!\n";
        }
        void set_key(int cur_key)
        {
            if(key_mutable_)
                key_ = cur_key;
            // else std::cout << "Key is immutable, don't change it!\n";
        }
        void set_data(T* data)
        {
            if(data_mutable_)
                *data_ = *data;
            // else std::cout << "Data is immutable, don't change it!\n";
        }
        void set_root(Node* cur_root)
        {
            if(relatives_mutable_)
                root_ = cur_root;
            // else std::cout << "Root is immutable, don't change it!\n";
        }
        void set_left(Node* cur_left)
        {
            if(relatives_mutable_)
                left_ = cur_left;
            // else std::cout << "Left relative is immutable, don't change it!\n";
        }
        void set_right(Node* cur_right)
        {
            if(relatives_mutable_)
                right_ = cur_right;
            // else std::cout << "Right relative is immutable, don't change it!\n";
        }
        void set_parent(Node* cur_parent)
        {
            if(relatives_mutable_)
                parent_ = cur_parent;
            // else std::cout << "Parent is immutable, don't change it!\n";
        }

        //Getters:
        int key()
        {
            return key_;
        }
        const T data()
        {
            return* data_;
        }

        COLOR color()
        {
            return color_;
        }
        Node* get_root()
        {
            return root_;
        }
        Node* get_left()
        {
            return left_;
        }
        Node* get_right()
        {
            return right_;
        }
        Node* get_parent()
        {
            return parent_;
        }


    protected:
        template <typename U = T>
        void operator delete(void* ptr, U); //! Либо вместо U использовать - typename std::enable_if<!std::is_same<U, BinarySearchTree>::value>::type* = nullptr - это механизм позволяющий контролировать, что объекту, который захочет удалить Node, разрешено это делать - на самом деле не работает в данном случае, так что это просто как вариант действий

        static void destroy(Node* node)
        {
            node->~Node();
            free(node);
        }

        friend class BinaryRedBlackTree;
    };
    static Node *null;


    Node *search(Node* node, int key)
    {
        if(node == null) return null;
        if(node->key() == key) return node;

        return (key < node->key()) ? search(node->get_left(), key) : search(node->get_right(), key);
    }

    Node *getMin(Node* node)
    {
        if(node->get_left() == null) return node;
        return getMin(node->get_left());
    }

    Node *getMax(Node* node)
    {
        if(node->get_right() == null) return node;
        return getMax(node->get_right());
    }

    void insert(Node* __restrict node, int key, T* __restrict data)
    {
        insert_process(node, key, data);
    }

    void remove(Node*& node, int key)
    {
        remove_process(node, key);
    }


    void printTree(Node* node)
    {
        if(node == nullptr || node == null) { return;}
        printTree(node->get_right());
        printTree(node->get_left());
        std::cout << "Node's key: " << node->key() << "  -  Node's color: " << node->color() << "  -  Node's value: " << node->data() << "  -  Node's adress: " << node << "  -  Root adress: " << node->get_root() << std::endl;
    }

    void deleteTree(Node*& node)
    {
        if(node == nullptr || node == null) return;

        Node* delLeft = node->get_left();
        Node* delRight = node->get_right();
        deleteTree(delLeft);
        deleteTree(delRight);

        Node::destroy(node);
        node = nullptr;
    }

    void copyTree(Node* node_donor, Node* node_recipient)
    {
        if(node_donor == null) return;
        copyTree(node_donor->get_right(), node_recipient->get_right());
        copyTree(node_donor->get_left(), node_recipient->get_left());
        node_recipient->set_key(node_donor->key());
        node_recipient->set_data(node_donor->data());
    }

    int getSizeTree(Node* node)
    {
        int countNodes = 0;
        checkSizeTree_process(node, &countNodes);

        return countNodes;
    }


private:
    void balance(Node* tree, Node*& newNode)
    {
        Node* uncle;

        if(newNode->get_parent()->get_parent() != null)
        {
            while((newNode != tree->get_root()) && (newNode->get_parent()->color() == RED))
            {
                if(newNode->get_parent() == newNode->get_parent()->get_parent()->get_left())
                {
                    uncle = newNode->get_parent()->get_parent()->get_right();

                    if(uncle->color() == RED)
                    {
                        newNode->get_parent()->set_color(BLACK);
                        uncle->set_color(BLACK);
                        newNode->get_parent()->get_parent()->set_color(RED);
                        newNode = newNode->get_parent()->get_parent();
                    }
                    else
                    {
                        if(newNode == newNode->get_parent()->get_right())
                        {
                            leftRotate(newNode->get_parent());
                        }

                        newNode->get_parent()->set_color(RED);
                        rightRotate(newNode->get_parent()->get_parent());

                        newNode = newNode->get_parent();
                    }
                }
                else
                {
                    uncle = newNode->get_parent()->get_parent()->get_left();

                    if(uncle->color() == RED)
                    {
                        newNode->get_parent()->set_color(BLACK);
                        uncle->set_color(BLACK);
                        newNode->get_parent()->get_parent()->set_color(RED);
                        newNode = newNode->get_parent()->get_parent();
                    }
                    else
                    {
                        if(newNode == newNode->get_parent()->get_left())
                        {
                            rightRotate(newNode->get_parent());
                        }

                        newNode->get_parent()->set_color(RED);
                        leftRotate(newNode->get_parent()->get_parent());

                        newNode = newNode->get_parent();
                    }
                }
            }
        }

        tree->get_root()->set_color(BLACK);
    }

    void insert_process(Node* __restrict node, int key, T* __restrict data)
    {
        Node* currentNode = node->get_root();
        Node* parent = null;

        while(node_exists(currentNode))
        {
            parent = currentNode;
            if(key < currentNode->key())
                currentNode = currentNode->get_left();
            else
                currentNode = currentNode->get_right();
        }


        Node* newNode = create_node(key, data, RED, parent);

        if(key < parent->key())
            parent->set_left(newNode);
        else
            parent->set_right(newNode);
        newNode->set_root(parent->get_root());

        balance(node, newNode);
    }

    void balance(Node* tree, Node* child, Node* parent)
    {
        while(child != tree->get_root() && child->color() == BLACK)
        {
            Node* brother;

            if(child == parent->get_left())
            {
                brother = parent->get_right();
                if(brother->color() == RED)
                {
                    brother->set_color(BLACK);
                    parent->set_color(RED);
                    leftRotate(parent);
                    brother = parent->get_right();
                }
                if((!brother->get_left() || brother->get_left()->color() == BLACK) && (!brother->get_right() || brother->get_right()->color() == BLACK))
                {
                    brother->set_color(RED);
                    child = parent;
                    parent = child->get_parent();
                }
                else
                {
                    if(!(brother->get_right()) || brother->get_right()->color() == BLACK)
                    {
                        brother->get_left()->set_color(BLACK);
                        brother->set_color(RED);
                        rightRotate(brother);
                    }

                    brother->set_color(parent->color());
                    parent->set_color(BLACK);
                    brother->get_right()->set_color(BLACK);
                    leftRotate(parent);

                    brother = parent->get_right();
                    child = tree->get_root();
                }
            }
            else
            {
                brother = parent->get_left();
                if(brother->color() == RED)
                {
                    brother->set_color(BLACK);
                    parent->set_color(RED);
                    rightRotate(parent);
                    brother = parent->get_left();
                }
                if((!brother->get_left() || brother->get_left()->color() == BLACK) && (!brother->get_right() || brother->get_right()->color() == BLACK))
                {
                    brother->set_color(RED);
                    child = parent;
                    parent = child->get_parent();
                }
                else
                {
                    if(!(brother->get_left()) || brother->get_left()->color() == BLACK)
                    {
                        brother->get_right()->set_color(BLACK);
                        brother->set_color(RED);
                        leftRotate(brother);
                        brother = parent->get_left();
                    }

                    brother->set_color(parent->color());
                    parent->set_color(BLACK);
                    brother->get_left()->set_color(BLACK);
                    rightRotate(parent);

                    child = tree->get_root();
                }
            }
        }

        if(child->get_left()->color() == RED || child->get_right()->color() == RED)
            child->set_color(BLACK);
    }

    void remove_process(Node*& node, int key)
    {
        Node* nodeToDelete = search(node->get_root(), key);
        bool removedNodeColor = nodeToDelete->color();
        Node* parent = nodeToDelete->get_parent();
        Node* child;

        if(getChildrenCount(nodeToDelete) < 2)
        {
            child = getChildOrMock(nodeToDelete);
            transplainNode(node, nodeToDelete, child);
        }
        else
        {
            Node* minNode = getMin(nodeToDelete->get_right());

            parent = minNode->get_parent();
            removedNodeColor = minNode->color();

            nodeToDelete->set_key(minNode->key());
            nodeToDelete->set_data(new T(minNode->data()));

            child = getChildOrMock(minNode);
            transplainNode(node, minNode, child);
        }


        if(removedNodeColor == BLACK)
            balance(node, child, parent);
    }

    void checkSizeTree_process(Node* node, int* count)
    {
        if(node == nullptr || node == null) return;
        checkSizeTree_process(node->get_left(), count);
        *count += 1;
        checkSizeTree_process(node->get_right(), count);
    }


    bool node_exists(Node *node)
    {
        return node != null;
    }

    Node *create_node(int key, T* __restrict data, COLOR color, Node* __restrict parent)
    {
        Node* newNode = new Node(key, data, color);
        newNode->set_parent(parent);

        return newNode;
    }

    void swap(Node* node_1, Node* node_2)
    {
        int node_1_key = node_1->key();
        T node_1_value = node_1->data();

        node_1->set_key(node_2->key());
        node_2->set_key(node_1_key);

        node_1->set_data(new T(node_2->data()));
        node_2->set_data(&node_1_value);
    }

    int getChildrenCount(Node* node)
    {
        int count = 0;
        if(node_exists(node->get_left()))
            count += 1;
        if(node_exists(node->get_right()))
            count += 1;

        return count;
    }

    Node* getChildOrMock(Node* node)
    {
        return node_exists(node->get_left()) ? node->get_left() : node->get_right();
    }

    void fixRoot(Node* tree, Node* node)
    {
        if(node == null) return;

        fixRoot(tree, node->get_left());
        node->set_root(tree->get_root());
        fixRoot(tree, node->get_right());
    }

    void transplainNode(Node*& tree, Node*& toNode, Node* fromNode)
    {
        if(toNode == tree->get_root())
        {
            tree = fromNode;
            tree->set_root(fromNode);
            fixRoot(tree, tree);
        }
        else if(toNode == toNode->get_parent()->get_left())
            toNode->get_parent()->set_left(fromNode);
        else
            toNode->get_parent()->set_right(fromNode);


        if(fromNode != null)
        {
            fromNode->set_parent(toNode->get_parent());
            Node::destroy(toNode);
        }
    }

    void rightRotate(Node* node)
    {
        swap(node, node->get_left());

        Node* buffer = node->get_right();
        node->set_right(node->get_left());

        node->set_left(node->get_right()->get_left());
        node->get_left()->set_parent(node);

        node->get_right()->set_left(node->get_right()->get_right());
        node->get_right()->set_right(buffer);
        node->get_right()->get_right()->set_parent(node->get_right());
    }

    void leftRotate(Node* node)
    {
        swap(node, node->get_right());

        Node* buffer = node->get_left();
        node->set_left(node->get_right());

        node->set_right(node->get_left()->get_right());
        node->get_right()->set_parent(node);

        node->get_left()->set_right(node->get_left()->get_left());
        node->get_left()->set_left(buffer);
        node->get_left()->get_left()->set_parent(node->get_left());
    }


    static Node NullNode;
};
template <typename T> typename BinaryRedBlackTree<T>::Node BinaryRedBlackTree<T>::NullNode(0, new T(), BLACK, IMMUTTABLE, IMMUTTABLE, IMMUTTABLE, IMMUTTABLE); //Для каждого типа, который будет использован в процессе работы программы(те с которым вызвана в коде данная библиотека), будет создана своя статическая переменная NullNode
template <typename T> typename BinaryRedBlackTree<T>::Node* BinaryRedBlackTree<T>::null = &NullNode;


/* * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * *
 * *       Access to trees       * *
 * * * * * * * * * * * * * * * * * *
   * * * * * * * * * * * * * * * * */
template <typename T, class Tree = BinaryTrees::BinarySearchTree<T>, class Mutex = ThreadingModel::SingleThreaded>  class BinaryTrees_API: public Tree, Mutex
{
#define ThreadSafe_ON Mutex::lock();
#define ThreadSafe_OFF Mutex::unlock();

    typename Tree::Node *tree = Tree::null;


    bool availability_key(int key)
    {
        if(Tree::search(tree, key) == Tree::null)
            return true;
        return false;
    }
    bool is_tree_empty()
    {
        if(!Tree::getSizeTree(tree))
            return true;
        return false;
    }


public:
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * *  This container must ensure the following constraints:                                                      * *
     * *    - the key of each node of the tree is unique!                                                            * *
     * *    - new nodes can be added:                                                                                * *
     * *        * by explicitly specifying the key                                                                   * *
     * *          (in this case, the node may not be created if such a key already exists -                          * *
     * *          the user will be notified of the success of the operation by boolean values                        * *
     * *          returned from the corresponding function)                                                          * *
     * *        * by passing only the data that should be stored in the tree                                         * *
     * *          (then the key is assigned to such data automatically)                                              * *
     * *    - nodes can be deleted only by the obtained key value, or all nodes of the tree can be deleted at once,  * *
     * *      or the tree itself can be deleted!                                                                     * *
     * *    - it is also possible to get the number of nodes in the tree                                             * *
     * *    - it is possible to get any node in the tree                                                             * *
     * *    - output to the console of all elements of the tree                                                      * *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    /*
    //Данный контейнер, должен обеспечивать гарантировать следующие ограничения:
    // - ключ каждого узла дерева - уникален!
    // - добавлять новые узлы можно:
    //   * явно задавая ключ (в этом случае узел может быть не создан, если такой ключ уже есть - об успехе операции пользователь будет уведомлён булевыми значениями, возращенными из соответствующей функции)
    //   * передавая лишь сами данные, которые должны быть сохранены в дереве (тогда ключ таким данным назначается автоматически)
    // - удалять узлы можно лишь по полученному значению ключа, либо удалять все узлы дерева разом, либо удалять само дерево!
    // - можно также получить количество узлов в дереве
    // - можно получить любой узел в дереве
    // - вывод в консоль всех элементов дерева
    */

    explicit BinaryTrees_API() = default;
    explicit BinaryTrees_API(int key, T data)
    {
        ThreadSafe_ON

        tree = new typename Tree::Node(key, &data); //По умолчанию, при создании данного дерева, узлов в нём нет, а потому любой ключ доступен, в отличии от добавления в уже созданый контейнер!

        ThreadSafe_OFF
    }
    virtual ~BinaryTrees_API()
    {
        deleteTree();
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * *
     * *      If the user does not specify the key       * *
     * *        they need, then the key is found         * *
     * *     automatically and the user should know      * *
     * *  it, so the key is returned from the function!  * *
       * * * * * * * * * * * * * * * * * * * * * * * * * * */
    int append(T data)
    {
        ThreadSafe_ON

        int key = 1;

        if(is_tree_empty())
            tree = new typename Tree::Node(key, &data);
        else
        {
            typename Tree::Node *node = Tree::getMax(tree);
            key += node->key();
            Tree::insert(node, key, &data);
        }

        ThreadSafe_OFF

        return key;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * *   If a node with the required key already exists,   * *
     * *   the node insertion does not occur, and the user   * *
     * *         is explicitly notified about it             * *
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    bool insert(int key, T data)
    {
        ThreadSafe_ON

        if(is_tree_empty())
            tree = new typename Tree::Node(key, &data);
        else
        {
            if(availability_key(key))
                Tree::insert(tree, key, &data);
            else
            {
                ThreadSafe_OFF
                return false;
            }
        }

        ThreadSafe_OFF

        return true;
    }

    typename Tree::Node *search(int key)
    {
        ThreadSafe_ON

        typename Tree::Node* node = Tree::search(tree, key);

        if(node == Tree::null)
        {
            throw std::out_of_range("Out of range! Node not found in the three..."); //Выкидываем исключение, с выводом сообщения об ошибке, либо:
            //or
            //std::cout << " <<  Warning! No data was found! Instead of the data, the empty stub is returned...  >> " << std::endl;
            //return new typename Tree::Node(0, new T()); //Возращаем пустую заглушку
        }

        ThreadSafe_OFF

        return node;
    }

    int size()
    {
        ThreadSafe_ON

        int size = Tree::getSizeTree(tree);

        ThreadSafe_OFF

        return size;
    }
    void printTree()
    {
        ThreadSafe_ON

        Tree::printTree(tree);

        ThreadSafe_OFF
    }

    void remove(int key)
    {
        ThreadSafe_ON

        Tree::remove(tree, key);

        if(is_tree_empty())
            tree = Tree::null;

        ThreadSafe_OFF
    }
    void deleteTree()
    {
        ThreadSafe_ON

        Tree::deleteTree(tree);
        tree = Tree::null;

        ThreadSafe_OFF
    }
};

}


namespace abt{//abt - Access to Binary Trees
/* * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * *
 * *   Hiding the features of working  * *
 * *      with trees in the library,   * *
 * *  as well as organizing simplified * *
 * *     user access to the trees      * *
 * * * * * * * * * * * * * * * * * * * * *
   * * * * * * * * * * * * * * * * * * * */

template<typename T> using SearchTree = BinaryTrees::BinaryTrees_API <T>;
template<typename T> using ABLTree = BinaryTrees::BinaryTrees_API <T, BinaryTrees::BinaryABLTree<T>>;
template<typename T> using RedBlackTree = BinaryTrees::BinaryTrees_API <T, BinaryTrees::BinaryRedBlackTree<T>>;


/* * * * * * * * * * * * * * * * * * * * * * *
 * *  _p - protected work with multy thread  * *
   * * * * * * * * * * * * * * * * * * * * * * */
template<typename T> using SearchThree_p = BinaryTrees::BinaryTrees_API <T, BinaryTrees::BinarySearchTree<T>, BinaryTrees::ThreadingModel::ObjectLevelLockable>;
template<typename T> using ABLTree_p = BinaryTrees::BinaryTrees_API <T, BinaryTrees::BinaryABLTree<T>, BinaryTrees::ThreadingModel::ObjectLevelLockable>;
template<typename T> using RedBlackTree_p = BinaryTrees::BinaryTrees_API <T, BinaryTrees::BinaryRedBlackTree<T>, BinaryTrees::ThreadingModel::ObjectLevelLockable>;

}


#endif // BINARYTREES_H
