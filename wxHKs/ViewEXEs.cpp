#include "wx/wxprec.h"
#include "ViewEXEs.h"

json jmain;
string jnewEXE = "EXE name";
string jfilePath = "userDATAexe.json";
std::vector<EXE*> EXEs;


void EXE::saveToDisck()
{
	string name = j.name;
	jmain[name] = j.path;
	ofstream(jfilePath) << jmain;
}

EXE::EXE(wxWindow* parent, wxWindowID id, wxString name, wxString path) : wxWindow(parent, id)
{
	// text controls
	{
		c.name = new wxTextCtrl(this, wxID_ANY, name, wxDefaultPosition, wxSize(150, 10));
		c.path = new wxTextCtrl(this, wxID_ANY, path);
		vbox->Add(c.name, 0, wxCENTER | wxALL | wxEXPAND, 2);
		vbox->Add(c.path, 1, wxCENTER | wxALL | wxEXPAND, 2);
	} 

	// buttons
	{
		c.search = new wxButton(this, ID_search, " ");
		c.del = new wxButton(this, ID_del, " ");
		wxBitmap search(wxICON(WXICON_SMALL_CLOSED_FOLDER));
		wxBitmap del(wxICON(IDI_ICON_DEL_CROSS));

		wxSize size(c.path->GetSize().y + 2, c.path->GetSize().y + 2);
		wxImage IMGsearch = search.ConvertToImage().Rescale(size.y - 4, size.y - 4);
		wxImage IMGdel = del.ConvertToImage().Rescale(size.y - 4, size.y - 4);

		c.search->SetBitmap(IMGsearch);
		c.del->SetBitmap(IMGdel);

		c.search->SetMinSize(size);
		c.del->SetMinSize(size);
		vbox->Add(c.search, 0, wxALL, 2);
		vbox->Add(c.del, 0, wxALL, 2);
	}

	// registry
	{
		//wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs"); 
		//rk.SetValue(name, path);
		if (name != jnewEXE)
		{
			jmain[name] = path;
		}
	}

	// bind events
	{
		Bind(wxEVT_BUTTON, &EXE::OnSearch, this, ID_search);
		Bind(wxEVT_BUTTON, &EXE::OnDel, this, ID_del);
	}

	this->originalNAME = name;
	this->originalPATH = path;
	EXEs.push_back(this);

	this->SetSizer(vbox);
	this->FitInside();

}
void EXE::OnSearch(wxCommandEvent& event)
{
	wxFileDialog
		openFileDialog(this, _("Search file"), "", "",
			"", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_OK)
	{
		c.path->SetValue(openFileDialog.GetPath());
	}
}
void EXE::OnDel(wxCommandEvent& event)
{
	wxMessageDialog* dialog = new wxMessageDialog(NULL,
		wxT("Are you sure ?"), wxT(" "),
		wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	if (dialog->ShowModal() == wxID_YES)
	{
		wxWindow* w = this->GetParent();
		std::vector<EXE*>::iterator it = std::find(EXEs.begin(), EXEs.end(), this);

		if (it != EXEs.end()) {
			int index = std::distance(EXEs.begin(), it);
			EXEs.erase(EXEs.begin() + index);
			jmain.erase(c.name->GetValue());
		}

		this->Destroy();
		w->SendSizeEvent();
	};
}

EXEScrollWND::EXEScrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id)
{}
void EXEScrollWND::newEXE() 
{
	EXE* h = new EXE(this, wxID_ANY, jnewEXE, "EXE path");

	sizer->Add(h, 0, wxEXPAND, 2);
	this->SetSizer(sizer);
	this->FitInside();
	this->SetScrollRate(10, 10);
}
void EXEScrollWND::getEXEs() 
{
	for (auto& i : jmain.items()) {
		string name = i.key();
		if (name != jnewEXE)
		{
			string _path = i.value();
	
			EXE* h = new EXE(this, wxID_ANY, name, _path);
			sizer->Add(h, 0, wxEXPAND, 2);
			this->SetSizer(sizer);
			this->FitInside();
			this->SetScrollRate(10, 10);
		}
	}
}

EXEsFrame::EXEsFrame(wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "EXE settings")
{
	wxIcon ICOviewEXEs(wxICON(IDI_ICON_CONFIGURE));
	this->SetIcon(ICOviewEXEs);

	ifstream ifile(jfilePath);
	if (ifile.peek() == ifstream::traits_type::eof())
	{
		ofstream(jfilePath, ios::out) << "{}";
	}
	else
	{
		ifstream(jfilePath) >> jmain;
	}

	// add elements
	{
		scrollwnd = new EXEScrollWND(this, wxID_ANY);
		MAINsizer = new wxBoxSizer(wxVERTICAL);

		MAINsizer->Add(scrollwnd, 1, wxEXPAND);

		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
		wxButton* New = new wxButton(this, ID_newEXE, "New EXE");
		wxButton* cancel = new wxButton(this, ID_cancel, "cancel");
		wxButton* ok = new wxButton(this, ID_ok, "ok");
		
		wxIcon ICOnewHK(wxICON(IDI_ICON_ADD_CROSS));
		New->SetBitmap(ICOnewHK);

		hbox->Add(New, 1, wxALL, 10);
		hbox->Add(cancel, 0, wxALL | wxEXPAND, 10);
		hbox->Add(ok, 0, wxALL | wxEXPAND, 10);

		MAINsizer->Add(hbox, 0, wxEXPAND | wxBOTTOM);

		this->scrollwnd->getEXEs();
	}

	Bind(wxEVT_BUTTON, &EXEsFrame::OnNewEXE, this, ID_newEXE);
	Bind(wxEVT_BUTTON, &EXEsFrame::OnCancle, this, ID_cancel);
	Bind(wxEVT_BUTTON, &EXEsFrame::OnOK, this, ID_ok);

	this->SetSize(600, 400);
	this->SetSizer(MAINsizer);
	this->Show();
}
EXEsFrame::~EXEsFrame()
{
	scrollwnd->Close();
	EXEs.clear();
}
void EXEsFrame::OnNewEXE(wxCommandEvent& event)
{
	scrollwnd->newEXE();
}
void EXEsFrame::OnCancle(wxCommandEvent& event)
{
	this->Destroy();
}

