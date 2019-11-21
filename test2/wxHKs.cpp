#include "wx/wxprec.h"
#include "wxHKs.h"
#include "ViewEXEs.h"

// todo : add delHK functionallity.

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
			new wxTextCtrl(this, ID_nextHK + 5, a),
		};
		searchBTN = new wxButton(this, ID_nextHK + 6, " ");
		deleteBTN = new wxButton(this, ID_nextHK + 7, " ");
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
			wxString strList[5]{ "CTRL", "ALT", "SHIFT", "NONE" };
			C.mod->Append(wxArrayString(4, strList));
			int index = C.mod->FindString(m);
			C.mod->SetSelection(index);

			if (m == mods.ctrl.first) { mod = mods.ctrl.second; }
			if (m == mods.alt.first) { mod = mods.alt.second; }
			if (m == mods.none.first) { mod = mods.none.second; }
			if (m == mods.shift.first) { mod = mods.shift.second; }
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
			if (v == "True") { vis = true; }
			else if (v == "False") { vis = false; }
			else { wxMessageBox("'v' (HK::vis) (from HK::HK) is nither true nor false"); }

			C.vis->Append("True");
			C.vis->Append("False");
			C.vis->SetSelection(C.vis->FindString(v));
		}

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

		wxIcon search(wxICON(WXICON_SMALL_CLOSED_FOLDER));
		wxIcon del(wxICON(IDI_ICON_DEL_CROSS));
		searchBTN->SetBitmap(search);
		deleteBTN->SetBitmap(del);
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

	// register and bind hotkey
	{
		ID = ID_nextHK;
		Bind(wxEVT_HOTKEY, &HK::executeHK, this, ID);
		ID_next_hotkey++;

		if (c)
		{
			registerHK();
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

	UnregisterHotKey(ID);
	Unbind(wxEVT_HOTKEY, &HK::executeHK, this, ID);
}
void HK::OnCheckBox(wxCommandEvent& event)
{
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + C.key->GetValue());

	if (this->CheckBox->GetValue() == true) {

		int keyCODE = VkKeyScanExA((int)key[0], GetKeyboardLayout(0));
		RegisterHotKey(ID, mod, keyCODE);

		C.key->Enable(true);
		C.mod->Enable(true);
		C.exe->Enable(true);
		C.vis->Enable(true);
		C.arg->Enable(true);
		searchBTN->Enable(true);
		rk.SetValue("checkbox", 1);

	}
	else {

		UnregisterHotKey(ID);

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

			registerHK();
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
	wxString m = C.mod->GetValue();
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
	rk.SetValue("mod", m);
	if (m == mods.ctrl.first) { mod = mods.ctrl.second;}
	if (m == mods.alt.first) { mod = mods.alt.second; }
	if (m == mods.none.first) { mod = mods.none.second; }
	if (m == mods.shift.first) { mod = mods.shift.second; }

	registerHK();
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

	if (vis) { vis = false; }
	else { vis = true; }
}
void HK::OnArg(wxCommandEvent& event)
{
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + key);
	rk.SetValue("arg", C.arg->GetValue());
}
void HK::OnSearch(wxCommandEvent& event)
{
	wxFileDialog
		openFileDialog(this, _("Search file"), "", "",
			"", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_OK) 
	{
		C.arg->SetValue(openFileDialog.GetPath());
	}

}
void HK::OnDelete(wxCommandEvent& event)
{
	wxWindow* w = this->GetParent();
	wxString oldKEY = key;
	this->~HK();

	w->SendSizeEvent();

	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs\\" + oldKEY);
	rk.DeleteSelf();
}
void HK::registerHK()
{
	if (key[0] != 'F') {
		int keyCODE = VkKeyScanExA((int)key[0], GetKeyboardLayout(0));
		UnregisterHotKey(ID);
		RegisterHotKey(ID, mod, keyCODE);
	}
	else {
		if (key == "F1") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x70); }
		else if (key == "F2") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x71); }
		else if (key == "F3") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x72); }
		else if (key == "F4") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x73); }
		else if (key == "F5") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x74); }
		else if (key == "F6") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x75); }
		else if (key == "F7") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x76); }
		else if (key == "F8") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x77); }
		else if (key == "F9") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x78); }
		else if (key == "F10") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x79); }
		else if (key == "F11") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x7A); }
		else if (key == "F12") { UnregisterHotKey(ID); RegisterHotKey(ID, mod, 0x7B); }
	}
}
void HK::executeHK(wxKeyEvent& event) {
	
	int arg;
	if (vis == false) { arg = SW_HIDE; }
	else if (vis == true) { arg = SW_SHOW; }
	else { wxMessageBox("vis (HK::executeHK) is not true nor false"); }

	if (C.exe->GetValue() == "Default") 
	{
		ShellExecuteA(0, 0, C.arg->GetValue(), 0, 0, arg);
	}
	else 
	{
		wxRegKey rg(wxRegKey::HKCU, "Software\\wxHKs\\"); 
		wxString exePATH; rg.QueryValue(C.exe->GetValue(), exePATH);
		ShellExecuteA(0, "open", exePATH, C.arg->GetValue(), 0, arg);
	}
}


MainScrollWND::MainScrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id) 
{}
void MainScrollWND::newHK() {

	if (isinKEYs("key") == false)
	{
		HK* h = new HK(this, wxID_ANY, 1, "CTRL", "key", "Default", "True", "file path");
		
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
	this->SetIcon(wxICON(IDI_ICON));
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
		wxIcon ICOnewHK(wxICON(IDI_ICON_ADD_CROSS));
		wxIcon ICOviewEXEs(wxICON(IDI_ICON_CONFIGURE));
		wxButton* newHKbtn = new wxButton(this, ID_newHKbtn, "New hotkey");
		wxButton* viewEXEs = new wxButton(this, ID_viewEXEs, "View EXEs");
		newHKbtn->SetBitmap(ICOnewHK);
		viewEXEs->SetBitmap(ICOviewEXEs);
		hbox->Add(newHKbtn, 1, wxALL, 10);
		hbox->Add(viewEXEs,1, wxALL, 10);

		MAINsizer->Add(hbox, 0, wxEXPAND | wxBOTTOM);
	}

	// registry
	{
		wxRegKey Kmain(wxRegKey::HKCU, "Software\\wxHKs");

		if (Kmain.Create(false)) 
		{
			wxMessageBox("Software\\wxHKs regKEY created");
			Kmain.SetValue("Default", " ");
			this->MainScroll->newHK();
		}
		else {
			this->MainScroll->getHKs();
		}
	}

	this->SetSize(800, 500);
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
							wxString tempSTR; reg.QueryValue("exe", tempSTR);
							if(oldNAME==tempSTR)
							{
								reg.SetValue("exe", newNAME);
							}
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







