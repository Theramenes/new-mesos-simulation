#pragma once
#include "resource.hpp"
// #include "util.hpp"

class Framework
{
private:
    /* data */
public:
    int frameworkId;

    Framework(int id): frameworkId(id){}

    Framework(int id, double cpuRequest, double memRequest){
        frameworkId = id;
        resourceRequest.cpuRequest = cpuRequest;
        resourceRequest.memRequest = memRequest;
    }

    ResourceRequest resourceRequest;

    void setResourceRequest(ResourceRequestModel rm)
    {
        switch (rm)
        {
            case CPUHeavy:
                resourceRequest.cpuRequest = CPUHEAVY.cpuRequest;
                resourceRequest.memRequest = CPUHEAVY.memRequest;
                break;
            case MemHeavy:
                resourceRequest.cpuRequest = MEMHEAVY.cpuRequest;
                resourceRequest.memRequest = MEMHEAVY.memRequest;
                break;
            case Balance:
                resourceRequest.cpuRequest = BALANCE.cpuRequest;
                resourceRequest.memRequest = BALANCE.memRequest;
                break;

            default:
                break;
        }
    }

    void setResourceRequest(ResourceRequestModel rm, float cpuRandom, float memRandom)
    {
        // int random = util::getAverageRandom(randomLimitLower, randomLimitUpper);
        // int randomSign = util::getAverageRandom(0, 10);
        double cpuOffset = cpuRandom;
        double memOffset = memRandom;

        switch (rm)
        {
            case CPUHeavy:
                cpuOffset *= CPUHEAVY.cpuRequest ;
                memOffset *= CPUHEAVY.memRequest; 

                resourceRequest.cpuRequest = CPUHEAVY.cpuRequest + cpuOffset;
                resourceRequest.memRequest = CPUHEAVY.memRequest + memOffset;
                break;
            case MemHeavy:
                cpuOffset *= MEMHEAVY.cpuRequest ;
                memOffset *= MEMHEAVY.memRequest; 

                resourceRequest.cpuRequest = MEMHEAVY.cpuRequest + cpuOffset;
                resourceRequest.memRequest = MEMHEAVY.memRequest + memOffset;
                break;
            case Balance:
                cpuOffset *= BALANCE.cpuRequest ;
                memOffset *= BALANCE.memRequest; 

                resourceRequest.cpuRequest = BALANCE.cpuRequest + cpuOffset;
                resourceRequest.memRequest = BALANCE.memRequest + memOffset;
                break;

            case CPUHeavySmall:
                cpuOffset *= CPUHEAVYSMALL.cpuRequest ;
                memOffset *= CPUHEAVYSMALL.memRequest; 

                resourceRequest.cpuRequest = CPUHEAVYSMALL.cpuRequest + cpuOffset;
                resourceRequest.memRequest = CPUHEAVYSMALL.memRequest + memOffset;
                break;
            case MemHeavySmall:
                cpuOffset *= MEMHEAVYSMALL.cpuRequest ;
                memOffset *= MEMHEAVYSMALL.memRequest; 

                resourceRequest.cpuRequest = MEMHEAVYSMALL.cpuRequest + cpuOffset;
                resourceRequest.memRequest = MEMHEAVYSMALL.memRequest + memOffset;
                break;
            case BalanceSmall:
                cpuOffset *= BALANCESMALL.cpuRequest ;
                memOffset *= BALANCESMALL.memRequest; 

                resourceRequest.cpuRequest = BALANCESMALL.cpuRequest + cpuOffset;
                resourceRequest.memRequest = BALANCESMALL.memRequest + memOffset;
                break;

            default:
                break;
        }
    }

    void printResourceRequest()
    {
        cout << "********** Resource request of Framework Id: " << frameworkId 
        << " CPU request: " << resourceRequest.cpuRequest << " core." 
        << " MEM request: " << resourceRequest.memRequest << " MB." << endl;
    }
};