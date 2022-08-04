/*
    Core functions export
*/
#pragma once
#include "params_manager.h"
#include "renderer.h"
#include "mem_reader.h"
#include <memory>

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

    std::unique_ptr<IMemoryReader> m_pMemoryReader;
    CRenderer m_Renderer;
    CParamsManager m_ParamsManager;
};
