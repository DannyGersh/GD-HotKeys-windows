#pragma once
#include "Include.h"
#include <nlohmann/json.hpp>

extern wxString thisPATH;
extern json jmain;
extern string hideOnBoot;
extern string jsonPath;


enum
{
	ID_START_ON_BOOT,
	ID_QUIT
};

class TrayIcon : public wxTaskBarIcon
{
public:
	wxFrame* mainFrame;
	bool check = true;

public:
	TrayIcon(wxFrame* mainFrame);
	~TrayIcon();

	void OnLeftClick(wxTaskBarIconEvent&);
	void OnQuit(wxCommandEvent&);
	void OnStartOnBoot(wxCommandEvent&);
	void OnStartOnBootUI(wxUpdateUIEvent&);
	virtual wxMenu *CreatePopupMenu() wxOVERRIDE;

	void saveToDisck(bool b)
	{
		jmain[hideOnBoot] = b;
		ofstream(jsonPath) << jmain;
	}
	wxDECLARE_EVENT_TABLE();
};




