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

    //helper for deallocating tree
    void destroyTree(Sat* satellite){
        if (satellite){
            destroyTree(satellite->m_left);
            destroyTree(satellite->m_right);
            delete satellite;
        }
    }
    //helper for getting the parent of a node
    Sat* getParent(Sat* satellite){
        if (satellite==nullptr){
            return nullptr;
        }
        Sat* parent = nullptr;
        Sat* current = m_root;
        while (current->m_id!=satellite->m_id){
            parent = current;
            if (satellite->m_id < current->m_id) {
                current = current->m_left;
            } else {
                current = current->m_right;
            }
        }
        return parent;
    }
    //helper for updating heights
    void updateHeight(Sat* node) {
        if (node) {
            // get height of left child if exists
            int leftHeight = (node->m_left) ? node->m_left->m_height : 0;
            // get height of right child if exists
            int rightHeight = (node->m_right) ? node->m_right->m_height : 0;
            node->m_height = ((leftHeight!=0||(rightHeight!=0))) ? 1 + max(leftHeight, rightHeight) : 0;
        }
    }
    //helper for left rotation
    Sat* leftRotate(Sat* x) {
        Sat* y = x->m_right;
        Sat* T2 = y->m_left;
        y->setLeft(x);
        x->setRight(T2);
        updateHeight(x);
        updateHeight(y);
        return y;
    }
    //helper for right rotation
    Sat* rightRotate(Sat* y) {
        Sat* x = y->m_left;
        Sat* T2 = x->m_right;
        x->setRight(y);
        y->setLeft(T2);
        updateHeight(y);
        updateHeight(x);
        return x;
    }
    //helper for left right rotation
    Sat* leftRightRotate(Sat* x) {
        x->m_left = leftRotate(x->m_left);
        return rightRotate(x);
    }
    //helper for right left rotation
    Sat* rightLeftRotate(Sat* x) {
        x->m_right = rightRotate(x->m_right);
        return leftRotate(x);
    }
    //helper for getting inorder predecessor
    Sat* getInorderPredecessor(Sat* root, int key) {
        if (!root){
            return nullptr;
        }
        Sat* predecessor = nullptr;
        while (root){
            if (key > root->m_id){
                predecessor = root;
                root = root->m_right;
            } else {
                root = root->m_left;
            }
        }

        return predecessor;
    }
    //helper for swaping the data in two nodes
    void swapNodes(Sat* node1, Sat* node2){
        int id = node1->m_id;
        ALT alt = node1->m_altitude;
        INCLIN inclin = node1->m_inclin;
        STATE state = node1->m_state; 
        int height = node1->m_height;
        node1->m_id = node2->m_id;
        node1->m_altitude = node2->m_altitude;
        node1->m_inclin = node2->m_inclin;
        node1->m_state = node2->m_state;
        node1->m_height = node2->m_height;
        node2->m_id = id;
        node2->m_altitude = alt;
        node2->m_inclin = inclin;
        node2->m_state = state;
        node2->m_height = height;
    }
};
#endif