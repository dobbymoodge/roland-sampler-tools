/************************************************************************/
/* NAME									*/
/*	samprint - ASCII dump for Roland 12-bit-sampler-disks		*/
/* SYNOPSIS								*/
/*	samprint Roland-12-bit-sample-disk-files			*/
/* DESCRIPTION								*/
/*	Samprint prints the contents of samples disks for the Roland	*/
/*	12-bit samplers (S-550, S-330, etc.).  This verifies the disk	*/
/*	format specified by the header file rs12.h that is derived from	*/
/*	the MIDI implementation detailed in the S-550 Owners Manual.	*/
/* AUTHOR								*/
/*	Copyright 1992 by Gary J. Murakami <gjm@research.att.com>	*/
/*	V1.1 (12 May 1992) Freely distibutable for non-comercial use.	*/
/*	All other rights reserved.					*/
/************************************************************************/

#include <stdio.h>
#include "rs12.h"

int
int7(byte c)
{
	signed char i;
	return (i = c);
}

long
uint24(byte * p)
{
	long	i;
	i = *p++;
	i = (i << 8) + *p++;
	i = (i << 8) + *p++;
	return (i);
}

int
sample(byte * p, int si)
{
	int	i, s;

	i = si*3/2;
	if (si % 2) /* odd */
	{	s = (*(p+i+1) & 0x80) ? (-1 << 11) : 0;
		s |= ((*(p+i) & 0x0F) /* << 0 */) | (*(p+i+1) << 4);
	}
	else	/* even */
	{	s = (*(p+i) & 0x80) ? (-1 << 11) : 0;
		s |= ((*(p+i) & 0xFF) << 4) | ((*(p+i+1) & 0xF0) >> 4);
	}
	return (s);
}

void
printDummy(char * n, int len, byte * p)
{
/*
	int	i;

	printf("%24s[%d]\t%s\t", n, len, "0-127");
	for (i = 0; i < len; i++) printf("%d ", *p++);
	putchar('\n');
*/
}

void
printSystemProgram(SystemProgram * p)
{
	printf("%24s[%s]\n", "data", "0000000:0176000 (octal)");
}

char * sKeyMode[] = {"Normal","V-Sw","X-Fade","V-Mix","Unison","","",""};
char * sAfterTouchAssign[] = {"Modulation","Volume","Bend +","Bend -",
	"Filter","","",""};
char * sKeyAssign[] = {"Rotary", "Fix"};
char * sOutputAssign[] = {"Out 1", "Out 2", "Out 3", "Out 4", "Out 5",
	"Out 6", "Out 7", "Out 8","Tone","","","","","","",""};

void
printPatchParameter(PatchParameter * p)
{
	int i;

	printf("%24s[1:12]\t%s\t\"%12.12s\"\n","patchName","ASCII",p->patchName);
	printf("%24s\t%s\t%d\n","bendRange","0-11",p->bendRange);
	printDummy("dummy1", 1, p->dummy1);
	printf("%24s\t%s\t%d\n","afterTouchSense","0-127",p->afterTouchSense);
	printf("%24s\t%s\t%d(%s)\n","keyMode","0-4",p->keyMode,
		sKeyMode[p->keyMode&0x03]);
	printf("%24s\t%s\t%d\n", "velocitySwThreshold","0-127",p->velocitySwThreshold);
	printf("%24s[1:109]\t%s\t","toneToKey1","0-31");
		for (i = 0; i < 109; i++) printf("%d ",p->toneToKey1[i]);
		putchar('\n');
	printf("%24s[1:109]\t%s\t","toneToKey2","0-31");
		for (i = 0; i < 109; i++) printf("%d ",p->toneToKey2[i]);
		putchar('\n');
	printf("%24s\t%s\t%d\n","copySource","0-7",p->copySource);
	printf("%24s\t%s\t%d\n","octaveShift","-2-+2",int7(p->octaveShift));
	printf("%24s\t%s\t%d\n","outputLevel","0-127",p->outputLevel);
	printDummy("dummy2", 1, p->dummy2);
	printf("%24s\t%s\t%d\n","detune","-64-+63",int7(p->detune));
	printf("%24s\t%s\t%d\n","velocityMixRatio","0-127",p->velocityMixRatio);
	printf("%24s\t%s\t%d(%s)\n","afterTouchAssign","0-4",p->afterTouchAssign,
		sAfterTouchAssign[p->afterTouchAssign&0x3]);
	printf("%24s\t%s\t%d(%s)\n","keyAssign","0-1",p->keyAssign,
		sKeyAssign[p->keyAssign&0x1]);
	printf("%24s\t%s\t%d(%s)\n", "outputAssign","0-8",p->outputAssign,
		sOutputAssign[p->outputAssign&0xF]);
	printDummy("dummy3", 12, p->dummy3);
}

