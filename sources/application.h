/*
    Core functions export
*/
#pragma once
#include "params.h"
#include "params_manager.h"
#include "renderer.h"

class CApplication
{
public:
    int Initialize(int argc, char *argv[]);
   
private:
    void StartScan();
    void ParseParams(int argc, char *argv[]);
    void UpdateWindowTitle();
    void ShowTitleText();
    void ReportParams();
    bool ProgramLoop();

    CRenderer m_Renderer;
    CParamsManager m_ParamsManager;
};
