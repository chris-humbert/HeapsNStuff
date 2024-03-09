#include "pqueue.h"
#include <random>
#include <vector>
using namespace std;

int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);

const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green",
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong",
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            m_generator = std::mt19937(m_device());
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            m_generator = std::mt19937(10);
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL){
            m_generator = std::mt19937(10);
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else {
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
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
        int result = 0;
        if(m_type == NORMAL){
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        double result = m_uniReal(m_generator);
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
};

int main(){
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i=0;i<10;i++){
        Patient patient(nameDB[nameGen.getRandNum()],
                    temperatureGen.getRandNum(),
                    oxygenGen.getRandNum(),
                    respiratoryGen.getRandNum(),
                    bloodPressureGen.getRandNum(),
                    nurseOpinionGen.getRandNum());
        aQueue.insertPatient(patient);
    }
    cout << "\nDump of the leftist heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    aQueue.setStructure(SKEW);
    cout << "\nDump of the skew heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn2 (MINHEAP):\n";
    aQueue.printPatientQueue();
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the skew queue with priorityFn1 (MAXHEAP):\n";
    aQueue.dump();

    return 0;
}

int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}
