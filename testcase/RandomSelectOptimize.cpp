#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <random>

using namespace std;

class RandomSelectOptimize
{
private:
    vector<int> _vec;
    int _offset;

public:
    double accuracy;
    double avgIteratTime;
    int iteratTime;
    int randomSelectBest; 
    bool acceptable;

    void randomSelect(vector<int> &vec);
    void randomSelect(vector<int> &vec, int k);
    void isAcceptable(int realBest);
    void getPerformanceInfo(int times, int realBest, int offset, vector<int>& vec);

};

void RandomSelectOptimize::randomSelect(vector<int> &vec)
{   
    int size = vec.size(); 
    int threshold = int(double(size) * 0.25);
    int tempMax = -1;

    // std::random_device rd;
    // std::default_random_engine rng(rd());
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng(seed);
    
    default_random_engine rng;
    shuffle(begin(vec), end(vec), eng);

    for(int i = 0; i < size; i++)
    {

        if(i < threshold) 
        {
            tempMax = tempMax > vec[i] ? tempMax : vec[i];    
            continue;
        }
        if(i == threshold)
            cout << "Temp Best is: " << tempMax << endl;

        int temp = vec[i];
        if(temp >= tempMax || temp >= tempMax - _offset)
        {
            randomSelectBest = temp;
            iteratTime = i + 1;
            return;
        }
    }

    randomSelectBest = vec[size - 1];
    iteratTime = size;
}


void RandomSelectOptimize::randomSelect(vector<int> &vec, int k)
{
    
}

void RandomSelectOptimize::isAcceptable(int realBest)
{
    acceptable = ((randomSelectBest == realBest) || 
    (randomSelectBest >= realBest - _offset)) ? true : false;
}

void RandomSelectOptimize::getPerformanceInfo(int times, int realBest, int offset, vector<int>& vec)
{
    _offset = offset;

    int totalIterateTime = 0;
    int totalAcceptTime = 0;
    
    for(int i = 0; i < times; i++)
    {
        cout << "No." << i << " Iteration." << endl;
        randomSelect(vec);
        isAcceptable(realBest);
        cout << "The random selected best is: " << randomSelectBest << endl;
        cout << "The result is: " << (acceptable ? "Accept" : "Not Accept") << endl;
        cout << "The iteration time is: " << iteratTime << endl << endl;

        totalIterateTime += iteratTime;
        if(acceptable)
            totalAcceptTime++;
    }

    accuracy = double(totalAcceptTime) / double(times);
    avgIteratTime = double(totalIterateTime) / double(times);
}


// int main()
// {
//     int arraySize = 10000;
//     vector<int> vec;

//     int realBest = -1;
//     for (int i = 0; i < arraySize; i++)
//     {
//         int temp = rand() % 10000 + 1;
//         // vec.push_back(i);
//         vec.push_back(temp);
//         realBest = realBest < temp ? temp : realBest;
//         // realBest = realBest < i ? i : realBest;

//     }

//     cout << "The Best number is: " << realBest << endl;

//     std::random_device r;
//     std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
//     std::mt19937 eng(seed);
    
//     default_random_engine rng;
//     shuffle(begin(vec), end(vec), eng);

//     int times = 100;
//     int offset = 10;

//     RandomSelectOptimize randomSelectOptimize;

//     cout << "######## Start Iterating ########" << endl;

//     randomSelectOptimize.getPerformanceInfo(times, realBest, offset, vec);

//     cout << "The Accuracy of Random Select is: " << randomSelectOptimize.accuracy << endl;
//     cout << "The Average Iteration Time of Random Select is: " << randomSelectOptimize.avgIteratTime << endl;

//     return 0;
// }