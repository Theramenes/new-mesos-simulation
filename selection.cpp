#include <iostream>
#include <algorithm>
#include <random>

#include "selection.hpp"
#include "framework.hpp"

using namespace std;

SlaveSelection::SlaveSelection(){}

SlaveSelection::SlaveSelection(double weight, double randomThreshold, double offset ) : _weight(weight), _randomThreshold(randomThreshold), _offset(offset){}

int SlaveSelection::defaultSelect(ResourceRequest &request, vector<Slave> &slaveVec, double weight)
{
    double tempMax = -1;
    double tempResMatch;
    
    int bestMatchSlaveIdx = 0; 

    bool canFindSlave = false;
    for(int i = 0; i < slaveVec.size(); i++)
    {
        if(!slaveVec[i].resourceInfo.isAllocatable(request))
        {
            
            continue;
        }
        canFindSlave = true;
        tempResMatch = slaveVec[i].resourceInfo.getResMatch(request, ResDistanceType::euclideanCentra, weight);

        if(tempResMatch > tempMax)
        {
            tempMax = tempResMatch;
            bestMatchSlaveIdx = i;
            continue;
        }
    }

    return canFindSlave ? bestMatchSlaveIdx : -1;
}

int SlaveSelection::randomSelect(ResourceRequest &request, vector<Slave> &slaveVec, double weight, double randomThreshold, double offset)
{
    double tempResMatch;
    double tempMax = -1.0;
    int bestMatchSlaveIdx = 0;
    
    int thresholdPoint = int(double(slaveVec.size()) * randomThreshold);
    int size = slaveVec.size(); 
    bool canFindSlave;

    for (int i = 0; i < slaveVec.size(); i++)
    {
        canFindSlave = false;
        if(!slaveVec[i].resourceInfo.isAllocatable(request))
        {
            continue;
        }
        tempResMatch = slaveVec[i].resourceInfo.getResMatch(request, ResDistanceType::euclideanCentra, weight);
        
        //  前一部分 只看不选
        if (i < thresholdPoint)
        {
            tempMax = tempMax > tempResMatch ? tempMax : tempResMatch;
            continue;
        }
        
        // 后半部分 能选
        canFindSlave = true;
        if(tempResMatch >= tempMax - offset)
        {
            bestMatchSlaveIdx = i;
            return bestMatchSlaveIdx;
        }
    }
    bestMatchSlaveIdx = size - 1;
    return canFindSlave ? bestMatchSlaveIdx : -1;
}

int SlaveSelection::randomShuffle(ResourceRequest &request, vector<Slave> &slaveVec)
{
    bool canFindSlave = false;
    int bestMatchSlaveIdx = 0;
    for (int i = 0; i < slaveVec.size(); i++)
    {
        if(!slaveVec[i].resourceInfo.isAllocatable(request))
            continue;

        canFindSlave = true;
        bestMatchSlaveIdx = i;
        /* code */
    }
    return canFindSlave ? bestMatchSlaveIdx : -1;
}
// bool BestMatchSelection::isAcceptable(int realBest)
// {
//     acceptable = ((randomSelectBest == realBest) || 
//     (randomSelectBest >= realBest - _offset)) ? true : false;
// }

// void BestMatchSelection::getPerformanceInfo(int times, int realBest, int offset, vector<int>& vec)
// {
//     _offset = offset;

//     int totalIterateTime = 0;
//     int totalAcceptTime = 0;
    
//     for(int i = 0; i < times; i++)
//     {
//         cout << "No." << i << " Iteration." << endl;
//         randomSelect(vec);
//         isAcceptable(realBest);
//         cout << "The random selected best is: " << randomSelectBest << endl;
//         cout << "The result is: " << (acceptable ? "Accept" : "Not Accept") << endl;
//         cout << "The iteration time is: " << iteratTime << endl << endl;

//         totalIterateTime += iteratTime;
//         if(acceptable)
//             totalAcceptTime++;
//     }

//     accuracy = double(totalAcceptTime) / double(times);
//     avgIteratTime = double(totalIterateTime) / double(times);
// }