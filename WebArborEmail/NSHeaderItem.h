// NSHeaderItem
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include<string>
#include"NSErrorClasses.h"
#include"NSStringFunctions.h"
#include"AMMimeUtils.h" // This is used for iso... header deciphering.

#ifndef NS_HEADER_ITEM
#define NS_HEADER_ITEM

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

	class NSHeaderItem : private NSStringFunctions, private AMMimeUtils {
	
	public:

		NSHeaderItem();
		~NSHeaderItem();

		string GetHeaderItem( string, const char * );

	private:


	};

} // End namespace definition

#endif
