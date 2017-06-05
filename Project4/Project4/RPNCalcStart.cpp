#include "RPNCalc.h"
//-------------------------------------------------------------------------------------------
//    Class:		CRPNCalc
//
//    File:             RPNCalc.cpp
//
//    Description:	This file contains the function definitions for CRPNCalc
//
//    Programmer:		
//   
//    Date:             
// 
//    Version:          1.0
//  
//    Environment:	Intel Xeon PC 
//                  	Software:   MS Windows 7 for execution; 
//                  	Compiles under Microsoft Visual C++.Net 2010
// 
//	  class CRPNCalc:
//
//	  Properties:
//				double m_registers[10];
//				stack<string> m_stack;
//				vector<list<string>> m_programs(NUMPROGRAMS);
//				istringstream m_inStrStream;
//	
//
//	  Non-inline Methods:
//	
//				CRPNCalc(bool on)
//				run()
//				print(ostream& ostr)
//				parse()
//				void add() -- 
//				void bin_prep(double& d1, double& d2) -- 
//				void clear() -- 
//				void clearAll() -- 
//				void divide() -- 
//				void exp() -- 
//				void getReg(int reg) -- 
//				void loadProgram() -- 
//				void mod() -- 
//				void multiply() -- 
//				void neg() -- 
//				void parse() -- 
//				void recordProgram() -- 
//				void rotateUp() -- 
//				void rotateDown() -- 
//				void runProgram() -- 
//				void saveToFile() -- 
//				void setReg(int reg) -- 
//				void subtract() -- 
//				void unary_prep(double& d) -- 
	
//	  related functions:
//				ostream &operator <<(ostream &ostr, const CRPNCalc &calc)
//    				istream &operator >>(istream &istr, CRPNCalc &calc)
//
//    History Log:
//                           
// ----------------------------------------------------------------------------	

// ----------------------------------------------------------------------------
// Temporary includes
// ----------------------------------------------------------------------------
#include "parse.h"

