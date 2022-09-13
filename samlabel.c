/************************************************************************/
/* NAME									*/
/*	samlabel - print a Roland 12-bit-sample-disk label and catalog	*/
/* SYNOPSIS								*/
/*	samlabel roland-12-bit-sample-disk-file				*/
/* DESCRIPTION								*/
/*	samlabel prints the disk label and contents for a Roland	*/
/*	12-bit sample-disk.						*/
/*	The disk label layout is a bit strange.				*/
/* AUTHOR								*/
/*	Copyright 1992 by Gary J. Murakami <gjm@research.att.com>	*/
/*	V1.0 (4 May 1992) Freely distibutable for non-comercial use.	*/
/*	All other rights reserved.					*/
/************************************************************************/

#include <stdio.h>
#include "rs12.h"

void
printLabel(Disk * disk)
{
	int	i, j;
	byte *	s;

	s = disk->functionParameter.diskLabel;
	
	printf("----------------\n");
	printf("| %-12.12s |\n", s);
	s += 12;
	for (i = 0; i < 4; i++)
	{	putchar('|');
		putchar(' ');
		for (j = 0; j < 12; j++)
			putchar(*(s + j*4 + i));
		putchar(' ');
		putchar('|');
		putchar('\n');
	}
	printf("----------------\n");
	for (j = 0; j < 8; j++)
	{	for (i = 0; i < 2; i++)
			printf("%d%d %-12.12s ", i + 1, j + 1,
				disk->patchParameter[i*8+j].patchName);
		putchar('\n');
	}
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
			printLabel(&disk);
			fclose(fin);
		}
		else	printf("%s: open failed for \"%s\"\n", argv[0], argv[i]);
	}
	exit(0);
}
