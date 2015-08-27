#include "stdafx.h"

#include <cacti/util/DES.h>

#include <memory.h>
#include <string.h>
#include <stdlib.h>

#define new DEBUG_NEW

namespace cacti
{
	// -----------------------------------------------------------------------
	// Initial of static data members. These data will be used by all the
	// instances of class,and can not be changed.
	// -----------------------------------------------------------------------
	unsigned char DES::Df_Key[24] = {
		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
		0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67 };

	unsigned short DES::bytebit[8] = {
		0200, 0100, 040, 020, 010, 04, 02, 01 };

	unsigned long DES::bigbyte[24] = {
		0x800000L, 0x400000L, 0x200000L, 0x100000L,
		0x80000L,  0x40000L,  0x20000L,  0x10000L,
		0x8000L,   0x4000L,   0x2000L,   0x1000L,
		0x800L,    0x400L,    0x200L,    0x100L,
		0x80L,     0x40L,     0x20L,     0x10L,
		0x8L,      0x4L,      0x2L,      0x1L        };

	unsigned char DES::pc1[56] = {
		56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
			9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
		62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
		13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11, 3   };

	unsigned char DES::totrot[16] = {
		1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28 };

	unsigned char DES::pc2[48] = {
		13, 16, 10, 23,  0,  4,      2, 27, 14,  5, 20,  9,
		22, 18, 11,  3, 25,  7,     15,  6, 26, 19, 12,  1,
		40, 51, 30, 36, 46, 54,     29, 39, 50, 44, 32, 47,
		43, 48, 38, 55, 33, 52,     45, 41, 49, 35, 28, 31   };

	unsigned long DES::SP1[64] = {
		0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
		0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
		0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
		0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
		0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
		0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
		0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
		0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
		0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
		0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
		0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
		0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
		0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
		0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
		0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
		0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L  };

	unsigned long DES::SP2[64] = {
		0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
		0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
		0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
		0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
		0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
		0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
		0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
		0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
		0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
		0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
		0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
		0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
		0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
		0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
		0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
		0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L  };

	unsigned long DES::SP3[64] = {
		0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
		0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
		0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
		0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
		0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
		0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
		0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
		0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
		0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
		0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
		0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
		0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
		0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
		0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
		0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
		0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L  };

	unsigned long DES::SP4[64] = {
		0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
		0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
		0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
		0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
		0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
		0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
		0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
		0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
		0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
		0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
		0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
		0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
		0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
		0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
		0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
		0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L  };

	unsigned long DES::SP5[64] = {
		0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
		0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
		0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
		0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
		0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
		0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
		0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
		0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
		0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
		0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
		0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
		0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
		0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
		0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
		0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
		0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L  };

	unsigned long DES::SP6[64] = {
		0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
		0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
		0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
		0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
		0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
		0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
		0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
		0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
		0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
		0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
		0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
		0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
		0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
		0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
		0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
		0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L  };

	unsigned long DES::SP7[64] = {
		0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
		0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
		0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
		0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
		0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
		0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
		0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
		0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
		0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
		0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
		0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
		0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
		0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
		0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
		0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
		0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L  };

	unsigned long DES::SP8[64] = {
		0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
		0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
		0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
		0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
		0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
		0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
		0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
		0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
		0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
		0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
		0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
		0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
		0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
		0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
		0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
		0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L  };

	int DES::encrypt(const char key[8], char* data, int blocks, int mode)
	{
		if ((!data)||(blocks<1))
			return 0;
		deskey((unsigned char*)key, ENCRYPT);
		if(mode == 1)
		{
			cbc_enc((unsigned char*)data, (unsigned char*)data, blocks);
		}
		else
		{
			des((unsigned char*)data, (unsigned char*)data, blocks);
		}

		return 1;
	}

	int DES::decrypt (const char key[8], char* data, int blocks, int mode)
	{
		if ((!data)||(blocks<1))
			return 0;
		deskey((unsigned char*)key, DECRYPT);
		if(mode == 1)
		{
			cbc_dec((unsigned char*)data, (unsigned char*)data, blocks);
		}
		else
		{
			des((unsigned char*)data, (unsigned char*)data, blocks);
		}
		return 1;
	}

