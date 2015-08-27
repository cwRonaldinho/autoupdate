/*
 * Encode a hex digest representation of a binary buffer.
 * Copyright (C) 2002 Johnathan Ingram, jingram@rogueware.org
 *
 * This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 *
 *
 * Notes:
 * 
 *
 */

#include "hexDigest.h"

#include <stdio.h>
#include <string.h> 
#include <ctype.h>


namespace HexDigest
{


//------------------------------------------------------------------------------
// HexDigest::encode
//------------------------------------------------------------------------------
void
encode(
      unsigned char *buffer,
      int size,
      char *hexDigest)
{
   char element[3];
   for (register int i=0; i<size; i++)
   {
      sprintf(element, "%02x", buffer[i]);
      hexDigest[i*2] = toupper(element[0]);
      hexDigest[(i*2)+1] = toupper(element[1]);
   }

   hexDigest[size*2] = '\0';
}  // End of HexDigest::encode




//------------------------------------------------------------------------------
// HexDigest::decode
//------------------------------------------------------------------------------
void
decode(
      char *hexDigest,
      unsigned char *buffer,
      int maxElements)
{
   register int i;
   register int len;
   register unsigned char val;

   if (!hexDigest || strlen(hexDigest) % 2 != 0)  // Must be even
      return;

   len = (int) strlen(hexDigest);
   for (i=0; i<len; i+=2)
   {
      if (i/2 >= maxElements)
         break;

      val = (unsigned char)hexDigest[i];
      /* Convert the ordinal value of the character */
      if (val >= 97)      /* chars abcdef */
      {
         val -= 87;
      }
      else
      {
         if (val >= 65)    /* chars ABCDEF */
            val -= 55;
         else
            val -= 48;      /* chars 0123456789 */
      }
      
      buffer[i/2] = (val << 4);

      val = (unsigned char)hexDigest[i+1];
      /* Convert the ordinal value of the character */
      if (val >= 97)      /* chars abcdef */
      {
         val -= 87;
      }
      else
      {
         if (val >= 65)     /* chars ABCDEF */
            val -= 55;
         else
            val -= 48;      /* chars 0123456789 */
      }
      buffer[i/2] += val;
   }
}  // End of HexDigest::decode

}
