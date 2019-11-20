#include "wx/wxprec.h"
#include "wxHKs.h"
#include "ViewEXEs.h"


bool MainApp::OnInit()
{
	MainFrame *frame = new MainFrame();
	frame->Show(true);

	return true;
}
wxIMPLEMENT_APP(MainApp);


HK::HK(wxWindow* parent, wxWindowID id, long c, wxString m, wxString k, wxString e, wxString v, wxString a)
	: 
	wxWindow(parent, id)
{	
	vbox = new wxBoxSizer(wxHORIZONTAL);
	ID_nextHK += 8;

	// init controls
	{
		CheckBox = new wxCheckBox(this, ID_nextHK, "");
		C = {
			new wxComboBox(this, ID_nextHK + 2, m, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxComboBox(this, ID_nextHK + 1, k),
			new wxComboBox(this, ID_nextHK + 3, e, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxComboBox(this, ID_nextHK + 4, v, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxComboBox(this, ID_nextHK + 5, a),
		};
		searchBTN = new wxButton(this, ID_nextHK + 6, "6");
		deleteBTN = new wxButton(this, ID_nextHK + 7, "7");
	}

	// checkbox
	{
		if (c) {
			C.key->Enable(true);
			C.mod->Enable(true);
			C.exe->Enable(true);
			C.vis->Enable(true);
			C.arg->Enable(true);
			searchBTN->Enable(true);
		}
		else {
			C.key->Enable(false);
			C.mod->Enable(false);
			C.exe->Enable(false);
			C.vis->Enable(false);
			C.arg->Enable(false);
			searchBTN->Enable(false);
		}
		CheckBox->SetValue(c);
		vbox->Add(CheckBox, 0, wxCENTER | wxALL | wxRIGHT, 2);
	}
	 
	// comboboxes
	{
		// mod
		{
			wxString strList[5]{ "CTRL", "ALT", "WIN", "SHIFT", "NONE" };
			C.mod->Append(wxArrayString(5, strList));
			int index = C.mod->FindString(m);
			C.mod->SetSelection(index);
		}
		// key
		{
			for (int i = 1; i <= 12; i++) {
				C.key->Append("F" + std::to_string(i));
			}
			C.key->SetValue(k);
			key = k;
		}
		// exe
		{
			size_t values; wxString valueNAME; long why{ 1 };
			wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs"); rk.Open();
			rk.GetKeyInfo(NULL, NULL, &values, NULL);
			rk.GetFirstValue(valueNAME, why);

			for (size_t i = 0; i < values; i++)
			{
				C.exe->Append(valueNAME);
				rk.GetNextValue(valueNAME, why);
			}
			int index = C.exe->FindString(e);
			C.exe->SetSelection(index);
		}
		// vis
		{
			C.vis->Append("True");
			C.vis->Append("False");
			C.vis->SetSelection(0);
		}
		// todo: arg

		vbox->Add(C.mod, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.key, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.exe, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.vis, 0, wxCENTER | wxALL, 2);
		vbox->Add(C.arg, 1, wxCENTER | wxALL, 2); // note second arg
	}

	// buttons
	{
		searchBTN->SetMinSize({ searchBTN->GetSize().y, searchBTN->GetSize().y });
		deleteBTN->SetMinSize({ deleteBTN->GetSize().y, deleteBTN->GetSize().y });
		vbox->Add(searchBTN, 0, wxALL, 2);
		vbox->Add(deleteBTN, 0, wxALL, 2);
	}

	// bind events
	{
		Bind(wxEVT_CHECKBOX, &HK::OnCheckBox, this, ID_nextHK);
		Bind(wxEVT_TEXT, &HK::OnKey, this, ID_nextHK + 1);
		Bind(wxEVT_TEXT, &HK::OnMod, this, ID_nextHK + 2);
		Bind(wxEVT_TEXT, &HK::OnExe, this, ID_nextHK + 3);
		Bind(wxEVT_TEXT, &HK::OnVis, this, ID_nextHK + 4);
		Bind(wxEVT_TEXT, &HK::OnArg, this, ID_nextHK + 5);
		Bind(wxEVT_BUTTON, &HK::OnSearch, this, ID_nextHK + 6);
		Bind(wxEVT_BUTTON, &HK::OnDelete, this, ID_nextHK + 7);
	}

	// bind and register hotkey
	{
		if (C.key->GetValue() != "key") {
			ID = ID_nextHK;
			int keyCODE = VkKeyScanExA(k[0], GetKeyboardLayout(0));
			RegisterHotKey(ID, mods.ctrl.second, keyCODE);
			Bind(wxEVT_HOTKEY, &HK::test, this, ID);
			ID_next_hotkey++;
		}
		else{
			// pseudo register hotkey
			RegisterHotKey(ID, mods.ctrl.second, 0);
			Bind(wxEVT_HOTKEY, &HK::test, this, ID);
		}
	}

	HKs.push_back(this);
	this->SetSizer(vbox);
	this->FitInside();

}
HK::~HK()
{
	CheckBox->Destroy();
	C.mod->Destroy();
	C.key->Destroy();
	C.exe->Destroy();
	C.vis->Destroy();
	C.arg->Destroy();
	searchBTN->Destroy();
	deleteBTN->Destroy();

	//HKs.erase(HKs.begin() + index);

}
void HK::OnCheckBox(wxCommandEvent& event)
{
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + C.key->GetValue());

	if (this->CheckBox->GetValue() == true) {
		C.key->Enable(true);
		C.mod->Enable(true);
		C.exe->Enable(true);
		C.vis->Enable(true);
		C.arg->Enable(true);
		searchBTN->Enable(true);
		rk.SetValue("checkbox", 1);
	}
	else {
		C.key->Enable(false);
		C.mod->Enable(false);
		C.exe->Enable(false);
		C.vis->Enable(false);
		C.arg->Enable(false);
		searchBTN->Enable(false);
		rk.SetValue("checkbox", 0);
	}
}
void HK::OnKey(wxCommandEvent& event)
{
	if (finSetup)
	{
		wxString newVal{ C.key->GetValue() };
		wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
		wxString oldVal; rk.QueryValue("key", oldVal);
		wxArrayString Fs = C.key->GetStrings();

		if (newVal == "") {
			C.key->ChangeValue(oldVal);
		}
		
		else if (isinKEYs(newVal) == false)
		{
			this->key = newVal;
			rk.Rename(newVal);
			rk.SetValue("key", newVal);

			int NEWkeyCODE = VkKeyScanExA((int)newVal[0], GetKeyboardLayout(0));
			int OLDkeyCODE = VkKeyScanExA((int)oldVal[0], GetKeyboardLayout(0));
			UnregisterHotKey(ID);
			RegisterHotKey(ID, mods.ctrl.second, NEWkeyCODE);
		}
		else
		{
			wxMessageBox("key not unique");
			C.key->ChangeValue(oldVal);
		}
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
void HK::OnSearch(wxCommandEvent& event)
{
}
void HK::OnDelete(wxCommandEvent& event)
{
}
void HK::test(wxKeyEvent& event) {
	ShellExecuteA(0, "open", "C:\\danny\\poooppp.txt", 0, 0, SW_MAXIMIZE);
}


MainScrollWND::MainScrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id) 
{}
void MainScrollWND::newHK() {

	if (isinKEYs("key") == false)
	{
		HK* h = new HK(this, wxID_ANY, 1, "CTRL", "key", "Paint", "is visible", "file path");
		
		// main
		{
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
				k.SetValue("mod", h->C.mod->GetString(0));
				k.SetValue("exe", h->C.exe->GetString(0));
				k.SetValue("vis", h->C.vis->GetString(0));
				k.SetValue("arg", "file path");
			}
		}

	}
}
void MainScrollWND::getHKs() {
	
	wxRegKey Kmain(wxRegKey::HKCU, "Software\\wxHKs"); Kmain.Open();
	size_t subkeys; wxString key_name; long why{ 1 };
	Kmain.GetKeyInfo(&subkeys, NULL, NULL, NULL);
	Kmain.GetFirstKey(key_name, why); 

	for (size_t i = 0; i < subkeys; i++)
	{
		long checkbox; wxString mod, key, exe, vis, arg; 
		
		// quary values
		{
			wxRegKey k0(wxRegKey::HKCU, "Software\\wxHKs\\" + key_name);
			k0.QueryValue("checkbox", &checkbox);
			k0.QueryValue("mod", mod);
			k0.QueryValue("key", key);
			k0.QueryValue("exe", exe);
			k0.QueryValue("vis", vis);
			k0.QueryValue("arg", arg);
		}

		HK* h = new HK(this, wxID_ANY, checkbox, mod, key, exe, vis, arg);
		sizer->Add(h, 0, wxEXPAND, 2);
		this->SetSizer(sizer);
		this->FitInside();

		Kmain.GetNextKey(key_name, why);
	}
}
void MainScrollWND::test(wxKeyEvent& event) {
	ShellExecuteA(0, "open", "C:\\danny\\poooppp.txt", 0, 0, SW_MAXIMIZE);
}

MainFrame::MainFrame()
	: wxFrame(NULL, wxID_ANY, "wx Hotkeys")
{
	this->SetIcon(wxICON("C:\\danny\\win32HKs.ico"));
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
		Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
		Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
		Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
	}

	// main sizer
	{
		MainScroll = new MainScrollWND(this, wxID_ANY);
		MAINsizer = new wxBoxSizer(wxVERTICAL);

		MAINsizer->Add(MainScroll, 1, wxEXPAND);

		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
		hbox->Add(
			new wxButton(this, ID_newHKbtn, "New hotkey"),
			1, wxALL, 10
		);
		hbox->Add(
			new wxButton(this, ID_viewEXEs, "View EXEs"),
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
			Kmain.SetValue("Paint", "C:\\Windows\\System32\\mspaint.exe");
			this->MainScroll->newHK();
		}
		else {
			this->MainScroll->getHKs();
		}
	}

	this->SetSize(800, 800);
	this->SetSizer(MAINsizer);
	this->Show();

	finSetup = true;
}
void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets Hello World example",
		"About Hello World", wxOK | wxICON_INFORMATION);
}
void MainFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::newHK(wxCommandEvent& event)
{
	this->MainScroll->newHK();
}
void MainFrame::viewEXEs(wxCommandEvent& event) 
{
	EXEsFrame * exe = new EXEsFrame();
	exe->Show();
}




