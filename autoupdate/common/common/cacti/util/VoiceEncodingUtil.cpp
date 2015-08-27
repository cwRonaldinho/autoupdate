#include "stdafx.h"

#include <cacti/util/VoiceEncodingUtil.h>
#include <cacti/util/FileSystem.h>

#include <iterator>
#include <algorithm>

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

#define new DEBUG_NEW

namespace cacti
{

#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)		/* Number of A-law segments. */
#define	SEG_SHIFT	(4)		/* Left shift for segment number. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */

	static short seg_end[8] = {0xFF, 0x1FF, 0x3FF, 0x7FF,
		0xFFF, 0x1FFF, 0x3FFF, 0x7FFF};

	/* copy from CCITT G.711 specifications */
	unsigned char _u2a[128] = {			/* u- to A-law conversions */
		1,	1,	2,	2,	3,	3,	4,	4,
		5,	5,	6,	6,	7,	7,	8,	8,
		9,	10,	11,	12,	13,	14,	15,	16,
		17,	18,	19,	20,	21,	22,	23,	24,
		25,	27,	29,	31,	33,	34,	35,	36,
		37,	38,	39,	40,	41,	42,	43,	44,
		46,	48,	49,	50,	51,	52,	53,	54,
		55,	56,	57,	58,	59,	60,	61,	62,
		64,	65,	66,	67,	68,	69,	70,	71,
		72,	73,	74,	75,	76,	77,	78,	79,
		81,	82,	83,	84,	85,	86,	87,	88,
		89,	90,	91,	92,	93,	94,	95,	96,
		97,	98,	99,	100,	101,	102,	103,	104,
		105,	106,	107,	108,	109,	110,	111,	112,
		113,	114,	115,	116,	117,	118,	119,	120,
		121,	122,	123,	124,	125,	126,	127,	128
	};

	unsigned char _a2u[128] = 
	{			/* A- to u-law conversions */
		1,	3,	5,	7,	9,	11,	13,	15,
		16,	17,	18,	19,	20,	21,	22,	23,
		24,	25,	26,	27,	28,	29,	30,	31,
		32,	32,	33,	33,	34,	34,	35,	35,
		36,	37,	38,	39,	40,	41,	42,	43,
		44,	45,	46,	47,	48,	48,	49,	49,
		50,	51,	52,	53,	54,	55,	56,	57,
		58,	59,	60,	61,	62,	63,	64,	64,
		65,	66,	67,	68,	69,	70,	71,	72,
		73,	74,	75,	76,	77,	78,	79,	79,
		80,	81,	82,	83,	84,	85,	86,	87,
		88,	89,	90,	91,	92,	93,	94,	95,
		96,	97,	98,	99,	100,	101,	102,	103,
		104,	105,	106,	107,	108,	109,	110,	111,
		112,	113,	114,	115,	116,	117,	118,	119,
		120,	121,	122,	123,	124,	125,	126,	127
	};


	const int ResetValue = 48;

	const std::string formats[3] = {"ADPCM", "Mu-Law", "A-Law"};

