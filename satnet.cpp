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

    // if (m_root!=nullptr){
    //     destroyTree(m_root);
    // }

    clear();
}

void SatNet::insert(const Sat& satellite){
    // Insert
    // preconditions: the tree exists
    // postconditions: if the passed sat node is valid, insert into the tree,
    //  update heights, check for imbalance, and fix imbalance

    // check for invalid ID
    if (satellite.getID() < MINID || satellite.getID() > MAXID) {
        cout << "Invalid ID. Insertion failed." << endl;
        return;
    }

    // create a new node 
    Sat* newNode = new Sat(satellite);

    // if tree is empty, make the new node the root
    if (m_root == nullptr) {
        m_root = newNode;
    } else {
        // insert the node recursively using helper
        Sat* new_root = insertRecursive(m_root, newNode);
        // check if insert was successful
        if (new_root){
            m_root = new_root;
        }
    }
}

void SatNet::clear(){
    // Clear
    // preconditions: tree is not empty
    // postconditions: deallocate all memory and make empty tree
    
    // if (m_root!=nullptr){
    //     destroyTree(m_root);
    //     m_root = nullptr;
    // }

    clearRecursive(m_root);
    m_root = nullptr; // reinitialize root so tree becomes an empty tree
}

void SatNet::remove(int id){
    // Remove
    // preconditions: tree is not empty
    // postconditions: if node with passed id is in tree, remove node,
    //  update heights, check for imbalance, fix imbalance

    // check if id is in tree
    if (!findSatellite(id)) {
        cout << "ID " << id << " not found. Removal failed." << endl;
        return;
    }
    // remove node recursively using helper
    m_root = removeRecursive(m_root, id);

    // if (m_root==nullptr){
    //     cout << "Network is empty. Cannot delete satellite." << endl;
    // }
    // // locate node to delete
    // Sat* current = m_root;
    // Sat* parent = nullptr;
    
    // while (current!=nullptr&&current->m_id!=id){
    //     parent = current;
    //     if (id<current->m_id){
    //         // traverse left
    //         current = current->m_left;
    //     } else {
    //         // traverse right
    //         current = current->m_right;
    //     }
    // }

    // if (current==nullptr){
    //     cout << "ID not found. Cannot delete satellite." << endl;
    //     return;
    // }

    // // case 1: node is leaf
    // if ((current->m_left==nullptr)&&(current->m_right==nullptr)){
    //     if (parent!=nullptr){
    //         if (parent->m_left == current) {
    //             parent->m_left = nullptr;
    //         } else {
    //             parent->m_right = nullptr;
    //         }
    //     } else {
    //         m_root = nullptr;
    //     }
    //     delete current;
    //     updateHeight(parent);
    // // case 2: node has one left child
    // } else if ((current->m_left!=nullptr&&(current->m_right==nullptr))){
    //     Sat* temp = current->m_left;
    //     if (parent!=nullptr){
    //         if (parent->m_left == current) {
    //             parent->m_left = temp;
    //         } else {
    //             parent->m_right = temp;
    //         }
    //         updateHeight(parent);
    //     } else {
    //         // root node is deleted
    //         m_root = temp;
    //     }
    //     delete current;
    // // case 3: node has one right child
    // } else if ((current->m_left==nullptr)&&(current->m_right!=nullptr)){
    //     Sat* temp = current->m_right;
    //     if (parent!=nullptr){
    //         if (parent->m_left == current) {
    //             parent->m_left = temp;
    //         } else {
    //             parent->m_right = temp;
    //         }
    //         updateHeight(parent);
    //     } else {
    //         // root node is deleted
    //         m_root = temp;
    //     }
    //     delete current;
    // // case 4: node has 2 children
    // } else {
    //     // get inorder predecessor
    //     Sat* temp = getInorderPredecessor(m_root, current->m_id);
    //     // swap data of predecessor and current
    //     swapNodes(current, temp);

    //     // check if predecessor has a left child
    //     if (temp->m_left!=nullptr){
    //         // swap data of predecessor and child
    //         swapNodes(temp, temp->m_left);
    //         // then delete child
    //         delete temp->m_left;
    //         temp->m_left = nullptr;

    //         updateHeight(temp);
    //         updateHeight(current);
    //         if (parent!=nullptr){
    //             updateHeight(parent);
    //         }
    //     } else {
    //         getParent(temp)->m_left = nullptr;
    //         delete temp;

    //         updateHeight(current);
    //         if (parent!=nullptr){
    //             updateHeight(parent);
    //         }
    //     }
    // }
    // // check for and fix imbalances 
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
    // FindSatellite
    // preconditions: tree exists
    // postconditions: return true if passed id is in the tree, else return false

    if (m_root==nullptr)
        return false;

    Sat* current = m_root;
    while (current!=nullptr){
        if (id<current->getID()){
            current = current->getLeft();
        } else if (id>current->getID()){
            current = current->getRight();
        } else {
            return true;
        }
    }

    return false;   
}

const SatNet & SatNet::operator=(const SatNet & rhs){
    
}

int SatNet::countSatellites(INCLIN degree) const{
    
}