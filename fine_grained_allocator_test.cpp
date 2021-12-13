#include "fine_grained_allocator_test.hpp"

using namespace std;

void testPerformance(int slave, double cpuMax, double memMax, int frameworkCount, Allocator &allocator, 
vector<Slave> &slaves, vector<Framework> &frameworks, const RMtoINt &RMGenerationWeight,
int testRounds, int offerRounds, AllocationStrategy strategy, ResDistanceType resDistanceType, 
double resMatchWeight, double randomOffset, double randomThreshold);

// 
int main()
{
    Allocator allocator;

    vector<Slave> slaves;
    vector<Framework> frameworks;

    int avgAllocationFailed = 0;
    double avgResUseRate = 0.0;
    double avgResDistance = 0.0;

    // allocator.buildSlaves(slaveCount ,cpuMax, memMax);
    // allocator.buildFrameworks(frameworkCount);

    //  Print Mesos Cluster Info
    cout << "********* Mesos Cluster Info **********" << endl;
    cout << "Slave number: " << slaveCount <<endl;
    cout << "Framework number: " << frameworkCount << endl;
    cout << "Offer rounds: " << offerRounds << endl << endl; 
    
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
    int avgRunningSlaveCount = 0;
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
            float cpuRandom = util::getAverageRandom(10, 40) * (util::getAverageRandom(0, 10) >= 5 ? 1 : -1) / 100.0;
            float memRandom = util::getAverageRandom(10, 40) * (util::getAverageRandom(0, 10) >= 5 ? 1 : -1) / 100.0;

            float runFreqRandom = util::getAverageRandom(0, 20) * (util::getAverageRandom(0, 10) >= 5 ? 1 : -1) / 100.0;
            float durationRandom = util::getAverageRandom(0, 20) * (util::getAverageRandom(0, 10) >= 5 ? 1 : -1) / 100.0;
            
            framework.generateTask(cpuRandom, memRandom, runFreqRandom, durationRandom);
            // framework.setResourceRequestModel(util::generateWeightedRandomResourceModel(RMGenerationWeight), cpuRandom, memRandom);
            frameworks.push_back(framework);
        }
        allocator.resetAllocator();
        allocator.setVecData(slaves, frameworks);

        perOfferIterationTime = 0;
        for(int i = 0; i <offerRounds; i++)
        {
            tempIterationTime = 0;
            // cout << "++++++++++++++++++++++++++++++ Offer Round: " << i << " ++++++++++++++++++++++++++++++" << endl << endl;

            allocator.updateSlaveTaskRunStatus();
            allocator.generateOffer(strategy, resDistanceType,resMatchWeight, randomThreshold, randomOffset, tempIterationTime);
            allocator.confirmOffer();

            // for(auto slave : allocator.slaves)
            // {
            //     // cout << "Offer Round: " << i << endl;
            //     slave.printResourceUsageChart();
            // }

            avgRunningSlaveCount += allocator.countRunningSlave();
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
    avgRunningSlaveCount /= testRounds;

    cout << "The Performance of: " << ToString(strategy) << ". Using Resource Distance Type of: " <<  ToString(resDistanceType) << endl;
    cout << "Tested " << testRounds << " Times." << endl;
    cout << "Average Allocation Failed: " << avgAllocationFailed << endl;
    cout << "Average Resource Use Rate: " << avgResUseRate << endl;
    cout << "Average Resource Distance: " << avgResDistance << endl;
    if(strategy == AllocationStrategy::RandomSelectFindMax)
        cout << "Average Iteration Time: " << avgIterationTime << endl;
    cout << "Average Running Server Count during "<< offerRounds << " rounds of Offering: " << avgRunningSlaveCount << endl << endl;  
}

