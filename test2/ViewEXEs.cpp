#include "wx/wxprec.h"
#include "ViewEXEs.h"


EXE::EXE(wxWindow* parent, wxWindowID id) : wxWindow(parent, id)
{
	size_t values; wxString valueNAME; long why{ 1 };
	wxRegKey rk(wxRegKey::HKCU, "Software\\wxHKs"); rk.Open();
	rk.GetKeyInfo(NULL, NULL, &values, NULL);
	rk.GetFirstValue(valueNAME, why);

	for (int i = 0; i < values; i++)
	{
		wxString data; rk.QueryValue(valueNAME, data);
		wxMessageBox(valueNAME + wxString(" ") + data, "value Name", wxOK);
		rk.GetNextValue(valueNAME, why);
	}

	pathCMB = new wxComboBox(this, ID_pathCMB + 2, "EXE path");
	vbox->Add(pathCMB, 1, wxCENTER | wxALL | wxEXPAND, 2);

	folderBTN = new wxButton(this, ID_folderBTN + 6, "6");
	delBTN = new wxButton(this, ID_delBTN + 7, "7");

	// buttons
	{
		folderBTN->SetMinSize({ folderBTN->GetSize().y, folderBTN->GetSize().y });
		delBTN->SetMinSize({ delBTN->GetSize().y, delBTN->GetSize().y });
		vbox->Add(folderBTN, 0, wxALL, 2);
		vbox->Add(delBTN, 0, wxALL, 2);
	}

	// bind events
	{
		Bind(wxEVT_CHECKBOX, &EXE::OnPathCMB, this, wxID_ANY);
		Bind(wxEVT_BUTTON, &EXE::OnFolderBTN, this, wxID_ANY);
		Bind(wxEVT_BUTTON, &EXE::OnDelBTN, this, wxID_ANY);
	}

	this->SetSizer(vbox);
	this->FitInside();

}
void EXE::OnPathCMB(wxCommandEvent& event)
{
	
}
void EXE::OnFolderBTN(wxCommandEvent& event)
{
}
void EXE::OnDelBTN(wxCommandEvent& event)
{
}



EXEScrollWND::EXEScrollWND(wxWindow* parent, wxWindowID id)
	: wxScrolledWindow(parent, id)
{}
void EXEScrollWND::newEXE() 
{
	EXE* h = new EXE(this, wxID_ANY);

	sizer->Add(h, 0, wxEXPAND, 2);
	this->SetSizer(sizer);
	this->FitInside();
	this->SetScrollRate(10, 10);
}
void EXEScrollWND::getEXEs() 
{

}


EXEsFrame::EXEsFrame()
	: wxFrame(NULL, wxID_ANY, "Hello World")
{
	Bind(wxEVT_BUTTON, &EXEsFrame::OnNewEXE, this, ID_newEXE);

	scrollwnd = new EXEScrollWND(this, wxID_ANY);
	MAINsizer = new wxBoxSizer(wxVERTICAL);

	MAINsizer->Add(scrollwnd, 1, wxEXPAND);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(
		new wxButton(this, ID_newEXE, "New EXE"),
		1, wxALL, 10
	);
	
	MAINsizer->Add(hbox, 0, wxEXPAND | wxBOTTOM);

	this->SetSizer(MAINsizer);
	this->Show();
}
void EXEsFrame::OnNewEXE(wxCommandEvent& event)
{
	scrollwnd->newEXE();
}
void EXEsFrame::OnDel(wxCommandEvent& event)
{
	Close(true);
}
