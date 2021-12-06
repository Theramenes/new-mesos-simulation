#pragma once
#include <iostream>
#include <map>
#include <math.h>

using namespace std;

struct ScalarResource
{
    double max;
    double usage = 0.0;
    double useRate = 0.0;

    void updateUseRate(){
        useRate = usage / max;
    }

    double getUseRate(double newUsage){
        return (usage + newUsage) / max; 
    }
};

struct ResourceRequest
{
    double cpuRequest;
    double memRequest;
};

struct ResourceOffer
{
    int slaveId;
    int frameworkId;

    double cpuOffer;
    double memOffer;

    void buildOffer(int _slaveId, int _frameworkId, double _cpuOffer, double _memOffer)
    {
        slaveId = _slaveId;
        frameworkId = _frameworkId;
        cpuOffer = _cpuOffer;
        memOffer = _memOffer;
    }

    void printOfferInfo()
    {
        cout << "********** Offer Resource From Slave: " << slaveId << " to Framework: " << frameworkId << endl;
        cout << "********** Offering CPU: " << cpuOffer << " cores, Mem: " << memOffer << " MB." << endl; 
    }
};

enum ResourceRequestModel
{
    CPUHeavy,
    MemHeavy,
    Balance
};

typedef map<ResourceRequestModel, int> RMtoINt;

enum ResDistanceType
{
    euclideanCentra,
    euclideanDecentra,
    chebyCentra,
    chebyDecentra
};

enum AllocationStrategy
{
    MesosDefaultRandomShuffle,
    DefaultFindMax,
    RandomSelectFindMax
};

inline const char* ToString(AllocationStrategy s)
{
    switch (s)
    {
        case MesosDefaultRandomShuffle:   return "Random Shuffle";
        case DefaultFindMax:   return "Default Find Max";
        case RandomSelectFindMax: return "Random Find Max";
        default:      return "Unknown";
    }
}

const ResourceRequest CPUHEAVY = {.cpuRequest = 6, .memRequest = 2024};
const ResourceRequest MEMHEAVY = {.cpuRequest = 2, .memRequest = 8192};
const ResourceRequest BALANCE = {.cpuRequest = 4, .memRequest = 4096};

struct ResourceInfo
{
    ScalarResource cpu;
    ScalarResource mem;

    double avgUseRate;

    void updateResourceInfo(double cpuUsageChange, double memUsageChange)
    {
        cpu.usage += cpuUsageChange;
        mem.usage += memUsageChange;

        cpu.updateUseRate();
        mem.updateUseRate();

        avgUseRate = (cpu.useRate + mem.useRate) / 2;
    }

    void initializeMax(double cpuMax, double memMax)
    {
        cpu.max = cpuMax;
        mem.max = memMax;
    }

    bool isAllocatable(ResourceRequest request)
    {
        if(cpu.usage == cpu.max || mem.usage == mem.max) 
            return false;

        if((cpu.usage + request.cpuRequest > cpu.max) || (mem.usage + request.memRequest > mem.max))
            return false;
        return true;
    }

    double getResMatch(ResourceRequest request, ResDistanceType type,  double weight)
    {
        double tempCPURate = cpu.getUseRate(request.cpuRequest);
        double tempMEMRate = mem.getUseRate(request.memRequest);

        double tempAvgUseRate = (tempCPURate + tempMEMRate) / 2.0;

        double euclideanResDistance;
        double chebyResDistance;
        double  resMatch;
        double resDistance;

        resDistance = getResDistance(tempCPURate, tempMEMRate, type);
        
        switch (type)
        {
        case euclideanCentra:
            // resDistance = getResDistance(tempCPURate, tempMEMRate, type);
            resMatch = double(1 - weight) * avgUseRate * 100.0 + weight / resDistance; break;
        case euclideanDecentra:
            // resDistance = getResDistance(tempCPURate, tempMEMRate, type);
            resMatch = double(1 - weight) / avgUseRate  +  weight / resDistance; break;
        case chebyCentra:            
            // resDistance = getResDistance(tempCPURate, tempMEMRate, type);
            resMatch = double(1 - weight) * avgUseRate  * 100.0 + weight / resDistance ; break;
        case chebyDecentra: 
            // resDistance = getResDistance(tempCPURate, tempMEMRate, type);
            resMatch = double(1 - weight) / avgUseRate  +  weight / resDistance; break;
            break;
        
        default:
            break;
        }
        return resMatch;
    }

    double getResDistance(double cpuUseRate, double memUseRate, ResDistanceType type)
    {
        double ResDistance;
        double tempAvgUseRate = (cpuUseRate + memUseRate) / 2.0;
        switch (type)
        {
        case euclideanCentra:
        case euclideanDecentra:
            ResDistance = sqrt(
            (cpuUseRate - tempAvgUseRate) * (cpuUseRate - tempAvgUseRate) + 
            (memUseRate - tempAvgUseRate) * (memUseRate - tempAvgUseRate));
            break;
        case chebyCentra:
        case chebyDecentra:
            ResDistance =  (cpuUseRate - tempAvgUseRate) > (memUseRate - tempAvgUseRate) ? 
        (cpuUseRate - tempAvgUseRate) : (memUseRate - tempAvgUseRate);
        default:
            break;
        }
        return ResDistance;
    }
    double getResDistance(ResDistanceType type)
    {
        double ResDistance;
        double tempAvgUseRate = (cpu.useRate + mem.useRate) / 2.0;
        switch (type)
        {
        case euclideanCentra:
        case euclideanDecentra:
            ResDistance = sqrt(
            (cpu.useRate - tempAvgUseRate) * (cpu.useRate - tempAvgUseRate) + 
            (mem.useRate - tempAvgUseRate) * (mem.useRate - tempAvgUseRate));
            break;
        case chebyCentra:
        case chebyDecentra:
            ResDistance =  (cpu.useRate - tempAvgUseRate) > ( mem.useRate - tempAvgUseRate) ? 
        (cpu.useRate - tempAvgUseRate) : ( mem.useRate - tempAvgUseRate);
        default:
            break;
        }
        return ResDistance;
    }


};