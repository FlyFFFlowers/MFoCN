/*==============================================================================
|
|  NAME
|
|     Primpoly.cpp
|
|  DESCRIPTION
|
|     Program for finding primitive polynomials of degree n modulo p for
|     any prime p >= 2 and any n >= 2.
|
|     Useful for generating PN sequences and finite fields for error control coding.
|
|     Please see the user manual and complete technical documentation at
|
|         http://seanerikoconnor.freeservers.com/Mathematics/AbstractAlgebra/PrimitivePolynomials/overview.html
|
|     This is a console app to be run in a terminal window.  The OS calls main(), which returns a status to the OS.
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


/*------------------------------------------------------------------------------
|                                Include Files                                 |
------------------------------------------------------------------------------*/

#include <cstdlib>      // abort()
#include <iostream>     // Basic stream I/O.
#include <new>          // set_new_handler()
#include <cmath>        // Basic math functions e.g. sqrt()
#include <limits>       // Numeric limits.
#include <complex>      // Complex data type and operations.
#include <fstream>      // File stream I/O.
#include <sstream>      // String stream I/O.
#include <vector>       // STL vector class.
#include <string>       // STL string class.
#include <algorithm>    // Iterators.
#include <stdexcept>    // Exceptions.
#include <cassert>      // assert()

using namespace std ;   // I don't want to use the std:: prefix everywhere.

#include "ctype.h"      // C string functions.


/*------------------------------------------------------------------------------
|                                PP Include Files                              |
------------------------------------------------------------------------------*/

#include "Primpoly.hpp"         // Global functions.
#include "ppArith.hpp"          // Basic arithmetic functions.
#include "ppBigInt.hpp"         // Arbitrary precision integer arithmetic.
#include "ppOperationCount.hpp" // OperationCount collection for factoring and poly finding.
#include "ppFactor.hpp"         // Prime factorization and Euler Phi.
#include "ppPolynomial.hpp"     // Polynomial operations and mod polynomial operations.
#include "ppParser.hpp"         // Parsing of polynomials and I/O services.
#include "ppUnitTest.hpp"       // Complete unit test.


/*=============================================================================
|
| NAME
|
|     Message strings.
|
| DESCRIPTION
|
|     Helpful information returned by top level main() function.
|
+============================================================================*/

static const string legalNotice
{
    "\n"
    "Primpoly Version 16.3 - A Program for Computing Primitive Polynomials.\n"
    "Copyright (C) 1999-2024 by Sean Erik O'Connor.  All Rights Reserved.\n"
   "\n"
    "Primpoly comes with ABSOLUTELY NO WARRANTY; for details see the\n"
    "GNU General Public License.  This is free software, and you are welcome\n"
    "to redistribute it under certain conditions; see the GNU General Public License\n"
    "for details.\n\n"
}  ;

static const string helpText
{
     "This program generates primitive polynomials of degree n modulo p.\n"
     "\n"
     "Usage:  Generate a single random polynomial of degree n modulo p where p is a prime >= 2 and n is an integer >= 2\n"
     "        Primpoly p n\n"
     "Example:\n"
     "        Primpoly 2 4\n"
     "          Self-check passes...\n"
     "          Primitive polynomial modulo 2 of degree 4\n"
     "          x ^ 4 + x + 1, 2\n"
     "Usage:  Test whether a polynomial is primitive modulo p.\n"
     "        Primpoly -t ""<Polynomial to test>, p""\n"
     "          If you leave off the modulus p we default to p = 2\n"
     "Examples:\n"
     "        Primpoly -t ""x^4 + x + 1, 2""\n"
     "          Self-check passes...\n"
     "          x ^ 4 + x + 1, 2 is primitive!\n"
     "\n"
     "        Primpoly -t ""x^4 + x + 1""\n"
     "          Self-check passes...\n"
     "          x ^ 4 + x + 1, 2 is primitive!\n"
     "Usage:  Generate all primitive polynomial of degree n modulo p.\n"
     "        Primpoly -a p n\n"
     "Example:\n"
     "        Primpoly -a 2 4\n"
     "          Self-check passes...\n"
     "          Primitive polynomial modulo 2 of degree 4\n"
     "          x ^ 4 + x + 1, 2\n"
     "          Primitive polynomial modulo 2 of degree 4\n"
     "          x ^ 4 + x ^ 3 + 1, 2\n"
     "Usage:  Same but show computation statistics.\n"
     "        Primpoly -s p n\n"
     "Examples:  \n"
     "\n"
     "        Primpoly.exe -s 13 19\n"
     "          Self-check passes...\n"
     "          Primitive polynomial modulo 13 of degree 19\n"
     "          x ^ 19 + 9 x + 2, 13\n"
     "\n"
     "          +--------- OperationCount --------------------------------\n"
     "          |\n"
     "          | Integer factorization:  Table lookup + Trial division + Pollard Rho\n"
     "          |\n"
     "          | Number of trial divisions :           0\n"
     "          | Number of gcd's computed :            9027\n"
     "          | Number of primality tests :           2\n"
     "          | Number of squarings:                  9026\n"
     "          |\n"
     "          | Polynomial Testing\n"
     "          |\n"
     "          | Total num. degree 19 poly mod 13 :      1461920290375446110677\n"
     "          | Number of possible primitive poly:    6411930599771980992\n"
     "          | Polynomials tested :                  120\n"
     "          | Const. coeff. was primitive root :    46\n"
     "          | Free of linear factors :              11\n"
     "          | Irreducible to power >=1 :            1\n"
     "          | Had order r (x^r = integer) :         1\n"
     "          | Passed const. coeff. test :           1\n"
     "          | Had order m (x^m != integer) :        1\n"
     "          |\n"
     "          +-----------------------------------------------------\n"
     "Usage:  Print help message.\n"
     "        Primpoly -h\n"
     "          <Prints this help message.>\n"
     "\n\n"
     "Primitive polynomials find many uses in mathematics and communications\n"
     "engineering:\n"
     "   * Generation of pseudonoise (PN) sequences for spread spectrum\n"
     "     communications and chip fault testing.\n"
     "   * Generating Sobol sequences for high dimensional numerical integration.\n"
     "   * Generation of CRC and Hamming codes.\n"
     "   * Generation of Galois (finite) fields for use in decoding Reed-Solomon\n"
     "     and BCH error correcting codes.\n"
     "\n"
     "For detailed technical information, see my web page\n"
     "    http://seanerikoconnor.freeservers.com/Mathematics/AbstractAlgebra/PrimitivePolynomials/overview.html\n"
     "\n"
} ;


