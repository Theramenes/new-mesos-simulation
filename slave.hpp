#pragma once
#include <queue>
#include "resource.hpp"
#include "task.hpp"

class Slave
{
private:

public:
    int slaveId;
    ResourceInfo resourceInfo;
    
    double tempMEMChange = 0.0;
    double tempCPUChange = 0.0;
    
    bool isAllocated = false;

    std::priority_queue<TaskRunInfo> taskQueue;
    std::vector<TaskRunInfo> taskRunInfoVec;

    Slave(){};

    Slave(int Id, double cpuMax, double memMax)
    {
        slaveId = Id;
        resourceInfo.initializeMax(cpuMax, memMax);
    };

    void offer(ResourceOffer offer, TaskRunInfo taskRunInfo)
    {
        if(offer.slaveId != slaveId)
            return;
        resourceInfo.updateResource(offer.cpuOffer, offer.memOffer);
        taskQueue.push(taskRunInfo);
    }

    void printResourceUsageChart();

    void setTempResourceChange(double _cpuChange, double _memChange)
    {
        tempCPUChange += _cpuChange;
        tempMEMChange += _memChange;
    }

    void clearTempResourceChange()
    {
        tempCPUChange = 0.0;
        tempMEMChange = 0.0;
    }

    void updateTaskQueue();
    void updateTaskRunInfo();

};