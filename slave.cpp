#include "slave.hpp"

void Slave::updateTaskQueue()
{
    // copy queue
    std::priority_queue<TaskRunInfo> tempTaskQueue(taskQueue);

    // clear queue
    priority_queue<TaskRunInfo> emptyQueue;
    swap(taskQueue, emptyQueue);

    while (!tempTaskQueue.empty())
    {
        TaskRunInfo tempTask = tempTaskQueue.top();
        tempTaskQueue.pop();
        
        tempTask.lifeTime--;

        if(tempTask.lifeTime == 0)
        {
            // decrease resource
            resourceInfo.updateResource(-1.0 * tempTask.request.cpuRequest, -1.0 * tempTask.request.memRequest);
            continue;
        }

        taskQueue.push(tempTask);
    }
}

void Slave::printResourceUsageChart()
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



// void Slave::updateTaskRunInfo()
// {
//     for (int i = 0; i < taskRunInfoVec.size(); i++)
//     {
//         taskRunInfoVec[i].lifeTime--;

    
//     }
//     taskRunInfoVec.

// }