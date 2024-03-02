//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
 #include <math.h>
 #include "main.h"
#include <ActnCtrls.hpp>
#include <ActnMan.hpp>
#include <ActnMenus.hpp>
#include <ToolWin.hpp>


//---------------------------------------------------------------------------
class TSForm : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl;
        TTabSheet *DimsPage;
        TTabSheet *ProteinPage;
        TButton *OK_Button;
        TTabSheet *SiteApage;
        TTabSheet *SiteBpage;
        TTabSheet *MembC_Page;
        TLabeledEdit *LengthEdit;
        TImage *CellImage;
        TLabeledEdit *WidthEdit;
        TLabeledEdit *HeightEdit;
        TCheckBox *NucleusChBox;
        TLabel *ConcentrationLab;
        TLabel *SurfaceLab;
        TLabel *VolumeLab;
        TLabel *SiteAlab;
        TLabel *SiteBlab;
        TLabeledEdit *CytoAnumEdit;
        TLabel *CytoAconsLab;
        TLabeledEdit *GreenCytoAnumEdit;
        TLabeledEdit *CytoAmobEdit;
        TButton *CancelButton;
        TLabel *CytoAjumpLab;
        TLabeledEdit *MembAnumEdit;
        TLabeledEdit *GreenMembAnumEdit;
        TLabel *SiteAdensLab;
	TLabeledEdit *MembAmobEdit;
        TLabel *SiteAjumpLab;
        TCheckBox *CytoAcanBindMembA_ChBox;
        TLabeledEdit *CytoAmembAdissRateEdit;
	TCheckBox *MembApolimerChBox;
        TLabeledEdit *AAdissRateEdit;
        TComboBox *MembAinitBox;
        TLabel *Label1;
        TLabeledEdit *MembBnumEdit;
        TLabel *SiteBdensLab;
        TLabeledEdit *GreenMembBnumEdit;
	TLabeledEdit *MembBmobEdit;
        TLabel *SiteBjumpLab;
        TCheckBox *CytoAcanBindMembB_ChBox;
        TLabeledEdit *CytoAmembBdissRateEdit;
        TCheckBox *MembAcanBindMembB_ChBox;
        TLabeledEdit *AfromBdissRateEdit;
        TLabeledEdit *GreenCytoAfractionEdit;
        TLabeledEdit *GreenMembAfractionEdit;
        TLabeledEdit *GreenMembBfractionEdit;
        TCheckBox *RaftsChBox;
        TLabeledEdit *RaftSizeEdit;
        TComboBox *MembBinitBox;
        TLabel *InitLab;
        TLabeledEdit *ReleaseRateEdit;
        TImage *Image1;
        TLabeledEdit *MonomersInFilamentEdit;
        TLabeledEdit *AAbindingRateEdit;
        TLabel *SiteAAtimeLab;
        TLabeledEdit *RaftMobRatioEdit;
        TLabel *ProtSiteAtimeLab;
        TLabeledEdit *CytoAmembAbindRateEdit;
        TLabeledEdit *SiteABbindingRateEdit;
        TLabel *SiteABtimeLab;
        TLabeledEdit *NucleusRadEdit;
        TLabeledEdit *RedCytoAfractionEdit;
        TLabeledEdit *RedMembBfractionEdit;
        TLabeledEdit *RedMembAfractionEdit;
        TLabeledEdit *RedCytoAnumEdit;
        TLabeledEdit *RedMembAnumEdit;
        TTabSheet *StaticApage;
        TLabeledEdit *FilamentPitchStepEdit;
        TCheckBox *CytoAcanMoveAlongStatA;
        TComboBox *StatAinitBox;
        TLabel *Label3;
        TLabel *Label4;
        TLabeledEdit *RedMembBnumEdit;
        TLabeledEdit *CytoAmembBbindRateEdit;
        TLabeledEdit *StatAnumEdit;
        TLabeledEdit *GreenStatAnumEdit;
        TLabeledEdit *GreenStatAfractionEdit;
        TCheckBox *CytoAcanBindStatAchBox;
        TLabeledEdit *RedStatAnumEdit;
        TLabeledEdit *RedStatAfractionEdit;
        TLabeledEdit *CytoAstatAbindRateEdit;
        TLabeledEdit *CytoAstatAdissRateEdit;
        TLabeledEdit *FlatteningEdit;
        TTabSheet *IllumImageSheet;
        TLabeledEdit *SpotSizeEdit;
        TLabeledEdit *GreenEmissionRateEdit;
        TLabeledEdit *GreenBleachRateEdit;
        TLabeledEdit *KobjEdit;
        TGroupBox *IllumGroupBox;
        TLabel *Label2;
        TImage *GaussImage;
        TLabeledEdit *IllumFWHMEdit;
        TLabeledEdit *IrisRadiusEdit;
        TLabeledEdit *TIRangleEdit;
        TLabeledEdit *RedEmissionRateEdit;
        TLabeledEdit *RedBleachRateEdit;
        TGroupBox *ImagingGroupBox;
        TLabeledEdit *FramesToSumEdit;
        TLabeledEdit *DarkFramesEdit;
        TLabeledEdit *CameraNoiseEdit;
        TCheckBox *EMCCD_ChBox;
        TLabeledEdit *GainEdit;
	TCheckBox *TwoCamChBox;
        TTabSheet *CytoB_Sheet;
        TLabeledEdit *CytoBnumEdit;
        TLabeledEdit *MembCnumEdit;
        TLabeledEdit *CytoBmobEdit;
        TCheckBox *OneColourChBox;
        TLabel *fps2Lab;
        TCheckBox *TwoAngleChBox;
        TCheckBox *AlterChBox;
	TTabSheet *StructuresSheet;
	TCheckBox *LamellaChBox;
	TCheckBox *FilopodiaChBox;
	TCheckBox *TubeNetworkChBox;
	TLabeledEdit *TubeRadEdit;
	TLabeledEdit *TubeDistEdit;
	TLabeledEdit *TubeLengthEdit;
	TImage *StructureImage;
	TCheckBox *TubesChBox;
	TLabeledEdit *SideGapEdit;
	TLabeledEdit *BottomGapEdit;
	TLabeledEdit *TopGapEdit;
	TComboBox *CytoAinitBox;
	TLabel *Label5;
	TLabeledEdit *CytoAstepSizeEdit;
	TLabeledEdit *CytoAstepRateEdit;
	TLabeledEdit *PoreRadiusEdit;
	TLabeledEdit *VesTiltEdit;
	TLabeledEdit *FiberExtentionEdit;
	TCheckBox *CytoAcanBindCytoB_ChBox;
	TLabel *CytoBconcLab;
	TLabeledEdit *CytoAcytoBbindRateEdit;
	TLabeledEdit *CytoAcytoBdissRateEdit;
	TComboBox *CytoBinitBox;
	TLabel *Label6;
	TLabeledEdit *GreenMembCnumEdit;
	TLabeledEdit *GreenMembCfractionEdit;
	TLabeledEdit *RedMembCnumEdit;
	TLabeledEdit *RedMembCfractionEdit;
	TLabeledEdit *MembCmobEdit;
	TCheckBox *MembBcanBindMembC_ChBox;
	TLabeledEdit *MembBCbindingRateEdit;
	TLabeledEdit *MembBCdissRateEdit;
	TComboBox *MembCinitBox;
	TLabel *Label7;
	TCheckBox *GreenBlinkingChBox;
	TCheckBox *RedBlinkingChBox;
	TCheckBox *FRETchBox;
	TCheckBox *MovingVesChBox;
	TLabeledEdit *VesMobEdit;
	TCheckBox *FuseVesiclesChBox;
	TCheckBox *TransferChBox;
	TCheckBox *MoveToNucleusChBox;
	TCheckBox *MembBmoveCytoAcyto;
	TCheckBox *RoundedCellBodyChBox;
	TLabeledEdit *BodyFilletEdit;
	TCheckBox *OutsideChBox;
	TLabel *SiteCdensLab;
	TLabeledEdit *VesRadEdit;
	TLabeledEdit *VesNumEdit;
	TGroupBox *ModelGrBox;
	TLabeledEdit *InteractionDistanceEdit;
	TLabeledEdit *TimeIntervalEdit;
	TLabel *fpsLab;
	TLabeledEdit *VoxelSizeEdit;
	TLabeledEdit *MemorySizeEdit;
	TLabel *MatrixLab;
	TLabeledEdit *MembApoliNumEdit;
	TLabeledEdit *ClusterNumEdit;
	TCheckBox *MitoChBox;
	TLabeledEdit *MonoPerFiloEdit;
	TLabeledEdit *BlinkOnEdit;
	TLabeledEdit *BlinkOffEdit;
	TCheckBox *Tube3Dbox;
	TLabeledEdit *GreenCytoBnumEdit;
	TLabeledEdit *RedCytoBnumEdit;
	TLabeledEdit *GreenCytoBfractionEdit;
	TLabeledEdit *RedCytoBfractionEdit;
	TCheckBox *CytoApolimerChBox;
	TLabeledEdit *CytoApolimerEdit;
	TLabeledEdit *CytoAdepoliEdit;
	TLabel *PlenLab;
	TLabeledEdit *CytoBdistAedit;
	TCheckBox *RecycleChBox;
	TLabel *VesiKdiffLab;
        void __fastcall OK_ButtonClick(TObject *Sender);
        void __fastcall LengthEditExit(TObject *Sender);
        void __fastcall LengthEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall WidthEditExit(TObject *Sender);
        void __fastcall WidthEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall HeightEditExit(TObject *Sender);
        void __fastcall HeightEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall NucleusChBoxClick(TObject *Sender);
        void __fastcall CytoAnumEditExit(TObject *Sender);
        void __fastcall CytoAnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall GreenCytoAnumEditExit(TObject *Sender);
        void __fastcall CytoAmobEditExit(TObject *Sender);
        void __fastcall CancelButtonClick(TObject *Sender);
        void __fastcall GreenCytoAnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CytoAmobEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MembAnumEditKeyPress(TObject *Sender, char &Key);
        void __fastcall MembAnumEditExit(TObject *Sender);
        void __fastcall GreenMembAnumEditExit(TObject *Sender);
        void __fastcall GreenMembAnumEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall MembAmobEditExit(TObject *Sender);
        void __fastcall MembAmobEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CytoAcanBindMembA_ChBoxClick(TObject *Sender);
        void __fastcall CytoAmembAdissRateEditExit(TObject *Sender);
        void __fastcall CytoAmembAdissRateEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MembApolimerChBoxClick(TObject *Sender);
        void __fastcall AAdissRateEditExit(TObject *Sender);
        void __fastcall AAdissRateEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MembAinitBoxChange(TObject *Sender);
        void __fastcall InteractionDistanceEditExit(TObject *Sender);
        void __fastcall InteractionDistanceEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MembBnumEditExit(TObject *Sender);
        void __fastcall MembBnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall GreenMembBnumEditExit(TObject *Sender);
        void __fastcall GreenMembBnumEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall MembBmobEditExit(TObject *Sender);
        void __fastcall MembBmobEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CytoAcanBindMembB_ChBoxClick(TObject *Sender);
        void __fastcall CytoAmembBdissRateEditExit(TObject *Sender);
        void __fastcall CytoAmembBdissRateEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MembAcanBindMembB_ChBoxClick(TObject *Sender);
        void __fastcall AfromBdissRateEditExit(TObject *Sender);
        void __fastcall AfromBdissRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall GreenCytoAfractionEditExit(TObject *Sender);
        void __fastcall GreenCytoAfractionEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall GreenMembAfractionEditExit(TObject *Sender);
        void __fastcall GreenMembAfractionEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall GreenMembBfractionEditExit(TObject *Sender);
        void __fastcall GreenMembBfractionEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
		void __fastcall RaftsChBoxClick(TObject *Sender);
        void __fastcall RaftSizeEditExit(TObject *Sender);
        void __fastcall RaftSizeEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	    void __fastcall MembBinitBoxChange(TObject *Sender);
        void __fastcall ClusterNumEditExit(TObject *Sender);
        void __fastcall ClusterNumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ReleaseRateEditExit(TObject *Sender);
        void __fastcall ReleaseRateEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall SpotSizeEditExit(TObject *Sender);
        void __fastcall SpotSizeEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall GreenEmissionRateEditExit(TObject *Sender);
        void __fastcall GreenEmissionRateEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall GreenBleachRateEditExit(TObject *Sender);
        void __fastcall GreenBleachRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall IllumFWHMEditExit(TObject *Sender);
        void __fastcall IllumFWHMEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall TimeIntervalEditExit(TObject *Sender);
        void __fastcall TimeIntervalEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall IrisRadiusEditExit(TObject *Sender);
        void __fastcall IrisRadiusEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FramesToSumEditExit(TObject *Sender);
        void __fastcall FramesToSumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall DarkFramesEditExit(TObject *Sender);
        void __fastcall DarkFramesEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CameraNoiseEditExit(TObject *Sender);
        void __fastcall CameraNoiseEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall EMCCD_ChBoxClick(TObject *Sender);
        void __fastcall GainEditExit(TObject *Sender);
        void __fastcall GainEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MonomersInFilamentEditExit(TObject *Sender);
        void __fastcall AAbindingRateEditExit(TObject *Sender);
        void __fastcall AAbindingRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall RaftMobRatioEditExit(TObject *Sender);
        void __fastcall RaftMobRatioEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CytoAmembAbindRateEditExit(TObject *Sender);
        void __fastcall CytoAmembAbindRateEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall SiteABbindingRateEditExit(TObject *Sender);
        void __fastcall SiteABbindingRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall TubeNetworkChBoxClick(TObject *Sender);
        void __fastcall FilamentPitchStepEditExit(TObject *Sender);
        void __fastcall FilamentPitchStepEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall TubeRadEditExit(TObject *Sender);
        void __fastcall TubeRadEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall NucleusRadEditExit(TObject *Sender);
        void __fastcall NucleusRadEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall RedCytoAfractionEditExit(TObject *Sender);
        void __fastcall RedCytoAfractionEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall RedMembAfractionEditExit(TObject *Sender);
        void __fastcall RedMembAfractionEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall RedMembBfractionEditExit(TObject *Sender);
        void __fastcall RedMembBfractionEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall TwoCamChBoxClick(TObject *Sender);
        void __fastcall KobjEditExit(TObject *Sender);
        void __fastcall KobjEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall RedCytoAnumEditExit(TObject *Sender);
        void __fastcall RedCytoAnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall RedMembAnumEditExit(TObject *Sender);
        void __fastcall RedMembAnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall RedBleachRateEditExit(TObject *Sender);
        void __fastcall RedBleachRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall RedEmissionRateEditExit(TObject *Sender);
        void __fastcall RedEmissionRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall RedMembBnumEditExit(TObject *Sender);
        void __fastcall RedMembBnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CytoAmembBbindRateEditExit(TObject *Sender);
        void __fastcall CytoAmembBbindRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall StatAnumEditExit(TObject *Sender);
        void __fastcall GreenStatAnumEditExit(TObject *Sender);
        void __fastcall GreenStatAnumEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall StatAnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall StatAinitBoxChange(TObject *Sender);
        void __fastcall GreenStatAfractionEditExit(TObject *Sender);
        void __fastcall GreenStatAfractionEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall CytoAcanBindStatAchBoxClick(TObject *Sender);
        void __fastcall RedStatAnumEditExit(TObject *Sender);
        void __fastcall RedStatAnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall RedStatAfractionEditExit(TObject *Sender);
        void __fastcall RedStatAfractionEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall CytoAstatAbindRateEditExit(TObject *Sender);
        void __fastcall CytoAstatAbindRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall CytoAstatAdissRateEditExit(TObject *Sender);
        void __fastcall CytoAstatAdissRateEditKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift);
        void __fastcall FlatteningEditExit(TObject *Sender);
        void __fastcall FlatteningEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CytoAcanMoveAlongStatAClick(TObject *Sender);
        void __fastcall TIRangleEditExit(TObject *Sender);
        void __fastcall TIRangleEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CytoBnumEditExit(TObject *Sender);
        void __fastcall CytoBnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MembCnumEditExit(TObject *Sender);
        void __fastcall MembCnumEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall TwoAngleChBoxClick(TObject *Sender);
        void __fastcall CytoBmobEditExit(TObject *Sender);
        void __fastcall CytoBmobEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall OneColourChBoxClick(TObject *Sender);
        void __fastcall AlterChBoxClick(TObject *Sender);
	void __fastcall FilopodiaChBoxClick(TObject *Sender);
	void __fastcall TubeDistEditExit(TObject *Sender);
	void __fastcall TubeDistEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall LamellaChBoxClick(TObject *Sender);
	void __fastcall TubeLengthEditExit(TObject *Sender);
	void __fastcall TubesChBoxClick(TObject *Sender);
	void __fastcall SideGapEditExit(TObject *Sender);
	void __fastcall SideGapEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BottomGapEditExit(TObject *Sender);
	void __fastcall BottomGapEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TopGapEditExit(TObject *Sender);
	void __fastcall TopGapEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoAinitBoxChange(TObject *Sender);
	void __fastcall CytoAstepSizeEditExit(TObject *Sender);
	void __fastcall CytoAstepSizeEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoAstepRateEditExit(TObject *Sender);
	void __fastcall CytoAstepRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TubeLengthEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PoreRadiusEditExit(TObject *Sender);
	void __fastcall PoreRadiusEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall VesTiltEditExit(TObject *Sender);
	void __fastcall VesTiltEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FiberExtentionEditExit(TObject *Sender);
	void __fastcall FiberExtentionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoAcytoBbindRateEditExit(TObject *Sender);
	void __fastcall CytoAcytoBbindRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoAcytoBdissRateEditExit(TObject *Sender);
	void __fastcall CytoAcytoBdissRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoAcanBindCytoB_ChBoxClick(TObject *Sender);
	void __fastcall CytoBinitBoxChange(TObject *Sender);
	void __fastcall MembBcanBindMembC_ChBoxClick(TObject *Sender);
	void __fastcall MembCinitBoxChange(TObject *Sender);
	void __fastcall MembCmobEditExit(TObject *Sender);
	void __fastcall GreenMembCnumEditExit(TObject *Sender);
	void __fastcall GreenMembCnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RedMembCnumEditExit(TObject *Sender);
	void __fastcall RedMembCnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GreenMembCfractionEditExit(TObject *Sender);
	void __fastcall GreenMembCfractionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RedMembCfractionEditExit(TObject *Sender);
	void __fastcall RedMembCfractionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MembBCbindingRateEditExit(TObject *Sender);
	void __fastcall MembBCbindingRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MembBCdissRateEditExit(TObject *Sender);
	void __fastcall MembBCdissRateEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FRETchBoxClick(TObject *Sender);
	void __fastcall RoundedCellBodyChBoxClick(TObject *Sender);
	void __fastcall BodyFilletEditExit(TObject *Sender);
	void __fastcall BodyFilletEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MovingVesChBoxClick(TObject *Sender);
	void __fastcall VesMobEditExit(TObject *Sender);
	void __fastcall VesMobEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MoveToNucleusChBoxClick(TObject *Sender);
	void __fastcall MembBmoveCytoAcytoClick(TObject *Sender);
	void __fastcall OutsideChBoxClick(TObject *Sender);
	void __fastcall MembCmobEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall VesNumEditExit(TObject *Sender);
	void __fastcall VesNumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall VesRadEditExit(TObject *Sender);
	void __fastcall VesRadEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall VoxelSizeEditExit(TObject *Sender);
	void __fastcall MemorySizeEditExit(TObject *Sender);
	void __fastcall VoxelSizeEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MembApoliNumEditExit(TObject *Sender);
	void __fastcall MembApoliNumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MitoChBoxClick(TObject *Sender);
	void __fastcall MonoPerFiloEditExit(TObject *Sender);
	void __fastcall MonoPerFiloEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BlinkOnEditExit(TObject *Sender);
	void __fastcall BlinkOffEditExit(TObject *Sender);
	void __fastcall BlinkOnEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BlinkOffEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Tube3DboxClick(TObject *Sender);
	void __fastcall GreenCytoBnumEditExit(TObject *Sender);
	void __fastcall GreenCytoBnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RedCytoBnumEditExit(TObject *Sender);
	void __fastcall RedCytoBnumEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GreenCytoBfractionEditExit(TObject *Sender);
	void __fastcall GreenCytoBfractionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RedCytoBfractionEditExit(TObject *Sender);
	void __fastcall RedCytoBfractionEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoApolimerChBoxClick(TObject *Sender);
	void __fastcall CytoApolimerEditExit(TObject *Sender);
	void __fastcall CytoApolimerEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoAdepoliEditExit(TObject *Sender);
	void __fastcall CytoAdepoliEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CytoBdistAeditExit(TObject *Sender);
	void __fastcall CytoBdistAeditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RecycleChBoxClick(TObject *Sender);
	void __fastcall FuseVesiclesChBoxClick(TObject *Sender);

 private:	// User declarations


