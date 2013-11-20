#include "Utils.h"
#include "SimulationEngine.h"

int main(int argc, char * argv[])
{
    SimulationEngine simulation;

    if( !simulation.Run("Solar System", 1024, 680, false) )
    {
        return 1;
    }

    return 0;
}