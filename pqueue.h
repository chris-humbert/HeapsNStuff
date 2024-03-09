#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

class Tester;
class PQueue;
class Patient;
#define EMPTY Patient() //This is an empty object
enum HEAPTYPE {MINHEAP, MAXHEAP};
enum STRUCTURE {SKEW, LEFTIST};

// Priority function pointer type
typedef int (*prifn_t)(const Patient&);
const int MINTEMP = 35; //Body temperature, celsius
const int MAXTEMP = 42;
const int MINOX = 70;   //Level of oxygen saturation (SpO2)
const int MAXOX = 101;
const int MINRR = 10;   //Respiratory Rate, per minute
const int MAXRR = 40;
const int MINBP = 70;   //Blood Pressure
const int MAXBP = 160;
const int MINOPINION = 1;   //Nurse opinion, between 1 - 10
const int MAXOPINION = 10;  //1 is highest priotity
//
//patient class
//
class Patient {
    public:
    friend class Tester; //contains test functions
    friend class PQueue;
    Patient() {
        //This is an empty object since name is empty
        m_patient = ""; m_temperature = 37; m_oxygen = 100;
        m_RR = 20;m_BP = 100;m_opinion=10;
    }
    Patient(string name, int temp, int ox, int rr, int bp, int op) {
        if ( (temp < MINTEMP || temp > MAXTEMP) ||
        (ox < MINOX || ox > MAXOX) || (rr < MINRR || rr > MAXRR) ||
        (bp < MINBP || bp > MAXBP) || (op < 1 || op > 10)){
            //create an empty object
            m_patient = ""; m_temperature = 37; m_oxygen = 100;
            m_RR = 20; m_BP = 100;m_opinion=10;
        }
        else{
            m_patient = name; m_temperature = temp; m_oxygen = ox;
            m_RR = rr; m_BP = bp;m_opinion=op;
        }
    }
    string getPatient() const {return m_patient;}
    int getTemperature() const {return m_temperature;}
    int getOxygen() const {return m_oxygen;}
    int getRR() const {return m_RR;}
    int getBP() const {return m_BP;}
    int getOpinion() const {return m_opinion;}
    void setPatient(string name) {m_patient=name;}
    void setTemperature(int val) {m_temperature=val;}
    void setOxygen(int val) {m_oxygen=val;}
    void setRR(int val) {m_RR=val;}
    void setBP(int val) {m_BP=val;}
    void setOpinion(int val) {m_opinion=val;}
    //Overloaded assignment operator
    const Patient & operator=(const Patient& rhs){
        if (this != &rhs){
            m_patient = rhs.m_patient;
            m_temperature = rhs.m_temperature;
            m_oxygen = rhs.m_oxygen;
            m_RR = rhs.m_RR;
            m_BP = rhs.m_BP;
            m_opinion = rhs.m_opinion;
        }
        return *this;
    }
    //Overloaded equality operator
    bool operator==(const Patient & rhs) const {
        return ((m_patient == rhs.m_patient) &&
                (m_temperature == rhs.m_temperature) &&
                (m_oxygen == rhs.m_oxygen) &&
                (m_RR == rhs.m_RR) &&
                (m_BP == rhs.m_BP) &&
                (m_opinion == rhs.m_opinion));
    }
    //Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Patient& patient);

    private:
    string m_patient;  //Patient's name, no need to be unique
    int m_temperature; //Body temperature, celsius
    int m_oxygen;      //Level of oxygen saturation (SpO2), percentage
    int m_RR;          //Respiratory Rate, per minute
    int m_BP;          //Blood Pressure
    int m_opinion;     //Nurse opinion, 1 - 10
};

class Node {
    public:
    friend class Tester;
    friend class PQueue;
    Node(Patient patient) {
        m_patient = patient;
        m_right = nullptr;
        m_left = nullptr;
        m_npl = 0;
    }
    Patient getPatient() const {return m_patient;}
    void setNPL(int npl) {m_npl = npl;}
    int getNPL() const {return m_npl;}

    //Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Node& node);

    private:
    Patient m_patient;   //Patient information
    Node *m_right;       //Right child
    Node *m_left;        //Left child
    int m_npl;           //null path length for leftist heap
};

class PQueue {
public:
    friend class Tester;
    PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure);
    ~PQueue();
    PQueue(const PQueue& rhs);
    PQueue& operator=(const PQueue& rhs);
    void insertPatient(const Patient& input);
    Patient getNextPatient();
    void mergeWithQueue(PQueue& rhs);
    void clear();
    int numPatients() const;
    void printPatientQueue() const;
    prifn_t getPriorityFn() const;
    void setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
    HEAPTYPE getHeapType() const;
    STRUCTURE getStructure() const;
    void setStructure(STRUCTURE structure);
    void dump() const;

