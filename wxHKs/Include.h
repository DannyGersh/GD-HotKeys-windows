#pragma once

#include <wx/wxprec.h>

#include <wx/msw/registry.h>
#include <nlohmann/json.hpp>
#include <fstream>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <forward_list>
#include <wx/taskbar.h>


using namespace nlohmann;
using namespace std;