// CMSC 341 - Fall 2023 - Project 3

#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);

// a name database for testing purposes
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green",
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong",
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:
        //Checks to see whether the PQueue is empty
        bool empty(PQueue &heap){
            if(heap.m_heap == nullptr && heap.m_size == 0)
                return true;
            else
                return false;
        }

        //Creates a new PQueue given priority, type, and structure and a size
        PQueue createHeap(prifn_t func, HEAPTYPE type, STRUCTURE struc, int size){
            //Creates new heap given member variables
            PQueue newHeap(func, type, struc);

            //Random class calls for patient variables
            Random name(0,NUMNAMES-1);
            Random temp(MINTEMP,MAXTEMP);
            Random oxygen(MINOX,MAXOX);
            Random resp(MINRR,MAXRR);
            Random blood(MINBP,MAXBP);
            Random opinion(MINOPINION,MAXOPINION);

            //This for loop creates each patient and inserts them into the PQueue until the size is reached
            for(int i = 0; i < size; i++){
                string patientName = nameDB[name.getRandNum()];
                int patientTemp = temp.getRandNum();
                int patientO2 = oxygen.getRandNum();
                int patientResp = resp.getRandNum();
                int patientBP = blood.getRandNum();
                int patientOpinion = opinion.getRandNum();
                Patient patient(patientName, patientTemp, patientO2, patientResp, patientBP, patientOpinion);
                newHeap.insertPatient(patient);
            }
            return newHeap;
        }

        ///THIS FUNCTION IS THE SAME AS THE PREVIOUS CREATE HEAP IT JUST ALSO HAS A VECTOR TO HELP WITH CHECKING REBUILDING
        //Creates a new PQueue given priority, type, and structure and a size, also modifies a vector
        PQueue createHeapAndVector(prifn_t func, HEAPTYPE type, STRUCTURE struc, int size, vector<Patient> &patientVector){
            //Creates new heap given member variables
            PQueue newHeap(func, type, struc);

            //Random class calls for patient variables
            Random name(0,NUMNAMES-1);
            Random temp(MINTEMP,MAXTEMP);
            Random oxygen(MINOX,MAXOX);
            Random resp(MINRR,MAXRR);
            Random blood(MINBP,MAXBP);
            Random opinion(MINOPINION,MAXOPINION);

            //This for loop creates each patient and inserts them into the PQueue until the size is reached
            for(int i = 0; i < size; i++){
                string patientName = nameDB[name.getRandNum()];
                int patientTemp = temp.getRandNum();
                int patientO2 = oxygen.getRandNum();
                int patientResp = resp.getRandNum();
                int patientBP = blood.getRandNum();
                int patientOpinion = opinion.getRandNum();
                Patient patient(patientName, patientTemp, patientO2, patientResp, patientBP, patientOpinion);
                newHeap.insertPatient(patient);
                patientVector.push_back(patient);
            }
            return newHeap;
        }

        //This function is the main call to check to see if the heaps property is correct
        bool heapChecker(PQueue &heap){
            //If the heap is empty it automatically follows the properties
            if(heap.m_heap == nullptr)
                return true;
            //Otherwise calls recursive function to see if heap is proper
            else
                return heapPropertyCheck(heap.m_heap, heap.m_heapType, heap.m_priorFunc);
        }

        //This function checks to see if the NPL values of each node in the tree are proper
        bool checkAllNPLs(PQueue &heap){
            //Makes sure the heap its checking is a LEFTIST heap, if it is checks to see if nodes have proper NPL values, otherwise returns false
            if(heap.m_structure == LEFTIST){
                //If the heap is empty it automatically fulfills the property
                if(heap.m_heap == nullptr)
                    return true;
                //If it isn't it will recursively check each node in the trees children to ensure the NPL value for each node is right
                else
                    return checkAllNPLsRecursive(heap.m_heap);
            }
            else
                return false;
        }

        //This function checks to see if the NPL values of the left trees are greater or equal to those of the right trees
        bool checkLeftistProperty(PQueue &heap){
            //Makes sure the heap its checking is a LEFTIST heap, if it is checks to see if nodes have proper NPL values, otherwise returns false
            if(heap.m_structure == LEFTIST){
                //If the heap is empty it automatically fulfills the property
                if(heap.m_heap == nullptr)
                    return true;
                //If it isn't it will recursively check each node in the trees children to ensure the NPL value is greater on the left
                else
                    return leftistPropertyCheck(heap.m_heap);
            }
            else
                return false;
        }

        //This function removes all the nodes from a heap and returns whether the remove in the correct way or not (AKA in order depending on priority)
        bool checkRemoval(PQueue &heap){
            //Checks to see that the heap isn't empty before attempting removal
            if(heap.m_heap != nullptr){
                //Gets the priority function used in the heap to compare values after removal
                //to ensure nodes are removed in correct order
                prifn_t func = heap.getPriorityFn();

                //Declaration for an int to be used int the function later
                int priorityGotten;

                //Declaration for a bool to check if the leftist property is maintained if its leftist
                //Otherwise will always be true and wont affect results
                bool checkLeftIfNecc = true;

                //Checks to see the heapType and does the proper removal
                if(heap.m_heapType == MINHEAP){
                    //Gets initial min value by removing first patient and getting its priority
                    int min = func(heap.m_heap->getPatient());

                    //This if statement checks to see if the leftist properties are maintained each step of the removal
                    if(heap.m_structure == LEFTIST)
                        checkLeftIfNecc = checkAllNPLs(heap) && checkLeftistProperty(heap);

                    //This while loop removes the top priority node and compares it to the last priority node
                    //if it is greater than or equal to the last one (MINHEAP property) it keeps removing and
                    //updates the min otherwise if not it returns false
                    while(heap.numPatients() != 0){
                        priorityGotten = func(heap.getNextPatient());
                        //If the structure is a leftist tree it updates the bool and checks to see if the properties are preserved during each removal
                        if(heap.m_structure == LEFTIST)
                            checkLeftIfNecc = checkAllNPLs(heap) && checkLeftistProperty(heap);
                        //Checks to see if removal is proper and then updates the min
                        //Also checks to see if heapProperty is maintained after removal and if leftist properties are preserved if its a leftist struct
                        if(priorityGotten >= min && heapChecker(heap) && checkLeftIfNecc)
                            min = priorityGotten;
                        else
                            return false;
                    }
                }
                //Checks to see the heapType and does the proper removal
                else if(heap.m_heapType == MAXHEAP){
                    //Gets initial max value by removing first patient and getting its priority
                    int max = func(heap.m_heap->getPatient());

                    //This if statement checks to see if the leftist properties are maintained each step of the removal
                    if(heap.m_structure == LEFTIST)
                        checkLeftIfNecc = checkAllNPLs(heap) && checkLeftistProperty(heap);

                    //This while loop removes the top priority node and compares it to the last priority node
                    //if it is less than or equal to the last one (MAXHEAP property) it keeps removing and
                    //updates the min otherwise if not it returns false
                    while(heap.numPatients() != 0){
                        int priorityGotten = func(heap.getNextPatient());
                        //If the structure is a leftist tree it updates the bool and checks to see if the properties are preserved during each removal
                        if(heap.m_structure == LEFTIST)
                            checkLeftIfNecc = checkAllNPLs(heap) && checkLeftistProperty(heap);

                        //Checks to see if removal is proper and then updates the min
                        //Also checks to see if heapProperty is maintained after removal and if leftist properties are preserved if its a leftist struct
                        if(priorityGotten <= max && heapChecker(heap) && checkLeftIfNecc)
                            max = priorityGotten;
                        else
                            return false;
                    }
                }
                //If it exits the while loop and if statement then all the nodes were removed correctly
                //It would've returned false earlier if not
                return true;
            }
            //If there are no nodes to remove then return false because function should not be called on empty object
            else
                return false;
        }

        //Takes in two PQueues and checks to see if they are equal
        bool heapComparer(PQueue &heapOne, PQueue &heapTwo){
        //If both m_heaps are nullptr they are both empty
        if(heapOne.m_heap == nullptr && heapTwo.m_heap == nullptr)
            return true;
        //If not checks each heap using a recursive function and returns whether they are equal
        else{
            if(recursiveNodeChecker(heapOne.m_heap, heapTwo.m_heap))
                return true;
            else
                return false;
        }
    }

        //Takes in a PQueue and a vector with all patients in the heap and attempts to find them all
        //If it finds them all and then the rebuild was successful
        bool rebuildChecker(PQueue &heap, vector<Patient> &patientVector){
            //Checks to make sure that heap isnt empty
            if(heap.m_heap != nullptr){
                //Checks to make sure there are the correct number of patients (should be the same) in both
                //the vector and the heap
                int vectorSize = patientVector.size();
                if(heap.numPatients() == vectorSize){
                    //Gets the bool returnVal from the helper function which checks to see that the rebuild was made
                    //with the same data
                    bool returnVal = rebuildCheckerRecursive(heap.m_heap, patientVector);

                    //If the patientVector is empty and the returnVal is true, it means all the patients were found and the rebuild was successful
                    //Also check to see if heapProperties are maintained
                    vectorSize = patientVector.size();
                    if(vectorSize == 0 && returnVal && heapChecker(heap))
                        return true;
                    else
                        return false;
                }
            }
            return false;
        }

    ///I put all the helper functions used in tester functions in private to help
    ///differentiate between them because I got too lazy to rename them
    private:
        ///THESE FUNCTIONS ARE HELPERS FOR CHECKING TO SEE IF THE LEFTIST PROPERTY IS PRESERVED
        //Checks the property of all nodes in the tree for the LEFTIST PROPERTY and returns whether they do or not
        bool leftistPropertyCheck(Node* node){
            //If the node is empty returns true, it doesnt need to check it
            if(node == nullptr)
                return true;

            //Checks children for proper NPLs
            bool leftSide = leftistPropertyCheck(node->m_left);
            bool rightSide = leftistPropertyCheck(node->m_right);

            //Passes the node to a function that checks if the NPL values are proper
            bool properNPL = checkNPLBalance(node);
            return properNPL && leftSide && rightSide;
        }
        //Checks the NPL value the node and returns whether its proper LEFTIST property or not
        bool checkNPLBalance(Node* node){
            //NPL values for nullptrs
            int leftNPL = -1;
            int rightNPL = -1;

            //Booleans to see if children exist
            bool left = node->m_left != nullptr;
            bool right = node->m_right != nullptr;

            //Updates the NPL values based on children if they exist
            if(left)
                leftNPL = node->m_left->getNPL();
            if(right)
                rightNPL = node->m_right->getNPL();

            //If LEFTIST property is preserved returns true, otherwise returns false
            if(leftNPL >= rightNPL)
                return true;
            else
                return false;
        }


        ///THESE FUNCTIONS ARE HELPER FUNCTIONS TO MAKE SURE THE HEAP PROPERTIES ARE PRESERVED GIVEN A HEAPTYPE(MIN OR MAX)
        //This recursive function checks to see that the heaps order is correct given its type and priority
        bool heapPropertyCheck(Node* heap, HEAPTYPE type, prifn_t func){
            if(heap == nullptr)
                return true;

            bool leftSide = heapPropertyCheck(heap->m_left, type, func);
            bool rightSide = heapPropertyCheck(heap->m_right, type, func);

            bool inspectNode = inspectNodeProperties(heap, type, func);
            return inspectNode && leftSide && rightSide;
        }
        //This function checks the nodes children and checks to make sure given the HEAPTYPE
        // and priority that the nodes are in the correct order
        bool inspectNodeProperties(Node* node, HEAPTYPE type, prifn_t func){
            //Checks to see if children exist
            bool left = node->m_left != nullptr;
            bool right = node->m_right != nullptr;

            //Sets up the return val bool
            bool result = true;

            //This if statement checks the left child to see if it has the correct heap properties
            if(left){
                //If the heap is a MAXHEAP it checks to see that the node's priority is greater than or equal to
                //Its child node, and then updates the bool accordingly
                if(type == MAXHEAP){
                    if(func(node->getPatient()) >= func(node->m_left->getPatient()))
                        result = result && true;
                    else
                        result = result && false;
                }
                //If the heap is a MINHEAP it checks to see that the node's priority is less than or equal to
                //Its childs node, and then updates the bool accordingly
                else if(type == MINHEAP){
                    if(func(node->getPatient()) <= func(node->m_left->getPatient()))
                        result = result && true;
                    else
                        result = result && false;
                }
            }
            //This if statement checks the right child to see if it has the correct heap properties
            if(right){
                //If the heap is a MAXHEAP it checks to see that the node's priority is greater than or equal to
                //Its child node, and then updates the bool accordingly
                if(type == MAXHEAP){
                    if(func(node->getPatient()) >= func(node->m_right->getPatient()))
                        result = result && true;
                    else
                        result = result && false;
                }
                //If the heap is a MINHEAP it checks to see that the node's priority is less than or equal to
                //Its childs node, and then updates the bool accordingly
                else if(type == MINHEAP){
                    if(func(node->getPatient()) <= func(node->m_right->getPatient()))
                        result = result && true;
                    else
                        result = result && false;
                }
            }
            //Returns the result
            return result;
        }


        ///THESE FUNCTIONS ARE USED TO CALCULATE AS HELPERS FOR CHECKING THE NODES NPL VALUES
        //Checks the property of all nodes in the tree for the LEFTIST PROPERTY and returns whether they do or not
        bool checkAllNPLsRecursive(Node* node){
            //If the node is empty returns true, it doesnt need to check it
            if(node == nullptr)
                return true;

            //Checks children for proper NPLs
            bool leftSide = checkAllNPLsRecursive(node->m_left);
            bool rightSide = checkAllNPLsRecursive(node->m_right);

            //Passes the node to a function that checks if the NPL values are proper
            bool properNPL = checkEachNPL(node);
            return properNPL && leftSide && rightSide;
        }
        //This checks the NPL of a node and determines whether it is what it should be
        bool checkEachNPL(Node* node){
            //Booleans to see if children exist
            bool left = node->m_left != nullptr;
            bool right = node->m_right != nullptr;
            int correctNPL;

            //If one of the children is a nullptr then the NPL is 0
            if(!left || !right)
                correctNPL = 0;
            //If both children exist calculate the NPL the node should have
            else if(left && right){
                if(node->m_left->getNPL() > node->m_right->getNPL())
                    correctNPL = node->m_right->getNPL() + 1;
                else if(node->m_left->getNPL() <= node->m_right->getNPL())
                    correctNPL = node->m_left->getNPL() + 1;
            }
            //If the correct NPL is equal to the nodes NPL then returns true
            if(node->getNPL() == correctNPL)
                return true;
            else
                return false;
        }


        ///THIS FUNCTION IS A HELPER TO SEE IF TWO HEAPS TREES ARE EQUAL
        //This function takes in two nodes and compares whether they are equal and then moves onto the next nodes recursively
        bool recursiveNodeChecker(Node* nodeOne, Node* nodeTwo){
            //If both nodes are empty the heap is equal
            if(nodeOne == nullptr && nodeTwo == nullptr)
                return true;
            //If both of the nodes exist then compare them both, if not return false
            if(nodeOne != nullptr && nodeTwo != nullptr){
                //Checks both children to see if they're equal
                bool resultLeft = recursiveNodeChecker(nodeOne->m_left, nodeTwo->m_left);
                bool resultRight = recursiveNodeChecker(nodeOne->m_right, nodeTwo->m_right);

                //Compares the nodes to see if they are equal and returns if they are or not
                if(nodeOne->getPatient() == nodeTwo->getPatient())
                    return true && resultLeft && resultRight;
                else
                    return false && resultLeft && resultRight;
            }
            else
                return false;
        }

        ///THIS FUNCTION IS A HELPER FUNCTION TO SEE IF THE REBUILD WAS SUCCESSFUL AND USED THE SAME DATA
        //This function checks through the tree to see if the same data was used to remake the tree
        //It checks it by looking through a vector containing all the patients in the heap
        bool rebuildCheckerRecursive(Node* node, vector<Patient> &patientVector){
            //If the node is empty returns true, it doesnt need to check it
            if(node == nullptr)
                return true;

            //Checks children for patient in the vector
            bool leftSide = rebuildCheckerRecursive(node->m_left, patientVector);
            bool rightSide = rebuildCheckerRecursive(node->m_right, patientVector);

            //Gets the size of the vector and iterates through to find the same patient as the node
            int vectorSize = patientVector.size();
            //Assuming it iterates through the loop and doesnt find the patient will return false after break outs of loop
            int patientExists = false;

            //This for loop goes through the vector in the reverse order to prevent memory errors
            for(int i = vectorSize -1; i >= 0; i--){
                //If it finds the same patient as the node it erases it, sets the bool of the patient
                //existing to true and sets i = 0 ending the loop to prevent deletion of other patients
                //incase there are duplicates
                if(node->getPatient() == patientVector[i]){
                    patientVector.erase(patientVector.begin() + i);
                    patientExists = true;
                    i = 0;
                }
            }
            //returns the return val and the ones gotten from the left and right
            return patientExists && leftSide && rightSide;
        }
};

