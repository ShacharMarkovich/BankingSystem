/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/
#ifndef _ECPF_H
#define _ECPF_H

#include <cassert>

#include "ippcp.h"
#include "bignumber.h"
#include "pf.h"


/*
 *	Prototype of the elliptic curve G3 point
 */

class G3Element;

/*
 *	Elliptic curve G3
 */

class G3ECPrimeField {
public:
	G3ECPrimeField();
	~G3ECPrimeField();

	// Conversion to IppsECCPState
	operator IppsECCPState* () const { return EcpfCtx; }

	// Elliptic curve arithmetic needed for SIGMA DH key derivation
	bool Exp(const G3Element& G, const BigNum& a, const G3Element& Ga) const;
	bool Exp(const G3Element& G, const G3OrderElement& a, const G3Element& Ga) const;

	// EC prime field group generator
	G3Element* g;
	// Order of the field
	G3OrderPrimeField* Order;

protected:
	// G3 EC Prime Field context pointer (ECC)
	IppsECCPState* EcpfCtx;
};


/*
 *	Point on the elliptic curve G3
 */

class G3Element {
public:
	G3Element(G3Element& Elem);
	G3Element(G3ECPrimeField* G3Field);
	G3Element(G3ECPrimeField* G3Field, BigNum& X, BigNum& Y);
	~G3Element();

	// Retrieve the EC context
	friend IppsECCPState* ECC(const G3Element& Elem) { return (IppsECCPState*)(*(Elem.G3)); }

	// Conversion to IppsECCPPointState
	operator IppsECCPPointState* () const { return EcpfPointCtx; }

	// Arithmetic operators
	G3Element& operator = (const G3Element& Elem);

	// Retrieve the coordinates values
	bool GetCoords(BigNum& X, BigNum& Y);

protected:
	// G3 EC prime field (elliptic curve)
	G3ECPrimeField* G3;

	// Point in G3 context pointer
	IppsECCPPointState* EcpfPointCtx;

	void InitPoint(G3ECPrimeField* G3Field);
};

#endif