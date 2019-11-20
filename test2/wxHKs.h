

class HK : public wxWindow
{
public:
	wxCheckBox* CheckBox;
	struct c {
		wxComboBox* mod; // ctrl / alt / win / shift / none
		wxComboBox* key; // keyboard key
		wxComboBox* exe; // path to executible
		wxComboBox* vis; // will the window be visible
		wxComboBox* arg; // path to the file to be executed, with args.
	} C;
	wxButton* searchBTN;
	wxButton* deleteBTN;
public:
	wxBoxSizer* vbox;
	wxString key;
	int index;
public:
	HK(wxWindow* parent, wxWindowID id);
	~HK();
	void OnCheckBox(wxCommandEvent& event);
	void OnKey(wxCommandEvent& event);
	void OnMod(wxCommandEvent& event);
	void OnExe(wxCommandEvent& event);
	void OnVis(wxCommandEvent& event);
	void OnArg(wxCommandEvent& event);
	void OnSearch(wxCommandEvent& event);
	void OnDelete(wxCommandEvent& event);

	void test(wxKeyEvent& event);

};

std::vector<HK*> HKs;
bool isinKEYs(wxString str) {
	int count{ 0 };
	for (auto& i : HKs) {
		if (i->key == str) {
			return true;
		}
		count++;
	}
	return false;
};
bool isinARGs(wxString str) {
	int count{ 0 };
	for (auto& i : HKs) {
		if (i->C.arg->GetValue() == str) {
			return true;
		}
		count++;
	}
	return false;
}


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

};




class MainApp : public wxApp
{
public:
	virtual bool OnInit();

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





