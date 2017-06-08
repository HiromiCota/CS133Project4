
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
		transform(rawInput.begin(), rawInput.end(), rawInput.begin(),
			[](unsigned char c) { return ::toupper(c); });			//Instruction stream to upper
		m_instrStream >> rawInput;		//Grab first element 

		// Valid first elements:
		// Double
		// Clear (C or CE)
		// Register Get (G0-G9)
		// Rotations (D)own or (U)p
		// (H)elp, (L)oad, save (F)ile, (M)ultiply top of stack by -1, record (P)rogram
		// (R)un program, e(X)it program


		if (isDouble(rawInput))			//Validates with regex. 
		{
			leftTerm = stod(rawInput);	//This will throw if the user enters over 300 digits
		}
		else if (isClear(rawInput))		//C or CE
		{
			if (rawInput == "C")
				clearEntry();
			else
				clearAll();
		}
		else if (isLetterOperator(rawInput))
		{
			if (rawInput == "D")
				rotateDown();
			else if (rawInput == "U")
				rotateUp();
			else if (rawInput == "H")
				cout << helpMenu;
			else if (rawInput == "L")
				loadProgram();
			else if (rawInput == "F")
				saveToFile();
			else if (rawInput == "M")
				neg();
			else if (rawInput == "P")
				recordProgram();
			else //if (rawInput == "R")
				runProgram();
			/*else if (rawInput == "X")
				exit();*/
		}
		else if (isRegisterGet(rawInput))	//G0-G9
		{
			rawInput = stripChar(rawInput);	//Strip 'G'
			int index = stoi(rawInput);
			getReg(index);
		}
		else
		{
			//The first string in the stream is not valid. Set error and throw.
			m_error = true;
			throw invalid_argument("Program stream must start with a number or an operator.");
		}
				
		// Valid second elements:
		// Double
		// Register set (S0-S9)
		// Unary operators (^)
		
		rawInput = "";
		m_instrStream >> rawInput;
		if (isDouble(rawInput))			//Validates with regex. 
			rightTerm = stod(rawInput);	//This will throw if the user enters over 300 digits
		else if (isRegisterSet(rawInput))
		{
			rawInput = stripChar(rawInput);	//Strip 'S'
			int index = stoi(rawInput);
			m_stack.push_front(leftTerm);	//Store the first value passed in
			setReg(index);
		}
		else if (isPow(rawInput))
		{
			rawInput = stripChar(rawInput);
			rightTerm = stod(rawInput);	//Already validated to be a number
			m_stack.push_front(rightTerm);	//Store exponent first
			m_stack.push_front(leftTerm);
			exp();
		}
		else
		{
			m_error = true;
			throw invalid_argument("Missing second number or a unary operator");
		}			
		
		// Valid third elements:
		// Binary operators (+,-,/,*,%)
		rawInput = "";
		m_instrStream >> rawInput;
		if (isBinaryOperator(rawInput))
		{
			m_stack.push_front(leftTerm);
			m_stack.push_front(rightTerm);
			if (rawInput == "+")
				add();
			else if (rawInput == "-")
				subtract();
			else if (rawInput == "/")
				divide();
			else if (rawInput == "*")
				multiply();
			else 
				mod();
		}
		else
		{
			m_error = true;
			throw invalid_argument("Third element must be a binary operator");
		}		
	}
	// ----------------------------------------------------------------------------
	//	Validates string only contains valid double elements
	// ----------------------------------------------------------------------------
	bool CRPNCalc::isDouble(string rawInput)
	{		
		regex validation("^([-+]?)([0-9]*)([.]?)([0-9]*)$");
		if (regex_match(rawInput, validation))
			return true;
		else
			return false;
	}
	// ----------------------------------------------------------------------------
	//	Validates string only contains ^ and a valid int
	//  Int can be negative
	// ----------------------------------------------------------------------------
	bool CRPNCalc::isPow(string rawInput)
	{		
		regex pow("^(\^)([\-]?)([0-9]*)$");	
		if (regex_match(rawInput, pow))
			return true;
		else
			return false;
	}
	// ----------------------------------------------------------------------------
	//	Validates string only contains C or CE (clear operations)
	// ----------------------------------------------------------------------------
	bool CRPNCalc::isClear(string rawInput)
	{
		//Validates C and CE
		regex clear("^(C)[(E)?]$");		
		if (regex_match(rawInput, clear))
			return true;
		else
			return false;
	}
	// ----------------------------------------------------------------------------
	//	Validates string only contains one of the single letter operators
	// ----------------------------------------------------------------------------
	bool CRPNCalc::isLetterOperator(string rawInput)
	{
		regex singleLetters("^([D,F,H,L,M,P,R,U,X])$");	//Validates single letter operations
		if (regex_match(rawInput, singleLetters))
			return true;
		else
			return false;
	}
	// ----------------------------------------------------------------------------
	//	Validates string only contains a Get Register call
	// ----------------------------------------------------------------------------
	bool CRPNCalc::isRegisterGet(string rawInput)
	{
		regex regGet("^(G)([0-9])$");	//Validates G0-G9 and S0-S9
		if (regex_match(rawInput, regGet))
			return true;
		else
			return false;
	}
	// ----------------------------------------------------------------------------
	//	Validates string only contains a Set Register call
	// ----------------------------------------------------------------------------
	bool CRPNCalc::isRegisterSet(string rawInput)
	{
		regex regSet("^(S)([0-9])$");	//Validates G0-G9 and S0-S9
		if (regex_match(rawInput, regSet))
			return true;
		else
			return false;
	}
	// ----------------------------------------------------------------------------
	//	Validates string only contains a single binary operator
	// ----------------------------------------------------------------------------
	bool CRPNCalc::isBinaryOperator(string rawInput)
	{
		regex binaryOp("^([\+,\-,\*,\/,\%])$");
		if (regex_match(rawInput, binaryOp))
			return true;
		else
			return false;
	}
	// ----------------------------------------------------------------------------
	//	Strips first character from string
	// ----------------------------------------------------------------------------
	string stripChar(string rawInput)
	{
		rawInput = rawInput.substr(1, rawInput.size() - 2);	//Strip first char
	}
}
#endif // !PARSE_H

