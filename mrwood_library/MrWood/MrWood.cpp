/***************************************************************************
 *
 * AK-Mr.Wood-L
 * Library for Arduino V1.0b
 *
 * MrWood.cpp
 * Copyright (C) 2014 Artekit Italy.
 * http://www.artekit.eu
 *
 * This is an Arduino library file for the Artekit AK-Mr.Wood-L, 7-segment
 * plus shift register board.
 *
 * This file contains some helper functions to use the AK-Mr.Wood-L, based on
 * basic digitalWrite() and shiftOut() Arduino functions.
 * There is also a predefined ASCII table that can be used with the functions
 * here below.
 *
 * Product page: 		http://www.artekit.eu/products/accessories/ak-mr-wood-l/
 * Library blog post: 	http://www.artekit.eu/ak-mr-wood-l-library-for-arduino/
 * Connection guide:	http://www.artekit.eu/using-the-ak-mr-wood-l/
 *  

#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

***************************************************************************/
 
#include "MrWood.h"

/* These two macros order a byte according to MSB or LSB bit ordering.
 * Each bit corresponds to a shift register -> display pin.
 */
#define SEVSEG_LSB(a,b,c,d,e,f,g,dp)	((uint8_t) 	(dp | (g << 1) | (f << 2) | (e << 3) | \
													(d << 4) | (c << 5) | (b << 6) | (a << 7)))
									
#define SEVSEG_MSB(a,b,c,d,e,f,g,dp)	((uint8_t) 	(a | (b << 1) | (c << 2) | (d << 3) | \
													(e << 4) | (f << 5) | (g << 6) | (dp << 7)))

/* By default we use shiftOut() with MSB. If for any reason you want to use LSB bit ordering,
 * add here #define USE_LSB
 */
#ifdef USE_LSB
#define SEVSEG 	SEVSEG_LSB
#define RSEVSEG(dp,g,f,e,d,c,b,a) SEVSEG_LSB(a,b,c,d,e,f,g,dp)
#define DP_FLAG 0x01
#else
#define SEVSEG SEVSEG_MSB
#define RSEVSEG(dp,g,f,e,d,c,b,a) SEVSEG_MSB(a,b,c,d,e,f,g,dp)
#define DP_FLAG 0x80
#endif

/* Our table starts at ASCII SPACE */
#define MRWOOD_TABLE_OFFSET 		0x20
#define MRWOOD_TABLE_MAX_VALUE		sizeof(mrwood_ascii_table)

/* These constants point to the special M, m, W and w characters in our ASCII table. */
#define M_UC_LOW		0x7B	/* Uppercase M low part */
#define M_UC_HIGH		0x7C	/* Uppercase M high part */
#define M_LC_LOW		0x7D	/* Lowercase m low part */
#define M_LC_HIGH		0x7E	/* Lowercase m high part */
#define W_UC_LOW		0x7F	/* Uppercase W low part */
#define W_UC_HIGH		0x80	/* Uppercase W high part */
#define W_LC_LOW		0x81	/* Lowercase w low part */
#define W_LC_HIGH		0x82	/* Lowercase w high part */

/* The ASCII table. Note that there are non-printable (NP) characters and
 * those will be shown as white spaces.
 */
