#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <vector>


int ID_nextHK = 100;

class HK : public wxWindow {
private:
	wxCheckBox* CheckBox;
	struct c {
		wxComboBox* key; // keyboard key
		wxComboBox* mod; // ctrl / alt / none
		wxComboBox* exe; // path to executible
		wxComboBox* vis; // will the window be visible
		wxComboBox* arg; // path to the file to be executed, with args.
	} C;
	wxButton* folderBTN;
	wxButton* delBTN;
public:
	wxBoxSizer* vbox = new wxBoxSizer(wxHORIZONTAL);
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
};

class scrollWND : public wxScrolledWindow
{
public:
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

public:
	scrollWND(wxWindow* parent, wxWindowID id);
	void newHK();
};



class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	scrollWND* scrollwnd;
	wxBoxSizer* sizer;

public:
	MyFrame();
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	
	void newHK(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();

};




enum
{
	ID_Hello = 1,
	ID_newHKbtn = 2,
	ID_test = 12345
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_BUTTON(ID_newHKbtn, MyFrame::newHK)
wxEND_EVENT_TABLE()





