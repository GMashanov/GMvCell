//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <stdio.h>
#include <Filectrl.hpp>
#include <ExtCtrls.hpp>

#define PI 3.14159265359
#define avogadro 6.0221415E23
#define kT 4.1e-21
#define KbT 0.041  // µN µm-1
#define CYTO_NUM 50000
#define MEMB_NUM 50000
#define STAT_NUM 50000
#define REC_LEN 200
#define CELL_MEMB 254
#define NUCL_MEMB 253
#define TUBE_MEMB 252
#define VESI_MEMB 251
#define RAFT_MEMB 100
#define STAT_VOXEL 50
#define EXTRACELL 0
#define CYTOPLASM 1
#define NUCLEOPLASM 3
#define T_LUMEN 4
#define V_LUMEN 5
#define GREEN 1
#define RED 2        // // //
#define MATRIX_SIZE 16000000000 // 16 Gigacells (@1byte) - 16GByte
#define VERT_LIM 50000000 // 50 mega vertices
#define SEED_LIM 1000000 //  1 mega seeds
#define	MAX_SPOTS 2000  // spots in gmi file
#define RtoDeg 57.295779513082320876798154814105

class Obj
{ public:
  bool green[4],red[4];
  bool openEnd,darkState;
  int cytoAind,cytoBind,vesicleInd,medium;
  int membAind,membBind,membCind,statAind,siteAind,siteBind;
  float *xPosUm,*yPosUm;
  float x,y,z,azi,ele;
};
//----------------------------------------------------------------------
 typedef struct { int sizeX,sizeY,sizeZ;
				  int int1,int2,int3,int4,int5;
                  double dob1,dob2,dob3,dob4;
                } MATRIX;
//-----------------------------------------------------------------
 typedef struct { bool docked,fused,empty;
				  int xVox,yVox,zVox,iniXvox,iniYvox,iniZvox,radVox;
				  float x,y,z,radNm,mob,dockTime,azimuth,elevation;
				} VESICLE;
				//-----------------------------------------------------------------
 typedef struct { bool docked,fused;
				  int xVox,yVox,zVox;
				  float *x,*y,*z,*azi,*ele,radNm;
				} FILO;
