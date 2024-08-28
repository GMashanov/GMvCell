//-----------------------------------------------------------------------
#ifndef Cell3DviewH
#define Cell3DviewH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TCell3D : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *Zoom;
	TMenuItem *Zoom1;
	TMenuItem *Zoom2;
	TMenuItem *Zoom3;
	TMenuItem *Tilt;
	TMenuItem *Swing;
	TMenuItem *SlicePosition;
	TMenuItem *Zoom6;
	TMenuItem *Zoom12;
	TMenuItem *SaveBMP;
	TSavePictureDialog *SaveBmpDlg;
	TMenuItem *SliceTilt;
	TMenuItem *VertShift;
	TMenuItem *BuildTube;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall Zoom1Click(TObject *Sender);
	void __fastcall Zoom2Click(TObject *Sender);
	void __fastcall Zoom3Click(TObject *Sender);
	void __fastcall TiltClick(TObject *Sender);
	void __fastcall SwingClick(TObject *Sender);
	void __fastcall SlicePositionClick(TObject *Sender);
	void __fastcall Zoom12Click(TObject *Sender);
	void __fastcall Zoom6Click(TObject *Sender);
	void __fastcall SaveBMPClick(TObject *Sender);
	void __fastcall SliceTiltClick(TObject *Sender);
	void __fastcall VertShiftClick(TObject *Sender);
	void __fastcall BuildTubeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TCell3D(TComponent* Owner);
void DrawObj(int x,int y, byte color);
void ObjectsToMatrix(void);
void DrawCell(byte *matrix);
void __fastcall DrawVoxel(int x,int y,byte colR,byte colG,byte colB);


 Graphics::TBitmap *Bmp;
 TPoint points[6];
 bool reDrawCell;
 int nmVox,zoom,oriX,oriY,oriZ,dX,dY,voxW,voxH,voxT,voxS,voxT2,voxTH,bmpX,bmpY;
 double tilt,swing,sliceTilt;
 __int64 matrixX,matrixY,matrixZ,matrixXY,faceY;
};
//---------------------------------------------------------------------------
extern PACKAGE TCell3D *Cell3D;
//---------------------------------------------------------------------------
#endif
