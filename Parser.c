
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
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / Daniel Cormier
*************************************************************/

/*************************************************************
* File name: Parser.c
* Compiler: MS Visual Studio 2019
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: Sep 01 2021
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
*************************************************************/



#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* TO_DO: For all functions adjust the datatypes used in your language */

/*************************************************************
 * Process Parser
 ************************************************************/
 /* TO_DO: This is the function to start the parser - check your program definition */

thrn_nul startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*************************************************************
 * Match Token
 ************************************************************/
 /* TO_DO: This is the main code for match - check your definitions
 **********************************CHANGED HERE****************************
 */

thrn_nul matchToken(thrn_int tokenCode, thrn_int tokenAttribute) {
	thrn_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	case REL_OP_T:
	case ART_OP_T:
	case LOG_OP_T:
		// TO_DO: Include the other cases
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*************************************************************
 * Syncronize Error Handler
 ************************************************************/
 /* TO_DO: This is the function to handler error - adjust basically datatypes */

thrn_nul syncErrorHandler(thrn_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*************************************************************
 * Print Error
 ************************************************************/
 /* TO_DO: This is the function to error printing - adjust basically datatypes */

thrn_nul printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case IRD_T:
	case FTD_T:
	case SGD_T:
		printf("%s\n", t.attribute.vidLexeme);
		break;
	case STR_T:
		printf("%s\n", getContent(stringLiteralTable, t.attribute.contentString));
		break;

	case FL_T:
		printf("%5.1f\n", t.attribute.floatValue);
		break;
	case IL_T:
	case ART_OP_T:
	case REL_OP_T:
	case LOG_OP_T:
		printf("%d\n", t.attribute.codeType);
		break;
	case SCC_OP_T:
	case ASS_OP_T:
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case COM_T:
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*************************************************************
 * Program statement
 * BNF: <program> -> MAIN { <opt_statements> }
 * FIRST(<program>)= { MNID_T }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul program() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == mainFunction) {
			matchToken(KW_T, mainFunction);
			matchToken(LB_T, NO_ATTR);
			dataSession();
			codeSession();
			matchToken(RB_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty: for optional
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*************************************************************
 * dataSession
 * BNF: <dataSession> -> DATA { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (DATA)}.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul dataSession() {
	matchToken(KW_T, DT);
	matchToken(LB_T, NO_ATTR);
	optVarListDeclarations();
	matchToken(RB_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (FLOAT) }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul optVarListDeclarations() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == thrn_Int || lookahead.attribute.codeType == thrn_Float || lookahead.attribute.codeType == thrn_String) {
			varListDeclarations();
			break;
		}
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*************************************************************
 * codeSession statement
 * BNF: <codeSession> -> CODE { <opt_statements> }
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */
thrn_nul codeSession() {
	matchToken(KW_T, CS);
	matchToken(LB_T, NO_ATTR);
	optionalStatements();
	matchToken(RB_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/*************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul optionalStatements() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == printout) {
			statements();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*************************************************************
 * Statements Prime
 * BNF: <statementsPrime>  <statement><statementsPrime> | ϵ
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul statementsPrime() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == printout) {
			statement();
			statementsPrime();
			break;
		}
	default:
		; //empty string
	}
}

/*************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <input statement> | <output statement>
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case printout:
			outputStatement();
			break;
		default:
			printError();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*************************************************************
 * Output Statement
 * BNF: <output statement> -> WRITE (<output statementPrime>);
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul outputStatement() {
	matchToken(KW_T, printout);
	matchToken(LP_T, NO_ATTR);
	outputVariableList();
	matchToken(RP_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

thrn_nul outputVariableList() {
	switch (lookahead.code) {
	case SGD_T:
		matchToken(SGD_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/* TO_DO: Continue the development (all non-terminal functions) */
thrn_nul inputStatement() {
	switch (lookahead.code) {
	case KW_T:
		matchToken(input, NO_ATTR);
		break;
	default:
		printError();
	}
}



thrn_nul conditionalExpression() {
	switch (lookahead.code) {
	case IRD_T:
		matchToken(IRD_T, NO_ATTR);
		break;
	case FTD_T:
		matchToken(FTD_T, NO_ATTR);
		break;
	case IL_T:
		matchToken(IL_T, NO_ATTR);
		break;
	case SGD_T:
		matchToken(SGD_T, NO_ATTR);
		break;
	case SL_T:
		matchToken(SL_T, NO_ATTR);
		break;
	default:
		printError();
	}
}


thrn_nul selectionStatement() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == If
			|| lookahead.attribute.codeType == Elif
			|| lookahead.attribute.codeType == Else) {
			conditionalExpression();
			break;
		}
	default:
		;
	}
}


/*---------------------------------------------------------------------*/

thrn_nul relationalStringExpression() {
	primaryStringExpression();
	stringExpressionPrime();
}


thrn_nul relationalExpression() {
	switch (lookahead.code) {
	case IRD_T:
	case FTD_T:
	case SGD_T:
	case SL_T:
	case IL_T:
		break;
	default:
		printError();
	}

}


thrn_nul relationalArithmeticExpression() {
	switch (lookahead.code) {
	case SCC_OP_T:
		matchToken(SCC_OP_T, OP_GT);
		matchToken(SCC_OP_T, OP_LT);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": relational Arithmetic Expression parsed");
}


thrn_nul logicalOrExpressionPrime() {
	switch (lookahead.code) {
	case LOG_OP_T:
		switch (lookahead.attribute.codeType) {
		case '.|.':
			matchToken(LOG_OP_T, OR);
			break;
		default:
			;
		}
	}
}


thrn_nul logicalOrExpression() {
	switch (lookahead.code) {
	case LOG_OP_T:
		matchToken(LOG_OP_T, OR);
		logicalOrExpressionPrime();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": logical Or Expression parsed");
}

thrn_nul logicalNotExpression() {
	switch (lookahead.code) {
	case LOG_OP_T:
		matchToken(LOG_OP_T, NOT);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": logical Not Expression parsed");
}


thrn_nul logicalAndExpressionPrime() {
	switch (lookahead.code) {
	case LOG_OP_T:
		switch (lookahead.attribute.codeType) {
		case '.&.':
			matchToken(LOG_OP_T, AND);
			break;
		default:
			;
		}
	}
}

thrn_nul logicalAndExpression() {
	switch (lookahead.code) {
	case LOG_OP_T:
		matchToken(LOG_OP_T, AND);
		logicalAndExpressionPrime();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": logical And Expression parsed");
}

thrn_nul iterationStatement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case Wloop:
		case Dwloop:
		case Floop:
			selectionStatement();
			break;
		default:
			printError();
		}
	}

}

