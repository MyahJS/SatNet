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
    clear();
}

void SatNet::insert(const Sat& satellite){
    // Insert
    // preconditions: the tree exists
    // postconditions: if the passed sat node is valid, insert into the tree,
    //  update heights, check for imbalance, and fix imbalance

    // check for invalid ID
    if (satellite.getID() < MINID || satellite.getID() > MAXID) {
        throw runtime_error("Invalid ID. Insertion failed.");
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
    clearRecursive(m_root); // call helper to deallocate memory using recursion
    m_root = nullptr; // reinitialize root so tree becomes an empty tree
}

void SatNet::remove(int id){
    // Remove
    // preconditions: tree is not empty
    // postconditions: if node with passed id is in tree, remove node,
    //  update heights, check for imbalance, fix imbalance

    // check if id is in tree
    if (!findSatellite(id)) {
        throw runtime_error("ID not found. Removal failed.");
        return;
    }
    // remove node recursively using helper
    m_root = removeRecursive(m_root, id);
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
    // ListSatellites
    // preconditions: network exists
    // postconditions: prints all the members of each node in the tree
    cout << endl;
    listNodes(m_root); // helper function for using recursion
    cout << endl;
}

bool SatNet::setState(int id, STATE state){
    // GetState
    // preconditions: network exists
    // postconditions: if locates node with specified id, set state to new state
    if (m_root==nullptr)
        return false;

    Sat* current = m_root;
    while (current!=nullptr){
        if (id<current->getID()){
            current = current->getLeft();
        } else if (id>current->getID()){
            current = current->getRight();
        } else {
            current->setState(state);
            return true;
        }
    }

    return false;
}

void SatNet::removeDeorbited(){
    // RemoveDeorbited
    // preconditions: network exists
    // postconditions: remove all nodes with deorbited state
    if (m_root==nullptr)
        return;
    m_root = removeDeorbitedRecursive(m_root); 
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
    // Overloaded assignment operator
    // preconditions: lhs exists and rhs exists
    // postconditions: create deep copy of rhs
    if (this != &rhs){
        clear();
        m_root = copyRecursive(rhs.m_root); // recursive helper function
    }

    return *this;
}

int SatNet::countSatellites(INCLIN degree) const{
    // CountSatellites
    // preconditions: network exists
    // postconditons: return number of nodes with the specified degree
    if (m_root==nullptr)
        return 0;
    
    return countRecursive(m_root, degree); // recursive helper function
}