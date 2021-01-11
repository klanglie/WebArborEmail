// NSUUCode.cpp
// v2.0.1.72 2005/11/30
// (c) 2003 - 2005 Langlie Systems, Inc. www.langliesystems.com
// All rights reserved.
// You may not re-use this code without permission from Langlie Systems, Inc.

#include "stdafx.h"
#include "NSUUCode.h"

using namespace WEBARBOR_EMAIL_COMPONENTS;

// NSStringFunctions constructor
NSUUCode::NSUUCode()
{ 

}

// NSStringFunctions destructor
// Make sure to delete all global pointers that have been allocated
NSUUCode::~NSUUCode()
{
}

inline int NSUUCode::ReadDataLine(FILE *f,char *buf)
{
        int c,i;

        i=0;
        while ((c=fgetc(f)) != EOF) {
                if (c == '\n') break;
                if (c != '\r') {
                        buf[i++] = c;
                }
        }
        buf[i] = 0;
        return(c == EOF ? 0 : 1);
}

char * NSUUCode::uudecode ( FILE *f, const char * pSaveLocation )
{
  register int n;
  register char ch, *p;
  char buf[2 * BUFSIZ];
  char *outname = new char[512];			// Kai Langlie, 31 December 2002.															
  //static char outname[200];	// Kai Langlie, 31 December 2002.
  FILE *out;

  do {
          if (!ReadDataLine(f,buf)) return(NULL);
  }
  while (strncmp(buf,"begin",5));
  p = buf + 6;


  while(*p && (strchr(" 01234567",*p))) p++;					// Clem Dye, 13 July 2001.
  // while (*p && (*p == ' '|| *p == '0' || *p == '6')) p++;	// Clem Dye, 13 July 2001.
  

  basic_string <char>::size_type i = 0;										// Kai Langlie, 31 December 2002.	
  for( i = 0; i < strlen(pSaveLocation); i++ ){		// Kai Langlie, 31 December 2002.
	outname[i] = pSaveLocation[i];					// Kai Langlie, 31 December 2002.
  }													// Kai Langlie, 31 December 2002.
  for( basic_string <char>::size_type j = 0 ; j < strlen(p); j++ ){			// Kai Langlie, 31 December 2002.
	outname[i] = *(p + j);							// Kai Langlie, 31 December 2002.
	i++;											// Kai Langlie, 31 December 2002.
  }													// Kai Langlie, 31 December 2002.
  outname[i] = 0;									// Kai Langlie, 31 December 2002.
  //strcpy(outname,p);								// Kai Langlie, 31 December 2002.


  /* Create output file and set mode.  */ 
  out = fopen(outname,"wb");
  if (out == NULL) {							// Russell Treleaven, October 2001.
		printf("I can't create %s.\n",outname);	// Russell Treleaven, October 2001.
		return(NULL);							// Russell Treleaven, October 2001.
  }												// Russell Treleaven, October 2001.	

  /* For each input line:  */
  while (1)
    {
        if (!ReadDataLine(f,buf)) break;
        if (!strncmp(buf,"end\r\n",5)) break;
          p = buf;

      /* N is used to avoid writing out all the characters at the end of the file.  */

      n = DEC (*p);
      if (n <= 0)
        break;
      for (++p; n > 0; p += 4, n -= 3)
        {
          if (n >= 3)
            {
              ch = DEC (p[0]) << 2 | DEC (p[1]) >> 4; 
              fputc(ch,out);
              ch = DEC (p[1]) << 4 | DEC (p[2]) >> 2; 
              fputc (ch,out);
              ch = DEC (p[2]) << 6 | DEC (p[3]);
              fputc (ch,out);
            }
          else
            {
              if (n >= 1)
                {
                  ch = DEC (p[0]) << 2 | DEC (p[1]) >> 4; 
                  fputc (ch,out);
                }
              if (n >= 2)
                {
                  ch = DEC (p[1]) << 4 | DEC (p[2]) >> 2; 
                  fputc (ch,out);
                }
            }
        }
    }

  fclose(out);
  return outname;
}