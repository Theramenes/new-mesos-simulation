#include <iostream>
#include "../util.hpp"

using namespace std;

int main()
{
    for (int i = 0; i < 5; i++)
    {
        cout << util::getAverageRandom(0,200) << endl;

    }
    
    return 0;
}