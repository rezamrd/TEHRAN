

/*
Student Names: Parsa Derakhshan,Shelby Gauthier
Student Number: 040978675, 040994144
Assignment Number: Assignment 1.1
Course Name and Number: 21S_CST8152_010 Compilers
Lab Section Number: 011
Professor's Name: Paulo Sousa
Due Date of assignment:12/11/2021
Submission Date:12/11/2021
*/




/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2021
*************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2019
* Author: Paulo Sousa (based on prof. Svillen Ranev work)
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A3.
* Date: Dec 11 2021
* Purpose: This file is the main header for Parser (.h)
*************************************************************/





#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"  
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */

/* TO_DO: Adjust all datatypes to your language */
static Token lookahead;
thrn_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern thrn_int line;
extern Token tokenizer();
extern thrn_chr* keywordTable[];

/* TO_DO: Adjust your language name here */
#define STR_LANGNAME "Tehran"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	mainFunction,
	DT,
	CS,
	thrn_String,
	thrn_Int,
	thrn_Float,
	If,
	Elif,
	Else,
	Wloop,
	Dwloop,
	Floop,
	printout,
	input
};

/* Function definitions */
thrn_nul startParser();

thrn_nul matchToken(thrn_int, thrn_int);

thrn_nul syncErrorHandler(thrn_int);

thrn_nul printError();
/* TO_DO: Place ALL non-terminal function declarations */
thrn_nul startParser();

thrn_nul matchToken(thrn_int tokenCode, thrn_int tokenAttribute);

thrn_nul syncErrorHandler(thrn_int syncTokenCode);

thrn_nul printError();

thrn_nul program();

thrn_nul dataSession();

thrn_nul optVarListDeclarations();

thrn_nul codeSession();

thrn_nul optionalStatements();

thrn_nul statements();
thrn_nul statementsPrime();

thrn_nul statement();

thrn_nul outputStatement();

thrn_nul outputVariableList();
thrn_nul inputStatement();

thrn_nul conditionalExpression();

thrn_nul selectionStatement();

thrn_nul relationalStringExpression();

thrn_nul relationalExpression();

thrn_nul relationalArithmeticExpression();

thrn_nul logicalOrExpressionPrime();

thrn_nul logicalOrExpression();

thrn_nul logicalNotExpression();

thrn_nul logicalAndExpressionPrime();

thrn_nul logicalAndExpression();

thrn_nul iterationStatement();

thrn_nul primaryStringExpression();

thrn_nul stringExpressionPrime();

thrn_nul stringExpression();

thrn_nul additiveArithmeticExpressionPrime();

thrn_nul additiveArithmeticExpression();

thrn_nul multiplicativeArithmeticExpressionPrime();

thrn_nul multiplicativeArithmeticExpression();

thrn_nul arithmeticExpression();

thrn_nul primaryRelationalStringExpression();

thrn_nul primaryRelationalArithmeticExpression();

thrn_nul assignmentStatement();

thrn_nul assignmentExpression();

thrn_nul primaryArithmeticExpression();

thrn_nul varListDeclarationsPrime();

thrn_nul varListDeclarations();

thrn_nul varListDeclaration();

thrn_nul variableListPrime();

thrn_nul variableList();

thrn_nul variableIdentifier();

thrn_nul stringVariableIdentifier();

thrn_nul floatVariableIdentifier();

thrn_nul intVariableIdentifier();

thrn_nul stringVarListDeclaration();

thrn_nul intVarListDeclaration();

thrn_nul floatVarListDeclaration();

thrn_nul intVarListPrime();

thrn_nul floatVarListPrime();

thrn_nul stringVarListPrime();

thrn_nul intVarList();

thrn_nul floatVarList();

thrn_nul stringVarList();

#endif