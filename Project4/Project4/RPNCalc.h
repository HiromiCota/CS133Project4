//----------------------------------------------------------------------------
//    File:		RPNCalc.h
//
//    Class:	CRPNCalc
//----------------------------------------------------------------------------
#ifndef RPNCALC_H
#define RPNCALC_H

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <new>
#include <sstream>
#include <stack>
#include <regex>
//----------------------------------------------------------------------------
//
//    Title:		RPNCalc Class
//
//    Description:	This file contains the class definition for CRPNCalc
//
//    Programmer:	Paul Bladek
//   
//    Date:		5/3/11
// 
//    Version:		1.03
//  
//    Environment:
//       Hardware: Intel Xeon PC
//       Software: Windows 7
//       Compiles under Microsoft Visual Studio 2010
// 
//	  class CRPNCalc:
//
//	  Properties:
//		double m_registers[10] -- 
//		string m_buffer -- 
//		stack<string> m_stack -- 
//		vector<list<string>> m_programs(NUMPROGRAMS) -- 
//		istringstream m_inStrStream -- 
//		m_on -- 
//		bool m_error --
//		bool m_helpOn --  
//
//	  Methods:
//	
//		inline:	None		
//
//		non-inline:
//		public:
//			CRPNCalc(bool on);
//			void run();                                        
//			void print(ostream& ostr);
//			void input(istream& istr);
//			void onOff(bool on = true) 
//		private:
//				
//			void add() -- 
//			void bin_prep(double& d1, double& d2) -- 
//			void clear() -- 
//			void clearAll() -- 
//			void divide() -- 
//			void exp() -- 
//			void getReg(int reg) -- 
//			bool isDouble(string rawInput);
//			bool isPow(string rawInput);
//			bool isClear(string rawInput);
//			bool isLetterOperator(string rawInput);
//			bool isRegisterGet(string rawInput);
//			bool isRegisterSet(string rawInput);
//			bool isBinaryOperator(string rawInput);
//			void loadProgram() -- 
//			void mod() -- 
//			void multiply() -- 
//			void neg() -- 
//			void parse() -- 
//			void recordProgram() -- 
//			void rotateUp() -- 
//			void rotateDown() -- 
//			void runProgram() -- 
//			void saveToFile() -- 
//			void setReg(int reg) -- 
//			string stripChar(string rawInput);
//			void subtract() -- 
//			void unary_prep(double& d) -- 		   
//
//    History Log:
//	4/20/03	PB  completed version 1.0
//	5/27/05	PB  minor modifications 1.01
//	5/3/11	PB  minor modifications 1.02
//	6/3/12  PB  minor modifications 1.03
//	6/5/07	HRC Added regex validation for input v1.1
// ----------------------------------------------------------------------------

using namespace std;

namespace PB_CALC
{
	const char helpMenu[] = "C clear stack   | CE clear entry  | D rotate down  | F save program to file\n"
				"G0-G9 get reg n | H help on/off   | L load program | M +/- | P program on/off\n"
				"R run program   | S0-S9 set reg n | U rotate up    | X exit\n";

	const char line[] =		"____________________________________________________________________________\n";

	const unsigned short NUMREGS = 10;

	class CRPNCalc
	{
	public:
		CRPNCalc(bool on = true);
		void run();                                        
		void print(ostream& ostr);  // changes m_error on error, so not const
		void input(istream& istr);		
		void onOff(bool on = true) { m_on = on; }

	private:
	// private methods
		//Unary methods
		void clearEntry();
		void clearAll();
		void getReg(int reg);
		void neg();
		void rotateUp();
		void rotateDown();
		void setReg(int reg);
		
		// Binary methods
		void add();		
		void divide();
		void exp();
		void mod();
		void multiply();
		void subtract();
		
		// I/O methods
		void loadProgram();
		void parse();
		void recordProgram();
		void runProgram();
		void saveToFile();
		
		// Regex tests to identify and validate input
		bool isDouble(string rawInput);
		bool isPow(string rawInput);
		bool isClear(string rawInput);
		bool isLetterOperator(string rawInput);
		bool isRegisterGet(string rawInput);
		bool isRegisterSet(string rawInput);
		bool isBinaryOperator(string rawInput);
		int whatIsThis(string rawInput);
		
		// Strips the first char of a string to allow easier parsing of G0-9, S0-9, and exponents
		string stripChar(string rawInput) { return (rawInput.substr(1, rawInput.size() - 2)); }

	// private properties
		double m_registers[NUMREGS];
		string m_buffer;
		deque<double> m_stack;
		list<string> m_program;
		istringstream m_instrStream;
		bool m_error;
		bool m_helpOn;
		bool m_on;
		bool m_programRunning;
	};

	ostream &operator <<(ostream &ostr, CRPNCalc &calc);
	istream &operator >>(istream &istr, CRPNCalc &calc);

} // end namespace PB_CALC

#endif
