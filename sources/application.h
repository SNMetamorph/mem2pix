/*
Copyright (C) 2019 SNMetamorph

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
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
