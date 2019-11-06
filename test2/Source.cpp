#include "wx/wxprec.h"
#include "Header.h"


wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame();
	frame->SetSize(800, 800);
	frame->Show(true);
	return true;
}



HK::HK(wxWindow* parent, wxWindowID id) : wxWindow(parent, id)
{
	CheckBox = new wxCheckBox(this, ID_nextHK, "");
	C = {
		new wxComboBox(this, ID_nextHK + 1, std::to_string(ID_nextHK + 1)),
		new wxComboBox(this, ID_nextHK + 2, std::to_string(ID_nextHK + 2)),
	    new wxComboBox(this, ID_nextHK + 3, std::to_string(ID_nextHK + 3)),
	    new wxComboBox(this, ID_nextHK + 4, std::to_string(ID_nextHK + 4)),
	    new wxComboBox(this, ID_nextHK + 5, std::to_string(ID_nextHK + 5)),
	};
	folderBTN = new wxButton(this, ID_nextHK + 6, "6");
	delBTN = new wxButton(this, ID_nextHK + 7, "7");

	// checkbox
	{
		CheckBox->SetValue(true);
		vbox->Add(CheckBox, 0, wxALL, 2);
	}
	 
	// comboboxes
	{
		vbox->Add(C.key, 0, wxALL, 1);
		vbox->Add(C.mod, 0, wxALL, 2);
		vbox->Add(C.exe, 0, wxALL, 2);
		vbox->Add(C.vis, 0, wxALL, 2);
		vbox->Add(C.arg, 1, wxALL, 2);
	}

	// buttons
	{
		folderBTN->SetMinSize({ folderBTN->GetSize().y, folderBTN->GetSize().y });
		delBTN->SetMinSize({ delBTN->GetSize().y, delBTN->GetSize().y });
		vbox->Add(folderBTN, 0, wxALL, 2);
		vbox->Add(delBTN, 0, wxALL, 2);
	}

	// bind events
	{
		Bind(wxEVT_CHECKBOX, &HK::OnCheckBox, this, ID_nextHK);
		Bind(wxEVT_TEXT, &HK::OnCheckBox, this, ID_nextHK + 1);
		Bind(wxEVT_TEXT, &HK::OnCheckBox, this, ID_nextHK + 2);
		Bind(wxEVT_TEXT, &HK::OnCheckBox, this, ID_nextHK + 3);
		Bind(wxEVT_TEXT, &HK::OnCheckBox, this, ID_nextHK + 4);
		Bind(wxEVT_TEXT, &HK::OnCheckBox, this, ID_nextHK + 5);
		Bind(wxEVT_BUTTON, &HK::OnCheckBox, this, ID_nextHK + 6);
		Bind(wxEVT_BUTTON, &HK::OnCheckBox, this, ID_nextHK + 7);
	}

	this->SetSizer(vbox);
	this->FitInside();

	ID_nextHK += 8;
}
void HK::OnCheckBox(wxCommandEvent& event)
{
	if (this->CheckBox->GetValue() == true) {
		this->C.key->Enable(true);
		this->C.mod->Enable(true);
		this->C.exe->Enable(true);
		this->C.vis->Enable(true);
		this->C.arg->Enable(true);
		this->folderBTN->Enable(true);
	}
	else {
		this->C.key->Enable(false);
		this->C.mod->Enable(false);
		this->C.exe->Enable(false);
		this->C.vis->Enable(false);
		this->C.arg->Enable(false);
		this->folderBTN->Enable(false);
	}
}
void HK::OnKey(wxCommandEvent& event)
{
}
void HK::OnMod(wxCommandEvent& event)
{
}
void HK::OnExe(wxCommandEvent& event)
{
}
void HK::OnVis(wxCommandEvent& event)
{
}
void HK::OnArg(wxCommandEvent& event)
{
}
void HK::OnFolderBTN(wxCommandEvent& event)
{
}
void HK::OnDelBTN(wxCommandEvent& event)
{
}



scrollWND::scrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id) 
{}
void scrollWND::newHK() {
	
	HK* h = new HK(this, wxID_ANY);
	
	sizer->Add(h, 0, wxEXPAND, 2);

	this->SetSizer(sizer);
	this->FitInside();
	this->SetScrollRate(10, 10);
}





MyFrame::MyFrame()
	: wxFrame(NULL, wxID_ANY, "Hello World")
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
		"Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
	Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

	scrollwnd = new scrollWND(this, wxID_ANY);
	sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(scrollwnd, 1, wxEXPAND);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(
		new wxButton(this, ID_newHKbtn, "POOP"),
		1, wxALL, 10
	);
	hbox->Add(
		new wxButton(this, ID_newHKbtn, "POOP"),
		1, wxALL, 10
	);

	sizer->Add(hbox, 0, wxEXPAND | wxBOTTOM);
	this->SetSizer(sizer);
	this->Show();
}
void MyFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets Hello World example",
		"About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}
void MyFrame::newHK(wxCommandEvent& event)
{
	this->scrollwnd->newHK();
}


