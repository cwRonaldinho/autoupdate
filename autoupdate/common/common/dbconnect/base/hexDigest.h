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
 * TODO:
 *   1  Optimize the decode method. Kinda crusty at the moment
 *
 */

#ifndef __HEX_DIGEST_H__
#define __HEX_DIGEST_H__

namespace HexDigest
{
   
   /* Encodes the buffer into a hex representation
    *
    * @param buffer    Pointer to the binary buffer to hex encode
    * @param size      Size of the buffer in bytes
    * @param hexDigest Pointer the char buffer to put the result. Must have memory allocated. (Buf Size = 2*sizeof(buffer)+1)
    *
    */
   void
   encode(
         unsigned char *buffer,
         int size,
         char *hexDigest);

   /* Decodes the buffer from a hex representation
    *
    * @param hexDigest   Pointer to the buffer containing the hex digest to decode
    * @param buffer      Pointer the buffer to put the result. Must have memory allocated. (Buf Size = strlen(hexDigest)/2)
    * @param size        Size of the buffer after it has been decoded
    *
    */
   void
   decode(
         char *hexDigest,
         unsigned char *buffer,
         int maxElements);
}

#endif
