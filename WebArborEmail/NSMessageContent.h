// NSMessageContent
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include <string>
using std::string;

#ifndef NS_MESSAGE_CONTENT
#define NS_MESSAGE_CONTENT

namespace WEBARBOR_EMAIL_COMPONENTS{ // Start the definition of this namespace

	class NSMessageContent {
	
	public:

		NSMessageContent();
		~NSMessageContent();

		void SetEncoding( int encoding ){ i_encoding = encoding; };
		void SetBody( string &sBody );

		int GetEncoding(){ return i_encoding; }
		string GetBody();
		

	private:

		int i_encoding;
		string sMessage;

	};

} // End namespace definition

#endif