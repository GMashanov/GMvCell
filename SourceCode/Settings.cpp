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
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Settings.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSForm *SForm;
//---------------------------------------------------------------------------
__fastcall TSForm::TSForm(TComponent* Owner)
        : TForm(Owner)
{ CellImage->Width=600;  CellImage->Height=400;
  StructureImage->Width=600;  StructureImage->Height=400;
  cond.tmInterMs=cond.framesToSum=10;
  CellImage->Canvas->Brush->Color=clBtnFace;
  GaussImage->Canvas->Brush->Color=clBtnFace;
  cond.cellXnm=30000; cond.cellYnm=10000; cond.cellZnm=3000;cond.voxelSize=5;
  DrawCell(); DrawGauss(); Statistics();
}
//---------------------------------------------------------------------------
double __fastcall TSForm::CalcPlen(void)
{ double dX1=0.0,dX2=0.0,dY1=0.0,dY2=0.0,dZ1=0.0,dZ2=0.0,azi=0.0,ele=0.0,m1,m2;
  double avCos=0.0,len=cond.voxelSize,pLen=0.0;  int con=0;
  for ( int i=0; i < 10000; i++ )
	{ dX1=dX2;dY1=dY2;dX2=COS(azi)*COS(ele);dY2=SIN(azi)*COS(ele);
	  azi+=RandOne()*cond.bendTilt;
	  if ( cond.tube3D )  // meandring in Z
		{ dZ1=dZ2; dZ2=SIN(ele); ele+=RandOne()*cond.bendTilt; }
	  m1=sqrt(dX1*dX1+dY1*dY1+dZ1*dZ1); m2=sqrt(dX2*dX2+dY2*dY2+dZ2*dZ2);
	  if (m1 > 0.0 && m2 > 0.0 )  // ang=acos(a*b / |a|*|b|)
		avCos+=cos(acos((dX1*dX2+dY1*dY2+dZ1*dZ2)/(m1*m2)));//)/(PI/180.0);
	  con++;
	}
  if ( con == 0 ) pLen=0.0;
  else pLen=-len/log(avCos/(double(con)));// COS(avAng));
  AnsiString str="Estimated Plen (nm)="; str+=(int)(pLen*10.0)*0.1;
  PlenLab->Caption=str;
  return 0.0;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::SetParam(void)
{ TimeIntervalEdit->Text=cond.tmInterMs;
  LengthEdit->Text=cond.cellXnm*0.001; HeightEdit->Text=cond.cellZnm*0.001;
  WidthEdit->Text=cond.cellYnm*0.001;   InteractionDistanceEdit->Text=cond.IDum;
  NucleusRadEdit->Text=cond.nucleusRadNm; TubeZedit->Text=cond.tubeZnm;
  VoxelSizeEdit->Text=cond.voxelSize;	 RaftSizeEdit->Text=cond.raftRadNm;
  MemorySizeEdit->Text=cond.memorySizeMb; FlatteningEdit->Text=cond.flattening;
  TubeRadEdit->Text=cond.tubeRadNm;   FiberExtentionEdit->Text=cond.actinBundleExtendNm;
  RaftsChBox->Checked=cond.rafts; Tube3Dbox->Checked=cond.tube3D;
  RaftMobRatioEdit->Text=cond.raftMobRatio; RaftNumEdit->Text=cond.raftNum;
  NucleusChBox->Checked=cond.nucleus; TubeNetworkChBox->Checked=cond.tubeNetwork;
  LamellaChBox->Checked=cond.lamella;    FilopodiaChBox->Checked=cond.filopodia;
  MovingVesChBox->Checked=cond.movingVesicles; VesMobEdit->Text=cond.vesicleMob;
  TubeLengthEdit->Text=cond.tubeLengthNm;  TubeDistEdit->Text=cond.interTubeDistNm;
  TubesChBox->Checked=cond.tubes; SideGapEdit->Text=cond.sideGapNm*0.001;
  BottomGapEdit->Text=cond.botGapNm*0.001;  TopGapEdit->Text=cond.topGapNm*0.001;
  PoreRadiusEdit->Text=cond.poreRadNm;  VesTiltEdit->Text=cond.bendTilt;
  BodyFilletEdit->Text=cond.bodyFilletNm; OutsideChBox->Checked=cond.outsideVesicles;
  RoundedCellBodyChBox->Checked=cond.roundedCellBody; RecycleChBox->Checked=cond.recycling;
  MitoChBox->Checked=cond.mitochondria; MonoPerFiloEdit->Text=cond.monoPerFilo;
  VesRadEdit->Text=cond.vesRadNm; VesNumEdit->Text=cond.vesNum;
  FuseVesiclesChBox->Checked=cond.fuseVes; TransferChBox->Checked=MainForm->transfer;
  double vKdiff=KbT/(6.0*PI*cond.vesRadNm*0.001);
  AnsiString str="Estim. D="; str+=Rounding(vKdiff,4); str+="µm2/s";
  VesiKdiffLab->Caption=str;
  // Illumination / Imaging Props
  RedBleachRateEdit->Text=cond.redBleachRate; GreenBleachRateEdit->Text=cond.greenBleachRate;
  GreenEmissionRateEdit->Text=cond.greenPhotonsPerSec; RedEmissionRateEdit->Text=cond.redPhotonsPerSec;
  SpotSizeEdit->Text=cond.FWHM; EMCCD_ChBox->Checked=cond.EMCCD; TIRangleEdit->Text=cond.TIRangle;
  KobjEdit->Text=cond.Kobj;  FramesToSumEdit->Text=cond.framesToSum;
  FramesToSumEdit->Text=cond.framesToSum; DarkFramesEdit->Text=cond.numOfDarkFrames;
  CameraNoiseEdit->Text=cond.noise;  GainEdit->Text=cond.gain;
  FRETchBox->Checked=MainForm->FRET;
  TwoCamChBox->Checked=cond.twoCam; AlterChBox->Checked=cond.alterIllum;
  BlinkOnEdit->Text=cond.greenBlinkONrate; BlinkOffEdit->Text=cond.greenBlinkOFFrate;
  IllumFWHMEdit->Text=cond.illumFWHMum;  IrisRadiusEdit->Text=cond.irisRadUm;

  // Cyto A prop
  CytoAnumEdit->Text=cond.cytoAnum; CytoAinitBox->ItemIndex=cond.cytoAinit;
  GreenCytoAnumEdit->Text=cond.greenCytoAnum; RedCytoAnumEdit->Text=cond.redCytoAnum;
  GreenCytoAfractionEdit->Text=cond.greenCytoAfraction;
  RedCytoAfractionEdit->Text=cond.redCytoAfraction;
  CytoAmobEdit->Text=cond.cytoAmob;
  CytoAcanBindMembA_ChBox->Checked=cond.cytoAcanBindMembA;
  CytoAcanBindMembB_ChBox->Checked=cond.cytoAcanBindMembB;
  CytoAstepSizeEdit->Text=cond.cytoAstepSizeNm;
  CytoAstepRateEdit->Text=cond.cytoAstepRate;
  CytoApolimerChBox->Checked=cond.cytoApoli;
  CytoApolimerEdit->Enabled=cond.cytoApoli;
  CytoApolimerEdit->Text=cond.cytoApoliRate;
  CytoAdepoliEdit->Text=cond.cytoAdepoliRate;

  // Cyto B prop
  CytoBnumEdit->Text=cond.cytoBnum; CytoBmobEdit->Text=cond.cytoBmob;
  CytoBinitBox->ItemIndex=cond.cytoBinit;
  GreenCytoBnumEdit->Text=cond.greenCytoBnum;RedCytoBnumEdit->Text=cond.redCytoBnum;
  GreenCytoBfractionEdit->Text=cond.greenCytoBfraction;
  RedCytoBfractionEdit->Text=cond.redCytoBfraction;
  CytoAcanBindCytoB_ChBox->Checked=cond.cytoAcanBindCytoB;
  CytoAcytoBbindRateEdit->Text=cond.cytoAcytoBbindRate;
  CytoAcytoBdissRateEdit->Text=cond.cytoAcytoBdissRate;
  CytoBdistAedit->Text=cond.cytoBdistA;
  // Memb A prop
  MembAnumEdit->Text=cond.membAnum;  OneColourChBox->Checked=cond.oneColourAA;
  GreenMembAnumEdit->Text=cond.greenMembAnum;  RedMembAnumEdit->Text=cond.redMembAnum;
  GreenMembAfractionEdit->Text=cond.greenMembAfraction;
  RedMembAfractionEdit->Text=cond.redMembAfraction;
  MembAmobEdit->Text=cond.membAmob; CytoAmembAbindRateEdit->Text=cond.cytoAmembAbindRate;
  CytoAmembAdissRateEdit->Text=cond.cytoAmembAdissRate;
  AAdissRateEdit->Text=cond.membAdepoliRate; AAbindingRateEdit->Text=cond.membApoliRate;
  MembAinitBox->ItemIndex=cond.membAinit; MembApoliNumEdit->Text=cond.membApoliNum;
  CytoAcanBindMembA_ChBox->Checked=cond.cytoAcanBindMembA;
  MembApolimerChBox->Checked=cond.membApoli;
  MoveToNucleusChBox->Checked=cond.membAorganPump;
 // Memb B prop
  MembBnumEdit->Text=cond.membBnum;  MembBmobEdit->Text=cond.membBmob;
  GreenMembBnumEdit->Text=cond.greenMembBnum;  RedMembBnumEdit->Text=cond.redMembBnum;
  GreenMembBfractionEdit->Text=cond.greenMembBfraction;
  RedMembBfractionEdit->Text=cond.redMembBfraction;
  CytoAmembBbindRateEdit->Text=cond.cytoAmembBbindRate;
  CytoAmembBdissRateEdit->Text=cond.cytoAmembBdissRate;
  SiteABbindingRateEdit->Text=cond.membAmembBbindRate;
  MembBinitBox->ItemIndex=cond.membBinit;
  AfromBdissRateEdit->Text=cond.membAmembBdissRate;
  MembAcanBindMembB_ChBox->Checked=cond.membAcanBindMembB;
  MembBmoveCytoAcyto->Checked=cond.membBcytoPump;
  // Memb C prop
  MembCnumEdit->Text=cond.membCnum; MembCmobEdit->Text=cond.membCmob;
  GreenMembCnumEdit->Text=cond.greenMembCnum; RedMembCnumEdit->Text=cond.redMembCnum;
  GreenMembCfractionEdit->Text=cond.greenMembCfraction;
  RedMembCfractionEdit->Text=cond.redMembCfraction;
  MembBCbindingRateEdit->Text=cond.membBmembCbindRate;
  MembBCdissRateEdit->Text=cond.membBmembCdissRate;
  MembBcanBindMembC_ChBox->Checked=cond.membBcanBindMembC;
  MembCinitBox->ItemIndex=cond.membCinit;
  // Stat A prop
  StatAnumEdit->Text=cond.statAnum; StatAinitBox->ItemIndex=cond.statAinit;
  GreenStatAnumEdit->Text=cond.greenStatAnum;
  GreenStatAfractionEdit->Text=cond.greenStatAfrac;
  RedStatAnumEdit->Text=cond.redStatAnum;
  RedStatAfractionEdit->Text=cond.redStatAfrac;
  CytoAstatAbindRateEdit->Text=cond.cytoAstatAbindRate;
  CytoAstatAdissRateEdit->Text=cond.cytoAstatAdissRate;
  FilamentPitchStepEdit->Text=cond.filamentPitchStepNm;
  CytoAcanMoveAlongStatA->Checked=cond.cytoAcanMoveAlongStatA;
  CytoAcanBindStatAchBox->Checked=cond.cytoAcanBindStatA;
  ClusterNumEdit->Text=cond.membAclusterNum;
  ReleaseRateEdit->Text=MainForm->moleculeReleaseRate; ////!!!!!!!!!!!
  MonomersInFilamentEdit->Text=cond.monomersPerFilament;

  DrawCell(); DrawGauss(); CalcPlen();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::OK_ButtonClick(TObject *Sender)
{  Close(); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CancelButtonClick(TObject *Sender)
{ cond.cancel=true; Close(); }
//---------------------------------------------------------------------------
void TSForm::Line(TCanvas *cnv,int x1,int y1,int x2,int y2)
{ cnv->MoveTo(x1,y1); cnv->LineTo(x2,y2); }
//---------------------------------------------------------------------------
void __fastcall TSForm::DrawCell(void)
{ int left=30,top=80,hgt=90,wdt=70,len=220,lamLen=200,lamHgt=20;
  if ( !LamellaChBox->Checked ) lamLen=lamHgt=0; // no lamella
  CellImage->Canvas->FillRect(Rect(0,0,CellImage->Width,CellImage->Height));
  CellImage->Canvas->Pen->Width=3; CellImage->Canvas->Pen->Color=clGray;
  CellImage->Canvas->Brush->Color=clBtnFace;
  CellImage->Canvas->Ellipse(Rect(0,120,CellImage->Width,300)); // coverslip
  CellImage->Canvas->TextOut(270,270,"Coverslip");
  CellImage->Canvas->Pen->Color=clBlue;
  // back lines
  Line(CellImage->Canvas,left+wdt,top,left+wdt,top+hgt); //  vert back
  CellImage->Canvas->LineTo(left,top+hgt+wdt);     // diag low left
  Line(CellImage->Canvas,left+wdt,top+hgt,left+wdt+len+lamLen,top+hgt); // back horiz
  CellImage->Canvas->Pen->Color=clGray;
  if ( RaftsChBox->Checked )
	for ( int y=0; y < wdt; y+=10 )
	  for ( int x=0; x < len; x+=20 )
		for ( int i=10; i < 20; i++ )
		  Line(CellImage->Canvas,left+wdt+i+x-y,top+hgt+y,left+wdt+i+x-y-5,top+hgt+y+5);
  CellImage->Canvas->Brush->Color=clBlack;
  if ( NucleusChBox->Checked )
	CellImage->Canvas->Ellipse(Rect(left+70,top+hgt-60,left+200,top+hgt+40));
  CellImage->Canvas->Brush->Color=clBtnFace;
  CellImage->Canvas->Pen->Color=clBlue;
  if ( FilopodiaChBox->Checked )
	for ( int y=5; y < wdt; y+=25 )
	  { CellImage->Canvas->Ellipse(Rect(left+len+wdt-y,top+70+y,left+len+wdt-10-y,top+90+y));
		Line(CellImage->Canvas,left+len+wdt-y-5,top+70+y,left+len+wdt+195-y,top+70+y);
		Line(CellImage->Canvas,left+len+wdt-y-5,top+90+y,left+len+wdt+195-y,top+90+y);
		CellImage->Canvas->Ellipse(Rect(left+len+wdt-y+200,top+70+y,left+len+wdt+190-y,top+90+y));
	  }
  if ( TubeNetworkChBox->Checked )
	{ for ( int i=10,x1,x2,y1,y2; i < wdt; i+=25 )
		{ x1=left+wdt-i+5; x2=left+len+wdt-i-10; y1=top+70+i; y2=y1+20;
		  CellImage->Canvas->Ellipse(Rect(x1-5,y1,x1+5,y2));
		  Line(CellImage->Canvas,x1,y1,x2,y1); Line(CellImage->Canvas,x1,y2,x2,y2);
		  CellImage->Canvas->Ellipse(Rect(x2-5,y1,x2+5,y2));
		}
	  for ( int i=35,x1,x2,y1,y2; i < len; i+=50 )
		{ x1=left+i; x2=left+wdt+i; y1=top+hgt+40; y2=top+60;
		  CellImage->Canvas->Ellipse(Rect(x1-10,y1,x1+10,y1+20));
		  CellImage->Canvas->Ellipse(Rect(x2-10,y2,x2+10,y2+20));
		  Line(CellImage->Canvas,x1,y1,x2,y2);
		  Line(CellImage->Canvas,x1,y1+20,x2,y2+20);
		}
	}
  if ( TubesChBox->Checked )
	for ( int i=30,x1,x2,y1,y2; i < wdt; i+=25 )
	  { x1=left+wdt-i+50; x2=left+len+wdt-i-50; y1=top+70+i; y2=y1+20;
		CellImage->Canvas->Ellipse(Rect(x1-5,y1,x1+5,y2));
		Line(CellImage->Canvas,x1,y1,x2,y1); Line(CellImage->Canvas,x1,y2,x2,y2);
		CellImage->Canvas->Ellipse(Rect(x2-5,y1,x2+5,y2));
	  }
   CellImage->Canvas->Brush->Color=clBlack;
  if ( NucleusChBox->Checked )
	CellImage->Canvas->Ellipse(Rect(left+70,top+hgt-60,left+200,top+hgt+40));
  CellImage->Canvas->Brush->Color=clBtnFace;

  CellImage->Canvas->Pen->Width=5;
  CellImage->Canvas->Font->Size=12;  CellImage->Canvas->Font->Color=clBlack;
  CellImage->Canvas->MoveTo(left,top+wdt);
  CellImage->Canvas->LineTo(left+wdt,top);  // diag left
  CellImage->Canvas->LineTo(left+len+wdt,top); // horiz top
  CellImage->Canvas->LineTo(left+len,top+wdt); // diag right
  CellImage->Canvas->LineTo(left,top+wdt); //  horiz front
  CellImage->Canvas->LineTo(left,top+hgt+wdt); // vert down
  if ( !LamellaChBox->Checked )  // No Lamella
	{ CellImage->Canvas->LineTo(left+len,top+hgt+wdt); // horiz front down
	  CellImage->Canvas->LineTo(left+len+wdt,top+hgt); // low right diag
	  CellImage->Canvas->LineTo(left+len+wdt,top); // vert to top
	  CellImage->Canvas->LineTo(left+len,top+wdt);
	  CellImage->Canvas->LineTo(left+len,top+hgt+wdt);
	}
  else   // Draw Lamella
	{ CellImage->Canvas->LineTo(left+len+lamLen,top+hgt+wdt); // horiz front down
	  CellImage->Canvas->LineTo(left+len+wdt+lamLen,top+hgt); // low right diag
	  CellImage->Canvas->LineTo(left+len+wdt+lamLen,top+hgt-lamHgt); // vert to top
	  CellImage->Canvas->LineTo(left+len+wdt,top+hgt-lamHgt);  // horiz top rear to body
	  CellImage->Canvas->LineTo(left+len+wdt,top);
	  Line(CellImage->Canvas,left+len,top+wdt,left+len,top+hgt+wdt-lamHgt); //  vert front right
	  CellImage->Canvas->LineTo(left+len+lamLen,top+hgt+wdt-lamHgt);
	  CellImage->Canvas->LineTo(left+len+lamLen,top+hgt+wdt); // lam front vert
	  Line(CellImage->Canvas,left+len+wdt+lamLen,top+hgt-lamHgt,left+len+lamLen,top+hgt+wdt-lamHgt);
	  Line(CellImage->Canvas,left+len+wdt,top+hgt-lamHgt,left+len,top+hgt+wdt-lamHgt);
	}
  CellImage->Canvas->TextOut(left-20,top+90,"Z");
  CellImage->Canvas->TextOut(left+7,top+5,"Y");
  CellImage->Canvas->TextOut(left+160,top-30,"X");
  int width=600,height=400;
  StructureImage->Canvas->CopyRect(Rect(0,0,600,400),CellImage->Canvas,
								   Rect(0,0,600,400));
}
//---------------------------------------------------------------------------
void __fastcall TSForm::Statistics(void)
{ double height=cond.cellZnm,width=cond.cellYnm,len=cond.cellXnm,numA=cond.cytoAnum;
  double cellVolume=(len*width*height)*1E-24,numB=cond.cytoBnum;
  double cellSurface=(len*width*2.0)+(len*height*2.0)+(height*width*2.0);
  if ( cond.roundedCellBody )  // shpere A=4*pi*r*r
	{ double rad=cond.bodyFilletNm; double diam=rad*2.0,lenC=len-rad-rad;
	  cellSurface-=diam*diam*6.0; cellSurface-=diam*lenC*4.0; // exclude edges ball and cylinder
	  cellSurface+=4.0*PI*rad*rad; cellSurface+=lenC*PI*diam;
	}
  double consA=((numA*1E9)/avogadro)/cellVolume,consB=((numB*1E9)/avogadro)/cellVolume;
  double membAdens=cond.membAnum/(cellSurface*1E-6);
  double membBdens=cond.membBnum/(cellSurface*1E-6);
  double membCdens=cond.membCnum/(cellSurface*1E-6);
  double protJumpNm=sqrt(6.0*cond.cytoAmob*cond.tmInterMs*0.001)*1000.0;
  double siteAjumpNm=sqrt(6.0*cond.membAmob*cond.tmInterMs*0.001)*1000.0;
  double siteBjumpNm=sqrt(6.0*cond.membBmob*cond.tmInterMs*0.001)*1000.0;
  AnsiString str="CytoA Consentr.="; str+=Rounding(consA,3); str+="nM";
  ConcentrationLab->Caption=str;
  str+=", "; str+=Rounding(numA/(cellVolume*1E15),3); str+=" molecules/µm3";
  CytoAconsLab->Caption=str;
  str="CytoB Consentr.="; str+=Rounding(consB,3); str+="nM";
  str+=", "; str+=Rounding(numB/(cellVolume*1E15),3); str+=" molecules/µm3";
  CytoBconcLab->Caption=str;
  str="Cell Volume="; str+=cellVolume*1E15; str+="µm3"; VolumeLab->Caption=str;
  str="Surface Area="; str+=Rounding(cellSurface*1E-6,3); str+="µm2";
  SurfaceLab->Caption=str;
  str="Average MembA Density="; str+=Rounding(membAdens,3); str+="µm-2";
  SiteAlab->Caption=str; SiteAdensLab->Caption=str;
  MainForm->membAdens=Rounding(membAdens,3);
  str="Average MembB Density="; str+=Rounding(membBdens,3); str+="µm-2";
  SiteBlab->Caption=str;   SiteBdensLab->Caption=str;
  str="Average MembC Density="; str+=Rounding(membCdens,3); str+="µm-2";
  SiteCdensLab->Caption=str;
  str="Aver. Jump (RMSD) per TmStep="; str+=Rounding(protJumpNm,2); str+="nm";
  CytoAjumpLab->Caption=str;
  str="Aver. Jump (RMSD) per TmStep="; str+=Rounding(siteAjumpNm,3); str+="nm";
  SiteAjumpLab->Caption=str;
  str="Aver. Jump (RMSD) per TmStep="; str+=Rounding(siteBjumpNm,3); str+="nm";
  SiteBjumpLab->Caption=str;
  /// Calculate interaction time Time Step 0.1 ms
  double rad=cond.IDum*0.5; //!!!!!!!!
  double IAum2=PI*(rad*rad),IVum3=(4.0/3.0)*PI*(rad*rad*rad);
  double tm=cond.tmInterMs*0.001,timeUs,mobProtSiteA=cond.cytoAmob+cond.membAmob;
  timeUs=MainForm->AIT2D(0.005,cond.membAmob*2.0,tm,IAum2)*1e6;
  str="Dist 5nm="; str+=((int)(timeUs*10.0))*0.1; str+="us, 10nm=";
  timeUs=MainForm->AIT2D(0.01,cond.membAmob*2.0,tm,IAum2)*1e6;
  str+=((int)(timeUs*10.0))*0.1; str+="us, 50nm=";
  timeUs=MainForm->AIT2D(0.05,cond.membAmob*2.0,tm,IAum2)*1e6;
  str+=((int)(timeUs*10.0))*0.1; str+="us, 100nm=";
  timeUs=MainForm->AIT2D(0.1,cond.membAmob*2.0,tm,IAum2)*1e6;
  str+=((int)(timeUs*10.0))*0.1; str+="us, 200nm=";
  timeUs=MainForm->AIT2D(0.2,cond.membAmob*2.0,tm,IAum2)*1e6;
  str+=((int)(timeUs*10.0))*0.1; str+="us, 500nm=";
  timeUs=MainForm->AIT2D(0.5,cond.membAmob*2.0,tm,IAum2)*1e6;
  str+=((int)(timeUs*10.0))*0.1; str+="us";
 // SiteAAtimeLab->Caption=str;
  timeUs=MainForm->AIT3D(0.005,mobProtSiteA,tm,IVum3)*1e6;
  str="Dist 5nm="; str+=((int)(timeUs*1000.0))*0.001; str+="us, 10nm=";
  timeUs=MainForm->AIT3D(0.01,mobProtSiteA,tm,IVum3)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 50nm=";
  timeUs=MainForm->AIT3D(0.05,mobProtSiteA,tm,IVum3)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 100nm=";
  timeUs=MainForm->AIT3D(0.1,mobProtSiteA,tm,IVum3)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 200nm=";
  timeUs=MainForm->AIT3D(0.2,mobProtSiteA,tm,IVum3)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 500nm=";
  timeUs=MainForm->AIT3D(0.5,mobProtSiteA,tm,IVum3)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us";
//  ProtSiteAtimeLab->Caption=str;
  double mobAB=cond.membAmob+cond.membBmob;
  timeUs=MainForm->AIT2D(0.005,mobAB,tm,IAum2)*1e6;
  str="Dist 5nm="; str+=((int)(timeUs*1000.0))*0.001; str+="us, 10nm=";
  timeUs=MainForm->AIT2D(0.01,mobAB,tm,IAum2)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 50nm=";
  timeUs=MainForm->AIT2D(0.05,mobAB,tm,IAum2)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 100nm=";
  timeUs=MainForm->AIT2D(0.1,mobAB,tm,IAum2)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 200nm=";
  timeUs=MainForm->AIT2D(0.2,mobAB,tm,IAum2)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us, 500nm=";
  timeUs=MainForm->AIT2D(0.5,mobAB,tm,IAum2)*1e6;
  str+=((int)(timeUs*1000.0))*0.001; str+="us";
 // SiteABtimeLab->Caption=str;
  int fps=100000.0/(cond.tmInterMs*cond.framesToSum);
  str=((float)fps)*0.01; str+="fps";
  fpsLab->Caption=str; fps2Lab->Caption=str;
  CheckSize(cond.cellXnm,cond.cellYnm,cond.cellZnm,cond.sideGapNm,cond.botGapNm,cond.topGapNm);
}
//---------------------------------------------------------------------------
// Cell settings
//---------------------------------------------------------------------------
bool TSForm::CheckSize(int x, int y, int z, int side, int bot, int top)
{ int scale=cond.voxelSize; UnicodeString str= "Matrix is Too BIG (";
  __int64 xx=(x/scale)+(side*2)/scale;
  __int64 yy=(y/scale)+(side*2)/scale;
  __int64 zz=(z/scale)+(bot/scale)+(top/scale);
  if ( cond.lamella || cond.filopodia )
    xx+=cond.tubeLengthNm/scale;
  matrixSize=xx*yy*zz;
  if ( matrixSize >= (__int64)MainForm->memorySizeMb*1000000 )
	{ str+=(matrixSize*0.000001); str+="Mb) => Reduce to ";
	  str+=MainForm->memorySizeMb;str+="Mb"; ShowMessage(str); return false;
	}
  str="Matrix Size "; str+=int(matrixSize*0.000001); str+="Mb (out of ";
  str+=MainForm->memorySizeMb; str+="Mb)";
  MatrixLab->Caption=str;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::LengthEditExit(TObject *Sender)
{ int buf=StrToFloatDef(LengthEdit->Text,-1.0)*1000.0;
  if ( !CheckSize(buf,cond.cellYnm,cond.cellZnm,cond.sideGapNm,cond.botGapNm,cond.topGapNm) )
   { LengthEdit->Text=cond.cellXnm*0.001; return; }
  if ( buf < cond.voxelSize*10 )
   { ShowMessage("Cell Length is too small");LengthEdit->Text=cond.cellXnm*0.001;return; }
  LengthEdit->Text=buf*0.001; cond.cellXnm=buf; DrawGauss(); Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::LengthEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) LengthEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::WidthEditExit(TObject *Sender)
{ int buf=StrToFloatDef(WidthEdit->Text,-1.0)*1000.0;
  if ( !CheckSize(cond.cellXnm,buf,cond.cellZnm,cond.sideGapNm,cond.botGapNm,cond.topGapNm) )
	{ WidthEdit->Text=cond.cellYnm*0.001; return; }
  if ( buf < cond.voxelSize*10 )
	{ ShowMessage("Cell Width is too small");WidthEdit->Text=cond.cellYnm*0.001;return; }
  WidthEdit->Text=buf*0.001; cond.cellYnm=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::WidthEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) WidthEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::HeightEditExit(TObject *Sender)
{ int buf=StrToFloatDef(HeightEdit->Text,-1.0)*1000.0;
  if ( !CheckSize(cond.cellXnm,cond.cellYnm,buf,cond.sideGapNm,cond.botGapNm,cond.topGapNm) )
    { HeightEdit->Text=cond.cellZnm*0.001;return; }
  if ( buf < cond.voxelSize*10 )
	{ ShowMessage("Cell Height is too small");HeightEdit->Text=cond.cellZnm*0.001;return; }
  HeightEdit->Text=buf*0.001; cond.cellZnm=buf;Statistics();DrawGauss();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::HeightEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) HeightEditExit(Sender);  }
//---------------------------------------------------------------------------
void __fastcall TSForm::NucleusChBoxClick(TObject *Sender)
{ cond.nucleus=NucleusChBox->Checked;
  NucleusRadEdit->Enabled=NucleusChBox->Checked;
  FlatteningEdit->Enabled=NucleusChBox->Checked;
  DrawCell();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::InteractionDistanceEditExit(TObject *Sender)
{ double buf=StrToFloatDef(InteractionDistanceEdit->Text,-1.0);
  if ( buf < 0.001 || buf > 0.1 )
    { ShowMessage("Distance Out of Range (0.001-0.1 um)");
      InteractionDistanceEdit->Text=cond.IDum; return;}
  cond.IDum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::InteractionDistanceEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) InteractionDistanceEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RaftsChBoxClick(TObject *Sender)
{ cond.rafts=RaftsChBox->Checked;  DrawCell();
  RaftSizeEdit->Enabled=cond.rafts; RaftNumEdit->Enabled=cond.rafts;
  RaftMobRatioEdit->Enabled=cond.rafts;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RaftSizeEditExit(TObject *Sender)
{ int buf=StrToIntDef(RaftSizeEdit->Text,-1);
  if ( buf < 10 || buf > 10000 )
	{ ShowMessage("Raft Radius should be between 10 and 10000 nm");
	  RaftSizeEdit->Text=cond.raftRadNm; return;
    }
  cond.raftRadNm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RaftSizeEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) RaftSizeEditExit(Sender);}
//---------------------------------------------------------------------------
void __fastcall TSForm::TimeIntervalEditExit(TObject *Sender)
{ double buf=StrToFloatDef(TimeIntervalEdit->Text,-1.0);
  if ( buf < 0.01 || buf > 1000.0 )
    { ShowMessage("Wrong Time Interval");TimeIntervalEdit->Text=cond.tmInterMs;return; }
  cond.tmInterMs=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TimeIntervalEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) TimeIntervalEditExit(Sender);  }
//---------------------------------------------------------------------------
// Protein settings
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(CytoAnumEdit->Text,-1);
  if ( buf < 0 || buf > CYTO_NUM )
	{ ShowMessage("Wrong number of CytoA molecules (0-50000)");
	  CytoAnumEdit->Text=cond.cytoAnum;return;
	}
  cond.cytoAnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAnumEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) CytoAnumEditExit(Sender);  }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenCytoAnumEdit->Text,-1);
  if ( buf < 0 || buf > 8 )
	{ ShowMessage("Number of Fluorophores should be between 0 and 8");
	  GreenCytoAnumEdit->Text=cond.greenCytoAnum;return; }
  cond.greenCytoAnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoAnumEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) GreenCytoAnumEditExit(Sender); }