static const uint8_t mrwood_ascii_table[] =
{
	/* From 0x20 to 0x2F */
	/*     dp g f e d c b a					       dp g f e d c b a            */
	RSEVSEG(0,0,0,0,0,0,0,0), /* SPACE */	RSEVSEG(1,0,0,0,0,0,1,0), /*   !   */
	RSEVSEG(0,0,1,0,0,0,1,0), /*   "   */	RSEVSEG(0,0,0,0,0,0,0,0), /* # NP  */
	RSEVSEG(0,0,0,0,0,0,0,0), /* $ NP  */	RSEVSEG(0,0,0,0,0,0,0,0), /* % NP  */
	RSEVSEG(0,0,0,0,0,0,0,0), /* & NP  */	RSEVSEG(0,0,0,0,0,0,1,0), /*   '   */
	RSEVSEG(0,0,1,1,1,0,0,1), /*   (   */	RSEVSEG(0,0,0,0,1,1,1,1), /*   )   */
	RSEVSEG(0,0,0,0,0,0,0,0), /* * NP  */	RSEVSEG(0,0,0,0,0,0,0,0), /* + NP  */
	RSEVSEG(0,0,0,0,0,0,0,0), /* , NP  */	RSEVSEG(0,1,0,0,0,0,0,0), /*   -   */
	RSEVSEG(1,0,0,0,0,0,0,0), /*   .   */   RSEVSEG(0,0,0,0,0,0,0,0), /* / NP  */
	
	/* Numbers (from 0x30 to 0x39) */
	/*     dp g f e d c b a					       dp g f e d c b a            */
	RSEVSEG(0,0,1,1,1,1,1,1), /*   0   */	RSEVSEG(0,0,0,0,0,1,1,0), /*   1   */
	RSEVSEG(0,1,0,1,1,0,1,1), /*   2   */	RSEVSEG(0,1,0,0,1,1,1,1), /*   3   */
	RSEVSEG(0,1,1,0,0,1,1,0), /*   4   */	RSEVSEG(0,1,1,0,1,1,0,1), /*   5   */
	RSEVSEG(0,1,1,1,1,1,0,1), /*   6   */	RSEVSEG(0,0,0,0,0,1,1,1), /*   7   */
	RSEVSEG(0,1,1,1,1,1,1,1), /*   8   */	RSEVSEG(0,1,1,0,1,1,1,1), /*   9   */
	
	/* From 0x3A to 0x40 */
	/*     dp g f e d c b a					       dp g f e d c b a            */
	RSEVSEG(0,0,0,0,0,0,0,0), /* : NP  */	RSEVSEG(0,0,0,0,0,0,0,0), /* ; NP  */
	RSEVSEG(0,0,0,0,0,0,0,0), /* < NP  */	RSEVSEG(0,1,0,0,1,0,0,0), /*   =   */
	RSEVSEG(0,0,0,0,0,0,0,0), /* > NP  */	RSEVSEG(0,1,0,1,0,0,1,1), /*   ?   */
	RSEVSEG(0,0,0,0,0,0,0,0), /* @ NP  */
	
	
	/* Upper case (from 0x41 to 0x5A) */		
	/*     dp g f e d c b a					       dp g f e d c b a            */
	RSEVSEG(0,1,1,1,0,1,1,1), /*   A   */	RSEVSEG(0,1,1,1,1,1,0,0), /*   B   */
	RSEVSEG(0,0,1,1,1,0,0,1), /*   C   */	RSEVSEG(0,1,0,1,1,1,1,0), /*   D   */
	RSEVSEG(0,1,1,1,1,0,0,1), /*   E   */	RSEVSEG(0,1,1,1,0,0,0,1), /*   F   */
	RSEVSEG(0,1,1,0,1,1,1,1), /*   G   */	RSEVSEG(0,1,1,1,0,1,1,0), /*   H   */
	RSEVSEG(0,0,0,0,0,1,1,0), /*   I   */	RSEVSEG(0,0,0,1,1,1,1,0), /*   J   */
	RSEVSEG(0,0,0,0,0,0,0,0), /* K NP  */	RSEVSEG(0,0,1,1,1,0,0,0), /*   L   */
	RSEVSEG(0,0,0,0,0,0,0,0), /* M NP  */	RSEVSEG(0,1,0,1,0,1,0,0), /*   N   */
	RSEVSEG(0,0,1,1,1,1,1,1), /*   O   */	RSEVSEG(0,1,1,1,0,0,1,1), /*   P   */
	RSEVSEG(0,0,0,0,0,0,0,0), /* Q NP  */	RSEVSEG(0,1,0,1,0,0,0,0), /*   R   */
	RSEVSEG(0,1,1,0,1,1,0,1), /*   S   */	RSEVSEG(0,1,1,1,1,0,0,0), /*   T   */
	RSEVSEG(0,0,1,1,1,1,1,0), /*   U   */	RSEVSEG(0,0,0,0,0,0,0,0), /* V NP  */
	RSEVSEG(0,0,0,0,0,0,0,0), /* W NP  */	RSEVSEG(0,0,0,0,0,0,0,0), /*   X   */
	RSEVSEG(0,1,1,0,1,1,1,0), /*   Y   */	RSEVSEG(0,1,0,1,1,0,1,1), /*   Z   */
	
	/* From 0x5B to 0x60 */
	/*     dp g f e d c b a					       dp g f e d c b a            */
	RSEVSEG(0,0,1,1,1,0,0,1), /*   [   */	RSEVSEG(0,0,0,0,0,0,0,0), /* \ NP  */
	RSEVSEG(0,0,0,0,1,1,1,1), /*   ]   */	RSEVSEG(0,0,0,0,0,0,0,0), /* ^ NP  */
	RSEVSEG(0,0,0,0,1,0,0,0), /*   _   */	RSEVSEG(0,1,1,0,0,0,1,1), /*   `   */
	
	/* Lower case (from 0x61 to 0x7A) */
	/*     dp g f e d c b a					       dp g f e d c b a            */
	RSEVSEG(0,1,0,1,1,1,1,1), /*   a   */	RSEVSEG(0,1,1,1,1,1,0,0), /*   b   */
	RSEVSEG(0,1,0,1,1,0,0,0), /*   c   */	RSEVSEG(0,1,0,1,1,1,1,0), /*   d   */
	RSEVSEG(0,1,1,1,1,0,0,1), /*   e   */	RSEVSEG(0,1,1,1,0,0,0,1), /*   f   */
	RSEVSEG(0,1,1,0,1,1,1,1), /*   g   */	RSEVSEG(0,1,1,1,0,1,0,0), /*   h   */
	RSEVSEG(0,0,0,0,0,1,0,0), /*   i   */	RSEVSEG(0,0,0,1,1,1,1,0), /*   k   */
	RSEVSEG(0,0,0,0,0,0,0,0), /*  k NP */	RSEVSEG(0,0,0,1,1,0,0,0), /*   l   */
	RSEVSEG(0,0,0,0,0,0,0,0), /*  m NP */	RSEVSEG(0,1,0,1,0,1,0,0), /*   n   */
	RSEVSEG(0,1,0,1,1,1,0,0), /*   o   */	RSEVSEG(0,1,1,1,0,0,1,1), /*   p   */
	RSEVSEG(0,0,0,0,0,0,0,0), /* q NP  */	RSEVSEG(0,1,0,1,0,0,0,0), /*   r   */
	RSEVSEG(0,1,1,0,1,1,0,1), /*   s   */	RSEVSEG(0,1,1,1,1,0,0,0), /*   t   */
	RSEVSEG(0,0,0,1,1,1,0,0), /*   u   */	RSEVSEG(0,0,0,0,0,0,0,0), /* v NP  */
	RSEVSEG(0,0,0,0,0,0,0,0), /* w NP  */	RSEVSEG(0,0,0,0,0,0,0,0), /* x NP  */
	RSEVSEG(0,1,1,0,1,1,1,0), /*   y   */	RSEVSEG(0,1,0,1,1,0,1,1), /*   Z   */
	
	/* Special chars for printing W, w, M and m */
	/*     dp g f e d c b a					       dp g f e d c b a            */
	RSEVSEG(0,0,1,1,0,0,1,1), /*   M   */	RSEVSEG(0,0,1,0,0,1,1,1), /*   M   */
	RSEVSEG(0,1,0,1,0,1,0,0), /*   m   */	RSEVSEG(0,1,0,1,0,1,0,0), /*   m   */
	RSEVSEG(0,0,1,1,1,1,0,0), /*   W   */	RSEVSEG(0,0,0,1,1,1,1,0), /*   W   */
	RSEVSEG(0,0,0,1,1,1,0,0), /*   w   */	RSEVSEG(0,0,0,1,1,1,0,0), /*   w   */
	
};