int main(){
    //Strings used to help print the results of each test
    string pass = "\t\tTest Passed: ";
    string fail = "\t!!!!!!!!Test Failed: ";

    Tester tester;

    //Variable to edit the amount of nodes that will be inserted into the queue/heap
    int insertTestSize = 300;

    cout << "-----Testing PQueue Class" << endl << endl;

    cout << "-----TESTING INSERTION-----" << endl;
    {
        cout << "\t(MINHEAP) Testing normal MINHEAP cases..." << endl;
        //This statement checks to see that the MINHEAP properties are preserved in a SKEW heap after insertion specified size above
        //This uses priorityfunction1
        {
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MINHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn1!" << endl;
            else
                cout << fail << "didn't preserve MINHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn1!" << endl;
        }

        //This statement checks to see that the MINHEAP properties are preserved in a LEFTIST heap after insertion specified size above
        //This uses priorityfunction1
        {
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, LEFTIST, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MINHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn1!" << endl;
            else
                cout << fail << "didn't preserve MINHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn1!" << endl;
        }

        //This statement checks to see that the MINHEAP properties are preserved in a SKEW heap after insertion specified size above
        //This uses priorityfunction2
        {
            PQueue aQueue = tester.createHeap(priorityFn2, MINHEAP, SKEW, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MINHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn2!" << endl;
            else
                cout << fail << "didn't preserve MINHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn2!" << endl;
        }

        //This statement checks to see that the MINHEAP properties are preserved in a LEFTIST heap after insertion specified size above
        //This uses priorityfunction2
        {
            PQueue aQueue = tester.createHeap(priorityFn2, MINHEAP, LEFTIST, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MINHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn2!" << endl;
            else
                cout << fail << "didn't preserve MINHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn2!" << endl;
        }
        //END OF TESTING FOR MINHEAP INSERTION

        cout << "\t(MAXHEAP) Testing normal MAXHEAP cases..." << endl;
        //This statement checks to see that the MAXHEAP properties are preserved in a SKEW heap after insertion specified size above
        //This uses priorityfunction1
        {
            PQueue aQueue = tester.createHeap(priorityFn1, MAXHEAP, SKEW, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MAXHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn1!" << endl;
            else
                cout << fail << "didn't preserve MAXHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn1!" << endl;
        }

        //This statement checks to see that the MAXHEAP properties are preserved in a LEFTIST heap after insertion specified size above
        //This uses priorityfunction1
        {
            PQueue aQueue = tester.createHeap(priorityFn1, MAXHEAP, LEFTIST, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MAXHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn1!" << endl;
            else
                cout << fail << "didn't preserve MAXHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn1!" << endl;
        }

        //This statement checks to see that the MAXHEAP properties are preserved in a SKEW heap after insertion specified size above
        //This uses priorityfunction2
        {
            PQueue aQueue = tester.createHeap(priorityFn2, MAXHEAP, SKEW, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MAXHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn2!" << endl;
            else
                cout << fail << "didn't preserve MAXHEAP properties after " << insertTestSize << " insertions in a SKEW heap using priorityFn2!" << endl;
        }

        //This statement checks to see that the MAXHEAP properties are preserved in a LEFTIST heap after insertion specified size above
        //This uses priorityfunction2
        {
            PQueue aQueue = tester.createHeap(priorityFn2, MAXHEAP, LEFTIST, insertTestSize);
            if(tester.heapChecker(aQueue))
                cout << pass << "preserves MAXHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn2!" << endl;
            else
                cout << fail << "didn't preserve MAXHEAP properties after " << insertTestSize << " insertions in a LEFTIST heap using priorityFn2!" << endl;
        }
        //END OF TESTING FOR MAXHEAP INSERTION
    }
    //End of tests for insertion

    cout << endl << "-----TESTING REMOVAL-----" << endl;
    {
        cout << "\t(MINHEAP) Testing normal MINHEAP cases..." << endl;
        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction1, SKEW and MINHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from SKEW MINHEAP using priorityFn1!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from SKEW MINHEAP using priorityFn1!" << endl;
        }

        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction1, LEFTIST and MINHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, LEFTIST, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from LEFTIST MINHEAP using priorityFn1!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from LEFTIST MINHEAP using priorityFn1!" << endl;
        }

        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction2, SKEW and MINHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MINHEAP, SKEW, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from SKEW MINHEAP using priorityFn2!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from SKEW MINHEAP using priorityFn2!" << endl;
        }

        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction1, LEFTIST and MINHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MINHEAP, LEFTIST, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from LEFTIST MINHEAP using priorityFn2!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from LEFTIST MINHEAP using priorityFn2!" << endl;
        }

        cout << "\t(MAXHEAP) Testing normal MAXHEAP cases..." << endl;
        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction1, SKEW and MAXHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MAXHEAP, SKEW, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from SKEW MAXHEAP using priorityFn1!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from SKEW MAXHEAP using priorityFn1!" << endl;
        }

        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction1, LEFTIST and MAXHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MAXHEAP, LEFTIST, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from LEFTIST MAXHEAP using priorityFn1!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from LEFTIST MAXHEAP using priorityFn1!" << endl;
        }

        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction2, SKEW and MAXHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MAXHEAP, SKEW, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from SKEW MAXHEAP using priorityFn2!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from SKEW MAXHEAP using priorityFn2!" << endl;
        }

        //This function block tests to see if all removals happen in the correct order
        //This uses priorityfunction1, LEFTIST and MAXHEAP properties
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MAXHEAP, LEFTIST, insertTestSize);
            //This if statement calls a function that checks to make sure it removes each node in order
            //it also checks to make sure properties of heap and leftist struct if used are preserved after each removal
            if(tester.checkRemoval(aQueue))
                cout << pass << "removes all nodes correctly from LEFTIST MAXHEAP using priorityFn2!" << endl;
            else
                cout << fail << "does not remove all nodes correctly from LEFTIST MAXHEAP using priorityFn2!" << endl;
        }

        cout << "\t(ERROR) Testing error cases..." << endl;
        //Attempts to remove from an empty heap
        //Should throw out of range error
        {
            //Makes an empty queue, types do no matter since no data is in the queue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, 0);
            try{
                //Attempts to dequeue empty queue
                aQueue.getNextPatient();
                cout << fail << "outOfRange error was not caught while trying to dequeue empty queue!" << endl;
            }
            catch(exception &e){
                cout << pass << "outOfRange error was caught when trying to dequeue empty queue!" << endl;
                cout << "\t\t\t" << e.what() << endl;
            }
        }

    }
    //End of tests for removal

    cout << endl << "-----TESTING LEFTIST PROPERTIES-----" << endl;
    cout << "*****Testing statements for: preservation of LHP(Leftist Heap Property)*****" << endl;
    {
        cout << "\t(NORMAL) Testing normal cases..." << endl;
        //Tests whether the leftist property is preserved when making a LEFTIST MINHEAP
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, LEFTIST, insertTestSize);
            //Checks to see if leftist property is valid through entire heap
            if(tester.checkLeftistProperty(aQueue))
                cout << pass << "MINHEAP preserves LHP using priorityFn1!" << endl;
            else
                cout << fail << "MINHEAP does not preserve LHP using priorityFn1!" << endl;
        }

        //Tests whether the leftist property is preserved when making a LEFTIST MAXHEAP
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MAXHEAP, LEFTIST, insertTestSize);
            //Checks to see if leftist property is valid through entire heap
            if(tester.checkLeftistProperty(aQueue))
                cout << pass << "MAXHEAP preserves LHP using priorityFn1!" << endl;
            else
                cout << fail << "MAXHEAP does not preserve LHP using priorityFn1!" << endl;
        }

        //Tests whether the leftist property is preserved when making a LEFTIST MINHEAP
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MINHEAP, LEFTIST, insertTestSize);
            //Checks to see if leftist property is valid through entire heap
            if(tester.checkLeftistProperty(aQueue))
                cout << pass << "MINHEAP preserves LHP using priorityFn2!" << endl;
            else
                cout << fail << "MINHEAP does not preserve LHP using priorityFn2!" << endl;
        }

        //Tests whether the leftist property is preserved when making a LEFTIST MAXHEAP
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MAXHEAP, LEFTIST, insertTestSize);
            //Checks to see if leftist property is valid through entire heap
            if(tester.checkLeftistProperty(aQueue))
                cout << pass << "MAXHEAP preserves LHP using priorityFn2!" << endl;
            else
                cout << fail << "MAXHEAP does not preserve LHP using priorityFn2!" << endl;
        }
    }
    cout << "*****Testing statements for: correct NPL values*****" << endl;
    {
        cout << "\t(NORMAL) Testing normal cases..." << endl;
        //Tests whether the NPL value of each node is what it should be
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, LEFTIST, insertTestSize);
            //Checks to see if all nodes have the proper NPL value they should
            if(tester.checkAllNPLs(aQueue))
                cout << pass << "all nodes have proper NPL values in MINHEAP using priorityFn1!" << endl;
            else
                cout << pass << "not nodes have proper NPL values in MINHEAP using priorityFn1!" << endl;
        }

        //Tests whether the NPL value of each node is what it should be
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MAXHEAP, LEFTIST, insertTestSize);
            //Checks to see if all nodes have the proper NPL value they should
            if(tester.checkAllNPLs(aQueue))
                cout << pass << "all nodes have proper NPL values in MAXHEAP using priorityFn1!" << endl;
            else
                cout << pass << "not nodes have proper NPL values in MAXHEAP using priorityFn1!" << endl;
        }

        //Tests whether the NPL value of each node is what it should be
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MINHEAP, LEFTIST, insertTestSize);
            //Checks to see if all nodes have the proper NPL value they should
            if(tester.checkAllNPLs(aQueue))
                cout << pass << "all nodes have proper NPL values in MINHEAP using priorityFn2!" << endl;
            else
                cout << pass << "not nodes have proper NPL values in MINHEAP using priorityFn2!" << endl;
        }

        //Tests whether the NPL value of each node is what it should be
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn2, MAXHEAP, LEFTIST, insertTestSize);
            //Checks to see if all nodes have the proper NPL value they should
            if(tester.checkAllNPLs(aQueue))
                cout << pass << "all nodes have proper NPL values in MAXHEAP using priorityFn2!" << endl;
            else
                cout << pass << "not nodes have proper NPL values in MAXHEAP using priorityFn2!" << endl;
        }
        ///In my function checkRemoval I already have a checker if the heap is a leftist heap to check both of these things
        cout << "$$$Don't need to check these properties after removal as checkRemoval() checks these properties after each removal :)" << endl;
    }
    //End of tests for leftist properties

    cout << endl << "-----TESTING MERGEQUEUES-----" << endl;
    {
        cout << "\t(NORMAL) Testing normal cases..." << endl;
        //Attempts to merge two priFn1, SKEW MINHEAPS
        {
            //Makes two of the same heap type but different variables heap
            vector<Patient> patVec;
            PQueue aQueue = tester.createHeapAndVector(priorityFn1, MINHEAP, SKEW, 5, patVec);

            vector<Patient> patVec2;
            PQueue bQueue = tester.createHeapAndVector(priorityFn1, MINHEAP, SKEW, 10, patVec2);

            //This small block of code adds each vector together to get the total nodes across both in a single vector
            int vectorTwoSize = patVec2.size();
            for(int i = 0; i < vectorTwoSize; i++){
                patVec.push_back(patVec2[i]);
            }

            //Attempts to merge the heaps together
            aQueue.mergeWithQueue(bQueue);

            //Checks to see whether the heap property is maintained after merging and that no data is lost and that the rhs Queue is empty
            if(tester.heapChecker(aQueue) && tester.rebuildChecker(aQueue, patVec) && tester.empty(bQueue))
                cout << pass << "mergeQueues() with queue works properly for two MIN SKEW, no data is lost!" << endl;
            else
                cout << fail << "mergeQueues() doesn't work properly under two MIN SKEW conditions!" << endl;
        }
        //Attempts to merge two priFn2, LEFTIST MAXHEAP
        {
            //Makes two of the same heap type but different variables heap
            vector<Patient> patVec;
            PQueue aQueue = tester.createHeapAndVector(priorityFn2, MAXHEAP, LEFTIST, 50, patVec);

            vector<Patient> patVec2;
            PQueue bQueue = tester.createHeapAndVector(priorityFn2, MAXHEAP, LEFTIST, 35, patVec2);

            //This small block of code adds each vector together to get the total nodes across both in a single vector
            int vectorTwoSize = patVec2.size();
            for(int i = 0; i < vectorTwoSize; i++){
                patVec.push_back(patVec2[i]);
            }

            //Attempts to merge the heaps together
            aQueue.mergeWithQueue(bQueue);

            //Checks to see whether the heap property is maintained after merging and that no data is lost and that the rhs Queue is empty
            if(tester.heapChecker(aQueue) && tester.rebuildChecker(aQueue, patVec) && tester.empty(bQueue))
                cout << pass << "mergeQueues() with queue works properly for two MAX LEFTIST, no data is lost!" << endl;
            else
                cout << fail << "mergeQueues() doesn't work properly under two MAX LEFTIST conditions!" << endl;
        }
        //Attempts to merge two heaps with different heapTypes
        {
            //Makes two of the same heap type but different variables heap
            vector<Patient> patVec;
            PQueue aQueue = tester.createHeapAndVector(priorityFn2, MINHEAP, LEFTIST, 15, patVec);

            vector<Patient> patVec2;
            PQueue bQueue = tester.createHeapAndVector(priorityFn2, MAXHEAP, LEFTIST, 10, patVec2);

            //This small block of code adds each vector together to get the total nodes across both in a single vector
            int vectorTwoSize = patVec2.size();
            for(int i = 0; i < vectorTwoSize; i++){
                patVec.push_back(patVec2[i]);
            }

            //Attempts to merge the heaps together
            aQueue.mergeWithQueue(bQueue);

            //Checks to see whether the heap property is maintained after merging and that no data is lost and that the rhs Queue is empty
            //Also checks leftist properties because it is a leftist heap
            if(tester.heapChecker(aQueue) && tester.rebuildChecker(aQueue, patVec) && tester.empty(bQueue) && tester.checkAllNPLs(aQueue) && tester.checkLeftistProperty(aQueue))
                cout << pass << "mergeQueues() with queue works properly when merging two different heapTypes!" << endl;
            else
                cout << fail << "mergeQueues() doesn't work properly when merging two different heapTypes!" << endl;
        }
        cout << "\t(ERROR) Testing error cases..." << endl;
        //Attempts to merge heaps of different priority functions
        {
            //Creates two different heaps
            PQueue aQueue = tester.createHeap(priorityFn2, MINHEAP, SKEW, 5);
            PQueue bQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, 10);

            //Attempts to merge the heaps together
            try{
                aQueue.mergeWithQueue(bQueue);
                cout << fail << "mergeQueue() did not catch error of different priorityFns!" << endl;
            }
            catch(exception &e){
                cout << pass << "mergeQueue() successfully caught error when attempting merge with different priorityFns!" << endl;
                cout << "\t\t\t" << e.what() << endl;
            }
        }
        //Attempts to merge heaps of different structures
        {
            //Makes two different heaps
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, 50);
            PQueue bQueue = tester.createHeap(priorityFn1, MINHEAP, LEFTIST, 10);

            //Attempts to merge the heaps together
            try{
                aQueue.mergeWithQueue(bQueue);
                cout << fail << "mergeQueue() did not catch error of different structures!" << endl;
            }
            catch(exception &e){
                cout << pass << "mergeQueue() successfully caught error of different structures!" << endl;
                cout << "\t\t\t" << e.what() << endl;
            }
        }
        //Attempts self merging
        {
            //Makes heap
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, 50);

            //Attempts to merge the heap with itself
            try{
                aQueue.mergeWithQueue(aQueue);
                cout << fail << "mergeQueue() did not catch error of attempted self merging!" << endl;
            }
            catch(exception &e){
                cout << pass << "mergeQueue() successfully caught error of attempted self merging!" << endl;
                cout << "\t\t\t" << e.what() << endl;
            }
        }
    }
    //End of tests for mergeQueues

    cout << endl << "-----TESTING REBUILDING HEAP-----" << endl;
    {
        cout << "\t(NORMAL) Testing normal cases..." << endl;
        //Attempts to rebuild a heap and switching the priority function
        {
            vector<Patient> patVec;
            //Makes the queue and creates a copy to compare to later
            PQueue aQueue = tester.createHeapAndVector(priorityFn2, MINHEAP, LEFTIST, 50, patVec);
            PQueue originalQueue(aQueue);

            //Rebuilds the heap using different priority
            aQueue.setPriorityFn(priorityFn1, MINHEAP);

            //Checks to see if rebuild was made with the same data, follows the heap properties, and isnt the same tree as before
            if(tester.rebuildChecker(aQueue, patVec) && tester.heapChecker(aQueue) && !tester.heapComparer(aQueue, originalQueue))
                cout << pass << "rebuild successful when changing priorityFn from 2 to 1!" << endl;
            else
                cout << fail << "rebuild unsuccessful heap not properly rebuilt!" << endl;
        }
        //Attempts to change from a type MINHEAP to MAXHEAP
        {
            vector<Patient> patVec;
            //Makes the queue and creates a copy to compare to later
            PQueue aQueue = tester.createHeapAndVector(priorityFn1, MINHEAP, SKEW, 50, patVec);
            PQueue originalQueue(aQueue);

            //Rebuilds the heap using different heapType
            aQueue.setPriorityFn(priorityFn1, MAXHEAP);

            //Checks to see if rebuild was made with the same data, follows the heap properties, and isnt the same tree as before
            if(tester.rebuildChecker(aQueue, patVec) && tester.heapChecker(aQueue) && !tester.heapComparer(aQueue, originalQueue))
                cout << pass << "rebuild successful when changing heapType from MIN to MAX!" << endl;
            else
                cout << fail << "rebuild unsuccessful heap not properly rebuilt from a MIN to a MAX heap!" << endl;
        }
        //Attempts to rebuild a heap and switching the STRUCTURE from SKEW to LEFTIST function
        {
            vector<Patient> patVec;
            //Makes the queue and creates a copy to compare to later
            PQueue aQueue = tester.createHeapAndVector(priorityFn1, MAXHEAP, SKEW, 50, patVec);
            PQueue originalQueue(aQueue);

            //Rebuilds the heap using different structure
            aQueue.setStructure(LEFTIST);

            //Checks to see if rebuild was made with the same data, follows the heap properties, and isnt the same tree as before and follows leftist properties
            if(tester.rebuildChecker(aQueue, patVec) && tester.heapChecker(aQueue) && !tester.heapComparer(aQueue, originalQueue) && tester.checkAllNPLs(aQueue) && tester.checkLeftistProperty(aQueue))
                cout << pass << "rebuild successful when changing structure from SKEW to LEFTIST!" << endl;
            else
                cout << fail << "rebuild unsuccessful, not properly rebuilt from SKEW to LEFTIST heap!" << endl;
        }
    }
    //End of tests for rebuilding

    cout << endl << "-----TESTING COPY CONSTRUCTOR-----" << endl;
    {
        cout << "\t(NORMAL) Testing normal cases..." << endl;
        //Tries copy constructor with prifn1 MINHEAP and SKEW types
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, insertTestSize);
            //Calls copy constructor on aQueue
            PQueue bQueue(aQueue);

            //Checks to see if the heaps were made the same
            if(tester.heapComparer(aQueue, bQueue))
                cout << pass << "copy successful on SKEW MINHEAP with priorityFn1!" << endl;
            else
                cout << fail << "copy unsuccessful on SKEW MINHEAP with priorityFn1!" << endl;
        }
        //Tries copy constructor with prifn2 MAXHEAP and LEFTIST types
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, insertTestSize);
            //Calls copy constructor on aQueue
            PQueue bQueue(aQueue);

            //Checks to see if the heaps were made the same
            if(tester.heapComparer(aQueue, bQueue))
                cout << pass << "copy successful on LEFTIST MAXHEAP with priorityFn2!" << endl;
            else
                cout << fail << "copy unsuccessful on LEFTIST MAXHEAP with priorityFn2!" << endl;
        }

        cout << "\t(EDGE) Testing edge cases..." << endl;
        //Tests copying empty object
        {
            //Makes the PQueue
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, 0);
            //Calls copy constructor on aQueue
            PQueue bQueue(aQueue);

            //Checks to see if the heaps were made the same in this case two empty heaps
            if(tester.heapComparer(aQueue, bQueue))
                cout << pass << "copy successful on empty heap, leaves 2 empty heaps!" << endl;
            else
                cout << fail << "copy unsuccessful on empty heap!" << endl;
        }
    }
    //End of tests for copy construct

    cout << endl << "-----TESTING ASSIGNMENT OPERATOR-----" << endl;
    {
        cout << "\t(NORMAL) Testing normal cases..." << endl;
        //Tests assigning two full different objects
        //Only testing once due to how assignment operators work
        {
            //Makes the PQueues
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, insertTestSize);
            PQueue bQueue = tester.createHeap(priorityFn2, MAXHEAP, LEFTIST, insertTestSize);

            //Assigns the heaps equal to each other
            aQueue = bQueue;

            //Checks to see if the heaps were set equal in this case two full heaps
            if(tester.heapComparer(aQueue, bQueue))
                cout << pass << "assignment successful, two equal heaps are returned!" << endl;
            else
                cout << fail << "assignment unsuccessful, heaps are not equal!" << endl;
        }

        cout << "\t(EDGE) Testing edge cases..." << endl;
        //Tests assigning empty object to full object
        {
            //Makes the PQueues
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, insertTestSize);
            PQueue bQueue = tester.createHeap(priorityFn2, MAXHEAP, LEFTIST, 0);

            //Assigns empty object to full object
            bQueue = aQueue;


            //Checks to see if the heaps were set equal the same in this case two full heaps
            if(tester.heapComparer(aQueue, bQueue) && bQueue.numPatients() > 0)
                cout << pass << "assignment successful, empty heap is now equal to the non-empty heap!" << endl;
            else
                cout << fail << "assignment unsuccessful, empty heap not assigned properly!" << endl;
        }
        //Tests assigning full object to empty object
        {
            //Makes the PQueues
            PQueue aQueue = tester.createHeap(priorityFn1, MINHEAP, SKEW, insertTestSize);
            PQueue bQueue = tester.createHeap(priorityFn2, MAXHEAP, LEFTIST, 0);

            //Assigns full object to empty object
            aQueue = bQueue;

            //Checks to see if the heaps were set equal in this case two empty heaps
            if(tester.heapComparer(aQueue, bQueue) && aQueue.numPatients() == 0)
                cout << pass << "assignment successful, non-empty heap now equal to empty heap!" << endl;
            else
                cout << fail << "assignment unsuccessful, non-empty heap not properly assigned to empty!" << endl;
        }
    }
    //End of tests for assignment operator

    return 0;
}

int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //temperature + respiratory + blood pressure
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //nurse opinion + oxygen
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}
