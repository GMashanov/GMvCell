   Cell/Matrix Properties 
voxelSizeNm,10;
memorySizeMb,24000;
modelTimeStepSec,0.001;
cellSizeXnm,10000;
cellSizeYnm,10000;
cellSizeZnm,2000;
sideGapNm,1000;
topGapNm,100;
botGapNm,40;
roundedCellBody,ON;
cellFilletNm,1000;
nucleusRadNm,3000;
flattening,4;
nucleus,ON;
rafts,OFF;
raftNumber,100;
raftRadNm,100;
raftMobilityRatio,0.1;
interactionDistanceUm,0.005;
filopodia,OFF;
lamella,OFF;
tubes,OFF;
tubeNetwork,OFF;
tubeRadNm,100;
tubeLengthNm,500;
interTubeDistNm,1000;
tubeZnm,180;
tubeGapNm,30;
bendingCoef,0;
tube3D,OFF;
vesicleRadNm,100;
vesicleNumber,20;
movingVesicles,OFF;
vesRecycling,OFF;
outsideVesicles,ON;
vesicleMob,0.02;
fuseVesicles,ON;
instantRelease,OFF;
poreRadNm,40;

  Illumination Settings;
FWHMnm,250;
greenPhotonsPerSec,6000;
greenBleachRate,0.02;
redPhotonsPerSec,10000;
redBleachRate,0.02;
greenBlinking,OFF;
greenBlinkONrate,1;
greenBlinkOFFrate,5;
FRET,OFF;
alternatingIllumination,OFF;
laserFWHMum,100;
diaphragmRadUm,50;
bleachSizeNm,5000;

  Camera Settings;
objectiveLensZcoef,0.6;
cameraNmPix,35;
focusNm,320;
twoCameraMode,OFF;
EMCCDmode,ON;
cameraGain,5;
timeStepsPerImage,50;
darkFramesNumber,0;
cameraNoiseCountsPix,1.5;
FramesToSave,0;

  CytoA Object Settings;
cytoAnumber,2000;
cytoAdistribution,CYTOPLASM;
greenFluorsPerCytoA,1;
greenFractionCytoA%,5;
redFluorsPerCytoA,0;
redFractionCytoA%,10;
cytoAdiffusionRate,2;
cytoAstepSizeNm,8;
cytoAstepRate,100;
cytoApolimer,OFF;
cytoApolimerRate,1000000;
cytoAdepolimerRate,1;

  CytoB Object Settings;
cytoBnumber,0;
cytoBdistribution,CYTOPLASM;
greenFluorsPerCytoB,3;
greenFractionCytoB%,50;
redFluorsPerCytoB,4;
redFractionCytoB%,80;
cytoBdiffusionRate,2;
cytoAbindingCytoB,OFF;
cytoAcytoBbindRate,1000000;
cytoAcytoBdissRate,0.1;

  MembA Object Settings;
membAnumber,1000;
membAdistribution,NUCLEUS_MEMBRANE;
greenFluorsPerMembA,1;
greenFractionMembA%,100;
redFluorsPerMembA,0;
redFractionMembA%,100;
membAdiffusionRate,0.2;
cytoAbindingMembA,ON;
cytoAmembAbindRate,1000000;
cytoAmembAdissRate,2;
membAorganellePump,ON;
membApolimer,OFF;
membApolimerRate,20000;
membAdepolimerRate,1;
membAmaxUnitPolimer,16;

  MembB Object Settings;
membBnumber,1000;
membBdistribution,NUCLEUS_MEMBRANE;
greenFluorsPerMembB,0;
greenFractionMembB%,100;
redFluorsPerMembB,1;
redFractionMembB%,100;
membBdiffusionRate,0.1;
cytoAbindingMembB,OFF;
cytoAmembBbindRate,1000000;
cytoAmembBdissRate,1;
membBcytoplasmPump,OFF;
membAbindingMembB,OFF;
membAmembBbindRate,10000;
membAmembBdissRate,0.1;

  MembC Object Settings;
membCnumber,0;
membCdistribution,NUCLEUS_MEMBRANE;
greenFluorsPerMembC,0;
greenFractionMembC%,100;
redFluorsPerMembC,4;
redFractionMembC%,100;
membCdiffusionRate,0.03;
membBbindingMembC,ON;
membBmembCbindRate,100000;
membBmembCdissRate,1;

  StatA Object Settings;
statAnumber,0;
statAdistribution,STRAIT_FIBERS_XY;
greenFluorsPerStatA,2;
greenFractionStatA%,100;
redFluorsPerStatA,0;
redFractionStatA%,100;
statAmonomerDistanceNm,8;
statAmonoPerFilo,200;
statAbundleExtensionNm,50;
statAbindingCytoA,OFF;
cytoAstatAbindRate,500000;
cytoAstatAdissRate,0.1;
cytoAmoveAlongStatA,OFF;