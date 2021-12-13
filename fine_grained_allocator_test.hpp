#pragma once

#include <iostream> 
#include <vector>
#include <algorithm>
#include <math.h>

#include "util.hpp"
#include "slave.hpp"
#include "framework.hpp"
#include "resource.hpp"
#include "selection.hpp"
#include "task.hpp"
#include "allocator.hpp"

const static int slaveCount = 100;
const static double cpuMax = 32;
const static double memMax = 262144; // 256GB

const static int frameworkCount =50;

const static int testRounds = 100;
const static int offerRounds = 50;

const static RMtoINt RMGenerationWeight = {
    {ResourceRequestModel::CPUHeavy, 10},
    {ResourceRequestModel::MemHeavy,10},
    {ResourceRequestModel::Balance,10},
    {ResourceRequestModel::CPUHeavySmall,100},
    {ResourceRequestModel::MemHeavySmall,100},
    {ResourceRequestModel::BalanceSmall,100}
    };

const static TaskTemplateInfo LongTask = {.rm = ResourceRequestModel::DefaultRM, .runFrequency = 15, .runDuration = 20};
const static TaskTemplateInfo MediumTask = {.rm = ResourceRequestModel::DefaultRM, .runFrequency = 5, .runDuration = 10};
const static TaskTemplateInfo ShortTask = {.rm = ResourceRequestModel::DefaultRM, .runFrequency = 1, .runDuration = 2};

const static IntTaskInfoMap TaskGenerationWeight = {
    {10, LongTask},
    {30, MediumTask},
    {60, ShortTask}
    };