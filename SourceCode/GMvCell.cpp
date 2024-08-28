//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("main.cpp", MainForm);
USEFORM("Settings.cpp", SForm);
USEFORM("Cell3Dview.cpp", Cell3D);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{ try
	{ Application->Initialize();
	  Application->MainFormOnTaskBar = true;
	  Application->Title = "GMmatrix";
	  Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TSForm), &SForm);
		Application->CreateForm(__classid(TCell3D), &Cell3D);
		Application->Run();
	}
  catch (Exception &exception)
	{ Application->ShowException(&exception); }
  catch (...)
	{ try
		{ throw Exception(""); 	}
		  catch (Exception &exception)
			{ Application->ShowException(&exception); }
		}
	return 0;
}
//---------------------------------------------------------------------------
