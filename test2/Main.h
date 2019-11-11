#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <vector>


int ID_nextHK = 100;
bool finSetup{ false };
bool processTextCtrl{ true };
std::vector<wxString> STRkeys; // for cheking if a new key is unique.

bool isin(wxArrayString& array, wxString str) 
{	
	for (auto& i : array) {
		if (i == str) {
			return true;
		}
	}
	return false;
}
bool isin(std::vector<wxString>& array, wxString str)
{
	for (auto& i : array) {
		if (i == str) {
			return true;
		}
	}
	return false;
}
void replaceSTRinVEC(std::vector<wxString>& vec, wxString old, wxString New) {
	std::vector<wxString>::iterator itr = std::find(vec.begin(), vec.end(), old);
	if (itr != vec.cend()) {
		int index = std::distance(vec.begin(), itr);
		vec[index] = New;
	}
}

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

class scrollWND : public wxScrolledWindow
{
public:
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

public:
	scrollWND(wxWindow* parent, wxWindowID id);
	void newHK();
	void getHKs();
};



class MyApp : public wxApp
{
public:
	virtual bool OnInit();

	//void poop() {
	//	wxMessageBox("POOP");
	//}

};

class MyFrame : public wxFrame
{
public:
	wxMenu* menuFile;
	wxMenu* menuHelp;
	wxMenuBar* menuBar;

	scrollWND* scrollwnd;
	wxBoxSizer* MAINsizer;

public:
	MyFrame();
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
	ID_Hello = 12345,
	ID_newHKbtn = wxID_ANY,
	ID_viewEXEs,
	ID_test = 1239
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)

EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_BUTTON(ID_newHKbtn, MyFrame::newHK)
EVT_BUTTON(ID_viewEXEs, MyFrame::viewEXEs)

wxEND_EVENT_TABLE()





