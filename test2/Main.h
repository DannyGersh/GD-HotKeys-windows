#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/msw/registry.h>
#endif

#include "ViewEXEs.h"
#include <vector>


int ID_nextHK = 100;
bool finSetup{ false };
bool processTextCtrl{ true };
wxArrayString STRkeys; // for cheking if a new key is unique.


struct MODS {
	struct pair {
		wxString str;
		int vk;
	};
	pair ctrl { "CTRL", wxMOD_CONTROL};
	pair alt  { "ALT", wxMOD_ALT };
	pair win  { "WIN", wxMOD_WIN };
	pair shift{ "SHIFT", wxMOD_SHIFT };
	pair none { "NONE", 0 };
} mods;



class HK : public wxWindow {
public:
	wxCheckBox* CheckBox;
	struct c {
		wxComboBox* mod; // ctrl / alt / win / shift / none
		wxComboBox* key; // keyboard key
		wxComboBox* exe; // path to executible
		wxComboBox* vis; // will the window be visible
		wxComboBox* arg; // path to the file to be executed, with args.
	} C;
	wxButton* folderBTN;
	wxButton* delBTN;
public:
	wxBoxSizer* vbox = new wxBoxSizer(wxHORIZONTAL);
	wxString key;
public:
	HK(wxWindow* parent, wxWindowID id);
	void OnCheckBox(wxCommandEvent& event);
	void OnKey(wxCommandEvent& event);
	void OnMod(wxCommandEvent& event);
	void OnExe(wxCommandEvent& event);
	void OnVis(wxCommandEvent& event);
	void OnArg(wxCommandEvent& event);
	void OnFolderBTN(wxCommandEvent& event);
	void OnDelBTN(wxCommandEvent& event);

	void test(wxKeyEvent& event);
};

class MainScrollWND : public wxScrolledWindow
{
public:
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

public:
	MainScrollWND(wxWindow* parent, wxWindowID id);
	void newHK();
	void getHKs();
};



class MainApp : public wxApp
{
public:
	virtual bool OnInit();

	//void poop() {
	//	wxMessageBox("POOP");
	//}

};

class MainFrame : public wxFrame
{
public:
	wxMenu* menuFile;
	wxMenu* menuHelp;
	wxMenuBar* menuBar;

	MainScrollWND* MainScroll;
	wxBoxSizer* MAINsizer;

public:
	MainFrame();

private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	void newHK(wxCommandEvent& event);
	void viewEXEs(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();

};




enum
{
	ID_Hello = 100,
	ID_newHKbtn = 101,
	ID_viewEXEs = 102,
	ID_test = 103
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)

EVT_MENU(ID_Hello, MainFrame::OnHello)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)

EVT_BUTTON(ID_newHKbtn, MainFrame::newHK)
EVT_BUTTON(ID_viewEXEs, MainFrame::viewEXEs)

wxEND_EVENT_TABLE()