//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedCytoAnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
    { ShowMessage("Number of Fluorophores should be between 0 and 8");
      RedCytoAnumEdit->Text=cond.redCytoAnum;return; }
  cond.redCytoAnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoAnumEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) RedCytoAnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmobEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAmobEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 20.0 )
    { ShowMessage("Wrong Value of Protein Diffusion Rate");CytoAmobEdit->Text=cond.cytoAmob;return; }
  cond.cytoAmob=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmobEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) CytoAmobEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoAfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenCytoAfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
	{ ShowMessage("Fluorescent Fraction should be between 0 and 100%");
	  GreenCytoAfractionEdit->Text=cond.greenCytoAfraction; return; }
  cond.greenCytoAfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoAfractionEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) GreenCytoAfractionEditExit(Sender); }
//---------------------------------------------------------------------------
// Site A settings
//---------------------------------------------------------------------------
void __fastcall TSForm::MembAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(MembAnumEdit->Text,-1);
  if ( buf < 0 || buf > MEMB_NUM )
	{ ShowMessage("Wrong number of memb A objects (0-50000)");
	  MembAnumEdit->Text=cond.membAnum;return;
	}
  cond.membAnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembAnumEditKeyPress(TObject *Sender, char &Key)
{ if ( Key == 13 ) MembAnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenMembAnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
	{ ShowMessage("Number of Fluorophores should be between 0 and 4");
	  GreenMembAnumEdit->Text=cond.greenMembAnum;return; }
  cond.greenMembAnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembAnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) GreenMembAnumEditExit(Sender); }

