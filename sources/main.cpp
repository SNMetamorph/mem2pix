#include "application.h"
#include <string>

#undef main
int main(int argc, char *argv[])
{
#ifdef WIN32
    // just some colors :)
    system("color 0A");
#endif
    CApplication application;
    return application.Initialize(argc, argv);
}