void EXEsFrame::OnOK(wxCommandEvent& event) 
{
	bool proceed{ true };
	
	// test if all names are valid
	{
		std::vector<EXE*> isUnique;
		for (auto& i : EXEs)
		{
			isUnique.push_back(i);
			int count{ 0 };
			for (auto& isU : isUnique)
			{
				if (isU->c.name->GetValue() == i->c.name->GetValue())
				{
					count++;
				}
				if (count == 2)
				{
					wxMessageBox("all names must be unique");
					proceed = false;
				}
			}


			if (i->c.name->GetValue() == "")
			{
				wxMessageBox("Pleas fill all names");
				proceed = false;
			}
		}
	}
	

	if (proceed) {

		// rename exe names in main RegKey
		{
			size_t values; wxString valueNAME; long why{ 1 };
			wxRegKey reg(wxRegKey::HKCU, "Software\\wxHKs"); reg.Open();
			reg.GetKeyInfo(NULL, NULL, &values, NULL);
			reg.GetFirstValue(valueNAME, why);

			for (size_t i = 0; i < values; i++)
			{
				for (auto& ii : EXEs) {
					if (valueNAME == ii->originalNAME)
					{
						reg.SetValue(ii->originalNAME, ii->c.path->GetValue());

						if (ii->originalNAME != ii->c.name->GetValue())
						{
							reg.RenameValue(ii->originalNAME, ii->c.name->GetValue());
						}
					}
				}
				reg.GetNextValue(valueNAME, why);
			}
			reg.Close();
		}

		// rename exe names in all subkeys of main key and update MainFrame
		{
			for (auto& i : EXEs) {

				wxString newNAME = i->c.name->GetValue();
				wxString oldNAME = i->originalNAME;
				
				for (auto& ii : HKs) {

					int index = ii->C.exe->FindString(oldNAME);

					if (index == wxNOT_FOUND) 
					{
						ii->C.exe->Append(newNAME);
					}
					else
					{
						ii->C.exe->SetString(index, newNAME);
						if (oldNAME != newNAME) {
							wxRegKey reg(wxRegKey::HKCU, "Software\\wxHKs\\" + ii->key); 
							reg.SetValue("exe", newNAME);
						}
					}

					wxWindow* w = ii->GetParent();
					ii->C.exe->Fit();
					w->SendSizeEvent();

				}
			}
		}

		this->Close();
	}

}