//-------------------------------------------------------------
 typedef struct { bool good,closed,engaged,selected;
				 int x,y,num,first,len,cent,grp,tagSpNum,redInd;
				 float greenInt,dist,distToEdge,averRedInt,Dlat,averVel,MSD0,angle,absX,absY;
				 float *spInt,*cX,*cY,*redInt;} SPOT;  // 74 bytes
 //---------------------------------------------------------------------------
 typedef struct { int leftX,width,topY,height,exp_in_ms,fr_size,fr_time; // 28 bytes
                  float X_nm_pix,Y_nm_pix;  // 8 bytes
                  byte Igain,Vgain,bit_pix,bin;      // 4 bytes
				  byte params; // bit0 - Laser 1 ON/OFF (1=ON)
                               // bit1 - Laser 2 ON/OFF (1=ON)
                               // bit2 - Laser 3 (Lamp) ON/OFF (1=ON)
                               // bit3 - Median filter ON/OFF (1=ON)
                               // bit4 - Neutral density filter ON/OFF (1=ON)
                               // bit5 - Gated illumination (part of Frame (1=ON)
							   // bit6 - NEW FORMAT(1=NEW)
				  byte addInfo;
				  WORD  las_power,temperature,illumTime;  // 6 bytes (illimTime in ms)
						 } PRM;  // 48 byts all together
 //-------------------------------------------------------
 typedef struct { int stageXnm,stageYnm,focusZnm,int1,int2,int3; // 24 bytes
				  float TIRangle,BeamZoom,float3,float4;  // 16 bytes
				} ADD_PRM;  // 40 byts all together
 // vertex structure
 typedef struct { int type;    // what king of membrane
				  int nInd[15];  // neighbour index 60 bytes
				  float x,y,z,sum; } VERT; // 16 bytes
 typedef struct { float x,y,dX,dY,dirX; bool done; } SEED; //
 //----------------------------------- main settings structure
 typedef struct { // cell properties
				  bool nucleus,tubeNetwork,grid,rafts,boolRes,filopodia,lamella;
				  bool tubes,roundedCellBody,movingVesicles,outsideVesicles;
				  bool mitochondria,tube3D,recycling,fuseVes,bool10;
				  int cellXnm,cellYnm,cellZnm,voxelSize,interTubeDistNm;
				  int botGapNm,sideGapNm,topGapNm;
				  int tubeRadNm,tubeLengthNm,nucleusRadNm,raftRadNm,memorySizeMb;
				  int poreRadNm,bodyFilletNm,vesNum,vesRadNm,tubeGapNm,int6,int7,int8,int9,int10;
				  double raftMobRatio,IDum,flattening;
				  double vesicleReleaseRate,vesicleDockingRate,filamentPitchStepNm;
				  double Kobj,cellD1,bendTilt,vesicleMob,cellD3,cellD4,cellD5,cellD6;
				  /// illumination and imaging
				  bool cancel,EMCCD,alterIllum,twoCam;
				  int gain,illumRes1,fenceJumpProb,illumRes2,FWHM,nmPix;
				  int greenPhotonsPerSec,redPhotonsPerSec,framesToSum,numOfDarkFrames;
				  double greenBleachRate,redBleachRate,noise,tmInterMs,TIRangle;
				  // cyto A prop
				  bool cytoAcanBindMembA,cytoAcanBindMembB,cytoAcanBindMembC;
				  bool cytoAcanMoveAlongStatA,cytoAcanBindStatA,cytoApoli,cytoApoli3D;
				  bool rBcytoA3,rBcytoA4,rBcytoA5,rBcytoA6,rBcytoA7,rBcytoA8;
				  int cytoAnum,greenCytoAnum,greenCytoAfraction,redCytoAnum,redCytoAfraction;
				  int cytoAinit,rIcytoA2,rIcytoA3,rIcytoA4;
				  double cytoAmob,cytoAmembAbindRate,cytoAmembAdissRate;
				  double cytoAmembBbindRate,cytoAmembBdissRate,cytoAstepSizeNm,cytoAstepRate;
				  double cytoAstatAbindRate,cytoAstatAdissRate,cytoApoliRate,cytoAdepoliRate;
				  // cyto B prop
				  bool cytoBCanBindMembA,cytoBcanBindMembB,cytoBcanBindMembC;
				  bool cytoBcanBindStatA,cytoBreserve,cytoAcanBindCytoB,rBcytoB2,rBcytoB3;
				  int cytoBnum,greenCytoBnum,greenCytoBfraction,redCytoBnum,redCytoBfraction;
				  int cytoBinit,resIcytoB2,resIcytoB3,resIcytoB4,resIcytoB5,resIcytoB6;
				  double cytoBmob,cytoBmembAbindRate,cytoBmembAdissRate;
				  double cytoBmembBbindRate,cytoBmembBdissRate,cytoBdistA;
				  double cytoBstatAbindRate,cytoBstatAdissRate,resDcytoB2;
				  double cytoAcytoBbindRate,cytoAcytoBdissRate,resDcytoB3;
				  // memb A prop
				  bool membApoli,membAcanBindMembB,oneColourAA,membAorganPump;
				  bool resBmembA2,resBmembA3,resBmembA4,resBmembA5,resBmembA6;
				  int membAnum,greenMembAnum,greenMembAfraction,redMembAnum,redMembAfraction;
				  int membAinit;
				  int monomersPerFilament,membApoliNum,membAclusterNum,rImembA3,rImembA4;
				  double membAmob,membAmembBbindRate,membAmembBdissRate,rDmembA4;
				  double membApoliRate,membAdepoliRate,rDmembA1,rDmembA2,rDmembA3;
				  // memb B prop
				  bool membBformDimers,membBcytoPump,resBmembB2,resBmembB3,resBmembB4;
				  int membBnum,membBinit,resBint;
				  int greenMembBnum,greenMembBfraction,redMembBnum,redMembBfraction;
				  int resImembB1,resImembB2,resImembB3;
				  double membBmob,membBBbindRate,membBBdissRate,resDmembB1,resDmembB2;
				  // memb c prop
				  bool membAcanBindMembC,membBcanBindMembC,membCtubePump,resBmembC2;
				  int membCnum,membCinit,resImembC1,resImembC2,resImembC3,resImembC4;
				  int greenMembCnum,greenMembCfraction,redMembCnum,redMembCfraction;
				  double membCmob,membBmembCbindRate,membBmembCdissRate;
				  double membAmembCbindRate,membAmembCdissRate,resDmembC3,resDmembC4,resDmembC5;
				  // Stat A prop
				  bool resBstatA1,resBstatA2,resBstatA3,resBstatA4,resBstatA5;
				  int statAnum,statAinit,actinBundleExtendNm,monoPerFilo,resIstatA3,resIstatA4;
				  int greenStatAnum,greenStatAfrac,redStatAnum,redStatAfrac;
				  double rDstatA1,rDstatA2,rDstatA3,rDstatA4,rDstatA5,rDstatA6;

				 // reserve
				  bool resB0,resB1, resB2,resB3,resB4,resB5,resB6,resB7,resB8,resB9;
				  int resI2,resI3,resI4,resI5,resI6,resI7,resI8,resI9,resI10;
				  double greenBlinkONrate,greenBlinkOFFrate,FRETratePercent,irisRadUm,illumFWHMum,resD6,resD7,resD8,resD9;
				} COND;

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *PopupMenu;
        TMenuItem *Param;
        TMenuItem *Run;
        TMenuItem *Step;
        TMenuItem *ShowFluor;
        TMenuItem *NewRun;
        TStatusBar *StBar;
        TSaveDialog *SaveDlg;
        TMainMenu *MainMenu;
        TMenuItem *File;
        TMenuItem *View;
        TMenuItem *ZoomIN;
        TMenuItem *ZoomOUT;
	TMenuItem *ShowMembObj;
	TMenuItem *ShowCytoObj;
        TMenuItem *SaveRec;
        TTrackBar *TrackBar;
        TMenuItem *Bleach;
        TMenuItem *Illum;
        TMenuItem *EpiIllum;
        TMenuItem *Confocal;
        TMenuItem *TIRillum;
        TMenuItem *Settings;
        TMenuItem *RenewCell;
        TSaveDialog *SaveGMIdlg;
        TMenuItem *BleachSpot;
        TMenuItem *SetBleachSize;
        TMenuItem *BleachStrip;
        TMenuItem *SettingsM;
        TMenuItem *RUNm;
        TMenuItem *SaveTracksInBMP;
        TSavePictureDialog *SaveBmpDlg;
        TMenuItem *FillBmp;
        TSaveDialog *SaveLogDlg;
	TTimer *InitTimer;
        TMenuItem *LoadParameters;
        TOpenDialog *OpenPrmDlg;
        TSaveDialog *SavePrmDlg;
        TMenuItem *SaveParameters;
        TMenuItem *BleachSquare;
        TStatusBar *InfoBar;
        TMenuItem *BlueLaser;
        TMenuItem *GreenLaser;
        TMenuItem *ShowStaticObj;
        TMenuItem *SaveBmpSeq;
        TMenuItem *ResetTime;
        TMenuItem *SaveBindingStat;
	TMenuItem *SetMemb;
	TMenuItem *ShowMemb;
	TMenuItem *Membrane;
	TMenuItem *Rounding;
	TMenuItem *Thickening;
	TMenuItem *SaveVertices;
	TSaveDialog *SaveVertDlg;
	TMenuItem *LoadVertices;
	TOpenDialog *OpenVertDlg;
	TMenuItem *BigRounding;
	TMenuItem *LoadBMPtemplate;
	TOpenPictureDialog *OpenBmpDlg;
	TMenuItem *MakeBMPshell;
	TProgressBar *ProgBar;
	TMenuItem *Zscan;
	TMenuItem *MembFromVertices;
	TMenuItem *Show3Dview;
	TMenuItem *SaveMembScanBMP;
	TMenuItem *Fission;
	TMenuItem *SetMembVertices;
	TMenuItem *SaveNframesOnly;
	TMenuItem *SetRandomRafts;
	TMenuItem *SaveGMI;
	TMenuItem *SaveMatrix;
	TMenuItem *LoadMatrix;
	TOpenDialog *OpenMatrixDlg;
	TSaveDialog *SaveMatrixDlg;
	TMenuItem *SaveTwoChan;
        void __fastcall RunClick(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall ZoomINClick(TObject *Sender);
        void __fastcall ZoomOUTClick(TObject *Sender);
        void __fastcall ShowMembObjClick(TObject *Sender);
        void __fastcall StepClick(TObject *Sender);
        void __fastcall ShowCytoObjClick(TObject *Sender);
        void __fastcall ShowFluorClick(TObject *Sender);
        void __fastcall NewRunClick(TObject *Sender);
        void __fastcall SaveRecClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall TrackBarChange(TObject *Sender);
        void __fastcall EpiIllumClick(TObject *Sender);
        void __fastcall ConfocalClick(TObject *Sender);
        void __fastcall TIRillumClick(TObject *Sender);
        void __fastcall RenewCellClick(TObject *Sender);
        void __fastcall BleachSpotClick(TObject *Sender);
        void __fastcall SetBleachSizeClick(TObject *Sender);
        void __fastcall BleachStripClick(TObject *Sender);
        void __fastcall SettingsMClick(TObject *Sender);
        void __fastcall RUNmClick(TObject *Sender);
        void __fastcall SaveTracksInBMPClick(TObject *Sender);
        void __fastcall FillBmpClick(TObject *Sender);
        void __fastcall LoadParametersClick(TObject *Sender);
        void __fastcall SaveParametersClick(TObject *Sender);
        void __fastcall BleachSquareClick(TObject *Sender);
        void __fastcall BlueLaserClick(TObject *Sender);
        void __fastcall GreenLaserClick(TObject *Sender);
        void __fastcall ShowStaticObjClick(TObject *Sender);
        void __fastcall SaveBmpSeqClick(TObject *Sender);
        void __fastcall ResetTimeClick(TObject *Sender);
        void __fastcall SaveBindingStatClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall InitTimerTimer(TObject *Sender);
	void __fastcall SetMembClick(TObject *Sender);
	void __fastcall ShowMembClick(TObject *Sender);
	void __fastcall RoundingClick(TObject *Sender);
	void __fastcall ThickeningClick(TObject *Sender);
	void __fastcall SaveVerticesClick(TObject *Sender);
	void __fastcall LoadVerticesClick(TObject *Sender);
	void __fastcall LoadBMPtemplateClick(TObject *Sender);
	void __fastcall MakeBMPshellClick(TObject *Sender);
	void __fastcall ZscanClick(TObject *Sender);
	void __fastcall MembFromVerticesClick(TObject *Sender);
	void __fastcall Show3DviewClick(TObject *Sender);
	void __fastcall SaveMembScanBMPClick(TObject *Sender);
	void __fastcall FissionClick(TObject *Sender);
	void __fastcall SetMembVerticesClick(TObject *Sender);
	void __fastcall SaveNframesOnlyClick(TObject *Sender);
	void __fastcall SetRandomRaftsClick(TObject *Sender);
	void __fastcall SaveGMIClick(TObject *Sender);
	void __fastcall SaveMatrixClick(TObject *Sender);
	void __fastcall LoadMatrixClick(TObject *Sender);
	void __fastcall SaveTwoChanClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
 inline double SIN(double ang) { return sin((PI/180.0)*ang);}
 inline double COS(double ang) { return cos((PI/180.0)*ang);}
 inline double TAN(double ang) { return tan((PI/180.0)*ang);}
 inline double AbsD(double val){ if ( val > 0.0 ) return val; return -val; }
 inline double Calc_D(double ang,double wavelength)
   { return (wavelength*1e-9)/(4.0*PI*sqrt(2.3043*(SIN(ang)*SIN(ang))-1.7689)); }
 inline void rotateXY(double x,double y,double ang,double *nX,double *nY)
   { *nX=x*COS(ang)-y*SIN(ang); *nY=x*SIN(ang)+y*COS(ang); }
 inline void Sph2cart(double P,double azi,double ele,double *x,double *y,double *z)
   { *x+=P*COS(azi)*COS(ele); *y+=P*SIN(azi)*COS(ele); *z+=P*SIN(ele); }
 inline void Cart2sph( double x,double y,double z,double *P,double *azi,double *ele)
   { *azi=atan2(y,x)*RtoDeg; *ele=atan2(z,sqrt(x*x+y*y))*RtoDeg; *P=sqrt(x*x+y*y+z*z); }
 inline void MoveObj(Obj *obj,float dX,float dY,float dZ)
   { obj->x+=dX; obj->y+=dY; obj->z+=dZ; }

//-------------------------------------------
 __fastcall TMainForm(TComponent* Owner);
 double DiffCalc(double d_time, double k_diff, double area,double d_conc,double d_x);
 double DistCalc(double time,double k_diff, double dist);
 void __fastcall CellPaint(Graphics::TBitmap *bmp);
 void StepCalc(void);
 void SetCell(void);
 int SetObjectProps(Obj *obj,int mType,int greenNum,int greenFrac,int redNum,int redFrac,int num);
 void ObjToVes(Obj *obj,int num,int shift);
 void SetMembranes(void);
 float __fastcall JumpSizeNm(double t_step, double diffCoef);
 void __fastcall PrintBmp(Graphics::TBitmap *PrintBmp);
 int __fastcall Emission(float x,float y,float z,float focus,float intens,int color);
 void __fastcall Zsort(void);
 void __fastcall WriteFrame();
 float __fastcall GaussRand();
 void CalcMembMovement(Obj *obj,float mob, int num);
 void __fastcall Bleaching(void);
 void EMCCD(void);
 void CytoAdynamics(void);
 void CytoBdynamics(void);
 void MembAdynamics(void);
 void MembBdynamics(void);
 void MembCdynamics(void);
 void StatAdynamics(void);
 void MoveCytoAalongStatA(float step);
 void  VarsToPrms(void);
 void  PrmsToVars(void);
 void __fastcall DrawFluor(Obj *obj,int num);
 void DrawMembrane(Graphics::TBitmap *bmp);
 void CalculateDimensions(void);
 bool CheckForMemb(float x,float y,float z,float dX,float dY, float dZ);
 bool CheckForGap(float x,float y,float z,float dX,float dY,float dZ);
 void CheckPathM(float x,float y,float z,float *dX,float *dY,float *dZ);
 void CheckPathC(float x,float y,float z,float *dX,float *dY,float *dZ);
 bool SetVertices(void);
 void SetMembranes(int choice);
 bool MakeBMPshape(void);
 bool MakeBody(void);
  int NeighbXYZ(int index, double *x,double *y,double *z);
 void FindFreeVoxel(float x,float y, float z,float *nX,float *nY, float *nZ);
 void MakeVesicle(int cX,int cY,int cZ,int rad,float shape, byte memb,byte lumen);
 void MoveVesicle(int ind);
 void GenerateVesicle(int ind);
 void FuseVesicle(int ind);
 void Fuse(int ind);
 bool FindFuseVoxel(int ind);
 void TubeFission(int rad,int cXvox,int cYvox,int cZvox,float stage);
 bool MoveToNearestVoxel(Obj *obj,int dist,int medium);
 void FillSphere(int rad,int cX,int cY,int cZ,int shell,int lumen,int noGo);
 void MakeRoundedShell(int X1,int X2,int Y1,int Y2,int Z1,int Z2,int rad);
 void MakeTubeNetwork(int rad);
 bool FindTube(int rad,int x,int y,int z,float azi,float ele,float *dAzi,float *dEle);
 void MakeFilopodia(int rad);
 void FiloDynamics(int rad);
 bool MakeNucleus(byte *matrix,int rad,float flattening);
 bool CheckSphere(int rad,int cX,int cY,int cZ,int med1,int med2,int med3);
 void MakeVGap(int xO,int yO,int zO,int rad,int gap,int medium,int memb);
 void MakeHGap(int xO,int yO,int zO,int rad,int gap,int lumen,int memb);
 void MakeRing(float cX,float cY,float cZ,int rad,float horAng,float vertAng);
 // See RWconditions.cpp
 void ReadSettings(TObject *Sender);
 void StrToParam(AnsiString str);
 void WriteSettings(void);
 //-----------------------------------
 VERT *vert; // poiter to vertices array
 //VERT vert[VERT_LIM+10];  // 25 mega vertices
 SEED seed[SEED_LIM];  int seedNum;
 Obj CytoA[CYTO_NUM+10],CytoB[CYTO_NUM+10];
 Obj MembA[MEMB_NUM+10],MembB[MEMB_NUM+10],MembC[MEMB_NUM+10];
 Obj StatA[STAT_NUM+10];
 MATRIX Mat;
 FILE *file,*fileG,*fileR,*fileV,*logfile;
 Graphics::TBitmap *CCD_Bmp,*maskBmp,*hiResBmp,*Bmp24;
 PRM prm; ADD_PRM addPrm;
 VESICLE vesicle[155];
 FILO filo[155];
 SPOT spot[MAX_SPOTS+5];
 byte *matrix;
 byte *bmpBuf;
 int *layer[3];
 WORD cell[2050][2050],intCCD[2050][2050]; // cell elements
 AnsiString note;
 char LogFileName[256],bmpName[250],settingsName[256],name[300];
 bool abort,las_open,showSites,tir,saveFrame,saveBMP,saveXY,loadSettings;
 float tm[10005];
 // matrix properties
 bool membOK,verticesReady; int vertZind[10000];
 int nmVox,bmpWidth,bmpHeight,vertNum,memorySizeMb;
 __int64 matrixX,matrixY,matrixZ,matrixXY,matrixSize,currentZ;
 // Cell properties
 bool grid,rafts,tubeNetwork,filopodia,transfer;
 bool nucleus,lamella,tubes,roundedCellBody,movingVes,fuseVesicles;
 bool outsideVes,mitochondria,tube3D,recycling;
 int cellXnm,cellYnm,cellZnm,tubeRadNm,sizeXpix,sizeYpix,sizeZpix;
 int cellXvox,cellYvox,cellZvox;
 int sideGapVox,botGapVox,topGapVox,botGapNm,sideGapNm,topGapNm,tubeLenVox;
 int interTubeDistNm,vesRadNm,bodyFilletNm,vesNum,raftRadNm;
 int tubeLengthNm,matrixXnm,matrixYnm,matrixZnm,tubeGapNm,poreRadNm,poreRadVox;
 int tubeGapVox,tubeRadVox,tubeZminVox,tubeZmaxVox;
 double flattening,nucleusRadNm,bendTilt,raftMobRatio,vesicleMob;
 double fissionCoef,tX,tY,tZ,tAzi,tEle;

 // Illumination and Imaging prop
 bool EMCCDmode,bleachSquare,bleachSpot,bleachStrip,twoCam,alterIllum,FRET;
 bool blueLaser,greenLaser,redLaser,twoAngles,greenBlinking,redBlinking;
 int gain,greenPhotonsPerSec,redPhotonsPerSec,frameLimit;
 int x_num,x_shift,numOFsites,nmPix,cycleNum,frameCount,intVar;
 int focusNm,gridSizeNm,bleachSizeNm;
 int framesToSum,numOfDarkFrames,ZscanDir,scanCount;
 double greenBleachRate,redBleachRate,greenBleachPerFrame,redBleachPerFrame;
 double greenPhotons,redPhotons;
 double irisRadUm,IDum,IAum2,IVum3,Kobj;
 double d488,d561,FWHMnm,illumFWHMum,noiseAmp,TIRangle;
 double greenBlinkONrate,greenBlinkOFFrate,FRETratePercent;
 double greenONperFrame,greenOFFperFrame;
 // Cyto A Obj properties
 bool cytoAcanBindMembA,cytoAcanBindMembB,cytoAcanBindStatA,cytoAinsideTube;
 bool cytoApoli;
 int cytoAinit;
 int cytoAnum,greenCytoAnum,redCytoAnum,greenCytoAfrac,redCytoAfrac,cytoApoliNum;
 double cytoAmob,cytoAmembAbindRate,cytoAmembAdissRate,cytoAmembBbindRate,cytoAmembBdissRate;
 double cytoAstatAbindRate,cytoAstatAdissRate,cytoAstepSizeNm,cytoAstepRate;
 double cytoAcytoBbindRate,cytoAcytoBdissRate,jumpCytoA,cytoApoliRate,cytoAdepoliRate;
 // Cyto B  properties
 bool cytoAcanBindCytoB,cytoBcanBindMembA,cytoBcanBindMembB,cytoBcanBindStatA;
 int cytoBnum,cytoBinit,greenCytoBnum,redCytoBnum,greenCytoBfrac,redCytoBfrac;
 double cytoBmob,cytoBmembAbindRate,cytoBmembAdissRate;
 double cytoBmembCbindRate,cytoBmembCdissRate,cytoBmembBbindRate,cytoBmembBdissRate;
 double cytoBstatAbindRate,cytoBstatAdissRate,jumpCytoAB,cytoAcytoBdist;
 // Memb A properties
 bool membApoli,membAcanBindMembB,oneColourAA,membAorganPump;
 int membAnum,membAinit,monomersPerFilament,spotNum,membApoliNum,membAclusterNum;
 int greenMembAfrac,greenMembAnum,redMembAfrac,redMembAnum;
 double membAmob,membApoliRate,membAdepoliRate;
 double membAmembBbindRate,membAmembBdissRate,jumpMembA;
 // Memb B properties
 bool membBcytoPump;
 int membBnum,membBinit,fenceJumpProb;
 int greenMembBfrac,greenMembBnum,redMembBfrac,redMembBnum;
 double membBmob,jumpMembAB;
 // Memb C properties
 bool membBcanBindMembC;
 int membCnum,membCinit;
 int greenMembCfrac,greenMembCnum,redMembCfrac,redMembCnum;
 double membCmob,membBmembCbindRate,membBmembCdissRate,jumpMembBmembC,jumpMembABC;
 // Static A properties
 bool cytoAcanMoveAlongStatA;
 int statAnum,statAinit,monoPerFilo,actinBundleExtendNm;
 int greenStatAfrac,greenStatAnum,redStatAfrac,redStatAnum;
 // Statistic
 int membAA[100],cytoAboundToStatAnum,stat[10][10000];
 int cytoAboundToMembAnum,cytoAboundToMembBnum,cytoAboundToMembCnum;
 int cytoBboundToMembAnum,cytoBboundToMembBnum,cytoBboundToMembCnum;
 int membAboundToMembBnum,membBboundToMembCnum,membAboundMembA;
 int cytoBboundToStatAnum,cytoAboundToCytoBnum,cytoAinOrgan,vesiGen;
 float delta[STAT_NUM+5];
 double membAdens,membAunitsInPoli,statApitchNm;
 double timeDistMembAA[505],timeDistCytoAmembA[505],timeDistCytoAmembB[505];
 double timeDistMembAmembB[505],timeDistCytoAstatA[505],timeDistMembBmembC[505];
 double timeDistCytoAcytoB[505],stepDistProb[303];
 double area,time,tmStep,avX,avY,setTime,unwindingRate;
 double ini_conc,common,vesicleReleaseRate,vesicleDockingRate;
 double moleculeReleaseRate;

//---------------------------------------
double Round(double val,int digits)
{ double mult=pow(10.0,digits);
  double intRes=(__int64)(val*mult);
  if ( val > 0.0 && (val*mult)-intRes > 0.5 ) intRes+=1.0;
  if ( val < 0.0 && (val*mult)-intRes < -0.5 ) intRes-=1.0;
  return intRes*pow(0.1,digits);
}
//------------------------------------------------------------------
bool inline CheckForFence(int x,int d1,int y, int d2)
{ if ( x/gridSizeNm != (x+d1)/gridSizeNm ) return true;
  if ( y/gridSizeNm != (y+d2)/gridSizeNm ) return true;
  return false;
}
//------------------------------------------------------------------
double inline RandOne(void)
   { double val=random(2000000)-1000000; return val*1e-6; }
//------------------------------------------------------------------
double Gauss2D(double x, double y,double sigma)
{ if ( sigma == 0.0 ) return 0.0;
  double res=exp(-( ((x*x)+(y*y))/(2.0*sigma*sigma) ) );
  return res;//*(1.0/(2.0*PI*sigma*sigma));
}
//------------------------------------------------------------------
double Gauss(double x, double sigma)
{ if ( sigma == 0.0 ) return 0.0;
  double res=exp(-((x*x)/(2.0*sigma*sigma)));
  return res;//*(1.0/(sqrt(2.0*PI*sigma*sigma)));
}
//-----------------------------------
double Prob1D(double coor, double Kdiff,double time)
{ double DT4=4.0*Kdiff*time;
  return exp(-(coor*coor)/DT4)/sqrt(DT4*PI);
}
//-----------------------------------
double Prob2D(double x, double D,double t)
{ double DT4=4.0*D*t;
  if ( DT4 == 0.0 ) return 0.0;
  return exp(-(x*x)/(DT4))/(DT4*PI);
}
//-----------------------------------
double Prob3D(double x, double D,double t)
{ double DT4=4.0*D*t;
  if ( DT4 == 0.0 ) return 0.0;
  double val=DT4*PI;
  return exp(-(x*x)/(DT4))/sqrt(val*val*val);
}
//-----------------------------------
double AIT2D(double x, double D,double t,double IA)
{ double time=0.0,timeStep=0.0001;
  for ( double curTime=1e-9; curTime < t; curTime+=timeStep )
	time+=Prob2D(x,D,curTime)*IA*timeStep;
  return time;
}
//-----------------------------------
double AIT3D(double x, double D,double t,double IV)
{ double time=0.0,timeStep=0.0001;
  for ( double curTime=1e-9; curTime < t; curTime+=timeStep )
	time+=Prob3D(x,D,curTime)*IV*timeStep;
  return time;
}

//---------------------------------------------------------------------------
int BitsToInt(char *bits,int size)
{ int val=0,i;
  for ( i=0; i < size; i++ ) if ( *(bits+i) == '1' ) val|=(1<<i);
  return val;
}
//-------------------------------------------------------
void IntToBits(int val,char *bits,int size)
{ for ( int i=0; i < size; i++ )
	if ( val & (1<<i) )  *(bits+i) = '1';
	else                 *(bits+i) = '0';
}
//-------------------------------------------------------
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

/*/---------------------------------------------------------------------------
bool TMainForm::CheckCell(__int64 x,__int64 y, __int64 z)
{ __int64 index=x+(y*volX)+(z*volXY);
  if ( index >= volSize ) { ShowMessage("Out of Volume"); return false; }
  if ( matrix[index] == 240 && vertNum < VERT_LIM )
	{ vert[vertNum].x=x; vert[vertNum].y=y;vert[vertNum].z=z;
	  for ( int j=0; j < 12; j++ ) vert[vertNum].nInd[j]=-1;
	  vertNum++;    matrix[index]=220;
	}
  return true;
}
//---------------------------------------------------------------------------
void TMainForm::SetVertices(void)
{ int maxX[10000],maxY[10000],dir[10000];
  int incZ=-1,dirZ=1,maxZ=1;
  __int64 x[10000],y[100000],z=0;
  for ( int i=0; i < 10000; i++ ) { x[i]=1;y[i]=0; maxX[i]=1;maxY[i]=1; dir[i]=-1; }
  vertNum=0;
  for ( __int64 i=0; i < volSize; i++ )
	{ x[z]+=dir[z];y[z]-=dir[z];
	  if ( !CheckCell(x[z],y[z],z) ) break;
	  if ( x[z] == maxX[z] )
		{ maxX[z]++; incZ++;
		  if ( maxX[z] >= volX-1 ) { maxX[z]--; y[z]++; }
		  x[z]=maxX[z]; maxY[z]++; dir[z]=-1;
		  if ( !CheckCell(x[z],y[z],z) ) break;
		}
	  if ( y[z] == maxY[z] )
		{ maxY[z]++; incZ++;
		  if ( maxY[z] >= volY-1 ) { maxY[z]--; x[z]++; }
		  y[z]=maxY[z]; maxX[z]++; dir[z]=1;
		  if ( !CheckCell(x[z],y[z],z) ) break;
		}
	}
}
*/


