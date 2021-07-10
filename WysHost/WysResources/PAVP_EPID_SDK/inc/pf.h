/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/
#ifndef _PF_H
#define _PF_H

#include <cassert>

#include "bignumber.h"

/*
 *	Prototype of the element of the G3 order prime field
 */

class G3OrderElement;

/*
 *	Prime field defining order of the G3 curve
 */

class G3OrderPrimeField {
public:
	G3OrderPrimeField(BigNum& Modulus);
	~G3OrderPrimeField();

	// Generate random element
	bool GetRandomElement(G3OrderElement& RandElem, IppsPRNGState* PrngCtx) const;

	// Modulus of the prime field
	BigNum* _Modulus;

protected:

};


/*
 *	Element of the G3 order prime field
 */

class G3OrderElement {
	friend class G3ECPrimeField;

public:
	G3OrderElement(G3OrderPrimeField* G3OrderField);
	G3OrderElement(G3OrderPrimeField* G3OrderField, BigNum& Value);
	~G3OrderElement();

	// Arithmetic operators
	G3OrderElement& operator = (const G3OrderElement& Elem);

	// Conversion to big number
	void GetBigNum(BigNum& Bn);

protected:
	// Pointer to the underlying prime field
	G3OrderPrimeField* G3Order;

	// The actual value of the prime field element
	BigNum* Data;
};

#endif