	const short mulaw[256] = 
	{
		-32124, -31100, -30076, -29052, -28028, -27004, -25980, -24956,
		-23932, -22908, -21884, -20860, -19836, -18812, -17788, -16764,
		-15996, -15484, -14972, -14460, -13948, -13436, -12924, -12412,
		-11900, -11388, -10876, -10364, -9852, -9340, -8828, -8316,
		-7932, -7676, -7420, -7164, -6908, -6652, -6396, -6140,
		-5884, -5628, -5372, -5116, -4860, -4604, -4348, -4092,
		-3900, -3772, -3644, -3516, -3388, -3260, -3132, -3004,
		-2876, -2748, -2620, -2492, -2364, -2236, -2108, -1980,
		-1884, -1820, -1756, -1692, -1628, -1564, -1500, -1436,
		-1372, -1308, -1244, -1180, -1116, -1052, -988, -924,
		-876, -844, -812, -780, -748, -716, -684, -652,
		-620, -588, -556, -524, -492, -460, -428, -396,
		-372, -356, -340, -324, -308, -292, -276, -260,
		-244, -228, -212, -196, -180, -164, -148, -132,
		-120, -112, -104, -96, -88, -80, -72, -64,
		-56, -48, -40, -32, -24, -16, -8, 0,
		32124, 31100, 30076, 29052, 28028, 27004, 25980, 24956,
		23932, 22908, 21884, 20860, 19836, 18812, 17788, 16764,
		15996, 15484, 14972, 14460, 13948, 13436, 12924, 12412,
		11900, 11388, 10876, 10364, 9852, 9340, 8828, 8316,
		7932, 7676, 7420, 7164, 6908, 6652, 6396, 6140,
		5884, 5628, 5372, 5116, 4860, 4604, 4348, 4092,
		3900, 3772, 3644, 3516, 3388, 3260, 3132, 3004,
		2876, 2748, 2620, 2492, 2364, 2236, 2108, 1980,
		1884, 1820, 1756, 1692, 1628, 1564, 1500, 1436,
		1372, 1308, 1244, 1180, 1116, 1052, 988, 924,
		876, 844, 812, 780, 748, 716, 684, 652,
		620, 588, 556, 524, 492, 460, 428, 396,
		372, 356, 340, 324, 308, 292, 276, 260,
		244, 228, 212, 196, 180, 164, 148, 132,
		120, 112, 104, 96, 88, 80, 72, 64,
		56, 48, 40, 32, 24, 16, 8, 0
	};

	const short alaw[256] = 
	{
		-5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
		-7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
		-2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
		-3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
		-22016, -20992, -24064, -23040, -17920, -16896, -19968, -18944,
		-30208, -29184, -32256, -31232, -26112, -25088, -28160, -27136,
		-11008, -10496, -12032, -11520, -8960, -8448, -9984, -9472,
		-15104, -14592, -16128, -15616, -13056, -12544, -14080, -13568,
		-344, -328, -376, -360, -280, -264, -312, -296,
		-472, -456, -504, -488, -408, -392, -440, -424,
		-88, -72, -120, -104, -24, -8, -56, -40,
		-216, -200, -248, -232, -152, -136, -184, -168,
		-1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
		-1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
		-688, -656, -752, -720, -560, -528, -624, -592,
		-944, -912, -1008, -976, -816, -784, -880, -848,
		5504, 5248, 6016, 5760, 4480, 4224, 4992, 4736,
		7552, 7296, 8064, 7808, 6528, 6272, 7040, 6784,
		2752, 2624, 3008, 2880, 2240, 2112, 2496, 2368,
		3776, 3648, 4032, 3904, 3264, 3136, 3520, 3392,
		22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
		30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
		11008, 10496, 12032, 11520, 8960, 8448, 9984, 9472,
		15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
		344, 328, 376, 360, 280, 264, 312, 296,
		472, 456, 504, 488, 408, 392, 440, 424,
		88, 72, 120, 104, 24, 8, 56, 40,
		216, 200, 248, 232, 152, 136, 184, 168,
		1376, 1312, 1504, 1440, 1120, 1056, 1248, 1184,
		1888, 1824, 2016, 1952, 1632, 1568, 1760, 1696,
		688, 656, 752, 720, 560, 528, 624, 592,
		944, 912, 1008, 976, 816, 784, 880, 848
	};

	const short SSadjust[8] = {-1, -1, -1, -1, 2, 4, 6, 8};

	const short SStable[50] = 
	{
		0, 16, 17, 19, 21, 23, 25, 28, 31,
		34, 37, 41, 45, 50, 55, 60, 66, 73,
		80, 88, 97, 107, 118, 130, 143, 157, 173,
		190, 209, 230, 253, 279, 307, 337, 371, 408,
		449, 494, 544, 598, 658, 724, 796, 876, 963,
		1060, 1166, 1282, 1411, 1552
	};

