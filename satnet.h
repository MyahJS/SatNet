// UMBC - CMSC 341 - Fall 2023 - Proj2
#ifndef SATNET_H
#define SATNET_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class SatNet;
const int MINID = 10000;
const int MAXID = 99999;
enum STATE {ACTIVE, DEORBITED, DECAYING};
enum ALT {MI208, MI215, MI340, MI350};  // altitude in miles
enum INCLIN {I48, I53, I70, I97};       // inclination in degrees
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_INCLIN I48
#define DEFAULT_ALT MI208
#define DEFAULT_STATE ACTIVE
class Sat{
    public:
    friend class SatNet;
    friend class Grader;
    friend class Tester;
    Sat(int id, ALT alt=DEFAULT_ALT, INCLIN inclin = DEFAULT_INCLIN, STATE state = DEFAULT_STATE)
        :m_id(id),m_altitude(alt), m_inclin(inclin), m_state(state) {
            m_left = nullptr;
            m_right = nullptr;
            m_height = DEFAULT_HEIGHT;
        }
    Sat(){
        m_id = DEFAULT_ID;
        m_altitude = DEFAULT_ALT;
        m_inclin = DEFAULT_INCLIN;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state){
            case ACTIVE:text = "Active";break;
            case DEORBITED:text = "Deorbited";break;
            case DECAYING:text = "Decaying";break;
            default:text = "UNKNOWN";break;
        }
        return text;
    }
    INCLIN getInclin() const {return m_inclin;}
    string getInclinStr() const {
        string text = "";
        switch (m_inclin){
            case I48:text = "48 degrees";break;
            case I53:text = "53 degrees";break;
            case I70:text = "70 degrees";break;
            case I97:text = "97 degrees";break;
            default:text = "UNKNOWN";break;
        }
        return text;
    }
    ALT getAlt() const {return m_altitude;}
    string getAltStr() const {
        string text = "";
        switch (m_altitude){
            case MI208:text = "208 miles";break;
            case MI215:text = "215 miles";break;
            case MI340:text = "340 miles";break;
            case MI350:text = "350 miles";break;
            default:text = "UNKNOWN";break;
        }
        return text;
    }
    int getHeight() const {return m_height;}
    Sat* getLeft() const {return m_left;}
    Sat* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setInclin(INCLIN degree){m_inclin=degree;}
    void setAlt(ALT altitude){m_altitude=altitude;}
    void setHeight(int height){m_height=height;}
    void setLeft(Sat* left){m_left=left;}
    void setRight(Sat* right){m_right=right;}
    private:
    int m_id;
    ALT m_altitude;
    INCLIN m_inclin;
    STATE m_state;
    Sat* m_left;    //the pointer to the left child in the BST
    Sat* m_right;   //the pointer to the right child in the BST
    int m_height;   //the height of node in the BST
};
class SatNet{
    public:
    friend class Grader;
    friend class Tester;
    SatNet();
    ~SatNet();
    // overloaded assignment operator
    const SatNet & operator=(const SatNet & rhs);
    void insert(const Sat& satellite);
    void clear();
    void remove(int id);
    void dumpTree() const;
    void listSatellites() const;
    bool setState(int id, STATE state);
    void removeDeorbited();//removes all deorbited satellites from the tree
    bool findSatellite(int id) const;//returns true if the satellite is in tree
    int countSatellites(INCLIN degree) const;
    
    private:
    Sat* m_root;    //the root of the BST
    //helper for recursive traversal
    void dump(Sat* satellite) const;

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************