/*=============================================================================
|
| NAME
|
|     main
|
| DESCRIPTION
|
|    Program for finding primitive polynomials of degree n modulo p for
|    any prime p >= 2 and any n >= 2.
|
|    Useful for generating PN sequences and finite fields for error control coding.
|
|    Run the program by typing
|
|        ./Primpoly p n
|
|    You will get an nth degree primitive polynomial modulo p.
|
|    To see all the options type
|
|        ./Primpoly -h
|
|    Please see the user manual and complete technical documentation at
|
|        http://seanerikoconnor.freeservers.com/Mathematics/AbstractAlgebra/PrimitivePolynomials/overview.html
|
|    The author's address is seanerikoconnor!AT!gmail!DOT!com
|    with the !DOT! replaced by . and the !AT! replaced by @
|
+============================================================================*/

int main( int argc, const char * argv[] )
{
    try
    {
        // Make my lawyers happy.
        cout << legalNotice ;
      
        // Set up the full parser for both command line parsing and polynomial parsing.
        PolyParser<PolySymbol, PolyValue> parser ;
        parser.parseCommandLine( argc, argv ) ;

        // Print the help message only and exit.
        if (parser.print_help_)
        {
            cout << helpText ;
            return static_cast<int>( ReturnStatus::AskForHelp ) ;
        }

        #ifdef SELF_CHECK
        // Do a self check always.  We might fail to pass one or more unit tests, or the unit test itself might fail.
        try
        {
            UnitTest unitTest ;
            if (!unitTest.run())
                  throw PrimpolyError( "Self-check failed!" ) ;
              else
                  cout << "Self-check passes..." << endl ;
        }
        catch (UnitTestError & e)
        {
            throw PrimpolyError( static_cast<string>( "Could not run the self-check!\n" ) + " [ " + e.what() + " ] " ) ;
        }
        #endif

        // The user input a polynomial.  Test it for primitivity.
        if (parser.test_polynomial_for_primitivity_)
        {
            // Test for primitivity with the quick test.
            Polynomial f( parser.test_polynomial_ ) ;
            #ifdef DEBUG_PP_PRIMITIVITY
            cout << "Factoring into primes r = (p^n-1)/(p-1) = " << " for n = " << parser.n << " p = " << parser.p << endl ;
            #endif // DEBUG_PP_PRIMITIVITY
            PolyOrder order( f ) ;
            cout << f << " is " << (order.isPrimitive() ? "" : "NOT") << " primitive!" << endl ;

            if (parser.print_operation_count_)
                cout << order.statistics_ << endl ;

            // Also do a very slow maximal order test for primitivity, if asked to do so.
            if (parser.slow_confirm_)
            {
                cout << confirmWarning ;
                cout << " confirmed " << (order.maximal_order() ? "" : "NOT") << " primitive!" << endl ;
            }
        }
        //  Find one primitive polynomial at random.  Optionally, find all primitive polynomials.
        else
        {
            //   Generate and test all possible n th degree, monic, modulo p polynomials
            //   f(x).  A polynomial is primitive if passes all the tests successfully.
            //                       n
            //   Initialize f(x) to x  + (-1).  Then, when f(x) passes through function
            //                                                                        n
            //   nextTrialPoly for the first time, it will have the correct value, x
            Polynomial f ;
            f.initialTrialPoly( parser.n, parser.p ) ;
            #ifdef DEBUG_PP_PRIMITIVITY
            cout << "Factoring into primes r = (p^n-1)/(p-1) = " << " for n = " << parser.n << " p = " << parser.p << endl ;
            #endif // DEBUG_PP_PRIMITIVITY
            PolyOrder order( f ) ;

            bool is_primitive_poly{ false } ;
            bool tried_all_poly{ false } ;
            bool stopTesting{ false } ;

            BigInt num_poly( 0u ) ;
            BigInt num_primitive_poly( 0u ) ;

            if (parser.list_all_primitive_polynomials_)
                cout << "\n\nThere are " << order.getNumPrimPoly() << " primitive polynomials modulo " << f.modulus() << " of degree " << f.deg() << "\n\n" ;

            do {
                ++num_poly ;
                
                #ifdef DEBUG_PP_PRIMITIVITY
                cout << "Testing polynomial # " << num_poly << ") p(x) = " << f << " for primitivity" << endl ;
                #endif // DEBUG_PP_PRIMITIVITY

                order.resetPolynomial( f ) ;
                is_primitive_poly = order.isPrimitive() ;

                if (is_primitive_poly)
                {
                    ++num_primitive_poly ;
                    cout << "\n\nPrimitive polynomial modulo " << f.modulus() << " of degree " << f.deg() << "\n\n" ;
                    cout << f ;
                    cout << endl << endl ;

                    // Do a very slow maximal order test for primitivity.
                    if (parser.slow_confirm_)
                    {
                        cout << confirmWarning ;
                        if (order.maximal_order())
                            cout << f << " confirmed primitive!" << endl ;
                        else
                        {
                            ostringstream os ;
                            os << "Fast test says " << f << " is a primitive polynomial but slow test disagrees.\n" ;
                            throw PolynomialError( os.str() ) ;
                        }
                    }

                    // Early out if we've found all the primitive polynomials.
                    if (num_primitive_poly >= order.getNumPrimPoly())
                        break ;
                }

                tried_all_poly = (num_poly >= order.getMaxNumPoly()) ;
                stopTesting = tried_all_poly || (!parser.list_all_primitive_polynomials_ && is_primitive_poly) ;

                f.nextTrialPoly() ;      // Try next polynomal in sequence.
            } while( !stopTesting ) ;

            if (parser.print_operation_count_)
                cout << order.statistics_ << endl ;

            // Didn't find a primitive polynomial in the find-only-one-primitive-polynomial case, which is an error.
            if (!parser.list_all_primitive_polynomials_ && !is_primitive_poly)
            {
                ostringstream os ;
                os << "Tested all " << order.getMaxNumPoly() << " possible polynomials, but failed to find a primitive polynomial" ;
                throw PolynomialError( os.str() ) ;
            }
        }
        return static_cast<int>( ReturnStatus::Success ) ;
    }
    // Catch all exceptions and report what happened to the user.
    // First do the user-defined exceptions.
    catch( PrimpolyError & e )
    {
        cerr << "\nTop Level Error: " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( ParserError & e )
    {
        cerr << "Inputs are incorrect or out of range: " << " [ " << e.what() << " ] " << endl << helpText ;
        return static_cast<int>( ReturnStatus::RangeError ) ;
    }
    catch( FactorError & e )
    {
        cerr << "Error in prime factorization:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( BigIntRangeError & e )
    {
        cerr << "Internal range error in multiple precision arithmetic:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( BigIntDomainError & e )
    {
        cerr << "Internal domain error in multiple precision arithmetic:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( BigIntUnderflow & e )
    {
        cerr << "Internal underflow error in multiple precision arithmetic:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( BigIntOverflow & e )
    {
        cerr << "Internal overflow error in multiple precision arithmetic:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( BigIntZeroDivide & e )
    {
        cerr << "Internal zero divide error in multiple precision arithmetic:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( BigIntMathError & e )
    {
        cerr << "Internal math error in multiple precision arithmetic:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( ArithModPError & e )
    {
        cerr << "Internal modulo p arithmetic error:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( PolynomialRangeError & e )
    {
        cout << "Error.  Polynomial has bad syntax or coefficients are out of range. " << " [ " << e.what() << " ] " << endl << helpText ;
        return static_cast<int>( ReturnStatus::RangeError ) ;
    }
    catch( PolynomialError & e )
    {
        cerr << "Error in polynomial arithmetic:  " << " [ " << e.what() << " ] " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    //  Standard library exceptions.
    catch( bad_alloc & e )
    {
        cerr << "Error allocating memory:  " << " [ " << e.what() << " ] "<< endl ;
        cerr << "Run on a different computer with more RAM or virtual memory." << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    catch( exception & e )
    {
        cerr << "System error: " << e.what() << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }
    // Catch all other uncaught exceptions, which would otherwise call terminate()
    // which in turn calls abort() and which would halt this program.
    //
    // Limitations:
    //     We can't handle the case where terminate() gets called because the
    //     exception mechanism finds a corrupt stack or catches a destructor
    //     throwing an exception.
    // 
    //     Also we can't catch exceptions which are thrown by initializing or
    //     destructing global variables.
    catch( ... )
    {
        cerr << "Unexpected exception: " << endl << writeToAuthorMessage ;
        return static_cast<int>( ReturnStatus::InternalError ) ;
    }

    return static_cast<int>( ReturnStatus::Success ) ;

} //========================== end of function main ========================