#define LATCH_HIGH()	digitalWrite(m_iSCLK, 1)
#define LATCH_LOW()		digitalWrite(m_iSCLK, 0)

Artekit_MrWood::Artekit_MrWood()
{
	m_iQty = m_iSRCLK = m_iSCLK = m_iSER = m_iOE = 0;
}

Artekit_MrWood::Artekit_MrWood(int32_t iQty, int32_t iSRCLK, int32_t iSCLK, int32_t iSER, int32_t iOE)
{
	init(iQty, iSRCLK, iSCLK, iSER, iOE);
}

/*
 * init()
 * Basic initialization. Call it when instancing the class with the constructor
 * without parameters. 
 * 
 * Parameters:
 *	iQty			The quantity of displays.
 *	iSRCLK			Arduino pin connected to CLK.
 *	iSCLK			Arduino pin connected to LATCH.
 *	iSER			Arduino pin connected to DATA.
 *	iOE				Arduino pin connected to ENABLE.
 *
 */
void Artekit_MrWood::init(int32_t iQty, int32_t iSRCLK, int32_t iSCLK, int32_t iSER, int32_t iOE)
{
	m_iQty = iQty;
	m_iSRCLK = iSRCLK;
	m_iSCLK = iSCLK;
	m_iSER = iSER;
	m_iOE = iOE;
	
	pinMode(m_iSER, OUTPUT);
	pinMode(m_iSRCLK, OUTPUT);
	pinMode(m_iSCLK, OUTPUT);
	pinMode(m_iOE, OUTPUT);
}

/*
 * ShiftOut()
 * Internal function. It shifts out a byte as is, without being converted through the ASCII table.
 *
 * Parameters:
 *	ucChar		Byte to shift out.
 *
 */