	/*
	* linear2alaw() - Convert a 16-bit linear PCM value to 8-bit A-law
	*
	* linear2alaw() accepts an 16-bit integer and encodes it as A-law data.
	*
	*		Linear Input Code	Compressed Code
	*	------------------------	---------------
	*	0000000wxyza			000wxyz
	*	0000001wxyza			001wxyz
	*	000001wxyzab			010wxyz
	*	00001wxyzabc			011wxyz
	*	0001wxyzabcd			100wxyz
	*	001wxyzabcde			101wxyz
	*	01wxyzabcdef			110wxyz
	*	1wxyzabcdefg			111wxyz
	*
	* For further information see John C. Bellamy's Digital Telephony, 1982,
	* John Wiley & Sons, pps 98-111 and 472-476.
	*/
	unsigned char VoiceEncodingUtil::linear2alaw(int pcm_val)	/* 2's complement (16-bit range) */
	{
		int		mask;
		int		seg;
		unsigned char	aval;

		if (pcm_val >= 0) {
			mask = 0xD5;		/* sign (7th) bit = 1 */
		} else {
			mask = 0x55;		/* sign bit = 0 */
			pcm_val = -pcm_val - 8;
		}

		/* Convert the scaled magnitude to segment number. */
		seg = search(pcm_val, seg_end, 8);

		/* Combine the sign, segment, and quantization bits. */

		if (seg >= 8)		/* out of range, return maximum value. */
			return (0x7F ^ mask);
		else {
			aval = seg << SEG_SHIFT;
			if (seg < 2)
				aval |= (pcm_val >> 4) & QUANT_MASK;
			else
				aval |= (pcm_val >> (seg + 3)) & QUANT_MASK;
			return (aval ^ mask);
		}
	}

	/*
	* alaw2linear() - Convert an A-law value to 16-bit linear PCM
	*
	*/
	int	VoiceEncodingUtil::alaw2linear(unsigned char a_val)
	{
		int		t;
		int		seg;

		a_val ^= 0x55;

		t = (a_val & QUANT_MASK) << 4;
		seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;
		switch (seg) {
	case 0:
		t += 8;
		break;
	case 1:
		t += 0x108;
		break;
	default:
		t += 0x108;
		t <<= seg - 1;
		}
		return ((a_val & SIGN_BIT) ? t : -t);
	}

#define	BIAS		(0x84)		/* Bias for linear code. */

	/*
	* linear2ulaw() - Convert a linear PCM value to u-law
	*
	* In order to simplify the encoding process, the original linear magnitude
	* is biased by adding 33 which shifts the encoding range from (0 - 8158) to
	* (33 - 8191). The result can be seen in the following encoding table:
	*
	*	Biased Linear Input Code	Compressed Code
	*	------------------------	---------------
	*	00000001wxyza			000wxyz
	*	0000001wxyzab			001wxyz
	*	000001wxyzabc			010wxyz
	*	00001wxyzabcd			011wxyz
	*	0001wxyzabcde			100wxyz
	*	001wxyzabcdef			101wxyz
	*	01wxyzabcdefg			110wxyz
	*	1wxyzabcdefgh			111wxyz
	*
	* Each biased linear code has a leading 1 which identifies the segment
	* number. The value of the segment number is equal to 7 minus the number
	* of leading 0's. The quantization interval is directly available as the
	* four bits wxyz.  * The trailing bits (a - h) are ignored.
	*
	* Ordinarily the complement of the resulting code word is used for
	* transmission, and so the code word is complemented before it is returned.
	*
	* For further information see John C. Bellamy's Digital Telephony, 1982,
	* John Wiley & Sons, pps 98-111 and 472-476.
	*/
	unsigned char VoiceEncodingUtil::linear2ulaw(int pcm_val)	/* 2's complement (16-bit range) */
	{
		int		mask;
		int		seg;
		unsigned char	uval;

		/* Get the sign and the magnitude of the value. */
		if (pcm_val < 0) {
			pcm_val = BIAS - pcm_val;
			mask = 0x7F;
		} else {
			pcm_val += BIAS;
			mask = 0xFF;
		}

		/* Convert the scaled magnitude to segment number. */
		seg = search(pcm_val, seg_end, 8);

		/*
		* Combine the sign, segment, quantization bits;
		* and complement the code word.
		*/
		if (seg >= 8)		/* out of range, return maximum value. */
			return (0x7F ^ mask);
		else {
			uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
			return (uval ^ mask);
		}

	}

