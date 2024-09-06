/*==============================================================================
|
|  NAME
|
|     ppUnitTest.hpp
|
|  DESCRIPTION
|
|     Header file for unit test routines.
|
|     User manual and technical documentation are described in detail in my web page at
|     http://seanerikoconnor.freeservers.com/Mathematics/AbstractAlgebra/PrimitivePolynomials/overview.html
|
|  LEGAL
|
|     Primpoly Version 16.3 - A Program for Computing Primitive Polynomials.
|     Copyright (C) 1999-2024 by Sean Erik O'Connor.  All Rights Reserved.
|
|     This program is free software: you can redistribute it and/or modify
|     it under the terms of the GNU General Public License as published by
|     the Free Software Foundation, either version 3 of the License, or
|     (at your option) any later version.
|
|     This program is distributed in the hope that it will be useful,
|     but WITHOUT ANY WARRANTY; without even the implied warranty of
|     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|     GNU General Public License for more details.
|
|     You should have received a copy of the GNU General Public License
|     along with this program.  If not, see http://www.gnu.org/licenses/.
|     
|     The author's address is seanerikoconnor!AT!gmail!DOT!com
|     with the !DOT! replaced by . and the !AT! replaced by @
|
==============================================================================*/

// Wrap this header file to prevent duplication if it is included
// accidentally more than once.
#ifndef __PP_UNITTEST_H__
#define __PP_UNITTEST_H__


/*=============================================================================
|
| NAME
|
|     unitTest
|
| DESCRIPTION
|
|     In the spirit of EXTREME programming, test each class and its member
|     functions.  Run the tests every time we run this application.
|     Place the test results into the current directory in the file
|     unitTest.txt.  If we can't write the file, log an error message
|     to the console and skip the tests.  If we can't even do that, throw
|     an exception.
|
+============================================================================*/

class UnitTest
{
    public:
        // Default constructor with a reasonable default file name.
        UnitTest( const char * fileName = "unitTest.log" ) ;

        ~UnitTest() ;

        // We have only one unit test object;  prevent any copy constructors or
        // assignment operators from being defined.
        UnitTest( const UnitTest & u ) = delete ;
        UnitTest & operator=( const UnitTest & unitTest ) = delete ;

        // Run all unit tests.
        bool run() ;

    protected:
        // Unit tests for different functional areas.
        bool unitTestSystemFunctions() ;
        bool unitTestBigIntBase10() ;
        bool unitTestBigIntDefaultBase() ;
        bool unitTestModPArithmetic() ;
        bool unitTestFactoring() ;
        bool unitTestPolynomials() ;
        bool unitTestPolynomialOrder() ;
        bool unitTestParser() ;

    protected:
        // File to log the unit test results.  Can be standard output (console) if we can't open the file.
        const char * unit_test_log_file_name_ ;
        ofstream fout_ ;
} ;


/*=============================================================================
|
| NAME
|
|     UnitTestError
|
| DESCRIPTION
|
|     Exception classes for the the UnitTest class 
|     derived from the STL exception class runtime_error.
|
+============================================================================*/

class UnitTestError : public runtime_error
{
    public:
        // Throw with error message, file name and line number.
        UnitTestError( const string & description, const string & file, const int & line )
        : runtime_error( description + " in file " + file + " at line " + to_string(line) )
        {
        } ;
    
        // Throw with an error message.
        UnitTestError( const string & description )
            : runtime_error( description )
        {
        } ; 

        // Default throw with no error message.
        UnitTestError()
            : runtime_error( "UnitTest exception: " )
        {
        } ;
} ;
#endif // __PP_UNITTEST_H__ -- End of wrapper for header file.
