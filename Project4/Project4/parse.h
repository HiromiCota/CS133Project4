
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
		char operation;
		string rawInput="";
		m_instrStream >> rawInput;		//Grab first element - Should be a number
		if (isDouble(rawInput))			//Validates with regex. 
			leftTerm = stod(rawInput);	//This will throw if the user enters over 300 digits
		else
		{
			//The first string in the stream is not a number. Set error and throw.
			m_error = true;
			throw invalid_argument("Program stream must start with a number.");
		}
		rawInput = "";
		m_instrStream >> rawInput;
		if (isDouble(rawInput))			//Validates with regex. 
			rightTerm = stod(rawInput);	//This will throw if the user enters over 300 digits
		else
		{
			transform(rawInput.begin(), rawInput.end(), rawInput.begin(),
				[](unsigned char c) { return std::toupper(c); });
			
			//Might be a user error. Might be a unary operation.
		}
		//If there's a binary operation, the operator should be next in the stream.
			 
		
		
	}
	bool CRPNCalc::isDouble(string rawInput)
	{
		// This matches only strings with:
		// A single leading + or - (optional)
		// Any number of digits.
		// A single decimal point (optional)
		// Any number of digits.
		// And NOTHING else.
		regex validation("^([-+]?)([0-9]*)([.]?)([0-9]*)$");
		if (regex_match(rawInput, validation))
			return true;
		else
			return false;
	}
	bool CRPNCalc::isOperation(string rawInput)
	{
		regex pow("^(/^)([0-9]*)$");	//Validates ^ and an int
		regex clear("^(C)[(E)?)$");		//Validates C and CE
		regex singleLetters("^([D,F,H,L,M,P,R,U,X]?)$");
		regex regGet("^(G)([0-9])$");	//Validates G0-G9
		regex regSet("^(S)([0-9])$");	//Validates S0-S9
		regex 
	}
}
#endif // !PARSE_H

