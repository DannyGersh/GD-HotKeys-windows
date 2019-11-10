#include "wx/wxprec.h"
#include <wx/msw/registry.h>
#include "Header.h"


/*
if (!RegisterHotKey(12345, 0, 'Q'))
{
	wxMessageBox("RegisterHotKey faild");
}
Bind(wxEVT_HOTKEY, &HK::test, this, 12345);
*/

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame();
	frame->SetSize(800, 800);
	frame->Show(true);

	return true;
}


HK::HK(wxWindow* parent, wxWindowID id) : wxWindow(parent, id)
{	
	// init controls
	{
		CheckBox = new wxCheckBox(this, ID_nextHK, "");
		C = {
			new wxComboBox(this, ID_nextHK + 1, "key"),
			new wxComboBox(this, ID_nextHK + 2, "mod"),
			new wxComboBox(this, ID_nextHK + 3, "exe path"),
			new wxComboBox(this, ID_nextHK + 4, "is visible"),
			new wxComboBox(this, ID_nextHK + 5, "file path"),
		};
		folderBTN = new wxButton(this, ID_nextHK + 6, "6");
		delBTN = new wxButton(this, ID_nextHK + 7, "7");
	}

	// checkbox
	{
		CheckBox->SetValue(true);
		vbox->Add(CheckBox, 0, wxCENTER | wxALL | wxRIGHT, 2);
	}
	 
	// comboboxes
	{
		// key
		{
			for (int i = 1; i <= 12; i++) {
				C.key->Append("F" + std::to_string(i));
			}
		}
		// mod
		{
			wxString strList[4]{ "CTRL", "ALT", "WIN", "NONE" };
			wxArrayString a(4, strList);
			C.mod->Append(a);
		}
		// vis
		{
			C.vis->Append("True");
			C.vis->Append("False");
		}

		vbox->Add(C.key, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.mod, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.exe, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.vis, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.arg, 1, wxCENTER | wxALL, 2);
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
		Bind(wxEVT_TEXT, &HK::OnKey, this, ID_nextHK + 1);
		Bind(wxEVT_TEXT, &HK::OnMod, this, ID_nextHK + 2);
		Bind(wxEVT_TEXT, &HK::OnExe, this, ID_nextHK + 3);
		Bind(wxEVT_TEXT, &HK::OnVis, this, ID_nextHK + 4);
		Bind(wxEVT_TEXT, &HK::OnArg, this, ID_nextHK + 5);
		Bind(wxEVT_BUTTON, &HK::OnFolderBTN, this, ID_nextHK + 6);
		Bind(wxEVT_BUTTON, &HK::OnDelBTN, this, ID_nextHK + 7);
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
	
	wxString newVal{ C.key->GetValue() }; // oldVal == key ... at the moment
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
	wxString oldVal; rk.QueryValue("key", oldVal);

	bool proceed{ true };
	if (newVal == "" || newVal.length() > 1) {
		proceed = false;
		for (auto i : C.key->GetStrings()) {
			if (i == newVal) {
				proceed = true;
			}
		}
	}
	for (auto i : STRkeys) {
		if (i == newVal) {
			proceed = false;
		}
	}

	if (proceed) 
	{
		// replace STRkeys old key
		{
			std::vector<wxString>::iterator itr = std::find(STRkeys.begin(), STRkeys.end(), oldVal);
			if (itr != STRkeys.cend()) {
				int index = std::distance(STRkeys.begin(), itr);
				STRkeys[index] = newVal;
			}
		}

		// update HK::key and registry
		{
			if (newVal != oldVal) 
			{
				key = newVal; // HK::key
				rk.Rename(newVal);
				rk.SetValue("key", newVal);
			}
		}

	}
	else // proceed
	{ 
		if (newVal.length() > 1) {
			if(isin(C.key->GetStrings(), newVal))
			{
				// bug - C.key->SetValue(key) does not work
				wxMessageBox("Key not unique");
				C.key->SetValue(key);
			}
			else
			{
				wxMessageBox("Onley one charekter allowd");
				C.key->SetValue(key);
			}
		}
		else if (newVal == "") {
			wxMessageBox("Hotkey must have a key");
			C.key->SetValue(key);
		}
		else if (newVal != key) {
			wxMessageBox("Key not unique");
			C.key->SetValue(key);
		}
	}

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

void HK::test(wxKeyEvent& event) {
	wxMessageBox("POOP");
}



scrollWND::scrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id) 
{}
void scrollWND::newHK() {

	// main
	{
		bool proceed{ true };
		for (auto i : STRkeys) {
			if (i == "key") {
				proceed = false;
				break;
			}
		}
		if (proceed) {
			HK* h = new HK(this, wxID_ANY);
			h->SetLabel("key");
			STRkeys.push_back("key");
			h->key = "key";

			sizer->Add(h, 0, wxEXPAND, 2);
			this->SetSizer(sizer);
			this->FitInside();
			this->SetScrollRate(10, 10);
		}
	}

	// registry
	{
		wxRegKey k(wxRegKey::HKCU, "Software\\wxHKs\\key");
		if (k.Create(false)) {
			k.SetValue("checkbox", 1);
			k.SetValue("key", "key");
			k.SetValue("mod", "mod");
			k.SetValue("exe", "exe path");
			k.SetValue("vis", "is visible");
			k.SetValue("arg", "file path");
		}
	}

}
void scrollWND::getHKs() {
	
	wxRegKey Kmain(wxRegKey::HKCU, "Software\\wxHKs");
	Kmain.Open();

	size_t subkeys;
	wxString key_name;
	long why{ 1 };

	Kmain.GetKeyInfo(&subkeys, NULL, NULL, NULL);
	Kmain.GetFirstKey(key_name, why);

	for (size_t i = 0; i < subkeys; i++)
	{
		HK* h = new HK(this, wxID_ANY);
		sizer->Add(h, 0, wxEXPAND, 2);
		this->SetSizer(sizer);
		this->FitInside();
		
		long checkbox; wxString key, mod, exe, vis, arg;
		wxRegKey k0(wxRegKey::HKCU, "Software\\wxHKs\\" + key_name);
		
		k0.QueryValue("checkbox", &checkbox);
		k0.QueryValue("key", key); h->key = key;
		k0.QueryValue("mod", mod); 
		k0.QueryValue("exe", exe);
		k0.QueryValue("vis", vis);
		k0.QueryValue("arg", arg);

		h->CheckBox->SetValue(checkbox);
		h->C.key->SetValue(key);
		h->C.mod->SetValue(mod);
		h->C.exe->SetValue(exe);
		h->C.vis->SetValue(vis);
		h->C.arg->SetValue(arg);

		STRkeys.push_back(key); 

		Kmain.GetNextKey(key_name, why);
	}
}





MyFrame::MyFrame()
	: wxFrame(NULL, wxID_ANY, "Hello World")
{
	// menue
	{
		menuFile = new wxMenu;
		menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
			"Help string shown in status bar for this menu item");
		menuFile->AppendSeparator();
		menuFile->Append(wxID_EXIT);

		menuHelp = new wxMenu;
		menuHelp->Append(wxID_ABOUT);

		menuBar = new wxMenuBar;
		menuBar->Append(menuFile, "&File");
		menuBar->Append(menuHelp, "&Help");
		SetMenuBar(menuBar);
		CreateStatusBar();
		SetStatusText("Welcome to wxWidgets!");
		Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
		Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
		Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
	}

	// setup MAINsizer
	{
		scrollwnd = new scrollWND(this, wxID_ANY);
		MAINsizer = new wxBoxSizer(wxVERTICAL);

		MAINsizer->Add(scrollwnd, 1, wxEXPAND);

		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
		hbox->Add(
			new wxButton(this, ID_newHKbtn, "POOP"),
			1, wxALL, 10
		);
		hbox->Add(
			new wxButton(this, ID_newHKbtn, "POOP"),
			1, wxALL, 10
		);

		MAINsizer->Add(hbox, 0, wxEXPAND | wxBOTTOM);
	}

	// registry
	{
		wxRegKey Kmain(wxRegKey::HKCU, "Software\\wxHKs");

		if (Kmain.Create(false)) 
		{
			wxMessageBox("Software\\wxHKs regKEY created");
			this->scrollwnd->newHK();
		}
		else {
			this->scrollwnd->getHKs();
		}
	}

	this->SetSizer(MAINsizer);
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




wxIMPLEMENT_APP(MyApp);