thrn_nul primaryStringExpression() {
	switch (lookahead.code) {
	case SGD_T:
		matchToken(SGD_T, NO_ATTR);
		break;
	case SL_T:
		matchToken(SL_T, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": primary string expression parsed");
}


thrn_nul stringExpressionPrime() {
	switch (lookahead.code) {
	case SGD_T:
		matchToken(SGD_T, NO_ATTR);
		break;
	case SL_T:
		matchToken(SL_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": String expression prime parsed");
}

thrn_nul stringExpression() {
	primaryStringExpression();
	stringExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": String expression parsed");
}

thrn_nul additiveArithmeticExpressionPrime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
			matchToken(ART_OP_T, OP_ADD);
			break;
		case OP_SUB:
			matchToken(ART_OP_T, OP_SUB);
			break;
		default:
			;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": additive Arithmetic Expression Prime parsed");
}

thrn_nul additiveArithmeticExpression() {
	multiplicativeArithmeticExpression();
	additiveArithmeticExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}

thrn_nul multiplicativeArithmeticExpressionPrime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_MUL:
			matchToken(ART_OP_T, OP_MUL);
			break;
		case OP_DIV:
			matchToken(ART_OP_T, OP_DIV);
			break;
		default:
			;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": multiplicative Arithmetic Expression Prime parsed");
}

thrn_nul multiplicativeArithmeticExpression() {
	switch (lookahead.code) {
	case LPR_T:
	case IRD_T:
	case FTD_T:
	case FL_T:
	case IL_T:
		primaryArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ":  multiplicative Arithmetic Expression parsed");
}

thrn_nul arithmeticExpression() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
			matchToken(ART_OP_T, OP_ADD);
			break;
		case OP_SUB:
			matchToken(ART_OP_T, OP_SUB);
			break;
		default:
			printError();
		}
		primaryArithmeticExpression();
		break;
	case LP_T:
	case IRD_T:
	case FTD_T:
	case FL_T:
	case IL_T:
		additiveArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