char * sOffOn[] = {"Off", "On"};
char * sVoiceMode[] = {"VAL","VAF","F1","F2","F3","F4","F5","F6","F7","F8",
	"F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19",
	"F20","F21","F22","","","","","","","",""};
char * sKeyboardDisplay[] = {"A","B","C","D","E","F","G","H","All",
	"","","","","","",""};
char * sExternalController[] = {"Off","Mouse","RC-100",""};

void
printFunctionParameter(FunctionParameter * p)
{
	int	i;

	printf("%24s\t%s\t%d\n","masterTune","-64-+63",int7(p->masterTune));
	printDummy("dummy1", 13, p->dummy1);
	printf("%24s\t%s\t%d(%s)\n","audioTrig","0-1",p->audioTrig,
		sOffOn[p->audioTrig&0x1]);
	printDummy("dummy2", 1, p->dummy2);
	printf("%24s\t%s\t%d(%s)\n","voiceMode","0-23",p->voiceMode,
		sVoiceMode[p->voiceMode&0x1F]);
	printf("%24s[1:8]\t%s\t","multiMidiRxCh","0-15");
		for (i = 0; i < 8; i++) printf("%d ", p->multiMidiRxCh[i]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","multiPatchNumber","0-31");
		for (i = 0; i < 8; i++) printf("%d ", p->multiPatchNumber[i]);
		putchar('\n');
	printDummy("dummy3", 9, p->dummy3);
	printf("%24s\t%s\t%d(%s)\n","keyboardDisplay","0-8",p->keyboardDisplay,
		sKeyboardDisplay[p->keyboardDisplay&0xF]);
	printf("%24s[1:8]\t%s\t","multiLevel","0-127");
		for (i = 0; i < 8; i++) printf("%d ", p->multiLevel[i]);
		putchar('\n');
	printf("%24s[1:60]\t%s\t\"%60s\"\n", "diskLabel","ASCII",p->diskLabel);
	printDummy("dummy4", 4, p->dummy4);
	printf("%24s\t%s\t%d(%s)\n","externalController","0-2",p->externalController,
		sExternalController[p->externalController&0x3]);
	printDummy("dummy5", 140, p->dummy5);
}

char * sRxChannel[] = {"1","2","3","4","5","6","7","8",
	"9","10","11","12","13","14","15","16",
	"Off","","","","","","","","","","","","","","",""};
void
printMidiParameter(MidiParameter * p)
{
	int	i;

	printDummy("dummy1", 32, p->dummy1);
	printf("%24s[1:8]\t%s\t","rxChannel","0-16");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxChannel[i],
			sRxChannel[p->rxChannel[i]&0xF]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","rxProgramChange","0-1");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxProgramChange[i],
			sOffOn[p->rxProgramChange[i]&0x1]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","rxBender","0-1");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxBender[i],
			sOffOn[p->rxBender[i]&0x1]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","rxModulation","0-1");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxModulation[i],
			sOffOn[p->rxModulation[i]&0x1]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","rxHold","0-1");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxHold[i],
			sOffOn[p->rxHold[i]&0x1]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","rxAfterTouch","0-1");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxAfterTouch[i],
			sOffOn[p->rxAfterTouch[i]&0x1]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","rxVolume","0-1");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxVolume[i],
			sOffOn[p->rxVolume[i]&0x1]);
		putchar('\n');
	printf("%24s[1:8]\t%s\t","rxBendRange","0-1");
		for (i = 0; i < 8; i++) printf("%d(%s) ",p->rxBendRange[i],
			sOffOn[p->rxBendRange[i]&0x1]);
		putchar('\n');
	printDummy("dummy2", 1, p->dummy2);
	printf("%24s\t%s\t%d(%s)\n","systemExclusive","0-1",p->systemExclusive,
		sOffOn[p->systemExclusive&0x1]);
	printf("%24s\t%s\t%d(%s)\n","deviceId","0-15",p->deviceId);
	printDummy("dummy3", 1, p->dummy3);
	printf("%24s[1:32]\t%s\t","rxProgramChangeNumber","0-127");
		for (i = 0; i < 32; i++) printf("%d ",p->rxProgramChangeNumber[i]);
		putchar('\n');
	printDummy("dummy4", 124, p->dummy4);
}

