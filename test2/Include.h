#pragma once
#include "resource.h"
#include <wx/wxprec.h>
#include <wx/msw/registry.h>
#include <vector>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


template <typename a, typename b>
struct pair {
	a a;
	b b;
};


extern int  ID_nextHK;
extern bool finSetup;
extern struct MODS {
	pair<wxString, int> ctrl;
	pair<wxString, int> alt;
	pair<wxString, int> win;
	pair<wxString, int> shift;
	pair<wxString, int> none;
};