thrn_nul primaryRelationalStringExpression() {
	switch (lookahead.code) {
	case IRD_T:
	case FTD_T:
	case SGD_T:
	case FL_T:
	case IL_T:
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ":  primary Relational String Expression parsed");
}

thrn_nul primaryRelationalArithmeticExpression() {
	switch (lookahead.code) {
	case FL_T:
	case IL_T:
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ":  primary Relational Arithmetic Expression parsed");
}

thrn_nul assignmentStatement() {
	assignmentExpression();
}

thrn_nul assignmentExpression() {
	switch (lookahead.code) {
	case IRD_T:
	case FTD_T:
	case SGD_T:
		arithmeticExpression();
		stringExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": assignment expression parsed");
}

thrn_nul primaryArithmeticExpression() {
	switch (lookahead.code) {
	case IL_T:
	case FL_T:
		arithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": primary arithmetic expression parsed");
}


thrn_nul varListDeclarationsPrime() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case thrn_Int:
		case thrn_Float:
		case thrn_String:
			varListDeclaration();
			varListDeclarationsPrime();
			break;
		default:
			printError();
		}
		break;
	default:
		; //empty string
	}
}

thrn_nul varListDeclarations() {
	varListDeclaration();
	varListDeclarationsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable Lists Declarations parsed");
}
thrn_nul varListDeclaration() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case thrn_Int:
			intVarListDeclaration();
			break;
		case thrn_Float:
			floatVarListDeclaration();
			break;
		case thrn_String:
			stringVarListDeclaration();
			break;
		default:
			printError();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Variable List Declaration parsed");
}

thrn_nul variableListPrime() {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		variableList();
		break;
	default:
		;
	}
}

thrn_nul variableList() {
	matchToken(KW_T, thrn_Int);
	matchToken(KW_T, thrn_Float);
	matchToken(KW_T, thrn_String);
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": Variable Lists parsed");
}

thrn_nul variableIdentifier() {
	switch (lookahead.code) {
	case IRD_T:
		matchToken(IRD_T, NO_ATTR);
		break;
	case FTD_T:
		matchToken(FTD_T, NO_ATTR);
		break;
	case SGD_T:
		matchToken(SGD_T, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": variable identifier parsed");
}

thrn_nul stringVariableIdentifier() {
	switch (lookahead.code) {
	case SGD_T:
		matchToken(SGD_T, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": String variable identifier parsed");
}

thrn_nul floatVariableIdentifier() {
	switch (lookahead.code) {
	case FTD_T:
		matchToken(FTD_T, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Float variable identifier parsed");
}

thrn_nul intVariableIdentifier() {
	switch (lookahead.code) {
	case IRD_T:
		matchToken(IRD_T, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Integer variable identifier parsed");
}

thrn_nul stringVarListDeclaration() {
	matchToken(KW_T, thrn_String);
	stringVarList();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": String var list declaration parsed");
}

thrn_nul intVarListDeclaration() {
	matchToken(KW_T, thrn_Int);
	intVarList();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Integer var list declaration parsed");
}

thrn_nul floatVarListDeclaration() {
	matchToken(KW_T, thrn_Float);
	floatVarList();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Float var list declaration parsed");
}

thrn_nul intVarListPrime() {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		intVariableIdentifier();
		intVarListPrime();
		break;
	default:
		;
	}
}

thrn_nul floatVarListPrime() {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		floatVariableIdentifier();
		floatVarListPrime();
		break;
	default:
		;
	}
}

thrn_nul stringVarListPrime() {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		stringVariableIdentifier();
		stringVarListPrime();
		break;
	default:
		;
	}
}

thrn_nul intVarList() {
	intVariableIdentifier();
	intVarListPrime();
	printf("%s%s\n", STR_LANGNAME, ": Integer var list parsed");
}

thrn_nul floatVarList() {
	floatVariableIdentifier();
	floatVarListPrime();
	printf("%s%s\n", STR_LANGNAME, ": Float var list parsed");
}

thrn_nul stringVarList() {
	stringVariableIdentifier();
	stringVarListPrime();
	printf("%s%s\n", STR_LANGNAME, ": String var list parsed");
}
