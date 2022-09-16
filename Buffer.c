/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
***********************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 - Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer (A12)
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
#include "Buffer.h"
#endif

 /*
 ***********************************************************
 * Function name: bCreate
 * Purpose: Creates the buffer according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: Ver 2.1
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to Buffer)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TO_DO:
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */
BufferPointer bufCreate(thrn_int size, thrn_int increment, thrn_int mode) {
	BufferPointer b;
	/* TODO: Defensive programming: check validity of parameters */
	if (size<0 || size>TEHRAN_MAX_SIZE)
		return NULL;
	if (!size) {
		size = BUFFER_DEFAULT_SIZE;
		increment = BUFFER_DEFAULT_INCREMENT;
	}
	/* TODO: When there is no increment, mode: fixed */
	b = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!b)
		return NULL;
	//if(b)
	b->content = (thrn_chr*)malloc(size);
	if (!b->content)
		return NULL;
	if (!increment)
		mode = MODE_FIXED;
	/* TO_DO: Adjust properties */
	if (b == NULL)
		return NULL;
	if (!b->content) {
		free(b);
		return NULL;
	}
	b->mode = mode;
	b->size = size;
	b->increment = increment;
	b->flags = TEHRAN_DEFAULT_FLAG | TEHRAN_SET_FLAG_EOB; //0000.0001
	return b;
}


/*
***********************************************************
* Function name: bufAddChar
* Purpose: Adds a char to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = char to be added
* Return value:
*	bPointer (pointer to Buffer)
* TODO ......................................................
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BufferPointer bufAddChar(BufferPointer const pBuffer, thrn_chr ch) {
	thrn_chr* temp = NULL;
	thrn_int newSize = 0;
	/* TODO: Defensive programming */
	/* TODO: Reset Realocation */
	/* TODO: Check if it is possible to be increased */
	/* TODO: Check if buffer is full */
	if (pBuffer->position.posWrte * (thrn_int)sizeof(thrn_chr) >= pBuffer->size) {
		switch (pBuffer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			/* TODO: Adjust new size for Additive increment */
			/* TODO: Test with defensive programming */
			newSize = pBuffer->size + pBuffer->increment;
			if (newSize < 0 || newSize <= pBuffer->size)
				return NULL;
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		case MODE_MULTI:
			/* TODO: Adjust new size for Additive increment */
			/* TODO: Test with defensive programming */
			newSize = pBuffer->size * pBuffer->increment;
			if (newSize < 0 || newSize <= pBuffer->size)
				return NULL;
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		default:
			return NULL;
		}
		/* TODO: Realloc the size for new buffer */
		/* TODO: Check the realocation by Defensive programming */
		/* TODO: If allowed, adjust the new buffer content */
		temp = (char*)realloc(pBuffer->content, newSize);
		if (!temp)
			return NULL;
		if (temp != pBuffer->content) {
			pBuffer->flags = pBuffer->flags | TEHRAN_SET_FLAG_RLC;
		}
			pBuffer->content = temp;
			pBuffer->size = newSize;
	}
	pBuffer->content[pBuffer->position.posWrte++] = ch;
	return pBuffer;
}

/*
***********************************************************
* Function name: bufClear
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufClear(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Clear the buffer reseting all positions and flags */
	if (!pBuffer)
		return TEHRAN_FALSE;
	pBuffer->position.posWrte = pBuffer->position.posMark = pBuffer->position.posRead = 0;
	pBuffer->flags = pBuffer->flags & TEHRAN_RST_FLAG_EOB;
	pBuffer->flags = pBuffer->flags & TEHRAN_RST_FLAG_RLC;
	return TEHRAN_TRUE;
}

/*
***********************************************************
* Function name: bufDestroy
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufDestroy(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Free the buffer (content and struct) */
	if (!pBuffer)
		return TEHRAN_FALSE;
	free(pBuffer->content);
	free(pBuffer);
	return TEHRAN_TRUE;
}

/*
***********************************************************
* Function name: bufChkFull
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufChkFull(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Check flag if buffer is FUL and return correct value */
	if (!pBuffer)
		return TEHRAN_FALSE;
	if (pBuffer->size == pBuffer->position.posWrte * (int)sizeof(char))
		return TEHRAN_TRUE;
	return TEHRAN_TRUE;
}

/*
***********************************************************
* Function name: bufChkEmpty
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufChkEmpty(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Check if it is empty */
	if (!pBuffer)
		return TEHRAN_FALSE;
	if (pBuffer->position.posWrte == 0)
		return TEHRAN_TRUE;
	return TEHRAN_FALSE;
}