char * sOrgSubTone[] = {"Org","Sub"};
char * sSamplingFrequency[] = {"30KHz","15KHz"};
char * sWaveBank[] = {"A","B"};
char * sLoopMode[] = {"Fwd","Alt","1Shot","Reverse"};
char * sLfoMode[] = {"Normal","One Shot"};
char * sLfoPolarity[] = {"Sine","Peak Hold"};
char * sTvfEgPolarity[] = {"Normal","Reverse"};
char * sRecPreTrigger[] = {"0ms","10ms","50ms","100ms"};

void
printToneParameter(ToneParameter * p)
{
	printf("%24s[1:8]\t%s\t\"%8s\"\n", "toneName","ASCII",p->toneName);
	printf("%24s\t%s\t%d\n","outputAssign","0-7",p->outputAssign);
	printf("%24s\t%s\t%d\n","sourceTone","0-31",p->sourceTone);
	printf("%24s\t%s\t%d(%s)\n","orgSubTone","0-1",p->orgSubTone,
		sOrgSubTone[p->orgSubTone&0x1]);
	printf("%24s\t%s\t%d(%s)\n","samplingFrequency","0-1",p->samplingFrequency,
		sSamplingFrequency[p->samplingFrequency&0x1]);
	printf("%24s\t%s\t%d\n","origKeyNumber","11-120",p->origKeyNumber);
	printf("%24s\t%s\t%d(%s)\n","waveBank","0-1",p->waveBank,
		sWaveBank[p->waveBank&0x1]);
	printf("%24s\t%s\t%d\n","waveSegmentTop","0-17",p->waveSegmentTop);
	printf("%24s\t%s\t%d\n","waveSegmentLength","0-18",p->waveSegmentLength);
	printf("%24s[1:3]\t%s\t%ld\n","startPoint","0-221180",uint24(p->startPoint));
	printf("%24s[1:3]\t%s\t%ld\n","endPoint","0-221180",uint24(p->endPoint));
	printf("%24s[1:3]\t%s\t%ld\n","loopPoint","0-221180",uint24(p->loopPoint));
	printf("%24s\t%s\t%d(%s)\n","loopMode","0-3",p->loopMode,
		sLoopMode[p->loopMode&0x2]);
	printf("%24s\t%s\t%d\n","tvaLfoDepth","0-127",p->tvaLfoDepth);
	printDummy("dummy1", 1, p->dummy1);
	printf("%24s\t%s\t%d\n","lfoRate","0-127",p->lfoRate);
	printf("%24s\t%s\t%d(%s)\n","lfoSync","0-1",p->lfoSync,
		sOffOn[p->lfoSync&0x1]);
	printf("%24s\t%s\t%d\n","lfoDelay","0-127",p->lfoDelay);
	printDummy("dummy2", 1, p->dummy2);
	printf("%24s\t%s\t%d(%s)\n","lfoMode","0-127",p->lfoMode,
		sLfoMode[p->lfoMode&0x1]);
	printf("%24s\t%s\t%d\n","oscLfoDepth","0-127",p->oscLfoDepth);
	printf("%24s\t%s\t%d(%s)\n","lfoPolarity","0-127",p->lfoPolarity,
		sLfoPolarity[p->lfoPolarity&0x1]);
	printf("%24s\t%s\t%d\n","lfoOffset","0-127",p->lfoOffset);
	printf("%24s\t%s\t%d\n","transpose","0-127",p->transpose);
	printf("%24s\t%s\t%d\n","fineTune","-64-+63",int7(p->fineTune));
	printf("%24s\t%s\t%d\n","tvfCutOff","0-127",p->tvfCutOff);
	printf("%24s\t%s\t%d\n","tvfResonance","0-127",p->tvfResonance);
	printf("%24s\t%s\t%d\n","tvfKeyFollow","0-127",p->tvfKeyFollow);
	printDummy("dummy3", 1, p->dummy3);
	printf("%24s\t%s\t%d\n","tvfLfoDepth","0-127",p->tvfLfoDepth);
	printf("%24s\t%s\t%d\n","tvfEgDepth","0-127",p->tvfEgDepth);
	printf("%24s\t%s\t%d\n","tvfEgPolarity","0-127",p->tvfEgPolarity,
		sTvfEgPolarity[p->tvfEgPolarity&0x1]);
	printf("%24s\t%s\t%d\n","tvfLevelCurve","0-5",p->tvfLevelCurve);
	printf("%24s\t%s\t%d\n","tvfKeyRateFollow","0-127",p->tvfKeyRateFollow);
	printf("%24s\t%s\t%d\n","tvfVelocityRateFollow","0-127",p->tvfVelocityRateFollow);
	printDummy("dummy4", 1, p->dummy4);
	printf("%24s\t%s\t%d(%s)\n","tvfSwitch","0-1",p->tvfSwitch,
		sOffOn[p->tvfSwitch&0x1]);
	printf("%24s\t%s\t%d(%s)\n","benderSwitch","0-1",p->benderSwitch,
		sOffOn[p->benderSwitch&0x1]);
	printf("%24s\t%s\t%d\n","tvaEnvSustainPoint","0-7",p->tvaEnvSustainPoint);
	printf("%24s\t%s\t%d\n","tvaEnvEndPoint","1-7",p->tvaEnvEndPoint);
	printf("%24s\t%s\t%d\n","tvaEnvLevel1","0-127",p->tvaEnvLevel1);
	printf("%24s\t%s\t%d\n","tvaEnvRate1","0-127",p->tvaEnvRate1);
	printf("%24s\t%s\t%d\n","tvaEnvLevel2","0-127",p->tvaEnvLevel2);
	printf("%24s\t%s\t%d\n","tvaEnvRate2","0-127",p->tvaEnvRate2);
	printf("%24s\t%s\t%d\n","tvaEnvLevel3","0-127",p->tvaEnvLevel3);
	printf("%24s\t%s\t%d\n","tvaEnvRate3","0-127",p->tvaEnvRate3);
	printf("%24s\t%s\t%d\n","tvaEnvLevel4","0-127",p->tvaEnvLevel4);
	printf("%24s\t%s\t%d\n","tvaEnvRate4","0-127",p->tvaEnvRate4);
	printf("%24s\t%s\t%d\n","tvaEnvLevel5","0-127",p->tvaEnvLevel5);
	printf("%24s\t%s\t%d\n","tvaEnvRate5","0-127",p->tvaEnvRate5);
	printf("%24s\t%s\t%d\n","tvaEnvLevel6","0-127",p->tvaEnvLevel6);
	printf("%24s\t%s\t%d\n","tvaEnvRate6","0-127",p->tvaEnvRate6);
	printf("%24s\t%s\t%d\n","tvaEnvLevel7","0-127",p->tvaEnvLevel7);
	printf("%24s\t%s\t%d\n","tvaEnvRate7","0-127",p->tvaEnvRate7);
	printf("%24s\t%s\t%d\n","tvaEnvLevel8","0-127",p->tvaEnvLevel8);
	printf("%24s\t%s\t%d\n","tvaEnvRate8","0-127",p->tvaEnvRate8);
	printDummy("dummy5", 1, p->dummy5);
	printf("%24s\t%s\t%d\n","tvaEnvKeyRate","0-127",p->tvaEnvKeyRate);
	printf("%24s\t%s\t%d\n","level","0-127",p->level);
	printf("%24s\t%s\t%d\n","envVelRate","0-127",p->envVelRate);
	printf("%24s\t%s\t%d\n","recThreshold","0-127",p->recThreshold);
	printf("%24s\t%s\t%d(%s)\n","recPreTrigger","0-3",p->recPreTrigger,
		sRecPreTrigger[p->recPreTrigger&0x3]);
	printf("%24s\t%s\t%d(%s)\n","recSamplingFrequency","0-1",p->recSamplingFrequency,
		sSamplingFrequency[p->recSamplingFrequency&0x1]);
	printf("%24s[1:3]\t%s\t%ld\n","recStartPoint","0-221180",uint24(p->recStartPoint));
	printf("%24s[1:3]\t%s\t%ld\n","recEndPoint","0-221180",uint24(p->recEndPoint));
	printf("%24s[1:3]\t%s\t%ld\n","recLoopPoint","0-221180",uint24(p->recLoopPoint));
	printf("%24s\t%s\t%d\n","zoomT","0-5",p->zoomT);
	printf("%24s\t%s\t%d\n","zoomL","0-5",p->zoomL);
	printf("%24s\t%s\t%d\n","copySource","0-31",p->copySource);
	printf("%24s\t%s\t%d\n","loopTune","-64-+63",int7(p->loopTune));
	printf("%24s\t%s\t%d\n","tvaLevelCurve","0-5",p->tvaLevelCurve);
	printDummy("dummy6", 12, p->dummy6);
	printf("%24s[1:3]\t%s\t%ld\n","loopLength","0-221180",uint24(p->loopLength));
	printf("%24s\t%s\t%d(%s)\n","pitchFollow","0-1",p->pitchFollow,
		sOffOn[p->pitchFollow&0x1]);
	printf("%24s\t%s\t%d\n","envZoom","0-5",p->envZoom);

	printf("%24s\t%s\t%d\n","tvfEnvSustainPoint","0-7",p->tvfEnvSustainPoint);
	printf("%24s\t%s\t%d\n","tvfEnvEndPoint","1-7",p->tvfEnvEndPoint);
	printf("%24s\t%s\t%d\n","tvfEnvLevel1","0-127",p->tvfEnvLevel1);
	printf("%24s\t%s\t%d\n","tvfEnvRate1","0-127",p->tvfEnvRate1);
	printf("%24s\t%s\t%d\n","tvfEnvLevel2","0-127",p->tvfEnvLevel2);
	printf("%24s\t%s\t%d\n","tvfEnvRate2","0-127",p->tvfEnvRate2);
	printf("%24s\t%s\t%d\n","tvfEnvLevel3","0-127",p->tvfEnvLevel3);
	printf("%24s\t%s\t%d\n","tvfEnvRate3","0-127",p->tvfEnvRate3);
	printf("%24s\t%s\t%d\n","tvfEnvLevel4","0-127",p->tvfEnvLevel4);
	printf("%24s\t%s\t%d\n","tvfEnvRate4","0-127",p->tvfEnvRate4);
	printf("%24s\t%s\t%d\n","tvfEnvLevel5","0-127",p->tvfEnvLevel5);
	printf("%24s\t%s\t%d\n","tvfEnvRate5","0-127",p->tvfEnvRate5);
	printf("%24s\t%s\t%d\n","tvfEnvLevel6","0-127",p->tvfEnvLevel6);
	printf("%24s\t%s\t%d\n","tvfEnvRate6","0-127",p->tvfEnvRate6);
	printf("%24s\t%s\t%d\n","tvfEnvLevel7","0-127",p->tvfEnvLevel7);
	printf("%24s\t%s\t%d\n","tvfEnvRate7","0-127",p->tvfEnvRate7);
	printf("%24s\t%s\t%d\n","tvfEnvLevel8","0-127",p->tvfEnvLevel8);
	printf("%24s\t%s\t%d\n","tvfEnvRate8","0-127",p->tvfEnvRate8);
	printf("%24s\t%s\t%d(%s)\n","afterTouchSwitch","0-1",p->afterTouchSwitch,
		sOffOn[p->afterTouchSwitch&0x1]);
	printDummy("dummy7", 2, p->dummy7);
}