	/*
	* ulaw2linear() - Convert a u-law value to 16-bit linear PCM
	*
	* First, a biased linear code is derived from the code word. An unbiased
	* output can then be obtained by subtracting 33 from the biased code.
	*
	* Note that this function expects to be passed the complement of the
	* original code word. This is in keeping with ISDN conventions.
	*/
	int VoiceEncodingUtil::ulaw2linear( unsigned char u_val)
	{
		int		t;

		/* Complement to obtain normal u-law value. */
		u_val = ~u_val;

		/*
		* Extract and bias the quantization bits. Then
		* shift up by the segment number and subtract out the bias.
		*/
		t = ((u_val & QUANT_MASK) << 3) + BIAS;
		t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

		return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
	}

	/* A-law to u-law conversion */
	unsigned char VoiceEncodingUtil::alaw2ulaw(unsigned char aval)
	{
		aval &= 0xff;
		return ((aval & 0x80) ? (0xFF ^ _a2u[aval ^ 0xD5]) :
			(0x7F ^ _a2u[aval ^ 0x55]));
	}

	/* u-law to A-law conversion */
	unsigned char VoiceEncodingUtil::ulaw2alaw( unsigned char uval)
	{
		uval &= 0xff;
		return ((uval & 0x80) ? (0xD5 ^ (_u2a[0xFF ^ uval] - 1)) :
			(0x55 ^ (_u2a[0x7F ^ uval] - 1)));
	}


	signed  char linear_16_8(signed short lval)
	{
		return  lval/256;

	}


	VoiceEncodingUtil::VoiceEncodingUtil(std::string InFileName, std::string OutFileName, int VoxFormat, int VoxRate, int VoxBitsPerSample)
	{
		FInFileName = InFileName;
		FOutFileName = OutFileName;
		FVoxFormat = VoxFormat;
		FVoxRate = VoxRate;
		FVoxBitsPerSample = VoxBitsPerSample;

		if (OutFileName == "")
			FOutFileName = FInFileName+".wav";
		rate = (short)FVoxRate;
		bit_rate = (short)FVoxBitsPerSample;

	}

	short VoiceEncodingUtil::decode(unsigned char sample, short Sn, unsigned short& SS, unsigned short& SSindex)
	{
		//转换公式如下
		//d(n) = (ss(n)*B2)+(ss(n)/2*B1)+(ss(n)/4*BO)+(ss(n)/8)
		//if (B3 = 1)
		//then d(n) = d(n) * (-1)
		//X(n) = X(n-1) + d(n)
		short Mn;
		Mn = 0; // calculate the linear adjustment
		if (sample & 0x4)
			Mn = SS;
		if (sample & 0x2)
			Mn += SS >> 1; //div 2
		if (sample & 0x1)
			Mn += SS >> 2; //div 4
		Mn += SS >> 3; //div 8
		//取Sample的符号位，即最高位
		if (sample & 0x8) //最高位为1，则符号位为负
			Sn -= Mn; // ...subtract the adjustment
		else // 符号位为正
			Sn += Mn; // ...add the adjustment

		if (Sn > 2047) // adjust if sample too large...
			Sn = 2047;
		if (Sn < -2048) // ...or too small
			Sn = -2048;

		// use as index into step size adjustment, adjust step size index
		SSindex += SSadjust[sample & 0x7];

		if (SSindex < 1)  // keep SSindex within bounds...
			SSindex = 1;
		if (SSindex > 49) 
			SSindex = 49;

		SS = SStable[SSindex]; // get new step size from table

		return Sn;
	}