/*
***********************************************************
* Function name: bSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TODO ......................................................
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufSetMark(BufferPointer const pBuffer, thrn_int mark) {
	/* TODO: Defensive programming */
	/* TODO: Adjust mark position */
	if (!pBuffer || mark<0 || mark > pBuffer->position.posWrte)
		return TEHRAN_FALSE;
	pBuffer->position.posMark = mark;
	return TEHRAN_TRUE;
}

/*
***********************************************************
* Function name: bPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Number of chars printed.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufPrint(BufferPointer const pBuffer) {
	thrn_int cont = 0;
	thrn_chr c;
	/* TODO: Defensive programming */
	if (!pBuffer || !(pBuffer->content))
		return BUFFER_ERROR;
	c = bufGetChar(pBuffer);
	/* TODO: Print all chars */
	while (cont < pBuffer->position.posWrte) {
		cont++;
		printf("%c", c);
		c = bufGetChar(pBuffer);
	}
	return cont;
}

/*
***********************************************************
* Function name: bufLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TODO ......................................................
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufLoad(BufferPointer const pBuffer, FILE* const fi) {
	thrn_int size = 0;
	thrn_chr c;
	/* TODO: Defensive programming */
	if (!fi || !pBuffer)
		return BUFFER_ERROR;
	c = (thrn_chr)fgetc(fi);
	while (!feof(fi)) {
		if (!bufAddChar(pBuffer, c)) {
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		c = (char)fgetc(fi);
		size++;
	}
	/* TODO: Defensive programming */
	if (ferror(fi))
		return BUFFER_ERROR;
	return size;
}

/*
***********************************************************
* Function name: bufRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value
*	Boolean value about operation success
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufRecover(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Reinitialize read and mark positions */
	if (!pBuffer)
		return TEHRAN_FALSE;
	pBuffer->position.posRead = 0;
	pBuffer->position.posMark = 0;
	return TEHRAN_TRUE;
}

/*
***********************************************************
* Function name: bufRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TODO ......................................................
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufRetract(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Check boundary conditions */
	/* TODO: Retract (return 1 pos read) */
	if (!pBuffer || pBuffer->position.posRead == 0)
		return TEHRAN_FALSE;
	pBuffer->position.posRead--;
	return TEHRAN_TRUE;
}

/*
***********************************************************
* Function name: bufRestore
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_bol bufRestore(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Return read position to mark position */
	if (!pBuffer)
		return TEHRAN_FALSE;
	pBuffer->position.posRead = pBuffer->position.posMark;
	return TEHRAN_TRUE;
}

/*
***********************************************************
* Function name: bufGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_chr bufGetChar(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Adjust EOB if necessary */
	if (!pBuffer)
		return BUFFER_ERROR;
	if (pBuffer->position.posRead == pBuffer->position.posWrte) {
		pBuffer->flags = pBuffer->flags | TEHRAN_SET_FLAG_EOB;
		//return '\0';
	}
	else {
		pBuffer->flags = pBuffer->flags & TEHRAN_RST_FLAG_EOB;
	}
	
	return pBuffer->content[pBuffer->position.posRead++];
}

/*
***********************************************************
* Function name: bufGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_chr* bufGetContent(BufferPointer const pBuffer, thrn_int pos) {
	/* TODO: Defensive programming */
	/* TODO: Return the pointer to content given by pos */
	if (!pBuffer || pos < 0 || pos > pBuffer->position.posWrte)
		return NULL;
	return pBuffer->content + pos;
}

/*
***********************************************************
* Function name: bufGetReadPos
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The readPos offset.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufGetReadPos(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Return read position */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.posRead;
}

/*
***********************************************************
* Function name: bufGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	addcPosition value
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufGetPosWrte(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Return the expected (write position) */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.posWrte;
}

/*
***********************************************************
* Function name: bufGetMarkPos
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	mark offset.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufGetMarkPos(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Return mark position */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.posMark;
}

/*
***********************************************************
* Function name: bufGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Size of buffer.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufGetSize(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Return buffer size */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->size;
}

/*
***********************************************************
* Function name: bufGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The Buffer increment.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufGetInc(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Return increment */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->increment;
}

/*
***********************************************************
* Function name: bufGetMode
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	operational mode.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
thrn_int bufGetMode(BufferPointer const pBuffer) {
	/* TODO: Defensive programming */
	/* TODO: Return buffer mode */
	if (!pBuffer)
		return (char)BUFFER_ERROR;
	return pBuffer->mode;
}


/*
***********************************************************
* Function name: bufGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Flags from Buffer.
* TODO ......................................................
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_

/* TODO: Defensive programming */
/* TODO: Return flags */
thrn_flag bufGetFlags(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	return pBuffer->flags;
}
#else
#define bufGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif
