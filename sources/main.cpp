#include "core.h"
#include "utils.h"
#include "params_manager.h"
#include "exception.h"
#include <string>
using namespace std;

static void ReportParams(const app_params_t &params)
{
    size_t dataOffset = reinterpret_cast<size_t>(
        params.dataAddress
    );

    printf(
        "Parameters:\n"
        "  Process ID     : %d\n"
        "  Data offset    : 0x%X\n"
        "  Image size     : %dx%d\n"
        "  Pixel format   : %s\n"
        "\n",
        params.processID,    dataOffset, 
        params.imageWidth,   params.imageHeight, 
        paramsManager()->GetPixelFormatAlias(params.pixelFormat)
    );
}

static void CheckParams(const app_params_t &params)
{
    if (params.processID < 0)
        EXCEPT("invalid process name/ID, check for valid");
    else if (params.imageWidth == 0)
        EXCEPT("invalid image width");
    else if (params.imageHeight == 0)
        EXCEPT("invalid image height");
    else if (params.pixelFormat == PIXFORMAT_INVALID)
        EXCEPT("invalid pixel format, must match one from list");
}

static void ShowTitleText()
{
    static char aliasList[2048];
    aliasList[0] = '\0';
    paramsManager()->GetFormatAliasList(
        aliasList, sizeof(aliasList)
    );
    printf(
        "\n"
        "  mem2pix - utility for visualizing process memory content as image\n"
        "  Version :  1.0\n" 
        "  Usage   :  mem2pix <required params> [optional params]\n"
        "\n"
        "  NOTE: If you will specify process name and will be found several processes\n"
        "  with same name, program will ask to specify process ID instead name\n"
        "\n"
        "  Required parameters:\n"
        "      -p     : Target process ID or name (desirable to use ID)\n"
        "      -o     : Content address in target process (DEC or HEX value)\n"
        "      -w     : Image representation width\n"
        "      -h     : Image representation height\n"
        "      -f     : Image pixel format, must match one of list below:\n"
        "%s\n"
        "  Optional parameters:\n"
        "      -b     : Force borderless window mode"
        "\n", aliasList
    );
}

#undef main
int main(int argc, char *argv[])
{
#ifdef WIN32
    // just some colors :)
    system("color 0A");
#endif

    if (argc <= 1)
    {
        ShowTitleText();
        return 0;
    }

    printf(
        "\n"
        " * \n"
        " * mem2pix started\n"
        " * \n"
        "\n"
    );

    try 
    {
        app_params_t params;
        ProgramParseParams(
            argc, argv, params
        );
        CheckParams(params);
        ReportParams(params);
        ProgramInit();
    }
    catch (CException& ex) 
    {
        ReportError(ex.getDescription());
        return -1;
    }

    return 0;
}