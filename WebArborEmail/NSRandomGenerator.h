// NSRandomGenerator.h
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include<time.h> // for random number generation.

#ifndef NS_RANDOM_GENERATION
#define NS_RANDOM_GENERATION

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

static	int	rgiState[2+55]; // leave this alone, it is for random number generation.

class NSRandomGeneration{

public:

	NSRandomGeneration();
	virtual ~NSRandomGeneration();

	// random number declarations
	void init_mm( );
	int number_range( int from, int to );
	// end of random number declarations

private:

	inline int number_mm( void );

};

} // end namespace

#endif