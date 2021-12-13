#pragma once
#include "resource.hpp"

struct TaskTemplateInfo
{
    ResourceRequestModel rm;
    int runFrequency;
    int runDuration;
};

struct TaskRunInfo
{
    ResourceRequest request;
    int lifeTime;

    void lifeTimeDecrease()
    {
        lifeTime--;
    }

    bool isFinish()
    {
        return lifeTime == 0 ? true : false;
    }

    bool operator< (const TaskRunInfo& a) const
    {
        return lifeTime > a.lifeTime;
    }
};


typedef map<int, TaskTemplateInfo> IntTaskInfoMap;

class Task
{
private:
    /* data */
public:
    ResourceRequest resourceRequest;
    int runFrequency;
    int runDuration;
    
    Task(/* args */);
    ~Task();
};


//  Task
// {
// private:
//     /* data */
// public:
//     Task(/* args */);
//     ~Task();

//     int runFrequency;
//     int runDuration;
// };

// Task::Task(/* args */)
// {
// }

// Task::~Task()
// {
// }
