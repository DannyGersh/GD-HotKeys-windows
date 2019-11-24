#include "wx/wxprec.h"
#include "trayICON.h"



TrayIcon::TrayIcon(wxFrame* _mainFrame)
{
	mainFrame = _mainFrame;
	SetIcon(wxICON(IDI_ICON));

	wxRegKey reg(wxRegKey::HKCU, "Software\\wxHKs");
	wxString isCheck; reg.QueryValue("Start_on_boot", isCheck);
	if (isCheck == "true") { check = true; }
	else { check = false; }
}
TrayIcon::~TrayIcon()
{
	this->Destroy();
}

void TrayIcon::OnMenuExit(wxCommandEvent&)
{
	mainFrame->Close(true);
}
void TrayIcon::OnStartOnBoot(wxCommandEvent&)
{
	if (check)
	{
		wxRegKey reg(wxRegKey::HKCU, "Software\\wxHKs");
		reg.SetValue("Start_on_boot", "false");

		wxMessageBox(
			"wxHKs will not start when your computer is turnd on.\n"
			"note that when this is activated, no window from this\n"
			"softwer will pop up when your computer starts."
		);
	}
	else
	{
		wxRegKey reg(wxRegKey::HKCU, "Software\\wxHKs");
		reg.SetValue("Start_on_boot", "true");

		wxMessageBox(
			"wxHKs will start working in the backgrownd\n"
			"when your computer is turnd on.\n"
		);
	}
	check = !check;
}
void TrayIcon::OnStartOnBootUI(wxUpdateUIEvent &event)
{
	event.Check(check);
}

wxMenu *TrayIcon::CreatePopupMenu()
{
	wxMenu *menu = new wxMenu;
	menu->AppendCheckItem(ID_START_ON_BOOT, "Start on boot");
	menu->AppendSeparator();
	menu->Append(ID_QUIT, "Quit");

	return menu;
}

void TrayIcon::OnLeftClick(wxTaskBarIconEvent&)
{
	if (mainFrame->IsVisible())
	{
		mainFrame->Show(false);
	}
	else
	{
		mainFrame->Show(true);
	}
}





wxBEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
EVT_MENU(ID_QUIT, TrayIcon::OnMenuExit)
EVT_MENU(ID_START_ON_BOOT, TrayIcon::OnStartOnBoot)
EVT_UPDATE_UI(ID_START_ON_BOOT, TrayIcon::OnStartOnBootUI)
EVT_TASKBAR_LEFT_UP(TrayIcon::OnLeftClick)
wxEND_EVENT_TABLE()