namespace PB_CALC
{
// ----------------------------------------------------------------------------
//	constructor
// ----------------------------------------------------------------------------
	CRPNCalc::CRPNCalc(bool on): m_on(on), m_error(false), m_helpOn(true),
		m_programRunning(false)
	{
		for(int i = 0; i < NUMREGS; i++)
			m_registers[i] = 0.0;
		if(m_on)
			run();
	}

// ----------------------------------------------------------------------------
//	starts the calculator running
// ----------------------------------------------------------------------------
	void CRPNCalc::run()
	{
		while (m_on == true)
		{
			runProgram();
		}
	} 

// ----------------------------------------------------------------------------
//	prints out calculator screen
// ----------------------------------------------------------------------------
	void CRPNCalc::print(ostream& ostr)
	{
		double d = 0.0;
		ostr << "[RPN Programmable Calculator] by Paul Bladek" << endl;
		if(m_helpOn)
			cout << helpMenu;
		else
			cout << endl << endl << endl;
		cout << line;
		if(!m_stack.empty())
		{
			d = m_stack.front();
			ostr << d;
		}
		ostr << endl << endl;
		if(m_error)
		{
			ostr << "<<error>>" << endl;
			m_error = false;
		}
	} 

// parse() moved to parse.h

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, adds them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------	
	void CRPNCalc::add()
	{
		if (m_stack.size >= 2)
		{
			double one = m_stack.pop_front;
			double two = m_stack.pop_front;
			double three = one + two;
			m_stack.push_front(three);
		}
		else 
		{
			cout << "There are not enough items to perform an operation";
		}
	} 

// ----------------------------------------------------------------------------
//	removes the top element from the stack
// ----------------------------------------------------------------------------
	void CRPNCalc::clearEntry()
	{
		m_stack.pop_front();
	} 

// ----------------------------------------------------------------------------
//	empties the stack
// ----------------------------------------------------------------------------
	void CRPNCalc::clearAll()
	{
		while(!m_stack.empty())	
			m_stack.pop_front();
	} 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, divides them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::divide()
   {
	   if (m_stack.size >= 2)
	   {
		   double one = m_stack.pop_front;
		   double two = m_stack.pop_front;
		   double three = one / two;
		   m_stack.push_front(three);
	   }
	   else
	   {
		   cout << "There are not enough items to perform an operation";
	   }
   } 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack,
//	  raises one element to the other power
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::exp()
   {
	   double one;
	   double two;
	   one = m_stack.pop_front;
	   two = m_stack.pop_front;
	   if (two == 0)
	   {
		   one = 1;
	   }
	   else 
	   {
		   for (int i = 0; i < two; i++)
		   {
			   one *= one;
		   }
	   }
	   m_stack.push_front(one);
   }  

// ----------------------------------------------------------------------------
//	pushes the given register's value onto the stack
// ----------------------------------------------------------------------------
	void CRPNCalc::getReg(int reg)
	{
		m_stack.push_front(m_registers[reg]);
	}  

// ----------------------------------------------------------------------------
//	retrieves the filename from the user and loads it into m_program
// ----------------------------------------------------------------------------
	void CRPNCalc::loadProgram()
	{
	
	}  

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, mods them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::mod()
   {
	   if (m_stack.size >= 2)
	   {
		   int one = m_stack.pop_front;
		   int two = m_stack.pop_front;
		   int three = one % two;
		   m_stack.push_front(three);
	   }
	   else
	   {
		   cout << "There are not enough items to perform an operation";
	   }
   } 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, multiplies them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::multiply()
   {
	   if (m_stack.size >= 2)
	   {
		   double one = m_stack.pop_front;
		   double two = m_stack.pop_front;
		   double three = one * two;
		   m_stack.push_front(three);
	   }
	   else
	   {
		   cout << "There are not enough items to perform an operation";
	   }
   }  

// ----------------------------------------------------------------------------
//	resets the top element of the stack to it's negative
// ----------------------------------------------------------------------------
	void CRPNCalc::neg()
	{
		double one = m_stack.pop_front;
		one *= -1;
		m_stack.push_front(one);
	}  

// ----------------------------------------------------------------------------
//	takes command-line input and loads it into m_program 
// ----------------------------------------------------------------------------
	void CRPNCalc::recordProgram()
	{

	} 

// ----------------------------------------------------------------------------
//	removes the bottom of the stack and adds it to the top
// ----------------------------------------------------------------------------
	void CRPNCalc::rotateDown()
	{
		if (!m_stack.empty())
		{
			m_stack.push_front(m_stack.pop_back);
		}
	} 

// ----------------------------------------------------------------------------
//	removes the top of the stack and adds it to the bottom
// ----------------------------------------------------------------------------
	void CRPNCalc::rotateUp()
	{
		if (!m_stack.empty())
		{
			m_stack.push_back(m_stack.pop_front);
		}
	} 

// ----------------------------------------------------------------------------
//	runs the program in m_program 
// ----------------------------------------------------------------------------
	void CRPNCalc::runProgram()
	{
	
	} 

// ----------------------------------------------------------------------------
//	asks the user for a filename and saves m_program to that file
// ----------------------------------------------------------------------------
	void CRPNCalc::saveToFile()
	{
			
	}  

// ----------------------------------------------------------------------------
//	gets the value from the top of the stack
//	  and places it into the given register
// ----------------------------------------------------------------------------
	void CRPNCalc::setReg(int reg)
	{
		m_registers[reg] = m_stack.pop_front;
	} 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, subtracts them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::subtract()
   {
	   if (m_stack.size >= 2)
	   {
		   double one = m_stack.pop_front;
		   double two = m_stack.pop_front;
		   double three = one - two;
		   m_stack.push_front(three);
	   }
	   else
	   {
		   cout << "There are not enough items to perform an operation";
	   }
   } 

// ----------------------------------------------------------------------------
//	inputs a line from the given stream
// ----------------------------------------------------------------------------
	void CRPNCalc::input(istream &istr)
	{

	} 

// ----------------------------------------------------------------------------
//	ostream's << defined for CRPNCalc
// ----------------------------------------------------------------------------
   ostream &operator <<(ostream &ostr, CRPNCalc &calc)
	{
		calc.print(ostr);
		return ostr;
	} 


// ----------------------------------------------------------------------------
//	istream's >> defined for CRPNCalc
// ----------------------------------------------------------------------------
	istream &operator >>(istream &istr, CRPNCalc &calc)
	{
		calc.input(istr);
		return istr;
	} 
} // end namespace PB_CALC