	void VoiceEncodingUtil::ConvertADPCM2PCM(FILE* infile, FILE* outfile)
	{
		int index; // index for misc. arrays
		int bytes; // number of bytes read into a buffer
		int outindex; //index for output arrays
		unsigned char outbytebuffer[20000]; //output buffer
		short outintbuffer[20000]; //output buffer
		short i;//Reset Count
		unsigned char HighByte;
		unsigned char LowByte;

		if (infile == NULL || outfile == NULL)
			return;

		Sn = 0; // initialize the ADPCM variables
		SS = 16; // initialize the Step
		SSindex = 1;
		i = 0;
		// read a byte of ADPCM data
		bytes = fread(buffer,1,10000,infile);
		while (bytes > 0) 
		{
			outindex = 0;
			for (index = 0; index <bytes; index++) 
			{
				sample = buffer[index];
				HighByte = sample >> 4;
				LowByte = sample & 0xF;
				if (HighByte ==0 || HighByte==8) 
					i++;
				// 先解压缩一个字节中的高4位...
				Sn = decode(sample >> 4, Sn, SS, SSindex);
				if (bit_rate == 1)  // if 8 bits per sample...
				{
					out_int = Sn >> 4; // rescale output from -128 thru 127
					if (out_int > 127)  // clip if above or below WAV bounds
						out_int = 127;
					if (out_int < -128) 
						out_int = -128;
					out_val = (unsigned char)(out_int - 128); // convert to .WAV format
					outbytebuffer[outindex++] = out_val; // write the output byte
				}
				else
				{
					out_int = Sn << 4; // rescale to 16 bits
					outintbuffer[outindex++] = out_int; // write the output int
				};
				if (i == ResetValue)   //Reset ADPCM variables
				{
					Sn = 0; // initialize the ADPCM variables
					SS = 16; // initialize the Step
					i = 0;
				};
				if (LowByte==0 || LowByte==8)
					i++;
				// now decode the low nibble...
				Sn = decode(sample & 0xF, Sn, SS, SSindex);
				if (bit_rate == 1)  // if 8 bits per sample...
				{
					out_int = Sn >> 4; // rescale output from -128 thru 127

					if (out_int > 127)  // clip if above or below WAV bounds
						out_int = 127;
					if (out_int < -128) 
						out_int = -128;

					out_val = out_int - 128; // convert to .WAV format
					outbytebuffer[outindex++] = out_val; // write the output byte
				}
				else
				{
					out_int = Sn << 4; // rescale to 16 bits
					outintbuffer[outindex++] = out_int; // write the output int
				};
				if (i == ResetValue)   //Reset ADPCM variables
				{
					Sn = 0; // initialize the ADPCM variables
					SS = 16; // initialize the Step
					i = 0;
				};
			};
			if (bit_rate == 1) 
				fwrite(outbytebuffer,1, bytes * 2,outfile);
			else
				fwrite(outintbuffer,1, bytes * 4,outfile);
			bytes = fread(buffer,1, 10000,infile);
		}
	}


	void VoiceEncodingUtil::ConvertMULAW2PCM(FILE* infile, FILE* outfile)
	{

		int index; // index for misc. arrays
		int bytes; // number of bytes read into a buffer
		int outindex; //index for output arrays
		unsigned char outbytebuffer[20000]; //output buffer
		short outintbuffer[20000]; //output buffer
		// read a block of mu-law data
		bytes = fread(buffer,1, 10000,infile);
		while (bytes > 0) 
		{
			outindex = 0;
			for (index = 0; index <bytes - 1; index++) 
			{
				out_int = mulaw[buffer[index]]; // convert mu-law to linear

				if (bit_rate = 1)  // if 8 bits per sample...
				{
					out_int = out_int >> 8; // rescale for 8 bit values
					out_val = out_int + 128; // convert to .WAV format
					outbytebuffer[outindex++] = out_val; // write the output byte
				}
				else
				{
					outintbuffer[outindex++] = out_int; // write the output int
				};
			};
			if (bit_rate = 1) 
				fwrite(outbytebuffer, 1, bytes * 2,outfile);
			else
				fwrite(outintbuffer, 1, bytes * 4,outfile);
			bytes = fread(buffer,1, 10000,infile);
		};
	}