//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedMembAnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
    { ShowMessage("Number of Fluorophores should be between 0 and 4");
      RedMembAnumEdit->Text=cond.redMembAnum;return; }
  cond.redMembAnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembAnumEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) RedMembAnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembAmobEditExit(TObject *Sender)
{ double buf=StrToFloatDef(MembAmobEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 10.0 )
	{ ShowMessage("Wrong MembA Diffusion Rate");
	  MembAmobEdit->Text=cond.membAmob;return;
	}
  cond.membAmob=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembAmobEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) MembAmobEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcanBindMembA_ChBoxClick(TObject *Sender)
{ cond.cytoAcanBindMembA=CytoAcanBindMembA_ChBox->Checked;
  CytoAmembAdissRateEdit->Enabled=cond.cytoAcanBindMembA;
  CytoAmembAbindRateEdit->Enabled=cond.cytoAcanBindMembA;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembAdissRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAmembAdissRateEdit->Text,-1.0);
  if ( buf <= 0.0 || buf > 10000 )
	{ ShowMessage("Protein dissociation rate must be 0-10000 s-1");
	  CytoAmembAdissRateEdit->Text=cond.cytoAmembAdissRate; return;
	}
  cond.cytoAmembAdissRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembAdissRateEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) CytoAmembAdissRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembApolimerChBoxClick(TObject *Sender)
{ cond.membApoli=MembApolimerChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::AAdissRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(AAdissRateEdit->Text,-1.0);
  if ( buf <= 0.0 || buf > 10000 )
    { ShowMessage("AA dissociation rate must be 0-10000 s-1");
	  AAdissRateEdit->Text=cond.membAdepoliRate; return;
	}
  cond.membAdepoliRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::AAdissRateEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) AAdissRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembAinitBoxChange(TObject *Sender)
{ cond.membAinit=MembAinitBox->ItemIndex; }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembAfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenMembAfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
    { ShowMessage("Fluorescent Fraction should be between 0 and 100%");
      GreenMembAfractionEdit->Text=cond.greenMembAfraction; return; }
  cond.greenMembAfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembAfractionEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{  if ( Key == 13 ) GreenMembAfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::ReleaseRateEditExit(TObject *Sender)
{double buf=StrToFloatDef(ReleaseRateEdit->Text,-1.0);
  if ( buf < 0.000001 || buf > 20 )
    { ShowMessage("Wrong release rate (must be > 0.0 and < 20.0 s-1)");
      ReleaseRateEdit->Text=MainForm->moleculeReleaseRate; return;
    }
  MainForm->moleculeReleaseRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::ReleaseRateEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) ReleaseRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MonomersInFilamentEditExit(TObject *Sender)
{ int buf=StrToIntDef(MonomersInFilamentEdit->Text,-1);
  if ( buf < 10 || buf > 2000 )
    { ShowMessage("Number of monomers should be between 10 and 2000");
      MonomersInFilamentEdit->Text=cond.monomersPerFilament; return; }
  cond.monomersPerFilament=buf;
}
//---------------------------------------------------------------------------
// Site B settings
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(MembBnumEdit->Text,-1);
  if ( buf < 0 || buf > MEMB_NUM )
	{ ShowMessage("Wrong number of site B molecules (0-50000)");
	  MembBnumEdit->Text=cond.membBnum;return;
	}
  cond.membBnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) MembBnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembBnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenMembBnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
    { ShowMessage("Number of Fluorophores should be between 0 and 4");
      GreenMembBnumEdit->Text=cond.greenMembBnum;return; }
  cond.greenMembBnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembBnumEditKeyDown(TObject *Sender,WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) GreenMembBnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembBnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedMembBnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
    { ShowMessage("Number of Fluorophores should be between 0 and 4");
      RedMembBnumEdit->Text=cond.redMembBnum;return; }
  cond.redMembBnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembBnumEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) RedMembBnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBmobEditExit(TObject *Sender)
{ double buf=StrToFloatDef(MembBmobEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 10.0 )
	{ ShowMessage("Wrong MembB Diffusion Rate");
	  MembBmobEdit->Text=cond.membBmob;return;
	}
  cond.membBmob=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBmobEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) MembBmobEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcanBindMembB_ChBoxClick(TObject *Sender)
{ cond.cytoAcanBindMembB=CytoAcanBindMembB_ChBox->Checked;
  CytoAmembBbindRateEdit->Enabled=cond.cytoAcanBindMembB;
  CytoAmembBdissRateEdit->Enabled=cond.cytoAcanBindMembB;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembBdissRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAmembBdissRateEdit->Text,-1.0);
  if ( buf <= 0.0 || buf > 10000000 )
    { ShowMessage("Wrong protein dissociation rate (must be > 0)");
      CytoAmembBdissRateEdit->Text=cond.cytoAmembBdissRate; return;
    }
  cond.cytoAmembBdissRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembBbindRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAmembBbindRateEdit->Text,-1.0);
  if ( buf <= 0.0 || buf > 10000000 )
    { ShowMessage("Wrong protein binding rate (must be > 0)");
      CytoAmembBbindRateEdit->Text=cond.cytoAmembBbindRate; return;
    }
  cond.cytoAmembBbindRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembBdissRateEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) CytoAmembBdissRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembAcanBindMembB_ChBoxClick(TObject *Sender)
{ cond.membAcanBindMembB=MembAcanBindMembB_ChBox->Checked;
  AfromBdissRateEdit->Enabled=cond.membAcanBindMembB;
  SiteABbindingRateEdit->Enabled=cond.membAcanBindMembB;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembBbindRateEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoAmembBbindRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::AfromBdissRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(AfromBdissRateEdit->Text,-1.0);
  if ( buf <= 0.0 || buf > 10000000 )
    { ShowMessage("Wrong A-B dissociation rate (must be > 0)");
      AfromBdissRateEdit->Text=cond.membAmembBdissRate; return;
    }
  cond.membAmembBdissRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::AfromBdissRateEditKeyDown(TObject *Sender,WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) AfromBdissRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembBfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenMembBfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
	{ ShowMessage("Fluorescent Fraction should be between 0 and 100%");
	  GreenMembBfractionEdit->Text=cond.greenMembBfraction; return; }
  cond.greenMembBfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembBfractionEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) GreenMembBfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBinitBoxChange(TObject *Sender)
{ cond.membBinit=MembBinitBox->ItemIndex; }
//---------------------------------------------------------------------------
void __fastcall TSForm::ClusterNumEditExit(TObject *Sender)
{ int buf=StrToIntDef(ClusterNumEdit->Text,-1);
  if ( buf < 1 || buf > cond.membAnum )
	{ ShowMessage("Number of MembA in one cluster is out of range");
      ClusterNumEdit->Text=cond.membAclusterNum; return;}
  cond.membAclusterNum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::ClusterNumEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) ClusterNumEditExit(Sender); }
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Illumination/imaging Settings
//---------------------------------------------------------------------------
void __fastcall TSForm::SpotSizeEditExit(TObject *Sender)
{ int buf=StrToIntDef(SpotSizeEdit->Text,-1);
  if ( buf < 50 || buf > 1000 )
    {ShowMessage("Wrong spot size (Should be between 50 and 1000 nm");
     SpotSizeEdit->Text=cond.FWHM; return;
    }
  cond.FWHM=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::SpotSizeEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) SpotSizeEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenEmissionRateEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenEmissionRateEdit->Text,-1);
  if ( buf < 1 || buf > 100000 )
	{ShowMessage("Photon Number out of range (1-100000)");
     GreenEmissionRateEdit->Text=cond.greenPhotonsPerSec;return;
	}
  cond.greenPhotonsPerSec=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenEmissionRateEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) GreenEmissionRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedEmissionRateEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedEmissionRateEdit->Text,-1);
  if ( buf < 1 || buf > 100000 )
	{ShowMessage("Photon Number out of range (1-100000)");
     RedEmissionRateEdit->Text=cond.redPhotonsPerSec;return;
    }
  cond.redPhotonsPerSec=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedEmissionRateEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) RedEmissionRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenBleachRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(GreenBleachRateEdit->Text,-1);
  if ( buf < 0.000001 || buf > 1000 )
	{ ShowMessage("Wrong PhotoBleech. rate (must be within 1e-6 and 1000 s-1");
      GreenBleachRateEdit->Text=cond.greenBleachRate; return; }
  cond.greenBleachRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenBleachRateEditKeyDown(TObject *Sender,WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) GreenBleachRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedBleachRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(RedBleachRateEdit->Text,-1);
  if ( buf < 0.000001 || buf > 1000 )
	{ ShowMessage("Wrong PhotoBleech. rate (must be within 1e-6  and 1000 s-1");
      RedBleachRateEdit->Text=cond.redBleachRate; return; }
  cond.redBleachRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedBleachRateEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{  if ( Key == 13 ) RedBleachRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::IllumFWHMEditExit(TObject *Sender)
{ double buf=StrToFloatDef(IllumFWHMEdit->Text,-1.0);
  if ( buf < 0.1 || buf > 10000.0 )
	{ ShowMessage("Illumination Profile FWHM is out of Order (0.1-10000µm)");
	  IllumFWHMEdit->Text=cond.illumFWHMum;return;
    }
  cond.illumFWHMum=buf; IllumFWHMEdit->Text=cond.illumFWHMum; DrawGauss();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::IllumFWHMEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) IllumFWHMEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::DrawGauss(void)
{ int left=30,top=30; float zoom=1.0;
  float ratio=(cond.illumFWHMum*100000.0)/(float)(cond.cellXnm);
  double sigma=ratio/2.3548;
  float irisRad=(cond.irisRadUm*100000.0)/(float)(cond.cellXnm);
  GaussImage->Canvas->Brush->Color=clBtnFace;
  GaussImage->Canvas->FillRect(Rect(0,0,GaussImage->Width,GaussImage->Height));
  GaussImage->Canvas->Pen->Color=clBlue; GaussImage->Canvas->Pen->Width=2;
  GaussImage->Canvas->MoveTo(12,100-MainForm->Gauss(12-112,sigma)*50.0);
  for ( int i=12;i < 212; i++ )
	{  GaussImage->Canvas->LineTo(i,100-MainForm->Gauss(i-112,sigma)*50.0); }
  if ( irisRad < 110 )
	{  GaussImage->Canvas->Pen->Color=clBlack;GaussImage->Canvas->Pen->Width=6;
	   GaussImage->Canvas->MoveTo(2,45); GaussImage->Canvas->LineTo(112-irisRad,45);
	   GaussImage->Canvas->MoveTo(220,45); GaussImage->Canvas->LineTo(112+irisRad,45);
	}
  GaussImage->Canvas->Pen->Color=clGray; GaussImage->Canvas->Pen->Width=2;
  GaussImage->Canvas->Rectangle(Rect(10,0,210,30));
  for ( int i=12;i < 212; i++ )
	{ GaussImage->Canvas->Pen->Color=(TColor)RGB(0,MainForm->Gauss(i-112,sigma)*200.0,0);
	  if ( i < 112-irisRad || i > 112+irisRad ) GaussImage->Canvas->Pen->Color=clBlack;
	  GaussImage->Canvas->MoveTo(i,2); GaussImage->Canvas->LineTo(i,28);
	}
}
//---------------------------------------------------------------------------
void __fastcall TSForm::IrisRadiusEditExit(TObject *Sender)
{ double buf=StrToFloatDef(IrisRadiusEdit->Text,-1.0);
  if ( buf < 0.1 || buf > 20000 )
	{ ShowMessage("Diaphragm Diameter is out of Order (0.1-2000)");
	  IrisRadiusEdit->Text=cond.irisRadUm;return;
	}
  cond.irisRadUm=buf; IrisRadiusEdit->Text=cond.irisRadUm; DrawGauss();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::IrisRadiusEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) IrisRadiusEditExit(Sender);  }
