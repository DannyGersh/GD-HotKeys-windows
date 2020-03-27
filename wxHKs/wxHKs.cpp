#include "wx/wxprec.h"
#include "wxHKs.h"


bool MainApp::OnInit()
{
	thisPATH = argv[0];
	if (argc > 1) bootARG = argv[1];
	
	MainFrame *frame = new MainFrame();

	return true;
}
wxIMPLEMENT_APP(MainApp);


HK::HK(wxWindow* parent, wxWindowID id, bool c, wxString m, wxString k, wxString e, wxString v, wxString a)
	: 
	wxWindow(parent, id)
{
	vbox = new wxBoxSizer(wxHORIZONTAL);
	ID_nextHK += 8;
	
	// init controls
	{
		wxTextValidator txtv(wxFILTER_ALPHANUMERIC);

		CheckBox = new wxCheckBox(this, ID_nextHK, "");
		C = {
			new wxComboBox(this, ID_nextHK + 2, m, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxComboBox(this, ID_nextHK + 1, k, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_SORT, txtv),
			new wxComboBox(this, ID_nextHK + 3, e, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxComboBox(this, ID_nextHK + 4, v, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY),
			new wxTextCtrl(this, ID_nextHK + 5, a),
		};
		searchBTN = new wxButton(this, ID_nextHK + 6, "");
		deleteBTN = new wxButton(this, ID_nextHK + 7, "");
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
			for (auto str : store.main[store.EXEs])
			{
				string _str = str;
				C.exe->Append(_str);
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
		wxSize size(C.arg->GetSize().y+2, C.arg->GetSize().y+2);
		searchBTN->SetMinSize(size);
		deleteBTN->SetMinSize(size);
		vbox->Add(searchBTN, 0, wxALL | wxEXPAND);
		vbox->Add(deleteBTN, 0, wxALL | wxEXPAND);

		wxBitmap search(wxICON(WXICON_SMALL_CLOSED_FOLDER));
		wxBitmap del(wxICON(IDI_ICON_DEL_CROSS));
		wxImage IMGsearch = search.ConvertToImage().Rescale(size.y - 4, size.y - 4);
		wxImage IMGdel = del.ConvertToImage().Rescale(size.y - 4, size.y - 4);
		
		searchBTN->SetBitmap(IMGsearch);
		deleteBTN->SetBitmap(IMGdel);
	}

	// set toolTips
	{
		wxToolTip* t = new wxToolTip("Enable or disable the hotkey");
		t->SetDelay(1500);
		CheckBox->SetToolTip(t);
		t = new wxToolTip("MODS: ctrl, alt, shift, none");
		t->SetDelay(2000); t->SetReshow(2000);
		C.mod->SetToolTip(t);
		t = new wxToolTip("Any key in the keyboard");
		t->SetDelay(2000); t->SetReshow(2000);
		C.key->SetToolTip(t);
		t = new wxToolTip("The app that executes the file you spesify");
		t->SetDelay(2000); t->SetReshow(2000);
		C.exe->SetToolTip(t);
		t = new wxToolTip("Determin if the window will be visible or not");
		t->SetDelay(2000); t->SetReshow(2000);
		C.vis->SetToolTip(t);
		t = new wxToolTip("The file path to be executed");
		t->SetDelay(2000); t->SetReshow(2000);
		C.arg->SetToolTip(t);
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

	// json
	{
		j.CheckBox = c;
		j.key = k;
		j.mod = m;
		j.exe = e;
		j.vis = v;
		j.arg = a;
		j.container[k] = { j.CheckBox, j.mod, j.key, j.exe, j.vis, j.arg };
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
	if (this->CheckBox->GetValue() == true) {

		int keyCODE = VkKeyScanExA((int)key[0], GetKeyboardLayout(0));
		registerHK();
		
		C.key->Enable(true);
		C.mod->Enable(true);
		C.exe->Enable(true);
		C.vis->Enable(true);
		C.arg->Enable(true);
		searchBTN->Enable(true);

		j.CheckBox = true;
		saveToDisck();
	}
	else {

		UnregisterHotKey(ID);

		C.key->Enable(false);
		C.mod->Enable(false);
		C.exe->Enable(false);
		C.vis->Enable(false);
		C.arg->Enable(false);
		searchBTN->Enable(false);

		j.CheckBox = false;
		saveToDisck();

	}
}
void HK::OnKey(wxCommandEvent& event)
{
	if (finSetup)
	{
		string newVal{ C.key->GetValue() };
		string oldVal = j.key;

		if (newVal == "") {
			C.key->ChangeValue(oldVal);
		}
		else if (newVal.size() > 1 && newVal[0] != 'F')
		{
			C.key->ChangeValue(oldVal);
		}
		else if (isinKEYs(newVal) == false)
		{
			this->key = newVal;

			store.main.erase(oldVal);
			j.key = newVal;
			saveToDisck();

			registerHK();
		}
		else
		{
			wxMessageBox("key not unique");
			C.key->ChangeValue(oldVal); // todo : bug : key changes to blanck
		}

		_MainFrame->SetFocus();
	}
}
void HK::OnMod(wxCommandEvent& event)
{
	wxString m = C.mod->GetValue();
	j.mod = m;
	saveToDisck();
	if (m == mods.ctrl.first) { mod = mods.ctrl.second;}
	if (m == mods.alt.first) { mod = mods.alt.second; }
	if (m == mods.none.first) { mod = mods.none.second; }
	if (m == mods.shift.first) { mod = mods.shift.second; }

	_MainFrame->SetFocus();
	registerHK();
}
void HK::OnExe(wxCommandEvent& event)
{
	j.exe = C.exe->GetValue();
	saveToDisck();
	_MainFrame->SetFocus();
}
void HK::OnVis(wxCommandEvent& event)
{
	string str = C.vis->GetValue();
	if (str == "True") { vis = true; }
	if (str == "False") { vis = false; }
	j.vis = str;
	saveToDisck();

	_MainFrame->SetFocus();
}
void HK::OnArg(wxCommandEvent& event)
{
	j.arg = C.arg->GetValue();
	saveToDisck();
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

	_MainFrame->SetFocus();
}
void HK::OnDelete(wxCommandEvent& event)
{
	wxMessageDialog* dialog = new wxMessageDialog(NULL,
		wxT("Are you sure ?"), wxT(" "),
		wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	if (dialog->ShowModal() == wxID_YES)
	{
		string oldVal = j.key;
		store.main.erase(oldVal);
		ofstream(store.path) << store.main;

		wxWindow* w = this->GetParent();
		this->~HK();
		w->SendSizeEvent();
	}
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
	else { wxMessageBox("vis (HK::executeHK()) is not true nor false"); }

	if (C.exe->GetValue() == store.deft.first) 
	{
		ShellExecuteA(0, 0, C.arg->GetValue().c_str(), 0, 0, arg);
	}
	else 
	{
		//wxRegKey rg(wxRegKey::HKCU, "Software\\wxHKs\\"); 
		//wxString exePATH; rg.QueryValue(C.exe->GetValue(), exePATH);
		wxString exePATH;
		json exes = store.main[store.EXEs];
		for (int i = 0; i < exes.size(); i++)
		{
			string str = exes[i][0];
			if (str == C.exe->GetValue())
			{
				string temp = exes[i][1];
				exePATH = temp;
			}
		}
		ShellExecuteA(0, "open", exePATH.c_str(), C.arg->GetValue().c_str(), 0, arg);
	}
}


MainScrollWND::MainScrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id) 
{}
void MainScrollWND::newHK() {

	if (isinKEYs("key") == false)
	{
		HK* h = new HK(this, wxID_ANY, true, "CTRL", "key", store.deft.first, "True", "file path");

		sizer->Add(h, 0, wxEXPAND, 2);
		this->SetSizer(sizer);
		this->FitInside();
		this->SetScrollRate(10, 10);

		//// registry
		//{
		//	wxRegKey k(wxRegKey::HKCU, "Software\\wxHKs\\key");
		//	if (k.Create(false)) {
		//		k.SetValue("checkbox", 1);
		//		k.SetValue("key", "key");
		//		k.SetValue("mod", h->C.mod->GetString(0));
		//		k.SetValue("exe", h->C.exe->GetString(0));
		//		k.SetValue("vis", h->C.vis->GetString(0));
		//		k.SetValue("arg", "file path");
		//	}
		//}

	}
}
void MainScrollWND::getHKs() {
	
	for (auto& i : store.main.items()) {
		string key = i.key();
		if (key != store.EXEs && key != store.hideOnBoot)
		{
			bool checkbox = i.value()[0];
			string mod = i.value()[1];
			string key = i.value()[2];
			string exe = i.value()[3];
			string vis = i.value()[4];
			string arg = i.value()[5];
			
			if (exe != "EXE name")
			{
				HK* h = new HK(this, wxID_ANY, checkbox, mod, key, exe, vis, arg);
				sizer->Add(h, 0, wxEXPAND, 2);
				this->SetSizer(sizer);
				this->FitInside();
			}
		}
	}

	//for (auto i : store.main)
	{
		//if (str != store.EXEs && str != store.hideOnBoot)
		{
			//bool checkbox = i[1];
			//string mod = i[2]; 
			//string key = i[3];
			//string exe = i[4];
			//string vis = i[5];
			//string arg = i[6];
			//
			//if (exe != "EXE name")
			//{
			//	HK* h = new HK(this, wxID_ANY, checkbox, mod, key, exe, vis, arg);
			//	sizer->Add(h, 0, wxEXPAND, 2);
			//	this->SetSizer(sizer);
			//	this->FitInside();
			//}
		}
	}

	// registry
	{
		//wxRegKey Kmain(wxRegKey::HKCU, "Software\\wxHKs"); Kmain.Open();
		//size_t subkeys; wxString key_name; long why{ 1 };
		//Kmain.GetKeyInfo(&subkeys, NULL, NULL, NULL);
		//Kmain.GetFirstKey(key_name, why);
		//
		//for (size_t i = 0; i < subkeys; i++)
		//{
		//	long checkbox; wxString mod, key, exe, vis, arg;
		//
		//	// quary values
		//	{
		//		wxRegKey k0(wxRegKey::HKCU, "Software\\wxHKs\\" + key_name);
		//		k0.QueryValue("checkbox", &checkbox);
		//		k0.QueryValue("mod", mod);
		//		k0.QueryValue("key", key);
		//		k0.QueryValue("exe", exe);
		//		k0.QueryValue("vis", vis);
		//		k0.QueryValue("arg", arg);
		//	}
		//
		//	if (exe != "EXE name")
		//	{
		//		HK* h = new HK(this, wxID_ANY, checkbox, mod, key, exe, vis, arg);
		//		sizer->Add(h, 0, wxEXPAND, 2);
		//		this->SetSizer(sizer);
		//		this->FitInside();
		//	}
		//
		//	Kmain.GetNextKey(key_name, why);
		//}
	}
}

MainFrame::MainFrame()
	: wxFrame(NULL, wxID_ANY, "wxHotkeys")
{
	_MainFrame = this;
	this->SetIcon(wxICON(IDI_ICON));
	trayICON = new TrayIcon(this);

	// main sizer
	{
		MainScroll = new MainScrollWND(this, wxID_ANY);
		MAINsizer = new wxBoxSizer(wxVERTICAL);

		MAINsizer->Add(MainScroll, 1, wxEXPAND);

		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
		wxIcon ICOnewHK(wxICON(IDI_ICON_ADD_CROSS));
		wxIcon ICOviewEXEs(wxICON(IDI_ICON_CONFIGURE));
		wxIcon ICOquit(wxICON(IDI_ICON_DEL_CROSS));
		wxButton* newHKbtn = new wxButton(this, ID_newHKbtn, "New hotkey");
		wxButton* viewEXEs = new wxButton(this, ID_viewEXEs, "View EXEs");
		wxButton* quit = new wxButton(this, ID_wxHKS_quit, "Quit");
		newHKbtn->SetBitmap(ICOnewHK);
		viewEXEs->SetBitmap(ICOviewEXEs);
		quit->SetBitmap(ICOquit);
		hbox->Add(newHKbtn, 1, wxALL, 10);
		hbox->Add(viewEXEs,1, wxALL, 10);
		hbox->Add(quit, 0, wxALL, 10);

		MAINsizer->Add(hbox, 0, wxEXPAND | wxBOTTOM);
	}

	// json
	{
		ifstream ifile(store.path);
		if (ifile.peek() == ifstream::traits_type::eof())
		{
			store.main[store.hideOnBoot] = true;
			store.main[store.EXEs] = { store.deft.first }; // {} initialize as json array
			ofstream(store.path, ios::out) << store.main;
		}
		else
		{
			ifile >> store.main;

			if (store.main.find(store.hideOnBoot) == store.main.end())
				store.main[store.hideOnBoot] = true;

			if (store.main.find(store.EXEs) == store.main.end())
				store.main[store.EXEs] = { store.deft.first }; // {} initialize as json array

			ofstream(store.path) << store.main;

			this->MainScroll->getHKs();
		}
		ifile.close();
	}

	this->SetSize(800, 500);
	this->SetSizer(MAINsizer);
	
	if (bootARG != "/MINIMIZED")
	{
		this->Show();
	}

	finSetup = true;
}
MainFrame::~MainFrame()
{
	trayICON->~TrayIcon();
}
void MainFrame::newHK(wxCommandEvent& event)
{
	this->MainScroll->newHK();
}
void MainFrame::viewEXEs(wxCommandEvent& event) 
{
	EXEsFrame * exe = new EXEsFrame(this);
	exe->Show();
}
void MainFrame::OnExit(wxCommandEvent& event)
{
	this->Destroy();
}
void MainFrame::close(wxCloseEvent& event)
{
	this->Show(false);
}

void EXEsFrame::OnOK(wxCommandEvent& event) 
{
	bool proceed{ true };
	std::forward_list<wxString> testUNIQ;
	
	// test if all names are valid
	for (auto& i : EXEs)
		{
			int count{ 0 };
			for (auto&ii : EXEs){
				if (ii->c.name->GetValue() == i->c.name->GetValue()){
					count++;
				}
				if (count == 2){
					wxMessageBox("make sure all names are unique");
					proceed = false;
					break;
				}
			}
	
			if (i->c.name->GetValue() == "")
			{
				wxMessageBox("Pleas fill all names");
				proceed = false;
			}
			if (i->c.name->GetValue() == "EXE name")
			{
				wxMessageBox("\"EXE name\" is not allowd.");
				proceed = false;
			}
			if (proceed == false)
			{
				break;
			}
		}
	
	if (proceed) {
	
		//wxRegKey reg(wxRegKey::HKCU, "Software\\wxHKs"); reg.Open();
		
		// delete all values from wxHKs regKey
		{
			ofstream(store.path, ios::out) << "";
			//size_t RGvalues; wxString valueNAME; long why{ 1 };
			//
			//reg.GetKeyInfo(NULL, NULL, &RGvalues, NULL);
			//reg.GetFirstValue(valueNAME, why);
			//
			//wxArrayString deleteTHIS;
			//for (size_t i = 0; i < RGvalues; ++i) 
			//{
			//	deleteTHIS.Add(valueNAME);
			//	reg.GetNextValue(valueNAME, why);
			//}
			//for (auto&i : deleteTHIS) 
			//{
			//	reg.DeleteValue(i);
			//}
			//for (auto&i : EXEs)
			//{
			//	reg.SetValue(i->c.name->GetValue(), i->c.path->GetValue());
			//}
			//reg.SetValue("Default", " ");
			//
			//reg.Close();
		}	
	
		for (auto&hk : HKs) // uppdate registry subkeys and HKs frame
		{
			//wxRegKey subREG(wxRegKey::HKCU, "Software\\wxHKs\\" + hk->key);
			wxComboBox* c = hk->C.exe;
	
			wxString oldSELECTION = c->GetStringSelection();
			c->Clear();
			c->Append(store.deft.first);
	
			for (auto&exe : EXEs)
			{
				wxString oldNAME = exe->originalNAME;
				wxString newNAME = exe->c.name->GetValue();
				
				if (newNAME != "EXE name")
				{
					exe->j.name = newNAME;
					exe->j.path = exe->c.path->GetValue();
					exe->saveToDisck();
				}

				if (newNAME != "EXE name")
				{
					c->Append(newNAME);
				}
	
				if (oldNAME == oldSELECTION) // the selected string wase changed
				{
					c->SetStringSelection(newNAME);
	
					//wxRegKey subREG(wxRegKey::HKCU, "Software\\wxHKs\\" + hk->key);
					//subREG.SetValue("exe", newNAME);
					hk->j.exe = newNAME;
					hk->saveToDisck();
				}
	
			}
	
			if (c->GetValue() == "") // selected value wase deleted
			{
				c->SetSelection(c->FindString("Default"));
				//subREG.SetValue("exe", "Default");
				hk->j.exe = store.deft.first;
				hk->saveToDisck();
			}
		}
		
		wxWindow* hkFRAME = HKs[0]->GetParent();
		hkFRAME->SendSizeEvent();
	
		this->Close();
	}

}















