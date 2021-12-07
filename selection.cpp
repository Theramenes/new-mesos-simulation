#include <iostream>
#include <algorithm>
#include <random>

#include "selection.hpp"
#include "framework.hpp"

using namespace std;

SlaveSelection::SlaveSelection(){}

SlaveSelection::SlaveSelection(double weight, double randomThreshold, double offset ) : _weight(weight), _randomThreshold(randomThreshold), _offset(offset){}

int SlaveSelection::defaultSelect(ResourceRequest &request, vector<Slave> &slaveVec, double weight, ResDistanceType resDistanceType)
{
    double tempMax = -1;
    double tempResMatch;
    
    int bestMatchSlaveIdx = -1; 

    // bool canFindSlave = false;
    for(int i = 0; i < slaveVec.size(); i++)
    {
        if(!slaveVec[i].resourceInfo.isAllocatable(request))
        {
            continue;
        }
        // canFindSlave = true;
        tempResMatch = slaveVec[i].resourceInfo.getResMatch(request, resDistanceType, weight);

        if(tempResMatch > tempMax)
        {
            tempMax = tempResMatch;
            bestMatchSlaveIdx = i;
            continue;
        }
    }

    return bestMatchSlaveIdx;
}

int SlaveSelection::randomSelect(ResourceRequest &request, vector<Slave> &slaveVec, double weight, double randomThreshold, double offset, 
ResDistanceType resDistanceType, float &iterationTime)
{
    double tempResMatch;
    double tempMax = -1.0;
    int bestMatchSlaveIdx = -1;

    float endThreshold = 0.95;
    int endOffset = 4;
    
    int thresholdPoint = int(double(slaveVec.size()) * randomThreshold);
    int endThresholdPoint = int(float(slaveVec.size()) * endThreshold);

    int size = slaveVec.size(); 
    bool canFindSlave;

    for (int i = 0; i < slaveVec.size(); i++)
    {
        iterationTime ++;
        canFindSlave = false;
        if(!slaveVec[i].resourceInfo.isAllocatable(request))
        {
            continue;
        }
        tempResMatch = slaveVec[i].resourceInfo.getResMatch(request, resDistanceType, weight);

        // cout << "Slave Id: " << slaveVec[i].slaveId << ". Resource Match: " << tempResMatch << endl;
        
        if(tempResMatch == INFINITY)
        {
            bestMatchSlaveIdx = i;
            return bestMatchSlaveIdx;
        }

        //  前一部分 只看不选
        if (i < thresholdPoint)
        {
            tempMax = tempMax > tempResMatch ? tempMax : tempResMatch;
            continue;
        }
        
        // 后半部分 能选
        canFindSlave = true;
        if(tempResMatch >= tempMax - (tempMax * offset) || tempResMatch == INFINITY)
        {
            bestMatchSlaveIdx = i;
            return bestMatchSlaveIdx;
        }

        if( i > endThresholdPoint)
        {
            if(slaveVec[i].resourceInfo.isAllocatable(request))
            // &&(tempResMatch >= tempMax - (tempMax * endOffset * offset) || tempResMatch == INFINITY))
            {
                bestMatchSlaveIdx = i;
                return bestMatchSlaveIdx;
            }
        }
    }
    // bestMatchSlaveIdx = size - 1;
    return bestMatchSlaveIdx;
}

int SlaveSelection::randomShuffle(ResourceRequest &request, vector<Slave> &slaveVec)
{
    bool canFindSlave = false;
    int bestMatchSlaveIdx = -1;
    for (int i = 0; i < slaveVec.size(); i++)
    {
        if(!slaveVec[i].resourceInfo.isAllocatable(request))
            continue;

        canFindSlave = true;
        bestMatchSlaveIdx = i;
        /* code */
    }
    return bestMatchSlaveIdx;
}