//---------------------------------------------------------------------------
void __fastcall TSForm::FramesToSumEditExit(TObject *Sender)
{ int buf=StrToIntDef(FramesToSumEdit->Text,-1);
  if ( buf < 1 || buf > 1000 )
	{ShowMessage("Number of time intervals is out of range (1-1000)");
     FramesToSumEdit->Text=cond.framesToSum; return;
    }
  cond.framesToSum=buf; int fps=100000.0/(cond.tmInterMs*cond.framesToSum);
  String str=((float)fps)*0.01; str+="fps";
  fpsLab->Caption=str; fps2Lab->Caption=str;
}  
//---------------------------------------------------------------------------
void __fastcall TSForm::FramesToSumEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{  if ( Key == 13 ) FramesToSumEditExit(Sender);}
//---------------------------------------------------------------------------
void __fastcall TSForm::DarkFramesEditExit(TObject *Sender)
{ int buf=StrToIntDef(DarkFramesEdit->Text,-1);
  if ( buf < 0 || buf > 10000 )
    {ShowMessage("Number of dark time intervals is out of range (0-10000)");
     DarkFramesEdit->Text=cond.numOfDarkFrames; return;
    }
  cond.numOfDarkFrames=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::DarkFramesEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) FramesToSumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CameraNoiseEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CameraNoiseEdit->Text,-1);
  if ( buf < 0.0 || buf > 30 )
	{ ShowMessage("Wrong Noise Level (0-30)"); CameraNoiseEdit->Text=cond.noise; return; }
  cond.noise=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CameraNoiseEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{ if ( Key == 13 ) CameraNoiseEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::EMCCD_ChBoxClick(TObject *Sender)
{ if ( EMCCD_ChBox->Checked ) { GainEdit->Enabled=true,cond.EMCCD=true; }
  else                        { GainEdit->Enabled=false; cond.EMCCD=false; };
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GainEditExit(TObject *Sender)
{ double buf=StrToFloatDef(GainEdit->Text,-1);
  if ( buf < 0.0 || buf > 255.0 )
	{ ShowMessage("Wrong Gain Level (0-255)"); GainEdit->Text=cond.gain; return; }
  cond.gain=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GainEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) GainEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::AAbindingRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(AAbindingRateEdit->Text,-1);
  if ( buf < 1 || buf > 1e7 )
    { ShowMessage("Rate is out of Range (1-1e7 s-1)");
	  AAbindingRateEdit->Text=cond.membApoliRate; return;
    }
  cond.membApoliRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::AAbindingRateEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) AAbindingRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RaftMobRatioEditExit(TObject *Sender)
{ double buf=StrToFloatDef(RaftMobRatioEdit->Text,-1);
  if ( buf < 0.0 || buf > 10.0 )
    { ShowMessage("Raft Mobility Ratio Out of Order (0-10)");
      RaftMobRatioEdit->Text=cond.raftMobRatio; return;
    }
  cond.raftMobRatio=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RaftMobRatioEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) RaftMobRatioEditExit(Sender);   }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembAbindRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAmembAbindRateEdit->Text,-1);
  if ( buf < 1.0 || buf > 1e7 )
    { ShowMessage("Rate is out of Range (1-1e7 s-1)");
      CytoAmembAbindRateEdit->Text=cond.cytoAmembAbindRate; return;
    }
  cond.cytoAmembAbindRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAmembAbindRateEditKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{  if ( Key == 13 ) CytoAmembAbindRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::SiteABbindingRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(SiteABbindingRateEdit->Text,-1);
  if ( buf < 1.0 || buf > 1e7 )
	{ ShowMessage("Rate is out of Range (1-1e7 s-1)");
	  SiteABbindingRateEdit->Text=cond.membAmembBbindRate; return;
	}
  cond.membAmembBbindRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::SiteABbindingRateEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{  if ( Key == 13 ) SiteABbindingRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::FilamentPitchStepEditExit(TObject *Sender)
{ double buf=StrToFloatDef(FilamentPitchStepEdit->Text,-1);
  if ( buf < 1.0 || buf > 1000 )
    { ShowMessage("Distance between the sites is Out of Order (1-1000nm)");
      GainEdit->Text=cond.filamentPitchStepNm; return;
    }
  cond.filamentPitchStepNm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::FilamentPitchStepEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) FilamentPitchStepEditExit(Sender);  }
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeRadEditExit(TObject *Sender)
{ int buf=StrToIntDef(TubeRadEdit->Text,-1);
  if ( buf < 20 || buf > cond.cellZnm-50 || buf > cond.cellXnm-50 ||
	   buf > cond.cellYnm-50 )
	{ ShowMessage("Tube/Ves Radius is Out of Range (>20nm and < cell sizes)");
	  TubeRadEdit->Text=cond.tubeRadNm; return;
    }
  cond.tubeRadNm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeRadEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) TubeRadEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::NucleusRadEditExit(TObject *Sender)
{ int buf=StrToIntDef(NucleusRadEdit->Text,-1);
  if ( buf < 100 || buf > 100000  )
    { ShowMessage("Nucleus Radius is out of range (100-100000 nm)");
      NucleusRadEdit->Text=cond.nucleusRadNm; return;
    }
  cond.nucleusRadNm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::NucleusRadEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) NucleusRadEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoAfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedCytoAfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100  )
	{ ShowMessage("Percentage of RED molecules must be 0-100%");
	  RedCytoAfractionEdit->Text=cond.redCytoAfraction; return;
	}
  cond.redCytoAfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoAfractionEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) RedCytoAfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembAfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedMembAfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100  )
    { ShowMessage("Percentage of RED molecules must be 0-100%");
      RedMembAfractionEdit->Text=cond.redMembAfraction; return;
    }
  cond.redMembAfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembAfractionEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) RedMembAfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembBfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedMembBfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100  )
    { ShowMessage("Percentage of RED molecules must be 0-100%");
      RedMembBfractionEdit->Text=cond.redMembBfraction; return;
    }
  cond.redMembBfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembBfractionEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) RedMembBfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::TwoCamChBoxClick(TObject *Sender)
{ cond.twoCam=TwoCamChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::KobjEditExit(TObject *Sender)
{ double buf=StrToFloatDef(KobjEdit->Text,-1);
  if ( buf < 0.0 || buf > 100 )
    { ShowMessage("Kobj is Out of Order (0-100)"); KobjEdit->Text=cond.Kobj; return; }
  cond.Kobj=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::KobjEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) KobjEditExit(Sender);}
