#include "fine_grained_allocator_test.hpp"

using namespace std;

//  Allocation Logic
class Allocator
{
private:
    vector<ResourceInfo> tempResourceInfos;
    vector<ResourceOffer> offers;

    void buildTempResourceInfos()
    {
        for(auto slave : slaves)
        {
            tempResourceInfos.push_back(slave.resourceInfo);
        }
    }

    void buildTempSlaves()
    {
        tempSlaves.clear();
        for(auto slave : slaves)
        {
            tempSlaves.push_back(slave);
        }
    }

    Slave findSlaveById(vector<Slave> &slaveVec, int Id)
    {
        Slave result;
        for (auto slave : slaveVec)
        {
            if(slave.slaveId == Id)
                result = slave;
        }
        return result;
    }

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

    Allocator()
    {    }

    Allocator(const vector<Slave> &_slaves, const vector<Framework> &_frameworks)
    {
        slaves.clear();
        frameworks.clear();
        for(auto slave : _slaves)
            slaves.push_back(slave);
        for(auto framework : _frameworks)
            frameworks.push_back(framework);
    }

    void setVecData(const vector<Slave> &_slaves, const vector<Framework> &_frameworks)
    {
        slaves.clear();
        frameworks.clear();
        for(auto slave : _slaves)
            slaves.push_back(slave);
        for(auto framework : _frameworks)
            frameworks.push_back(framework);
        buildTempSlaves();
    }

    void buildSlaves(int count, double cpuMax, double memMax)
    {
        for(int i = 0; i < count; i++)
        {
            Slave slave = Slave(i+1, cpuMax, memMax);
            slaves.push_back(slave);
        }
    }

    void buildFrameworks(int count, double cpuRequest, double memRequest)
    {
        for(int i = 0; i < count; i++)
        {
            Framework framework = Framework(i + 1, cpuRequest, memRequest);
            frameworks.push_back(framework);
        }
    }

    void generateOffer(AllocationStrategy strategy, ResDistanceType resDistanceType, double resMatchWeight, double randomThreshold, double randomOffset, float &iterationTime)
    {
        // double resMatchWeight = 0.5;
        // double randomOffset = 2;
        // double randomThreshold = 0.2;

        util::shuffleVector(slaves);
        buildTempSlaves();

        for (auto framework : frameworks)
        {
            // for (auto  slave : slaves)
            // {
            //     double resmatch = slave.getResMatch(framework.resourceRequest, euclideanCentra, resMatchWeight);
            // }
            ResourceRequest request = framework.resourceRequest;
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
            // _offer.printOfferInfo();
            tempSlaves[bestMatchSlaveIdx].setTempResourceChange(request.cpuRequest, request.memRequest);
            tempSlaves[bestMatchSlaveIdx].offer(_offer);
        }
        iterationTime /= frameworks.size();
    }

    void confirmOffer(){
        for (int i = 0; i < slaves.size(); i++)
        {
            slaves[i].resourceInfo.updateResourceInfo(tempSlaves[i].tempCPUChange, tempSlaves[i].tempMEMChange);
            tempSlaves[i].clearTempResourceChange();
        }
    }

    void printResourceAllocatePerform(AllocationStrategy strategy)
    {
        cout << "++ " << "********** Allocation Performance of: " << ToString(strategy) << " ********** " << endl << "++ " << endl;
        cout << "++ " << " Allocate Failed: " << allocationFailedCount << " Times." << endl;
        cout << "++ " << " Average Use Rate of Slaves: " << totalAvgUseRate << " Times." << endl;
        cout << "++ " << " Average Resource Distance: " << totalAvgResDistance << " Times." << endl;

    }

    void calculateResourceAllocatePerform()
    {
        for (int i = 0; i < slaves.size(); i++)
        {
            totalAvgUseRate += slaves[i].resourceInfo.avgUseRate;
            totalAvgResDistance += slaves[i].resourceInfo.getResDistance(euclideanCentra);
        }
        totalAvgUseRate = totalAvgUseRate / double(slaves.size());
        totalAvgResDistance = totalAvgResDistance / double(slaves.size());
    }

    void resetAllocator()
    {
        slaves.clear();
        tempSlaves.clear();
        frameworks.clear();
        totalAvgUseRate = 0.0;
        totalAvgResDistance = 0.0;
        allocationFailedCount = 0;
    }
};


void testPerformance(int slave, double cpuMax, double memMax, int frameworkCount, Allocator &allocator, 
vector<Slave> &slaves, vector<Framework> &frameworks, const RMtoINt &RMGenerationWeight,
int testRounds, int offerRounds, AllocationStrategy strategy, ResDistanceType resDistanceType, 
double resMatchWeight, double randomOffset, double randomThreshold);

