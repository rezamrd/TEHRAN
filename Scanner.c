/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Student Name / ID: Mehdi Golipoor / 041021560
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A22, A32.
* Date: Feb 21 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
thrn_int line;								/* Current line number of the source code */
extern thrn_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern thrn_int stateType[];
extern thrn_chr* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern thrn_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

thrn_int startScanner(BufferPointer psc_buf) {
	/* in case the buffer has been read previously  */
	bufRecover(psc_buf);
	bufClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	thrn_chr c;	/* input symbol */
	thrn_int state = 0;		/* initial state of the FSM */
	thrn_int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	thrn_int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/
	thrn_int lexLength;		/* token length */
	thrn_int i;				/* counter */
	thrn_chr newc;			/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bufGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO 15: All patterns that do not require accepting functions */
		switch (c) {

			/* Cases for spaces */
		case ' ':
			break;
		case '\n':
			line++;
			break;
			/* TODO_15A: See all other elements */
		case '=':
			currentToken.code = EQ_T;
			return currentToken;
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
		case ',':
			currentToken.code = COM_T;
			return currentToken;
		case '_':
			currentToken.code = US_T;
			return currentToken;
		case '}':
			currentToken.code = RB_T;
			return currentToken;
		case '{':
			currentToken.code = LB_T;
			return currentToken;
		case ')':
			currentToken.code = RP_T;
			return currentToken;

		case '(':
			currentToken.code = LP_T;
			return currentToken;

		case '+':
			newc = bufGetChar(sourceBuffer);
			if (newc == '+') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bufRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_ADD;
			return currentToken;
		case '-':
			newc = bufGetChar(sourceBuffer);
			if (newc == '-') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bufRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_SUB;
			return currentToken;
		case '%':
			newc = bufGetChar(sourceBuffer);
			if (newc == '%') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bufRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_DIV;
			return currentToken;
		case '*':
			newc = bufGetChar(sourceBuffer);
			if (newc == '*') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bufRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_MUL;
			return currentToken;
		case '>':
			newc = bufGetChar(sourceBuffer);
			if (newc == '>') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bufRetract(sourceBuffer);
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_GT;
			return currentToken;
		case '<':
			newc = bufGetChar(sourceBuffer);
			if (newc == '<') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bufRetract(sourceBuffer);
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_LT;
			return currentToken;

			/* TODO_15B: See all other elements */

		case '.':
			/* AND operator */
			bufSetMark(sourceBuffer, bufGetPosRead(sourceBuffer));
			if (bufGetChar(sourceBuffer) == '&' &&
				bufGetChar(sourceBuffer) == '.') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = AND;
				return currentToken;
			}
			else if (bufGetChar(sourceBuffer) == '|' ||
				bufGetChar(sourceBuffer) == '.') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OR;
				return currentToken;
			}
			else
				bufRestore(sourceBuffer);

			/* TODO_15C: Other logical operators */

		/* Comments */
		case '#':
			newc = bufGetChar(sourceBuffer);
			do {
				c = bufGetChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					bufRetract(sourceBuffer);
					return currentToken;
				}
				else if (c == '\n') {
					line++;
				}
			} while (c != '#' && c != CHARSEOF0 && c != CHARSEOF255);
			break;

			/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_EOF;
			return currentToken;

			/* ------------------------------------------------------------------------
				Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
				Note: Part 2 must follow Part 1 to catch the illegal symbols
				-----------------------------------------------------------------------
			*/

			/* TO_DO_16: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = bufGetPosRead(sourceBuffer) - 1;
			bufSetMark(sourceBuffer, lexStart);
			while (stateType[state] == NOAS) {
				c = bufGetChar(sourceBuffer);
				state = nextState(state, c);
			}
			if (stateType[state] == ASWR)
				bufRetract(sourceBuffer);
			lexEnd = bufGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = bufCreate((short)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not Create buffer\n");
				exit(1);
			}
			bufRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				bufAddChar(lexemeBuffer, bufGetChar(sourceBuffer));
			bufAddChar(lexemeBuffer, BUFFER_EOF);
			currentToken = (*finalStateTable[state])(bufGetContent(lexemeBuffer, 0));
			bufDestroy(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

thrn_int nextState(thrn_int state, thrn_chr c) {
	thrn_int col;
	thrn_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
 /* TO_DO: Use your column configuration */

 /* Adjust the logic to return next column in TT */
 /*	[A-z](0), [0-9](1),	_(2), &(3), "(4), SEOF(5), other(6) */

