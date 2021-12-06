#pragma once
#include "resource.hpp"

class Slave
{
private:

public:
    int slaveId;
    ResourceInfo resourceInfo;
    
    double tempMEMChange = 0.0;
    double tempCPUChange = 0.0;
    
    Slave(){};

    Slave(int Id, double cpuMax, double memMax)
    {
        slaveId = Id;
        resourceInfo.initializeMax(cpuMax, memMax);
    };

    void offer(ResourceOffer offer)
    {
        if(offer.slaveId != slaveId)
            return;

        resourceInfo.updateResourceInfo(offer.cpuOffer, offer.memOffer);
    }

    void printResourceUsageChart()
    {
        int chartLength;
        chartLength = 50;

        cout << "++ " << "********** The resource use rate of Slave: " << slaveId << " **********" << endl;
        cout << "++ " << endl;

        //  print CPU usage chart
        cout << "++ CPU Usage: " << resourceInfo.cpu.usage << " / " << resourceInfo.cpu.max << 
        ". Use rate: " << resourceInfo.cpu.useRate << endl;
        cout << "++ CPU Use Rate | ";
        int temp = int(resourceInfo.cpu.useRate * chartLength);
        for (int i = 0; i < chartLength; i++)
        {
            cout << ((i < temp) ? "#" : "-");
        }
        cout <<  " |" << endl;

        // print MEM usage chart 
        cout << "++ MEM Usage: " << resourceInfo.mem.usage << " / " << resourceInfo.mem.max << 
        ". Use rate: " << resourceInfo.mem.useRate << endl;
        cout << "++ MEM Use Rate | ";
        temp = int(resourceInfo.mem.useRate * chartLength);
        for (int i = 0; i < chartLength; i++)
        {
            cout << ((i < temp) ? "#" : "-");
        }
        cout <<  " |" << endl;

        cout << "++ " << endl;
        cout << "++ " << "********** The resource use rate of Slave: " << slaveId << " **********" << endl << endl;
    }

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
};