void Artekit_MrWood::shift(uint8_t ucChar)
{
	/* Basic shiftOut() call */
	#ifdef USE_LSB
	shiftOut(m_iSER, m_iSRCLK, LSBFIRST, ucChar);
	#else
	shiftOut(m_iSER, m_iSRCLK, MSBFIRST, ucChar);
	#endif
}

/*
 * PrintAscii()
 * Internal function. It shifts out an ASCII character converting it with the ASCII table.
 *
 * Parameters:
 *	ucChar			Character to shift out.
 *	bDecimal		If 1, decimal point will be displayed. If 0, it won't (default).
 *
 */
void Artekit_MrWood::printAscii(uint8_t ucChar, bool bDecimal)
{
	/* Remove offset to fit our ASCII table */
	ucChar -= MRWOOD_TABLE_OFFSET;
	
	/* Valid char? */
	if (ucChar > MRWOOD_TABLE_MAX_VALUE) return;
	
	/* Check for decimal point */
	if (bDecimal)
	{
		shift(mrwood_ascii_table[ucChar] | DP_FLAG);
	} else {
		shift(mrwood_ascii_table[ucChar]);
	}
}

/*
 * printSpecial()
 * Internal function. This function formats and displays a string to include special W and M
 * characters and the decimal point, automatically.
 * The function returns the quantity of used displays (that may not match the length of the
 * original string, since we're using two displays to show M and W characters, and using a
 * single display to show a character with a decimal point).
 *
 * Parameters:
 *	pszString		String to format and display.
 *
 */
uint32_t Artekit_MrWood::printSpecial(char* pszString)
{
	uint8_t offs;
	uint8_t len = 0;
	char* ptr = pszString;
	uint8_t dp = 0;
	uint8_t printed = 0;
	
	/* Get the string length */
	len = strlen(ptr);
	offs = len - 1;
	
	/* Start from the end of the string */
	while (len--)
	{
		switch(ptr[offs])
		{
			/* Special W character */
			case 'W':
				if (dp)
				{
					printAscii(W_UC_HIGH, true);
					dp = 0;
				} else {
					printAscii(W_UC_HIGH, false);
				}

				printAscii(W_UC_LOW);
				offs--;
				printed += 2;
				break;
				
			/* Special w character */
			case 'w':
				if (dp)
				{
					printAscii(W_LC_HIGH, true);
					dp = 0;
				} else {
					printAscii(W_LC_HIGH);
				}

				printAscii(W_LC_LOW);
				offs--;
				printed += 2;
				break;
				
			/* Special M character */
			case 'M':
				if (dp)
				{
					printAscii(M_UC_HIGH, true);
					dp = 0;
				} else {
					printAscii(M_UC_HIGH);
				}

				printAscii(M_UC_LOW);
				offs--;
				printed += 2;
				break;
				
			/* Special m character */
			case 'm':
				if (dp)
				{
					printAscii(M_LC_HIGH, true);
					dp = 0;
				} else {
					printAscii(M_LC_HIGH);
				}

				printAscii(M_LC_LOW);
				offs--;
				printed += 2;
				break;
				
			/* Special decimal point character */
			case '.':
				if (offs != 0)
				{
					dp = 1;					
				} else {
					printAscii(ptr[offs]);
					printed++;
				}
				offs--;
				break;
				
			/* Any other character */
			default:
				if (dp)
				{
					printAscii(ptr[offs--], true);
					dp = 0;
				} else {
					printAscii(ptr[offs--]);
				}
				
				printed++;
				break;
		}		
	}
	
	if (dp)
	{
		printAscii('.', 1);
		printed++;
	}

	return printed;
}

/*
 * clear()
 * This function clears all the the displays, based on the user-declared quantity of displays.
 *
 */
void Artekit_MrWood::clear()
{
	uint8_t i;

	LATCH_LOW();
	
	for (i = 0; i < m_iQty; i++)
	{
		/* Reset all the shift register pins */
		shift(0x00);
	}
	
	LATCH_HIGH();
}

/*
 * enable()
 * This function enables all the displays.
 *
 */
void Artekit_MrWood::enable()
{
	digitalWrite(m_iOE, 0);
}

/*
 * disable()
 * This function disables all the displays.
 *
 */
void Artekit_MrWood::disable()
{
	digitalWrite(m_iOE, 1);
}

/*
 * printInt()
 * Prints a integer number
 *
 * Parameters:
 *	iNum	An integer numbert.
 *
 */
void Artekit_MrWood::printInt(int32_t iNum)
{
	memset(szConvBuf, 0, MRWOOD_INT_CONVERSION_LEN);
	sprintf(szConvBuf, "%i", iNum);
	printFormatted(szConvBuf);
}