// 
int main()
{
    int slaveCount = 100;
    double cpuMax = 16;
    double memMax = 32768;

    int frameworkCount =50;
    double cpuRequest = 1;
    double memRequest = 512;

    Allocator allocator;

    vector<Slave> slaves;
    vector<Framework> frameworks;

    const RMtoINt RMGenerationWeight = {
        {ResourceRequestModel::CPUHeavy, 10},
        {ResourceRequestModel::MemHeavy,10},
        {ResourceRequestModel::Balance,10},
        {ResourceRequestModel::CPUHeavySmall,10},
        {ResourceRequestModel::MemHeavySmall,10},
        {ResourceRequestModel::BalanceSmall,10}
        };

    int avgAllocationFailed = 0;
    double avgResUseRate = 0.0;
    double avgResDistance = 0.0;

    // allocator.buildSlaves(slaveCount ,cpuMax, memMax);
    // allocator.buildFrameworks(frameworkCount);
    int testRounds = 100;
    int offerRounds = 15;
    
    // 测试Mesos默认的性能
    cout << "********** Testing Strategy: " << ToString(AllocationStrategy::MesosDefaultRandomShuffle) << ". **********" << endl;
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::MesosDefaultRandomShuffle,ResDistanceType::euclideanCentra, 0,0,0);

    cout << "********** Testing Strategy: " << ToString(AllocationStrategy::DefaultFindMax) << ". **********" << endl;
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::DefaultFindMax,ResDistanceType::euclideanCentra,0.9, 0, 0);
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::DefaultFindMax,ResDistanceType::euclideanDecentra, 0.4, 0, 0);
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::DefaultFindMax,ResDistanceType::chebyCentra, 0.9, 0, 0);
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::DefaultFindMax,ResDistanceType::chebyDecentra, 0.4, 0, 0);

    cout << "********** Testing Strategy: " << ToString(AllocationStrategy::RandomSelectFindMax) << ". **********" << endl;
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::RandomSelectFindMax, ResDistanceType::euclideanCentra, 0.8, 0.1, 0.5);
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::RandomSelectFindMax, ResDistanceType::euclideanDecentra, 0.4, 0.2, 0.2);
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::RandomSelectFindMax, ResDistanceType::chebyCentra, 0.8, 0.1, 0.5);
    testPerformance(slaveCount, cpuMax, memMax, frameworkCount, allocator, slaves, frameworks, RMGenerationWeight, testRounds, offerRounds, 
    AllocationStrategy::RandomSelectFindMax, ResDistanceType::chebyDecentra, 0.4, 0.2, 0.2);

    return 0;
}


void testPerformance(int slaveCount, double cpuMax, double memMax, int frameworkCount, Allocator &allocator, 
    vector<Slave> &slaves, vector<Framework> &frameworks, const RMtoINt &RMGenerationWeight,
    int testRounds, int offerRounds, AllocationStrategy strategy, ResDistanceType resDistanceType,
    double resMatchWeight, double randomThreshold, double randomOffset)
{
    int avgAllocationFailed = 0;
    double avgResUseRate = 0;
    double avgResDistance = 0;

    float avgIterationTime = 0;
    float perOfferIterationTime = 0;
    float tempIterationTime;

    for (int i = 0; i < testRounds; i++)
    {
        slaves.clear();
        frameworks.clear();
        for(int i = 0; i < slaveCount; i++)
        {
            Slave slave = Slave(i+1, cpuMax, memMax);
            slaves.push_back(slave);
        }

        for (int i = 0; i < frameworkCount; i++)
        {
            Framework framework = Framework(i+1);
            float cpuRandom = util::getAverageRandom(20, 80) * (util::getAverageRandom(0, 10) >= 5 ? 1 : -1) / 100.0;
            float memRandom = util::getAverageRandom(20, 80) * (util::getAverageRandom(0, 10) >= 5 ? 1 : -1) / 100.0;
            
            framework.setResourceRequest(util::getWeightedRandomResourceModel(RMGenerationWeight), cpuRandom, memRandom);
            frameworks.push_back(framework);
        }
        allocator.resetAllocator();
        allocator.setVecData(slaves, frameworks);

        perOfferIterationTime = 0;
        for(int i = 0; i <offerRounds; i++)
        {
            tempIterationTime = 0;
            // cout << "++++++++++++++++++++++++++++++ Offer Round: " << i << " ++++++++++++++++++++++++++++++" << endl << endl;

            // for(auto slave : allocator.slaves)
            // {
            //     // cout << "Offer Round: " << i << endl;
            //     slave.printResourceUsageChart();
            // }

            allocator.generateOffer(strategy, resDistanceType,resMatchWeight, randomThreshold, randomOffset, tempIterationTime);

            allocator.confirmOffer();
            perOfferIterationTime += tempIterationTime;
        }
        allocator.calculateResourceAllocatePerform();
        // allocator.printResourceAllocatePerform(strategy);

        avgIterationTime += perOfferIterationTime / offerRounds;
        avgAllocationFailed += allocator.allocationFailedCount;
        avgResUseRate += allocator.totalAvgUseRate;
        avgResDistance += allocator.totalAvgResDistance;
    }
            // for(auto slave : allocator.slaves)
            // {
            //     // cout << "Offer Round: " << i << endl;
            //     slave.printResourceUsageChart();
            // }

    avgAllocationFailed = avgAllocationFailed / testRounds;
    avgResUseRate = avgResUseRate / testRounds;
    avgResDistance = avgResDistance / testRounds;
    avgIterationTime = avgIterationTime / testRounds;

    cout << "The Performance of: " << ToString(strategy) << ". Using Resource Distance Type of: " <<  ToString(resDistanceType) << endl;
    cout << "Tested " << testRounds << " Times." << endl;
    cout << "Average Allocation Failed: " << avgAllocationFailed << endl;
    cout << "Average Resource Use Rate: " << avgResUseRate << endl;
    cout << "Average Resource Distance: " << avgResDistance << endl;
    cout << "Average Iteration Time: " << avgIterationTime << endl << endl;
}

