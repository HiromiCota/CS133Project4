#ifndef PARSE_H
#define PARSE_H

#include "RPNCalc.h"
using namespace std;

namespace PB_CALC
{
	// ----------------------------------------------------------------------------
	//	parses the next command from m_instrStream
	// ----------------------------------------------------------------------------
	void CRPNCalc::parse()
	{
		//Takes m_instrStream istream and calls appropriate function with correct arguments
		double leftTerm, rightTerm;
		m_instrStream >> leftTerm;	//Grab first number and consume
		m_instrStream >> rightTerm;	//Grab second number and consume
		while (isdigit(m_instrStream.peek()) != 1)	//Seek stream for 
		{

		}
		
		
	}
}
#endif // !PARSE_H

