#include "pqueue.h"
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure){
    //Assigns each member variable to its respective passed in variable
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
    m_heap = nullptr;
    m_size = 0;
}

PQueue::~PQueue(){
    clear();
}

void PQueue::clear() {
    //If the heap isn't already empty it clears it
    if(m_heap != nullptr){
        clearHeap(m_heap);
        m_heap = nullptr;
        m_size = 0;
    }
}

PQueue::PQueue(const PQueue& rhs){
    //Assigns each variable to its source variable
    m_heap = nullptr;
    m_structure = rhs.m_structure;
    m_heapType = rhs.m_heapType;
    m_priorFunc = rhs.m_priorFunc;
    m_size = rhs.m_size;

    //Checks to see if the source heap is empty, if it isnt it calls a helper function to copy the heap
    if(rhs.m_heap != nullptr)
        m_heap = copyConstructHelper(rhs.m_heap, m_heap);
}

PQueue& PQueue::operator=(const PQueue& rhs) {
    //Checks to see if objects are equal first
    if(this != & rhs)
        clear();

    m_heap = nullptr;
    m_structure = rhs.m_structure;
    m_heapType = rhs.m_heapType;
    m_priorFunc = rhs.m_priorFunc;
    m_size = rhs.m_size;

    //Checks to see if the source heap is empty, if it isnt it calls a helper function to copy the heap
    if(rhs.m_heap != nullptr)
        m_heap = copyConstructHelper(rhs.m_heap, m_heap);

    return *this;
}

void PQueue::mergeWithQueue(PQueue& rhs) {
    //This if statement checks to make sure the heaps attempting merge are the same
    if(m_structure == rhs.m_structure){
        //Checks to make sure the priority functions are the same before merging
        if(m_priorFunc == rhs.m_priorFunc){
            //Protection against self merging
            if(m_heap != rhs.m_heap){
                //If the heapTypes arent the same rebuild the rhs heap so it can be easily inserted into current heap
                if(m_heapType != rhs.m_heapType)
                    rhs.setPriorityFn(m_priorFunc, m_heapType);

                //Adds the sizes together
                m_size = m_size + rhs.m_size;
                //Merges the heaps together
                m_heap = mergeHeaps(m_heap, rhs.m_heap);

                //Sets the rhs object to empty
                rhs.m_heap = nullptr;
                rhs.m_size = 0;
            }
            else
                throw domain_error("ERROR: Function attempts to self merge. Self merging is not allowed!");
        }
        else
            throw domain_error("ERROR: Cannot merge two heaps with different priority functions!");
    }
    else
        throw domain_error("ERROR: Cannot merge two heaps with different structures!");


}

void PQueue::insertPatient(const Patient& patient) {
    //Makes a new node(heap) to merge and increments size
    Node* toInsert = new Node(patient);
    m_size++;

    //Merges m_heap with the new node its adding
    m_heap = mergeHeaps(m_heap, toInsert);
}

int PQueue::numPatients() const{
    //returns number of patients (aka size of heap)
    return m_size;
}

prifn_t PQueue::getPriorityFn() const {
    //Returns the priority function
    return m_priorFunc;
}

Patient PQueue::getNextPatient() {
    //If the size is not 0 removes a patient, if it is it throws an error
    if(m_size != 0){
        //Saves the children of the top node to merge them after removal of root of heap
        Node* leftHeap = m_heap->m_left;
        Node* rightHeap = m_heap->m_right;

        //Gets the top value and deletes the root node
        Patient patient = m_heap->getPatient();
        delete m_heap;
        ///CAUSES ERROR WITH removing 1 probably because I always increment size in merge
        //Subtracts 2 because mergeHeaps() always adds 1 when it find a nullptr
        m_size--;

        //Merges the two heaps into one heap and returns the patient
        m_heap = mergeHeaps(leftHeap, rightHeap);
        return patient;
    }
    else
        throw out_of_range("ERROR: Cannot dequeue an empty queue!");
}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    //Updates members given new conditions
    m_priorFunc = priFn;
    m_heapType = heapType;

    //Makes a pointer to a new heap
    Node* newHeap = nullptr;

    //Passes the new heap by reference and the oldHeap by value and inserts each node of the oldHeap one by one into the new heap
    remakeHeap(m_heap, newHeap);

    //Updates the heap to newly made heap
    m_heap = newHeap;
}

void PQueue::setStructure(STRUCTURE structure){
    //Updates member variable given new condition
    m_structure = structure;

   //Makes a pointer to a new heap
    Node* newHeap = nullptr;

    //Passes the new heap by reference and goes through the old heap and inserts each node one by one
    remakeHeap(m_heap, newHeap);

    //Updates the heap to newly made heap
    m_heap = newHeap;
}

STRUCTURE PQueue::getStructure() const {
    //Returns the structure of the heap
    return m_structure;
}

void PQueue::printPatientQueue() const {
    //If the heap isn't empty then print the list of patients in preOrder
    if(m_heap != nullptr){
        cout << "Contents of the queue:" << endl;
        printHeap(m_heap);
    }
    else
        cout << "Empty Patient Queue.\n" << endl;
}

void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient()
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}