void
printToneList(ToneList * p)
{
	int	i;

	printf("%24s[1:8]\t%s\t\"%8s\"\n", "toneName","ASCII",p->toneName);
	printf("%24s[1:8]\t%s\t", "unknown","0-127");
		for (i = 0; i < 8; i++) printf("%d ", p->unknown[i]);
		putchar('\n');
}

void
printWaveData(WaveData * p)
{
	int	si;
	for (si = 0; si < SamplesPerSegment; si++)
		printf("[%d]\t%d\n", si, sample(p->data, si));
}

void
printDisk(Disk * disk)
{
	int	i;

	printf("systemProgram\n");
	printSystemProgram(&disk->systemProgram);
	for (i = 0; i < 16; i++)
	{	printf("patchParameter[%d]\n", i + 1);
		printPatchParameter(&disk->patchParameter[i]);
	}
	printf("functionParameter\n");
	printFunctionParameter(&disk->functionParameter);
	printf("midiParameter\n");
	printMidiParameter(&disk->midiParameter);
	for (i = 0; i < 32; i++)
	{	printf("toneParameter[%d]\n", i + 1);
		printToneParameter(&disk->toneParameter[i]);
	}
	for (i = 0; i < 32; i++)
	{	printf("toneList[%d]\n", i + 1);
		printToneList(&disk->toneList[i]);
	}
	for (i = 0; i < 18; i++)
	{	printf("waveDataA[%d]\n", i + 1);
		printWaveData(&disk->waveDataA[i]);
	}
	for (i = 0; i < 18; i++)
	{	printf("WaveDataB[%d]\n", i + 1);
		printWaveData(&disk->waveDataB[i]);
	}
}

Disk	disk;

main(int argc, char ** argv)
{
	int	i;
	FILE *	fin;

	if (argc <= 1)
	{	printf("usage: %s Roland-12-bit-sample-disk-files\n", argv[0]);
		exit(1);
	}
	for (i = 1; i < argc; i++)
	{	if ((fin = fopen(argv[i], "r")) != NULL)
		{	fread(&disk, sizeof(Disk), 1, fin);
			printDisk(&disk);
			fclose(fin);
		}
		else	printf("%s: open failed for \"%s\"\n", argv[0], argv[i]);
	}
	exit(0);
}