	// -----------------------------------------------------------------------
	// des
	//      Encrpts/Decrypts(according to the key currently loaded int the
	//      internal key register) SOME blocks of eight bytes at address 'in'
	//      into the block at address 'out'. They can be the same.
	//
	//      "in"
	//      "out"
	//      "block"  Number of blocks.
	// -----------------------------------------------------------------------
	void DES::des ( unsigned char* in, unsigned char* out, int blocks )
	{
		for (int i=0; i<blocks; i++,in+=8,out+=8)
			des_block(in,out);
	}

	void DES::cbc_enc ( unsigned char* in, unsigned char* out, int blocks )
	{
		unsigned char CBCVector[8] = {0};
		//memcpy( CBCVector, key, 8 );

		unsigned char inbuf[8] = {0};
		unsigned char CBCbuf[8] = {0};
		for (int i = 0; i < blocks; i++,in+=8,out+=8)
		{
			memcpy( inbuf, in, 8 );
			for ( int j=0; j<8; j++ )
			{
				CBCbuf[j] = CBCVector[j] ^ inbuf[j];
			}

			des_block( CBCbuf, out );
			memcpy( CBCVector, out, 8 );
		}
	}

	void DES::cbc_dec ( unsigned char* in, unsigned char* out, int blocks )
	{
		unsigned char CBCVector[8] = {0};
		//memcpy( CBCVector, key, 8 );

		unsigned char inbuf[8] = {0};
		unsigned char CBCbuf[8] = {0};
		memcpy( CBCVector, in, 8 );
		des_block(in,out);
		in+=8;
		out+=8;

		for (int i = 1; i < blocks; i++,in+=8,out+=8)
		{
			memcpy( inbuf, in, 8 );
			des_block(in,out);
			for ( int j=0; j<8; j++ )
			{
				CBCbuf[j] = CBCVector[j] ^ out[j];
			}

			memcpy( out, CBCbuf, 8 );
			memcpy( CBCVector, inbuf, 8 );
		}
	}
	
	// -----------------------------------------------------------------------
	// des_block
	//      Encrpts/Decrypts(according to the key currently loaded int the
	//      internal key register) one block of eight bytes at address 'in'
	//      into the block at address 'out'. They can be the same.
	//
	//      "in"
	//      "out"
	// -----------------------------------------------------------------------
	void DES::des_block(unsigned char *in, unsigned char *out)
	{
		unsigned long work[2];

		scrunch(in, work);
		desfunc(work, KnL);
		unscrun(work, out);
	}

	// ----------------------------------------------------------------------
	// deskey
	//       Sets the internal key register (KnR) according to the hexadecimal
	//       key contained in the 8 bytes of hexkey, according to the DES,
	//       for encryption or decrytion according to MODE
	//
	//       "key" is the 64 bits key.
	//       "md"  means encryption or decryption.
	// ----------------------------------------------------------------------
	void DES::deskey(const unsigned char key[8], Mode md)  /* Thanks to James Gillogly &am
														   p; Phil Karn! */
	{
		register int i, j, l, m, n;
		unsigned char pc1m[56], pcr[56];
		unsigned long kn[32];

		for (j = 0; j < 56; j++) {
			l = pc1[j];
			m = l & 07;
			pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1:0;
		}
		for (i = 0; i < 16; i++) {
			if (md == DECRYPT) m = (15 - i) << 1;
			else m = i << 1;
			n = m + 1;
			kn[m] = kn[n] = 0L;
			for (j = 0; j < 28; j++) {
				l = j + totrot[i];
				if (l < 28) pcr[j] = pc1m[l];
				else pcr[j] = pc1m[l - 28];
			}
			for (j = 28; j < 56; j++) {
				l = j + totrot[i];
				if (l < 56) pcr[j] = pc1m[l];
				else pcr[j] = pc1m[l - 28];
			}
			for (j = 0; j < 24; j++) {
				if (pcr[ pc2[j] ]) kn[m] |= bigbyte[j];
				if (pcr[ pc2[j+24] ]) kn[n] |= bigbyte[j];
			}
		}
		cookey(kn);
		return;
	}

