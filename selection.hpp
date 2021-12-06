#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include "slave.hpp"
#include "framework.hpp"
#include "resource.hpp"

using namespace std;

class SlaveSelection
{
private:
    vector<int> _vec;
    int _iteratTime;
    
    int _offset;
    double _weight;
    double _randomThreshold;

public:
    double accuracy;
    double avgIteratTime;
    
    double randomSelectBest; 
    double realBest;

    SlaveSelection();
    SlaveSelection(double weight, double randomThreshold, double offset);

    int randomSelect(ResourceRequest &request, vector<Slave> &slaveVec);
    int randomSelect(ResourceRequest &request, vector<Slave> &slaveVec, double weight, double randomThreshold, double offset);

    int defaultSelect(ResourceRequest &request, vector<Slave> &slaveVec, double weight);

    int randomShuffle(ResourceRequest &request, vector<Slave> &slaveVec);

    bool isAcceptable();
    void getPerformanceInfo(int times, int realBest, int offset, vector<int>& vec);
};

