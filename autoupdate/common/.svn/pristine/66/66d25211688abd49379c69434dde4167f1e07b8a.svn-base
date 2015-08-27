#ifndef INCLUDED_CACTI_UTIL_DES_H
#define INCLUDED_CACTI_UTIL_DES_H

#include "stdio.h"

#include <string>
#include <vector>

namespace cacti {
	class DES 
	{ 
	public: 
		// Encrypt/decrypt the data in "data", according to the "key". 
		// Caller is responsible for confirming the buffer size of "data" 
		// points to is 8*"blocks" bytes.
		// The data encrypted/decrypted is stored in data. 
		// The return code is 1:success, other:failed. 

		int encrypt(const char key[8], char* data, int blocks = 1, int mode=0); 
		int decrypt(const char key[8], char* data, int blocks = 1, int mode=0);

		int extend(int size) {return (size/8+1)*8;};  

	private: 
		void des(unsigned char* in, unsigned char* out, int blocks); 
		void des_block(unsigned char* in, unsigned char* out);  

		void cbc_enc(unsigned char* in, unsigned char* out, int blocks); 
		void cbc_dec(unsigned char* in, unsigned char* out, int blocks);
		
	private: 
		enum Mode { ENCRYPT, DECRYPT }; 

		unsigned long KnL[32];
		void deskey(const unsigned char key[8], Mode md); 
		void usekey(unsigned long *); 
		void cookey(unsigned long *);  

	private: 
		void scrunch(unsigned char *, unsigned long *); 
		void unscrun(unsigned long *, unsigned char *); 
		void desfunc(unsigned long *, unsigned long *);  

	private: 
		static unsigned char Df_Key[24]; 
		static unsigned short bytebit[8]; 
		static unsigned long bigbyte[24]; 
		static unsigned char pc1[56]; 
		static unsigned char totrot[16]; 
		static unsigned char pc2[48]; 
		static unsigned long SP1[64]; 
		static unsigned long SP2[64]; 
		static unsigned long SP3[64]; 
		static unsigned long SP4[64]; 
		static unsigned long SP5[64]; 
		static unsigned long SP6[64]; 
		static unsigned long SP7[64]; 
		static unsigned long SP8[64];  
	};


	class EasyDES
	{
	public:
		EasyDES(const char* key);

		void encrypt(const char* clearText, std::string& encrypted, int outType=0, int clearTextLen=0, int mode=0);
		void decrypt(const char* code, std::string& clearText, int inType=0, int codeLen=0, int mode=0);
		void setKey(const char* key);
		const char* getKey();

	private:
		void toHex(const std::vector<char>& bin, std::string& hex);
		void toBin(const char* hex, size_t len, std::vector<char>& bin);

	private:
		DES m_des;
		std::string m_key;
	};

}

#endif	// INCLUDED_CACTI_UTIL_DES_H
