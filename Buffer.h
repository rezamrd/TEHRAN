/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: Buffer.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 - Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Buffer (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define BUFFER_ERROR (-1)					/* General error message */
#define BUFFER_EOF '\0'						/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (TEHRAN) .................................. */
#define BUFFER_DEFAULT_SIZE			100		/* default initial buffer capacity */
#define BUFFER_DEFAULT_INCREMENT	10		/* default increment factor */


#define TEHRAN_MAX_SIZE INT_MAX-1		/*maximum capacity*/ 
/* Add your bit-masks constant definitions here - Defined for TEHRAN */
/* BITS                                (7654.3210) */
#define TEHRAN_DEFAULT_FLAG 0xEB 	    
#define TEHRAN_SET_FLAG_RLC 0x08		
#define TEHRAN_RST_FLAG_RLC 0xF7		
#define TEHRAN_CHK_FLAG_RLC 0x08		
#define TEHRAN_SET_FLAG_EOB 0x04		
#define TEHRAN_RST_FLAG_EOB 0xFB		
#define TEHRAN_CHK_FLAG_EOB 0x04

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (TEHRAN) .................................. */

/* TO_DO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	thrn_int posMark;			/* the offset to the mark position (in chars) */
	thrn_int posRead;			/* the offset to the get a char position (in chars) */
	thrn_int posWrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct buffer {
	thrn_chr* content;		/* pointer to the beginning of character array (character buffer) */
	thrn_int	size;		    /* current dynamic memory size (in bytes) allocated to character buffer */
	thrn_int	increment;		/* character array increment factor */
	thrn_int	mode;			/* operational mode indicator */
	thrn_flag	flags;			/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;		/* Offset / position field */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	bufCreate(thrn_int, thrn_int, thrn_int);
BufferPointer	bufAddChar(BufferPointer const, thrn_chr);
thrn_bol		bufClear(BufferPointer const);
thrn_bol		bufDestroy(BufferPointer const);
thrn_bol		bufChkFull(BufferPointer const);
thrn_bol		bufChkEmpty(BufferPointer const);
thrn_bol		bufSetMark(BufferPointer const, thrn_int);
thrn_int		bufPrint(BufferPointer const);
thrn_int		bufLoad(BufferPointer const, FILE* const);
thrn_bol		bufRecover(BufferPointer const);
thrn_bol		bufRetract(BufferPointer const);
thrn_bol		bufRestore(BufferPointer const);
/* Getters */
thrn_chr		bufGetChar(BufferPointer const);
thrn_chr*		bufGetContent(BufferPointer const, thrn_int);
thrn_int		bufGetPosRead(BufferPointer const);
thrn_int		bufGetPosWrte(BufferPointer const);
thrn_int		bufGetPosMark(BufferPointer const);
thrn_int		bufGetSize(BufferPointer const);
thrn_int		bufGetInc(BufferPointer const);
thrn_int		bufGetMode(BufferPointer const);
thrn_flag		bufGetFlags(BufferPointer const);
#endif