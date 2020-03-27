#include "wx/wxprec.h"
#include "trayICON.h"


TrayIcon::TrayIcon(wxFrame* _mainFrame)
{
	mainFrame = _mainFrame;
	SetIcon(wxICON(IDI_ICON));

	wxRegKey reg(wxRegKey::HKCU, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	if (reg.HasValue("wxHKs")) { check = true; }
	else{ check = false; }

	if (jmain.find(hideOnBoot) == jmain.end())
	{
		jmain[hideOnBoot] = true;
	}
	check = jmain[hideOnBoot];
}
TrayIcon::~TrayIcon()
{
	this->Destroy();
}

void TrayIcon::OnQuit(wxCommandEvent&)
{
	mainFrame->Destroy();
}
void TrayIcon::OnStartOnBoot(wxCommandEvent&)
{
	if (check)
	{
		wxMessageBox(
			"wxHKs will not start when your computer is turnd on.\n"
			"note that when this is activated, no window from this\n"
			"softwer will pop up when your computer starts."
		);

		wxRegKey reg(wxRegKey::HKCU, "c");
		if (reg.Exists() == false)
			wxMessageBox("canot start on startup");
		else 
			if (reg.HasValue("wxHKs"))
				reg.DeleteValue("wxHKs");

		saveToDisck(false);
	}
	else
	{
		wxMessageBox(
			"wxHKs will start working in the backgrownd\n"
			"when your computer is turnd on.\n"
		);

		wxRegKey reg(wxRegKey::HKCU, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		if (reg.Exists() == false)
			wxMessageBox("canot start on startup");
		else
			reg.SetValue("wxHKs", "\"" + thisPATH + "\" /MINIMIZED");

		saveToDisck(true);
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
		mainFrame->Iconize(false); 
		mainFrame->SetFocus();
		mainFrame->Raise();  
		mainFrame->Show(true);
	}
}





wxBEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
EVT_MENU(ID_QUIT, TrayIcon::OnQuit)
EVT_MENU(ID_START_ON_BOOT, TrayIcon::OnStartOnBoot)
EVT_UPDATE_UI(ID_START_ON_BOOT, TrayIcon::OnStartOnBootUI)
EVT_TASKBAR_LEFT_UP(TrayIcon::OnLeftClick)
wxEND_EVENT_TABLE()