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
  if ( param == "cellSizeXnm" ) { cellXnm=val.ToIntDef(1000); return; }
  if ( param == "cellSizeYnm" ) { cellYnm=val.ToIntDef(1000); return; }
  if ( param == "cellSizeZnm" ) { cellZnm=val.ToIntDef(1000); return; }
  if ( param == "sideGapNm" ) { sideGapNm=val.ToIntDef(1000); return; }
  if ( param == "topGapNm" ) { topGapNm=val.ToIntDef(1000); return; }
  if ( param == "botGapNm" ) { botGapNm=val.ToIntDef(1000); return; }
  if ( param == "cellFilletNm" ) { bodyFilletNm=val.ToIntDef(1000); return; }

  if ( param == "roundedCellBody" ) { roundedCellBody=state; return; }
  if ( param == "tubeNetwork" )   { tubeNetwork=state; return; }
  if ( param == "nucleus" )      { nucleus=state; return; }
  if ( param == "rafts" )          { rafts=state; return; }





	//--- Illumination

  if ( param == "greenBleachRate" ){ greenBleachRate=StrToFloatDef(val,1.0); return; }
  if ( param == "redBleachRate" )  { redBleachRate=StrToFloatDef(val,1.0); return; }


}
//--------------------------------------------------------------------
void TMainForm::WriteSettings(void)
{ FILE *outfile;
  if ( !SavePrmDlg->Execute() ) return;
  outfile=fopen(AnsiString(SavePrmDlg->FileName).c_str(),"wt");
  if ( outfile == NULL ) { ShowMessage("File Writing Error"); return; }
  AnsiString str=""; char txt[5000];
  str="cellSizeXnm,"; str+=cellXnm; str+=";\n";
  str+="cellSizeYnm,"; str+=cellYnm; str+=";\n";
  str+="cellSizeZnm,"; str+=cellZnm; str+=";\n";
  str+="sideGapNm,"; str+=sideGapNm; str+=";\n";
  str+="topGapNm,"; str+=topGapNm; str+=";\n";
  str+="botGapNm,"; str+=botGapNm; str+=";\n";
  str+="nucleus,"; if ( nucleus ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="roundedCellBody,";
  if ( roundedCellBody ) str+="ON"; else str+="OFF"; str+=";\n";
  str+="rafts,"; if ( rafts ) str+="ON"; else str+="OFF"; str+=";\n";



  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);

  str="greenBleachRate,"; str+=Round(greenBleachRate,3); str+=";\n";
  str+="redBleachRate,";  str+=Round(redBleachRate,3); str+=";\n";
  StrCopy(txt,str.c_str()); str=""; fwrite(&txt,strlen(txt),1,outfile);




  fclose(outfile);
}