//---------------------------------------------------------------------------
void __fastcall TSForm::StatAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(StatAnumEdit->Text,-1);
  if ( buf < 0 || buf > STAT_NUM )
	{ ShowMessage("Wrong number of static A objects (0-100000)");
	  StatAnumEdit->Text=cond.statAnum;return;
	}
  cond.statAnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::StatAnumEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) StatAnumEditExit(Sender);  }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenStatAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenStatAnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
	{ ShowMessage("Wrong number of Fluorophores per Object(0-4)");
      GreenStatAnumEdit->Text=cond.greenStatAnum;return;
    }
  cond.greenStatAnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenStatAnumEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) GreenStatAnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::StatAinitBoxChange(TObject *Sender)
{ cond.statAinit=StatAinitBox->ItemIndex; }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenStatAfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenStatAfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
	{ ShowMessage("Wrong percentage (0-100%)");
      GreenStatAfractionEdit->Text=cond.greenStatAfrac;return;
    }
  cond.greenStatAfrac=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenStatAfractionEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) GreenStatAfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcanBindStatAchBoxClick(TObject *Sender)
{ cond.cytoAcanBindStatA=CytoAcanBindStatAchBox->Checked;
  CytoAstatAdissRateEdit->Enabled=cond.cytoAcanBindStatA;
  CytoAstatAbindRateEdit->Enabled=cond.cytoAcanBindStatA;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedStatAnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedStatAnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
	{ ShowMessage("Wrong number of Fluorophores per Object (0-4)");
      RedStatAnumEdit->Text=cond.redStatAnum;return;
    }
  cond.redStatAnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedStatAnumEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{  if ( Key == 13 ) RedStatAnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedStatAfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedStatAfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
	{ ShowMessage("Wrong percentage (0-100%)");
	  RedStatAfractionEdit->Text=cond.redStatAfrac; return;
    }
  cond.redStatAfrac=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedStatAfractionEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) RedStatAfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstatAbindRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAstatAbindRateEdit->Text,-1);
  if ( buf < 1.0 || buf > 1e7 )
    { ShowMessage("Rate is out of Range (1-1e7 s-1)");
      CytoAstatAbindRateEdit->Text=cond.cytoAstatAbindRate; return;
    }
  cond.cytoAstatAbindRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstatAbindRateEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoAstatAbindRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstatAdissRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAstatAdissRateEdit->Text,-1.0);
  if ( buf <= 0.0 || buf > 10000 )
    { ShowMessage("CytoA dissociation rate must be 0-10000 s-1");
      CytoAstatAdissRateEdit->Text=cond.cytoAstatAdissRate; return;
    }
  cond.cytoAstatAdissRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstatAdissRateEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoAstatAdissRateEditExit(Sender);  }
