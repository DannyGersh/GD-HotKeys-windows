#include "Include.h"


class EXE : public wxWindow 
{
public:
	struct Controls {
		wxTextCtrl* name;
		wxTextCtrl* path;
		wxButton* search;
		wxButton* del;
	} c;

public:
	/*
	pair< constant oldVal, changing newVal >
	this is to enable the user to press OK or CANCEL.
	oldVal is the old registry value name, 
	that will be changed to newVal if OK precced.
	*/
	wxString originalNAME;
	wxString originalPATH;

public:
	wxBoxSizer* vbox = new wxBoxSizer(wxHORIZONTAL);

public:
	EXE(wxWindow* parent, wxWindowID id, wxString name, wxString path);
	void OnSearch(wxCommandEvent& event);
	void OnDel(wxCommandEvent& event);

};

extern std::vector<EXE*> EXEs;


class EXEScrollWND : public wxScrolledWindow
{
public:
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

public:
	EXEScrollWND(wxWindow* parent, wxWindowID id);
	void newEXE();
	void getEXEs();
};


class EXEsFrame : public wxFrame
{
public:
	wxBoxSizer* MAINsizer;
	EXEScrollWND* scrollwnd;

public:
	EXEsFrame();
	~EXEsFrame();

private:
	void OnNewEXE(wxCommandEvent& event);
	void OnCancle(wxCommandEvent& event);
	void OnOK(wxCommandEvent& event); // defined in Main.cpp for access to other classes

};



enum
{
	ID_newEXE = 200,
	ID_search = 202,
	ID_del = 203,
	ID_cancel = 204,
	ID_ok = 205
};