private:
    Node * m_heap;
    int m_size;
    prifn_t m_priorFunc;
    HEAPTYPE m_heapType;
    STRUCTURE m_structure;

    void dump(Node *pos) const;
    //Recursive function that prints the heap in preOrder
    void printHeap(Node* node)const{
        //Checks to see if node exists
        if(node != nullptr){
            //Outputs the patients priority and stats using the overloaded << operator
            cout << "[" << m_priorFunc(node->getPatient()) << "] " << *node << endl;

            //Traverses to the children
            printHeap(node->m_left);
            printHeap(node->m_right);
        }
    }

    //Recursive delete function to clear heap
    void clearHeap(Node*& node){
        //Checks to make sure function wont try to delete a
        if(node != nullptr){
            clearHeap(node->m_left);
            clearHeap(node->m_right);

            //Deletes the node and decrements size
            delete node;
            m_size--;
            node = nullptr;
        }
    }

    //Function to set the NPL variable of a node using the children
    void setNPL(Node*& node){
        //Bools to see if the children exist or not
        bool left = node->m_left != nullptr;
        bool right = node->m_right != nullptr;

        //If one of the children is nullptr then the NPL is 0
        if((!left && right) || (!right && left))
            node->setNPL(0);
        //Otherwise checks to see which child has a lesser NPL value and then adds 1 to it to get the new NPL value
        else{
            int newNPL = 0;
            if(node->m_left->getNPL() > node->m_right->getNPL())
                newNPL = node->m_right->getNPL() + 1;
            else
                newNPL = node->m_left->getNPL() + 1;

            //Sets the nodes NPL based on children NPL
            node->setNPL(newNPL);
        }
    }

    //Takes in a the previous nodes in the original heap and remakes a new heap using those nodes
    //Passes in the newHeap by reference to make it easier to access after its done remaking it
    void remakeHeap(Node* node, Node*& newHeap){
        //Checks to make sure the node isn't empty
        if(node != nullptr){
            //Traverses to the leaves of the heap
            remakeHeap(node->m_left, newHeap);
            remakeHeap(node->m_right, newHeap);

            //Sets the children to nullptr to ensure no duplicates will be added due to how merge works
            //Sets the NPL to 0 incase of a LEFTIST heap
            node->setNPL(0);
            node->m_left = nullptr;
            node->m_right = nullptr;
            //Adds each node to the new heap and sets the newHeap to what the remade heap
            newHeap = mergeHeaps(newHeap, node);
        }
    }

    //This function gets the NPL values of the children if they exist and return a bool
    // to tell a different function to swap the children in a LEFTIST HEAP
    bool compareNPL(Node* node){
        //Default nullptr NPL values
        int left = -1;
        int right = -1;

        //If the children exist update the NPL values
        if(node->m_right != nullptr)
            right = node->m_right->getNPL();
        if(node->m_left != nullptr)
            left = node->m_left->getNPL();

        //If the right subtrees NPL is bigger than the lefts then returns true to swap chilren
        if(right > left)
            return true;
        else
            return false;
    }

    //Swaps heapOne and heapTwo
    void swapHeaps(Node*& heapOne, Node*& heapTwo){
        Node* tempOne = heapOne;
        Node* tempTwo = heapTwo;

        //Swaps the nodes
        heapOne = tempTwo;
        heapTwo = tempOne;
    }

    //Recursively merges two heaps together into one heap
    Node* mergeHeaps(Node* heapOne, Node* heapTwo){
        //If one of the heaps has an open space (nullptr) inserts the other heap into the slot
        if(heapOne == nullptr)
            return heapTwo;
        //
        if(heapTwo == nullptr)
            return heapOne;

        //If the type of heap is a MAXHEAP and the firstHeap's priority is less than or equal to the second heap
        //It swaps the heaps so the secondHeap is the firstHeap
        else if(m_heapType == MAXHEAP){
            if(m_priorFunc(heapOne->getPatient()) <= m_priorFunc(heapTwo->getPatient()))
                swapHeaps(heapOne, heapTwo);
        }
        //If the type of heap is a MINHEAP and the firstHeap's priority is greater than or equal to the second heap
        //It swaps the heaps so the secondHeap is the firstHeap
        else if(m_heapType == MINHEAP){
            if(m_priorFunc(heapOne->getPatient()) >= m_priorFunc(heapTwo->getPatient()))
                swapHeaps(heapOne, heapTwo);
        }

        //If the structure is a SKEW heap it swaps the children and  merges the rightChild of heapOne and the remaining heapTwo
        if(m_structure == SKEW){
            //Swaps children
            Node* temp = heapOne->m_right;
            heapOne->m_right = heapOne->m_left;
            //Sets the left child of the heap to the recursive call of mergeHeaps using the right child and heapTwo
            heapOne->m_left = mergeHeaps(heapTwo, temp);
        }
        //If the structure is a LEFTIST heap merges the rightChild of heapOne with heapTwo and the child
        else if (m_structure == LEFTIST){
            //Sets the right child of heapOne to the merged heap it returns when merging heapTwo with the child
            heapOne->m_right = mergeHeaps(heapTwo, heapOne->m_right);

            //During unwind sets the NPL of each node and checks if it needs to swap the children based of it
            setNPL(heapOne);
            //If the checker determines that the right subtree's NPL is larger than it swaps the children of heapOne
            if(compareNPL(heapOne)){
                Node* temp = heapOne->m_left;
                heapOne->m_left = heapOne->m_right;
                heapOne->m_right = temp;
            }
        }
        //Returns the heap gotten after merging
        return heapOne;
    }

    //This recursive function creates a copy of a binary tree in this case it creates an identical copy of the heap tree
    Node* copyConstructHelper(Node* source, Node* copyNode){
        //If the node gotten is nullptr it returns nullptr
        if(source == nullptr)
            return nullptr;
        //Creates a new node given the sources node's data in this case a Patient object
        copyNode = new Node(source->getPatient());

        //Sets the children to the Node returned when recursively going through the tree
        copyNode->m_left = copyConstructHelper(source->m_left, copyNode->m_left);
        copyNode->m_right = copyConstructHelper(source->m_right, copyNode->m_right);

        //If the structure is a leftist heap it copies over the NPL value as well
        if(m_structure == LEFTIST)
            copyNode->m_npl = source->m_npl;

        //Returns the new node
        //Because of recursive unwind it winds up returning the heaps root node when function ends
        return copyNode;
    }
};

#endif