//---------------------------------------------------------------------------
void __fastcall TSForm::FlatteningEditExit(TObject *Sender)
{ double buf=StrToFloatDef(FlatteningEdit->Text,-1.0);
  if ( buf < 1.0 || buf > 5.0 )
	{ ShowMessage("Flattering value must be between 1.0 and 5");
      FlatteningEdit->Text=cond.flattening; return;
    }
  cond.flattening=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::FlatteningEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) FlatteningEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcanMoveAlongStatAClick(TObject *Sender)
{ // VelocityEdit->Enabled=
cond.cytoAcanMoveAlongStatA=CytoAcanMoveAlongStatA->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TIRangleEditExit(TObject *Sender)
{ double buf=StrToFloatDef(TIRangleEdit->Text,-1.0);
  if ( buf < 63.0 || buf > 75.0 )
    { ShowMessage("TIR angle range (63o and 75o)");
      TIRangleEdit->Text=cond.TIRangle; return;
    }
  cond.TIRangle=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TIRangleEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) TIRangleEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoBnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(CytoBnumEdit->Text,-1);
  if ( buf < 0 || buf > CYTO_NUM )
    { ShowMessage("Wrong number of CytoB molecules (0-50000)");
      CytoBnumEdit->Text=cond.cytoBnum;return;
    }
  cond.cytoBnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoBnumEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) CytoBnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembCnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(MembCnumEdit->Text,-1);
  if ( buf < 0 || buf > MEMB_NUM )
    { ShowMessage("Wrong number of MembC molecules (0-50000)");
      MembCnumEdit->Text=cond.membCnum;return;
    }
  cond.membCnum=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembCnumEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{ if ( Key == 13 ) MembCnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::TwoAngleChBoxClick(TObject *Sender)
{ MainForm->twoAngles=TwoAngleChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoBmobEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoBmobEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 50.0 )
	{ ShowMessage("Wrong Value of CytoB Diffusion Rate (0-50um2/s)");CytoBmobEdit->Text=cond.cytoBmob;return; }
  cond.cytoBmob=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoBmobEditKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{ if ( Key == 13 ) CytoBmobEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::OneColourChBoxClick(TObject *Sender)
{ cond.oneColourAA=OneColourChBox->Checked;
  if ( cond.oneColourAA )
	{ GreenMembAnumEdit->Text=cond.greenMembAnum=0; RedMembAnumEdit->Text=cond.redMembAnum=0; }
}
//---------------------------------------------------------------------------
void __fastcall TSForm::AlterChBoxClick(TObject *Sender)
{ cond.alterIllum=AlterChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeDistEditExit(TObject *Sender)
{ int buf=StrToIntDef(TubeDistEdit->Text,-1);
  if ( buf < 100 || buf > 100000 )
	{ ShowMessage("Distance is out of Range (100-100000 nm)");
	  TubeDistEdit->Text=cond.interTubeDistNm; return;
	}
  cond.interTubeDistNm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeDistEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) TubeDistEditExit(Sender);  }

//---------------------------------------------------------------------------
void __fastcall TSForm::LamellaChBoxClick(TObject *Sender)
{ if ( cond.filopodia ) FilopodiaChBox->Checked=cond.filopodia=false;
  if ( cond.tubeNetwork ) TubeNetworkChBox->Checked=cond.tubeNetwork=false;
  if ( cond.tubes )   TubesChBox->Checked=cond.tubes=false;
  cond.lamella=LamellaChBox->Checked; DrawCell();
  TubeRadEdit->Enabled=FilopodiaChBox->Checked;
  TubeLengthEdit->Enabled=LamellaChBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::FilopodiaChBoxClick(TObject *Sender)
{ if ( cond.lamella )  LamellaChBox->Checked=cond.lamella=false;
  if ( cond.tubeNetwork ) TubeNetworkChBox->Checked=cond.tubeNetwork=false;
  if ( cond.tubes )   TubesChBox->Checked=cond.tubes=false;
  cond.filopodia=FilopodiaChBox->Checked;    DrawCell();
  TubeRadEdit->Enabled=FilopodiaChBox->Checked;
  TubeDistEdit->Enabled=FilopodiaChBox->Checked;
  TubeLengthEdit->Enabled=FilopodiaChBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeNetworkChBoxClick(TObject *Sender)
{ if ( cond.filopodia ) FilopodiaChBox->Checked=cond.filopodia=false;
  if ( cond.lamella )   LamellaChBox->Checked=cond.lamella=false;
  if ( cond.tubes )   TubesChBox->Checked=cond.tubes=false;
  cond.tubeNetwork=TubeNetworkChBox->Checked;  DrawCell();
  VesNumEdit->Enabled=true;
  TubeRadEdit->Enabled=TubeNetworkChBox->Checked;
  TubeDistEdit->Enabled=TubeNetworkChBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeLengthEditExit(TObject *Sender)
{ int buf=StrToIntDef(TubeLengthEdit->Text,-1),lenLim=100000;
  if ( !CheckSize(cond.cellXnm,cond.cellYnm,cond.cellZnm,cond.sideGapNm,cond.botGapNm,cond.topGapNm) )
   { TubeLengthEdit->Text=cond.tubeLengthNm; return; }
  if ( TubesChBox->Checked ) lenLim=cond.cellXnm;
  if ( buf < 20 || buf > lenLim || buf < cond.tubeRadNm*2.0  )
	{ ShowMessage("Length is out of Range (Cell length?)");
	  TubeLengthEdit->Text=cond.tubeLengthNm; return;
	}
  cond.tubeLengthNm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeLengthEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{  if ( Key == 13 ) TubeLengthEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::TubesChBoxClick(TObject *Sender)
{ if ( cond.tubeNetwork ) TubeNetworkChBox->Checked=cond.tubeNetwork=false;
  if ( cond.lamella )  LamellaChBox->Checked=cond.lamella=false;
  if (  cond.movingVesicles )
	{ cond.movingVesicles=false; MovingVesChBox->Checked=false; }
  cond.tubes=TubesChBox->Checked; DrawCell();
  TubeRadEdit->Enabled=TubesChBox->Checked;
  TubeDistEdit->Enabled=TubesChBox->Checked;
  TubeLengthEdit->Enabled=TubesChBox->Checked;
  if ( cond.cellXnm < cond.tubeLengthNm ) cond.tubeLengthNm=cond.cellXnm;
  TubeLengthEdit->Text=cond.tubeLengthNm;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::SideGapEditExit(TObject *Sender)
{ int buf=StrToFloatDef(SideGapEdit->Text,-1.0)*1000.0;
  if ( !CheckSize(cond.cellXnm,cond.cellYnm,cond.cellZnm,buf,cond.botGapNm,cond.topGapNm) )
   { SideGapEdit->Text=cond.sideGapNm*0.001; return; }
  if ( buf < cond.voxelSize*2 )
	{ ShowMessage("Side Gap is too small");SideGapEdit->Text=cond.sideGapNm*0.001;return; }
  SideGapEdit->Text=buf*0.001; cond.sideGapNm=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::SideGapEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) SideGapEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::BottomGapEditExit(TObject *Sender)
{ int buf=StrToFloatDef(BottomGapEdit->Text,-1.0)*1000.0;
  if ( !CheckSize(cond.cellXnm,cond.cellYnm,cond.cellZnm,cond.sideGapNm,buf,cond.topGapNm) )
   { BottomGapEdit->Text=cond.botGapNm*0.001; return; }
   if ( buf < cond.voxelSize*2 )
	{ ShowMessage("Bottom Gap is too small");BottomGapEdit->Text=cond.botGapNm*0.001;return;}
  BottomGapEdit->Text=buf*0.001; cond.botGapNm=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::BottomGapEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) BottomGapEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::TopGapEditExit(TObject *Sender)
{ int buf=StrToFloatDef(TopGapEdit->Text,-1.0)*1000.0;
  if ( !CheckSize(cond.cellXnm,cond.cellYnm,cond.cellZnm,cond.sideGapNm,cond.botGapNm,buf) )
	{ TopGapEdit->Text=cond.topGapNm*0.001; return; }
  if ( buf < cond.voxelSize*2 )
	{ ShowMessage("Top Gap is too small");TopGapEdit->Text=cond.topGapNm*0.001;return;}
  TopGapEdit->Text=buf*0.001; cond.topGapNm=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TopGapEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) TopGapEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAinitBoxChange(TObject *Sender)
{ cond.cytoAinit=CytoAinitBox->ItemIndex; }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstepSizeEditExit(TObject *Sender)
{ double buf=StrToIntDef(CytoAstepSizeEdit->Text,-1.0);
  if ( buf < 1.0 || buf > 100.0 )
	{ ShowMessage("Step Size is Out of Range (1-100nm)");
	  CytoAstepSizeEdit->Text=cond.cytoAstepSizeNm;  return;
	}
  cond.cytoAstepSizeNm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstepSizeEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoAstepSizeEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstepRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAstepRateEdit->Text,-1.0);
  if ( buf < 0.001 || buf > 100.0 )
	{ ShowMessage("Stepping Rate is Out of Range (0.001-100s-1)");
	  CytoAstepRateEdit->Text=cond.cytoAstepRate;  return;
	}
  cond.cytoAstepRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAstepRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoAstepRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::PoreRadiusEditExit(TObject *Sender)
{ int buf=StrToFloatDef(PoreRadiusEdit->Text,-1.0);
  if ( buf < 0 || buf > cond.vesRadNm )
	{ ShowMessage("Pore External Diam is Out of Range (0-VesiRad)");
	  PoreRadiusEdit->Text=cond.poreRadNm;  return;
	}
  PoreRadiusEdit->Text=buf; cond.poreRadNm=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::PoreRadiusEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) PoreRadiusEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::VesTiltEditExit(TObject *Sender)
{ double buf=StrToFloatDef(VesTiltEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 100.0 )
	{ ShowMessage("Vesicle Tilt Coef. is out of order (0.0-100.0)");
	  VesTiltEdit->Text=cond.bendTilt;  return;
	}
  cond.bendTilt=buf;
  CalcPlen();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::VesTiltEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) VesTiltEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::FiberExtentionEditExit(TObject *Sender)
{ int buf=StrToFloatDef(FiberExtentionEdit->Text,-1.0);
  if ( buf < -1000 || buf > 10000 )
	{ ShowMessage("Fiber Extention out of range (+/- 1000nm");
	  FiberExtentionEdit->Text=cond.actinBundleExtendNm;  return;
	}
  FiberExtentionEdit->Text=buf; cond.actinBundleExtendNm=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::FiberExtentionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) FiberExtentionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcytoBbindRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAcytoBbindRateEdit->Text,-1);
  if ( buf < 1.0 || buf > 1e7 )
	{ ShowMessage("Rate is out of Range (1-1e7 s-1)");
	  CytoAcytoBbindRateEdit->Text=cond.cytoAcytoBbindRate; return;
	}
  cond.cytoAcytoBbindRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcytoBbindRateEditKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{ if ( Key == 13 ) CytoAcytoBbindRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcytoBdissRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAcytoBdissRateEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 10000.0 )
	{ ShowMessage("Protein dissociation rate must be 0-10000 s-1");
	  CytoAcytoBdissRateEdit->Text=cond.cytoAcytoBdissRate; return;
	}
  cond.cytoAcytoBdissRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcytoBdissRateEditKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{ if ( Key == 13 ) CytoAcytoBdissRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAcanBindCytoB_ChBoxClick(TObject *Sender)
{ cond.cytoAcanBindCytoB=CytoAcanBindCytoB_ChBox->Checked;
  CytoAcytoBdissRateEdit->Enabled=cond.cytoAcanBindCytoB;
  CytoAcytoBbindRateEdit->Enabled=cond.cytoAcanBindCytoB;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoBinitBoxChange(TObject *Sender)
{ cond.cytoBinit=CytoBinitBox->ItemIndex; }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBcanBindMembC_ChBoxClick(TObject *Sender)
{ cond.membBcanBindMembC=MembBcanBindMembC_ChBox->Checked;
  MembBCdissRateEdit->Enabled=cond.membBcanBindMembC;
  MembBCbindingRateEdit->Enabled=cond.membBcanBindMembC;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembCinitBoxChange(TObject *Sender)
{ cond.membCinit=MembCinitBox->ItemIndex; }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembCmobEditExit(TObject *Sender)
{ double buf=StrToFloatDef(MembCmobEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 10.0 )
	{ ShowMessage("Wrong MembC Diffusion Rate");
	  MembCmobEdit->Text=cond.membCmob;return;
	}
  cond.membCmob=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembCmobEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) MembCmobEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembCnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenMembCnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
	{ ShowMessage("Number of Fluorophores should be between 0 and 4");
	  GreenMembCnumEdit->Text=cond.greenMembCnum; return; }
  cond.greenMembCnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembCnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) GreenMembCnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembCnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedMembCnumEdit->Text,-1);
  if ( buf < 0 || buf > 4 )
	{ ShowMessage("Number of Fluorophores should be between 0 and 4");
	  RedMembCnumEdit->Text=cond.redMembCnum;return; }
  cond.redMembCnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembCnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) RedMembCnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembCfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenMembCfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
	{ ShowMessage("Fluorescent Fraction should be between 0 and 100%");
	  GreenMembCfractionEdit->Text=cond.greenMembCfraction; return; }
  cond.greenMembCfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenMembCfractionEditKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{ if ( Key == 13 ) GreenMembCfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembCfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedMembCfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
	{ ShowMessage("Fluorescent Fraction should be between 0 and 100%");
	  RedMembCfractionEdit->Text=cond.redMembCfraction; return; }
  cond.redMembCfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedMembCfractionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) RedMembCfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBCbindingRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(MembBCbindingRateEdit->Text,-1);
  if ( buf < 1.0 || buf > 1e7 )
	{ ShowMessage("Rate is out of Range (1-1e7 s-1)");
	  MembBCbindingRateEdit->Text=cond.membBmembCbindRate; return;
	}
  cond.membBmembCbindRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBCbindingRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) MembBCbindingRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBCdissRateEditExit(TObject *Sender)
{ double buf=StrToFloatDef(MembBCdissRateEdit->Text,-1.0);
  if ( buf <= 0.0 || buf > 10000000 )
	{ ShowMessage("Wrong MembB-MembC dissociation rate (must be > 0)");
	  MembBCdissRateEdit->Text=cond.membBmembCdissRate; return;
	}
  cond.membBmembCdissRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBCdissRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) MembBCdissRateEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::FRETchBoxClick(TObject *Sender)
{ if ( FRETchBox->Checked ) FRETchBox->Font->Color=clRed;
  else                      FRETchBox->Font->Color=clBlue;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RoundedCellBodyChBoxClick(TObject *Sender)
{ cond.roundedCellBody=RoundedCellBodyChBox->Checked;
  // if ( cond.tubulesON ) TubulesChBox->Checked=cond.tubulesON=false;
  // if ( cond.lamellaON ) LamellaChBox->Checked=cond.lamellaON=false;
  // if ( cond.vesiclesON ) VesicleChBox->Checked=cond.vesiclesON=false;
  BodyFilletEdit->Enabled=RoundedCellBodyChBox->Checked;Statistics();DrawCell();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::BodyFilletEditExit(TObject *Sender)
{ int buf=StrToIntDef(BodyFilletEdit->Text,-1);
  if ( buf < 20 || buf > cond.cellZnm*0.5 || buf > cond.cellXnm*0.5 || buf > cond.cellYnm*0.5 )
	{ ShowMessage("Cell Body Fillet is out of range (cellSizes/2)");
	  BodyFilletEdit->Text=cond.bodyFilletNm; return;
	}
  cond.bodyFilletNm=buf; Statistics();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::BodyFilletEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) BodyFilletEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MovingVesChBoxClick(TObject *Sender)
{ cond.movingVesicles=MovingVesChBox->Checked; VesRadEdit->Enabled=MovingVesChBox->Checked;
  VesNumEdit->Enabled=MovingVesChBox->Checked;
  cond.tubes=false; TubesChBox->Checked=false;
  DrawCell();
}
//---------------------------------------------------------------------------
void __fastcall TSForm::VesMobEditExit(TObject *Sender)
{ double buf=StrToFloatDef(VesMobEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 5.0 )
	{ ShowMessage("Vesicle mobility is out of range (0 - 5.0µm2/s)");
	  VesMobEdit->Text=cond.vesicleMob; return;
	}
  cond.vesicleMob=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::VesMobEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) VesMobEditExit(Sender);  }
//---------------------------------------------------------------------------
void __fastcall TSForm::MoveToNucleusChBoxClick(TObject *Sender)
{ cond.membAorganPump=MoveToNucleusChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::MembBmoveCytoAcytoClick(TObject *Sender)
{ cond.membBcytoPump=MembBmoveCytoAcyto->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::OutsideChBoxClick(TObject *Sender)
{ cond.outsideVesicles=OutsideChBox->Checked;  }
//---------------------------------------------------------------------------
void __fastcall TSForm::VesNumEditExit(TObject *Sender)
{ int buf=StrToIntDef(VesNumEdit->Text,-1);
  if ( buf < 1 || buf > 150 )
	{ ShowMessage("Too many or too few vesicles");
	  VesNumEdit->Text=cond.vesNum; return;
	}
  cond.vesNum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::VesNumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) VesNumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::VesRadEditExit(TObject *Sender)
{ int buf=StrToIntDef(VesRadEdit->Text,-1);
  if ( buf < 10 || buf > 1000 )
	{ ShowMessage("Wrong radius (Must be 10-1000nm)");
	  VesRadEdit->Text=cond.vesRadNm; return;
	}
  cond.vesRadNm=buf;
  double vKdiff=KbT/(6.0*PI*cond.vesRadNm*0.001);
  AnsiString str="Estim. D="; str+=Rounding(vKdiff,4); str+="µm2/s";
  VesiKdiffLab->Caption=str;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::VesRadEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) VesRadEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::VoxelSizeEditExit(TObject *Sender)
{ int buf=StrToIntDef(VoxelSizeEdit->Text,-1);
  if ( buf < 1 || buf > 100 )
	{ ShowMessage("Voxel Size Must be 1-100nm");
	  VoxelSizeEdit->Text=cond.voxelSize; return;
	}
  if ( !CheckSize(cond.cellXnm,cond.cellYnm,cond.cellZnm,cond.sideGapNm,cond.botGapNm,cond.topGapNm) )
	{ VoxelSizeEdit->Text=cond.voxelSize; return; }
  cond.voxelSize=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::VoxelSizeEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) VoxelSizeEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MemorySizeEditExit(TObject *Sender)
{ int buf=StrToIntDef(MemorySizeEdit->Text,-1);
  if ( buf < 8000 || buf > 48000 )
	{ ShowMessage("Memory Size Must be 8-48000 Mbyte");
	  MemorySizeEdit->Text=cond.memorySizeMb; return;
	}
  cond.memorySizeMb=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembApoliNumEditExit(TObject *Sender)
{ int buf=StrToIntDef(MembApoliNumEdit->Text,-1);
  if ( buf < 2 || buf > 16 )
	{ ShowMessage("Number of Units should be 2-16");
	  MembApoliNumEdit->Text=cond.membApoliNum; return;
	}
  cond.membApoliNum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MembApoliNumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{  if ( Key == 13 ) MembApoliNumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::MitoChBoxClick(TObject *Sender)
{ cond.mitochondria=MitoChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::MonoPerFiloEditExit(TObject *Sender)
{ int buf=StrToIntDef(MonoPerFiloEdit->Text,-1);
  if ( buf < 10 || buf > 100000 )
	{ ShowMessage("Number of monomers should be between 10-100000");
	  MonoPerFiloEdit->Text=cond.monoPerFilo; return;
	}
  cond.monoPerFilo=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::MonoPerFiloEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) MonoPerFiloEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::BlinkOnEditExit(TObject *Sender)
{ double buf=StrToFloatDef(BlinkOnEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 100.0 )
	{ ShowMessage("ON rate is out of range (0 - 100.0s-1)");
	  BlinkOnEdit->Text=cond.greenBlinkONrate; return;
	}
  cond.greenBlinkONrate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::BlinkOffEditExit(TObject *Sender)
{ double buf=StrToFloatDef(BlinkOffEdit->Text,-1.0);
  if ( buf < 0.0 || buf > 100.0 )
	{ ShowMessage("OFF rate is out of range (0 - 100.0s-1)");
	  BlinkOffEdit->Text=cond.greenBlinkOFFrate; return;
	}
  cond.greenBlinkOFFrate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::BlinkOnEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) BlinkOnEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::BlinkOffEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{  if ( Key == 13 ) BlinkOffEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::Tube3DboxClick(TObject *Sender)
{ cond.tube3D=Tube3Dbox->Checked; CalcPlen(); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoBnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenCytoBnumEdit->Text,-1);
  if ( buf < 0 || buf > 8 )
	{ ShowMessage("Number of Fluorophores should be between 0 and 8");
	  GreenCytoBnumEdit->Text=cond.greenCytoBnum;return; }
  cond.greenCytoBnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoBnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) GreenCytoBnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoBnumEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedCytoBnumEdit->Text,-1);
  if ( buf < 0 || buf > 8 )
	{ ShowMessage("Number of Fluorophores should be between 0 and 8");
	  RedCytoBnumEdit->Text=cond.redCytoBnum;return; }
  cond.redCytoBnum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoBnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) RedCytoBnumEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoBfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(GreenCytoBfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100 )
	{ ShowMessage("Fluorescent Fraction should be between 0 and 100%");
	  GreenCytoBfractionEdit->Text=cond.greenCytoBfraction; return; }
  cond.greenCytoBfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::GreenCytoBfractionEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{ if ( Key == 13 ) GreenCytoBfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoBfractionEditExit(TObject *Sender)
{ int buf=StrToIntDef(RedCytoBfractionEdit->Text,-1);
  if ( buf < 0 || buf > 100  )
	{ ShowMessage("Percentage of RED molecules must be 0-100%");
	  RedCytoBfractionEdit->Text=cond.redCytoBfraction; return;
	}
  cond.redCytoBfraction=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RedCytoBfractionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) RedCytoBfractionEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoApolimerChBoxClick(TObject *Sender)
{ CytoApolimerEdit->Enabled=cond.cytoApoli=CytoApolimerChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoApolimerEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoApolimerEdit->Text,-1);
  if ( buf < 1.0 || buf > 1e7 )
	{ ShowMessage("Rate is out of Range (1-1e7 s-1)");
	  CytoApolimerEdit->Text=cond.cytoApoliRate; return;
	}
  cond.cytoApoliRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoApolimerEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoApolimerEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAdepoliEditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoAdepoliEdit->Text,-1);
  if ( buf < 0.01 || buf > 1e7 )
	{ ShowMessage("Rate is out of Range (0.01-1e7 s-1)");
	  CytoAdepoliEdit->Text=cond.cytoAdepoliRate; return;
	}
  cond.cytoAdepoliRate=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoAdepoliEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoAdepoliEditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoBdistAeditExit(TObject *Sender)
{ double buf=StrToFloatDef(CytoBdistAedit->Text,-1.0);
  if ( buf < 0.0 || buf > 1000.0 )
	{ ShowMessage("The Distance must be 0-1000.0 nm");
	  CytoBdistAedit->Text=cond.cytoBdistA; return;
	}
  cond.cytoBdistA=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::CytoBdistAeditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) CytoBdistAeditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RecycleChBoxClick(TObject *Sender)
{ cond.recycling=RecycleChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::FuseVesiclesChBoxClick(TObject *Sender)
{ cond.fuseVes=FuseVesiclesChBox->Checked; }
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeZeditExit(TObject *Sender)
{ int buf=StrToIntDef(TubeZedit->Text,-1);
  if ( buf < cond.tubeRadNm+cond.botGapNm+cond.tubeGapNm+cond.voxelSize*2
	   || buf > cond.cellZnm+cond.botGapNm-cond.tubeRadNm-cond.voxelSize*2 )
	{ ShowMessage("Tube Z plane is Out of Range (botomGap+tubeRad <> cellZ)");
	  TubeZedit->Text=cond.tubeZnm; return;
	}
  cond.tubeZnm=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::TubeZeditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) TubeZeditExit(Sender); }
//---------------------------------------------------------------------------
void __fastcall TSForm::RaftNumEditExit(TObject *Sender)
{ int buf=StrToIntDef(RaftNumEdit->Text,-1);
  if ( buf < 1 || buf > 1000 )
	{ ShowMessage("Raft Number Must be between 1-1000");
	  RaftNumEdit->Text=cond.raftNum; return;
	}
  cond.raftNum=buf;
}
//---------------------------------------------------------------------------
void __fastcall TSForm::RaftNumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ if ( Key == 13 ) RaftNumEditExit(Sender); }
//---------------------------------------------------------------------------