/*
 * printFloat()
 * Prints a floating point number
 *
 * Parameters:
 *	fNum			An floating point numbert.
 *	iTotalLength	Total length of the resulting string, including decimals,
 *					decimal point and sign.
 *	ucPrecision		Quantity of decimals.		
 *
 */
void Artekit_MrWood::printFloat(float fNum, int8_t iTotalLength, uint8_t ucPrecision)
{
	memset(szConvBuf, 0, MRWOOD_INT_CONVERSION_LEN);
	/* Call AVR float conversion function */
	dtostrf(fNum, iTotalLength, ucPrecision, szConvBuf);
	printFormatted(szConvBuf);
}

/*
 * printBytes()
 * Shifts out an array of bytes as is, without being converted through the ASCII table.
 *
 * Parameters:
 *	ucBytes			Bytes to shift out.
 *	ulLength		Quantity of bytes
 *
 */
void Artekit_MrWood::printBytes(uint8_t* ucBytes, uint32_t ulLength)
{
	uint32_t i;
	
	/* Reset LATCH */
	LATCH_LOW();
	
	for (i = 0; i < ulLength; i++)
	{
		/* Shift out every byte in the array */
		shift(*(ucBytes++));
	}
	
	/* Set LATCH */
	LATCH_HIGH();
}

/*
 * printByte()
 * Shifts out a byte, without being converted through the ASCII table.
 *
 * Parameters:
 *	ucByte		Bytes to shift out.
 *
 */
void Artekit_MrWood::printByte(uint8_t ucByte)
{
	/* Reset LATCH */
	LATCH_LOW();
	
	shift(ucByte);
	
	/* Set LATCH */
	LATCH_HIGH();
}


/*
 * printChar()
 * This function shifts out an ASCII character converting it with the ASCII table.
 *
 * Parameters:
 *	ucChar		Character to shift out.
 *	bDecimal	If true, decimal point will be displayed. If false, it won't (default).
 *
 */
void Artekit_MrWood::printChar(uint8_t ucChar, bool bDecimal)
{
	/* Reset LATCH */
	LATCH_LOW();
	
	printAscii(ucChar, bDecimal);
	
	/* Set LATCH */
	LATCH_HIGH();
}

/*
 * Print()
 * This function displays an array of characters, of an user-defined length, converting it with the
 * ASCII table. Decimal point is not considered. To print an array of bytes with decimal point,
 * use the mrwood_print_string() function.
 *
 * Parameters:
 *	ucChars		Array to shift out.
 *	ulLength	Length of the array.
 *
 */
void Artekit_MrWood::printChars(uint8_t* ucChars, uint32_t ulLength)
{
	uint8_t i;
	
	/* Reset LATCH */
	LATCH_LOW();
	
	for (i = 0; i < ulLength; i++)
	{
		printAscii(*(ucChars++));
	}
	
	/* Set LATCH */
	LATCH_HIGH();
}

/*
 * printFormatted()
 * This function formats and displays a string to include special W and M characters and the
 * decimal point, automatically.
 * Set the align parameter to 1 to right-align the string. This depends on the quantity of
 * displays the user declares when calling the mrwood_init() function.
 *
 * Parameters:
 *	string		String to format and display.
 *	align		When 1, the string is right-aligned. When 0, the string is left-aligned.
 *
 */
void Artekit_MrWood::printFormatted(char* pszString, uint8_t ucAlign)
{
	uint8_t printed = 0;
	uint8_t i;
	
	/* Reset LATCH */
	LATCH_LOW();
	
	if (ucAlign == 0)
	{
		/* Default alignment to the left */
		printSpecial(pszString);
	} else {

		/* Normal formatted print */
		printed = printSpecial(pszString);
		
		/* Spaces left? */
		if (m_iQty > printed)
		{
			/* Yes, fill them with SPACE */
			for (i = 0; i < (m_iQty - printed); i++)
			{
				printAscii(0x20);
			}
		}
	}
	
	/* Set LATCH */
	LATCH_HIGH();
}

/*
 * dimmer()
 * This function uses Arduino's PWM to apply a percentual dimmer on all the displays.
 *
 * Parameters:
 *	percent		PWM duty cycle, in percent.
 *
 */
void Artekit_MrWood::dimmer(uint8_t ucPercent)
{
	/* Calculate percent and convert it to a value between 0 and 255, for PWM */
	if (ucPercent > 100) ucPercent = 100;
	ucPercent = (ucPercent * 255) / 100;
	
	analogWrite(m_iOE, ucPercent);
}
