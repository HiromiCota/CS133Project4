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

	// ----------------------------------------------------------------------------
	//	parses the next command from m_instrStream
	// ----------------------------------------------------------------------------
	void CRPNCalc::parse()
	{
		//Takes m_instrStream istream and calls appropriate function with correct arguments
		double leftTerm, rightTerm;
		string rawInput = "";
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
			else if (rawInput == "R")
				runProgram();
			else //if (rawInput == "X")
				m_on = false;
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
		///potential fixes/changes
		///SHOULD WE BE DELETING THE CONTENTS OF THE m_program LIST FIRST?

		string filename;			//string storing the user-created filename
		string line_contents;			//string containing the contents from file (by line)
		ifstream if_handler;			//input filestream
		list<string>::iterator list_it;		//iterator used to push program contents into m_program
		bool filename_error = false;		//used for errors with opening a file with filename input

		while (!if_handler.is_open())
		{
			if (filename_error == true)	//checks if the loop had executed once previously
				cout << "Could not load program from \"" << filename << "\". Please Try again." << endl;
			cout << "Enter filename (\".txt\" will be added): ";
			cin >> filename;			///SHOULD THIS BE USING getline() INSTEAD? A safeRead() FUNC?
			cin.clear();
			cin.ignore(FILENAME_MAX, '\n');
			filename += ".txt";
			if_handler.open(filename);
			filename_error = true;	//sets a flag for future loops to tell the user that the file could not be opened
		}
		filename_error = false; //resets error flag upon completion of while() loop, file is open

		while (getline(if_handler, line_contents))
			m_program.push_back(line_contents);


		if_handler.close();
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
		m_program.clear(); //clears the current program list before recording
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
		///potential fixes/changes
		string filename;			//string storing the user-created filename
		ofstream of_handler;			//output filestream
		list<string>::iterator list_it;		//iterator used to print program contents
		bool filename_error = false;		//used for errors with opening a file with filename input

		while (!of_handler.is_open())
		{
			if (filename_error == true)	//checks if the loop had executed once previously
				cout << "Could not save to a file using \"" << filename << "\". Please Try again." << endl;
			cout << "Enter filename (\".txt\" will be added): ";
			cin >> filename;		///SHOULD THIS BE USING getline() INSTEAD?
			cin.clear();
			cin.ignore(FILENAME_MAX, '\n');
			filename += ".txt";
			of_handler.open(filename);
			filename_error = true;	//sets a flag for future loops to tell the user that the file could not be opened
		}
		filename_error = false; //resets error flag upon completion of while() loop, file is open

		for (list_it = m_program.begin(); list_it != m_program.end(); list_it++)
			of_handler << *list_it << endl;


		of_handler.close();	
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
