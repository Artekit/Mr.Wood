/***************************************************************************
 *
 * AK-Mr.Wood-L
 * Library for Arduino V1.0b
 *
 * MrWood.h
 * Copyright (C) 2014 Artekit Italy.
 * http://www.artekit.eu
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
#ifndef __MRWOOD_H__
#define __MRWOOD_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define ALIGN_LEFT	0
#define ALIGN_RIGHT	1

#define MRWOOD_INT_CONVERSION_LEN	16

class Artekit_MrWood
{
public:
	Artekit_MrWood(int32_t iQty, int32_t iSRCLK, int32_t iSCLK, int32_t iSER, int32_t iOE);
	Artekit_MrWood();
	
	void init(int32_t iQty, int32_t iSRCLK, int32_t iSCLK, int32_t iSER, int32_t iOE);
	void enable();
	void disable();
	void clear();
	void dimmer(uint8_t ucPercent);
	
	void printChar(uint8_t ucChar, bool bDecimal = false);
	void printChars(uint8_t* ucChars, uint32_t ulLength);
	void printFormatted(char* pszString, uint8_t ucAlign = ALIGN_LEFT);
	void printInt(int32_t iNum);
	void printFloat(float fNum, int8_t iTotalLength, uint8_t ucPrecision);
	
	void printByte(uint8_t ucByte);
	void printBytes(uint8_t* ucBytes, uint32_t ulLength);

private:

	/* User-defined arduino pins, connected to the AK-Mr.Wood-L */
	int	m_iSRCLK;	/* Clock */
	int	m_iSCLK;	/* Latch */
	int	m_iSER;		/* Data */
	int	m_iOE;		/* Enable */
	
	/* The quantity of AK-Mr.Wood-L this library is managing. */
	int m_iQty;
	
	char szConvBuf[MRWOOD_INT_CONVERSION_LEN];
		
	void shift(uint8_t ucChar);
	void printAscii(uint8_t ucChar, bool bDecimal = false);
	uint32_t printSpecial(char* pszString);
};

#endif /* __MRWOOD_H__ */
