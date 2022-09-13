/************************************************************************/
/* NAME									*/
/*	raw8 - convert Roland 12-bit-sample-disk to raw 8-bit samples	*/
/* SYNOPSIS								*/
/*	raw8 roland-12-bit-sample-disk-file > raw-8-bit-sample-file	*/
/* DESCRIPTION								*/
/*	raw8 converts Roland 12-bit-samples to raw 8-bit samples	*/
/* AUTHOR								*/
/*	Copyright 1992 by Gary J. Murakami <gjm@research.att.com>	*/
/*	V1.0 (4 May 1992) Freely distibutable for non-comercial use.	*/
/*	All other rights reserved.					*/
/************************************************************************/

#include <stdio.h>
#include "rs12.h"

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
convertDisk(Disk * disk)
{
	int	seg, si;
	for (seg = 0; seg < 18; seg ++)
		for (si = 0; si < SamplesPerSegment; si++)
			putc(sample(disk->waveDataA[seg].data, si) >> 4, stdout);
	for (seg = 0; seg < 18; seg ++)
		for (si = 0; si < SamplesPerSegment; si++)
			putc(sample(disk->waveDataB[seg].data, si) >> 4, stdout);
}

Disk	disk;

main(int argc, char ** argv)
{
	int	i;
	FILE *	fin;

	if (argc <= 1)
	{	printf("usage: %s Roland-12-bit-sample-disk-file\n", argv[0]);
		exit(1);
	}
	i = 1;
	if (i < argc)
	{	if ((fin = fopen(argv[i++], "r")) != NULL)
		{	fread(&disk, sizeof(Disk), 1, fin);
			convertDisk(&disk);
			fclose(fin);
		}
		else	printf("%s: open failed for \"%s\"\n", argv[0], argv[i]);
	}
	exit(0);
}