	// ----------------------------------------------------------------------
	// cookey
	//       Only called by deskey.
	// -----------------------------------------------------------------------
	void DES::cookey(register unsigned long *raw1)
	{
		register unsigned long *cook, *raw0;
		unsigned long dough[32];
		register int i;

		cook = dough;
		for (i = 0; i < 16; i++, raw1++) {
			raw0 = raw1++;
			*cook = (*raw0 & 0x00fc0000L) << 6;
			*cook |= (*raw0 & 0x00000fc0L) << 10;
			*cook |= (*raw1 & 0x00fc0000L) >> 10;
			*cook++ |= (*raw1 & 0x00000fc0L) >> 6;
			*cook = (*raw0 & 0x0003f000L) << 12;
			*cook |= (*raw0 & 0x0000003fL) << 16;
			*cook |= (*raw1 & 0x0003f000L) >> 4;
			*cook++ |= (*raw1 & 0x0000003fL);
		}
		usekey(dough);
		return;
	}

	// ----------------------------------------------------------------------
	// usekey
	//       Only called by cookey.
	//       Loads the interal key register with the data in cookedkey.
	// -----------------------------------------------------------------------
	void DES::usekey(register unsigned long *from)
	{
		register unsigned long *to, *endp;

		to = KnL, endp = &KnL[32];
		while (to < endp) *to++ = *from++;
		return;
	}

	void DES::scrunch(register unsigned char *outof, register unsigned long *into )
	{
		*into = (*outof++ & 0xffL) << 24;
		*into |= (*outof++ & 0xffL) << 16;
		*into |= (*outof++ & 0xffL) << 8;
		*into++ |= (*outof++ & 0xffL);
		*into = (*outof++ & 0xffL) << 24;
		*into |= (*outof++ & 0xffL) << 16;
		*into |= (*outof++ & 0xffL) << 8;
		*into |= (*outof & 0xffL);
		return;
	}

	void DES::unscrun(register unsigned long *outof, register unsigned char *into)
	{
		*into++ = (unsigned char)((*outof >> 24) & 0xffL);
		*into++ = (unsigned char)((*outof >> 16) & 0xffL);
		*into++ = (unsigned char)((*outof >> 8) & 0xffL);
		*into++ = (unsigned char)(*outof++ & 0xffL);
		*into++ = (unsigned char)((*outof >> 24) & 0xffL);
		*into++ = (unsigned char)((*outof >> 16) & 0xffL);
		*into++ = (unsigned char)((*outof >> 8) & 0xffL);
		*into = (unsigned char)(*outof & 0xffL);
		return;
	}

