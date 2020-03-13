#include "Include.h"
#include "ViewEXEs.h"
#include "trayICON.h"

class MainApp : public wxApp
{
public:
	virtual bool OnInit();

};
int  ID_nextHK{ 100 };
int  ID_next_hotkey{ 500 };
bool finSetup{ false };
wxString thisPATH, bootARG;
struct MODS {
	std::pair<wxString, int> ctrl{ "CTRL", wxMOD_CONTROL };
	std::pair<wxString, int> alt{ "ALT", wxMOD_ALT };
	std::pair<wxString, int> shift{ "SHIFT", wxMOD_SHIFT };
	std::pair<wxString, int> none{ "NONE", 0 };
} mods;



class HK : public wxWindow
{
public:
	wxCheckBox* CheckBox;
	struct c {
		wxComboBox* mod; // ctrl / alt / win / shift / none
		wxComboBox* key; // keyboard key
		wxComboBox* exe; // path to executible
		wxComboBox* vis; // will the window be visible
		wxTextCtrl* arg; // path to the file to be executed, with args.
	} C;
	wxButton* searchBTN;
	wxButton* deleteBTN;
public:
	wxBoxSizer* vbox;
	wxString key;
	bool vis;
	int mod;
	int ID;
public:
	HK(wxWindow* parent, wxWindowID id, long c, wxString m, wxString k, wxString e, wxString v, wxString a);
	~HK();
	void OnCheckBox(wxCommandEvent& event);
	void OnKey(wxCommandEvent& event);
	void OnMod(wxCommandEvent& event);
	void OnExe(wxCommandEvent& event);
	void OnVis(wxCommandEvent& event);
	void OnArg(wxCommandEvent& event);
	void OnSearch(wxCommandEvent& event);
	void OnDelete(wxCommandEvent& event);
	void registerHK();
	void executeHK(wxKeyEvent& event);

};

std::vector<HK*> HKs;
bool isinKEYs(wxString str) {
	for (auto& i : HKs) {
		if (i->key == str) {
			return true;
		}
	}
	return false;
};


class MainScrollWND : public wxScrolledWindow
{
private:
	std::vector<HK*> HKs;

public:
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

public:
	MainScrollWND(wxWindow* parent, wxWindowID id);
	void newHK();
	void getHKs();
	void test(wxKeyEvent& event);

};


class MainFrame : public wxFrame
{
public:
	TrayIcon* trayICON;
	MainScrollWND* MainScroll;
	wxBoxSizer* MAINsizer;

public:
	MainFrame();
	~MainFrame();

private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	void newHK(wxCommandEvent& event);
	void viewEXEs(wxCommandEvent& event);

	void close(wxCloseEvent& event);

	wxDECLARE_EVENT_TABLE();

};



enum
{
	ID_wxHKS_quit = 100,
	ID_newHKbtn = 101,
	ID_viewEXEs = 102,
	ID_test = 103,

	ID_ONtrayLEFT = 104
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)

EVT_BUTTON(ID_newHKbtn, MainFrame::newHK)
EVT_BUTTON(ID_viewEXEs, MainFrame::viewEXEs)
EVT_BUTTON(ID_wxHKS_quit, MainFrame::OnExit)

EVT_CLOSE(MainFrame::close)

wxEND_EVENT_TABLE()