    //helper for deallocating tree using recursion
    void clearRecursive(Sat* satellite) {
        if (satellite) {
            clearRecursive(satellite->getLeft());  // recursively clear the left subtree
            clearRecursive(satellite->getRight()); // recursively clear the right subtree
            delete satellite; // deallocate memory for the current node
        }
    }
    //helper for getting height of a node (helpful to avoid getting height of null node)
    int getHeight(Sat* node) {
        if (node == nullptr)
            return 0;
        return node->getHeight();
    }
    //helper for getting balance factor of a node
    int getBalance(Sat* node) {
        if (node == nullptr)
            return 0;
        return getHeight(node->getLeft()) - getHeight(node->getRight());
    }
    //helper for right rotation
    Sat* rotateRight(Sat* node) {
        Sat* newRoot = node->getLeft();
        Sat* temp = newRoot->getRight();

        // Perform the rotation
        newRoot->setRight(node);
        node->setLeft(temp);

        // Update heights
        node->setHeight(1 + max(getHeight(node->getLeft()), getHeight(node->getRight())));
        newRoot->setHeight(1 + max(getHeight(newRoot->getLeft()), getHeight(newRoot->getRight())));

        return newRoot;
    }
    //helper for left rotation
    Sat* rotateLeft(Sat* node) {
        Sat* newRoot = node->getRight();
        Sat* temp = newRoot->getLeft();

        // Perform the rotation
        newRoot->setLeft(node);
        node->setRight(temp);

        // Update heights
        node->setHeight(1 + max(getHeight(node->getLeft()), getHeight(node->getRight())));
        newRoot->setHeight(1 + max(getHeight(newRoot->getLeft()), getHeight(newRoot->getRight())));

        return newRoot;
    }
    //helper for inserting node using recursion
    Sat* insertRecursive(Sat* root, Sat* newNode) {
        if (root == nullptr) {
            return newNode;
        }

        // Insert the node based on the ID as the key
        if (newNode->getID() < root->getID()) {
            root->setLeft(insertRecursive(root->getLeft(), newNode));
        } else if (newNode->getID() > root->getID()) {
            root->setRight(insertRecursive(root->getRight(), newNode));
        } else {
            // delete duplicate 
            cout << "Duplicate ID. Insertion failed." << endl;
            delete newNode;
            return nullptr;
        }

        
        // update height of current node
        root->setHeight(1 + max(getHeight(root->getLeft()), getHeight(root->getRight())));

        // check for imbalance 
        int balance = getBalance(root);

        // LL: right rotation
        if (balance > 1 && newNode->getID() < root->getLeft()->getID()) {
            return rotateRight(root);
        }
        // RR: left rotation
        if (balance < -1 && newNode->getID() > root->getRight()->getID()) {
            return rotateLeft(root);
        }
        // LR: left right rotation
        if (balance > 1 && newNode->getID() > root->getLeft()->getID()) {
            root->setLeft(rotateLeft(root->getLeft()));
            return rotateRight(root);
        }
        // RL: right left rotation
        if (balance < -1 && newNode->getID() < root->getRight()->getID()) {
            root->setRight(rotateRight(root->getRight()));
            return rotateLeft(root);
        }

        return root;
    }
    //helper for finding the leftmost node in a subtree (for finding inorder successor)
    Sat* findMinNode(Sat* node) {
        Sat* current = node;
        
        while (current->getLeft() != nullptr) {
            current = current->getLeft();
        }
        
        return current;
    }
    //helper for removing a node using recursion
    Sat* removeRecursive(Sat* root, int id) {
        if (root == nullptr)
            return root;

        // Find the node to remove
        if (id < root->getID())
            root->setLeft(removeRecursive(root->getLeft(), id));
        else if (id > root->getID())
            root->setRight(removeRecursive(root->getRight(), id));
        else {
            // Node found, perform removal
            if (root->getLeft() == nullptr || root->getRight() == nullptr) {
                Sat* temp = root->getLeft() ? root->getLeft() : root->getRight();

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;

                delete temp;
            } else {
                // Node with two children, find the inorder successor
                Sat* temp = findMinNode(root->getRight());

                // Copy the inorder successor's data to this node
                *root = *temp;

                // Delete the inorder successor
                root->setRight(removeRecursive(root->getRight(), temp->getID()));
            }
        }

        // check if tree is now empty
        if (root == nullptr)
            return root;

        // update height of current node
        root->setHeight(1 + max(getHeight(root->getLeft()), getHeight(root->getRight())));

        // check for imbalance 
        int balance = getBalance(root);

        // LL: right rotation
        if (balance > 1 && getBalance(root->getLeft()) >= 0)
            return rotateRight(root);
        // RR: left rotation
        if (balance < -1 && getBalance(root->getRight()) <= 0)
            return rotateLeft(root);
        // LR: left right rotation
        if (balance > 1 && getBalance(root->getLeft()) < 0) {
            root->setLeft(rotateLeft(root->getLeft()));
            return rotateRight(root);
        }
        // RL: right left rotation
        if (balance < -1 && getBalance(root->getRight()) > 0) {
            root->setRight(rotateRight(root->getRight()));
            return rotateLeft(root);
        }

        return root;
    }
    //helper for recusively listing members of each node
    void listNodes(Sat* satellite) const{
        // first check if exists
        if (satellite==nullptr){
            return;
        }
        // visit left child
        listNodes(satellite->m_left);
        // print members of current node
        cout << satellite->getID() << ": " << satellite->getState() << ": " << satellite->getInclinStr() << ": " << satellite->getAltStr() << endl;
        // visit right child
        listNodes(satellite->getRight());
    }

};
#endif