#ifndef RS12_H
#define RS12_H

/************************************************************************/
/* NAME									*/
/*	rs12.h								*/
/* DESCRIPTION								*/
/*	Roland 12-bit-sampler disk format for the S-550, S-330,	etc.).	*/
/* AUTHOR								*/
/*	Copyright 1992 by Gary J. Murakami <gjm@research.att.com>	*/
/*	V1.0 (4 May 1992) Freely distibutable for non-comercial use.	*/
/*	All other rights reserved.					*/
/************************************************************************/

#define	SamplesPerSegment	12288

typedef unsigned char byte;

typedef struct systemProgram
{	char	data[0176000];		/* 0xFC00 hex == 64512 dec == 017600 oct */
} SystemProgram;

typedef struct patchParameter
{	byte	patchName[12];
	byte	bendRange;
	byte	dummy1[1];
	byte	afterTouchSense;
	byte	keyMode;
	byte	velocitySwThreshold;
	byte	toneToKey1[109];
	byte	toneToKey2[109];
	byte	copySource;
	byte	octaveShift;
	byte	outputLevel;
	byte	dummy2[1];
	byte	detune;
	byte	velocityMixRatio;
	byte	afterTouchAssign;
	byte	keyAssign;
	byte	outputAssign;
	byte	dummy3[12];
} PatchParameter;

typedef struct functionParameter
{	byte	masterTune;
	byte	dummy1[13];
	byte	audioTrig;
	byte	dummy2[1];
	byte	voiceMode;
	byte	multiMidiRxCh[8];
	byte	multiPatchNumber[8];
	byte	dummy3[9];
	byte	keyboardDisplay;
	byte	multiLevel[8];
	byte	diskLabel[60];
	byte	dummy4[4];
	byte	externalController;
	byte	dummy5[140];
} FunctionParameter;

typedef struct midiParameter
{	byte	dummy1[32];
	byte	rxChannel[8];
	byte	rxProgramChange[8];
	byte	rxBender[8];
	byte	rxModulation[8];
	byte	rxHold[8];
	byte	rxAfterTouch[8];
	byte	rxVolume[8];
	byte	rxBendRange[8];
	byte	dummy2[1];
	byte	systemExclusive;
	byte	deviceId;
	byte	dummy3[1];
	byte	rxProgramChangeNumber[32];
	byte	dummy4[124];
} MidiParameter;

typedef struct toneParameter
{	byte	toneName[8];
	byte	outputAssign;
	byte	sourceTone;
	byte	orgSubTone;
	byte	samplingFrequency;
	byte	origKeyNumber;
	byte	waveBank;
	byte	waveSegmentTop;
	byte	waveSegmentLength;
	byte	startPoint[3];
	byte	endPoint[3];
	byte	loopPoint[3];
	byte	loopMode;
	byte	tvaLfoDepth;
	byte	dummy1[1];
	byte	lfoRate;
	byte	lfoSync;
	byte	lfoDelay;
	byte	dummy2[1];
	byte	lfoMode;
	byte	oscLfoDepth;
	byte	lfoPolarity;
	byte	lfoOffset;
	byte	transpose;
	byte	fineTune;
	byte	tvfCutOff;
	byte	tvfResonance;
	byte	tvfKeyFollow;
	byte	dummy3[1];
	byte	tvfLfoDepth;
	byte	tvfEgDepth;
	byte	tvfEgPolarity;
	byte	tvfLevelCurve;
	byte	tvfKeyRateFollow;
	byte	tvfVelocityRateFollow;
	byte	dummy4[1];
	byte	tvfSwitch;
	byte	benderSwitch;
	byte	tvaEnvSustainPoint;
	byte	tvaEnvEndPoint;
	byte	tvaEnvLevel1;
	byte	tvaEnvRate1;
	byte	tvaEnvLevel2;
	byte	tvaEnvRate2;
	byte	tvaEnvLevel3;
	byte	tvaEnvRate3;
	byte	tvaEnvLevel4;
	byte	tvaEnvRate4;
	byte	tvaEnvLevel5;
	byte	tvaEnvRate5;
	byte	tvaEnvLevel6;
	byte	tvaEnvRate6;
	byte	tvaEnvLevel7;
	byte	tvaEnvRate7;
	byte	tvaEnvLevel8;
	byte	tvaEnvRate8;
	byte	dummy5[1];
	byte	tvaEnvKeyRate;
	byte	level;
	byte	envVelRate;
	byte	recThreshold;
	byte	recPreTrigger;
	byte	recSamplingFrequency;
	byte	recStartPoint[3];
	byte	recEndPoint[3];
	byte	recLoopPoint[3];
	byte	zoomT;
	byte	zoomL;
	byte	copySource;
	byte	loopTune;
	byte	tvaLevelCurve;
	byte	dummy6[12];
	byte	loopLength[3];
	byte	pitchFollow;
	byte	envZoom;
	byte	tvfEnvSustainPoint;
	byte	tvfEnvEndPoint;
 	byte	tvfEnvLevel1;
	byte	tvfEnvRate1;
	byte	tvfEnvLevel2;
	byte	tvfEnvRate2;
	byte	tvfEnvLevel3;
	byte	tvfEnvRate3;
	byte	tvfEnvLevel4;
	byte	tvfEnvRate4;
	byte	tvfEnvLevel5;
	byte	tvfEnvRate5;
	byte	tvfEnvLevel6;
	byte	tvfEnvRate6;
	byte	tvfEnvLevel7;
	byte	tvfEnvRate7;
	byte	tvfEnvLevel8;
	byte	tvfEnvRate8;
	byte	afterTouchSwitch;
	byte	dummy7[2];
} ToneParameter;

typedef struct toneList
{	byte	toneName[8];
	byte	unknown[8];
} ToneList;

typedef struct waveData
{	byte	data[SamplesPerSegment*3/2];
} WaveData;

typedef struct disk
{	SystemProgram		systemProgram;		/* 0000000 */
	PatchParameter		patchParameter[16];	/* 0176000 */
	FunctionParameter	functionParameter;	/* 0206000 */
	MidiParameter		midiParameter;		/* 0206400 */
	ToneParameter		toneParameter[32];	/* 0207000 */
	ToneList		toneList[32];		/* 0217000 */
	WaveData		waveDataA[18];		/* 0220000 */
	WaveData		waveDataB[18];
} Disk;

#endif
