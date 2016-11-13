//
//  main.cpp
//  CppVariants
//
//  Created by Uli Kusterer on 13/11/2016.
//  Copyright © 2016 Uli Kusterer. All rights reserved.
//

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>


#define CPPVMAX2(a,b) (((a) > (b)) ? (a) : (b))
#define CPPVMAX3(a,b,c)	CPPVMAX2(CPPVMAX2((a), (b)),(c))


using namespace std;


char varnames[26] = {	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
						'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
						'u', 'v', 'w', 'x', 'y', 'z' };


void	MakeMacros( size_t maxNumArgs )
{
	stringstream	ss;
	size_t			currNumArgs = 3;
	
	ss << "#define CPPVMAX2(a,b) (((a) > (b)) ? (a) : (b))" << endl;
	
	for( size_t x = 2; x < maxNumArgs; x++ )
	{
		ss << "#define CPPVMAX" << (x+1) << "(";
		size_t y = 0;
		for( ; y < currNumArgs; y++ )
		{
			if( y == 0 ) ss << varnames[y];
			else ss << "," << varnames[y];
		}
		ss << ")\tCPPVMAX2(CPPVMAX" << x << "(";
		y = 0;
		for( ; y < (currNumArgs -1); y++ )
		{
			if( y == 0 ) ss << "(" << varnames[y] << ")";
			else ss << ", (" << varnames[y] << ")";
		}
		ss << "),(" << varnames[y] << "))" << endl;
		
		currNumArgs++;
	}
	
	cout << ss.str().c_str() << endl;
}



class CppVariantBase
{
public:
	CppVariantBase() { cout << "CppVariantBase constructor." << endl;  }
	virtual ~CppVariantBase() { cout << "CppVariantBase destructor." << endl; }
	
	virtual int		GetAsInt() = 0;
	virtual double	GetAsDouble() = 0;
	
	virtual void	SetAsInt( int n );
	virtual void	SetAsDouble( double n );
};


class CppVariantInt : public CppVariantBase
{
public:
	CppVariantInt( int n = 0 ) : mInt(n)
	{
		cout << "CppVariantInt constructor." << endl;
	}
	~CppVariantInt() { cout << "CppVariantInt destructor." << endl; }

	virtual int		GetAsInt()		{ return mInt; }
	virtual double	GetAsDouble()	{ return mInt; }
	
	virtual void	SetAsInt( int n )			{ mInt = n; }
	
protected:
	int		mInt;
};


class CppVariantDouble : public CppVariantBase
{
public:
	CppVariantDouble( double n = 0 ) : mDouble(n)
	{
		cout << "CppVariantDouble constructor." << endl;
	}
	~CppVariantDouble()
	{
		cout << "CppVariantDouble destructor." << endl;
	}

	virtual int		GetAsInt()
	{
		if( int(mDouble) == mDouble )
			return mDouble;
		else
			return round(mDouble);
	}
	virtual double	GetAsDouble()			{ return mDouble; }
	
	virtual void	SetAsDouble( int n )	{ mDouble = n; }

protected:
	double mDouble;
};


class CppVariant
{
public:
	CppVariant( int n = 0 )	{ new (mBuf) CppVariantInt(n); }
	~CppVariant()			{ ((CppVariantBase*)mBuf)->~CppVariantBase(); }
	
	operator CppVariantBase* () { return (CppVariantBase*)mBuf; }
	CppVariantBase* operator -> () { return (CppVariantBase*)mBuf; }

protected:
	uint8_t mBuf[CPPVMAX3(sizeof(CppVariantBase),sizeof(CppVariantInt),sizeof(CppVariantDouble))];
};


void	CppVariantBase::SetAsInt( int n )
{
	this->~CppVariantBase();
	new (this) CppVariantInt(n);
}


void	CppVariantBase::SetAsDouble( double n )
{
	this->~CppVariantBase();
	new (this) CppVariantDouble(n);
}


int main(int argc, const char * argv[]) {
	//MakeMacros( 26 );	// Build the CPPMAXn() macros for mBuf's size.
	
	CppVariant someNum(42);
	
	cout << "Original int: " << someNum->GetAsInt() << " (" << someNum->GetAsDouble() << ")" << endl;
	
	someNum->SetAsInt(700);

	cout << "Changed int: " << someNum->GetAsInt() << " (" << someNum->GetAsDouble() << ")" << endl;
	
	someNum->SetAsDouble(12.34);

	cout << "Converted to Double: " << someNum->GetAsInt() << " (" << someNum->GetAsDouble() << ")" << endl;
	
    return 0;
}
