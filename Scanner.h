/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 - Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Jul 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	IL_T,		/*  1: Integer literal token */
	FL_T,		/*  2: Float literal token */
	SL,			/*  3: String literal token */
	IRD_T,		/*  4: Integer delimitat=u*/
	FTD_T,		/*  5: Float delimitator */
	SGD_T,		/*  6: String literal token */
	SLC_T,		/*  7: Single line comment token*/
	MLC_T,		/*  8: Multi lines comentt token*/
	LP_T,		/*  9: Left parenthesis token */
	RP_T,		/* 10: Right parenthesis token */
	LB_T,		/* 11: Left brace token */
	RB_T,		/* 12: Right brace token */
	KW_T,		/* 13: Keyword token */
	COM_T,		/* Comma token */
	US_T,        /* Under score token*/
	EQ_T,        /*equal token*/
	LOG_OP_T,	/* Logical operator token */
	SCC_OP_T,	/* String concatenation operator token: (++) */
	ASS_OP_T,	/* Assignment operator token */
	ART_OP_T,	/* Arithmetic operator token */
	REL_OP_T,	/* Relational operator token */
	EOS_T,		/* 14: End of statement (semicolon) */
	RTE_T,		/* 15: Run-time error token */
	SEOF_T		/* 16: End-of-file token */
};

/* CHECK: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { AND, OR, NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_EOF } EofOperator;

/* TO_DO 3A: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	thrn_int codeType;						/* integer attributes accessor */
	AriOperator arithmeticOperator;			/* arithmetic operator attribute code */
	RelOperator relationalOperator;			/* relational operator attribute code */
	LogOperator logicalOperator;			/* logical operator attribute code */
	EofOperator seofType;					/* source-end-of-file attribute code */
	thrn_int intValue;						/* integer literal attribute (value) */
	thrn_int keywordIndex;					/* keyword index in the keyword table */
	thrn_int contentString;					/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	thrn_flt floatValue;					/* floating-point literal attribute (value) */
	thrn_chr idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	thrn_chr errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO 3B: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	thrn_flag flags;			/* Flags information */
	union {
		thrn_int intValue;				/* Integer value */
		thrn_flt floatValue;			/* Float value */
		thrn_nul* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	thrn_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .&., .|. , .!. , SEOF.
 */

 /* TO_DO 4: Error states and illegal state */
#define ES	16		/* Error state  with no retract */
#define ER	17		/* Error state  with retract */
#define IS	-1		/* Illegal state */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 11

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '&'
#define CHRCOL4 '\''
#define CHRCOL5 '.'
#define CHRCOL6 '~'
#define CHRCOL7 '@'

/* These constants will be used on VID function */
#define IRDPREFIX '@'
#define FTDPREFIX '%'
#define SGDPREFIX '$'

/* TO_DO: Transition table - type of states defined in separate table */
static thrn_int transitionTable[][TABLE_COLUMNS] = {
	/*			%,		$,		@,		[0-9],	',		[A-z],	\n,		.,		_,		other,	EOF		*/
	/*			F(0),	S(1),	I(2),	D(3),	Q(4),	L(5),	N(6),	P(7)	U(8)	O(9)	E(10)	*/
	/* S00 */	{1,		3,		5,		7,		12,		14,		ES,		ES,		ES,		ES,		ER},	/* NOAS [0] */
	/* S01 */	{2,		2,		2,		1,		2,		1,		2,		2,		1,		2,		ER},	/* NOAS [1] */
	/* S02 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (FTD) [2] */
	/* S03 */	{4,		4,		4,		3,		4,		3,		4,		4,		3,		4,		ER},	/* NOAS [3] */
	/* S04 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASNR (SGD) [4] */
	/* S05 */	{6,		6,		6,		5,		6,		5,		6,		6,		5,		6,		ER},	/* NOAS [5] */
	/* S06 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (IRD) [6] */
	/* S07 */	{8,		8,		8,		7,		8,		8,		8,		9,		8,		8,		ER},	/* NOAS [7] */
	/* S08 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (IL) [8] */
	/* S09 */	{ES,	ES,		ES,		10,		ES,		ES,		ES,		ES,		ES,		ES,		ER},	/* NOAS [9] */
	/* S10 */	{11,	11,		11,		10,		11,		11,		11,		11,		11,		11,		ER},	/* NOAS [10] */
	/* S11 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (FL) [11] */
	/* S12 */	{12,	12,		12,		12,		13,		12,		12,		12,		12,		12,		ER},	/* NOAS [12] */
	/* S13 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASNR (SL) [13] */
	/* S14 */	{15,	15,		15,		15,		15,		14,		15,		15,		15,		15,		ER},	/* NOAS [14] */
	/* S15 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (KEY) [15] */
	/* s16 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS},	/* ASNR (ES) [16] */
	/* s17 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS,		IS}		/* ASWR (ER) [17] */
};

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static thrn_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASWR, /* 02 (FTD) */
	NOAS, /* 03 */
	ASWR, /* 04 (SGD) */
	NOAS, /* 05 */
	ASWR, /* 06 (IRD) */
	NOAS, /* 07 */
	ASWR, /* 08 (IL) */
	NOAS, /* 09 */
	NOAS, /* 10 */
	ASWR, /* 11 (FL) */
	NOAS, /* 12 */
	ASNR, /* 13 (SL) */
	NOAS, /* 14 */
	ASWR, /* 15 (KEY) */
	ASWR, /* 16 (Err1 - no retract) */
	ASWR, /* 16 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
thrn_int startScanner(BufferPointer psc_buf);
static thrn_int nextClass(thrn_chr c);			/* character class function */
static thrn_int nextState(thrn_int, thrn_chr);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(thrn_chr* lexeme);

/* Declare accepting states functions */
Token funcID(thrn_chr* lexeme);	//valid delimitator
Token funcIL(thrn_chr* lexeme);	//Integer delimitator
Token funcFL(thrn_chr* lexeme);	//Float delimitator
Token funcSL(thrn_chr* lexeme);	//String delimitator
Token funcKEY(thrn_chr* lexeme);
Token funcErr(thrn_chr* lexeme);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* 00 */
	NULL,		/* 01 */
	funcID,		/* 02 */
	NULL,		/* 03 */
	funcID,		/* 04 */
	NULL,		/* 05 */
	funcID,		/* 06 */
	NULL,		/* 07 */
	funcIL,		/* 08 */
	NULL,		/* 09 */
	NULL,		/* 10 */
	funcFL,		/* 11 */
	NULL,		/* 12 */
	funcSL,		/* 13 */
	NULL,		/* 14 */
	funcKEY,	/* 15 */
	funcErr,	/* 16 */
	funcErr		/* 17 */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 10

/* TO_DO: Define the list of keywords */
static thrn_chr* keywordTable[KWT_SIZE] = {
	"float",
	"int",
	"String",
	"if",
	"elseif",
	"else",
	"for",
	"while",
	"dowhile",
	"print"
};

#endif