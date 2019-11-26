#pragma once
#include "Include.h"

extern wxString thisPATH;


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

	wxDECLARE_EVENT_TABLE();
};




