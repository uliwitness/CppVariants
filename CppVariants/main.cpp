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


using namespace std;


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



union CppVariantUnion { CppVariantInt b; CppVariantDouble c; };

class CppVariant
{
public:
	CppVariant( int n = 0 )	{ new (mBuf) CppVariantInt(n); }
	~CppVariant()			{ ((CppVariantBase*)mBuf)->~CppVariantBase(); }
	
	operator CppVariantBase* () { return (CppVariantBase*)mBuf; }
	CppVariantBase* operator -> () { return (CppVariantBase*)mBuf; }

protected:
	uint8_t mBuf[sizeof(union CppVariantUnion)];
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
	CppVariant someNum(42);
	
	cout << "Original int: " << someNum->GetAsInt()
		<< " (" << someNum->GetAsDouble() << ")" << endl;
	
	someNum->SetAsInt(700);	// This is just a setter call.

	cout << "Changed int: " << someNum->GetAsInt()
		<< " (" << someNum->GetAsDouble() << ")" << endl;
	
	someNum->SetAsDouble(12.34);	// This changes the class to CppVariantDouble.

	cout << "Converted to Double: " << someNum->GetAsInt()
		<< " (" << someNum->GetAsDouble() << ")" << endl;
	
    return 0;
}
