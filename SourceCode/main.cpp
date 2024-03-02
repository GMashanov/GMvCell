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
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <windows.h>
#include <printers.hpp>
#include "main.h"
#include "Settings.h"
#include "Cell3Dview.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
double radToDeg=57.295779513082320876798154814105;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner):TForm(Owner)
{ struct  time t;        Left=10;Top=5;
  gettime(&t); srand(43);  // randomizing
  for ( int i=0; i < t.ti_min+t.ti_sec; i++ ) random(t.ti_min+t.ti_sec);  //GM

 // General properties
  tmStep=0.005; // (s)
  prm.leftX=prm.topY=0; prm.Igain=prm.Vgain=prm.bin=1;
  nmVox=5; nmPix=20; memorySizeMb=MATRIX_SIZE/(1e6);

  // cell properties
  membOK=false; movingVes=false;  roundedCellBody=true;
  cellXnm=2000; cellYnm=1000; cellZnm=600; tubeRadNm=100; bodyFilletNm=300;
  sideGapNm=200;botGapNm=40;topGapNm=40; filopodia=true;//tubeNetwork=true;// nucleus=true;
  vesNum=3; interTubeDistNm=1000; tubeLengthNm=3000; tubeGapNm=300; vesRadNm=100;
  fenceJumpProb=20; raftMobRatio=0.1; nucleusRadNm=400;
  raftRadNm=100; gridSizeNm=1000; flattening=1.2;  vesiGen=0;
  statApitchNm=8;   IDum=0.005;  // interaction distance in µm
  intVar=20;  time=0.0;  TrackBar->Max=cellZnm+botGapNm; poreRadNm=40;
  vesicleMob=0.02; fuseVesicles=false; recycling=false; // filopodiaON=true;
 // twoCam=true;  // tube3D=true;

	CalculateDimensions();

  bendTilt=1.0; // vesicles horisontal

  // illumination and camera prop

   numOfDarkFrames=0;
  Kobj=0.6; EMCCDmode=true; noiseAmp=0.5; gain=5; framesToSum=10;
  FWHMnm=250; greenPhotonsPerSec=6000,redPhotonsPerSec=10000;
  illumFWHMum=100.0; bleachSizeNm=5000; TIRangle=63;
  d488=Calc_D(TIRangle,488);d561=Calc_D(TIRangle,561);  blueLaser=true;
  greenBlinkONrate=1.0,greenBlinkOFFrate=5.0,FRETratePercent=50.0;
  irisRadUm=50.0;
  TrackBar->Position=focusNm=0; ZscanDir=1;
 // ShowMessage(d561);
  greenBleachRate=0.5; redBleachRate=0.2; // (s-1)
  tir=true; abort=false; las_open=false;
  // Cyto A prop
  cytoAnum=1000; greenCytoAnum=0; redCytoAnum=1; cytoAmob=0.5; // (um2/s)
  greenCytoAfrac=100;  redCytoAfrac=100; cytoAinit=0;
  cytoAmembAbindRate=1e6;  cytoAmembAdissRate=0.2; cytoAmembBbindRate=1e6;
  cytoAstatAbindRate=1e6; cytoAstatAdissRate=0.1;  cytoAmembBdissRate=0.01;
  cytoApoliRate=5e5;  cytoAdepoliRate=0.1;
  cytoAstepSizeNm=8.0,cytoAstepRate=100.0;
 // Cyto B prop
  cytoBnum=0;  greenCytoBnum=3; redCytoBnum=4; cytoBmob=2.0; // (um2/s)
  greenCytoBfrac=50; redCytoBfrac=80; cytoAcytoBdist=100.0;
  cytoBmembAbindRate=1e6; cytoBmembAdissRate=0.2;
  cytoBmembBbindRate=1e6; cytoBmembBdissRate=0.2;
  cytoBstatAbindRate=1e6; cytoBstatAdissRate=0.1;
  cytoAcytoBbindRate=1e6; cytoAcytoBdissRate=0.1;
  // Memb A prop
  membAnum=2000; membAmob=0.1;greenMembAnum=1;redMembAnum=0;
  membAinit=1;membAdepoliRate=1.0;monomersPerFilament=500; membApoliRate=100000.0;
  membAmembBbindRate=10000; membAmembBdissRate=0.1; greenMembAfrac=100;
  redMembAfrac=100; membApoliNum=8; membAclusterNum=8;
  // Memb B prop
  membBnum=0;  membBinit=2;
  greenMembBnum=0; redMembBnum=1;  greenMembBfrac=100; redMembBfrac=100;
  unwindingRate=1.0;  moleculeReleaseRate=1.0;membBmob=0.1;
  vesicleReleaseRate=0.5,vesicleDockingRate=0.5;
  // Memb C prop
  membCnum=0; membCmob=0.03; membCinit=2;  membBcanBindMembC=true;
  greenMembCnum=0; redMembCnum=4; greenMembCfrac=100; redMembCfrac=100;
  membBmembCbindRate=1e5;membBmembCdissRate=1.0;
  // Staic A properties
  statAnum=0,statAinit=0;   actinBundleExtendNm=50; monoPerFilo=200;
  greenStatAfrac=100,greenStatAnum=2,redStatAfrac=100,redStatAnum=0;
  cytoAstatAbindRate=5e5;  cytoAstatAdissRate=0.1;
  // Image arrays mem alloc
  CCD_Bmp=new Graphics::TBitmap();
  CCD_Bmp->Height=3000; CCD_Bmp->Width=3000; CCD_Bmp->PixelFormat=pf32bit;
  Bmp24=new Graphics::TBitmap();
  Bmp24->Height=2000; Bmp24->Width=2000; Bmp24->PixelFormat=pf24bit;
  maskBmp=new Graphics::TBitmap();
  maskBmp->Height=1200; maskBmp->Width=1600;   maskBmp->PixelFormat=pf32bit;
  maskBmp->Canvas->Pen->Color=clWhite; CCD_Bmp->Canvas->Brush->Color=clBlack;
  hiResBmp=new Graphics::TBitmap();
  hiResBmp->Height=1000; hiResBmp->Width=1000;   hiResBmp->PixelFormat=pf24bit;
  hiResBmp->Canvas->Brush->Color=clWhite; hiResBmp->Canvas->Pen->Color=clBlack;
  // chemical intractions
  for ( int d=0; d < 504; d++ )
	timeDistMembAA[d]=timeDistCytoAmembA[d]=timeDistCytoAmembB[d]=timeDistMembAmembB[d]=0.0;
  for ( int d=0; d < 502; d++ )  timeDistCytoAstatA[d]=timeDistMembBmembC[d]=0.0;
  double rad=IDum*0.5; IAum2=PI*(rad*rad); IVum3=(4.0/3.0)*PI*(rad*rad*rad);
  for ( int d=0; d < 502; d++ ) // time in usec
	{ rad=d; rad*=0.001;
	  timeDistMembAA[d]=AIT2D(rad,membAmob*2.0,tmStep,IAum2);
	  timeDistMembAmembB[d]=AIT2D(rad,membAmob+membBmob,tmStep,IAum2);
	  timeDistMembBmembC[d]=AIT2D(rad,membBmob+membCmob,tmStep,IAum2);
	  timeDistCytoAmembA[d]=AIT3D(rad,membAmob+cytoAmob,tmStep,IVum3);
	  timeDistCytoAmembB[d]=AIT3D(rad,membBmob+cytoAmob,tmStep,IVum3);
	  timeDistCytoAcytoB[d]=AIT3D(rad,cytoAmob+cytoBmob,tmStep,IVum3);
	  timeDistCytoAstatA[d]=AIT3D(rad,cytoAmob,tmStep,IVum3);
	}
  for ( int i=0; i < STAT_NUM; i++ ) delta[i]=0.0;
  for ( int d=0; d < 300; d++ ) // dist in nm
	stepDistProb[d]=Gauss(d-100-cytoAstepSizeNm,cytoAstepSizeNm*0.3);
  abort=true; //SetCell();
  // float fact=exp((-((double)(200))*1e-9)/d65_488);
  AnsiString str=Calc_D(65,488); str+="   ";str+=Calc_D(65,560);
  ProgBar->Visible=false;
  // Caption=StatA[580].x; //
 // char bits[32]; bits[8]='\0'; IntToBits(252,bits,8); str; Caption=bits; // exp(-0.8);

 // alloc memory for spots
  spotNum=MAX_SPOTS;
  for ( int i=0; i < spotNum; i++ ) // allocating and zerroing the arrays
	{ spot[i].num=i;    spot[i].spInt=new float[10001];
	  spot[i].cX=new float[10001]; spot[i].cY=new float[10001];
	  if ( spot[i].spInt == NULL || spot[i].cX == NULL || spot[i].cY == NULL )
		 { ShowMessage("Spot Data can not be allocated"); Close(); }
	  spot[i].redInt=new float[10001];
	  if ( spot[i].redInt == NULL )
		{ ShowMessage("Can Not allocate spRedInt"); Close(); }
	  spot[i].len=spot[i].x=spot[i].y=0;spot[i].greenInt=0.0;
	  spot[i].closed=false; spot[i].tagSpNum=-1;spot[i].dist=1000;
	  spot[i].good=true;   spot[i].selected=false;
	  for ( int j=0; j < 10000; j++ )
		spot[i].redInt[j]=spot[i].spInt[j]=spot[i].cX[j]=spot[i].cY[j]=0.0;
	}
  for ( int i=0; i < 150; i++ )
	{ filo[i].x=new float[10001];filo[i].y=new float[10001];filo[i].z=new float[10001];
	  filo[i].azi=new float[10001];filo[i].ele=new float[10001];
	  if ( filo[i].x == NULL || filo[i].y == NULL || filo[i].z == NULL )
		{ ShowMessage("Filopodia Data can not be allocated"); Close(); }
	  if ( filo[i].azi == NULL || filo[i].ele == NULL )
		{ ShowMessage("Filopodia Data can not be allocated"); Close(); }
	}
  spotNum=membAnum;
  if ( membAnum > MAX_SPOTS ) spotNum=MAX_SPOTS;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{ abort=true; gain=0;  }
//---------------------------------------------------------------------------
void TMainForm::CalculateDimensions(void)
{ matrixX=cellXvox=cellXnm/nmVox; matrixY=cellYvox=cellYnm/nmVox;
  matrixZ=cellZvox=cellZnm/nmVox; // matrix sizes
  matrixX+=(sideGapNm*2)/nmVox;  matrixY+=(sideGapNm*2)/nmVox; // add side space
  matrixZ+=(topGapNm/nmVox)+(botGapNm/nmVox);tubeLenVox=tubeLengthNm/nmVox;// add top and bottom
  if ( filopodia || lamella ) matrixX+=tubeLenVox;  // add size of filopodia
  matrixXY=matrixX*matrixY;
  matrixSize=matrixX; matrixSize*=matrixY; matrixSize*=matrixZ;
  matrixXnm=matrixX*nmVox; matrixYnm=matrixY*nmVox; matrixZnm=matrixZ*nmVox;
  sizeXpix=matrixXnm/nmPix; sizeYpix=matrixYnm/nmPix;sizeZpix=matrixZnm/nmPix;
  if ( twoCam ) sizeXpix*=2; // two cameras
  tubeGapVox=tubeGapNm/nmVox;
  sideGapVox=sideGapNm/nmVox; botGapVox=botGapNm/nmVox; topGapVox=topGapNm/nmVox;
  tubeRadVox=tubeRadNm/nmVox;  poreRadVox=poreRadNm/nmVox;
  tubeZminVox=botGapVox+tubeGapVox+tubeRadVox;
  tubeZmaxVox=botGapVox+cellZvox-tubeRadVox-tubeGapVox;
  if ( tubeZmaxVox > tubeZminVox+tubeRadVox*4 ) tubeZmaxVox=tubeZminVox+tubeRadVox*4;
}
//---------------------------------------------------------------------------
bool TMainForm::MakeBMPshape(void) // use BMP image as cell footprint and build 3D shell
{ verticesReady=membOK=false; Canvas->Font->Size=20;
  Canvas->TextOut(20,20,"Setting Matrix from BMP => WAIT      ");
  SetMembVertices->Enabled=false; MembFromVertices->Enabled=false;
  // zerroing matrix to Extracell
  for ( __int64 i=0; i < matrixSize; i++ ) matrix[i]=0;
  int top=matrixZ-topGapVox,right=matrixX-sideGapVox,down=matrixY-sideGapVox;
  if (filopodia ) right-=tubeLenVox;
  float xRatio=(float)bmpWidth/(float)(right-sideGapVox),
  yRatio=(float)bmpHeight/(float)(down-sideGapVox);
  bool inside=true;
  for ( __int64 z=botGapVox,bmpX,bmpY; z < top; z++ )
	for ( __int64 y=sideGapVox,bY=0; y < down; y++,bY++ )
	  for ( __int64 x=sideGapVox,bX=0; x < right; x++,bX++ )
		{ bmpX=bX*xRatio; bmpY=bY*yRatio;
		  if ( bmpBuf[bmpX+(bmpY*bmpWidth)] > 20 )
			matrix[x+(y*matrixX)+(z*matrixXY)]=CELL_MEMB;
		}
  for ( __int64 z=botGapVox; z < top; z++ )
	for ( __int64 y=1; y < matrixY-2; y++ )
	  for ( __int64 x=1; x < matrixX-2; x++ )
		if ( matrix[x+(y*matrixX)+(z*matrixXY)] == CELL_MEMB )
		  { inside=true;
			for ( __int64 zz=z-1; zz < z+2; zz++ )
			  for ( __int64 yy=y-1; yy < y+2; yy++ )
				for ( __int64 xx=x-1; xx < x+2; xx++ )
				  if ( matrix[xx+(yy*matrixX)+(zz*matrixXY)] == 0 )
					{ inside=false; break; }
			if ( inside ) matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
		  }
  Canvas->TextOut(20,20,"Matrix From BMP file ---- DONE ----     ");
  return true; // READY for running
}
//----------------------------------------------------------------------------
void TMainForm::TubeFission(int rad,int cXvox,int cYvox,int cZvox,float stage)
{ int dX,dY,dZ,rrr;
  int beg=cXvox-(tubeLenVox*0.5),end=cXvox+(tubeLenVox*0.5);
  if ( stage > 1.0 ) stage=1.0;
  for ( __int64 x=beg; x < end; x++ )
	for ( __int64 y=cYvox-rad-2; y < cYvox+rad+2; y++ )
	  for ( __int64 z=cZvox-rad-2; z < cZvox+rad+2; z++ )
		{ dY=cYvox-y; dZ=cZvox-z;
		  rrr=sqrt(dY*dY+dZ*dZ);  // round end
		  if ( x < beg+rad ) dX=((beg+rad)-x); // left beg
		  if ( x > end-rad ) dX=((end-rad)-x);
		  if ( x > beg+rad && x < cXvox )  dX=((beg+rad)-x)*stage;
		  if ( x < end-rad && x >= cXvox ) dX=((end-rad)-x)*stage;
		  rrr=sqrt(dY*dY+dZ*dZ+dX*dX);
		  matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM; // replace old structure
		  if ( rrr < rad )   matrix[x+(y*matrixX)+(z*matrixXY)]=VESI_MEMB;
		  if ( rrr < rad-3 ) matrix[x+(y*matrixX)+(z*matrixXY)]=T_LUMEN;
	   }
}
//----------------------------------------------------------------------------
bool TMainForm::FindFuseVoxel(int ind)
{ float cX=vesicle[ind].xVox,cY=vesicle[ind].yVox,cZ=vesicle[ind].zVox;
  int rad=vesicle[ind].radNm/nmVox,minDist=1000000000;
  __int64 absX,absY,absZ; bool contact=false;
  for ( int x=-rad-5,dist,dist2; x <= rad+5; x++ )  // check for obsticles
	for ( int y=-rad-5; y <= rad+5; y++ )
	  for ( int z=-rad-5; z <= rad+5; z++ )
		{ dist2=((x*x)+(y*y)+(z*z)); dist=sqrt(dist2);
		  if ( dist > rad+6 || dist < rad ) continue;
		  absX=cX+x; absY=cY+y; absZ=cZ+z;
		  if ( absX < 0 || absY < 0 || absZ < 0 || absX >= matrixX ||
			   absY >= matrixY || absZ >= matrixZ ) continue;  // check array
		  if ( matrix[absX+(absY*matrixX)+(absZ*matrixXY)] == CELL_MEMB && dist2 <= minDist )
			{ vesicle[ind].azimuth=atan2(y,x)*radToDeg+180.0;
			  vesicle[ind].elevation=atan2(z,sqrt(x*x+y*y))*radToDeg-90.0;
			  minDist=dist2; contact=true;
			}
		}
  return contact;
}
//---------------------------------------------------------------------------
void TMainForm::GenerateVesicle(int ind)
{ if ( !vesicle[ind].empty ) return;
  int cX=vesicle[ind].iniXvox,cY=vesicle[ind].iniYvox,cZ=vesicle[ind].iniZvox;
  float dX,dY,dZ,dist,cXnm=cX*nmVox+nmVox*0.5,cYnm=cY*nmVox+nmVox*0.5,cZnm=cZ*nmVox+nmVox*0.5;
  int vRad=vesRadNm/nmVox,tRad=tubeRadVox,nX,nY,nZ,span; span=tRad+vRad;
  nX=random(span*2)-span+cX; nY=random(span*2)-span+cY;nZ=random(span*2)-span+cZ;
  if ( !CheckSphere(vRad,nX,nY,nZ,CYTOPLASM,CYTOPLASM,CYTOPLASM) ) return; // space occupied
  FillSphere(vRad,nX,nY,nZ,VESI_MEMB,V_LUMEN,V_LUMEN);
  for ( int i=0; i < cytoAnum; i++ ) // move CytoA from tube to vesicle
	{ if ( CytoA[i].vesicleInd > -1 ) continue; // already in vesicle
	  dX=CytoA[i].x-cXnm; dY=CytoA[i].y-cYnm; dZ=CytoA[i].z-cZnm;
	  if ( sqrt(dX*dX+dY*dY+dZ*dZ) > tubeRadNm+nmVox ) continue;
	  CytoA[i].vesicleInd=ind; CytoA[i].medium=V_LUMEN;
	  CytoA[i].x=nX*nmVox;CytoA[i].y=nY*nmVox;CytoA[i].z=nZ*nmVox; // vesicle center
	  MoveToNearestVoxel(&CytoA[i],vRad,CytoA[i].medium); // alloc to lumen
	}
  for ( int i=0; i < membAnum; i++ ) // move MembA from tube to vesicle
	if ( MembA[i].vesicleInd < 0 )
	  { dX=MembA[i].x-cXnm;dY=MembA[i].y-cYnm; dZ=MembA[i].z-cZnm;
		if ( sqrt(dX*dX+dY*dY+dZ*dZ) < tubeRadNm+nmVox )
		  { MembA[i].vesicleInd=ind; MembA[i].medium=VESI_MEMB;
			MembA[i].x=nX*nmVox;MembA[i].y=nY*nmVox;MembA[i].z=nZ*nmVox; // vesicle center
			MoveToNearestVoxel(&MembA[i],vRad+2,MembA[i].medium); // alloc to membrane
		  }
	   }
	vesicle[ind].x=nX*nmVox;vesicle[ind].y=nY*nmVox;vesicle[ind].z=nZ*nmVox;
	vesicle[ind].radNm=vesRadNm; vesiGen++;
	vesicle[ind].xVox=nX;vesicle[ind].yVox=nY;vesicle[ind].zVox=nZ;
	vesicle[ind].empty=vesicle[ind].docked=vesicle[ind].fused=false;
}
//--------------------------------------------------------------------------- ALL UNITS ARE VOXELS ----------------------------
void TMainForm::FillSphere(int rad,int cX,int cY,int cZ,int shell,int lumen,int noGo)
{ __int64 absX,absY,absZ;  int val=0;
  for ( int x=-rad-1,dist; x <= rad+1; x++ )  //
	for ( int y=-rad-1; y <= rad+1; y++ )
	  for ( int z=-rad-1; z <= rad+1; z++ )
		{ absX=cX+x;absY=cY+y;absZ=cZ+z; dist=sqrt((x*x)+(y*y)+(z*z));
          if ( absX < 0 || absY < 0 || absZ < 0 || absX >= matrixX ||
			   absY >= matrixY || absZ >= matrixZ ) continue; // check limits
		  val=matrix[absX+(absY*matrixX)+(absZ*matrixXY)];
		  if ( dist <= rad && val != noGo && val != lumen )
			matrix[absX+(absY*matrixX)+(absZ*matrixXY)]=shell;
		  if ( dist < rad-1 && val != noGo )
			matrix[absX+(absY*matrixX)+(absZ*matrixXY)]=lumen;
		}
}
//----------------------------------------------------------------------------
void TMainForm::Fuse(int ind)
{ float cX=vesicle[ind].xVox,cY=vesicle[ind].yVox,cZ=vesicle[ind].zVox;
  int rad=vesicle[ind].radNm/nmVox,poreR=poreRadVox,delMed=CYTOPLASM;
  int len=rad+poreR*2,voxVal,medium=VESI_MEMB,noGo=EXTRACELL,cytoMed=EXTRACELL;
   __int64 absX,absY,absZ;
  if ( outsideVes )  { cytoMed=noGo=CYTOPLASM;delMed=0; } // for construction and medium
  if ( transfer ) len=0;
 /*if ( ind == 1 ) { poreR=25; len=rad+50; }  // for illustration purpose
   if ( ind == 2 ) { poreR=100; len=rad+200; }
   if ( ind == 3 ) len=0;
 */
  if ( !transfer ) // fill vesicle with correct fluid medium
	FillSphere(rad,cX,cY,cZ,VESI_MEMB,cytoMed,CELL_MEMB);
  for ( int i=0; i < len; i++ )  // make a pore
	{ cX+=SIN(vesicle[ind].elevation)*COS(vesicle[ind].azimuth);
	  cY+=SIN(vesicle[ind].azimuth)*SIN(vesicle[ind].elevation);
	  cZ+=COS(vesicle[ind].elevation);
	  FillSphere(poreR,cX,cY,cZ,VESI_MEMB,cytoMed,noGo);
	}
  if ( transfer )
	{ len=rad*3; medium=CELL_MEMB;
	  for ( int i=0; i < cytoAnum; i++ ) // move
		if ( CytoA[i].vesicleInd == ind ) MoveToNearestVoxel(&CytoA[i],len+rad,cytoMed);
	}
  for ( int i=0; i < cytoAnum; i++ ) // correct medium type and NO vesicleInd
	if ( CytoA[i].vesicleInd == ind )
	  { CytoA[i].medium=cytoMed; CytoA[i].vesicleInd=-1; }
  for ( int i=0; i < membAnum; i++ ) // move membA out of the pore
	if ( MembA[i].vesicleInd == ind )
	  { MoveToNearestVoxel(&MembA[i],len,medium);MembA[i].medium=medium;
		MembA[i].vesicleInd=-1;
	  }
  for ( int i=0; i < membBnum; i++ )   // move membB out of the pore
	if ( MembB[i].vesicleInd == ind )
	  MoveToNearestVoxel(&MembB[i],len,medium);
  for ( int i=0; i < membCnum; i++ )   // move membC out of the pore
	if ( MembC[i].vesicleInd == ind )
	  MoveToNearestVoxel(&MembC[i],len,medium);
  vesicle[ind].fused=true;  vesicle[ind].empty=true;  /// trail
  if ( transfer )  // remove vesicle
	{ FillSphere(rad,cX,cY,cZ,delMed,delMed,delMed);vesicle[ind].empty=true; }
}
//----------------------------------------------------------------------------
void TMainForm::MoveVesicle(int ind)
{ __int64 absX,absY,absZ; bool freeToGo=true;
  if ( ind == -10 )   // change to 0 to work as FISSION
   { int rad=tubeRadVox,dist=interTubeDistNm/nmVox,dX,dY,dZ,rrr;
	 int cXvox=vesicle[ind].x/nmVox,cYvox=vesicle[ind].y/nmVox,cZvox=vesicle[ind].z/nmVox;
	 int beg=cXvox-(tubeLenVox*0.5),end=cXvox+(tubeLenVox*0.5);
	 if ( Fission->Checked ) fissionCoef+=0.0001;
	 if ( fissionCoef > 1.0 ) { fissionCoef=1.0; return; }
	  Caption=fissionCoef;
	 TubeFission(rad,cXvox,cYvox,cZvox,fissionCoef);
	 for ( int i=0; i < membAnum; i++ )
		if ( MembA[i].vesicleInd == ind ) // move membrane objects
		  { absX=MembA[i].x/nmVox; absY=MembA[i].y/nmVox;absZ=MembA[i].z/nmVox;
			if ( matrix[absX+(absY*matrixX)+(absZ*matrixXY)] != VESI_MEMB )
				MoveToNearestVoxel(&MembA[i],10,VESI_MEMB);
		  }
	 return;
   }
   if ( vesicle[ind].empty ) return;  // still not in use
  if ( vesicle[ind].docked ) // no movements
	{ if ( !vesicle[ind].fused && time > vesicle[ind].dockTime+1.0 ) Fuse(ind);
	  return;
	}
  int radVox=vesicle[ind].radNm/nmVox,gap=10;
  double jump=JumpSizeNm(tmStep,vesicleMob),minDist=100000000.0;
  float dX=GaussRand()*jump,dY=GaussRand()*jump,dZ=GaussRand()*jump,radNm=vesicle[ind].radNm;
  float newXnm=vesicle[ind].x+dX,newYnm=vesicle[ind].y+dY,newZnm=vesicle[ind].z+dZ;
  if ( newXnm < radNm+gap || newYnm < radNm+gap || newZnm < radNm+gap )
	return;   // check borders - no movemement
  if ( newXnm+radNm > matrixXnm-gap || newYnm+radNm > matrixYnm-gap ||
	   newZnm+radNm > matrixZnm-gap )   // check borders - no movemement
	return;
  int newXvox=newXnm/nmVox,newYvox=newYnm/nmVox,newZvox=newZnm/nmVox,medium=CYTOPLASM;
  if ( newXvox == vesicle[ind].xVox && newYvox == vesicle[ind].yVox && newZvox == vesicle[ind].zVox )
	{ vesicle[ind].x=newXnm; vesicle[ind].y=newYnm; vesicle[ind].z=newZnm;
	  return; // no movement in matrix space => return
	}
  if ( outsideVes ) medium=EXTRACELL;// remove vesicle from matrix
  FillSphere(radVox,vesicle[ind].xVox,vesicle[ind].yVox,vesicle[ind].zVox,medium,medium,CELL_MEMB);
  for ( int x=-radVox-3,dist; x <= radVox+3; x++ )  // check for obsticles
	for ( int y=-radVox-3; y <= radVox+3; y++ )
	  for ( int z=-radVox-3; z <= radVox+3; z++ )
		{ dist=sqrt((x*x)+(y*y)+(z*z));
		  if ( dist > radVox+3 || dist < radVox-1 ) continue; //
		  absX=newXvox+x; absY=newYvox+y; absZ=newZvox+z;
		  if ( matrix[absX+(absY*matrixX)+(absZ*matrixXY)] > 90 )
			{ freeToGo=false; break; }
		}
  if ( freeToGo ) //
	{ vesicle[ind].x=newXnm;vesicle[ind].y=newYnm;vesicle[ind].z=newZnm;
	  dX=(newXvox-vesicle[ind].xVox)*nmVox;dY=(newYvox-vesicle[ind].yVox)*nmVox;
	  dZ=(newZvox-vesicle[ind].zVox)*nmVox;
	  for ( int i=0; i < membAnum; i++ )
		if ( MembA[i].vesicleInd == ind ) MoveObj(&MembA[i],dX,dY,dZ);
	  for ( int i=0; i < membBnum; i++ )
		if ( MembB[i].vesicleInd == ind ) MoveObj(&MembB[i],dX,dY,dZ);
	  for ( int i=0; i < membCnum; i++ )
		if ( MembC[i].vesicleInd == ind ) MoveObj(&MembC[i],dX,dY,dZ);
	  for ( int i=0; i < cytoAnum; i++ )
		if ( CytoA[i].vesicleInd == ind ) MoveObj(&CytoA[i],dX,dY,dZ);
	  vesicle[ind].xVox=newXvox,vesicle[ind].yVox=newYvox,vesicle[ind].zVox=newZvox;
	}
  FillSphere(radVox,vesicle[ind].xVox,vesicle[ind].yVox,vesicle[ind].zVox,VESI_MEMB,V_LUMEN,CELL_MEMB);
  if ( fuseVesicles && !freeToGo )  // if fusion is ON and vesiclee close to something
	if ( FindFuseVoxel(ind) ) // if correct CELL_MEMB voxel found start docking
	  { vesicle[ind].docked=true; vesicle[ind].dockTime=time; }
}
//--------------- units in voxels ----- for mitochondria septas -------------
void TMainForm::MakeVGap(int xO,int yO,int zO,int rad,int gap,int lumen,int memb)
{ for ( __int64 y=yO; y < yO+rad+2; y++ ) // fill the gap with membrane voxels
	for ( __int64 z=zO; z < zO+rad*2; z++ )
	  for ( __int64 x=xO; x < xO+gap; x++ )
		if ( matrix[x+(y*matrixX)+(z*matrixXY)] == lumen )
		  matrix[x+(y*matrixX)+(z*matrixXY)]=memb;
  for ( __int64 y=yO+1; y < yO+rad; y++ ) // make a gap
	for ( __int64 z=zO; z < zO+rad*2; z++ )
	  for ( __int64 x=xO+1; x < xO+gap-1; x++ )
		matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
}
//--------------- units in voxels --------- for mitochondria septas -----------
void TMainForm::MakeHGap(int xO,int yO,int zO,int rad,int gap,int lumen,int memb)
{ for ( __int64 y=yO; y < yO+rad*2; y++ ) // fill the gap with membrane voxels
	for ( __int64 z=zO; z < zO+rad; z++ )
	  for ( __int64 x=xO; x < xO+gap; x++ )
		if ( matrix[x+(y*matrixX)+(z*matrixXY)] == lumen )
		  matrix[x+(y*matrixX)+(z*matrixXY)]=memb;
  for ( __int64 y=yO; y < yO+rad*2; y++ ) // make a gap
	for ( __int64 z=zO+1; z < zO+rad-1; z++ )
	  for ( __int64 x=xO+1; x < xO+gap-1; x++ )
		matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
}
//--------------- units in voxels ----- check space for vesicle constraction ---
bool TMainForm::CheckSphere(int rad,int cX,int cY,int cZ,int med1,int med2,int med3)
{ if ( cX-rad-3 < 0 || cY-rad-3 < 0 || cZ-rad-3 < 0 ) return false;
  if ( cX+rad+4 > matrixX || cY+rad+4 > matrixY || cZ+rad+4 > matrixZ ) return false;
  __int64 absX,absY,absZ; int value;
  for ( int x=-rad-2,dist; x < rad+2; x++ )  // check for obsticles
	for ( int y=-rad-2; y < rad+2; y++ )
	  for ( int z=-rad-2; z < rad+2; z++ )
		{ dist=sqrt((x*x)+(y*y)+(z*z));
		  if ( dist < rad+2 && dist > rad-2 )
			{ absX=cX+x; absY=cY+y; absZ=cZ+z;
			  value=matrix[absX+(absY*matrixX)+(absZ*matrixXY)];
			  if ( value!= med1 && value!= med2 && value!= med3 ) return false;
			}
		}
  return true;
}
//----------   find TUBE_MEMB voxel in the direction of grouth (cone) ------
bool TMainForm::FindTube(int ddd,int x,int y,int z,float azi,float ele,float *dAzi,float *dEle)
{ int dist=tubeRadVox*4,rad,min=100000; float rAzi,rEle; __int64 xx,yy,zz; bool OK=false;
  for ( int i=0; i < 100; i++ )
	{ rAzi=random(20)-10;rEle=random(20)-10; rad=random(dist)+tubeRadVox*2;
	  xx=COS(rAzi+azi)*COS(rEle+ele)*rad+x;yy=SIN(rAzi+azi)*COS(rEle+ele)*rad+y;
	  zz=SIN(rEle+ele)*rad+z;
	  if ( xx < 1 || yy < 1 || zz < 1 || xx > matrixX-2 || yy > matrixY-2 || zz > matrixZ-2 )
	    continue; // out of borders
	  if ( matrix[xx+(yy*matrixX)+(zz*matrixXY)] == TUBE_MEMB && rad < min )
		{ min=rad; *dAzi=rAzi; *dEle=rEle; OK=true; }
	}
   if ( OK ) return true;
   return false;
}
//------------  ALL UNITS ARE VOXELS  -------------------------------
void TMainForm::MakeTubeNetwork(int rad)
{ double azi,ele,tubeX,tubeY,tubeZ,newZ,m1,m2; float dAzi,dEle;
  double dX1=0.0,dX2=0.0,dY1=0.0,dY2=0.0,dZ1=0.0,dZ2=0.0,avCos=0.0,avLen=0.0,pL=0.0;
  for ( int i=0,medium; i < vesNum; i++ )
	{ tubeZ=tubeZminVox; azi=random(360);ele=0.0; dX2=dY2=dZ2=0.0;
	  if (tube3D ) tubeZ=tubeZminVox+(tubeZmaxVox-tubeZminVox)*0.5;
	  for ( int att=0; att < 210; att++ )
		{ tubeX=random(matrixX); tubeY=random(matrixY);
		  if ( CheckSphere(rad+6,tubeX,tubeY,tubeZ,T_LUMEN,TUBE_MEMB,CYTOPLASM) )
			{ vesicle[i].iniXvox=tubeX;vesicle[i].iniYvox=tubeY;
			  vesicle[i].iniZvox=tubeZ; vesicle[i].empty=true; break;
			}
		  if ( att > 200 ) { ShowMessage("Cell is too crowded"); return; }
		} // check space and build tube
	  note="Making Tube ";note+=i;note+=",           WAIT    ";
	  Canvas->TextOut(20,20,note); Application->ProcessMessages();
	  while( CheckSphere(rad+3,tubeX,tubeY,tubeZ,T_LUMEN,TUBE_MEMB,CYTOPLASM) )
		{ FillSphere(rad,tubeX,tubeY,tubeZ,TUBE_MEMB,T_LUMEN,CELL_MEMB);
		  dX1=dX2;dY1=dY2;tubeX+=dX2=COS(azi)*COS(ele);
		  tubeY+=dY2=SIN(azi)*COS(ele); azi+=RandOne()*bendTilt;
		  if (tube3D )  // meandring in Z
			{ ele+=RandOne()*bendTilt*0.05;
			  if ( tubeZ < tubeZminVox+10 && ele < 0.0 ) ele+=1.0; // avoid top/bottom
			  if ( tubeZ > tubeZmaxVox-10 && ele > 0.0 ) ele-=1.0;
			  dZ1=dZ2; dZ2=SIN(ele); newZ=tubeZ+dZ2;
			  if ( newZ > tubeZminVox && newZ < tubeZmaxVox ) tubeZ=newZ;
			  if ( FindTube(4,tubeX,tubeY,tubeZ,azi,ele,&dAzi,&dEle) ) //???????
				{ azi+=dAzi*bendTilt*0.02; ele+=dEle*bendTilt*0.05; }
			}                   // angle between two vectors
		  m1=sqrt(dX1*dX1+dY1*dY1+dZ1*dZ1); m2=sqrt(dX2*dX2+dY2*dY2+dZ2*dZ2);
		  if (m1 > 0.0 && m2 > 0.0 )  // ang=acos(a*b / |a|*|b|)
			avCos+=cos(acos((dX1*dX2+dY1*dY2+dZ1*dZ2)/(m1*m2)));//)/(PI/180.0);
		  avLen+=sqrt(dX1*dX1+dY1*dY1+dZ1*dZ1);
		}
	// raft where tube begins for future vesicle formation
	 if ( recycling )
	   FillSphere(rad,vesicle[i].iniXvox,vesicle[i].iniYvox,vesicle[i].iniZvox,RAFT_MEMB,T_LUMEN,T_LUMEN);
	}
}
//------------  ALL UNITS ARE VOXELS  -------------------------------
void TMainForm::MakeVesicle(int cX,int cY,int cZ,int rad,float shape,byte memb,byte lumen)
{ float dX,dY,dZ,dist,azi,pol; int thick=2+shape/2,pRad=poreRadVox;
  for ( __int64 z=cZ-rad/shape; z < cZ+rad/shape; z++ )
	for ( __int64 y=cY-rad; y < cY+rad; y++ )
	  for ( __int64 x=cX-rad; x < cX+rad; x++ )
		{ dX=(x-cX); dY=y-cY; dZ=(z-cZ)*shape;
		  dist=sqrt(dX*dX+dY*dY+dZ*dZ);
		  if ( dist <= rad )       matrix[x+(y*matrixX)+(z*matrixXY)]=memb;
		  if ( dist <= rad-thick ) matrix[x+(y*matrixX)+(z*matrixXY)]=lumen;
		}
/* for ( __int64 i=0,azi,ele,x,y,z; i < 10000; i++ )
	 { azi=random(360); ele=random(360);
	   x=cX+(COS(azi)*COS(ele)*rad);y=cY+(SIN(azi)*COS(ele)*rad);z=cZ+SIN(ele)*rad;
	   matrix[x+(y*matrixX)+(z*matrixXY)]=memb;
	 }
*/
 /* make pores in membrane
 for ( int num=0; num < vesNum; num++ )
   { dX=cX,dY=cY,dZ=cZ; azi=random(360); pol=random(360);
	 for ( int i=0; i < rad; i++ )
	   { dX+=SIN(azi)*COS(pol); dY+=SIN(pol)*SIN(azi); dZ+=COS(azi);
		 if ( dZ < botGapVox+pRad+2 || dZ > botGapVox+cellZvox-2 ) break;
		 FillSphere(pRad,dX,dY,dZ,memb,lumen,CYTOPLASM);
	   }
   } */
}
//------------  ALL UNITS ARE VOXELS  -------------------------------
void TMainForm::FiloDynamics(int rad)
{ float filoX,filoY,filoZ,azi,ele,step=statApitchNm/nmVox;
  for ( int i=0,j=0; i < vesNum; i++,j=0 )
	{ while ( filo[i].x[j+1] > 0.0 && j < 9990 ) j++;
	  filoX=filo[i].x[j]/nmVox;filoY=filo[i].y[j]/nmVox;filoZ=filo[i].z[j]/nmVox;
	  azi=filo[i].azi[j]; ele=filo[i].ele[j];
	  filoX+=COS(azi)*COS(ele)*step; filoY+=SIN(azi)*COS(ele)*step;
	  filoZ+=SIN(ele)*step; 	     azi+=RandOne()*bendTilt;
	  if (tube3D ) ele+=RandOne()*bendTilt;
	  if ( filoX > matrixX-sideGapVox-rad ) continue; // check boarders
	  if ( filoY < sideGapVox+rad || filoY > matrixY-sideGapVox-rad ) continue;
	  if ( filoZ < botGapVox+rad || filoZ > matrixZ-topGapVox-rad ) continue;
	  FillSphere(rad,filoX,filoY,filoZ,CELL_MEMB,CYTOPLASM,CYTOPLASM);
	  j++;  filo[i].azi[j]=azi; filo[i].ele[j]=ele;
	  filo[i].x[j]=filoX*nmVox;filo[i].y[j]=filoY*nmVox;filo[i].z[j]=filoZ*nmVox;
	  filo[i].x[j+1]=filo[i].y[j+1]=filo[i].z[j+1]=0.0;
	}
  for ( int i=0; i < cytoAnum; i++ ) MoveToNearestVoxel(&CytoA[i],20,CytoA[i].medium);
  for ( int i=0; i < cytoBnum; i++ ) MoveToNearestVoxel(&CytoB[i],20,CytoB[i].medium);
  for ( int i=0; i < membAnum; i++ ) MoveToNearestVoxel(&MembA[i],20,MembA[i].medium);
  for ( int i=0; i < membBnum; i++ ) MoveToNearestVoxel(&MembB[i],20,MembB[i].medium);
  for ( int i=0; i < membCnum; i++ ) MoveToNearestVoxel(&MembC[i],20,MembC[i].medium);
}
//------------  ALL UNITS ARE VOXELS  -------------------------------
void TMainForm::MakeFilopodia(int rad)
{ int filletVox=bodyFilletNm/nmVox,firstX=sideGapVox+cellXvox;
  int minY=sideGapVox+rad,maxY=matrixY-sideGapVox-rad,maxX=matrixX-sideGapVox-rad;
  float filoX,filoY,filoZ,firstZ=botGapVox+rad,azi,ele,step=statApitchNm/nmVox;
  if ( roundedCellBody )
	{ firstX-=filletVox; minY+=filletVox; maxY-=filletVox; }
   if (tube3D ) firstZ+=cellZvox*0.5;
  for ( int i=0; i < vesNum; i++ )
	{ filoX=firstX;filoY=random(maxY-minY)+minY;filoZ=firstZ; azi=0.0; ele=0.0;
	  for ( int j=0; j < 10000; j++ )
		{ // if ( CheckSphere(rad+3,filoX,filoY,filoZ,T_LUMEN,TUBE_MEMB,CYTOPLASM) )
		  FillSphere(rad,filoX,filoY,filoZ,CELL_MEMB,CYTOPLASM,CYTOPLASM);
		  filo[i].x[j]=filoX*nmVox;filo[i].y[j]=filoY*nmVox;filo[i].z[j]=filoZ*nmVox;
		  filo[i].x[j+1]=filo[i].y[j+1]=filo[i].z[j+1]=0.0;
		  filo[i].azi[j]=azi; filo[i].ele[j]=ele;
          filoX+=COS(azi)*COS(ele)*step; filoY+=SIN(azi)*COS(ele)*step;
		  filoZ+=SIN(ele)*step; 	     azi+=RandOne()*bendTilt;
		  if (tube3D ) ele+=RandOne()*bendTilt;
		  if ( filoX < firstX || filoX > maxX ) break; // check boarders
		  if ( filoY < sideGapVox+rad || filoY > matrixY-sideGapVox-rad ) break;
		  if ( filoZ < botGapVox+rad || filoZ > matrixZ-topGapVox-rad ) break;
		}
	}
}
//---------------------------------------------------------------------------
void TMainForm::MakeRing(float cX,float cY,float cZ,int rad,float azi,float ele)
{ double step=12.0/rad; __int64 xVox,yVox,zVox;
  double aX=1.0,aY=0.0,aZ=0.0,bX=0.0,bY=1.0,bZ=0.0;
  ele+=90.0;
  aX=SIN(ele)*COS(azi); aY=SIN(ele)*SIN(azi); aZ=COS(ele);
  azi+=90.0;
  bX=SIN(ele)*COS(azi); bY=SIN(ele)*SIN(azi); bZ=COS(ele);
  for ( int r=0; r <= rad; r++ )
	for ( float ang=0.0; ang < 380.0; ang+=step )
	  { xVox=r*COS(ang)*aX+r*SIN(ang)*bX+cX;  // wrong
		yVox=r*COS(ang)*aY+r*SIN(ang)*bY+cY;
		zVox=r*COS(ang)*aZ+r*SIN(ang)*bZ+cZ;
		if ( xVox < 0 || yVox < 0 || zVox < 0 ) continue; // check borders
		if ( xVox >= matrixX || yVox >= matrixY || zVox >= matrixZ ) continue;
		if ( r < rad-2 ) matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)]=CYTOPLASM;
		else           matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)]=VESI_MEMB;
	  }
}
//--------  ALL UNITS ARE VOXELS  -----------------------------
void TMainForm::MakeRoundedShell(int X1,int X2,int Y1,int Y2,int Z1,int Z2,int rad)
{ int cellTop=Z2,cellRight=Y2,cellDown=Y2;
  int x1=X1+rad,x2=X2-rad,y1=Y1+rad,y2=Y2-rad,z1=Z1+rad,z2=Z2-rad;
//  if ( filopodiaON ) { cellRight-=tubeLenVox;x2=cellRight-rad; }
  for ( __int64 z=Z1,dX,dY,dZ,dist; z <= Z2; z++ ) // make borders
	for ( __int64 y=Y1; y <= Y2; y++ )
	  for ( __int64 x=X1; x <= X2; x++ )
		if ( z < z1 || y < y1 || x < x1 || z > z2 ||  y > y2 || x > x2 ) // rounded parts of matrix
		   { dX=0; dY=0; dZ=0;
			 if ( z < z1 ) dZ=(z1-z)*0.99;
			 if ( z > z2 ) dZ=(z-z2)*0.99;
			 if ( y < y1 ) dY=(y1-y);
			 if ( y > y2 ) dY=(y-y2);
			 if ( x < x1 ) dX=(x1-x);
			 if ( x > x2 ) dX=(x-x2);
			 dist=sqrt(dX*dX+dY*dY+dZ*dZ);//-sqrt(dX*dX+dY*dY);
			 if ( dist < rad )   matrix[x+(y*matrixX)+(z*matrixXY)]=CELL_MEMB;
			 if ( dist < rad-1 && z != botGapVox && z != botGapVox+1 && z != cellTop )
			   matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
		   }
		else matrix[x+(y*matrixX)+(z*matrixXY)]=CELL_MEMB;
  for ( __int64 z=z1; z < z2+1; z++ ) // central cavity
	for ( __int64 y=y1; y < y2+1; y++ )
	  for ( __int64 x=x1; x < x2+1; x++ ) matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
}
//---------------------------------------------------------------------------
bool TMainForm::MakeBody(void)  // Make geometrical 3D shell
{ __int64 left=sideGapVox,right=matrixX-sideGapVox;
  if ( filopodia || lamella ) right-=tubeLengthNm/nmVox; // size of filopodia
  __int64 up=sideGapVox,down=matrixY-sideGapVox,lamTop=(2*tubeRadNm)/nmVox;
  __int64 bot=botGapVox,top=matrixZ-topGapVox,rCon=bodyFilletNm/nmVox;
  for ( __int64 i=0; i < matrixSize; i++ ) matrix[i]=EXTRACELL; // zerroing
  if ( MakeBMPshell->Checked ) MakeBMPshape();
  if ( roundedCellBody && !MakeBMPshell->Checked ) // rounded body
		{ MakeRoundedShell(sideGapVox,right,sideGapVox,
			  matrixY-sideGapVox,botGapVox,matrixZ-topGapVox,bodyFilletNm/nmVox);
		   /*	int shellX=matrixX/3-50,shellY=matrixY/2-50;  // 6 cell scenario
			 for ( int y=sideGapVox; y < matrixY-shellY; y+=shellY+8 )
		for ( int x=sideGapVox; x < matrixX-shellX; x+=shellX+8 )
		  MakeRoundedShell(x,x+shellX,y,y+shellY,botGapVox,matrixZ-topGapVox,bodyFilletNm/nmVox);
   */	}
   if ( !roundedCellBody && !MakeBMPshell->Checked ) // simple paralelipiped body
	 { for ( __int64 z=bot; z < top; z++ )
		 for ( __int64 y=up; y < down; y++ )
		   for ( __int64 x=left; x < right; x++ )
			 matrix[x+(y*matrixX)+(z*matrixXY)]=CELL_MEMB;
		 for ( __int64 z=bot+1; z < top-1; z++ ) // make cavity
		   for ( __int64 y=up+1; y < down-1; y++ )
			 for ( __int64 x=left+1; x < right-1; x++ )
			   matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
	   if ( lamella )
		 { for ( __int64 z=bot; z < lamTop; z++ )
			 for ( __int64 y=up; y < down; y++ )
			   for ( __int64 x=right; x < matrixX-sideGapVox; x++ )
				 matrix[x+(y*matrixX)+(z*matrixXY)]=CELL_MEMB;
		   for ( __int64 z=bot+1; z < lamTop-1; z++ ) // make cavity
			 for ( __int64 y=up+1; y < down-1; y++ )
			   for ( __int64 x=right-1; x < matrixX-sideGapVox-1; x++ )
				 matrix[x+(y*matrixX)+(z*matrixXY)]=CYTOPLASM;
		 }
     }
  if ( nucleus ) // MakeNucleus(matrix,nucleusRadNm/nmVox,flattening);
	MakeVesicle(sideGapVox+cellXvox/2,sideGapVox+cellYvox/2,botGapVox+cellZvox/2,
				nucleusRadNm/nmVox,flattening,NUCL_MEMB,NUCLEOPLASM);
  if ( filopodia ) MakeFilopodia(tubeRadVox); // set matrix for filopodia
  if ( tubeNetwork ) MakeTubeNetwork(tubeRadVox);
  if ( tubes )  // set Matrix for elongated vesicles
	{ int rad=tubeRadVox,dist=interTubeDistNm/nmVox,tubeZ=bot+tubeGapVox+rad;
	  int mid=(right-left)*0.5+left,axisY=up+dist/2,gap=poreRadNm/nmVox;
	  int beg=mid-(tubeLenVox*0.5)+rad,end=mid+(tubeLenVox*0.5)-rad;
	  float cZ=0.0;
	  for ( vesNum=0; axisY <= down-rad; axisY+=dist,vesNum++ )
		{ for ( __int64 x=beg; x <= end; x++ )
			{ cZ=(x-beg)*bendTilt;
			  FillSphere(rad,x,axisY,tubeZ+cZ,TUBE_MEMB,T_LUMEN,CELL_MEMB);
			}
		  if ( mitochondria )
			for ( __int64 x=beg-gap; x <= end; x+=gap*4 )
			  { cZ=(x-beg)*bendTilt;
				MakeVGap(x,axisY+5,tubeZ-rad+cZ,rad,gap,T_LUMEN,TUBE_MEMB);
				MakeVGap(x,axisY-5-rad,tubeZ-rad+cZ,rad,gap,T_LUMEN,TUBE_MEMB);
				MakeHGap(x+gap*2,axisY-rad,tubeZ+5+cZ+1,rad,gap,T_LUMEN,TUBE_MEMB);
				MakeHGap(x+gap*2,axisY-rad,tubeZ-5-rad+cZ+1,rad,gap,T_LUMEN,TUBE_MEMB);
			  }
		  vesicle[vesNum].x=mid*nmVox;vesicle[vesNum].y=axisY*nmVox;
		  vesicle[vesNum].z=(tubeZ+cZ)*nmVox; vesicle[vesNum].radNm=tubeRadNm;
		}
	}
  if ( movingVes && !recycling )  /// separate from static ALWAYS round + NO recycling
	{ int rad=vesRadNm/nmVox,dist,medium=CYTOPLASM,num=0;  bool noGo=false;
	  __int64 absX,absY,absZ,vesXvox,vesYvox,vesZvox;
	  for ( int i=0,vesXnm,vesYnm,vesZnm; i < vesNum; i++,num=i,noGo=false )
		{


		  for ( int t=0; t < 100; t++ )  /// try to find free space 100 times
			{ vesXnm=random(cellXnm-(vesRadNm*2))+sideGapNm+vesRadNm;
			  vesYnm=random(cellYnm-(vesRadNm*2))+sideGapNm+vesRadNm;
			  vesZnm=random(cellZnm-(vesRadNm*2))+botGapNm+vesRadNm;
			  if ( outsideVes && vesRadNm*2 < matrixZnm )
				{ vesZnm=matrixZnm-vesRadNm*2-nmVox*3; medium=EXTRACELL; }



			  vesXvox=vesXnm/nmVox; vesYvox=vesYnm/nmVox;vesZvox=vesZnm/nmVox;
			  if ( CheckSphere(rad+2,vesXvox,vesYvox,vesZvox,medium,medium,medium) )
				break;
			  if ( t >= 98 )
				{ ShowMessage("Can not allocate vesicle"); noGo=true; break; }
			}
		  if ( noGo ) break; //
	 /*  vesYnm=500+sideGapNm;  // For illustartion purpose
		  if ( i == 0 ) { vesXnm=200+sideGapNm;  vesZnm=125+botGapNm; }  // temp
		  if ( i == 1 ) { vesXnm=500+sideGapNm;  vesZnm=125+botGapNm; }  // temp
		  if ( i == 2 ) { vesXnm=800+sideGapNm;  vesZnm=0; }  // temp
		  vesXvox=vesXnm/nmVox; vesYvox=vesYnm/nmVox;vesZvox=vesZnm/nmVox;
	   */
		  vesicle[i].x=vesXnm; vesicle[i].y=vesYnm; vesicle[i].z=vesZnm;
		  vesicle[i].radNm=vesRadNm;
		  vesicle[i].xVox=vesXvox;vesicle[i].yVox=vesYvox;vesicle[i].zVox=vesZvox;
		  vesicle[i].docked=vesicle[i].fused=vesicle[i].empty=false;
		  FillSphere(rad,vesicle[i].xVox,vesicle[i].yVox,vesicle[i].zVox,VESI_MEMB,V_LUMEN,0/*CELL_MEMB*/);
		}
	  if ( num < vesNum ){ vesNum=num;AnsiString str=num;str+=" vesicles only"; }
	}
  Canvas->TextOut(80,20,"Setting cell body DONE      ");
  return true; // READY for running
}
//---------------------------------------------------------------------------
bool TMainForm::SetVertices(void)
{ Canvas->TextOut(20,20,"Set Vertices Coordinates          "); FormPaint(0);
  int index=0,nLim,curZ=0,first,last,layer2first,layer2last,layersNum=0;
  ProgBar->Visible=true;
  for ( int i=0; i < 10000; i++ ) vertZind[i]=0;
  for ( __int64 z=0; z < matrixZ && index < VERT_LIM; z++ )
	{ for ( __int64 y=0; y < matrixY && index < VERT_LIM; y++ )
		for ( __int64 x=0; x < matrixX && index < VERT_LIM; x++ )
		  if ( matrix[x+(y*matrixX)+(z*matrixXY)] > 100 ) // membr
			{ vert[index].x=x;vert[index].y=y;vert[index].z=z; // corrdinates
			  for ( int j=0; j < 15; j++ ) vert[index].nInd[j]=-1; // no index
			  if ( z > curZ ) { curZ=z; vertZind[z]=index; layersNum++; } // at z
			  vert[index].type=matrix[x+(y*matrixX)+(z*matrixXY)]; // type of membrane
			  index++;
			}
	  ProgBar->Position=(z*100)/matrixZ; Application->ProcessMessages();
	}
  vertNum=index; vertZind[curZ+1]=vertNum; // last layer
  if ( vertNum == 0 ) { ShowMessage("CRITICAL ERROR - No vertices"); return false; }
  if ( vertNum >= VERT_LIM ) { ShowMessage("ERROR - NOT ENOUGH VERTICES"); return false; }
  // connecting vetices
  Canvas->TextOut(80,20,"  Connecting vertices      ");
  for (int i=0; i < 3; i++ ) layer[i] = new int[matrixXY];  // create 3 layers
  if ( layer[0] == NULL || layer[1] == NULL || layer[2] == NULL )
	{ ShowMessage("Layer allocation ERROR"); return false; }
  for ( int i=0; i < matrixXY; i++ ) layer[0][i]=layer[1][i]=layer[2][i]=-1; // zerroing
  // before Search layer 2 filled with initial Z
  curZ=vert[0].z;   int maxCon=0;
  layer2first=vertZind[curZ];layer2last=vertZind[curZ+1];
  for ( int i=layer2first,xx,yy; i < layer2last; i++ )
	{ xx=vert[i].x; yy=vert[i].y; layer[2][xx+(yy*matrixX)]=i; }
  // main connecting cycle
  for ( int i=0; i < layersNum; i++,curZ++ )
	 { for ( int i=0; i < matrixXY; i++ ) // shift layers
		 { layer[0][i]=layer[1][i]; layer[1][i]=layer[2][i]; layer[2][i]=-1; }
	   layer2first=vertZind[curZ+1],layer2last=vertZind[curZ+2];
	   maxCon=0;
	   for ( int i=layer2first,xx,yy; i < layer2last; i++ ) // fill layer 2
		 { xx=vert[i].x; yy=vert[i].y; layer[2][xx+yy*matrixX]=i; }
	   // find first and last vertex in a layer
	   first=vertZind[curZ];last=vertZind[curZ+1]; //indexing
	   for ( int j=first,x,y; j < last; j++ )
		 { x=vert[j].x; y=vert[j].y; nLim=-1;
		   for ( int zz=0,ind; zz < 3; zz++ ) // search if cells around occupaied
			 for ( int yy=y-1; yy < y+2; yy++ )
			   for ( int xx=x-1; xx < x+2; xx++ )
				 { ind=layer[zz][xx+(yy*matrixX)];
				   if ( ind > -1 && ind != j && vert[j].type == vert[ind].type && nLim < 14 )
					 { nLim++; vert[j].nInd[nLim]=ind;
					   if ( nLim > maxCon ) maxCon=nLim;
					 }
				 }
		 }
	  //AnsiString str=i; str+=" layers Done ("; str+=first;str+="-";str+=last;str+=")";
	  //Canvas->TextOut(400,80,str);
	  ProgBar->Position=(i*100)/layersNum; Application->ProcessMessages();
	//  ShowMessage(maxCon);
	 }
   if ( vertNum > 1 ) { Rounding->Enabled=true; Thickening->Enabled=true;  }

  ProgBar->Visible=false;	return true; // READY for running
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MakeBMPshellClick(TObject *Sender)
{ MakeBMPshell->Checked=!MakeBMPshell->Checked; }
//-----------------  Set geometrical shapes -------------------------
void __fastcall TMainForm::SetMembClick(TObject *Sender)
{ Thickening->Enabled=verticesReady=membOK=false; Canvas->Font->Size=20;
  SetMembVertices->Enabled=false; MembFromVertices->Enabled=false;
  note="Making Geometrical Shapes => WAIT";
  Canvas->TextOut(20,20,"Making Geometrical Shapes => WAIT      ");
  Cursor=crHourGlass;
  SetMembVertices->Enabled=membOK=MakeBody();
  Thickening->Enabled=membOK; SetRandomRafts->Enabled=membOK;
  Canvas->TextOut(20,20,"Geometrical Matrix   ---- DONE ----             ");
  AnsiString str="Matix "; str+=(int)(matrixSize/1000000); str+="Mb";
  InfoBar->Panels->Items[0]->Text=str;  Canvas->Font->Size=8;Cursor=crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetMembVerticesClick(TObject *Sender)
{ Canvas->Font->Size=20; FormPaint(0);
  Canvas->TextOut(20,20,"Making Vertices               ");
  MembFromVertices->Enabled=verticesReady=SetVertices();
  Canvas->TextOut(20,20,"Vertices Ready                ");Canvas->Font->Size=8;
  AnsiString str="Matix "; str+=(int)(matrixSize/1000000); str+="Mb (";
  str+=vertNum; str+=" vert.)";
  InfoBar->Panels->Items[0]->Text=str;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MembFromVerticesClick(TObject *Sender)
{ verticesReady=membOK=true; Canvas->Font->Size=20;
  Canvas->TextOut(20,20,"Setting Matrix using Vetices => WAIT      ");
  for ( __int64 i=0; i < matrixSize; i++ )
	if ( matrix[i] == CELL_MEMB ) matrix[i]=EXTRACELL; // zerroing
  for ( __int64 i=0,x,y,z; i < vertNum; i++ )
	{ x=vert[i].x; y=vert[i].y; z=vert[i].z;
	  matrix[x+(y*matrixX)+(z*matrixXY)]=vert[i].type;
	}
  Canvas->TextOut(20,20,"Matrix from Vetices ----- DONE -----     "); Canvas->Font->Size=8;
}
//---------------------------------------------------------------------------
int TMainForm::NeighbXYZ(int index, double *x,double *y,double *z)
{ for ( int i=0; i < 15; i++ )
	{ if ( vert[index].nInd[i] < 0 ) return i;
	  *x+=vert[vert[index].nInd[i]].x;*y+=vert[vert[index].nInd[i]].y;
	  *z+=vert[vert[index].nInd[i]].z;
	}
  return 0;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RoundingClick(TObject *Sender)
{ double x,y,z,nNum=1.0;
  float *newX,*newY,*newZ;
  newX=new float[vertNum]; newY=new float[vertNum];newZ=new float[vertNum];
  if ( newX == NULL || newY == NULL || newZ == NULL )
	{ ShowMessage("Rounding Buffer Allocation ERROR"); return; }
  ProgBar->Visible=true;
  for ( int i=0; i < vertNum; i++,nNum=1.0 )
	{ //x=vert[i].x; y=vert[i].y; z=vert[i].z;
	  x=y=z=0.0;
	  if ( vert[i].nInd[0] < 0 ) { 	Beep();ShowMessage("No neib"); break; }
	   ///!!!!! ONLY neighb coordinates
	   x+=vert[vert[i].nInd[0]].x;y+=vert[vert[i].nInd[0]].y;
	   z+=vert[vert[i].nInd[0]].z;
	  // just 1 neighb
	  for ( int j=1; j < 15; j++ )
		{ if ( vert[i].nInd[j] < 0 ) break;  // no more neighb
		//  nNum+=NeighbXYZ(vert[i].nInd[j],&x,&y,&z);
		  x+=vert[vert[i].nInd[j]].x;y+=vert[vert[i].nInd[j]].y;
		  z+=vert[vert[i].nInd[j]].z;    nNum+=1.0;
		}   // averaged new position in temporary XYZ arrays
	  newX[i]=x/nNum; newY[i]=y/nNum; newZ[i]=z/nNum;
	  if ( !(i%1000) )
		{ ProgBar->Position=(i*100)/vertNum; }
	}
  for ( __int64 i=0; i < matrixSize; i++ )
	if ( matrix[i] == CELL_MEMB ) matrix[i]=EXTRACELL; // zerroing
  for ( __int64 i=0,xx,yy,zz; i < vertNum; i++ ) // set new vertices XYZ
	{ xx=vert[i].x=newX[i]; yy=vert[i].y=newY[i]; zz=vert[i].z=newZ[i];
	  matrix[xx+(yy*matrixX)+(zz*matrixXY)]=vert[i].type; // and fill matrix
	}
  ProgBar->Visible=false;
  FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ThickeningClick(TObject *Sender)
{ ProgBar->Visible=true; byte voxVal;
  for ( __int64 z=1; z < matrixZ-1; z++ )
	{ for ( __int64 y=1; y < matrixY-1; y++ )
		for ( __int64 x=1; x < matrixX-1; x++ )  // except moving vesicles
		  if ( (voxVal=matrix[x+(y*matrixX)+(z*matrixXY)]) > TUBE_MEMB )
			{ for ( __int64 zz=z-1; zz < z+2; zz++ )
				for ( __int64 yy=y-1; yy < y+2; yy++ )
				  for ( __int64 xx=x-1; xx < x+2; xx++ )
					if ( matrix[xx+(yy*matrixX)+(zz*matrixXY)] < 200 )
					  matrix[xx+(yy*matrixX)+(zz*matrixXY)]=voxVal-100;
			}
	   ProgBar->Position=(z*100)/matrixZ;
	}
  for ( __int64 i=0; i < matrixSize; i++ )
    if ( matrix[i] < 200 && matrix[i] > 100 ) matrix[i]+=100;
  ProgBar->Visible=false; FormPaint(Sender);
}
//---------------------------------------------------------------------------
int TMainForm::SetObjectProps(Obj *obj,int mType,int greenNum,int greenFrac,int redNum,int redFrac,int num)
{ __int64 x,y,z; int failed=0;
  for ( int i=0,xx,yy,zz,con=0; i < num; i++ )
	{ obj[i].membAind=obj[i].membBind=obj[i].membCind=obj[i].statAind=-1;
	  obj[i].siteAind=obj[i].siteBind=obj[i].cytoAind=obj[i].cytoBind=-1;
	  obj[i].azi=obj[i].ele=0.0;obj[i].vesicleInd=-1;obj[i].medium=mType;
	  for ( int j=0; j < 4; j++ ) obj[i].green[j]=obj[i].red[j]=false;
	  for ( int j=0; j < greenNum; j++ )
		if ( random(100) < greenFrac ) obj[i].green[j]=true;
	  for ( int j=0; j < redNum; j++ )
		if ( random(100) < redFrac ) obj[i].red[j]=true;
	  if ( mType == V_LUMEN || mType == VESI_MEMB ) continue; // set xyz later
	  for ( int j=0; j < 10010; j++ ) // random positions at mType voxels
		{ xx=random(matrixXnm);yy=random(matrixYnm);zz=random(matrixZnm);
		  x=xx/nmVox,y=yy/nmVox,z=zz/nmVox;
		  if ( matrix[x+y*matrixX+z*matrixXY] == mType )
			{ obj[i].x=xx; obj[i].y=yy; obj[i].z=zz; break; }
		  if ( j > 9999 ) { failed++;obj[i].x=obj[i].y=obj[i].z=0.0; break; }
		}
	}
  return failed;
}
//---------------------------------------------------------------------------
void TMainForm::ObjToVes(Obj *obj,int num,int shift)
{  for ( int i=0,ind=0; i < num; i++ )
	 { obj[i].x=vesicle[ind].x; obj[i].y=vesicle[ind].y;
	   obj[i].z=vesicle[ind].z+shift; obj[i].vesicleInd=ind;
	   if ( (++ind) >= vesNum ) ind=0;
	 }
}
//---------------------------------------------------------------------------
void TMainForm::SetCell(void)
{ int xx=2000,yy=2000,zz=0,mType=0;
  __int64 x,y,z;
  // cytoA init ------------------------------------------------------
  if ( cytoAinit == 0 || cytoAinit == 5 ) mType=CYTOPLASM;
  if ( cytoAinit == 1 ) mType=NUCLEOPLASM;
  if ( cytoAinit == 2 ) mType=T_LUMEN;
  if ( cytoAinit == 3 ) mType=V_LUMEN;
  if ( cytoAinit == 4 ) mType=EXTRACELL;
  if ( SetObjectProps(CytoA,mType,greenCytoAnum,greenCytoAfrac,redCytoAnum,redCytoAfrac,cytoAnum) )
	ShowMessage("Failed to distribute CytoA");
  if ( movingVes && cytoAinit == 3 ) ObjToVes(CytoA,cytoAnum,0);
  if ( cytoAinit == 5 )
	for ( int i=0; i < cytoAnum; i++ )
	  { CytoA[i].x=cellXnm/2+sideGapNm;CytoA[i].y=cellYnm/2+sideGapNm;CytoA[i].z=cellZnm/2+botGapNm; }
  // Cyto B init -----------------------------------------------------
  if ( cytoBinit == 0 || cytoBinit == 2 ) mType=CYTOPLASM;
  if ( cytoBinit == 1 ) mType=EXTRACELL;
  if ( SetObjectProps(CytoB,mType,greenCytoBnum,greenCytoBfrac,redCytoBnum,redCytoBfrac,cytoBnum) )
	ShowMessage("Failed to distribute CytoB");
  if ( cytoBinit == 2 )
   for ( int i=0,xx,yy,zz,con=0; i < cytoBnum; i++ )
	 { CytoB[i].x=cellXnm*0.5+sideGapNm; CytoB[i].y=cellYnm*0.5+sideGapNm;
	   CytoB[i].z=cellZnm*0.5+botGapNm;
	 }
  //// Memb A init --------------------------------------------------------
  if ( membAinit == 0 || membAinit == 1 || membAinit == 5 || membAinit == 6 )
	mType=CELL_MEMB;
  if ( membAinit == 2 ) mType=NUCL_MEMB;
  if ( membAinit == 3 ) mType=TUBE_MEMB;
  if ( membAinit == 4 ) mType=VESI_MEMB;
  if ( membAinit == 6 )  // Logo distribution
	{ maskBmp->Canvas->Brush->Style=bsSolid; maskBmp->Canvas->Brush->Color=clBlack;
	  maskBmp->Canvas->FillRect(Rect(0,0,maskBmp->Width,maskBmp->Height));
	  maskBmp->Canvas->Font->Color=clWhite;
	  maskBmp->Canvas->Font->Size=36;maskBmp->Canvas->TextOut(10,10,"GMvCell");
	}
  if ( SetObjectProps(MembA,mType,greenMembAnum,greenMembAfrac,redMembAnum,redMembAfrac,membAnum) )
	ShowMessage("Failed to distribute MembA");
  if ( membAinit == 4 ) ObjToVes(MembA,membAnum,vesRadNm); // vesicle membrane
  for ( int i=0; i < membAnum; i++ )
	{ if ( membAinit == 0 )
		{ MembA[i].x=cellXnm*0.5+sideGapNm+1.0;MembA[i].y=cellYnm*0.5+sideGapNm+1.0;
		  MembA[i].z=botGapNm+nmVox*0.5; // default init 0
		}
	  if ( membAinit == 6 ) // Logo
		{ MembA[i].x=sideGapNm+10; MembA[i].y=sideGapNm+10;
		  for ( int j=0; j < 1000; j++ )
			{ xx=random(cellXnm-10)+sideGapNm; yy=random(cellYnm-10)+sideGapNm;
			  if ( maskBmp->Canvas->Pixels[xx/nmPix][yy/nmPix] > 0 )
				{ MembA[i].x=xx;  MembA[i].y=yy; break; }
			}
		}
	}
  if ( membAinit == 5 )  // Clusters
	for ( int i=0,con; i < membAnum; )
	  for ( int j=0; j < 5000; j++ ) // try to place memb object on right membrane
		{ xx=random(matrixXnm); yy=random(matrixYnm);zz=random(matrixZnm);
		  x=xx/nmVox,y=yy/nmVox,z=zz/nmVox;
		  if ( matrix[x+y*matrixX+z*matrixXY] == mType )
			{ for ( int ind=0; ind < membAclusterNum; ind++,i++ )
				{ MembA[i].x=xx; MembA[i].y=yy; MembA[i].z=zz; }
			  break;
			}
		}
  // Memb B initial properties
  if ( membBinit == 0 || membBinit == 1 ) mType=CELL_MEMB;
  if ( membBinit == 2 ) mType=NUCL_MEMB;
  if ( membBinit == 3 ) mType=TUBE_MEMB;
  if ( membBinit == 4 ) mType=VESI_MEMB;
  if ( SetObjectProps(MembB,mType,greenMembBnum,greenMembBfrac,redMembBnum,redMembBfrac,membBnum) )
	ShowMessage("Failed to distribute MembB");
  if ( membBinit == 4 ) ObjToVes(MembB,membBnum,vesRadNm); // vesicle membrane
  if ( membBinit == 0 )
	for ( int i=0,con=0; i < membBnum; i++ )
	  { MembB[i].x=cellXnm*0.5+sideGapNm; MembB[i].y=cellYnm*0.5+sideGapNm;
		MembB[i].z=botGapNm+nmVox*0.5;
	  }
  // Memb C initial properties
  if ( membCinit == 0 || membCinit == 1 ) mType=CELL_MEMB;
  if ( membCinit == 2 ) mType=NUCL_MEMB;
  if ( membCinit == 3 ) mType=TUBE_MEMB;
  if ( membCinit == 4 ) mType=VESI_MEMB;
  if ( SetObjectProps(MembC,mType,greenMembCnum,greenMembCfrac,redMembCnum,redMembCfrac,membCnum) )
  ShowMessage("Failed to distribute MembC");
  if ( membCinit == 4 ) ObjToVes(MembC,membCnum,vesRadNm); // vesicle membrane
  if ( membCinit == 0 )
	for ( int i=0,con=0; i < membCnum; i++ )
	  { MembC[i].x=cellXnm*0.5+sideGapNm; MembC[i].y=cellYnm*0.5+sideGapNm;
		MembC[i].z=botGapNm+nmVox*0.5;
	  }
  // Static A init
  double azi,ele,curX=0,curY=0,curZ=100,pitch=statApitchNm,ang=0.0,hexRad=10.0;
  double hX,hY,hZ;
  __int64 x64,y64,z64; int fN=0;
  for ( int i=0; i < STAT_NUM; i++ ) // free matrix from old StatA voxels
	if ( StatA[i].x != 0 || StatA[i].y != 0 || StatA[i].z != 0 )
	  { x64=StatA[i].x/nmVox; y64=StatA[i].y/nmVox; z64=StatA[i].z/nmVox;
		matrix[x64+(y64*matrixX)+(z64*matrixXY)]=CYTOPLASM;
	  }
  for ( int i=0; i < statAnum; i++ ) // set binding and fluorescence
	{ StatA[i].cytoAind=StatA[i].cytoBind=-1;StatA[i].membAind=StatA[i].membBind=-1;
	  StatA[i].x=StatA[i].y=StatA[i].z=0.0;
	  for ( int j=0; j < 4; j++ ) StatA[i].green[j]=StatA[i].red[j]=false;
	  for ( int j=0; j < greenStatAnum; j++ )
		if ( random(100) < greenStatAfrac ) StatA[i].green[j]=true;
	  for ( int j=0; j < redStatAnum; j++ )
		if ( random(100) < redStatAfrac ) StatA[i].red[j]=true;
	} // 0 - Strait in 2D, 1 - Strait in 3D, 2 curved in 2D, 3 - curved in 3D
  if ( statAinit != 4 ) // all except bundle in filopodia
	for ( int i=0; i < statAnum; )
	  { for ( int j=0; j < 10100; j++ ) // seed first object in filamaent
		  { x64=random(matrixX); y64=random(matrixY); z64=100.0/nmVox+botGapVox;
			if ( matrix[x64+(y64*matrixX)+(z64*matrixXY)] == CYTOPLASM )
			  { curX=x64*nmVox;  curY=y64*nmVox; curZ=z64*nmVox; break; }
			if ( j == 9999 ) { ShowMessage("ERROR - StatA seeding");j=statAnum;break; }
		  }
		azi=random(360); ele=0.0;
		if ( statAinit == 1 ) ele=random(360);
		for ( int j=0; j < monoPerFilo; j++ ) // init 0 Strait Fibres in cytoplasm z=100nm
		  { if ( i >= statAnum ) break;  // all StatA done
			if ( matrix[x64+(y64*matrixX)+(z64*matrixXY)] != CYTOPLASM ) break;
			StatA[i].x=curX; StatA[i].y=curY; StatA[i].z=curZ; // set x,y,z
			curX+=pitch*COS(azi)*COS(ele);curY+=pitch*SIN(azi)*COS(ele);
			if ( statAinit == 1 || statAinit == 3 ) curZ+=pitch*SIN(ele); // 3D
			if ( statAinit == 2 || statAinit == 3 ) azi+=RandOne()*bendTilt; // curved
			if ( statAinit == 3 ) ele+=RandOne()*bendTilt;  // 3D-curved
			x64=curX/nmVox; y64=curY/nmVox; z64=curZ/nmVox;
			i++;
		  }
	  }
   curX=sideGapNm+cellXnm-tubeRadNm-5; curY=sideGapNm+tubeRadNm+5;
   curZ=botGapNm+tubeRadNm+5;
   if ( statAinit == 5 ) // filament along the edge
	 for ( int i=0; i < statAnum && curY < sideGapNm+cellYnm-5; curY+=pitch, i++ )
	   { StatA[i].x=curX; StatA[i].y=curY; StatA[i].z=curZ; }
  if ( statAinit == 4 && filopodia ) // 6 Core Bundles in Filopodia
	for ( int i=0,j=0,con=0; i < statAnum; j++ )
	  { if ( filo[fN].x[j] <= 0.0 )
		  { j=0; fN++;  ang=0.0;
			if ( fN >= vesNum ) { ShowMessage("Too many StatA"); break; }
		  }
		for ( int a=0; a < 6 && i < statAnum; a++,i++ )
		  { azi=filo[fN].azi[j]; ele=filo[fN].ele[j]; 	ang+=60.0;
		//	hX=filo[fN].x[j]; hY=filo[fN].y[j]; hZ=filo[fN].z[j];
		//	Sph2cart(hexRad,azi+ang,ele,&hX,&hY,&hZ);
		//	StatA[i].x=hX; StatA[i].y=hY; StatA[i].z=hZ;
			StatA[i].x=filo[fN].x[j]-(SIN(azi)*COS(ele)*COS(ang)*hexRad);
			StatA[i].y=filo[fN].y[j]+(COS(azi)*COS(ele)*COS(ang)*hexRad);
			StatA[i].z=filo[fN].z[j]+(COS(ele)*SIN(ang)*hexRad);
		  }
	  }
	for ( int i=0; i < statAnum; i++ ) // fill matrix with new StatA voxels
	 { x64=StatA[i].x/nmVox; y64=StatA[i].y/nmVox; z64=StatA[i].z/nmVox;
	   matrix[x64+(y64*matrixX)+(z64*matrixXY)]=STAT_VOXEL;
	 }
  fissionCoef=0.0; time=setTime=0.0; frameCount=cycleNum=0; vesiGen=0;
  hiResBmp->Canvas->FillRect(Rect(0,0,hiResBmp->Width,hiResBmp->Height));
}
//---------------------------------------------------------------------------
float TMainForm::JumpSizeNm(double t_step, double Kdiff)
{ return sqrt(2.0*Kdiff*t_step)*1e3; }
//---------------------------------------------------------------------------
void TMainForm::CheckPathC(float x,float y,float z,float *dX,float *dY,float *dZ)
{ int steps=abs(*dX/nmVox)+2;
  if ( abs(*dY/nmVox)+2 > steps ) steps=abs(*dY/nmVox)+2;
  if ( abs(*dZ/nmVox)+2 > steps ) steps=abs(*dZ/nmVox)+2;
  float stepXnm=*dX/steps,stepYnm=*dY/steps,stepZnm=*dZ/steps;
  __int64 xVox,yVox,zVox;  // 64 bit integer variables
  *dX=*dY=*dZ=0.0;
  for ( int i=1; i <= steps; i++ )
	{ x+=stepXnm; y+=stepYnm; z+=stepZnm;
	  xVox=x/nmVox; yVox=y/nmVox; zVox=z/nmVox;
	  if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] < 10 ) // NON membrane voxel
		{ *dX=stepXnm*i; *dY=stepYnm*i; *dZ=stepZnm*i; }
	  else return;
	}
}
//---------------------------------------------------------------------------
void TMainForm::CheckPathM(float x,float y,float z,float *dX,float *dY,float *dZ)
{ __int64 xVox=x/nmVox,yVox=y/nmVox,zVox=z/nmVox;
  int stepsX=abs(*dX/nmVox)+2,stepsY=abs(*dY/nmVox)+2,stepsZ=abs(*dZ/nmVox)+2;
  float stepXnm=*dX/stepsX,stepYnm=*dY/stepsY,stepZnm=*dZ/stepsZ;
  *dX=*dY=*dZ=0.0;
  for ( int i=1; i <= stepsX; i++ )
	{ x+=stepXnm; xVox=x/nmVox;
	  if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] > 99 ) *dX=stepXnm*i;
	  else { x-=stepXnm; break; }  // 1 step back;
	} xVox=x/nmVox;
  for ( int i=1; i <= stepsY; i++ )
	{ y+=stepYnm; yVox=y/nmVox;
	  if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] > 99 ) *dY=stepYnm*i;
	  else { y-=stepYnm; break; } // 1 step back;
	} yVox=y/nmVox;
  for ( int i=1; i <= stepsZ; i++ )
	{ z+=stepZnm; zVox=z/nmVox;
	  if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] > 99 ) *dZ=stepZnm*i;
	  else break;
	}
}
//---------- Moves object to a nearest correct voxel  --------------------------
bool TMainForm::MoveToNearestVoxel(Obj *obj,int rad,int medium)
{ int cX=obj->x/nmVox,cY=obj->y/nmVox,cZ=obj->z/nmVox,dist=100000000,span=rad*2,min=0;
  __int64 x=cX,y=cY,z=cZ; float hVox=(float)nmVox*0.5; bool result=false;
  if ( matrix[x+(y*matrixX)+(z*matrixXY)] == medium ) return true; // no move
  for ( int i=0,good=0; i < 1000 && good < 10; i++ ) // 1000 attempts
	 { x=cX+random(span)-rad;y=cY+random(span)-rad;z=cZ+random(span)-rad;
	   if ( x < 0 || y < 0 || z < 0 ) continue; // check scope
	   if ( x >= matrixX || y >= matrixY || z >= matrixZ ) continue;
	   if ( matrix[x+(y*matrixX)+(z*matrixXY)] == medium )
		 if ( (min=sqrt((x-cX)*(x-cX)+(y-cY)*(y-cY)+(z-cZ)*(z-cZ))) < dist )
		   { obj->x=(x*nmVox)+hVox; obj->y=(y*nmVox)+hVox;obj->z=(z*nmVox)+hVox;
			 dist=min; result=true; good++; // limit to 10 good voxels
		   }
  	}
  return result;
}
//----------    x,nX,y,nY,z,nZ in voxel units  ----------------------------
void TMainForm::CalcMembMovement(Obj *obj,float mob,int num)
{ double jump=JumpSizeNm(tmStep,mob),curJump; float dX,dY,dZ;
  double raftRatio=JumpSizeNm(tmStep,mob*raftMobRatio),poliMob;
  double jumpAB=JumpSizeNm(tmStep,membBmob/(1.0+(membBmob/(membAmob+1e-9))));
  double jumpAC=JumpSizeNm(tmStep,membBmob/(1.0+(membCmob/(membAmob+1e-9))));
  double jumpBC=JumpSizeNm(tmStep,membCmob/(1.0+(membCmob/(membBmob+1e-9))));
  double ABratio=0.0;
  if ( membAmob > 0.0 )
	ABratio=(membBmob/(1.0+(membBmob/(membAmob+1e-9))))/membAmob;
  if ( jump > 0.0 ) raftRatio/=jump;
  __int64 x,nX,y,nY,z,nZ;
  for ( int i=0,j; i < num; i++ )
	{ if ( obj[i].membAind > -1 ) continue; // memb bound to membA
	  if ( obj[i].siteAind > -1 /*|| obj[i].boundToStatA */ ) continue; // in polimer
	  curJump=jump;
	  if ( obj[i].membBind > -1 ) curJump=jumpAB;
	  if ( obj[i].membCind > -1 ) curJump=jumpBC;
	  if ( obj[i].siteBind > -1 )  // slow down polimer
		{ j=obj[i].siteBind; poliMob=membAmob;
		  while ( j > -1 )
			{ j=MembA[j].siteBind; poliMob*=0.7;
			  if ( MembA[j].membBind > -1 ) poliMob*=ABratio;
			}
          curJump=JumpSizeNm(tmStep,poliMob); // reduced by polimer size
		}
	  x=obj[i].x/nmVox; y=obj[i].y/nmVox; z=obj[i].z/nmVox;  // current voxel
	  if ( matrix[x+(y*matrixX)+(z*matrixXY)] == RAFT_MEMB ) curJump*=raftRatio;
	  dX=GaussRand()*curJump; dY=GaussRand()*curJump; dZ=GaussRand()*curJump;
	  nX=(obj[i].x+dX)/nmVox; nY=(obj[i].y+dY)/nmVox; nZ=(obj[i].z+dZ)/nmVox;
	  if ( nX < 0 || nX >= matrixX ) dX=0.0;  // check volume boarders
	  if ( nY < 0 || nY >= matrixY ) dY=0.0;  // check volume boarders
	  if ( nZ < 0 || nZ >= matrixZ ) dZ=0.0;  // check volume boarders
	  CheckPathM(obj[i].x,obj[i].y,obj[i].z,&dX,&dY,&dZ);
	  obj[i].x+=dX; obj[i].y+=dY;obj[i].z+=dZ;
	  j=obj[i].siteBind; // oligomers
	  while ( j > -1 )    // move other membA in oligomers
		{ obj[j].x=obj[i].x;obj[j].y=obj[i].y;obj[j].z=obj[i].z; j=obj[j].siteBind; }
	}
}
//---------------------------------------------------------------------------
void TMainForm::FindFreeVoxel(float x,float y,float z,float *nX,float *nY,float *nZ)
{ __int64 x64=x/nmVox,y64=y/nmVox,z64=z/nmVox;
  for ( __int64 zz=z64-1; zz < z64+2; zz++ )
	for ( __int64 yy=y64-1; yy < y64+2; yy++ )
	  for ( __int64 xx=x64-1; xx < x64+2; xx++ )
		if ( matrix[xx+(yy*matrixX)+(zz*matrixXY)] == 0 )
		  { *nX=xx*nmVox; *nY=yy*nmVox; *nZ=zz*nmVox; return; }
}
//---------------------------------------------------------------------------
void TMainForm::MoveCytoAalongStatA(float stepNm)
{ float cX,cY,cZ,dX,dY,dZ,dist,nX,nY,nZ,minDist=stepNm*0.5;
  float maxDist=stepNm*1.5;///+sqrt(stepNm);
  double averStep=0.0,steps=0.0;
  double stepRate=(1.0/(cytoAstepRate*tmStep));
  int statInd[120],good,ind,occupado=0;
  for ( int i=0,statAind,distInd=-1,bestInd; i < cytoAnum; i++ )
	{ if ( CytoA[i].statAind < 0 ) continue;  // skip - cytoA is NOT bound
	  if ( random(stepRate+1.0) > 0 ) continue; // skip - cytoA is NOT ready to step
	  cX=CytoA[i].x; cY=CytoA[i].y; cZ=CytoA[i].z;
	  statAind=CytoA[i].statAind; good=0;
	  for ( int j=0; j < 120; j++ ) statInd[j]=0;
	  // search for free StatA within step size
	  for ( int j=statAind+1; j < statAind+115 && j < statAnum; j++ )
		{ if ( StatA[j].cytoAind > -1 ) continue; // statA occupied
		  dX=StatA[j].x-cX; dY=StatA[j].y-cY; dZ=StatA[j].z-cZ;
		  dist=sqrt((dX*dX)+(dY*dY)+(dZ*dZ));
		  if ( dist > minDist && dist < maxDist ) { statInd[good]=j; good++; }
		}
	  if ( good > 0 ) // make a step
		{ ind=random(good); ind=statInd[ind];
		  nX=StatA[ind].x; nY=StatA[ind].y; nZ=StatA[ind].z;
		 // move CytoA clossest to StatA
	   //	  FindFreeVoxel(StatA[ind].x,StatA[ind].y,StatA[ind].z,&nX,&nY,&nZ);
		  dX=nX-CytoA[i].x; dY=nY-CytoA[i].y; dZ=nZ-CytoA[i].z;
		  averStep+=sqrt(dX*dX+dY*dY+dZ*dZ);
		  steps+=1.0;
		  StatA[statAind].cytoAind=-1; CytoA[i].statAind=ind;
		  StatA[ind].cytoAind=i; CytoA[i].x=nX; CytoA[i].y=nY; CytoA[i].z=nZ;
		}
	 }
  if ( steps == 0.0 ) { Caption="No steps"; return; }/// return to avoid DIV by ZERO
  averStep/=steps; AnsiString str="AverStepSize ";str+=Round(averStep,1);
  str+="nm, "; str+=steps; str+=" steps"; Caption=str;
}
//--------- units in nm except x64, y64, z64 ---------------------------------
bool TMainForm::CheckForGap(float x,float y,float z,float dX,float dY,float dZ)
{ int steps=abs(dX/nmVox)+2;
  if ( abs(dY/nmVox)+2 > steps ) steps=abs(dY/nmVox)+2;
  if ( abs(dZ/nmVox)+2 > steps ) steps=abs(dZ/nmVox)+2;
  float stepXnm=dX/steps,stepYnm=dY/steps,stepZnm=dZ/steps;
  __int64 xVox,yVox,zVox;  // 64 bit integer variables
  dX=dY=dZ=0.0;
  for ( int i=1; i <= steps; i++ )
	{ x+=stepXnm; y+=stepYnm; z+=stepZnm;
	  xVox=x/nmVox; yVox=y/nmVox; zVox=z/nmVox;
	  if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] < 100 ) // solution voxel
		return true;
	}
  return false;
}
//--------- units in nm except x64, y64, z64 ---------------------------------
bool TMainForm::CheckForMemb(float x,float y,float z,float dX,float dY,float dZ)
{ int steps=abs(dX/nmVox)+2;
  if ( abs(dY/nmVox)+2 > steps ) steps=abs(dY/nmVox)+2;
  if ( abs(dZ/nmVox)+2 > steps ) steps=abs(dZ/nmVox)+2;
  float stepXnm=dX/steps,stepYnm=dY/steps,stepZnm=dZ/steps;
  __int64 xVox,yVox,zVox;  // 64 bit integer variables
  dX=dY=dZ=0.0;
  for ( int i=1; i <= steps; i++ )
	{ x+=stepXnm; y+=stepYnm; z+=stepZnm;
	  xVox=x/nmVox; yVox=y/nmVox; zVox=z/nmVox;
	  if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] > 100 ) // membrane voxel
		return true;
	}
  return false;
}
//---------------------------------------------------------------------------
void TMainForm::CytoAdynamics(void)
{ float jump=JumpSizeNm(tmStep,cytoAmob),nX,nY,nZ,dX,dY,dZ,rXY,tX,tY,tZ,prob;
  float jumpAB=JumpSizeNm(tmStep,cytoBmob/(1.0+(cytoBmob/(cytoAmob+1e-9))));
  __int64 xVox,yVox,zVox;  // 64 bit integer variables
  for ( int i=0; i < cytoAnum; i++ ) // Calculate movements of CytoA molecules
	{ if ( CytoA[i].membAind > -1 ) { cytoAboundToMembAnum++; continue; } // cytoA will move with membA
	  if ( CytoA[i].membBind > -1 ) { cytoAboundToMembBnum++; continue; } // cytoA will move with membB
	  if ( CytoA[i].statAind > -1 ) { cytoAboundToStatAnum++; continue; } // no movement
	  if ( CytoA[i].cytoAind > -1 ) { cytoApoliNum++; continue; } // no movement
	  if ( CytoA[i].cytoBind > -1 ) // slow mobility for cytoA with bound cytoB
		{ dX=GaussRand()*jumpAB;dY=GaussRand()*jumpAB;dZ=GaussRand()*jumpAB; }
	  else
		{ dX=GaussRand()*jump; dY=GaussRand()*jump; dZ=GaussRand()*jump; }
	  nX=CytoA[i].x+dX; nY=CytoA[i].y+dY; nZ=CytoA[i].z+dZ;
	  // check for volume borders
	  if ( nX < 0.0 || nX > matrixXnm ) dX=0.0;
	  if ( nY < 0.0 || nY > matrixYnm ) dY=0.0;
	  if ( nZ < 0.0 || nZ > matrixZnm ) dZ=0.0;
	  CheckPathC(CytoA[i].x,CytoA[i].y,CytoA[i].z,&dX,&dY,&dZ);  // check path
	  CytoA[i].x+=dX;CytoA[i].y+=dY;CytoA[i].z+=dZ; // move CytoA
	}
  // binding and other actions with cytoA
  for ( int i=0,ind; i < cytoAnum; i++ )
	{ if ( CytoA[i].medium == NUCLEOPLASM || CytoA[i].medium == T_LUMEN || CytoA[i].medium == V_LUMEN )
		cytoAinOrgan++;
	  if ( cytoAcanBindCytoB )  // check for binding to cytoB
		{ for ( int j=0,dist; j < cytoBnum; j++ )
			{ if ( CytoB[j].cytoAind > -1 ) continue; // CytoB is already bound
			  dX=CytoA[i].x-CytoB[j].x;dY=CytoA[i].y-CytoB[j].y;dZ=CytoA[i].z-CytoB[j].z;
			  dist=sqrt(dX*dX+dY*dY+dZ*dZ);
			  if ( dist > 499 || dist < 0 ) continue; // check if distance is too long (>500nm)
			  if ( CheckForMemb(CytoA[i].x,CytoA[i].y,CytoA[i].z,-dX,-dY,-dZ) )
				 continue; // membrane between cytoA and cytoB
			  prob=timeDistCytoAcytoB[dist]; prob*=cytoAcytoBbindRate;
			  if ( prob < 0.0001 ) continue; // AIT (interaction time) is too short
			  if ( random(1.0/prob) > 0 )  continue; // NO binding
			  CytoA[i].azi=random(360); CytoA[i].ele=random(360);
			  nX=CytoA[i].x+(COS(CytoA[i].azi)*COS(CytoA[i].ele)*cytoAcytoBdist);
			  nY=CytoA[i].y+(SIN(CytoA[i].azi)*COS(CytoA[i].ele)*cytoAcytoBdist);
			  nZ=CytoA[i].z+SIN(CytoA[i].ele)*cytoAcytoBdist;
			  if ( nX < 0.0 || nY < 0.0 || nZ < 0.0 ) continue; // out of borders
			  if ( nX > matrixXnm-20 || nY > matrixYnm-20 || nZ > matrixZnm-20 ) continue;
			  xVox=nX/nmVox;yVox=nY/nmVox;zVox=nZ/nmVox;
			  if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] == CytoB[j].medium ) // solution voxel
				{ CytoA[i].cytoBind=j; CytoB[j].cytoAind=i;
				  CytoB[j].x=nX;CytoB[j].y=nY;CytoB[j].z=nZ; // moving CytoB dist CytoA
				  break;
				}
			}
		}
	  // ------------ Polymerization
	  if ( cytoApoli /*&& CytoA[i].statAind > -1 */ && CytoA[i].cytoAind < 0 ) // seed bound
		if ( random(10000) == 0 ) // seeding of bound CytoA
		  { /* CytoA[i].cytoAind=1000000; */ CytoA[i].azi=random(360);
			CytoA[i].siteAind=1; }// make it a seed NOTE index 1000000
	  if ( cytoApoli && CytoA[i].siteAind > -1 ) // check for cytoA binding to polimer CytoA
		{ // if it is random
		  nX=CytoA[i].x+(COS(CytoA[i].azi)*COS(CytoA[i].ele)*statApitchNm);
		  nY=CytoA[i].y+(SIN(CytoA[i].azi)*COS(CytoA[i].ele)*statApitchNm);
		  nZ=CytoA[i].z+SIN(CytoA[i].ele)*statApitchNm;
		  xVox=nX/nmVox;yVox=nY/nmVox;zVox=nZ/nmVox;
		  for ( int j=0,dist; j < cytoAnum; j++ )
			{ if ( i == j || CytoA[j].cytoAind > -1 || CytoA[j].statAind > -1 ) continue; // skip if cytoA is already bound to cytoA
			  dX=CytoA[j].x-CytoA[i].x;dY=CytoA[j].y-CytoA[i].y;dZ=CytoA[j].z-CytoA[i].z;
			  dist=sqrt(dX*dX+dY*dY+dZ*dZ);
			  if ( dist > 499 ) continue ; // check if distance is too long (500nm)
			  prob=timeDistCytoAstatA[dist]; prob*=cytoApoliRate;
			  if ( prob < 0.0001 ) continue; // AIT is too short
			  if (  random(1.0/prob) == 0 )  // probability of binding
				if ( matrix[xVox+(yVox*matrixX)+(zVox*matrixXY)] < 10 ) // solution voxel
				  { CytoA[j].x=nX;CytoA[j].y=nY;CytoA[j].z=nZ;
					CytoA[j].cytoAind=i; CytoA[j].siteAind=1; CytoA[i].siteAind=-1; // last in filament
					if ( SForm->cond.cytoApoli3D )
					  CytoA[j].ele=CytoA[i].ele+RandOne()*bendTilt;
					CytoA[j].azi=CytoA[i].azi+RandOne()*bendTilt;
					break;
				  }
			}
		} // check for free CytoA binding to StatA
	  if ( cytoAcanBindStatA && CytoA[i].statAind < 0 && CytoA[i].cytoAind < 0 )
		{ for ( int j=0,dist; j < statAnum; j++ )
			{ if ( StatA[j].cytoAind > -1 ) continue; // skip if statA is already bound cytoA
			  dX=StatA[j].x-CytoA[i].x;dY=StatA[j].y-CytoA[i].y;dZ=StatA[j].z-CytoA[i].z;
			  dist=sqrt(dX*dX+dY*dY+dZ*dZ);
			  if ( dist > 499 ) continue; // check if distance is too long (500nm)
			  prob=timeDistCytoAstatA[dist]; prob*=cytoAstatAbindRate;
			  if ( prob < 0.0001 ) continue ; // AIT is too short
			  if (  random(1.0/prob) == 0 )  // probability of binding
				{ StatA[j].cytoAind=i;CytoA[i].statAind=j;CytoA[i].x=StatA[j].x;
				  CytoA[i].y=StatA[j].y; CytoA[i].z=StatA[j].z;  break;
				}
			}
		} // move CytoA bound either to membA or to membB
	  if ( (ind=CytoA[i].membAind) > -1 )
		{ CytoA[i].x=MembA[ind].x;CytoA[i].y=MembA[ind].y;CytoA[i].z=MembA[ind].z; }
	  if ( (ind=CytoA[i].membBind) > -1 )
		{ CytoA[i].x=MembB[ind].x;CytoA[i].y=MembB[ind].y;CytoA[i].z=MembB[ind].z; }
	}
  double depoliProb=(1.0/(cytoAdepoliRate*tmStep));
  if ( cytoAnum )   // protein dissociation / depolimerization
	for ( int i=0; i < cytoAnum; i++ )
	  if ( CytoA[i].siteAind == 1 ) // bound, but not first
		if ( random(depoliProb+1.0) < 1 )
		  { if ( CytoA[i].cytoAind > -1 ) CytoA[CytoA[i].cytoAind].siteAind=1;
			CytoA[i].cytoAind=CytoA[i].siteAind=-1;
		  }
}
//---------------------------------------------------------------------------
void TMainForm::CytoBdynamics(void)
{ float jump=JumpSizeNm(tmStep,cytoBmob),dX,dY,dZ,nX,nY,nZ,rXY;
  double dissRateAB=0.0; // (1.0/(cytoAcytoBdissRate*tmStep));
  if ( cytoAcytoBdissRate > 1e-9 ) dissRateAB=(1.0/(cytoAcytoBdissRate*tmStep));
  for ( int i=0; i < cytoBnum && dissRateAB > 0.0; i++ ) // CytoA dissociation from cytoB
	if ( CytoB[i].cytoAind > -1 && random(dissRateAB+1.0) < 1 ) // dissociation
	  if ( MoveToNearestVoxel(&CytoB[i],10,CYTOPLASM) )
		{ CytoA[CytoB[i].cytoAind].cytoBind=-1; CytoB[i].cytoAind=-1; }
  for ( int i=0; i < cytoBnum; i++ ) // Calculate movements of protein molecules
	{ if ( CytoB[i].cytoAind > -1 ) { cytoAboundToCytoBnum++; continue; } // cytoB will move with cytoA
	  if ( CytoB[i].membAind > -1 ) { cytoBboundToMembAnum++; continue; } // cytoB will move with membA
	  if ( CytoB[i].membBind > -1 ) { cytoBboundToMembBnum++; continue; } // cytoB will move with membB
	  if ( CytoB[i].statAind > -1 ) { cytoBboundToStatAnum++; continue; }
	  dX=GaussRand()*jump; dY=GaussRand()*jump; dZ=GaussRand()*jump;
	  nX=CytoB[i].x+dX; nY=CytoB[i].y+dY; nZ=CytoB[i].z+dZ;
	  if ( nX < 0.0 || nX > matrixXnm ) dX=0.0;
	  if ( nY < 0.0 || nY > matrixYnm ) dY=0.0;
	  if ( nZ < 0.0 || nZ > matrixZnm ) dZ=0.0;
	  CheckPathC(CytoB[i].x,CytoB[i].y,CytoB[i].z,&dX,&dY,&dZ); // check path for membrane
	  CytoB[i].x+=dX; CytoB[i].y+=dY;CytoB[i].z+=dZ;
	}
  if ( cytoAmob == 0.0 ) return; // do not move cytoB if cytoA immobile
  for ( int i=0,j; i < cytoBnum; i++ )  // move bound CytoB with CytoA
	if ( (j=CytoB[i].cytoAind) > -1 )
	  { CytoB[i].x=CytoA[j].x;CytoB[i].y=CytoA[j].y;CytoB[i].z=CytoA[j].z; }
}
//---------------------------------------------------------------------------
void TMainForm::MembAdynamics(void)
{ float dX,dY,dZ,prob;  int medium=CYTOPLASM;
  double dissRateA=(1.0/(cytoAmembAdissRate*tmStep)),depoliRate=(1.0/(membAdepoliRate*tmStep));
  if ( membApoli ) // polimerisation
	{ for ( int i=0,dist,ind,num; i < membAnum; i++ )
		{ if ( MembA[i].siteAind > -1 ) continue; // skip if membA already bound
		  for ( int j=0; j < membAnum; j++ ) // check for possible binding
			{ if ( MembA[j].siteBind > -1 || i == j ) continue; // skip if site already bound
			  ind=i; num=1;  // check if B-site the same poli or too long
			  while ( MembA[ind].siteBind > -1 ) { ind=MembA[ind].siteBind;num++; }
			  ind=j;
			  while ( MembA[ind].siteAind > -1 && ind != i && num < membApoliNum )
				{ ind=MembA[ind].siteAind; num++; }
			  if ( ind == i || num >= membApoliNum ) continue; // the same polimer
			  dX=MembA[j].x-MembA[i].x; dY=MembA[j].y-MembA[i].y; dZ=MembA[j].z-MembA[i].z;
			  dist=sqrt(dX*dX+dY*dY+dZ*dZ);
			  if ( dist > 499 ) continue ;// check if distance is too far (500nm)
			  prob=timeDistMembAA[dist]; prob*=membApoliRate;
			  if ( prob < 0.0001 ) continue ; // AIT too short
			  if (  random(1.0/prob) == 0 )  // probability of binding
				{ MembA[i].siteAind=j; MembA[j].siteBind=i; MembA[j].x=MembA[i].x;
				  MembA[j].y=MembA[i].y; MembA[j].z=MembA[i].z;  break;
				}
			}
		}
	}
  membAunitsInPoli=0.0; membAboundMembA=0; int sum=0,poliNum=0;
  for ( int i=0; i < membAnum; i++ ) // depolimerization
	  { if ( MembA[i].siteBind < 0  ) continue; // skip if site inside polimer
		if ( random(depoliRate+1.0) < 1 ) // dissociation
		  { MembA[MembA[i].siteBind].siteAind=-1; MembA[i].siteBind=-1; }
	  }
  for ( int i=0,ind,count; i < membAnum; i++ )  // counting
	{ if ( MembA[i].siteAind > -1  ) continue;  // bound to
	  ind=MembA[i].siteBind; count=1;
	  while ( ind > -1 && count < 100 ) { ind=MembA[ind].siteBind; count++; }
	  if ( count > 1 ) { sum+=count; poliNum++; }
	}
  membAboundMembA=poliNum;
  if ( poliNum > 0 ) membAunitsInPoli=(float)sum/(float)poliNum;
  // cytoA dissociation from membA
  for ( int i=0,j; i < membAnum; i++ )
	if ( (j=MembA[i].cytoAind) > -1 ) // possible CytoA dissociation
	 if ( random(dissRateA+1.0) < 1 )
	   { medium=CytoA[j].medium;
		 if ( membAorganPump ) // change CytoA medium type to where it pumped
		   { if ( MembA[i].medium == NUCL_MEMB ) medium=NUCLEOPLASM;
			 if ( MembA[i].medium == TUBE_MEMB ) medium=T_LUMEN;
             if ( MembA[i].medium == VESI_MEMB ) medium=V_LUMEN;
		   }
		 if ( MoveToNearestVoxel(&CytoA[j],10,medium) ) // move to nucleus voxel
		   { if ( membAorganPump ) CytoA[j].medium=medium; // restore fluorescence if needed
			 CytoA[j].membAind=-1; MembA[i].cytoAind=-1;
		   }
	   }

  if ( membAinit == 3 && recycling ) // return released MembA molecules to ER
	for ( __int64 i=0,x,y,z,xx,yy,zz,fail; i < membAnum; i++,fail=0 )
	  { if ( MembA[i].medium != CELL_MEMB ) continue; // not on cell membrane
		if ( random(dissRateA+1.0) < 1 )
		  { for ( int j=0; j < 10010; j++ ) // random positions at mType voxels
			  { xx=random(matrixXnm);yy=random(matrixYnm);zz=random(matrixZnm);
				x=xx/nmVox,y=yy/nmVox,z=zz/nmVox;
				if ( matrix[x+y*matrixX+z*matrixXY] == TUBE_MEMB )
				  { MembA[i].x=xx; MembA[i].y=yy; MembA[i].z=zz; break; }
				if ( j > 9999 ) { fail=1; break; }
			  }
            if ( fail ) break; // abort assigement
			MembA[i].azi=MembA[i].ele=0.0;MembA[i].vesicleInd=-1;MembA[i].medium=TUBE_MEMB;
			for ( int j=0; j < 4; j++ ) MembA[i].green[j]=MembA[i].red[j]=false;
			for ( int j=0; j < greenMembAnum; j++ )
			  if ( random(100) < greenMembAfrac ) MembA[i].green[j]=true;
			for ( int j=0; j < redMembAnum; j++ )
			  if ( random(100) < redMembAfrac ) MembA[i].red[j]=true;
		  }
	  } // END of recycling
  if ( membAmob > 0.0 ) CalcMembMovement(MembA,membAmob,membAnum);
  if ( !cytoAcanBindMembA ) return; // NO CytoA binding
  for ( int i=0; i < membAnum; i++ ) // possible binding cytoA
	{ if ( MembA[i].cytoAind > -1 ) continue; // skip if membA occupied
	  for ( int j=0,dist; j < cytoAnum; j++ )
		{ if ( CytoA[j].membAind > -1 || CytoA[j].membBind > -1 ) continue; // already bound
		  if ( MembA[i].medium == CELL_MEMB &&  CytoA[j].medium > 1 )
			continue;  // wrong compartment
		  if ( membAorganPump && CytoA[j].medium != CYTOPLASM ) continue; // already flipped
		  dX=CytoA[j].x-MembA[i].x;dY=CytoA[j].y-MembA[i].y;dZ=CytoA[j].z-MembA[i].z;
		  dist=(int)sqrt(dX*dX+dY*dY+dZ*dZ);
		  if ( dist > 499 ) continue; // check if distance is too long (500nm)
		  prob=timeDistCytoAmembA[dist]; prob*=cytoAmembAbindRate;
		  if ( prob < 0.0001 ) continue; // AIT is too short
		  if ( random(1.0/prob) == 0 )     // probability of binding
			{ MembA[i].cytoAind=j; CytoA[j].membAind=i; // bind cytoA to membA
			  CytoA[j].x=MembA[i].x;CytoA[j].y=MembA[i].y;CytoA[j].z=MembA[i].z;
			  break;
			}
		}
	}
}
//---------------------------------------------------------------------------
void TMainForm::MembBdynamics(void)
{ float dX,dY,dZ,nX,nY,nZ,rXY,prob; int medium;
  double dissRateB=(1.0/(cytoAmembBdissRate*tmStep));
  double membABdissRate=(1.0/(membAmembBdissRate*tmStep));
  if ( cytoAcanBindMembB && cytoAnum > 0 ) // check cytoA binding to MembB
	for ( int i=0,ind; i < membBnum; i++ )
	  { if ( MembB[i].cytoAind > -1 ) continue; // skip if membA occupied
		for ( int j=0,dist; j < cytoAnum; j++ )
		  { if (   CytoA[j].membAind > -1 || CytoA[j].membBind > -1 ) continue; // already bound
			if ( MembB[i].medium == CELL_MEMB &&  CytoA[j].medium > 1 )
			  continue;  // wrong compartment
			if ( membBcytoPump && CytoA[j].medium == CYTOPLASM ) continue; // already flipped
			dX=CytoA[j].x-MembB[i].x;dY=CytoA[j].y-MembB[i].y;dZ=CytoA[j].z-MembB[i].z;
			dist=(int)sqrt(dX*dX+dY*dY+dZ*dZ);
			if ( dist > 499 ) continue; // check if distance is too long (500nm)
			prob=timeDistCytoAmembB[dist]; prob*=cytoAmembBbindRate;
			if ( prob < 0.0001 ) continue; // AIT is too short
			if ( random(1.0/prob) == 0 )     // probability of binding
			  { MembB[i].cytoAind=j; CytoA[j].membBind=i; // bind cytoA to membA
				CytoA[j].x=MembB[i].x;CytoA[j].y=MembB[i].y;CytoA[j].z=MembB[i].z;
				break;
			  }
		  }
	  }
  if ( cytoAnum > 0 )    // cytoA dissociation from membB
	for ( int i=0,ind; i < membBnum; i++ )
	  if ( (ind=MembB[i].cytoAind) > -1 )  // if bound
		if ( random(dissRateB+1.0) < 1 )   // possible protein dissociation
		  { medium=CytoA[ind].medium;
			if ( membBcytoPump ) medium=CYTOPLASM;
			if ( MoveToNearestVoxel(&CytoA[ind],10,medium) ) // check right voxel
			  { if ( membBcytoPump ) CytoA[ind].medium=medium;
				CytoA[ind].membBind=-1;MembB[i].cytoAind=-1;
			  }
		  }
  if ( membAcanBindMembB )  // A to B binding
	for ( int i=0,dist; i < membBnum; i++ )
	  { if ( MembB[i].membAind > -1 ) continue; // skip if membB already bound membA
		  for ( int j=0; j < membAnum; j++ ) // check for possible binding
			{ if ( MembA[j].membBind > -1 ) continue; // skip if site A is already bound to site B
			  dX=MembA[j].x-MembB[i].x; dY=MembA[j].y-MembB[i].y; dZ=MembA[j].z-MembB[i].z;
			  dist=sqrt(dX*dX+dY*dY+dZ*dZ);
			  if ( dist > 499 ) continue ;// check if distance is too far (500nm)
			  if ( CheckForGap(MembB[i].x,MembB[i].y,MembB[i].z,dX,dY,dZ) )
				{ continue; }// solution gap between membA and membB
			  prob=timeDistMembAmembB[dist]; prob*=membAmembBbindRate;
			  if ( prob < 0.0001 ) continue ; // AIT too short
			  if (  random(1.0/prob) == 0 )  // probability of binding
				{ MembB[i].membAind=j; MembA[j].membBind=i; MembA[j].x=MembB[i].x;
				  MembA[j].y=MembB[i].y; MembA[j].z=MembB[i].z;
				  break; // active obj has index of bound object
				}
			}
		}
  for ( int i=0; i < membBnum; i++ ) // check possible MembA dissociation
	if ( MembB[i].membAind > -1 )
	  if ( random(membABdissRate+1.0) < 1 )  // MembB dissociation
		{ MembA[MembB[i].membAind].membBind=-1; MembB[i].membAind=-1; }
  for ( int i=0,j; i < membBnum; i++ ) // move MembB bound to MembA
	if ( (j=MembB[i].membAind) > -1 )
	  { MembB[i].x=MembA[j].x; MembB[i].y=MembA[j].y; MembB[i].z=MembA[j].z; }
  for ( int i=0; i < membBnum; i++ ) // count AB couples
	if ( MembB[i].membAind > -1 ) membAboundToMembBnum++;
  if ( membBmob > 0.0 ) CalcMembMovement(MembB,membBmob,membBnum); // diffusion
}
//---------------------------------------------------------------------------
void TMainForm::MembCdynamics(void)
{ float dX,dY,dZ,nX,nY,nZ,rXY,prob;
  double membBCdissRate=(1.0/(membBmembCdissRate*tmStep));
  if ( membBcanBindMembC && membBnum > 0 )  // B to C binding
	for ( int i=0,dist; i < membCnum; i++ )
	  { if ( MembC[i].membBind > -1 ) continue; // skip if site already bound one
		  for ( int j=0; j < membBnum; j++ ) // check for possible binding
			{ if ( MembB[j].membCind > -1 ) continue; // skip if membB is already bound to membC
			  dX=MembB[j].x-MembC[i].x; dY=MembB[j].y-MembC[i].y; dZ=MembB[j].z-MembC[i].z;
			  dist=sqrt(dX*dX+dY*dY+dZ*dZ);
			  if ( dist > 499 ) continue;// check if distance is too far (500nm)
			  if ( CheckForGap(MembC[i].x,MembC[i].y,MembC[i].z,dX,dY,dZ) )
				{ continue; }// solution gap between membC and membB
			  prob=timeDistMembBmembC[dist]; prob*=membBmembCbindRate;
			  if ( prob < 0.0001 ) continue ; // AIT too short
			  if (  random(1.0/prob) == 0 )  // probability of binding
				{ MembC[i].membBind=j; MembB[j].membCind=i; MembB[j].x=MembC[i].x;
				  MembB[j].y=MembC[i].y; MembB[j].z=MembC[i].z;
				  break;
				}
			}
		}
  for ( int i=0; i < membCnum; i++ ) // possible MembB dissociation
	if ( MembC[i].membBind > -1 )
	  if ( random(membBCdissRate+1.0) < 1 )  // MembB dissociation
		{ MembB[MembC[i].membBind].membCind=-1; MembC[i].membBind=-1; }
  for ( int i=0,j; i < membCnum; i++ ) // move MembC bound to MembB
	if ( (j=MembC[i].membBind) > -1 )
	  { MembC[i].x=MembB[j].x; MembC[i].y=MembB[j].y; MembC[i].z=MembB[j].z; }
  for ( int i=0; i < membCnum; i++ ) // count membBC couples
	if ( MembC[i].membBind > -1 ) membBboundToMembCnum++;
  if ( membCmob > 0.0 ) CalcMembMovement(MembC,membCmob,membCnum); // lateral diffusion
}
//---------------------------------------------------------------------------
void TMainForm::StatAdynamics(void)
{ double dissRateCytoAstatA=(1.0/(cytoAstatAdissRate*tmStep));
  if ( cytoAnum )   // cytoA dissociation from statA
	for ( int i=0; i < statAnum; i++ )
	  if ( StatA[i].cytoAind > -1 && CytoA[StatA[i].cytoAind].cytoAind < 0 ) // possible cytoA dissociation
		if ( random(dissRateCytoAstatA+1.0) < 1 )
		  { CytoA[StatA[i].cytoAind].statAind=-1; StatA[i].cytoAind=-1; }
  if ( cytoAcanMoveAlongStatA ) MoveCytoAalongStatA(cytoAstepSizeNm);
}
//---------------------------------------------------------------------------
void TMainForm::StepCalc(void)
{ float dX,dY;
  cytoAboundToMembAnum=cytoAboundToMembBnum=membAboundToMembBnum=0;
  cytoAboundToCytoBnum=cytoAboundToStatAnum=0,cytoAinOrgan=0;
  membBboundToMembCnum=0; cytoApoliNum=0;
  if ( bleachSpot || bleachStrip || bleachSquare ) Bleaching();
  if ( filopodia && !movingVes ) FiloDynamics(tubeRadVox); //only if NO vesicles
  if ( movingVes )
	{ if ( recycling )
		for ( int i=0; i < vesNum; i++ ) GenerateVesicle(i);
	  for ( int i=0; i < vesNum; i++ ) MoveVesicle(i);
	  for ( int i=0; i < cytoAnum; i++ ) // move to avoid vesicles
		if ( CytoA[i].vesicleInd < 0 )
		  MoveToNearestVoxel(&CytoA[i],20,CytoA[i].medium);
	  for ( int i=0; i < cytoBnum; i++ ) // move to avoid vesicles
		if ( CytoB[i].vesicleInd < 0 )
		  MoveToNearestVoxel(&CytoB[i],20,CytoB[i].medium);
	}
  CytoAdynamics();
  CytoBdynamics();
  MembAdynamics();
  MembBdynamics();
  MembCdynamics();
  StatAdynamics();
  // float minDist;
  int numBasal=0;
  setTime+=tmStep; time+=tmStep; cycleNum++;
  CellPaint(CCD_Bmp);   // check it
  frameCount=cycleNum/framesToSum;
  if ( cycleNum%framesToSum == 0 )
	{ FormPaint(0);
	  if ( Cell3D->Visible ) Cell3D->FormPaint(0);
	  if ( frameCount < 10000 ) // record spots
		for ( int i=0,n=frameCount; i < spotNum; i++ )
		  { spot[i].cX[n]=MembA[i].x*0.001;spot[i].cY[n]=MembA[i].y*0.001; // in µm
			spot[i].spInt[n]=MembA[i].z*0.001; //random(10)*random(10)+50;
		  }
	  if ( frameCount < 10000 ) tm[frameCount]=time;
 	}
}
//---------------------------------------------------------------------------
void TMainForm::EMCCD(void)
{ for ( int y=0,val; y < sizeYpix; y++ )
	for ( int x=0; x < sizeXpix; x++ )
     { val=cell[x][y];
	   val*=gain;
       val+=sqrt(val)*GaussRand();
       if ( val > 0 ) cell[x][y]=val;
	   else           cell[x][y]=0;
     }
}
//---------------------------------------------------------------------------
int __fastcall TMainForm::Emission(float x,float y,float z,float focus,float photons,int colour)
{ int dX=x-(cellXnm*0.5)+sideGapNm,dY=y-(cellYnm*0.5)+sideGapNm,gaussX,gaussY;
  int irisNm=irisRadUm*1000;
  if ( (dX*dX)+(dY*dY) > (irisNm*irisNm) ) return 0;
  if ( TIRillum->Checked && colour == GREEN )
	photons*=exp((-((double)(z))*1e-9)/d488);
  if ( TIRillum->Checked && colour == RED )
	photons*=exp((-((double)(z))*1e-9)/d561);
  if ( Confocal->Checked ) photons*=1.0/(1.0+(abs(z-focus)*0.003));
  else photons*=Gauss2D(dX,dY,(illumFWHMum*1000.0)/2.355);
  photons+=GaussRand()*sqrt(photons);
  if ( photons < 1.0 ) return 0;
  float curFWHM=Kobj*0.001*(focus-z)*(focus-z)+FWHMnm, sigma=curFWHM/2.355;
  int redShift=0;
  if ( twoCam && colour == RED ) redShift=sizeXpix*0.5;
  for ( int i=0; i < photons; i++ )
	{ gaussX=GaussRand()*sigma; gaussY=GaussRand()*sigma;
	  if ( Confocal->Checked )
		if ( abs((gaussX*gaussX+gaussY*gaussY)) > 20000 ) continue;
	  dX=(x+gaussX)/nmPix+redShift;
	  dY=(y+gaussY)/nmPix;
	  if ( dX < 0 || dX > sizeXpix-1 || dY < 0 || dY > sizeYpix-1 ) continue;
	  if ( cell[dX][dY] < 64000 ) cell[dX][dY]++;
	}
  return photons;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DrawFluor(Obj *obj,int num)
{ for ( int i=0,outcome; i < num; i++ )
	{ if ( blueLaser ) // Green fluorescence
		for ( int j=0; j < 4; j++ )
		  { //if ( obj[i].cytoBind < 0 ) continue; // for calcium imaging only
			if ( !obj[i].green[j] ) continue;
			if ( greenBlinking )
			  { if ( obj[i].darkState && random(greenONperFrame) < 1 ) // blinking
				  obj[i].darkState=false;
				if ( obj[i].darkState ) continue;
				if ( !obj[i].darkState && random(greenOFFperFrame) < 1 )
				  { obj[i].darkState=true; continue; } // in to dark state
			  }
			outcome=Emission(obj[i].x,obj[i].y,obj[i].z,focusNm,greenPhotons,GREEN);
			if ( outcome > 0 )  // photobleaching
			  if ( random(greenBleachPerFrame/outcome) < 1 ) obj[i].green[j]=false;
			if ( FRET ) // check red fluorophores around
			  { float dX,dY,dZ,dist;
				for ( int k=0;  k < membBnum; k++ )
				  { dX=obj[i].x-MembB[k].x; dY=obj[i].y-MembB[k].y;
					dZ=obj[i].z-MembB[k].z; dist=sqrt((dX*dX)+(dY*dY)+(dZ*dZ));
					if ( dist > 20.0 ) continue; // too far
					for ( int j=0; j < 4; j++ )
					  { if ( !MembB[k].red[j] ) continue;  // not active
						Emission(MembB[k].x,MembB[k].y,MembB[k].z,focusNm,redPhotons/(dist+1.0),RED);
					  }
				  }
			  }
		  }
      if ( greenLaser ) // red fluorescence
		for ( int j=0; j < 4; j++ )
		  { if ( !obj[i].red[j] ) continue;
			outcome=Emission(obj[i].x,obj[i].y,obj[i].z,focusNm,redPhotons,RED);
            if ( outcome > 0 )  // photobleaching
              if ( random(redBleachPerFrame/outcome) < 1 ) obj[i].red[j]=false;
          }
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CellPaint(Graphics::TBitmap *bmp)
{ if ( alterIllum )  // Alternating illumination
    if ( (cycleNum-1)%framesToSum == 0 )
      { blueLaser=!blueLaser; greenLaser=!blueLaser; }
  if ( (cycleNum-1)%framesToSum == 0 && twoAngles )
    { if ( TIRangle == 63.0 ) TIRangle=68.0;
      else                    TIRangle=63.0;
      d488=Calc_D(TIRangle,488);d561=Calc_D(TIRangle,561);
   //   Caption=TIRangle;
    }
  if ( ShowFluor->Checked )
	{ greenPhotons=((double)greenPhotonsPerSec)*tmStep;
	  greenBleachPerFrame=(1.0/(tmStep*greenBleachRate))*greenPhotons;
	  greenONperFrame=1.0/(tmStep*greenBlinkONrate);
	  greenOFFperFrame=1.0/(tmStep*greenBlinkOFFrate);
	  redPhotons=((double)redPhotonsPerSec)*tmStep;
	  redBleachPerFrame=(1.0/(tmStep*redBleachRate))*redPhotons;
 	  if ( (cycleNum-1)%framesToSum == 0 )
		for ( int x=0,val; x < sizeXpix; x++ ) // photoNoise
		  for ( int y=0; y < sizeYpix; y++ )
			{ if ( (val=noiseAmp*3.0+GaussRand()*noiseAmp) > 0 ) cell[x][y]=val;
			  else                                               cell[x][y]=0;
			}
	  DrawFluor(CytoA,cytoAnum);
	  DrawFluor(CytoB,cytoBnum);
	  DrawFluor(MembA,membAnum);
	  DrawFluor(MembB,membBnum);
	  DrawFluor(MembC,membCnum);
	  DrawFluor(StatA,statAnum);
      if ( gain > 0 && EMCCDmode && cycleNum%framesToSum == 0 ) EMCCD(); // use EMCCD camera
	  int *ptr;  int centX=sizeXpix/2;
	  // Draw fluorescent image
	  if ( twoCam )
		for ( int y=0; y < sizeYpix; y++ )
		  { ptr=(int *)bmp->ScanLine[y];
			for ( int x = 0; x < centX; x++ )
			  if ( cell[x][y] < 255 ) ptr[x]=cell[x][y]<<8;
			  else                    ptr[x]=255<<8;
			for ( int x = centX; x < sizeXpix; x++ )
			  if ( cell[x][y] < 255 ) ptr[x]=cell[x][y]<<16;
			  else                    ptr[x]=255<<16;
		   }
      else  // one camera
        { if ( greenLaser )
			for ( int y=0; y < sizeYpix; y++ )
              { ptr=(int *)bmp->ScanLine[y];
				for ( int x = 0; x < sizeXpix; x++ )
                if ( cell[x][y] < 255 ) ptr[x]=cell[x][y]<<16;
				else                    ptr[x]=255<<16;
			  }
		  else
			for ( int y=0; y < sizeYpix; y++ )
			  { ptr=(int *)bmp->ScanLine[y];
				for ( int x = 0; x < sizeXpix; x++ )
                  if ( cell[x][y] < 255 ) ptr[x]=cell[x][y]<<8;
				  else                    ptr[x]=255<<8;
          }
        }
      } // end of fluorescence output
  else
	{ bmp->Canvas->Brush->Style=bsSolid; bmp->Canvas->Brush->Color=clBlack;
	  bmp->Canvas->FillRect(Rect(0,0,bmp->Width,bmp->Height));
    }
  if ( ShowStaticObj->Checked ) // Static Objects drawn first
	{ bmp->Canvas->Pen->Color=(TColor)RGB(20,20,254);
	  for ( int i=0,x,y; i < statAnum; i++ )
		{ x=StatA[i].x/nmPix; y=StatA[i].y/nmPix;
		  bmp->Canvas->Rectangle(x-1,y-1,x+1,y+1);
		}
	}
  if ( ShowMembObj->Checked )
	 { bmp->Canvas->Pen->Color=(TColor)RGB(250,250,0); bmp->Canvas->Brush->Style=bsClear;
	   for ( int i=0,x,y; i < membAnum; i++ )
		 { x=MembA[i].x/nmPix; y=MembA[i].y/nmPix;
		   bmp->Canvas->Rectangle(x-1,y-1,x+1,y+1);
		   if ( MembA[i].cytoAind > -1 ) bmp->Canvas->Rectangle(x-3,y-3,x+3,y+3);
		   if ( MembA[i].siteBind > -1 )
			{ bmp->Canvas->Rectangle(x-2,y-2,x+2,y+2);bmp->Canvas->Rectangle(x-4,y-4,x+4,y+4); }
		 }
	   bmp->Canvas->Pen->Color=(TColor)RGB(160,160,0);
	   for ( int i=0,x,y; i < membBnum; i++ )   // mark membB
		 { x=MembB[i].x/nmPix; y=MembB[i].y/nmPix;
		   bmp->Canvas->Rectangle(x-1,y-1,x+1,y+1);
		   if  ( MembB[i].cytoAind > -1 ) bmp->Canvas->Rectangle(x-3,y-3,x+3,y+3);
		   if  ( MembB[i].membAind > -1 ) bmp->Canvas->Rectangle(x-4,y-4,x+4,y+4);
		  }
	   bmp->Canvas->Pen->Color=(TColor)RGB(254,200,100);
	   for ( int i=0,x,y; i < membCnum; i++ )
		 { x=MembC[i].x/nmPix; y=MembC[i].y/nmPix;
		   bmp->Canvas->Rectangle(x-1,y-1,x+1,y+1);
		   if  ( MembC[i].membBind > -1 ) bmp->Canvas->Rectangle(x-3,y-3,x+3,y+3);
		  }
	  }
   if ( ShowCytoObj->Checked )
	 { bmp->Canvas->Pen->Color=(TColor)RGB(254,254,254);
       bmp->Canvas->Brush->Style=bsClear;
	   for ( int i=0,x,y; i < cytoAnum; i++ )
		 { x=CytoA[i].x/nmPix; y=CytoA[i].y/nmPix;
		   bmp->Canvas->Rectangle(x-1,y-1,x+1,y+1);
		 }
	   bmp->Canvas->Pen->Color=(TColor)RGB(160,160,160);
	   for ( int i=0,x,y; i < cytoBnum; i++ )
		 { x=CytoB[i].x/nmPix; y=CytoB[i].y/nmPix;
		   if ( CytoB[i].cytoAind > -1 ) bmp->Canvas->Rectangle(x-2,y-2,x+2,y+2);
		   else bmp->Canvas->Rectangle(x-1,y-1,x+1,y+1);
		 }
	 }
}
//---------------------------------------------------------------------------
double TMainForm::DiffCalc(double d_time, double k_diff, double area,double d_conc,double d_x)
 { return (k_diff*area)*(d_conc/d_x)*d_time; }
//---------------------------------------------------------------------------
double TMainForm::DistCalc(double tm,double k_diff, double dist)
{ double fourDt=4.0*k_diff*tm;
  return (1.0/sqrt(4.0*PI*fourDt))*exp(-((dist*dist)/fourDt));
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RunClick(TObject *Sender)
{ abort=false; time=0.0; static int frCount=0;
  while ( abort == false && membOK )     // no running with unprepared membrane
	{ StepCalc();
	  Application->ProcessMessages();
	  if ( numOfDarkFrames )
		{ if ( frCount == 0 ) ShowFluor->Checked=false;
		  if ( frCount == numOfDarkFrames ) ShowFluor->Checked=true;
		  frCount++;
		  if ( frCount >= numOfDarkFrames+framesToSum ) frCount=0;
		}
	  if ( frameLimit > 0 && frameCount >= frameLimit && saveFrame ) abort=true;
	  if ( saveFrame && ShowFluor->Checked && cycleNum%framesToSum == 0 )
		WriteFrame();

	  if ( saveBMP /* && ShowFluor->Checked */ && cycleNum%framesToSum == 0 )
		{ AnsiString str=bmpName; str+=frameCount; str+=".bmp";
		  if ( Cell3D->Visible )
			{ if ( Cell3D->BuildTube->Checked )
				{ if ( CheckSphere(tubeRadVox+3,tX,tY,tZ,T_LUMEN,TUBE_MEMB,CYTOPLASM) )
					{ FillSphere(tubeRadVox,tX,tY,tZ,TUBE_MEMB,T_LUMEN,CELL_MEMB);
					  if (tube3D ) { tEle+=RandOne()*bendTilt;tZ+=SIN(tEle); }
					  tX+=COS(tAzi);tY+=SIN(tAzi);tAzi+=RandOne()*bendTilt;
					}
				  else
					{ tX=random(matrixX-100)+20;tY=85; // random(matrixY-80)+40;
					  tZ=random(matrixZ-20)+20;tAzi=random(360);tEle=random(360);
					}
				 }
			  Cell3D->reDrawCell=true; Cell3D->FormPaint(Sender);
			  if ( Cell3D->bmpX < 4950 ) Cell3D->Bmp->Width=Cell3D->bmpX+50;
			  else Cell3D->Bmp->Width=5000;
			  if ( Cell3D->bmpY < 2950 ) Cell3D->Bmp->Height=Cell3D->bmpY+50;
			  else Cell3D->Bmp->Height=3000;
			  Cell3D->Bmp->SaveToFile(str);
			}
		  else
			{ Bmp24->Canvas->CopyRect(Rect(0,0,Bmp24->Width,Bmp24->Height),
				  CCD_Bmp->Canvas,Rect(0,0,Bmp24->Width,Bmp24->Height));
			  str="fr ";str+=frameCount; str+=", time=";
			  str+=((int)(time*1000.0000000001))/1000.0; str+="s, focus=";
			  str+=focusNm; str+="nm ("; str+=nmPix; str+="nm/pix         ";
			  Bmp24->Canvas->TextOut(2,sizeYpix+2,str);
			  str=bmpName; str+=frameCount; str+=".bmp";
			  Bmp24->SaveToFile(str);
			}
		}
	  scanCount++;
	  if ( Zscan->Checked && frameCount%10 == 0 )
		{ focusNm+=ZscanDir*100;
		  if ( focusNm > cellZnm+botGapNm ) ZscanDir=-1;
		  if ( focusNm <= 0 ) ZscanDir=1;
		  TrackBar->Position=focusNm; scanCount=0;
		}
	}
  if ( saveBMP ) saveBMP=false;
  if ( saveFrame ) { fclose(fileV);fclose(fileG);fclose(fileR);saveFrame=false; }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject *Sender)
{ CalculateDimensions();
  if ( ClientWidth < 200 ) ClientWidth=200;
  if ( sizeXpix > ClientWidth ) sizeXpix=ClientWidth;
  if ( sizeYpix > ClientHeight-StBar->Height ) sizeYpix=ClientWidth-StBar->Height;
  maskBmp->Width=sizeXpix; maskBmp->Height=sizeYpix;
  CCD_Bmp->Width=sizeXpix; CCD_Bmp->Height=sizeYpix; Bmp24->Width=sizeXpix;
  Bmp24->Height=sizeYpix+20;
  Canvas->Brush->Color=clBlack;
  Canvas->FillRect(Rect(0,0,ClientWidth,ClientHeight));
  TrackBar->Left=ClientWidth-TrackBar->Width;
  Canvas->Font->Color=clYellow;
  FormPaint(Sender);
}
//---------------------------------------------------------------------------
void TMainForm::DrawMembrane(Graphics::TBitmap *bmp)
{ int *ptr,val=0; double scl=(double)nmPix/(double)nmVox;
  __int64 indX,indY,indZ;
  int szX=matrixX/scl,szY=matrixY/scl,szZ=matrixZ/scl;
  if ( szX > ClientWidth ) szX=ClientWidth;
  if ( szY > ClientHeight ) szY=ClientHeight;
  AnsiString str=szX; str+="   ";str+=szY; str+="   "; str+=(int)scl;
  str+="  curZ="; str+=currentZ*nmVox; str+="(nm)";// Caption=str;
  for ( int y=0; y < szY; y++ )
	{ ptr=(int *)bmp->ScanLine[y];
	  indY=y; indY*=scl; indZ=currentZ;
	  for ( int x = 0; x < szX; x++ )
		{ indX=x; indX*=scl;
		  val=matrix[indX+(indY*matrixX)+(indZ*matrixXY)];
		  if ( val == CYTOPLASM ) val=40;
		  if ( val == T_LUMEN ) val=60;
		  if ( val == NUCLEOPLASM ) val=80;
		  if ( val == V_LUMEN ) val=100;
 		  ptr[x]=val<<8;
		}
	}
 bmp->Canvas->Pen->Color=(TColor)RGB(20,20,254);
 for ( int i=0,x,y; i < statAnum; i++ )
   { if ( StatA[i].z > currentZ*nmVox+nmVox || StatA[i].z < currentZ*nmVox-nmVox ) continue;
	 x=StatA[i].x/nmPix; y=StatA[i].y/nmPix;
	 bmp->Canvas->Rectangle(x-1,y-1,x+1,y+1);
   }
  str=currentZ*nmVox; str+=" nm";  bmp->Canvas->Font->Size=14;
  bmp->Canvas->Font->Color=clWhite; bmp->Canvas->TextOut(2,2,str);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormPaint(TObject *Sender)
{ static int indAA=0; double averAA=0.0;
  Canvas->Draw(0,0,CCD_Bmp);
  if ( ShowMemb->Checked ) DrawMembrane(CCD_Bmp); // show membrane structure
  AnsiString str=nmPix; str+=" nm/pix, Focus="; str+=focusNm;
  str+="nm, frameN="; str+=frameCount; str+=", Time=";
  str+=((int)(time*1000.0000000001))/1000.0; str+="s";
  StBar->Panels->Items[0]->Text=str; str="";
  str=" ";
  if ( cytoAboundToCytoBnum )
	{ str+=cytoAboundToCytoBnum; str+=" CytoA-CytoB pairs,"; }
  if ( cytoAboundToMembAnum )
	{ str+=cytoAboundToMembAnum; str+=" CytoA-MembA pairs,"; }
  if ( cytoAboundToMembBnum )
	{ str+=" ";  str+=cytoAboundToMembBnum; str+=" CytoA-MembB pairs,"; }
  if ( membAorganPump )
	{ str+=" ";  str+=cytoAinOrgan; str+=" CytoA in organelle,"; }
  if ( membAboundMembA )
	{  indAA++;
	   if ( indAA > 99 ) indAA=0;
	   str+=" ";str+=membAboundMembA; str+=" MembAA poli., ";
	   str+=Round(membAunitsInPoli,1); str+=" units/poli., ";
    }
  if ( membAboundToMembBnum )
	{ str+=" ";str+=membAboundToMembBnum;str+=" membA-B pairs,"; }
  if ( membBboundToMembCnum )
	{ str+=" ";str+=membBboundToMembCnum; str+=" membB-C pairs,"; }
  if ( cytoAboundToStatAnum )
	{ str+=" ";  str+=cytoAboundToStatAnum; str+=" CytoA-StatA pairs"; }
  if ( cytoApoliNum )
	{ str+=" "; str+=cytoApoliNum; str+=" CytoA polimer"; }
  if ( vesiGen ) { str+=" "; str+=vesiGen;str+=" Generated Vesicles"; }
  InfoBar->Panels->Items[0]->Text=str;
  if ( frameCount < 10000 )
	{ stat[0][frameCount]=cytoAboundToMembAnum; stat[1][frameCount]=cytoAboundToMembBnum;
	  stat[2][frameCount]=cytoAboundToMembCnum; stat[3][frameCount]=cytoAboundToStatAnum;
	  stat[4][frameCount]=membAboundToMembBnum; stat[5][frameCount]=membBboundToMembCnum;
	  stat[6][frameCount]=membAboundMembA;
	}
  Canvas->TextOut(ClientWidth-60,TrackBar->Height+5,"Focus");
}
//---------------------------------------------------------------------------
void TMainForm::VarsToPrms(void)
{ // Cell Properties
  SForm->cond.cellXnm=cellXnm; SForm->cond.cellYnm=cellYnm; SForm->cond.cellZnm=cellZnm;
  SForm->cond.fenceJumpProb=fenceJumpProb; SForm->cond.raftMobRatio=raftMobRatio;
  SForm->cond.voxelSize=nmVox; SForm->cond.raftRadNm=raftRadNm;
  SForm->cond.tmInterMs=tmStep*1000.0;  SForm->cond.IDum=IDum; SForm->cond.flattening=flattening;
  SForm->cond.tubeRadNm=tubeRadNm; SForm->cond.memorySizeMb=memorySizeMb;
  SForm->cond.interTubeDistNm=interTubeDistNm; SForm->cond.tubeLengthNm=tubeLengthNm;
  SForm->cond.nucleusRadNm=nucleusRadNm;  SForm->cond.nucleus=nucleus;
  SForm->cond.tubeNetwork=tubeNetwork; SForm->cond.grid=grid;
  SForm->cond.rafts=rafts;
  SForm->cond.filopodia=filopodia;  SForm->cond.tubes=tubes;
  SForm->cond.movingVesicles=movingVes; SForm->cond.vesicleMob=vesicleMob;
  SForm->cond.sideGapNm=sideGapNm; SForm->cond.botGapNm=botGapNm;
  SForm->cond.topGapNm=topGapNm;  SForm->cond.vesRadNm=vesRadNm;
  SForm->cond.poreRadNm=poreRadNm; SForm->cond.bendTilt=bendTilt;
  SForm->cond.bodyFilletNm=bodyFilletNm; SForm->cond.roundedCellBody=roundedCellBody;
  SForm->cond.vesNum=vesNum; SForm->cond.mitochondria=mitochondria;
  SForm->cond.outsideVesicles=outsideVes;
  SForm->cond.tube3D=tube3D; SForm->cond.recycling=recycling;SForm->cond.fuseVes=fuseVesicles;
  // Illumination and Camera
  SForm->cond.greenBleachRate=greenBleachRate; SForm->cond.redBleachRate=redBleachRate;
  SForm->cond.Kobj=Kobj; SForm->cond.TIRangle=TIRangle; SForm->cond.FWHM=FWHMnm;
  SForm->cond.illumFWHMum=illumFWHMum; SForm->cond.irisRadUm=irisRadUm;
  SForm->cond.framesToSum=framesToSum;  SForm->cond.nmPix=nmPix;
  SForm->cond.numOfDarkFrames=numOfDarkFrames;
  SForm->cond.noise=noiseAmp;   SForm->cond.gain=gain;  SForm->cond.EMCCD=EMCCDmode;
  SForm->cond.greenPhotonsPerSec=greenPhotonsPerSec;
  SForm->cond.redPhotonsPerSec=redPhotonsPerSec;
  SForm->cond.twoCam=twoCam; SForm->cond.alterIllum=alterIllum;
  SForm->cond.greenBlinkONrate=greenBlinkONrate;
  SForm->cond.greenBlinkOFFrate=greenBlinkOFFrate;
  // cyto A prop
  SForm->cond.cytoAnum=cytoAnum;  SForm->cond.cytoAinit=cytoAinit;
  SForm->cond.cytoAmob=cytoAmob;  SForm->cond.membAmob=membAmob;
  SForm->cond.greenCytoAnum=greenCytoAnum; SForm->cond.redCytoAnum=redCytoAnum;
  SForm->cond.greenCytoAfraction=greenCytoAfrac;
  SForm->cond.redCytoAfraction=redCytoAfrac; SForm->cond.cytoApoli=cytoApoli;
  SForm->cond.membAmembBbindRate=membAmembBbindRate;
  SForm->cond.cytoAmembAbindRate=cytoAmembAbindRate;
  SForm->cond.cytoAcanBindMembA=cytoAcanBindMembA; SForm->cond.cytoApoliRate=cytoApoliRate;
  SForm->cond.cytoAcanBindMembB=cytoAcanBindMembB; SForm->cond.cytoAdepoliRate=cytoAdepoliRate;
  SForm->cond.cytoAstepSizeNm=cytoAstepSizeNm,SForm->cond.cytoAstepRate=cytoAstepRate;
  // Cyto B props
  SForm->cond.cytoBnum=cytoBnum;  SForm->cond.cytoBmob=cytoBmob;
  SForm->cond.cytoAcanBindCytoB=cytoAcanBindCytoB; SForm->cond.cytoBinit=cytoBinit;
  SForm->cond.cytoAcytoBbindRate=cytoAcytoBbindRate;
  SForm->cond.cytoAcytoBdissRate=cytoAcytoBdissRate;
  SForm->cond.greenCytoBnum=greenCytoBnum; SForm->cond.redCytoBnum=redCytoBnum;
  SForm->cond.greenCytoBfraction=greenCytoBfrac;
  SForm->cond.redCytoBfraction=redCytoBfrac; SForm->cond.cytoBdistA=cytoAcytoBdist;
  // Memb A prop
  SForm->cond.membAnum=membAnum;   SForm->cond.membAinit=membAinit;
  SForm->cond.oneColourAA=oneColourAA; SForm->cond.membApoliNum=membApoliNum;
  SForm->cond.greenMembAnum=greenMembAnum; SForm->cond.redMembAnum=redMembAnum;
  SForm->cond.greenMembAfraction=greenMembAfrac;
  SForm->cond.redMembAfraction=redMembAfrac;
  SForm->cond.membAdepoliRate=membAdepoliRate;  SForm->cond.membApoliRate=membApoliRate;
  SForm->cond.cytoAmembAdissRate=cytoAmembAdissRate;
  SForm->cond.membAmembBdissRate=membAmembBdissRate;
  SForm->cond.membApoli=membApoli;
  SForm->cond.membAcanBindMembB=membAcanBindMembB;
  // Memb B prop
  SForm->cond.membBnum=membBnum;  SForm->cond.membBinit=membBinit;
  SForm->cond.membBmob=membBmob;
  SForm->cond.greenMembBnum=greenMembBnum;    SForm->cond.redMembBnum=redMembBnum;
  SForm->cond.greenMembBfraction=greenMembBfrac;
  SForm->cond.redMembBfraction=redMembBfrac;
  SForm->cond.cytoAmembBbindRate=cytoAmembBbindRate;
  SForm->cond.cytoAmembBdissRate=cytoAmembBdissRate;
  // Memb C prop
  SForm->cond.membCnum=membCnum; SForm->cond.membCinit=membCinit;
  SForm->cond.membCmob=membCmob; SForm->cond.membBcanBindMembC=membBcanBindMembC;
  SForm->cond.greenMembCnum=greenMembCnum; SForm->cond.redMembCnum=redMembCnum;
  SForm->cond.greenMembCfraction=greenMembCfrac;
  SForm->cond.redMembCfraction=redMembCfrac;
  SForm->cond.membBmembCbindRate=membBmembCbindRate;
  SForm->cond.membBmembCdissRate=membBmembCdissRate;
  // Static A prop
  SForm->cond.statAnum=statAnum;    SForm->cond.statAinit=statAinit;
  SForm->cond.greenStatAnum=greenStatAnum;           SForm->cond.redStatAnum=redStatAnum;
  SForm->cond.greenStatAfrac=greenStatAfrac; SForm->cond.redStatAfrac=redStatAfrac;
  SForm->cond.cytoAstatAbindRate=cytoAstatAbindRate;
  SForm->cond.cytoAstatAdissRate=cytoAstatAdissRate;
  SForm->cond.cytoAcanMoveAlongStatA=cytoAcanMoveAlongStatA;
  SForm->cond.filamentPitchStepNm=statApitchNm;
  SForm->cond.cytoAcanBindStatA=cytoAcanBindStatA;
  SForm->cond.actinBundleExtendNm=actinBundleExtendNm;
  SForm->cond.membAclusterNum=membAclusterNum;
  SForm->cond.monoPerFilo=monoPerFilo;
}
//---------------------------------------------------------------------------
void TMainForm::PrmsToVars(void)
{  if ( SForm->cond.memorySizeMb != memorySizeMb )  // resize main memory
	 { memorySizeMb=SForm->cond.memorySizeMb;
	   if ( abort == false ) { abort=true; Sleep(100); }
	   matrix=NULL; Canvas->TextOut(400,20,"Allocating Main Memory Block");
	   FormPaint(0); delete matrix;
	   __int64 size=memorySizeMb*1e6;// MATRIX_SIZE;
	   matrix = new byte[size];
	   if ( matrix == NULL )
		 { ShowMessage("CRITICAL ERROR- matrix memory Alloc" ); Close(); }
	   Canvas->TextOut(400,20,"------- DONE -------------");
	  }
  if ( cellXnm != SForm->cond.cellXnm || cellYnm != SForm->cond.cellYnm ||
	   cellZnm != SForm->cond.cellZnm || SForm->cond.flattening != flattening ||
	   SForm->cond.voxelSize != nmVox || SForm->cond.raftRadNm != raftRadNm ||
	   SForm->cond.nucleusRadNm != nucleusRadNm ||
	   nmPix != SForm->cond.nmPix ||  SForm->cond.nucleus != nucleus ||
	   SForm->cond.tubeNetwork != tubeNetwork || SForm->cond.grid != grid ||
	   SForm->cond.rafts != rafts ||
	   SForm->cond.filopodia != filopodia ||  SForm->cond.lamella != lamella ||
	   SForm->cond.interTubeDistNm != interTubeDistNm ||
	   SForm->cond.tubeLengthNm != tubeLengthNm || SForm->cond.tubeRadNm != tubeRadNm ||
	   SForm->cond.tubes != tubes || SForm->cond.movingVesicles != movingVes ||
	   SForm->cond.sideGapNm != sideGapNm || SForm->cond.botGapNm != botGapNm ||
	   SForm->cond.topGapNm != topGapNm || SForm->cond.roundedCellBody !=roundedCellBody ||
	   SForm->cond.bodyFilletNm != bodyFilletNm ||
	   SForm->cond.outsideVesicles != outsideVes || SForm->cond.vesNum != vesNum ||
	   SForm->cond.vesRadNm != vesRadNm
	 )
	{ ShowMemb->Checked=false; currentZ=0;// new membrane
	  filopodia=SForm->cond.filopodia; lamella=SForm->cond.lamella;
	  interTubeDistNm=SForm->cond.interTubeDistNm; tubeLengthNm=SForm->cond.tubeLengthNm;
	  tubeRadNm=SForm->cond.tubeRadNm; tubes=SForm->cond.tubes;
	  cellXnm=SForm->cond.cellXnm;cellYnm=SForm->cond.cellYnm; cellZnm=SForm->cond.cellZnm;
	  roundedCellBody=SForm->cond.roundedCellBody; bodyFilletNm=SForm->cond.bodyFilletNm;
	  movingVes=SForm->cond.movingVesicles; outsideVes=SForm->cond.outsideVesicles;
	  vesNum=SForm->cond.vesNum;  vesRadNm=SForm->cond.vesRadNm;
      tubeRadNm=SForm->cond.tubeRadNm; nucleusRadNm=SForm->cond.nucleusRadNm;
	  IDum=SForm->cond.IDum;  nmVox=SForm->cond.voxelSize;
	  nmVox=SForm->cond.voxelSize; raftRadNm=SForm->cond.raftRadNm;
	  TrackBar->Max=cellZnm+botGapNm+topGapNm;
	 // nm_pix=SForm->cond.nm_pix;
	  flattening=SForm->cond.flattening;  nucleusRadNm=SForm->cond.nucleusRadNm;
	  nucleus=SForm->cond.nucleus;
	  tubeNetwork=SForm->cond.tubeNetwork; grid=SForm->cond.grid; rafts=SForm->cond.rafts;
	  sideGapNm=SForm->cond.sideGapNm; botGapNm=SForm->cond.botGapNm;
	  topGapNm=SForm->cond.topGapNm;
	  CalculateDimensions();
	  ShowMessage("Marix dimesions / structure changed => Remake Matrix => RESET");
	}
  if ( // cyto A prop
	   cytoAnum != SForm->cond.cytoAnum || cytoAinit != SForm->cond.cytoAinit ||
	   greenCytoAnum != SForm->cond.greenCytoAnum || SForm->cond.redCytoAnum != redCytoAnum ||
	   greenCytoAfrac != SForm->cond.greenCytoAfraction ||
	   redCytoAfrac != SForm->cond.redCytoAfraction ||
	   // cyto B props
	   SForm->cond.cytoBnum != cytoBnum || SForm->cond.cytoBinit != cytoBinit ||
	   greenCytoBnum != SForm->cond.greenCytoBnum || SForm->cond.redCytoBnum != redCytoBnum ||
	   greenCytoBfrac != SForm->cond.greenCytoBfraction ||
	   redCytoBfrac != SForm->cond.redCytoBfraction ||
	  // memb A check
	   membAnum != SForm->cond.membAnum || membAinit != SForm->cond.membAinit ||
	   greenMembAnum != SForm->cond.greenMembAnum || redMembAnum != SForm->cond.redMembAnum ||
	   greenMembAfrac != SForm->cond.greenMembAfraction ||
	   redMembAfrac  != SForm->cond.redMembAfraction ||
	   // memb B check
	   membBnum != SForm->cond.membBnum || membBinit != SForm->cond.membBinit ||
	   greenMembBnum != SForm->cond.greenMembBnum || redMembBnum != SForm->cond.redMembBnum ||
	   greenMembBfrac != SForm->cond.greenMembBfraction ||
	   redMembBfrac != SForm->cond.redMembBfraction ||
	   // memb C check
	   membCnum != SForm->cond.membCnum || SForm->cond.membCinit != membCinit ||
	   greenMembCnum != SForm->cond.greenMembCnum || redMembCnum != SForm->cond.redMembCnum ||
	   greenMembCfrac != SForm->cond.greenMembCfraction ||
	   redMembCfrac != SForm->cond.redMembCfraction   ||
	   // stat A check
	   statAnum != SForm->cond.statAnum || statAinit != SForm->cond.statAinit ||
	   greenStatAnum != SForm->cond.greenStatAnum || redStatAnum != SForm->cond.redStatAnum ||
	   greenStatAfrac != SForm->cond.greenStatAfrac ||
	   redStatAfrac  != SForm->cond.redStatAfrac ||
	   statApitchNm != SForm->cond.filamentPitchStepNm ||
	   SForm->cond.actinBundleExtendNm != actinBundleExtendNm
	) // set new critical paprams
   { // cyto A prop
	 cytoAnum=SForm->cond.cytoAnum;  cytoAinit=SForm->cond.cytoAinit;
	 greenCytoAnum=SForm->cond.greenCytoAnum; redCytoAnum=SForm->cond.redCytoAnum;
	 greenCytoAfrac=SForm->cond.greenCytoAfraction;
	 redCytoAfrac=SForm->cond.redCytoAfraction;
	// Cyto B props
	 cytoBnum=SForm->cond.cytoBnum; cytoBinit=SForm->cond.cytoBinit;
	 greenCytoBnum=SForm->cond.greenCytoBnum; redCytoBnum=SForm->cond.redCytoBnum;
	 greenCytoBfrac=SForm->cond.greenCytoBfraction;
	 redCytoBfrac=SForm->cond.redCytoBfraction;
	// memb A prop
	 membAnum=SForm->cond.membAnum;   membAinit=SForm->cond.membAinit;
	 greenMembAnum=SForm->cond.greenMembAnum;           redMembAnum=SForm->cond.redMembAnum;
	 greenMembAfrac=SForm->cond.greenMembAfraction; redMembAfrac=SForm->cond.redMembAfraction;
     spotNum=membAnum;
     if ( membAnum > MAX_SPOTS ) spotNum=MAX_SPOTS; // limit number of spots
	 // memb B prop
	 membBnum=SForm->cond.membBnum; membBinit=SForm->cond.membBinit;
	 greenMembBnum=SForm->cond.greenMembBnum;           redMembBnum=SForm->cond.redMembBnum;
	 greenMembBfrac=SForm->cond.greenMembBfraction; redMembBfrac=SForm->cond.redMembBfraction;
	  // memb C props
	 membCnum=SForm->cond.membCnum; membCinit=SForm->cond.membCinit;
	 greenMembCnum=SForm->cond.greenMembCnum; redMembCnum=SForm->cond.redMembCnum;
	 greenMembCfrac=SForm->cond.greenMembCfraction; redMembCfrac=SForm->cond.redMembCfraction;
	 // Stat A prop
	 statAnum=SForm->cond.statAnum;  statAinit=SForm->cond.statAinit;
	 statApitchNm=SForm->cond.filamentPitchStepNm;
	 greenStatAnum=SForm->cond.greenStatAnum;
	 redStatAnum=SForm->cond.redStatAnum;
	 greenStatAfrac=SForm->cond.greenStatAfrac;
	 redStatAfrac=SForm->cond.redStatAfrac;
	 actinBundleExtendNm=SForm->cond.actinBundleExtendNm;
	 ShowMessage("Object numbers / properties changed => RESET");
	}
  tmStep=SForm->cond.tmInterMs*0.001;
  // cell prop
  fenceJumpProb=SForm->cond.fenceJumpProb; raftMobRatio=SForm->cond.raftMobRatio;
  poreRadNm=SForm->cond.poreRadNm; bendTilt=SForm->cond.bendTilt;
  vesicleMob=SForm->cond.vesicleMob; transfer=SForm->TransferChBox->Checked;
  fuseVesicles=SForm->cond.fuseVes; recycling=SForm->cond.recycling;
  mitochondria=SForm->cond.mitochondria; tube3D=SForm->cond.tube3D;
  // imaging prop
  Kobj=SForm->cond.Kobj; EMCCDmode=SForm->cond.EMCCD; TIRangle=SForm->cond.TIRangle;
  noiseAmp=SForm->cond.noise;   gain=SForm->cond.gain;  FWHMnm=SForm->cond.FWHM;
  greenPhotonsPerSec=SForm->cond.greenPhotonsPerSec; redPhotonsPerSec=SForm->cond.redPhotonsPerSec;
  greenBleachRate=SForm->cond.greenBleachRate; redBleachRate=SForm->cond.redBleachRate;
  irisRadUm=SForm->cond.irisRadUm; illumFWHMum=SForm->cond.illumFWHMum;
  framesToSum=SForm->cond.framesToSum;
  twoCam=SForm->cond.twoCam; alterIllum=SForm->cond.alterIllum;
  d488=Calc_D(TIRangle,488);d561=Calc_D(TIRangle,561);
  greenBlinking=SForm->GreenBlinkingChBox->Checked;
  redBlinking=SForm->RedBlinkingChBox->Checked;
  FRET=SForm->FRETchBox->Checked; numOfDarkFrames=SForm->cond.numOfDarkFrames;
  greenBlinkONrate=SForm->cond.greenBlinkONrate;
  greenBlinkOFFrate=SForm->cond.greenBlinkOFFrate;
  // CytoA prop
  cytoAmob=SForm->cond.cytoAmob; cytoApoli=SForm->cond.cytoApoli;
  cytoAcanBindMembA=SForm->cond.cytoAcanBindMembA;
  cytoAcanBindMembB=SForm->cond.cytoAcanBindMembB;
  cytoAstepSizeNm=SForm->cond.cytoAstepSizeNm,cytoAstepRate=SForm->cond.cytoAstepRate;
  cytoApoliRate=SForm->cond.cytoApoliRate; cytoAdepoliRate=SForm->cond.cytoAdepoliRate;
  // CytoB prop
  cytoBmob=SForm->cond.cytoBmob; cytoAcanBindCytoB=SForm->cond.cytoAcanBindCytoB;
  cytoAcytoBbindRate=SForm->cond.cytoAcytoBbindRate;
  cytoAcytoBdissRate=SForm->cond.cytoAcytoBdissRate;
  cytoAcytoBdist=SForm->cond.cytoBdistA;
  // memb A prop
  membAmob=SForm->cond.membAmob;  membAorganPump=SForm->cond.membAorganPump;
  oneColourAA=SForm->cond.oneColourAA; membApoliNum=SForm->cond.membApoliNum;
  cytoAmembAbindRate=SForm->cond.cytoAmembAbindRate;
  cytoAmembAdissRate=SForm->cond.cytoAmembAdissRate;
  membAdepoliRate=SForm->cond.membAdepoliRate; membApoliRate=SForm->cond.membApoliRate;
  membApoli=SForm->cond.membApoli;
  membAcanBindMembB=SForm->cond.membAcanBindMembB;
  // memb B prop
  membBmob=SForm->cond.membBmob; membBinit=SForm->cond.membBinit;
  membAclusterNum=SForm->cond.membAclusterNum;
  membBcytoPump=SForm->cond.membBcytoPump;
  cytoAmembBbindRate=SForm->cond.cytoAmembBbindRate;
  cytoAmembBdissRate=SForm->cond.cytoAmembBdissRate;
  membAmembBbindRate=SForm->cond.membAmembBbindRate;
  membAmembBdissRate=SForm->cond.membAmembBdissRate;
  // memb C props
  membCmob=SForm->cond.membCmob; membCinit=SForm->cond.membCinit;
  membBmembCbindRate=SForm->cond.membBmembCbindRate;
  membBmembCdissRate=SForm->cond.membBmembCdissRate;
  membBcanBindMembC=SForm->cond.membBcanBindMembC;
  // stat A prop
  statAinit=SForm->cond.statAinit;
  monomersPerFilament=SForm->cond.monomersPerFilament;
  statApitchNm=SForm->cond.filamentPitchStepNm;
  cytoAstatAbindRate=SForm->cond.cytoAstatAbindRate;
  cytoAstatAdissRate=SForm->cond.cytoAstatAdissRate;
  cytoAcanMoveAlongStatA=SForm->cond.cytoAcanMoveAlongStatA;
  cytoAcanBindStatA=SForm->cond.cytoAcanBindStatA;
   monoPerFilo=SForm->cond.monoPerFilo;
  for ( int d=0; d < 502; d++ )
    timeDistMembAA[d]=timeDistCytoAmembA[d]=timeDistCytoAmembB[d]=timeDistMembAmembB[d]=0.0;
  for ( int d=0; d < 502; d++ )  timeDistCytoAstatA[d]=timeDistMembBmembC[d]=0.0;
  double rad=IDum*0.5; IAum2=PI*(rad*rad); IVum3=(4.0/3.0)*PI*(rad*rad*rad);
  for ( int d=0; d < 300; d++ ) // distance in nm
	stepDistProb[d]=Gauss(d-100-cytoAstepSizeNm,cytoAstepSizeNm*0.3);
  for ( int d=0; d < 502; d++ ) // time in usec
	{ rad=d; rad*=0.001;
      timeDistMembAA[d]=AIT2D(rad,membAmob*2.0,tmStep,IAum2);
      timeDistMembAmembB[d]=AIT2D(rad,membAmob+membBmob,tmStep,IAum2);
      timeDistMembBmembC[d]=AIT2D(rad,membBmob+membCmob,tmStep,IAum2);
      timeDistCytoAmembA[d]=AIT3D(rad,membAmob+cytoAmob,tmStep,IVum3);
	  timeDistCytoAmembB[d]=AIT3D(rad,membBmob+cytoAmob,tmStep,IVum3);
	  timeDistCytoAcytoB[d]=AIT3D(rad,cytoAmob+cytoBmob,tmStep,IVum3);
	  timeDistCytoAstatA[d]=AIT3D(rad,cytoAmob,tmStep,IVum3);
	}
 // Caption=timeDistCytoAmembA[0];
  FormResize(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SettingsMClick(TObject *Sender)
{ VarsToPrms();  SForm->cond.cancel=false;
  SForm->SetParam(); SForm->Statistics();
  SForm->ShowModal();
  if (  SForm->cond.cancel  ) return;  // no new settings
  PrmsToVars();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZoomOUTClick(TObject *Sender)
{ nmPix+=5; FormResize(Sender); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZoomINClick(TObject *Sender)
{ if ( nmPix < 10 ) return;
  nmPix-=5;
  int CCDwidth=sizeXpix,CCDheight=sizeYpix;
  if ( twoCam ) CCDwidth*=2;
  if ( CCDwidth > 2040 || CCDheight > 2040 )
	{ nmPix+=5; ShowMessage("Pixel size is too small for chosen cell sizes"); }
  FormResize(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowMembObjClick(TObject *Sender)
{ ShowMembObj->Checked=!ShowMembObj->Checked; StepCalc(); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::StepClick(TObject *Sender)
{  StepCalc(); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::TIRillumClick(TObject *Sender)
{  TIRillum->Checked=true; StepCalc(); Illum->Caption="Illum(TIR)"; FormPaint(Sender);}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EpiIllumClick(TObject *Sender)
{ EpiIllum->Checked=true; StepCalc(); Illum->Caption="Illum(Epi)"; FormPaint(Sender); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::ConfocalClick(TObject *Sender)
{ Confocal->Checked=true; StepCalc(); Illum->Caption="Illum(Conf)"; FormPaint(Sender); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowCytoObjClick(TObject *Sender)
{ ShowCytoObj->Checked=!ShowCytoObj->Checked;StepCalc(); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowFluorClick(TObject *Sender)
{ ShowFluor->Checked=!ShowFluor->Checked; StepCalc();
  if ( ShowFluor->Checked ) { BlueLaser->Enabled=true; GreenLaser->Enabled=true;  }
  else                      { BlueLaser->Enabled=false;GreenLaser->Enabled=false; }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowStaticObjClick(TObject *Sender)
{ ShowStaticObj->Checked=!ShowStaticObj->Checked;StepCalc(); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewRunClick(TObject *Sender)
{ SetCell(); frameCount=cycleNum=0; StepCalc(); }
//---------------------------------------------------------------------------
float __fastcall TMainForm::GaussRand()
{ static bool odd=false;
  static float val0,val1,w;
  if ( odd ) { odd=false; return val1*w; }
  do { val0 = ((float)(random(2000000)-1000000))/1000000.0;
       val1 = ((float)(random(2000000)-1000000))/1000000.0;
       w =val0*val0+val1*val1;
	 } while ( w >= 1.0 || w == 0.0 );
  w = sqrt( (-2.0 * log( w ) ) / w );
  odd=true;
  return val0*w;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WriteFrame(void)
{ int max=0,szX=matrixXnm/nmPix,szY=(matrixYnm)/nmPix;
  if ( twoCam && !SaveTwoChan->Checked ) szX*=2;
  prm.bit_pix=0;prm.width=szX,prm.height=szY; prm.fr_size=szX*szY;
  prm.fr_time=(cycleNum)*(tmStep*1000.001);
  prm.exp_in_ms=(tmStep*1000.01)*(float)framesToSum;
  prm.X_nm_pix=prm.Y_nm_pix=nmPix; prm.params&=128; // varible
  prm.Igain=gain; prm.addInfo=1;  addPrm.stageXnm=0; addPrm.stageYnm=0;
  addPrm.focusZnm=focusNm;   addPrm.TIRangle=TIRangle;
  if ( blueLaser ) prm.params|=1;
  if ( greenLaser ) prm.params|=2;
  WORD *buf16; byte *buf8;
  buf16 = new WORD[prm.fr_size];
  for ( int y=0,i=0; y < szY; y++ )
	for ( int x=0; x < szX; x++,i++ )
	  { if ( cell[x][y] > max ) max=cell[x][y];
		buf16[i]=cell[x][y];
	  }
  while ( (1<<(++prm.bit_pix)) <= max && prm.bit_pix < 16 );
  // prm.bit_pix=10;  /// for Kdiff image
  if ( twoCam && SaveTwoChan->Checked ) file=fileG;
  else                                  file=fileV;
  fwrite(&prm,sizeof(prm),1,file);fwrite(&addPrm,sizeof(addPrm),1,file);
  if (  prm.bit_pix < 9 )  // one byte per pixel
	{ buf8 = new byte[prm.fr_size];
	  for ( int i=0; i < prm.fr_size; i++ ) buf8[i]=buf16[i];
	  fwrite(buf8,prm.fr_size,1,file);
	}
  else { fwrite(buf16,prm.fr_size*2,1,file); } // two bytes per pixel
  if ( !twoCam || !SaveTwoChan->Checked ) return; // just one channel
  for ( int y=0,i=0; y < szY; y++ )
	for ( int x=szX; x < szX*2; x++,i++ )
	  { if ( cell[x][y] > max ) max=cell[x][y];
		buf16[i]=cell[x][y];
	  }
  prm.bit_pix=0;
  while ( (1<<(++prm.bit_pix)) <= max && prm.bit_pix < 16 );
  fwrite(&prm,sizeof(prm),1,fileR);fwrite(&addPrm,sizeof(addPrm),1,fileR);
  if (  prm.bit_pix < 9 )  // one byte per pixel
	{ buf8 = new byte[prm.fr_size];
	  for ( int i=0; i < prm.fr_size; i++ ) buf8[i]=buf16[i];
	  fwrite(buf8,prm.fr_size,1,fileR);
	}
  else { fwrite(buf16,prm.fr_size*2,1,fileR); } // two bytes per pixel
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveRecClick(TObject *Sender)
{ if ( !SaveDlg->Execute() ) return;
  if ( twoCam && SaveTwoChan->Checked ) // open gmG and gmR
	{ StrCopy(name,AnsiString(SaveDlg->FileName).c_str());
	  int index=strlen(name);
	  name[index-1]='G'; fileG=fopen(name,"wb");
	  name[index-1]='R'; fileR=fopen(name,"wb");
	  if ( !fileG || !fileR )
		{ fclose(fileG);fclose(fileR);ShowMessage("File G/R open ERROR");return; }
	}
  else
	{ fileV=fopen(AnsiString(SaveDlg->FileName).c_str(),"wb");
	  if ( !fileV ) { fclose(fileV); ShowMessage("File open ERROR"); return; }
	}
  frameCount=cycleNum=0;time=0.0;saveFrame=true;Caption="GMvCell-"+ SaveDlg->FileName;
  RunClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{ // ShowMessage(Key);
// ShowMessage( asin(1.4/1.52)/(PI/180.0));
  if ( Key == 107 && currentZ < matrixZ-1 ) { currentZ++; FormPaint(Sender); }
  if ( Key == 109 && currentZ > 0 ) { currentZ--;  FormPaint(Sender); }
  if ( Key == 27 ) { abort=true; Beep(); }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TrackBarChange(TObject *Sender)
{ TrackBar->PageSize=cellZnm/10; focusNm=TrackBar->Position; FormPaint(Sender); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::Bleaching(void)
{ int centX=cellXnm/2+sideGapNm,centY=sideGapNm+cellYnm/2,dX,dY;
  int boader=sideGapNm+cellXnm-bleachSizeNm,rad=bleachSizeNm/2;
  bool bleachGreen=false,bleachRed=false;
  for ( int i=0; i < membAnum; i++ )
	if ( MembA[i].z < botGapNm+15 || EpiIllum->Checked || Confocal->Checked )
	  { dX=MembA[i].x-centX; dY=MembA[i].y-centY;
		if (  bleachSpot && sqrt(dX*dX+dY*dY) < rad )
		  { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachSquare && abs(dX) < rad && abs(dY) < rad )
		  { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachStrip && MembA[i].x > boader )
		  { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachGreen )
		  for ( int j=0; j < 4; j++ ) MembA[i].green[j]=false;
		if ( bleachRed )
		  for ( int j=0; j < 4; j++ ) MembA[i].red[j]=false;
		bleachRed=bleachGreen=false;
	  }
  for ( int i=0; i < membBnum; i++ )
	if ( MembB[i].z < botGapNm+15 || EpiIllum->Checked || Confocal->Checked )
	  { dX=MembB[i].x-centX; dY=MembB[i].y-centY;
		if (  bleachSpot && sqrt(dX*dX+dY*dY) < rad )
		  { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachSquare && abs(dX) < rad && abs(dY) < rad )
		  { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachStrip && MembB[i].x > boader )
		  { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachGreen )
		  for ( int j=0; j < 4; j++ ) MembB[i].green[j]=false;
        if ( bleachRed )
          for ( int j=0; j < 4; j++ ) MembB[i].red[j]=false;
        bleachRed=bleachGreen=false;
      }
  if ( EpiIllum->Checked || Confocal->Checked )
	for ( int i=0; i < cytoAnum; i++ )
      { dX=CytoA[i].x-centX; dY=CytoA[i].y-centY;
        if (  bleachSpot && sqrt(dX*dX+dY*dY) < rad )
          { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachSquare && abs(dX) < rad && abs(dY) < rad )
          { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
		if ( bleachStrip && CytoA[i].x > boader )
          { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
        if ( bleachGreen )
		  for ( int j=0; j < 4; j++ ) CytoA[i].green[j]=false;
		if ( bleachRed )
		  for ( int j=0; j < 4; j++ ) CytoA[i].red[j]=false;
        bleachRed=bleachGreen=false;
      }
   if ( EpiIllum->Checked || Confocal->Checked )
	 for ( int i=0; i < statAnum; i++ )
       { dX=StatA[i].x-centX; dY=StatA[i].y-centY;
         if (  bleachSpot && sqrt(dX*dX+dY*dY) < rad )
           { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
         if ( bleachSquare && abs(dX) < rad && abs(dY) < rad )
           { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
         if ( bleachStrip && StatA[i].x > boader )
           { bleachGreen=blueLaser;  bleachRed=GreenLaser; }
         if ( bleachGreen )
		   for ( int j=0; j < 4; j++ ) StatA[i].green[j]=false;
         if ( bleachRed )
		   for ( int j=0; j < 4; j++ ) StatA[i].red[j]=false;
         bleachRed=bleachGreen=false;
       }
  bleachStrip=bleachSquare=bleachSpot=false;     
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BleachSpotClick(TObject *Sender)
{ bleachSpot=true; }
//---------------------------------------------------------------------------
void __fastcall TMainForm::BleachStripClick(TObject *Sender)
{ bleachStrip=true; }
//---------------------------------------------------------------------------
void __fastcall TMainForm::RenewCellClick(TObject *Sender)
{  SetCell(); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetBleachSizeClick(TObject *Sender)
{ double size=((double)bleachSizeNm)*0.001; UnicodeString str=size;
  if ( !InputQuery("Set Bleach Size ","Diam/Square/Strip Length (um)=",str) ) return;
  size=StrToFloatDef(str,1.0);
  if ( size > 20.0 || size < 0.1 )
	{ ShowMessage("Bleach Size is Out of Order (0.1-20 um)"); return; }
  bleachSizeNm=(size*1000.0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RUNmClick(TObject *Sender)
{ if ( abort ) { RUNm->Caption="STOP";  RunClick(Sender); }
  else {  abort=true;     RUNm->Caption="RUN"; }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveTracksInBMPClick(TObject *Sender)
{ if ( !SaveBmpDlg->Execute() ) return;
  hiResBmp->SaveToFile(SaveBmpDlg->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FillBmpClick(TObject *Sender)
{ FillBmp->Checked=!FillBmp->Checked; }
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadParametersClick(TObject *Sender)
{ ReadSettings(Sender); FormResize(Sender);Canvas->Font->Size=20;
  Canvas->TextOut(20,20,"Update Matrix if Geometry Changed !!!     ");

  return; // old part
  FILE *infile;
  if ( !OpenPrmDlg->Execute() ) return;
  infile=fopen(AnsiString(OpenPrmDlg->FileName).c_str(),"rb");
  if ( !infile ) { fclose(infile);  return; }
  fread(&SForm->cond,sizeof(SForm->cond),1,infile);
  fclose(infile); PrmsToVars();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveParametersClick(TObject *Sender)
{ WriteSettings(); return; // old part
  FILE *outfile;
  if ( !SavePrmDlg->Execute() ) return;
  outfile=fopen(AnsiString(SavePrmDlg->FileName).c_str(),"wb");
  if ( !outfile ){  fclose(outfile);  return; }
  VarsToPrms(); fwrite(&SForm->cond,sizeof(SForm->cond),1,outfile);
  fclose(outfile);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BleachSquareClick(TObject *Sender)
{ bleachSquare=true; }
//---------------------------------------------------------------------------
void __fastcall TMainForm::BlueLaserClick(TObject *Sender)
{ blueLaser=!blueLaser;
  if ( blueLaser ) BlueLaser->Caption="BlueLaser_ON";
  else             BlueLaser->Caption="BlueLaser_OFF";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::GreenLaserClick(TObject *Sender)
{ greenLaser=!greenLaser;
  if ( greenLaser ) GreenLaser->Caption="GrenLaser_ON";
  else              GreenLaser->Caption="GreenLaser_OFF";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveBmpSeqClick(TObject *Sender)
{ if ( !SaveBmpDlg->Execute() ) return;
  StrCopy(bmpName,AnsiString(SaveBmpDlg->FileName).c_str());
  for ( int i=(int)StrLen(bmpName); i > 0 ; i-- )
	   if ( bmpName[i] == '.' ) { bmpName[i]='\0'; break; }
  frameCount=cycleNum=0; time=0.0; saveBMP=true;
  Caption="GMvCell - "+ SaveBmpDlg->FileName;
  tX=100;tY=100.0; tZ=50.0; tEle=0; tAzi=0.0;
  RunClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ResetTimeClick(TObject *Sender)
{  time=setTime=0.0; frameCount=cycleNum=0; }
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveBindingStatClick(TObject *Sender)
{ if ( !SaveLogDlg->Execute() ) return;
  FILE *outfile;
  outfile=fopen(AnsiString(SaveLogDlg->FileName).c_str(),"wt");
  if ( !outfile ) return;
  AnsiString str="time\tCytoA-MembA\tCytoA-MembB\tCytoA-MembC\tCytoA-StatA\tMembA-MembB\tMembB-MembC\tMembAdimers\n";
  char txt[500]; double buf;
  StrCopy(txt,str.c_str()); fwrite(&txt,strlen(txt),1,outfile);
  for ( register int num=0; num < frameCount && num < 10000; num++ )
     { buf=tmStep*num; buf=((int)(buf*1000.0))*0.001; str=buf;
	   for ( int i=0; i < 7; i++ ) { str+="\t"; str+=stat[i][num]; }
	   str+="\n"; StrCopy(txt,str.c_str()); fwrite(&txt,strlen(txt),1,outfile);
     }
  fclose(outfile);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{ if ( ParamCount() > 0 )
	{ loadSettings=true;
	  AnsiString str=ParamStr(1);
	  StrCopy(settingsName,str.c_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::InitTimerTimer(TObject *Sender)
{ InitTimer->Enabled=false; /// run one time ONLY
  if ( loadSettings )
	{ int buf=StrToIntDef(settingsName,16);
	  if ( buf >=8 && buf <= 30 ) memorySizeMb=buf*1000;
	}
  matrix=NULL; Canvas->TextOut(400,20,"Allocating Main Memory Block");
  FormPaint(Sender);
  __int64 size=memorySizeMb*1e6;// MATRIX_SIZE;
  matrix = new byte[size];
  if ( matrix == NULL ) ShowMessage("Matrix - memory Alloc ERROR" );
 /*
  Canvas->TextOut(400,50,"Checking Main Memory Block           ");
  FormPaint(Sender);
  for ( __int64 i=0; i < size; i++ )
	{ if ( !(i%1000000) ) Canvas->TextOut(400,70,i/1000000);
	  matrix[i]=0;
	}
  */
  Canvas->TextOut(400,20,"                 READY                         ");
  vert = new VERT[VERT_LIM+10];
  if ( vert == NULL ) ShowMessage(" vertices Alloc ERROR" );
  return;
  if ( !loadSettings ) return;
  Caption=settingsName;
  FILE *infile;
  infile=fopen(settingsName,"rb");
  if ( !infile ) { fclose(infile); ShowMessage("Can not open Params");  return; }
  fread(&SForm->cond,sizeof(SForm->cond),1,infile);
  fclose(infile);  PrmsToVars(); FormResize(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowMembClick(TObject *Sender)
{ ShowMemb->Checked=!ShowMemb->Checked; FormPaint(0); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveVerticesClick(TObject *Sender)
{ FILE *outfile;
  if ( !SaveVertDlg->Execute() ) return;
  outfile=fopen(AnsiString(SaveVertDlg->FileName).c_str(),"wb");
  if ( !outfile ) return;
  for ( int i=0; i < vertNum; i++ )
	fwrite(&vert[i],sizeof(VERT),1,outfile);
  fclose(outfile);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadVerticesClick(TObject *Sender)
{ FILE *infile;  vertNum=0;
  if ( !OpenVertDlg->Execute() ) return;
  infile=fopen(AnsiString(OpenVertDlg->FileName).c_str(),"rb");
  if ( !infile ) return;
  while ( fread(&vert[vertNum],sizeof(VERT),1,infile) > 0 ) vertNum++;
  fclose(infile); Caption=vertNum;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadBMPtemplateClick(TObject *Sender)
{ if ( !OpenBmpDlg->Execute() ) return;
  Graphics::TBitmap *Bmp8=new Graphics::TBitmap();
  Bmp8->LoadFromFile(OpenBmpDlg->FileName);
  bmpWidth=Bmp8->Width;  bmpHeight=Bmp8->Height; delete bmpBuf; // clean old
  bmpBuf=new byte[bmpWidth*bmpHeight+20]; byte *ptr;
  for ( int y=0; y < bmpHeight; y++ )
	{ ptr=(byte *)Bmp8->ScanLine[y];
	  for ( int x=0; x < bmpWidth; x++ ) bmpBuf[x+(y*bmpWidth)]=(byte)ptr[x];
	}
   MakeBMPshell->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZscanClick(TObject *Sender)
{ Zscan->Checked=!Zscan->Checked; }
//---------------------------------------------------------------------------
void __fastcall TMainForm::Show3DviewClick(TObject *Sender)
{  Cell3D->Show(); }
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveMembScanBMPClick(TObject *Sender)
{ if ( !SaveBmpDlg->Execute() ) return;
  StrCopy(bmpName,AnsiString(SaveBmpDlg->FileName).c_str());
  for ( int i=(int)StrLen(bmpName); i > 0 ; i-- )
	   if ( bmpName[i] == '.' ) { bmpName[i]='\0'; break; }
  AnsiString str;
  for ( currentZ=0; currentZ < matrixZ-1; currentZ++ )
	{ FormPaint(Sender); str=bmpName; str+=currentZ; str+=".bmp";
	  CCD_Bmp->SaveToFile(str);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FissionClick(TObject *Sender)
{ Fission->Checked=!Fission->Checked; }
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveNframesOnlyClick(TObject *Sender)
{ int buf=frameLimit; UnicodeString str=buf,cap="Save N frames only, N=";
  if ( !InputQuery("Set Limit to Frames Recorded","Limit (N)=",str) ) return;
  buf=StrToIntDef(str,0);
  if ( buf < 0 )
	{ ShowMessage("Set N to 0 if you do not want to limit the record"); return; }
  frameLimit=buf; cap+=buf;SaveNframesOnly->Caption=cap;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetRandomRaftsClick(TObject *Sender)
{ __int64 left=sideGapVox,right=matrixX-sideGapVox;
  __int64 up=sideGapVox,down=matrixY-sideGapVox,bot=botGapVox,raftX,raftY,x,y;
  int raftRad=raftRadNm/nmVox;
  for ( int i=0; i < 10; i++ )
	{ raftX=random(cellXvox-raftRad*2)+left+raftRad;
	  raftY=random(cellYvox-raftRad*2)+up+raftRad;
	  for ( int yy=-raftRad; yy < raftRad; yy++ )
		for ( int xx=-raftRad; xx < raftRad; xx++ )
		  if ( sqrt(xx*xx+yy*yy) < raftRad )
			{ x=xx+raftX; y=yy+raftY;
			  for ( __int64 z=bot-2; z < bot+5; z++ )
				if ( matrix[x+(y*matrixX)+(z*matrixXY)] == CELL_MEMB )
				  matrix[x+(y*matrixX)+(z*matrixXY)]=RAFT_MEMB;
			}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveGMIClick(TObject *Sender)
{ FILE *outfile; float buf=0.0; int intBuf=2022,addInfo=8; // new format 2022
  if ( !SaveGMIdlg->Execute() ) return;
  outfile=fopen(AnsiString(SaveGMIdlg->FileName).c_str(),"wb");
  if ( !outfile )
	{ ShowMessage("Can not Open GMI File");  return; }
  int len=frameCount;
  if ( len > 10000 ) len=10000;

  for ( int num=0; num < spotNum; num++ )
	 { fwrite(&intBuf,4,1,outfile); // new format 2022
	   intBuf=spot[num].len=len; fwrite(&intBuf,4,1,outfile); // length
	   intBuf=spot[num].first=1; fwrite(&intBuf,4,1,outfile); // first data point
	   intBuf=addInfo;           fwrite(&intBuf,4,1,outfile); // additinal data
	   fwrite(&tm[spot[num].first],4*spot[num].len,1,outfile); // time (seconds)
	   fwrite(&spot[num].spInt[spot[num].first],4*spot[num].len,1,outfile);
	   fwrite(&spot[num].cX[spot[num].first],4*spot[num].len,1,outfile);
	   fwrite(&spot[num].cY[spot[num].first],4*spot[num].len,1,outfile);
	 }
  fclose(outfile);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveMatrixClick(TObject *Sender)
{ FILE *outfile; byte buf[2050],val=matrix[0],comp=0; int sz=0;
  if ( !SaveMatrixDlg->Execute() ) return;
  outfile=fopen(AnsiString(SaveMatrixDlg->FileName).c_str(),"wb");
  if ( !outfile ) return;
  Mat.sizeX=matrixX;  Mat.sizeY=matrixY;  Mat.sizeZ=matrixZ;
  fwrite(&Mat,sizeof(MATRIX),1,outfile);
  for ( __int64 i=0; i < matrixSize; )
	{ while ( val == matrix[i] && comp < 250 ) { comp++; i++; }
	  buf[sz]=val; buf[sz+1]=comp; sz+=2; comp=0; val=matrix[i];
	  if ( sz >= 2000 ) { fwrite(&buf,sz,1,outfile);sz=0;  }
	}
  if ( sz ) fwrite(&buf,sz,1,outfile); // remaning part
  fclose(outfile);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadMatrixClick(TObject *Sender)
{ FILE *infile;
  if ( !OpenMatrixDlg->Execute() ) return;
  infile=fopen(AnsiString(OpenMatrixDlg->FileName).c_str(),"rb");
  if ( !infile ) return;
  fread(&Mat,sizeof(MATRIX),1,infile);
  __int64 newSize=Mat.sizeX; newSize*=Mat.sizeY; newSize*=Mat.sizeZ;
  __int64 oldSize=memorySizeMb*1e6,i=0; byte buf,comp;
  if ( newSize > oldSize )
	{	ShowMessage("New matrix is too big -> increase memory to"); return; }
  matrixX=Mat.sizeX; matrixY=Mat.sizeY; matrixZ=Mat.sizeZ;
  matrixSize=newSize;
  for ( __int64 i=0; i < newSize; i++ ) matrix[i]=0; // zero the matrix
  while ( fread(&buf,1,1,infile) > 0 ) // read and unpack
	{ fread(&comp,1,1,infile);
	  for ( int j=0; j < comp; j++ ) matrix[i++]=buf;
	}
  fclose(infile);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveTwoChanClick(TObject *Sender)
{ SaveTwoChan->Checked=!SaveTwoChan->Checked; }
//---------------------------------------------------------------------------


