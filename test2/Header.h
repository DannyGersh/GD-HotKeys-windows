#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <vector>


int ID_nextHK = 100;
std::vector<wxString> STRkeys; // this is for cheking if a new key is unique.

bool isin(wxArrayString array, wxString str) 
{	
	for (auto& i : array) {
		if (i == str) {
			return true;
		}
	}
	return false;
}

class HK : public wxWindow {
public:
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

	wxDECLARE_EVENT_TABLE();

};




enum
{
	ID_Hello = 12345,
	ID_newHKbtn = wxID_ANY,
	ID_test = 1239
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_BUTTON(ID_newHKbtn, MyFrame::newHK)

//EVT_HOTKEY(ID_test, MyFrame::OnHello)

wxEND_EVENT_TABLE()





