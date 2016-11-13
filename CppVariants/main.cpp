//
//  main.cpp
//  CppVariants
//
//  Created by Uli Kusterer on 13/11/2016.
//  Copyright © 2016 Uli Kusterer. All rights reserved.
//

#include <iostream>
#include <string>


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
	
protected:
	union {
		int			intValue;
		double		doubleValue;
	} m;
};


class CppVariantInt : public CppVariantBase
{
public:
	CppVariantInt( int n = 0 ) { cout << "CppVariantInt constructor." << endl; m.intValue = n; }
	~CppVariantInt() { cout << "CppVariantInt destructor." << endl; }

	virtual int		GetAsInt()		{ return m.intValue; }
	virtual double	GetAsDouble()	{ return m.intValue; }
	
	virtual void	SetAsInt( int n )			{ m.intValue = n; }
};


class CppVariantDouble : public CppVariantBase
{
public:
	CppVariantDouble( double n = 0 ) { cout << "CppVariantDouble constructor." << endl; m.doubleValue = n; }
	~CppVariantDouble() { cout << "CppVariantDouble destructor." << endl; }

	virtual int		GetAsInt()				{ if( int(m.doubleValue) == m.doubleValue ) return m.doubleValue; else return 0; }
	virtual double	GetAsDouble()			{ return m.doubleValue; }
	
	virtual void	SetAsDouble( int n )	{ m.doubleValue = n; }
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
	char buf[sizeof(CppVariantBase)];
	CppVariantBase* someNum = new (buf) CppVariantInt(42);
	
	cout << "Original int: " << someNum->GetAsInt() << " (" << someNum->GetAsDouble() << ")" << endl;
	
	someNum->SetAsInt(700);

	cout << "Changed int: " << someNum->GetAsInt() << " (" << someNum->GetAsDouble() << ")" << endl;
	
	someNum->SetAsDouble(12.34);

	cout << "Converted to Double: " << someNum->GetAsInt() << " (" << someNum->GetAsDouble() << ")" << endl;
	
	someNum->~CppVariantBase();
	
    return 0;
}
