// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet(){
    // Constructor
    // preconditions: none
    // postconditions: create empty tree
    m_root = nullptr;
}

SatNet::~SatNet(){
    // Destructor
    // preconditions: tree is not empty
    // postconditions: deallocate all memory
    if (m_root!=nullptr){
        destroyTree(m_root);
    }
}

void SatNet::insert(const Sat& satellite){
    // Insert
    // preconditions: the tree exists
    // postconditions: if the passed sat node is valid, insert into the tree,
    //  update heights, check for imbalance, and fix imbalance
    Sat* newNode = new Sat(satellite.m_id, satellite.m_altitude, satellite.m_inclin, satellite.m_state);

    // check validity
    if (newNode->m_id < MINID || newNode->m_id > MAXID){
        delete newNode; // deallocate memory
        cout << "Invalid ID. Cannot insert the satellite." << endl;
    }

    // if the tree is empty, make the new node the root
    if (m_root == nullptr){
        m_root = newNode;
        return;
    }

    // initialize parent and current pointers for traversal
    Sat* parent = nullptr;
    Sat* current = m_root;

    while (current){
        parent = current;

        // check if duplicate
        if (newNode->m_id == current->m_id){
            delete newNode; // Deallocate memory
            cout << "Duplicate ID. Cannot insert the satellite." << endl;
        }

        if (newNode->m_id < current->m_id){
            current = current->m_left;
        } else {
            current = current->m_right;
        }
    }

    // insert node on left or right based on id
    if (newNode->m_id < parent->m_id){
        parent->m_left = newNode;
    } else {
        parent->m_right = newNode;
    }

    // update heights on path
    current = newNode;
    while (current!=nullptr){
        // update height of current node
        updateHeight(current);
        // move up the tree
        current = parent;
        parent = getParent(current);
    }

    // check balance factors on path
    // fix imbalance when problem node found
    current = newNode;
    parent = getParent(current);
    int leftHeight = 0;     // height of left child
    int rightHeight = 0;    // height of right child
    int balanceFactor = 0;  // balance factor of current node
    int prevBalance = 0;    // balance factor of current node's child

    while (current!=nullptr){
        prevBalance = balanceFactor;

        leftHeight = (current->m_left!=nullptr) ? current->m_left->m_height : 0;
        rightHeight = (current->m_right!=nullptr) ? current->m_right->m_height : 0;
        balanceFactor = leftHeight - rightHeight;

        // check if right heavy
        if (balanceFactor<(-1)){
            // check if RR
            if (prevBalance<=0){
                // left rotation here
                current = leftRotate(current);
            } else {
                // right left rotation
                current = rightLeftRotate(current);
            }
        // check if left heavy
        } else if (balanceFactor>1){
            // check if LL
            if (prevBalance>=0){
                // right rotation here
                current = rightRotate(current);
            } else {
                // left right rotation here
                current = leftRightRotate(current);
            }
        }


        // move up tree
        current = parent;
        parent = getParent(current);
    }
}

void SatNet::clear(){
    // Clear
    // preconditions: tree is not empty
    // postconditions: deallocate all memory and make empty tree
    if (m_root!=nullptr){
        destroyTree(m_root);
        m_root = nullptr;
    }
}

void SatNet::remove(int id){
    // Remove
    // preconditions: tree is not empty
    // postconditions: if node with passed id is in tree, remove node,
    //  update heights, check for imbalance, fix imbalance
    if (m_root!=nullptr){
        // locate node to delete
        Sat* current = m_root;
        Sat* parent = nullptr;
        
        while (current!=nullptr&&current->m_id!=id){
            parent = current;
            if (id<current->m_id){
                // traverse left
                current = current->m_left;
            } else {
                // traverse right
                current = current->m_right;
            }
        }

        if (current!=nullptr){
            // case 1: node is leaf
            if ((current->m_left==nullptr)&&(current->m_right==nullptr)){
                if (parent!=nullptr){
                    if (parent->m_left == current) {
                        parent->m_left = nullptr;
                    } else {
                        parent->m_right = nullptr;
                    }
                } else {
                    m_root = nullptr;
                }
                delete current;
                updateHeight(parent);
            // case 2: node has one left child
            } else if ((current->m_left!=nullptr&&(current->m_right==nullptr))){
                Sat* temp = current->m_left;
                if (parent!=nullptr){
                    if (parent->m_left == current) {
                        parent->m_left = temp;
                    } else {
                        parent->m_right = temp;
                    }
                    updateHeight(parent);
                } else {
                    // root node is deleted
                    m_root = temp;
                }
                delete current;
            // case 3: node has one right child
            } else if ((current->m_left==nullptr)&&(current->m_right!=nullptr)){
                Sat* temp = current->m_right;
                if (parent!=nullptr){
                    if (parent->m_left == current) {
                        parent->m_left = temp;
                    } else {
                        parent->m_right = temp;
                    }
                    updateHeight(parent);
                } else {
                    // root node is deleted
                    m_root = temp;
                }
                delete current;
            // case 4: node has 2 children
            } else {
                // get inorder predecessor
                Sat* temp = getInorderPredecessor(m_root, current->m_id);
                // swap data of predecessor and current
                swapNodes(current, temp);

                // check if predecessor has a left child
                if (temp->m_left!=nullptr){
                    // swap data of predecessor and child
                    swapNodes(temp, temp->m_left);
                    // then delete child
                    delete temp->m_left;
                    temp->m_left = nullptr;

                    updateHeight(temp);
                    updateHeight(current);
                    if (parent!=nullptr){
                        updateHeight(parent);
                    }
                } else {
                    getParent(temp)->m_left = nullptr;
                    delete temp;

                    updateHeight(current);
                    if (parent!=nullptr){
                        updateHeight(parent);
                    }
                }
            }

            // check for and fix imbalances 
            int leftHeight = (current->m_left) ? current->m_left->m_height : 0;
            int rightHeight = (current->m_right) ? current->m_right->m_height : 0;
            // calculate balance factor
            int balanceFactor = leftHeight - rightHeight;
            int prevBalance = 0;

            while (current!=nullptr){
                prevBalance = balanceFactor;
                current = parent;
                parent = getParent(current);

                leftHeight = (current->m_left) ? current->m_left->m_height : 0;
                rightHeight = (current->m_right) ? current->m_right->m_height : 0;
                balanceFactor = leftHeight - rightHeight;

                // check if right heavy
                if (balanceFactor<(-1)){
                    // check if RR
                    if (prevBalance<=0){
                        // left rotation here
                        current = leftRotate(current);
                        parent->m_right = current;
                    } else {
                        // right left rotation
                        current = rightLeftRotate(current);
                        parent->m_right = current;
                    }
                // check if left heavy
                } else if (balanceFactor>1){
                    // check if LL
                    if (prevBalance>=0){
                        // right rotation here
                        current = rightRotate(current);
                        parent->m_left = current;
                    } else {
                        // left right rotation here
                        current = leftRightRotate(current);
                        parent->m_left = current;
                    }
                }
            }
        }
    }
}

void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

void SatNet::listSatellites() const {

}

bool SatNet::setState(int id, STATE state){
    
}

void SatNet::removeDeorbited(){
    
}

bool SatNet::findSatellite(int id) const {
    
}

const SatNet & SatNet::operator=(const SatNet & rhs){
    
}

int SatNet::countSatellites(INCLIN degree) const{
    
}