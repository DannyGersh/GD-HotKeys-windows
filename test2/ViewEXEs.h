// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class EXE : public wxWindow {
public:
	wxComboBox* pathCMB; 
	wxButton* folderBTN;
	wxButton* delBTN;
public:
	wxBoxSizer* vbox = new wxBoxSizer(wxHORIZONTAL);
public:
	EXE(wxWindow* parent, wxWindowID id);
	void OnPathCMB(wxCommandEvent& event);
	void OnFolderBTN(wxCommandEvent& event);
	void OnDelBTN(wxCommandEvent& event);

};

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
	EXEScrollWND* scrollwnd;
	wxBoxSizer* MAINsizer;

public:
	EXEsFrame();
private:
	void OnNewEXE(wxCommandEvent& event);
	void OnDel(wxCommandEvent& event);

};


enum
{
	ID_newEXE = 200,
	ID_pathCMB = 201,
	ID_folderBTN = 202,
	ID_delBTN = 203
};