	void DES::desfunc(register unsigned long *block,register unsigned long *keys)
	{
		register unsigned long fval, work, right, leftt;
		register int round;

		leftt = block[0];
		right = block[1];
		work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
		right ^= work;
		leftt ^= (work << 4);
		work = ((leftt >> 16) ^ right) & 0x0000ffffL;
		right ^= work;
		leftt ^= (work << 16);
		work = ((right >> 2) ^ leftt) & 0x33333333L;
		leftt ^= work;
		right ^= (work << 2);
		work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
		leftt ^= work;
		right ^= (work << 8);
		right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
		work = (leftt ^ right) & 0xaaaaaaaaL;
		leftt ^= work;
		right ^= work;
		leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;

		for (round = 0; round < 8; round++) {
			work = (right << 28) | (right >> 4);
			work ^= *keys++;
			fval  = SP7[work         & 0x3fL];
			fval |= SP5[(work >>  8) & 0x3fL];
			fval |= SP3[(work >> 16) & 0x3fL];
			fval |= SP1[(work >> 24) & 0x3fL];
			work = right ^ *keys++;
			fval |= SP8[work         & 0x3fL];
			fval |= SP6[(work >>  8) & 0x3fL];
			fval |= SP4[(work >> 16) & 0x3fL];
			fval |= SP2[(work >> 24) & 0x3fL];
			leftt ^= fval;
			work = (leftt << 28) | (leftt >> 4);
			work ^= *keys++;
			fval  = SP7[work         & 0x3fL];
			fval |= SP5[(work >>  8) & 0x3fL];
			fval |= SP3[(work >> 16) & 0x3fL];
			fval |= SP1[(work >> 24) & 0x3fL];
			work = leftt ^ *keys++;
			fval |= SP8[work         & 0x3fL];
			fval |= SP6[(work >>  8) & 0x3fL];
			fval |= SP4[(work >> 16) & 0x3fL];
			fval |= SP2[(work >> 24) & 0x3fL];
			right ^= fval;
		}
		right = (right << 31) | (right >> 1);
		work = (leftt ^ right) & 0xaaaaaaaaL;
		leftt ^= work;
		right ^= work;
		leftt = (leftt << 31) | ( leftt >> 1);
		work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
		right ^= work;
		leftt ^= (work << 8);
		work = ((leftt >> 2) ^ right) & 0x33333333L;
		right ^= work;
		leftt ^= (work << 2);
		work = ((right >> 16) ^ leftt) & 0x0000ffffL;
		leftt ^= work;
		right ^= (work << 16);
		work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
		leftt ^= work;
		right ^= (work << 4);
		*block++ = right;
		*block = leftt;
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	void EasyDES::toHex(const std::vector<char>& bin, std::string& hex)
	{
		char tmp[3];
		hex.reserve(bin.size()*2+1);
		for(size_t i=0; i<bin.size(); i++)
		{
			sprintf(tmp, "%02X", (unsigned char)bin[i]);
			hex += tmp;
		}
	}

	void EasyDES::toBin(const char* hex, size_t len, std::vector<char>& bin)
	{
		char tmp[3];
		for(size_t i=0; i<len; i+=2)
		{
			strncpy(tmp, hex+i, 2);
			bin.push_back((unsigned char)strtol(tmp, NULL, 16));
		}
	}

	EasyDES::EasyDES(const char* key)
		: m_key(key)
	{
	}

	void EasyDES::setKey(const char* key)
	{
		m_key = key;
	}

	const char* EasyDES::getKey()
	{
		return m_key.c_str();
	}

	void EasyDES::encrypt(const char* clearText, std::string& encrypted, int outType, int clearTextLen, int mode)
	{
		size_t textLength = (clearTextLen == 0) ? (strlen(clearText)+1) : (clearTextLen+1);
		size_t extendSize = (textLength/8+1)*8;
		size_t additiveSize = extendSize-textLength;
		
		std::vector<char> result(extendSize);
		result.assign(clearText, clearText+textLength-1);
		
		for(size_t i=0; i<additiveSize; ++i)
			result.push_back(0);

		// add the additive size in the end
		// this value will be needed when decrypting
		result.push_back((char)('0'+additiveSize));
		
		int blocks = (int)extendSize/8;
		m_des.encrypt(m_key.c_str(), &result[0], blocks, mode);
		if(outType  == 0)
		{
			toHex(result, encrypted);
		}
		else
		{
			encrypted.assign(result.begin(), result.end());
		}
	}

	void EasyDES::decrypt(const char* code, std::string& clearText, int inType, int codeLen, int mode)
	{
		size_t len = (codeLen == 0) ? strlen(code) : codeLen;
		if(len == 0)
			return ;

		std::vector<char> result;
		if(inType == 0)
		{
			result.reserve(len/2+1);
			toBin(code, len, result);
		}
		else
		{
			result.reserve(len+1);
			result.assign(code, code+len);
		}
		int blocks = (int)(result.size()-1)/8+1;
		if(mode == 1)
		{
			result.resize(blocks*8 + 1);
		}
		else
		{
			result.resize(result.size());
		}

		m_des.decrypt(m_key.c_str(), &result[0], blocks, mode);
		if(mode == 1)
		{
			clearText.assign(result.begin(), result.end());
		}
		else
		{
			size_t additiveSize = result[result.size()-1]-'0'+1;
			if(additiveSize < result.size())
				clearText.assign(result.begin(), result.end()-additiveSize);
		}
	}
}
