#pragma once

#include <iostream> 
#include <vector>
#include <algorithm>
#include <math.h>

#include "slave.hpp"
#include "framework.hpp"
#include "resource.hpp"
#include "selection.hpp"

//  Allocation Logic
class Allocator
{
private:
    vector<ResourceInfo> tempResourceInfos;
    vector<ResourceOffer> offers;

    void buildTempResourceInfos();
    void buildTempSlaves();

    Slave findSlaveById(vector<Slave> &slaveVec, int Id);

public:
    vector<Slave> slaves;
    vector<Framework> frameworks;

    vector <Slave> tempSlaves;
    vector<ResourceInfo> resourceInfos;

    SlaveSelection matchSelection;

    //  Allocation Performance
    double totalAvgUseRate = 0.0;
    double totalAvgResDistance = 0.0;
    int allocationFailedCount = 0;

    Allocator();

    Allocator(const vector<Slave> &_slaves, const vector<Framework> &_frameworks);

    void setVecData(const vector<Slave> &_slaves, const vector<Framework> &_frameworks);

    void buildSlaves(int count, double cpuMax, double memMax);

    void buildFrameworks(int count, double cpuRequest, double memRequest);

    void generateOffer(AllocationStrategy strategy, ResDistanceType resDistanceType, double resMatchWeight, double randomThreshold, double randomOffset, float &iterationTime);

    void confirmOffer();
    
    void printResourceAllocatePerform(AllocationStrategy strategy);
    
    int countIdleSlave();

    int countRunningSlave();

    void calculateResourceAllocatePerform();

    void resetAllocator();

    void updateSlaveTaskRunStatus();

    void updateFrameworkRequestingStatus();

};