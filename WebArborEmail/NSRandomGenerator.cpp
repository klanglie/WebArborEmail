// NSRandomGenerator.cpp
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSRandomGenerator.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

// NSStringFunctions constructor
NSRandomGeneration::NSRandomGeneration()
{ 

}

// NSStringFunctions destructor
// Make sure to delete all global pointers that have been allocated
NSRandomGeneration::~NSRandomGeneration()
{
}


inline int NSRandomGeneration::number_mm( void )


    {
    int *piState;
    int iState1;
    int iState2;
    int iRand;
    piState		= &rgiState[2];
    iState1	 	= piState[-2];
    iState2	 	= piState[-1];
    iRand	 	= ( piState[iState1] + piState[iState2] )
    			& ( ( 1 << 30 ) - 1 );
    piState[iState1]	= iRand;
    if ( ++iState1 == 55 )
    	iState1 = 0;
    if ( ++iState2 == 55 )
    	iState2 = 0;
    piState[-2]		= iState1;
    piState[-1]		= iState2;
    return iRand >> 6;
}
/*
* Generate a random number.
*/
int NSRandomGeneration::number_range( int from, int to )


    {
    int power;
    int number;
    if ( ( to = to - from + 1 ) <= 1 )
    	return from;
    for ( power = 2; power < to; power <<= 1 )
    	;
    while ( ( number = number_mm( ) & ( power - 1 ) ) >= to )
    	;
    return from + number;
}
/*
* this is the Mitchell-Moore algorithm from Knuth Volume II. 
*/
void NSRandomGeneration::init_mm( )


    {
    int *piState;
    int iState;
    piState	= &rgiState[2];
    piState[-2]	= 55 - 55;
    piState[-1]	= 55 - 24;
    piState[0]	= ( (int) time( NULL ) ) & ( ( 1 << 30 ) - 1 );
    piState[1]	= 1;
    for ( iState = 2; iState < 55; iState++ )


        {
        	piState[iState] = ( piState[iState-1] + piState[iState-2] )
        			& ( ( 1 << 30 ) - 1 );
    }
    return;
}