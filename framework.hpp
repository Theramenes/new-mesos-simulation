#pragma once
#include "resource.hpp"
#include "task.hpp"

class Framework
{
private:
    int roundsBeforeRequest;
    void _setResourceRequest(ResourceRequest request);
    void _setResourceRequest(ResourceRequest request, float cpuRandom, float memRandom);
    bool _isRequesting;

public:
    int frameworkId;
    Task task;
    
    // ResourceRequest resourceRequest;

    Framework(int id);

    Framework(int id, double cpuRequest, double memRequest);

    void generateTask(float cpuRandom, float memRandom, float runFreqRandom, float durationRandom);

    void setResourceRequest(ResourceRequestModel rm);

    void setResourceRequestModel(ResourceRequestModel rm, float cpuRandom, float memRandom);

    void printResourceRequest();

    void updateRequestingStatus();

    void sleepTurns(int turn);

    bool isRequesting();
};