// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

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
    double sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize);

    bool insertTest(){
        bool all_result = true;

        // normal case: insert valid node into existing tree
        bool result = true;
        SatNet aNet;
        Sat aSat(10001, MI208, I48, ACTIVE);
        Sat bSat(20002, MI208, I48, ACTIVE);
        Sat cSat(30003, MI208, I48, ACTIVE);
        aNet.insert(aSat);
        aNet.insert(bSat);
        aNet.insert(cSat);
        result = result && (aNet.findSatellite(10001)&&aNet.findSatellite(20002)&&aNet.findSatellite(30003));
        std::cout << "Insert normal case: ";
        if (result){
            std::cout << "PASSED" << endl;
        } else {
            std::cout << "FAILED" << endl;
        }
        all_result = all_result && result;

        // edge case: insert node with duplicate id 
        result = true;
        Sat dSat(30003, MI208, I48, ACTIVE);
        try{
            aNet.insert(dSat);
        } catch(runtime_error& e) {
            result = result && (string(e.what())=="Duplicate ID. Insertion failed.");
        }
        std::cout << "Insert edge case: ";
        if (result){
            std::cout << "PASSED" << endl;
        } else {
            std::cout << "FAILED" << endl;
        }
        all_result = all_result && result;

        // error case: insert node with invalid id
        result = true;
        Sat eSat(1, MI208, I48, ACTIVE);
        try{
            aNet.insert(eSat);
        } catch(runtime_error& e){
            result = result && (string(e.what())=="Invalid ID. Insertion failed.");
        }
        std::cout << "Insert error case: ";
        if (result){
            std::cout << "PASSED" << endl;
        } else {
            std::cout << "FAILED" << endl;
        }
        all_result = all_result && result;

        return all_result;
    }

    bool removeTest(){
        bool all_result = true;
        SatNet aNet;
        Sat aSat(10001, MI208, I48, ACTIVE);
        Sat bSat(20002, MI208, I48, ACTIVE);
        Sat cSat(30003, MI208, I48, ACTIVE);
        aNet.insert(aSat);
        aNet.insert(bSat);
        aNet.insert(cSat);

        // normal case: remove valid node from existing tree
        bool result = true;
        aNet.remove(10001);
        result = result && (!aNet.findSatellite(10001));
        std::cout << "Remove normal case: ";
        if (result){
            std::cout << "PASSED" << endl;
        } else {
            std::cout << "FAILED" << endl;
        }
        all_result = all_result && result;

        // edge case: remove last node from existing tree
        result = true;
        aNet.remove(20002);
        aNet.remove(30003);
        result = result && (aNet.m_root==nullptr);
        std::cout << "Remove edge case: ";
        if (result){
            std::cout << "PASSED" << endl;
        } else {
            std::cout << "FAILED" << endl;
        }
        all_result = all_result && result;

        // error case: remove invalid node from an existing tree
        result = true;
        aNet.insert(aSat);
        aNet.insert(bSat);
        aNet.insert(cSat);
        try{
            aNet.remove(40004);
        } catch(runtime_error& e) {
            result = result && (string(e.what())=="ID not found. Removal failed.");
        }
        std::cout << "Remove error case: ";
        if (result){
            std::cout << "PASSED" << endl;
        } else {
            std::cout << "FAILED" << endl;
        }
        all_result = all_result && result;

        return all_result;
    }
    //i
    bool balanceTest(SatNet network){
        return network.isBalanced(network.m_root);
    }

    bool BSTTest(SatNet network){
        return network.isBST(network.m_root, nullptr, nullptr);
    }


};

int main(){
    Tester tester;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);  // there are 4 inclination
    Random altGen(0,3);     // there are 4 altitudes

    if(tester.insertTest()){
        std::cout << "InsertTest all cases passed!" << endl << endl;
    } else {
        std::cout << "InsertTest not all cases passed!" << endl << endl;
    }

    if(tester.removeTest()){
        std::cout << "RemoveTest all cases passed!" << endl << endl;
    } else {
        std::cout << "RemoveTest not all cases passed!" << endl << endl;
    }

    {
        SatNet network;
        int teamSize = 10;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<teamSize;i++){
            try{
                ID = idGen.getRandNum();
                if (i == teamSize / 2) tempID = ID;//we store this ID for later use
                Sat satellite(ID,
                            static_cast<ALT>(altGen.getRandNum()),
                            static_cast<INCLIN>(inclinGen.getRandNum()));
                network.insert(satellite);
            } catch (const runtime_error& e) {
                std::cout << string(e.what()) << endl;
            }  
        }
        std::cout << "\nDump after inserting " << teamSize << " nodes:\n\n";
        network.dumpTree();
        std::cout << "\n\nList of satellites after inserting " << teamSize << " nodes:\n";
        network.listSatellites();
        std::cout << endl;

        network.remove(tempID);
        std::cout << "\nDump after removig the node with ID: " << tempID << "\n\n";
        network.dumpTree();
        std::cout << "\n\nList of satellites after removing the node with ID: " << tempID << "\n";
        network.listSatellites();
        std::cout << endl;
    }
    SatNet network1;
    int size = 300;
    int tempIDs[301] = {0};
    int id = 0;
    for(int i=0;i<size;i++){
        try{
            id = idGen.getRandNum();
            tempIDs[i] = id;//we store this ID for later use
            Sat satellite(id,
                        static_cast<ALT>(altGen.getRandNum()),
                        static_cast<INCLIN>(inclinGen.getRandNum()));
            network1.insert(satellite);
        } catch (const runtime_error& e) {
            std::cout << string(e.what()) << endl;
        }
    }

    if(tester.balanceTest(network1)){
        std::cout << "BalanceTest after " << size << " insertions passed!" << endl;
    } else {
        std::cout << "BalanceTest after " << size << " insertions failed!" << endl;
    }
    if(tester.BSTTest(network1)){
        std::cout << "BSTTest after " << size << " insertions passed!" << endl << endl;
    } else {
        std::cout << "BSTTest after " << size << " insertions failed!" << endl << endl;
    }

    for(int i=0;i<(size/2);i++){
        try{
            network1.remove(tempIDs[i]);
        } catch (const runtime_error& e) {
            std::cout << string(e.what()) << endl;
        }
    }

    if(tester.balanceTest(network1)){
        std::cout << "BalanceTest after " << size/2 << " removals passed!" << endl;
    } else {
        std::cout << "BalanceTest after " << size/2 << " removals failed!" << endl;
    }
    if(tester.BSTTest(network1)){
        std::cout << "BSTTest after " << size/2 << " removals passed!" << endl << endl;
    } else {
        std::cout << "BSTTest after " << size/2 << " removals failed!" << endl << endl;
    }

    std::cout << endl << "Calling Tester::sampleTimeMeasurement(...): " << endl;
    std::cout << "\tFinding 1000 nodes takes " << tester.sampleTimeMeasurement(network1, tempIDs, size) << " seconds." << endl;
    
    return 0;
}

double Tester::sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize){
    double T = 0.0;//stores running times
    clock_t start, stop;//stores the clock ticks while running the program
    start = clock();
    // the algorithm to be measured
    for (int j=0;j<arraySize;j++){
        aNet.findSatellite(tempArray[j]);
    }
    stop = clock();
    T = stop - start;//number of clock ticks the algorithm took
    double measureTime = T/CLOCKS_PER_SEC;//time in seconds
    return measureTime;
}