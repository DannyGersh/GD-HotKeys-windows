#include "wx/wxprec.h"
#include <wx/msw/registry.h>
#include "Main.h"


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
	RegisterHotKey(12345, mods.ctrl.vk, 0x70);
	Bind(wxEVT_HOTKEY, &HK::test, this, 12345);

	// init controls
	{
		CheckBox = new wxCheckBox(this, ID_nextHK, "");
		C = {
			new wxComboBox(this, ID_nextHK + 2, "mod", wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxComboBox(this, ID_nextHK + 1, "key"),
			new wxComboBox(this, ID_nextHK + 3, "exe path", wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxComboBox(this, ID_nextHK + 4, "is visible", wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
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
		// mod
		{
			wxString strList[5]{ "CTRL", "ALT", "WIN", "SHIFT", "NONE" };
			C.mod->Append(wxArrayString(5, strList));
			C.mod->SetSelection(0);
		}
		// key
		{
			for (int i = 1; i <= 12; i++) {
				C.key->Append("F" + std::to_string(i));
			}
		}
		// vis
		{
			C.vis->Append("True");
			C.vis->Append("False");
			C.vis->SetSelection(0);
		}

		vbox->Add(C.mod, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.key, 0, wxCENTER | wxALL, 2);
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
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);

	if (this->CheckBox->GetValue() == true) {
		C.key->Enable(true);
		C.mod->Enable(true);
		C.exe->Enable(true);
		C.vis->Enable(true);
		C.arg->Enable(true);
		folderBTN->Enable(true);
		rk.SetValue("checkbox", 1);
	}
	else {
		C.key->Enable(false);
		C.mod->Enable(false);
		C.exe->Enable(false);
		C.vis->Enable(false);
		C.arg->Enable(false);
		folderBTN->Enable(false);
		rk.SetValue("checkbox", 0);
	}
}
void HK::OnKey(wxCommandEvent& event)
{
	if (finSetup && processTextCtrl)
	{
		processTextCtrl = false;

		wxString newVal{ C.key->GetValue() };
		wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
		wxString oldVal; rk.QueryValue("key", oldVal); // oldVal == key ... at the moment
		wxArrayString Fs = C.key->GetStrings();

		int scenario{ 0 };

		if (isin(Fs, newVal) && oldVal != newVal)
		{
			if (isin(STRkeys, newVal)) { scenario = 1; } 
		}
		else if(oldVal != newVal)
		{
			if (isin(STRkeys, newVal)) { scenario = 2; } 
		}
		if (newVal == "")
		{
			scenario = 3;
		}

		/*
		scenarios:
		0. key is unique
		1. newVal is NOT unique and its F1, F2 ...
		2. newVal is NOT unique and its NOT F1, F2 ...
		3. newVal is empty ""
		*/
		
		switch (scenario)
		{

		case 0: 
		{
			if (newVal != oldVal)
			{
				replaceSTRinVEC(STRkeys, oldVal, newVal);
				key = newVal; // HK::key
				rk.Rename(newVal);
				rk.SetValue("key", newVal);
			}
			break;
		}
		case 1:
		{
			wxMessageBox("Key not unique:" + wxString(" ") + newVal);
			C.key->SetValue(oldVal);
			break;
		}
		case 2:
		{
			wxMessageBox("Key not unique:" + wxString(" ") + newVal);
			C.key->SetValue(oldVal);
			break;
		}
		case 3:
		{
			C.key->SetValue(oldVal);
			break;
		}

		}
	}
	if (!processTextCtrl)
	{
		processTextCtrl = true;
	}
} 
void HK::OnMod(wxCommandEvent& event)
{
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
	rk.SetValue("mod", C.mod->GetValue());
}
void HK::OnExe(wxCommandEvent& event)
{
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
	rk.SetValue("exe", C.exe->GetValue());
}
void HK::OnVis(wxCommandEvent& event)
{
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
	rk.SetValue("vis", C.vis->GetValue());
}
void HK::OnArg(wxCommandEvent& event)
{
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
	rk.SetValue("arg", C.arg->GetValue());
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

	if (!isin(STRkeys, "key"))
	{
		// main
		{
			HK* h = new HK(this, wxID_ANY);
			h->SetLabel("key");
			STRkeys.push_back("key");
			h->key = "key";

			sizer->Add(h, 0, wxEXPAND, 2);
			this->SetSizer(sizer);
			this->FitInside();
			this->SetScrollRate(10, 10);
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

		// registry
		{
			wxRegKey k0(wxRegKey::HKCU, "Software\\wxHKs\\" + key_name);

			k0.QueryValue("checkbox", &checkbox);
			k0.QueryValue("key", key); h->key = key;
			k0.QueryValue("mod", mod);
			k0.QueryValue("exe", exe);
			k0.QueryValue("vis", vis);
			k0.QueryValue("arg", arg);
		}

		// set control values
		{
			h->CheckBox->SetValue(checkbox);
			h->C.key->SetValue(key);
			h->C.mod->SetValue(mod);
			h->C.exe->SetValue(exe);
			h->C.vis->SetValue(vis);
			h->C.arg->SetValue(arg);
		}

		// checkbox
		{
			if (checkbox == true) {
				h->C.key->Enable(true);
				h->C.mod->Enable(true);
				h->C.exe->Enable(true);
				h->C.vis->Enable(true);
				h->C.arg->Enable(true);
				h->folderBTN->Enable(true);
			}
			else {
				h->C.key->Enable(false);
				h->C.mod->Enable(false);
				h->C.exe->Enable(false);
				h->C.vis->Enable(false);
				h->C.arg->Enable(false);
				h->folderBTN->Enable(false);
			}
		}

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

	// main sizer
	{
		scrollwnd = new scrollWND(this, wxID_ANY);
		MAINsizer = new wxBoxSizer(wxVERTICAL);

		MAINsizer->Add(scrollwnd, 1, wxEXPAND);

		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
		hbox->Add(
			new wxButton(this, ID_newHKbtn, "New hotkey"),
			1, wxALL, 10
		);
		hbox->Add(
			new wxButton(this, ID_newHKbtn, "View EXEs"),
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

	finSetup = true;
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

void MyFrame::viewEXEs(wxCommandEvent& event) {

}




wxIMPLEMENT_APP(MyApp);