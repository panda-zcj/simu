#include "times.h"
#include "log.h"
#include "communication.h"
#include "event.h"
#ifdef SIMULATOR
#include "simulator.h"
#else
#include "node.h"
#endif

using namespace simu;

int main()
{
#ifdef SIMULATOR
    nanoLogInit("simulator");
#else
    nanoLogInit("node");
#endif

#ifdef SIMULATOR
    Simulator::exec();
#else
    Node::exec();
#endif
    return 0;
}