//-----------------------------------------------------------------
double Rounding(double val,int digits)
{ double mult=pow(10.0,digits);
  double intRes=(__int64)(val*mult);
  if ( val > 0.0 && (val*mult)-intRes > 0.5 ) intRes+=1.0;
  if ( val < 0.0 && (val*mult)-intRes < -0.5 ) intRes-=1.0;
  return intRes*pow(0.1,digits);
}
//------------------------------------------------------------------

 public:		// User declarations
		__fastcall TSForm(TComponent* Owner);
 void __fastcall DrawCell(void);
 double __fastcall TextToFloat(AnsiString text,double defNum);
 void __fastcall SetParam(void);
 void __fastcall Statistics(void);
 void __fastcall DrawGauss(void);
 void Line(TCanvas *cnv,int x1,int y1,int x2,int y2);
 bool CheckSize(int x, int y, int z, int side, int bot, int top);
 double __fastcall CalcPlen(void);
  COND cond;
  __int64 matrixSize;

 inline double SIN(double ang) { return sin((PI/180.0)*ang);}
 inline double COS(double ang) { return cos((PI/180.0)*ang);}
 inline double TAN(double ang) { return tan((PI/180.0)*ang);}
 double inline RandOne(void)
   { double val=random(2000000)-1000000; return val*1e-6; }



};
//---------------------------------------------------------------------------
extern PACKAGE TSForm *SForm;
//---------------------------------------------------------------------------
#endif