	void VoiceEncodingUtil::ConvertALAW2PCM(FILE* infile, FILE* outfile)
	{

		int index; // index for misc. arrays
		int bytes; // number of bytes read into a buffer
		int outindex; //index for output arrays
		unsigned char outbytebuffer[20000]; //output buffer
		short outintbuffer[20000]; //output buffer
		// read a block of mu-law data
		bytes = fread(buffer,1, 10000,infile);
		while (bytes > 0) 
		{
			outindex = 0;
			for (index = 0; index <bytes - 1; index++) 
			{
				out_int = alaw[buffer[index]]; // convert mu-law to linear

				if (bit_rate = 1)  // if 8 bits per sample...
				{
					out_int = out_int >> 8; // rescale for 8 bit values
					out_val = out_int + 128; // convert to .WAV format
					outbytebuffer[outindex++] = out_val; // write the output byte
				}
				else
				{
					outintbuffer[outindex++] = out_int; // write the output int
				};
			};
			if (bit_rate = 1) 
				fwrite(outbytebuffer, 1, bytes * 2,outfile);
			else
				fwrite(outintbuffer, 1, bytes * 4,outfile);
			bytes = fread(buffer,1, 10000,infile);
		};
	};

/*
	int fileSize(const char* file)
	{
		struct stat stbuf;
		if (stat(file, &stbuf) == -1)
			return 0;
		else
			return stbuf.st_size;
	}*/


	short VoiceEncodingUtil::Convert()
	{
		int filesize = 0;
		FILE* infile = NULL;
		FILE* outfile = NULL;
		TWaveHead WaveHead;
		TDataHead DataHead;
		infile = fopen(FInFileName.c_str(), "rb");
		if (infile == NULL)  //If can't create Input File...
			return -2;
		outfile = fopen(FOutFileName.c_str(), "wb");
		if (outfile == NULL) // If can't create Output File....
		{
			fclose(infile); //close Input File
			return -3;
		};
		//filesize = fseek(infile, 0, SEEK_END); //get size of input file
		filesize = cacti::FileSystem::fileSize(FInFileName.c_str());
		if (FVoxFormat == VF_ADPCM)  // if using ADPCM input format...
			filesize = filesize * 2; // change from bytes to samples
		fseek(infile, 0, SEEK_SET); // seek back to beginning of input
		//---------Create Wave File Head ---------------------------------------------
		memcpy(WaveHead.cHead, "RIFF", sizeof(WaveHead.cHead)); // .WAV begins with "RIFF"
		WaveHead.nLength = (filesize * bit_rate) + sizeof(TWaveHead); // size of .WAV file (data+header)
		memcpy(WaveHead.cWaveTag, "WAVEfmt ", sizeof(WaveHead.cWaveTag));
		WaveHead.nHeaderLength = 18; // size of .WAV file header
		WaveHead.wFormatTag = 1; // format tag (01 = Windows PCM)
		WaveHead.nChannels = 1; // channels (1=mono, 2=stereo)
		WaveHead.nSamplesPerSec = rate; // samples per second
		WaveHead.nAvgBytesPerSec = rate * bit_rate; // bytes per second during play
		WaveHead.nBlockAlign = bit_rate; // bytes per sample
		WaveHead.wBitsPerSample = 8 * bit_rate; // bits per sample
		memcpy(DataHead.cDataTag, "data", sizeof(DataHead.cDataTag)); // specify "data" follows
		DataHead.nDatalen = filesize * bit_rate; // write size of .WAV data portion
		fwrite(&WaveHead, 1, sizeof(TWaveHead), outfile);
		fwrite("\0\0", 1, 2, outfile);
		fwrite(&DataHead, 1, sizeof(TDataHead),outfile);
		switch (FVoxFormat)
		{
		case VF_ADPCM: 
			ConvertADPCM2PCM(infile, outfile);
			break;
		case VF_MULAW: 
			ConvertMULAW2PCM(infile, outfile);
			break;

		case VF_ALAW: 
			ConvertALAW2PCM(infile, outfile);
			break;
		default:
			break;
		}


		fclose(infile);
		fclose(outfile);
		return 0;


	}

