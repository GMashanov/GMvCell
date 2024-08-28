//---------------------------------------------------------------------------
// Copyright 2024 Gregory Mashanov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.//
//
//  GMvCell project, G. Mashanov, Crick Institute,2015-2024 (GMcellModel, 2007)
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "main.h"
#include "Cell3Dview.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCell3D *Cell3D;
//---------------------------------------------------------------------------
__fastcall TCell3D::TCell3D(TComponent* Owner)
	: TForm(Owner)
{ Bmp=new Graphics::TBitmap();   Width=800;Height=1000;  Left=20;Top=50;
  Bmp->Height=3000; Bmp->Width=5000; Bmp->PixelFormat=pf24bit;
  Bmp->Canvas->Brush->Style=bsSolid; Bmp->Canvas->Brush->Color=clBlack;
  Bmp->Canvas->FillRect(Rect(0,0,Bmp->Width,Bmp->Height));
  zoom=6; oriX=0,oriY=500,oriZ=500; tilt=0.7; swing=0.2;faceY=110;
  dX=1;dY=1; voxW=2, voxH=6; voxT=3,voxS=6; voxTH=9; voxT2=6; sliceTilt=0.0;
  nmVox=MainForm->nmVox; // very importnat
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::DrawVoxel(int x,int y,byte colR,byte colG,byte colB)
{ points[0]=Point(x,y);         // top romb
  points[1]=Point(x+voxS,y+voxT);
  points[2]=Point(x,y+voxT2);
  points[3]=Point(x-voxS,y+voxT);
  Bmp->Canvas->Brush->Color=(TColor)RGB(colR,colG,colB);
  Bmp->Canvas->Polygon(points,3);
  points[0]=Point(x,y+voxT2); // right romb
  points[1]=Point(x+voxS,y+voxT);
  points[2]=Point(x+voxS,y+voxTH);
  points[3]=Point(x,y+voxT+voxTH);
  Bmp->Canvas->Brush->Color=(TColor)RGB(colR-60,colG-60,colB-60);
  Bmp->Canvas->Polygon(points,3);
  points[0]=Point(x,y+voxT2); // left romb
  points[1]=Point(x,y+voxT+voxTH);
  points[2]=Point(x-voxS,y+voxTH);
  points[3]=Point(x-voxS,y+voxT);
  Bmp->Canvas->Brush->Color=(TColor)RGB(colR-30,colG-30,colB-30);
  Bmp->Canvas->Polygon(points,3);
}
//---------------------------------------------------------------------------
void TCell3D::DrawObj(int x,int y, byte color)
{ if ( x > bmpX ) bmpX=x;      // BMP sizes to save in file
  if ( y > bmpY ) bmpY=y;
  if ( color == 10 )   // CytoObject  Red
	{ Bmp->Canvas->Pen->Color=(TColor)RGB(50,0,0);
	  Bmp->Canvas->Brush->Color=(TColor)RGB(120,0,0);
	  Bmp->Canvas->Ellipse(x-zoom*0.7-1,y-zoom*0.7-1,x+zoom*0.7+1,y+zoom*0.7+1);
	  if ( zoom > 5 )
		{ Bmp->Canvas->Pen->Color=(TColor)RGB(150,0,0);
		  Bmp->Canvas->Brush->Color=(TColor)RGB(220,0,0);
		  Bmp->Canvas->Ellipse(x-zoom*0.6-1,y-zoom*0.6-1,x+zoom*0.4+1,y+zoom*0.4+1);
	  }
	  return;
	  //  Bmp->Canvas->Pen->Color=clYellow; Bmp->Canvas->Brush->Color=clWhite;
	  // Bmp->Canvas->Ellipse(x-zoom-1,y-zoom-1,x+zoom+1,y+zoom+1);
	  // return;
	}
  if ( color == 20 )    // Memb1 Object  Green  Color
	{ Bmp->Canvas->Pen->Color=(TColor)RGB(0,50,0);
	  Bmp->Canvas->Brush->Color=(TColor)RGB(0,120,0);
	  Bmp->Canvas->Ellipse(x-zoom*0.7-1,y-zoom*0.7-1,x+zoom*0.7+1,y+zoom*0.7+1);
	  if ( zoom > 5 )
		{ Bmp->Canvas->Pen->Color=(TColor)RGB(0,150,0);
		  Bmp->Canvas->Brush->Color=(TColor)RGB(0,220,0);
		  Bmp->Canvas->Ellipse(x-zoom*0.6-1,y-zoom*0.6-1,x+zoom*0.4+1,y+zoom*0.4+1);
		}
	  return;
	 /* if ( zoom > 5 )
		{ Bmp->Canvas->Pen->Color=(TColor)RGB(20,20,20); DrawVoxel(x,y,255,60,60); }
	  else
		{ Bmp->Canvas->Pen->Color=clRed; Bmp->Canvas->Brush->Color=clRed;
		  Bmp->Canvas->Ellipse(x-zoom-1,y-zoom-1,x+zoom+1,y+zoom+1);
		}
	  return; */
	}
  if ( color == 40 || color == 30 )    // StatObject Blue
	{ Bmp->Canvas->Pen->Color=(TColor)RGB(0,0,50);
	  Bmp->Canvas->Brush->Color=(TColor)RGB(0,0,120);
	  Bmp->Canvas->Ellipse(x-zoom*0.7-1,y-zoom*0.7-1,x+zoom*0.7+1,y+zoom*0.7+1);
	  if ( zoom > 5 )
		{ Bmp->Canvas->Pen->Color=(TColor)RGB(0,0,150);
		  Bmp->Canvas->Brush->Color=(TColor)RGB(0,0,220);
		  Bmp->Canvas->Ellipse(x-zoom*0.6-1,y-zoom*0.6-1,x+zoom*0.4+1,y+zoom*0.4+1);
	  }
	  return;
	}
 /* if ( color == 30 ) // Memb2 Object
	{ Bmp->Canvas->Pen->Color=clGreen; Bmp->Canvas->Brush->Color=clGreen;
	  Bmp->Canvas->Ellipse(x-zoom-1,y-zoom-1,x+zoom+1,y+zoom+1);
	  return;
	}
 */
  if ( zoom > 5 )
	{ Bmp->Canvas->Pen->Color=(TColor)RGB(20,20,20);
	  if ( color == 252 || color == 251 ) DrawVoxel(x,y,color+1,color+3,color+5);
	  else                DrawVoxel(x,y,color,color,color);
	}
  else
	Bmp->Canvas->Pixels[x][y]=(TColor)RGB(color,color,color);
}
//---------------------------------------------------------------------------
void TCell3D::ObjectsToMatrix(void)
{ __int64 x,y,z;
  byte *mat; mat=MainForm->matrix;
  if ( MainForm->ShowStaticObj->Checked )
	for ( int i=0; i < MainForm->statAnum; i++ )
		{ y=MainForm->StatA[i].y/nmVox;
		  if ( y > faceY ) continue;
		  x=MainForm->StatA[i].x/nmVox; z=MainForm->StatA[i].z/nmVox;
		  mat[x+(y*matrixX)+(z*matrixXY)]=STAT_VOXEL;
		}
  if ( MainForm->ShowMembObj->Checked )
	{ for ( int i=0; i < MainForm->membAnum; i++ )
		 { y=MainForm->MembA[i].y/nmVox;
		   if ( y > faceY ) continue;
		   x=MainForm->MembA[i].x/nmVox; z=MainForm->MembA[i].z/nmVox;
		   if ( mat[x+(y*matrixX)+(z*matrixXY)] < 200 ) continue; // no memb
		   mat[x+(y*matrixX)+(z*matrixXY)]=MainForm->MembA[i].medium-10;  // main cell membrane
		 }
	   for ( int i=0; i < MainForm->membBnum; i++ )
		 { y=MainForm->MembB[i].y/nmVox;
		   if ( y > faceY ) continue;
		   x=MainForm->MembB[i].x/nmVox; z=MainForm->MembB[i].z/nmVox;
		   if ( mat[x+(y*matrixX)+(z*matrixXY)] < 200 ) continue;  // no memb
		   mat[x+(y*matrixX)+(z*matrixXY)]=MainForm->MembB[i].medium-5;  // main cell membrane
		}
	 }
   if ( MainForm->ShowCytoObj->Checked )
	 for ( int i=0; i < MainForm->cytoAnum; i++ )
		{ y=MainForm->CytoA[i].y/nmVox;
		  if ( y > faceY ) continue;
		  x=MainForm->CytoA[i].x/nmVox; z=MainForm->CytoA[i].z/nmVox;
		  mat[x+(y*matrixX)+(z*matrixXY)]=MainForm->CytoA[i].medium+10;
		}
}
//---------------------------------------------------------------------------
void TCell3D::DrawCell(byte *matrix)
{ matrixX=MainForm->matrixX; matrixY=MainForm->matrixY; nmVox=MainForm->nmVox;
  matrixZ=MainForm->matrixZ; matrixXY=MainForm->matrixXY;
  Cursor=crHourGlass; // ShowMessage("Draw cell: Beginning");
  ObjectsToMatrix();
  Bmp->Canvas->Brush->Style=bsSolid; Bmp->Canvas->Brush->Color=clBlack;
  Bmp->Canvas->FillRect(Rect(0,0,Bmp->Width,Bmp->Height));
  Bmp->Canvas->Brush->Style=bsClear; Bmp->Canvas->Font->Color=clYellow;
  Bmp->Canvas->Pen->Color=(TColor)RGB(20,20,20);
  if ( faceY > matrixY ) faceY=matrixY;
  byte color; float cut=faceY;
  for ( __int64 z=0; z < matrixZ && cut > 10; z++,cut-=sliceTilt ) // scan cell body
	for ( __int64 y=0; y <= cut; y++ )
	  for ( __int64 x=0; x < matrixX; x++ )
		{ color=matrix[x+(y*matrixX)+(z*matrixXY)];
          if ( color == 50 ) color=150; /// for rafts
		  if ( color == EXTRACELL || color == CYTOPLASM ) continue; // empty voxel
		  if ( color == T_LUMEN || color == V_LUMEN || color == NUCLEOPLASM ) continue;
		  if ( color == STAT_VOXEL )  // Static Object
			{ MainForm->matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
			  DrawObj(oriX+(x+y*swing)*zoom,oriY+(y*tilt-z)*zoom*tilt,40);
			  continue;
			}
		  if ( color > 9 && color < 16 )  // Cyto1 Object
			{ matrix[x+(y*matrixX)+(z*matrixXY)]=color-10; // return to empty voxel
			  DrawObj((x+y*swing)*zoom,oriY+(y*tilt-z)*zoom*tilt,10);
			  continue;
			}
		  if ( color > 240 && color < 245 )  // Memb1 coding
			{ matrix[x+(y*matrixX)+(z*matrixXY)]=color+10; // restore value
			  DrawObj((x+y*swing)*zoom,oriY+(y*tilt-z)*zoom*tilt,20);
			  continue;
			}
		  if ( color > 245 && color < 250 )  // Memb2
			{ matrix[x+(y*matrixX)+(z*matrixXY)]=color+5; // restore value
			  DrawObj((x+y*swing)*zoom,oriY+(y*tilt-z)*zoom*tilt,30);
			  continue;
			}
		  if ( y == (int)cut && color != 150 ) color=204;  // cut surface
		  DrawObj(oriX+(x+y*swing)*zoom,oriY+(y*tilt-z)*zoom*tilt,color);  // membrane
		}
  Bmp->Canvas->Pen->Color=clWhite;  Bmp->Canvas->Font->Size=20;
  AnsiString str=MainForm->Round(MainForm->time,2); str+=" s";
  Bmp->Canvas->Brush->Color=clBlack; Bmp->Canvas->TextOut(5,5,str);
  Cursor=crDefault; // ShowMessage("Draw cell: end");
  reDrawCell=false;
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::FormPaint(TObject *Sender)
{ if ( MainForm->membOK && reDrawCell ) DrawCell(MainForm->matrix);
  Canvas->Draw(0,0,Bmp);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::Zoom1Click(TObject *Sender)
{ Zoom1->Checked=true; zoom=1; Zoom->Caption="Zoom(1)"; reDrawCell=true;
  voxW=2; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::Zoom2Click(TObject *Sender)
{ Zoom2->Checked=true; zoom=2; Zoom->Caption="Zoom(2)"; reDrawCell=true;
  voxW=4; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::Zoom3Click(TObject *Sender)
{ Zoom3->Checked=true; zoom=3; Zoom->Caption="Zoom(3)"; reDrawCell=true;
  voxW=6; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::TiltClick(TObject *Sender)
{ double buf=((int)(tilt*100.0))*0.01; UnicodeString str=buf;
  if ( !InputQuery("Image Tilt","Value (0.0-1.0)",str) ) return;
  buf=StrToFloatDef(str,0.5);
  if ( buf > 1.0 || buf < 0.0 )
	{ ShowMessage("Tilt is OUT of Order (0.0-1.0)"); return; }
  tilt=buf; dY=tilt*zoom+1;str="Tilt(";str+=buf; str+=")"; Tilt->Caption=str;
  reDrawCell=true; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::SwingClick(TObject *Sender)
{ double buf=((int)(swing*100.0))*0.01; UnicodeString str=buf;
  if ( !InputQuery("Image Swing","Value (0.0-1.0)",str) ) return;
  buf=StrToFloatDef(str,0.5);
  if ( buf > 1.0 || buf < 0.0 )
	{ ShowMessage("Swing is OUT of Order (0.0-1.0)"); return; }
  swing=buf; dX=swing*zoom+1;str="Swing(";str+=buf; str+=")"; Swing->Caption=str;
  reDrawCell=true; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::SlicePositionClick(TObject *Sender)
{ int buf=faceY*nmVox; UnicodeString str=buf;
  if ( !InputQuery("Slice Image at Y","Value in nm",str) ) return;
  buf=StrToIntDef(str,0.0);
  if ( buf > MainForm->matrixY*nmVox || buf < 10 )
	{ ShowMessage("Y value is OUT of Scope"); return; }
  faceY=buf/nmVox; str="Slice(";str+=buf; str+="nm)"; SlicePosition->Caption=str;
  reDrawCell=true; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::Zoom12Click(TObject *Sender)
{ Zoom12->Checked=true; zoom=12; Zoom->Caption="Zoom(12)"; reDrawCell=true;
  voxH=6; voxT=3,voxS=6; voxTH=9; voxT2=6; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::Zoom6Click(TObject *Sender)
{ Zoom6->Checked=true; zoom=6; Zoom->Caption="Zoom(6)"; reDrawCell=true;
  voxH=3; voxT=2,voxS=3; voxTH=5; voxT2=4; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::SaveBMPClick(TObject *Sender)
{ if ( !SaveBmpDlg->Execute() ) return;
  if ( bmpX < 4950 ) Bmp->Width=bmpX+50;
  else Bmp->Width=5000;
  if ( bmpY < 2950 ) Bmp->Height=bmpY+50;
  else Bmp->Height=3000;
  Bmp->SaveToFile(SaveBmpDlg->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::SliceTiltClick(TObject *Sender)
{ double buf=((int)(sliceTilt*100.0))*0.01; UnicodeString str=buf;
  if ( !InputQuery("Slice Tilt","Value (0.0-0.8)",str) ) return;
  buf=StrToFloatDef(str,0.5);
  if ( buf > 0.8 || buf < 0.0 )
	{ ShowMessage("SliceTilt is OUT of Order (0.0-0.8)"); return; }
  sliceTilt=buf; str="SliceTilt(";str+=buf; str+=")";
  SliceTilt->Caption=str; reDrawCell=true; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::VertShiftClick(TObject *Sender)
{ int buf=oriY; UnicodeString str=buf;
  if ( !InputQuery("VerticalShift","Value (pixels)",str) ) return;
  buf=StrToIntDef(str,0);
  if ( buf < 0 || buf > 3000 )
	{ ShowMessage("Shift is out of order"); return; }
  oriY=buf; str="VertShift(";str+=buf; str+=")";
  VertShift->Caption=str; reDrawCell=true; FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCell3D::BuildTubeClick(TObject *Sender)
{ BuildTube->Checked=!BuildTube->Checked;
  if ( BuildTube->Checked ) BuildTube->Caption="BuildTubeON";
  else                      BuildTube->Caption="BuildTubeOFF";
}
//---------------------------------------------------------------------------

