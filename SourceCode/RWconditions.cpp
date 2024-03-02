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

// This is an extension of main.cpp to deal with text settings Read / Write

#pragma hdrstop
#include "Settings.h"
#include "main.h"
#include "RWconditions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//--------------------------------------------------------------------
void TMainForm::ReadSettings(TObject *Sender)
{ FILE *infile;
  if ( !OpenPrmDlg->Execute() ) return;
  infile=fopen(AnsiString(OpenPrmDlg->FileName).c_str(),"rt");
  if ( !infile ) { fclose(infile); ShowMessage("Can NOT open file"); return; }
  AnsiString str=""; char txt[1005],buf; int ind=0,pos=0,step=0;
  while ( fread(&buf,1,1,infile) )
	{ if ( ind > 1000 ) { ShowMessage("File Read ERROR, abort"); break; }
	  if ( buf != ';' && buf != '\n' ) { txt[ind]=buf; txt[ind+1]='\0'; ind++; }
	  else { str=txt; ;ind=0; StrToParam(str); }
	}
  fclose(infile); VarsToPrms(); FormResize(Sender);
}
//---------------------------------------------------------------------------
void TMainForm::StrToParam(AnsiString str)
{ int strLength=str.Length(),firstChar=0,comLength=0,index=0;
  AnsiString param,val;  bool state=false;
  while ( str.c_str()[index] != ',' && index < strLength ) index++;
  comLength=index;
  param=str.SubString(0,comLength); index++;
  while ( index < strLength && str.c_str()[index] != ',' && str.c_str()[index] != ';' )
	index++;
  val=str.SubString(comLength+2,index-comLength-1);
 // cell property
  if ( val == "ON" ) state=true; // set boolian value;
  if ( param == "voxelSizeNm" ) { nmVox=val.ToIntDef(10); return; }
  if ( param == "memorySizeMb" ) { memorySizeMb=val.ToIntDef(10); return; }
  if ( param == "cellSizeXnm" ) { cellXnm=val.ToIntDef(1000); return; }
  if ( param == "cellSizeYnm" ) { cellYnm=val.ToIntDef(1000); return; }
  if ( param == "cellSizeZnm" ) { cellZnm=val.ToIntDef(1000); return; }
  if ( param == "sideGapNm" ) { sideGapNm=val.ToIntDef(1000); return; }
  if ( param == "topGapNm" ) { topGapNm=val.ToIntDef(1000); return; }
  if ( param == "botGapNm" ) { botGapNm=val.ToIntDef(1000); return; }
  if ( param == "cellFilletNm" ) { bodyFilletNm=val.ToIntDef(1000); return; }
  if ( param == "roundedCellBody" ) { roundedCellBody=state; return; }
  if ( param == "nucleusRadNm" ) { nucleusRadNm=val.ToIntDef(100); return; }
  if ( param == "flattening" ) { flattening=StrToFloatDef(val,1.0); return; }
  if ( param == "nucleus" )      { nucleus=state; return; }
  if ( param == "rafts" )        { rafts=state; return; }
  if ( param == "raftRadNm" )    { raftRadNm=val.ToIntDef(100); return; }
  if ( param == "raftMobilityRatio" )  { raftMobRatio=StrToFloatDef(val,1.0); return; }
  if ( param == "filopodia" )    { filopodia=state; return; }
  if ( param == "lamella" )    { lamella=state; return; }
  if ( param == "tubes" )    { tubes=state; return; }
  if ( param == "tubeNetwork" )   { tubeNetwork=state; return; }
  if ( param == "tube3D" )   { tube3D=state; return; }
  if ( param == "tubeRadNm" ) { tubeRadNm=val.ToIntDef(100); return; }
  if ( param == "tubeLengthNm" ) { tubeLengthNm=val.ToIntDef(100); return; }
  if ( param == "interTubeDistNm" ) { interTubeDistNm=val.ToIntDef(100); return; }
  if ( param == "tubeGapNm" ) { tubeGapNm=val.ToIntDef(100); return; }
  if ( param == "bendingCoef" ) { bendTilt=StrToFloatDef(val,1.0); return; }
  if ( param == "movingVesicles" )   { movingVes=state; return; }
  if ( param == "vesRecycling" )   { recycling=state; return; }
  if ( param == "outsideVesicles" )  { outsideVes=state; return; }
  if ( param == "fuseVesicles" )     { fuseVesicles=state; return; }
  if ( param == "instantRelease" )   { transfer=state; return; }
  if ( param == "vesicleNumber" ) { vesNum=val.ToIntDef(100); return; }
  if ( param == "poreRadNm" ) { poreRadNm=val.ToIntDef(100); return; }
  if ( param == "vesicleMob" ) { vesicleMob=StrToFloatDef(val,1.0); return; }
  if ( param == "interactionDistanceUm" ) { IDum=StrToFloatDef(val,1.0); return; }
  if ( param == "modelTimeStepSec" )  { tmStep=StrToFloatDef(val,1.0); return; }

  //--- Illumination
  if ( param == "cameraNmPix" ) { nmPix=val.ToIntDef(100); return; }
  if ( param == "FWHMnm" ) { FWHMnm=val.ToIntDef(100); return; }
  if ( param == "objectiveLensZcoef" ) { Kobj=StrToFloatDef(val,1.0); return; }
  if ( param == "laserFWHMum")      { illumFWHMum=StrToFloatDef(val,1.0); return; }
  if ( param == "diaphragmRadUm")      { irisRadUm=StrToFloatDef(val,1.0); return; }
  if ( param == "greenPhotonsPerSec" ){ greenPhotonsPerSec=val.ToIntDef(100); return; }
  if ( param == "greenBleachRate" ){ greenBleachRate=StrToFloatDef(val,1.0); return; }
  if ( param == "redPhotonsPerSec" ) { redPhotonsPerSec=val.ToIntDef(100); return; }
  if ( param == "redBleachRate" )  { redBleachRate=StrToFloatDef(val,1.0); return; }
  if ( param == "FRET" ) { FRET=state; return; }
  if ( param == "alternatingIllumination" ) { alterIllum=state; return; }
  if ( param == "bleachSizeNm"  ) { bleachSizeNm=val.ToIntDef(100); return; }

   //----- Camera
  if ( param == "cameraNmPix" ) { nmPix=val.ToIntDef(100); return; }
  if ( param == "focusNm" ) { focusNm=val.ToIntDef(100); return; }
  if ( param == "twoCameraMode" )   { twoCam=state; return; }
  if ( param == "EMCCDmode" )   { EMCCDmode=state; return; }
  if ( param == "cameraGain" ) { gain=val.ToIntDef(10); return; }
  if ( param == "timeStepsPerImage" ) { framesToSum=val.ToIntDef(10); return; }
  if ( param == "darkFramesNumber" )  {	numOfDarkFrames=val.ToIntDef(10); return; }
  if ( param == "cameraNoiseCountsPix" ) { noiseAmp=StrToFloatDef(val,1.0); return; }
  if ( param == "framesToSave" ) { frameLimit=val.ToIntDef(100); return; }

  // CytoA properties
  if ( param == "cytoAdistribution" )
	{ if ( val == "CYTOPLASM" ) { cytoAinit=0; return; }
	  if ( val == "NUCLEOPLASM" ) { cytoAinit=1; return; }
	  if ( val == "TUBES" ) { cytoAinit=2; return; }
	  if ( val == "VESICLES" ) { cytoAinit=3; return; }
	  if ( val == "OUTSIDE" ) { cytoAinit=4; return; }
	}
  if ( param == "cytoAnumber" ) { cytoAnum=val.ToIntDef(100); return; }
  if ( param == "greenFluorsPerCytoA" ) { greenCytoAnum=val.ToIntDef(100); return; }
  if ( param == "greenFractionCytoA%" )	{ greenCytoAfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "redFluorsPerCytoA" ) { redCytoAnum=val.ToIntDef(100); return; }
  if ( param == "redFractionCytoA%" ) { redCytoAfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAdiffusionRate" )	{ cytoAmob=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAstepSizeNm" )	{ cytoAstepSizeNm=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAstepRate" )	{ cytoAstepRate=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoApolimer" )   { cytoApoli=state; return; }
  if ( param == "cytoApolimerRate" )	{ cytoApoliRate=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAdepolimerRate" )	{ cytoAdepoliRate=StrToFloatDef(val,1.0); return; }

  // CytoB properties
  if ( param == "cytoBdistribution" )
	{ if ( val == "CYTOPLASM" ) { cytoBinit=0; return; }
	  if ( val == "OUTSIDE" ) { cytoBinit=1; return; }
	  if ( val == "CELL_CENTER" ) { cytoBinit=2; return; }
	}
  if ( param == "cytoBnumber" ) { cytoBnum=val.ToIntDef(100); return; }
  if ( param == "greenFluorsPerCytoB" ) { greenCytoBnum=val.ToIntDef(100); return; }
  if ( param == "greenFractionCytoB%" )	{ greenCytoBfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "redFluorsPerCytoB" ) { redCytoBnum=val.ToIntDef(100); return; }
  if ( param == "redFractionCytoB%" ) { redCytoBfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoBdiffusionRate" )	{ cytoBmob=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAbindingCytoB" )   { cytoAcanBindCytoB=state; return; }
  if ( param == "cytoAcytoBbindRate" )	{ cytoAcytoBbindRate=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAcytoBdissRate" )	{ cytoAcytoBdissRate=StrToFloatDef(val,1.0); return; }


  // MembA properties
  if ( param == "membAdistribution" )
	{ if ( val == "ONE_POINT_BOTTOM" ) { membAinit=0; return; }
	  if ( val == "CELL_MEMBRANE" ) { membAinit=1; return; }
	  if ( val == "NUCLEUS_MEMBRANE" ) { membAinit=2; return; }
	  if ( val == "TUBE_MEMBRANE" ) { membAinit=3; return; }
	  if ( val == "VESICLE_MEMBRANE" ) { membAinit=4; return; }
	  if ( val == "CLUSTERS_CELL_MEMBRANE" ) { membAinit=5; return; }
	}
  if ( param == "membAnumber" ) { membAnum=val.ToIntDef(100); return; }
  if ( param == "greenFluorsPerMembA" ) { greenMembAnum=val.ToIntDef(100); return; }
  if ( param == "greenFractionMembA%" )	{ greenMembAfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "redFluorsPerMembA" ) { redMembAnum=val.ToIntDef(100); return; }
  if ( param == "redFractionMembA%" ) { redMembAfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "membAdiffusionRate" )	{ membAmob=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAbindingMembA" )   { cytoAcanBindMembA=state; return; }
  if ( param == "cytoAmembAbindRate" )	{ cytoAmembAbindRate=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAmembAdissRate" )	{ cytoAmembAdissRate=StrToFloatDef(val,1.0); return; }
  if ( param == "membAorganellePump" )   { membAorganPump=state; return; }
  if ( param == "membApolimer" )   { membApoli=state; return; }
  if ( param == "membApolimerRate" )	{ membApoliRate=StrToFloatDef(val,1.0); return; }
  if ( param == "membAdepolimerRate" )	{ membAdepoliRate=StrToFloatDef(val,1.0); return; }
  if ( param == "membAmaxUnitPolimer" ) { membApoliNum=val.ToIntDef(100); return; }

  // MembB properties
  if ( param == "membBdistribution" )
	{ if ( val == "ONE_POINT_BOTTOM" ) { membBinit=0; return; }
	  if ( val == "CELL_MEMBRANE" ) { membBinit=1; return; }
	  if ( val == "NUCLEUS_MEMBRANE" ) { membBinit=2; return; }
	  if ( val == "TUBE_MEMBRANE" ) { membBinit=3; return; }
	  if ( val == "VESICLE_MEMBRANE" ) { membBinit=4; return; }
	}
  if ( param == "membBnumber" ) { membBnum=val.ToIntDef(100); return; }
  if ( param == "greenFluorsPerMembB" ) { greenMembBnum=val.ToIntDef(100); return; }
  if ( param == "greenFractionMembB%" )	{ greenMembBfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "redFluorsPerMembB" ) { redMembBnum=val.ToIntDef(100); return; }
  if ( param == "redFractionMembB%" ) { redMembBfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "membBdiffusionRate" )	{ membBmob=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAbindingMembB" )   { cytoAcanBindMembB=state; return; }
  if ( param == "cytoAmembBbindRate" )	{ cytoAmembBbindRate=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAmembBdissRate" )	{ cytoAmembBdissRate=StrToFloatDef(val,1.0); return; }
  if ( param == "membAcytoplasmPump" )   { membBcytoPump=state; return; }
  if ( param == "membAbindingMembB" )   { membAcanBindMembB=state; return; }
  if ( param == "membAmembBbindRate" )	{ membAmembBbindRate=StrToFloatDef(val,1.0); return; }
  if ( param == "membAmembBdissRate" )	{ membAmembBdissRate=StrToFloatDef(val,1.0); return; }

  // MembC properties
  if ( param == "membCdistribution" )
	{ if ( val == "ONE_POINT_BOTTOM" ) { membCinit=0; return; }
	  if ( val == "CELL_MEMBRANE" ) { membCinit=1; return; }
	  if ( val == "NUCLEUS_MEMBRANE" ) { membCinit=2; return; }
	  if ( val == "TUBE_MEMBRANE" ) { membCinit=3; return; }
	  if ( val == "VESICLE_MEMBRANE" ) { membCinit=4; return; }
	}
  if ( param == "membCnumber" ) { membCnum=val.ToIntDef(100); return; }
  if ( param == "greenFluorsPerMembC" ) { greenMembCnum=val.ToIntDef(100); return; }
  if ( param == "greenFractionMembC%" )	{ greenMembCfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "redFluorsPerMembC" ) { redMembCnum=val.ToIntDef(100); return; }
  if ( param == "redFractionMembC%" ) { redMembCfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "membCdiffusionRate" )	{ membCmob=StrToFloatDef(val,1.0); return; }
  if ( param == "membBbindingMembB" )   { membBcanBindMembC=state; return; }
  if ( param == "membBmembCbindRate" )	{ membBmembCbindRate=StrToFloatDef(val,1.0); return; }
  if ( param == "membBmembCdissRate" )	{ membBmembCdissRate=StrToFloatDef(val,1.0); return; }

  // StatA properties
  if ( param == "statAdistribution" )
	{ if ( val == "STRAIT_FIBERS_XY" ) { statAinit=0; return; }
	  if ( val == "STRAIT_FIBERS_3D" ) { statAinit=1; return; }
	  if ( val == "CURVED_FIBERS_XY" ) { statAinit=2; return; }
	  if ( val == "CURVED_FIBERS_3D" ) { statAinit=3; return; }
	  if ( val == "BUNDLE_FILOPODIA" ) { statAinit=4; return; }
	}
  if ( param == "statAnumber" ) { statAnum=val.ToIntDef(100); return; }
  if ( param == "greenFluorsPerStatA" ) { greenStatAnum=val.ToIntDef(100); return; }
  if ( param == "greenFractionStatA%" )	{ greenStatAfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "redFluorsPerStatA" ) { redStatAnum=val.ToIntDef(100); return; }
  if ( param == "redFractionStatA%" )	{ redStatAfrac=StrToFloatDef(val,1.0); return; }
  if ( param == "statAmonomerDistanceNm" )	{ statApitchNm=StrToFloatDef(val,1.0); return; }
  if ( param == "statAmonoPerFilo" ) { monoPerFilo=val.ToIntDef(100); return; }
  if ( param == "statAbundleExtensionNm" ) { actinBundleExtendNm=val.ToIntDef(100); return; }
  if ( param == "statAbindingCytoA" )   { cytoAcanBindStatA=state; return; }
  if ( param == "cytoAstatAbindRate" )	{ cytoAstatAbindRate=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAstatAdissRate" )	{ cytoAstatAdissRate=StrToFloatDef(val,1.0); return; }
  if ( param == "cytoAmoveAlongStatA" )   { cytoAcanMoveAlongStatA=state; return; }


}
//--------------------------------------------------------------------
void TMainForm::WriteSettings(void)
{ FILE *outfile;
  if ( !SavePrmDlg->Execute() ) return;
  outfile=fopen(AnsiString(SavePrmDlg->FileName).c_str(),"wt");
  if ( outfile == NULL ) { ShowMessage("File Writing Error"); return; }
  AnsiString str="   Cell/Matrix Properties \n"; char txt[5000];
  str+="voxelSizeNm,"; str+=nmVox; str+=";\n";
  str+="memorySizeMb,"; str+=memorySizeMb; str+=";\n";
  str+="modelTimeStepSec,"; str+=tmStep; str+=";\n";
  str+="cellSizeXnm,"; str+=cellXnm; str+=";\n";
  str+="cellSizeYnm,"; str+=cellYnm; str+=";\n";
  str+="cellSizeZnm,"; str+=cellZnm; str+=";\n";
  str+="sideGapNm,"; str+=sideGapNm; str+=";\n";
  str+="topGapNm,"; str+=topGapNm; str+=";\n";
  str+="botGapNm,"; str+=botGapNm; str+=";\n";
  str+="roundedCellBody,";
  if ( roundedCellBody ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="cellFilletNm,"; str+=bodyFilletNm; str+=";\n";
  str+="nucleusRadNm,"; str+=nucleusRadNm; str+=";\n";
  str+="flattening,"; str+=flattening; str+=";\n";
  str+="nucleus,"; if ( nucleus ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="rafts,";   if ( rafts ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="raftRadNm,"; str+=raftRadNm; str+=";\n";
  str+="raftMobilityRatio,"; str+=raftMobRatio; str+=";\n";
  str+="interactionDistanceUm,"; str+=IDum; str+=";\n";
  str+="filopodia,"; if ( filopodia ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="lamella,"; if ( lamella ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="tubes,"; if ( tubes ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="tubeNetwork,"; if ( tubeNetwork ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="tubeRadNm,"; str+=tubeRadNm; str+=";\n";
  str+="tubeLengthNm,"; str+=tubeLengthNm; str+=";\n";
  str+="interTubeDistNm,";   str+=interTubeDistNm; str+=";\n";
  str+="tubeGapNm,";   str+=tubeGapNm; str+=";\n";
  str+="bendingCoef,";   str+=bendTilt; str+=";\n";
  str+="tube3D,"; if ( tube3D ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="vesicleRadNm,";   str+=vesRadNm; str+=";\n";
  str+="vesicleNumber,";   str+=vesNum; str+=";\n";
  str+="movingVesicles,";
  if ( movingVes ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="vesRecycling,";
  if ( recycling ) str+="ON"; else str+="OFF"; str+=";\n";
   str+="outsideVesicles,";
  if ( outsideVes ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="vesicleMob,";   str+=vesicleMob; str+=";\n";
  str+="fuseVesicles,";
  if ( fuseVesicles ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="instantRelease,";
  if ( transfer ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="poreRadNm,";   str+=poreRadNm; str+=";\n";

  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);


  str="\n  Illumination Settings;\n";
  str+="greenPhotonsPerSec,"; str+=greenPhotonsPerSec; str+=";\n";
  str+="greenBleachRate,"; str+=Round(greenBleachRate,5); str+=";\n";
  str+="redPhotonsPerSec,"; str+=redPhotonsPerSec; str+=";\n";
  str+="redBleachRate,";  str+=Round(redBleachRate,5); str+=";\n";
  str+="FRET,"; if ( FRET ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="FWHMnm,"; str+=FWHMnm; str+=";\n";
  str+="alternatingIllumination,";
  if ( alterIllum ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="laserFWHMum,";  str+=Round(illumFWHMum,3); str+=";\n";
  str+="diaphragmRadUm,";  str+=Round(irisRadUm,3); str+=";\n";
  str+="bleachSizeNm,"; str+=bleachSizeNm; str+=";\n";

  str+="\n  Camera Settings;\n";
  str+="objectiveLensZcoef,"; str+=Kobj; str+=";\n";
  str+="cameraNmPix,"; str+=nmPix; str+=";\n";
  str+="focusNm,"; str+=focusNm; str+=";\n";
  str+="twoCameraMode,";
  if ( twoCam ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="EMCCDmode,";
  if ( EMCCDmode ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="cameraGain,";  str+=gain; str+=";\n";
  str+="timeStepsPerImage,"; str+=framesToSum; str+=";\n";
  str+="darkFramesNumber,";  str+=numOfDarkFrames; str+=";\n";
  str+="cameraNoiseCountsPix,"; str+=noiseAmp;  str+=";\n";
  str+="FramesToSave,"; str+=frameLimit;  str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);

  str="\n  CytoA Object Settings;\n";
  str+="cytoAnumber,"; str+=cytoAnum;  str+=";\n";
  str+="cytoAdistribution,";
  if ( cytoAinit == 0 ) str+="CYTOPLASM;\n";
  if ( cytoAinit == 1 ) str+="NUCLEOPLASM;\n";
  if ( cytoAinit == 2 ) str+="TUBES;\n";
  if ( cytoAinit == 3 ) str+="VESICLES;\n";
  if ( cytoAinit == 4 ) str+="OUTSIDE;\n";
  str+="greenFluorsPerCytoA,"; str+=greenCytoAnum;  str+=";\n";
  str+="greenFractionCytoA%,"; str+=greenCytoAfrac;  str+=";\n";
  str+="redFluorsPerCytoA,"; str+=redCytoAnum;  str+=";\n";
  str+="redFractionCytoA%,"; str+=redCytoAfrac;  str+=";\n";
  str+="cytoAdiffusionRate,"; str+=cytoAmob;  str+=";\n";
  str+="cytoAstepSizeNm,"; str+=cytoAstepSizeNm;  str+=";\n";
  str+="cytoAstepRate,"; str+=cytoAstepRate;  str+=";\n";
  str+="cytoApolimer,";
  if ( cytoApoli ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="cytoApolimerRate,"; str+=cytoApoliRate;  str+=";\n";
  str+="cytoAdepolimerRate,"; str+=cytoAdepoliRate;  str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);

  str="\n  CytoB Object Settings;\n";
  str+="cytoBnumber,"; str+=cytoBnum;  str+=";\n";
  str+="cytoBdistribution,";
  if ( cytoBinit == 0 ) str+="CYTOPLASM;\n";
  if ( cytoBinit == 1 ) str+="OUTSIDE;\n";
  if ( cytoBinit == 2 ) str+="CELL_CENTER;\n";
  str+="greenFluorsPerCytoB,"; str+=greenCytoBnum;  str+=";\n";
  str+="greenFractionCytoB%,"; str+=greenCytoBfrac;  str+=";\n";
  str+="redFluorsPerCytoB,"; str+=redCytoBnum;  str+=";\n";
  str+="redFractionCytoB%,"; str+=redCytoBfrac;  str+=";\n";
  str+="cytoBdiffusionRate,"; str+=cytoBmob;  str+=";\n";
  str+="cytoAbindingCytoB,";
  if ( cytoAcanBindCytoB ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="cytoAcytoBbindRate,"; str+=cytoAcytoBbindRate;  str+=";\n";
  str+="cytoAcytoBdissRate,"; str+=cytoAcytoBdissRate;  str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);


  str="\n  MembA Object Settings;\n";
  str+="membAnumber,"; str+=membAnum;  str+=";\n";
  str+="membBdistribution,";
  if ( membAinit == 0 ) str+="ONE_POINT_BOTTOM;\n";
  if ( membAinit == 1 ) str+="CELL_MEMBRANE;\n";
  if ( membAinit == 2 ) str+="NUCLEUS_MEMBRANE;\n";
  if ( membAinit == 3 ) str+="TUBE_MEMBRANE;\n";
  if ( membAinit == 4 ) str+="VESICLE_MEMBRANE;\n";
  if ( membAinit == 5 ) str+="CLUSTERS_CELL_MEMBRANE;\n";
  str+="greenFluorsPerMembA,"; str+=greenMembBnum;  str+=";\n";
  str+="greenFractionMembA%,"; str+=greenMembBfrac;  str+=";\n";
  str+="redFluorsPerMembA,"; str+=redMembBnum;  str+=";\n";
  str+="redFractionMembA%,"; str+=redMembBfrac;  str+=";\n";
  str+="membAdiffusionRate,"; str+=membBmob;  str+=";\n";
  str+="cytoAbindingMembA,";
  if ( cytoAcanBindMembA ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="cytoAmembAbindRate,"; str+=cytoAmembAbindRate;  str+=";\n";
  str+="cytoAmembAdissRate,"; str+=cytoAmembAdissRate;  str+=";\n";
  str+="membAorganellePump,";
  if ( membAorganPump ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="membApolimer,";
  if ( membApoli ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="membApolimerRate,"; str+=membApoliRate;  str+=";\n";
  str+="membAdepolimerRate,"; str+=membAdepoliRate;  str+=";\n";
  str+="membAmaxUnitPolimer,"; str+=membApoliNum;  str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);



  str="\n  MembB Object Settings;\n";
  str+="membBnumber,"; str+=membBnum;  str+=";\n";
  str+="membBdistribution,";
  if ( membBinit == 0 ) str+="ONE_POINT_BOTTOM;\n";
  if ( membBinit == 1 ) str+="CELL_MEMBRANE;\n";
  if ( membBinit == 2 ) str+="NUCLEUS_MEMBRANE;\n";
  if ( membBinit == 3 ) str+="TUBE_MEMBRANE;\n";
  if ( membBinit == 4 ) str+="VESICLE_MEMBRANE;\n";
  str+="greenFluorsPerMembB,"; str+=greenMembBnum;  str+=";\n";
  str+="greenFractionMembB%,"; str+=greenMembBfrac;  str+=";\n";
  str+="redFluorsPerMembB,"; str+=redMembBnum;  str+=";\n";
  str+="redFractionMembB%,"; str+=redMembBfrac;  str+=";\n";
  str+="membBdiffusionRate,"; str+=membBmob;  str+=";\n";
  str+="cytoAbindingMembB,";
  if ( cytoAcanBindMembB ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="cytoAmembBbindRate,"; str+=cytoAmembBbindRate;  str+=";\n";
  str+="cytoAmembBdissRate,"; str+=cytoAmembBdissRate;  str+=";\n";
  str+="membBcytoplasmPump,";
  if ( membBcytoPump ) str+="ON"; else str+="OFF"; str+=";\n";

  str+="membAbindingMembB,";
  if ( membAcanBindMembB ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="membAmembBbindRate,"; str+=membAmembBbindRate;  str+=";\n";
  str+="membAmembBdissRate,"; str+=membAmembBdissRate;  str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);


  str="\n  MembC Object Settings;\n";
  str+="membCnumber,"; str+=membCnum;  str+=";\n";
  str+="membCdistribution,";
  if ( membCinit == 0 ) str+="ONE_POINT_BOTTOM;\n";
  if ( membCinit == 1 ) str+="CELL_MEMBRANE;\n";
  if ( membCinit == 2 ) str+="NUCLEUS_MEMBRANE;\n";
  if ( membCinit == 3 ) str+="TUBE_MEMBRANE;\n";
  if ( membBinit == 4 ) str+="VESICLE_MEMBRANE;\n";
  str+="greenFluorsPerMembC,"; str+=greenMembCnum;  str+=";\n";
  str+="greenFractionMembC%,"; str+=greenMembCfrac;  str+=";\n";
  str+="redFluorsPerMembC,"; str+=redMembCnum;  str+=";\n";
  str+="redFractionMembC%,"; str+=redMembCfrac;  str+=";\n";
  str+="membCdiffusionRate,"; str+=membCmob;  str+=";\n";
  str+="membBbindingMembC,";
  if ( membBcanBindMembC ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="membBmembCbindRate,"; str+=membBmembCbindRate;  str+=";\n";
  str+="membBmembCdissRate,"; str+=membBmembCdissRate;  str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);



  str="\n  StatA Object Settings;\n";
  str+="statAnumber,"; str+=statAnum;  str+=";\n";
  str+="statAdistribution,";
  if ( statAinit == 0 ) str+="STRAIT_FIBERS_XY;\n";
  if ( statAinit == 1 ) str+="STRAIT_FIBERS_3D;\n";
  if ( statAinit == 2 ) str+="CURVED_FIBERS_XY;\n";
  if ( statAinit == 3 ) str+="CURVED_FIBERS_3D;\n";
  if ( statAinit == 4 ) str+="BUNDLE_FILOPODIA;\n";
  str+="greenFluorsPerStatA,"; str+=greenStatAnum;  str+=";\n";
  str+="greenFractionStatA%,"; str+=greenStatAfrac;  str+=";\n";
  str+="redFluorsPerStatA,"; str+=redStatAnum;  str+=";\n";
  str+="redFractionStatA%,"; str+=redStatAfrac;  str+=";\n";
  str+="statAmonomerDistanceNm,"; str+=statApitchNm;  str+=";\n";
  str+="statAmonoPerFilo,"; str+=monoPerFilo;  str+=";\n";
  str+="statAbundleExtensionNm,"; str+=actinBundleExtendNm;  str+=";\n";
  str+="statAbindingCytoA,";
  if ( cytoAcanBindStatA ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="cytoAstatAbindRate,"; str+=cytoAstatAbindRate;  str+=";\n";
  str+="cytoAstatAdissRate,"; str+=cytoAstatAdissRate;  str+=";\n";
  str+="cytoAmoveAlongStatA,";
  if ( cytoAcanMoveAlongStatA ) str+="ON"; else str+="OFF"; str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);







  bool cytoAcanBindMembA,cytoAcanBindMembB,cytoAcanBindStatA,cytoAinsideTube;
 bool cytoApoli;

 int cytoApoliNum;
 double cytoAmob,cytoAmembAbindRate,cytoAmembAdissRate,cytoAmembBbindRate,cytoAmembBdissRate;
 double cytoAstatAbindRate,cytoAstatAdissRate,cytoAstepSizeNm,cytoAstepRate;
 double cytoAcytoBbindRate,cytoAcytoBdissRate,jumpCytoA,cytoApoliRate,cytoAdepoliRate;




  fclose(outfile);
}