	int VoiceEncodingUtil::pcm2aulaw(const std::string& pcmFile, const std::string& newFile, bool isAlaw)
	{
		FILE* fp1 = fopen(pcmFile.c_str(), "rb");
		if (fp1 == NULL)
			return -1;

		FILE* fp2 = fopen(newFile.c_str(), "wb");
		if (fp2 == NULL)
		{
			fclose(fp1);
			return -2;
		}

		char buff[1024] = {0};
		char writeBuff[512] = {0};
		u32 size = 0;
		u32 totalSize = 0;


		TWaveHead pcmFileHead;
		TDataHead pcmDataHead;

		size = fread(&pcmFileHead, 1, sizeof(TWaveHead), fp1);//wav文件头
		if (pcmFileHead.nHeaderLength > 0x10)
		{
			size = fread(buff, 1, pcmFileHead.nHeaderLength-0x10, fp1);//wav文件头
			memset(buff,0,1024);
		}
		
		size = fread(&pcmDataHead, 1, sizeof(TDataHead), fp1);//wav文件头


		int newFileDataSize = pcmDataHead.nDatalen/2;
		if (newFileDataSize <0)
			newFileDataSize = 0;

		TWaveHead newFileHead = pcmFileHead;
		newFileHead.nLength = newFileDataSize + 0x32;
		newFileHead.nHeaderLength = 0x12;
		newFileHead.wFormatTag = isAlaw?6:7; // format tag (01 = Windows PCM)
		newFileHead.nChannels = 1; // channels (1=mono, 2=stereo)
		newFileHead.nSamplesPerSec = 8000; // samples per second
		newFileHead.nAvgBytesPerSec = 8000; // bytes per second during play
		newFileHead.nBlockAlign = 1; // bytes per sample
		newFileHead.wBitsPerSample = 8; // bits per sample

		totalSize+= fwrite(&newFileHead, 1, sizeof(TWaveHead), fp2);
		totalSize+= fwrite("\0\0", 1, 2, fp2);

		TDataHeadEx DataHead;
		memcpy(DataHead.fact,"fact",sizeof(DataHead.fact));
		DataHead.unknownInt = 4;//暂时不明是啥东西
		DataHead.nDatalen = newFileDataSize;
		memcpy(DataHead.cDataTag,"data",sizeof(DataHead.cDataTag));
		totalSize+= fwrite(&DataHead, 1, sizeof(TDataHeadEx), fp2);



		int convertSize = 0;
		while (!feof(fp1) && convertSize<pcmDataHead.nDatalen)
		{
			memset(buff,0,1024);
			memset(writeBuff,0,512);

			if (pcmDataHead.nDatalen - convertSize >1000)
				size = fread(buff, 1, 1000, fp1);
			else
				size = fread(buff, 1, pcmDataHead.nDatalen - convertSize, fp1);

			convertSize +=size;

			for (u32 i=0; i<(size/2); i++)
			{
				if (isAlaw)
					writeBuff[i] = linear2alaw(buff[i*2+1]*256+buff[i*2]);
				else
					writeBuff[i] = linear2ulaw(buff[i*2+1]*256+buff[i*2]);
			}
			totalSize+= fwrite(writeBuff, 1, size/2, fp2);
		}

		fclose(fp1);
		fclose(fp2);
		return totalSize;
	}

}


