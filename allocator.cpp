#include "allocator.hpp"
#include "util.hpp"
#include "task.hpp"

void Allocator::buildTempResourceInfos()
{
    for(auto slave : slaves)
    {
        tempResourceInfos.push_back(slave.resourceInfo);
    }
}

void Allocator::buildTempSlaves()
{
    tempSlaves.clear();
    for(auto slave : slaves)
    {
        tempSlaves.push_back(slave);
    }
}

Slave Allocator::findSlaveById(vector<Slave> &slaveVec, int Id)
{
    Slave result;
    for (auto slave : slaveVec)
    {
        if(slave.slaveId == Id)
            result = slave;
    }
    return result;
}


Allocator::Allocator(){}

Allocator::Allocator(const vector<Slave> &_slaves, const vector<Framework> &_frameworks)
{
    slaves.clear();
    frameworks.clear();
    for(auto slave : _slaves)
        slaves.push_back(slave);
    for(auto framework : _frameworks)
        frameworks.push_back(framework);
}

void Allocator::setVecData(const vector<Slave> &_slaves, const vector<Framework> &_frameworks)
{
    slaves.clear();
    frameworks.clear();
    for(auto slave : _slaves)
        slaves.push_back(slave);
    for(auto framework : _frameworks)
        frameworks.push_back(framework);
    buildTempSlaves();
}

void Allocator::buildSlaves(int count, double cpuMax, double memMax)
{
    for(int i = 0; i < count; i++)
    {
        Slave slave = Slave(i+1, cpuMax, memMax);
        slaves.push_back(slave);
    }
}

void Allocator::buildFrameworks(int count, double cpuRequest, double memRequest)
{
    for(int i = 0; i < count; i++)
    {
        Framework framework = Framework(i + 1, cpuRequest, memRequest);
        frameworks.push_back(framework);
    }
}

void Allocator::generateOffer(AllocationStrategy strategy, ResDistanceType resDistanceType, double resMatchWeight, double randomThreshold, double randomOffset, float &iterationTime)
{
    // double resMatchWeight = 0.5;
    // double randomOffset = 2;
    // double randomThreshold = 0.2;

    util::shuffleVector(slaves);
    buildTempSlaves();

    for (auto framework : frameworks)
    { 
        framework.updateRequestingStatus();
        if (!framework.isRequesting())
            continue;

        // for (auto  slave : slaves)
        // {
        //     double resmatch = slave.getResMatch(framework.resourceRequest, euclideanCentra, resMatchWeight);
        // }
        ResourceRequest request = framework.task.resourceRequest;
        // int bestMatchSlaveId;
        // bestMatchSlaveId =  
        // Slave bestMatchSlave = findSlaveById(tempSlaves, bestMatchSlaveId);

        int bestMatchSlaveIdx; 
        switch (strategy)
        {
        case MesosDefaultRandomShuffle:
            bestMatchSlaveIdx = matchSelection.randomShuffle(request, tempSlaves);
            break;
        case DefaultFindMax:
            bestMatchSlaveIdx = matchSelection.defaultSelect(request,tempSlaves,resMatchWeight, resDistanceType);
            break;
        case RandomSelectFindMax:
            bestMatchSlaveIdx =  matchSelection.randomSelect(request, tempSlaves, resMatchWeight, randomThreshold, randomOffset, resDistanceType, iterationTime); 
            break;
        
        default:
            break;
        }

        if (bestMatchSlaveIdx == -1)
        {
            allocationFailedCount++;
            // cout << "********** Can not find resource for Framework with Id: " << framework.frameworkId << " **********" << endl;
            // framework.printResourceRequest();
            continue;
        }
        
        ResourceOffer _offer;
        _offer.buildOffer(tempSlaves[bestMatchSlaveIdx].slaveId, framework.frameworkId, request.cpuRequest, request.memRequest);
        offers.push_back(_offer);
        TaskRunInfo taskRunInfo;

        taskRunInfo.lifeTime = framework.task.runDuration;
        taskRunInfo.request = request;
        // _offer.printOfferInfo();
        tempSlaves[bestMatchSlaveIdx].setTempResourceChange(request.cpuRequest, request.memRequest);
        tempSlaves[bestMatchSlaveIdx].offer(_offer, taskRunInfo);
    }
    iterationTime /= frameworks.size();
}

void Allocator::confirmOffer()
{
    for (int i = 0; i < slaves.size(); i++)
    {
        slaves[i].resourceInfo = tempSlaves[i].resourceInfo;
        slaves[i].taskQueue = tempSlaves[i].taskQueue;

        // slaves[i].resourceInfo.updateResource(tempSlaves[i].tempCPUChange, tempSlaves[i].tempMEMChange);
        // tempSlaves[i].clearTempResourceChange();
        // while (!tempSlaves[i].taskQueue.empty())
        // {
            
        // }
    }
}

void Allocator::printResourceAllocatePerform(AllocationStrategy strategy)
{
    cout << "++ " << "********** Allocation Performance of: " << ToString(strategy) << " ********** " << endl << "++ " << endl;
    cout << "++ " << " Allocate Failed: " << allocationFailedCount << " Times." << endl;
    cout << "++ " << " Average Use Rate of Slaves: " << totalAvgUseRate << " Times." << endl;
    cout << "++ " << " Average Resource Distance: " << totalAvgResDistance << " Times." << endl;

}

int Allocator::countIdleSlave()
{
    int idleSlaveCount = 0;

    for(auto slave : slaves)
    {
        if(slave.resourceInfo.avgUseRate == 0.0)
            idleSlaveCount++;
    }

    return idleSlaveCount;
}

int Allocator::countRunningSlave()
{
    int runningSlaveCount = 0;

    for(auto slave : slaves)
    {
        if(slave.resourceInfo.avgUseRate != 0.0)
            runningSlaveCount++;
    }

    return runningSlaveCount;
}

void Allocator::calculateResourceAllocatePerform()
{
    for (int i = 0; i < slaves.size(); i++)
    {
        totalAvgUseRate += slaves[i].resourceInfo.avgUseRate;
        totalAvgResDistance += slaves[i].resourceInfo.getResDistance(euclideanCentra);
    }
    totalAvgUseRate = totalAvgUseRate / double(slaves.size());
    totalAvgResDistance = totalAvgResDistance / double(slaves.size());
}

void Allocator::resetAllocator()
{
    slaves.clear();
    tempSlaves.clear();
    frameworks.clear();
    totalAvgUseRate = 0.0;
    totalAvgResDistance = 0.0;
    allocationFailedCount = 0;
}

void Allocator::updateSlaveTaskRunStatus()
{
    for(auto slave : slaves)
    {
        slave.updateTaskQueue();
    }
}
