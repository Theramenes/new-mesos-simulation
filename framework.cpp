#include "framework.hpp"
#include "util.hpp"
#include "fine_grained_allocator_test.hpp"

Framework::Framework(int id, double cpuRequest, double memRequest)
{
    frameworkId = id;
    task.resourceRequest.cpuRequest = cpuRequest;
    task.resourceRequest.memRequest = memRequest;
}

Framework::Framework(int id): frameworkId(id){}

void Framework::generateTask(float cpuRandom, float memRandom, float runFreqRandom, float durationRandom)
{
    ResourceRequestModel tempRM = util::generateWeightedRandomResourceModel(RMGenerationWeight);
    TaskTemplateInfo tempTaskInfo = util::generateTaskTemplateInfo(TaskGenerationWeight);

    ResourceRequest tempRequest = RMToRequest(tempRM);
    
    task.resourceRequest.cpuRequest = tempRequest.cpuRequest * (1 + cpuRandom);
    task.resourceRequest.memRequest = tempRequest.memRequest * (1 +  memRandom); 

    int tempDuration = tempTaskInfo.runDuration * (1 + durationRandom);
    int tempFreq = tempTaskInfo.runFrequency * (1 + runFreqRandom);

    task.runDuration = (tempDuration < 1) ? 1 : tempDuration;
    task.runFrequency = (tempFreq < 1) ? 1 : tempFreq;

    roundsBeforeRequest = task.runFrequency;
    _isRequesting = false;
}

void Framework::setResourceRequest(ResourceRequestModel rm)
{
    _setResourceRequest(RMToRequest(rm));
}

void Framework::setResourceRequestModel(ResourceRequestModel rm, float cpuRandom, float memRandom)
{
    _setResourceRequest(RMToRequest(rm), cpuRandom, memRandom);
}

void Framework::printResourceRequest()
{
    cout << "********** Resource request of Framework Id: " << frameworkId 
    << " CPU request: " << task.resourceRequest.cpuRequest << " core." 
    << " MEM request: " << task.resourceRequest.memRequest << " MB." << endl;
}

void Framework::_setResourceRequest(ResourceRequest request)
{
    task.resourceRequest.cpuRequest = request.cpuRequest;
    task.resourceRequest.memRequest = request.memRequest;
}


void Framework::_setResourceRequest(ResourceRequest request, float cpuRandom, float memRandom)
{
    task.resourceRequest.cpuRequest = request.cpuRequest * (1 + cpuRandom);
    task.resourceRequest.memRequest = request.memRequest * (1 +  memRandom); 
}

void Framework::sleepTurns(int turn)
{
    roundsBeforeRequest = turn;
}

void Framework::updateRequestingStatus()
{
    roundsBeforeRequest--;

    _isRequesting = (roundsBeforeRequest == 0) ? true : false;
}

bool Framework::isRequesting()
{
    return _isRequesting;
}
