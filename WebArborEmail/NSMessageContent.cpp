// NSMessageContent
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSMessageContent.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

NSMessageContent::NSMessageContent()
{
	i_encoding = 0; 
	sMessage = "";
}

NSMessageContent::~NSMessageContent()
{

}

void NSMessageContent::SetBody( string &sBody )
{
	sMessage = sBody;
}

string NSMessageContent::GetBody()
{
	return sMessage;
}