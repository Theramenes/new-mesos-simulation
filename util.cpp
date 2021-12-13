#include "util.hpp"

namespace util
{


    ResourceRequestModel generateWeightedRandomResourceModel(RMtoINt weights)
    {
        int weightSum = 0;
        RMtoINt::iterator it;
        for (it = weights.begin(); it != weights.end(); it++)
        {
            weightSum += it->second;
        }
        int random = getAverageRandom(0, weightSum);
        ResourceRequestModel rm;

        for (it = weights.begin(); it != weights.end(); it++)
        {
            if(random < it->second)
            {
                rm = it->first;
                break;
            }
            random -= it->second;
        }
        return rm;
    }

    TaskTemplateInfo generateTaskTemplateInfo(IntTaskInfoMap weights)
    {
        int weightSum = 0;
        IntTaskInfoMap::iterator it;
        for (it = weights.begin(); it != weights.end(); it++)
        {
            weightSum += it->first;
        }
        int random = getAverageRandom(0, weightSum);
        TaskTemplateInfo taskInfo;

        for (it = weights.begin(); it != weights.end(); it++)
        {
            if(random < it->first)
            {
                taskInfo = it->second;
                break;
            }
            random -= it->first;
        }
        return taskInfo;
    }

}