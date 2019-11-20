#include "wx/wxprec.h"
#include "ViewEXEs.h"

extern int ID_nextHK;
extern bool finSetup;
extern MODS mods;

// pair < original exe name, changing name > // from ViewEXEs.h
std::vector<  pair<wxString, wxString>  > namePAIRS; //extern
std::vector<  pair<wxString, wxString>  > pathPAIRS; //extern
std::vector<EXE*> EXEs; // extern


EXE::EXE(wxWindow* parent, wxWindowID id, wxString name, wxString path) : wxWindow(parent, id)
{
	// text controls
	{
		c.name = new wxTextCtrl(this, ID_name, name, wxDefaultPosition, wxSize(150, 10));
		c.path = new wxTextCtrl(this, ID_path, path);
		vbox->Add(c.name, 0, wxCENTER | wxALL | wxEXPAND, 2);
		vbox->Add(c.path, 1, wxCENTER | wxALL | wxEXPAND, 2);
	} 

	// buttons
	{
		c.search = new wxButton(this, ID_search, "6");
		c.del = new wxButton(this, ID_del, "7");

		c.search->SetMinSize({ c.search->GetSize().y, c.search->GetSize().y });
		c.del->SetMinSize({ c.del->GetSize().y, c.del->GetSize().y });
		vbox->Add(c.search, 0, wxALL, 2);
		vbox->Add(c.del, 0, wxALL, 2);
	}

	// registry
	{
		wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs"); 
		rk.SetValue(name, path);
	}

	// bind events
	{
		Bind(wxEVT_TEXT, &EXE::OnName, this, ID_name);
		Bind(wxEVT_TEXT, &EXE::OnPath, this, ID_path);
		Bind(wxEVT_BUTTON, &EXE::OnSearch, this, ID_search);
		Bind(wxEVT_BUTTON, &EXE::OnDel, this, ID_del);
	}

	this->originalNAME = name;
	this->originalPATH = path;
	EXEs.push_back(this);

	this->SetSizer(vbox);
	this->FitInside();

}
void EXE::OnName(wxCommandEvent& event)
{
}
void EXE::OnPath(wxCommandEvent& event)
{
}
void EXE::OnSearch(wxCommandEvent& event)
{
}
void EXE::OnDel(wxCommandEvent& event)
{
}




EXEScrollWND::EXEScrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id)
{}
void EXEScrollWND::newEXE() 
{
	EXE* h = new EXE(this, wxID_ANY, "EXE name", "EXE path");

	sizer->Add(h, 0, wxEXPAND, 2);
	this->SetSizer(sizer);
	this->FitInside();
	this->SetScrollRate(10, 10);
}
void EXEScrollWND::getEXEs() 
{
	size_t values; wxString valueNAME; long why{ 1 };
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs"); rk.Open();
	rk.GetKeyInfo(NULL, NULL, &values, NULL);
	rk.GetFirstValue(valueNAME, why);
		 
	for (size_t i = 0; i < values; i++)
	{
		wxString data; rk.QueryValue(valueNAME, data);
	
		EXE* h = new EXE(this, wxID_ANY, valueNAME, data);
		sizer->Add(h, 0, wxEXPAND, 2);
		this->SetSizer(sizer);
		this->FitInside();
		this->SetScrollRate(10, 10);
	
		rk.GetNextValue(valueNAME, why);
	}
}


EXEsFrame::EXEsFrame()
	: wxFrame(NULL, wxID_ANY, "Hello World")
{
	// get original EXE names
	{
		size_t valuews; wxString valueNAME; long why{ 1 };
		wxRegKey rkMAIN(wxRegKey::HKCU, "Software\\wxHKs"); rkMAIN.Open();
		rkMAIN.GetKeyInfo(NULL, NULL, &valuews, NULL);
		rkMAIN.GetFirstValue(valueNAME, why);

		for (size_t i = 0; i < valuews; i++)
		{
			// pair < original_VAL, VAL_to_change >
			pair<wxString, wxString> P{ valueNAME ,valueNAME };
			namePAIRS.push_back({ valueNAME, valueNAME  });
			rkMAIN.GetNextValue(valueNAME, why);
		}
	}

	// main
	{
		scrollwnd = new EXEScrollWND(this, wxID_ANY);
		MAINsizer = new wxBoxSizer(wxVERTICAL);

		MAINsizer->Add(scrollwnd, 1, wxEXPAND);

		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
		wxButton* New    = new wxButton(this, ID_newEXE, "New EXE");
		wxButton* cancel = new wxButton(this, ID_cancel, "cancel");
		wxButton* ok     = new wxButton(this, ID_ok, "ok");
		cancel->SetMinSize({ 100, cancel->GetSize().y });
		ok->SetMinSize({ 100, ok->GetSize().y });

		hbox->Add(New, 1, wxALL, 10);
		hbox->Add(cancel, 0, wxALL, 10);
		hbox->Add(ok, 0, wxALL, 10);

		MAINsizer->Add(hbox, 0, wxEXPAND | wxBOTTOM);
	}

	Bind(wxEVT_BUTTON, &EXEsFrame::OnNewEXE, this, ID_newEXE);
	Bind(wxEVT_BUTTON, &EXEsFrame::OnCancle, this, ID_cancel);
	Bind(wxEVT_BUTTON, &EXEsFrame::OnOK, this, ID_ok);

	this->scrollwnd->getEXEs();

	this->SetSize(600, 400);
	this->SetSizer(MAINsizer);
	this->Show();
}
EXEsFrame::~EXEsFrame()
{
	scrollwnd->Close();
	EXEs.clear();
	namePAIRS.clear();
}
void EXEsFrame::OnNewEXE(wxCommandEvent& event)
{
	scrollwnd->newEXE();
}
void EXEsFrame::OnCancle(wxCommandEvent& event)
{
	this->Close();
}