thrn_int nextClass(thrn_chr c) {
	thrn_int val = -1;
	switch (c) {
	case CHRCOL2:
		val = 8;
		break;
	case CHRCOL4:
		val = 4;
		break;
	case CHRCOL5:
		val = 7;
		break;
	case CHRCOL7:
		val = 2;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 10;
		break;
	case '%':
		val = 0;
		break;
	case '\n':
		val = 6;
		break;
	case '$':
		val = 1;
		break;
	default:
		if (isalpha(c))
			val = 5;
		else if (isdigit(c))
			val = 3;
		else
			val = 9;
	} //switch
	return val;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(thrn_chr lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	char lastch = lexeme[length - 1];
	int isID = TEHRAN_FALSE;
	switch (lastch) {
	case IRDPREFIX:
		currentToken.code = IRD_T;
		break;
	case FTDPREFIX:
		currentToken.code = FTD_T;
		break;
	case SGDPREFIX:
		currentToken.code = SGD_T;
		break;
	default:
		// Test Keyword
		currentToken.code = ERR_T;
		break;
	}
	if (isID == TEHRAN_TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */

Token funcSL(thrn_chr lexeme[]) {
	Token currentToken = { 0 };
	thrn_int i = 0, len = (thrn_int)strlen(lexeme);
	currentToken.attribute.contentString = bufGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!bufAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!bufAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = SL;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(thrn_chr lexeme[]) {
	Token currentToken = { 0 };
	thrn_int kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcID(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(thrn_chr lexeme[]) {
	Token currentToken = { 0 };
	thrn_int i = 0, len = (thrn_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

thrn_nul printToken(Token t) {
	extern char* keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case IRD_T:
		printf("IRD_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case FTD_T:
		printf("FTD_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case SGD_T:
		printf("SGD_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case FL_T:
		printf("FL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case IL_T:
		printf("IL_T\t\t%d\n", t.attribute.codeType);
		break;
	case SL:
		printf("SL\t\t%d\t ", (short)t.attribute.codeType);
		printf("%s\n", bufGetContent(stringLiteralTable, (short)t.attribute.codeType));
		break;
	case SCC_OP_T:
		printf("SCC_OP_T\n");
		break;
	case ASS_OP_T:
		printf("ASS_OP_T\n");
		break;
	case ART_OP_T:
		printf("ART_OP_T\t%d\n", t.attribute.codeType);
		break;
	case REL_OP_T:
		printf("REL_OP_T\t%d\n", t.attribute.codeType);
		break;
	case LOG_OP_T:
		printf("LOG_OP_T\t%d\n", t.attribute.codeType);
		break;
	case LP_T:
		printf("LP_T\n");
		break;
	case RP_T:
		printf("RP_T\n");
		break;
	case LB_T:
		printf("LB_T\n");
		break;
	case RB_T:
		printf("RB_T\n");
		break;
	case US_T:
		printf("US_T\n");
		break;
	case EQ_T:
		printf("EQ_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/



Token funcFL(thrn_chr lexeme[]) {
	Token currentToken = { 0 };
	thrn_dbl tdouble = atof(lexeme);
	if (tdouble == 0.0 || tdouble >= FLT_MIN && tdouble <= FLT_MAX) {
		currentToken.code = FL_T;
		currentToken.attribute.floatValue = (thrn_flt)tdouble;
	}
	else {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	return currentToken;
}


Token funcIL(thrn_chr lexeme[]) {
	Token currentToken = { 0 };
	thrn_lng tlong;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = IL_T;
			currentToken.attribute.intValue = (thrn_int)tlong;
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}
	return currentToken;
}