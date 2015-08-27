#ifndef INCLUDED_CACTI_UTILVOICEENCODING_UTIL_H
#define INCLUDED_CACTI_UTILVOICEENCODING_UTIL_H

#include <string>
#include <vector>
#include <list>
#include <climits>
#include <stdarg.h>

#include "cacti/message/TypeDef.h"
#include "cacti/message/ServiceIdentifier.h"

namespace cacti
{
	class VoiceEncodingUtil 
	{
	public:

		static unsigned char linear2alaw(int pcm_val);	/* 2's complement (16-bit range) */
		static int alaw2linear(unsigned char	a_val);

		static unsigned char linear2ulaw(int pcm_val);	/* 2's complement (16-bit range) */
		static int ulaw2linear(unsigned char	u_val);

		static unsigned char alaw2ulaw(unsigned char	aval);
		static unsigned char ulaw2alaw(unsigned char	uval);	
		static signed char linear_16_8(signed short	lval);	

		static int pcm2alaw(const std::string& pcmFile, const std::string& alawFile)
		{
			return pcm2aulaw(pcmFile, alawFile, true);
		}
		static int pcm2mulaw(const std::string& pcmFile, const std::string& alawFile)
		{
			return pcm2aulaw(pcmFile, alawFile, false);
		}

		VoiceEncodingUtil(std::string InFileName, std::string OutFileName="", int VoxFormat = VF_ADPCM,
			int VoxRate= VR_6K, int VoxBitsPerSample = VB_8);
		~VoiceEncodingUtil();
		short Convert();


	private:
		static const int VF_ADPCM = 1;
		static const int VF_MULAW = 2; 
		static const int VF_ALAW = 3;

		static const int VR_6K = 6000;
		static const int VR_8K = 8000;

		static const int VB_8 = 1;
		static const int VB_16 = 2;


#pragma pack(push) //保存对齐状态

#pragma pack(1)//设定为4字节对齐

		struct TWaveHead
		{
			char cHead[4];
			int nLength;
			char cWaveTag[8];
			int nHeaderLength;
			unsigned short wFormatTag;
			unsigned short nChannels;
			int nSamplesPerSec;
			int nAvgBytesPerSec;
			unsigned short nBlockAlign;
			unsigned short wBitsPerSample;

		};


		struct TDataHead
		{
			char cDataTag[4];
			int nDatalen;
		};
		struct TDataHeadEx
		{
			char fact[4];
			int unknownInt;
			int nDatalen;
			char cDataTag[4];
		};
#pragma pack(pop) //保存对齐状态


		static int	search(	int	val,short *table,int size)	
		{
			for ( int i = 0; i < size; i++) {
				if (val <= *table++)
					return (i);
			}
			return (size);
		};

		std::string	FInFileName;
		std::string	FOutFileName;
		int		FVoxFormat;
		int		FVoxRate;
		int		FVoxBitsPerSample;
		short	bit_rate;
		short	rate;
		unsigned char	sample;
		unsigned char	buffer[10000];

		unsigned short	SS;
		unsigned short	SSindex;
		short	Sn;
		unsigned char	out_val;
		short	out_int;

		short	decode(unsigned char sample, short Sn, unsigned short& SS, unsigned short& SSindex);
		void	ConvertADPCM2PCM(FILE* infile, FILE* outfile);
		void	ConvertMULAW2PCM(FILE* infile, FILE* outfile);
		void	ConvertALAW2PCM(FILE* infile, FILE* outfile);

		static int pcm2aulaw(const std::string& pcmFile, const std::string& newFile, bool isAlaw);





	};

};

#endif // INCLUDED_CACTI_UTILVOICEENCODING_UTIL_H
