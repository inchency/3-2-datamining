#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <atlconv.h> // EUC-KR->UTF-8 º¯È¯ÇÒ¶§ ¾¸
#include <string>

#define FILE_NAME "949.txt"
#define MAX_SIZE 1000
using namespace std;

void readchar(ifstream &); // input.txt ÀĞ´Â ÇÔ¼ö
void convertBin(int w); // 10Áø¼ö¸¦ ÀÌ¿ëÇØ 2Áø¼ö·Î º¯È¯ÇØÁÖ´Â ÇÔ¼ö
void printBin(int *w, int len);
void printOct(char *w, int len); // 8Áø¼ö·Î Ãâ·Â
void printDec(char *w, int len); // 10Áø¼ö·Î Ãâ·Â
void printHex(char *w, int len); // 16Áø¼ö·Î Ãâ·Â
string compareEUCKR(char *w, int len); // Ã³À½¿¡ EUC-KRÀÎÁö ¾Æ´ÑÁö È®ÀÎÇØÁÖ´Â ÇÔ¼ö
bool checkOnlyOne(char *w, int len); // ÇÑ±ÛÀÚÀÎÁö ¿©·¯±ÛÀÚÀÎÁö ÆÇº°ÇØÁÖ´Â ÇÔ¼ö
string compareOnlyOne(char *w, int len); // ¹®ÀÚ¿­À» ÀÎÀÚ·Î Àü´ŞÇÏ¿© ±× ¹®ÀÚ¿­ÀÇ ÇÑ±ÛÄÚµå°¡ ¹«¾ùÀÎÁö ¹ÙÀÌÆ®¸¦ °è»êÇØ ÀÚµ¿À¸·Î ÀÎ½ÄÇÏ´Â ÇÔ¼ö (´Ü ÇÑ±ÛÀÚ ÀÏ¶§¸¸ °¡´É)
void compare(char *w, int len); // ¾î¶°ÇÑ ÇÑ±ÛÄÚµåÀÎÁö ÆÇº° (2±ÛÀÚÀÌ»ó °¡´É, BOM »ç¿ë)
void convert8BintoUniBin(int *w, int len); // UTF-8 2Áø¼ö -> Unicode 2Áø¼ö·Î º¯È¯ÇØÁÖ´Â ÇÔ¼ö
void convertUniBintoDec(int *w, int len); // Unicode 2Áø¼ö -> 10Áø¼ö·Î º¯È¯ÇØÁÖ´Â ÇÔ¼ö (= UTF-16BE)
void convert(char *w, int len); // UTF16BE°¡ ¾Æ´Ò¶§ º¯È¯ÇØÁÖ´Â ÇÔ¼ö
void convertLEtoBE(char *w, int len); // UTF16LE->BE º¯È¯ ÇÔ¼ö
void convertEUCto8(int *in, int nOut); // EUC-KR->UTF-8 º¯È¯ ÇÔ¼ö
bool checkNoHanGul(char *w, int len); // ÇÑ±ÛÀÌ ¾Æ´Ñ ¹®ÀÚ°¡ ÇÏ³ª¶óµµ µé¾î°¡ ÀÖÀ¸¸é false ¹İÈ¯
void homeWork4(); // °úÁ¦(4¹ø) ÇÔ¼ö (UTF-16(BE)·Î µÈ ÇÑ±Û ¹®ÀÚ¿­À» ÃÊ¼º(0~18), Áß¼º(0~20), Á¾¼º(0~27)À¸·Î ºĞ¸®ÇÏ´Â ÇÔ¼ö ÀÛ¼º.)
void homeWork5(class Jamo *jamo); // °úÁ¦(5¹ø) À§ 4¹øÀÇ ±¸Á¶Ã¼ ¹è¿­ ³»¿ëÀ» UTF - 16(BE)ÇÑ±Û ½ºÆ®¸µÀ¸·Î Á¶ÇÕÇÏ´Â ÇÔ¼ö ÀÛ¼º.
void homeWork6(); // °úÁ¦(6¹ø) À§ ÇÔ¼öµéÀ» ÀÌ¿ëÇÏ¿© ÀÓÀÇÀÇ ÇÑ±ÛÄÚµå·Î ÀÛ¼ºµÈ ÇÑ±Û ÅØ½ºÆ® ÆÄÀÏÀÇ ³»¿ëÀ» ÃÊ/Áß/Á¾¼ºÀ¸·Î Ç®¾î¾´ ÇüÅÂ·Î Ãâ·Â ¹× ÃÊ¼ºµé¸¸ Ãâ·ÂÇÏ´Â ÇÁ·Î±×·¥ ÀÛ¼º.
void HW2(); // [°úÁ¦ 2] °úÁ¦ 1¿¡¼­ ±¸ÇöÇÑ "ÇÑ±Û¹®Àå Ç®¾î¾²±â" ±â´É ¼öÁ¤-º¸¿Ï

			// ¹Ø¿¡´Â ÆíÇÏ°Ô »ç¿ëÇÏ±âÀ§ÇØ Àü¿ªº¯¼ö·Î ¼±¾ğ
char buffer[MAX_SIZE] = { NULL }; // ¹®ÀÚ¿­À» ¹Ş¾Æ buffer ¹è¿­¿¡ ÀúÀå
int bufferNew[MAX_SIZE] = { NULL }; // buffer[i] - 0xffffff00; ÇÒ°ÅÀÓ
int bufferReal[MAX_SIZE] = { NULL }; // 8±ÛÀÚ¸¦ ¹è¿­1Ä­¿¡ ³ÖÀ» ÁøÂ¥ 2Áø¼ö¸¦ ÀúÀåÇÒ ¹è¿­
int bufferUniDec[MAX_SIZE] = { NULL }; // À¯´Ï 10Áø¼ö ÀúÀåÇÏ´Â ¹è¿­
int bufferUniDecLen = 0;
int buffer16BE[MAX_SIZE] = { NULL }; // º¯È¯µÈ UTF-16BE ´ã´Â ¹è¿­
int buffer16BELen = 0;
int buffer8[MAX_SIZE] = { NULL }; // º¯È¯µÈ UTF-8 ´ã´Â ¹è¿­
int bufferLen; // bufferÀÇ ±æÀÌ
bool EUC_KR;
int bufferBin[MAX_SIZE] = { NULL };
int bufferBinLen; // 2Áø¼öbufferÀÇ ±æÀÌ
int uniBin[MAX_SIZE] = { NULL }; // À¯´ÏÄÚµå 2Áø¼ö·Î³ªÅ¸³½ ¹è¿­
int iBin = 0;
int start = 0; // convert8BintoUniBin¿¡¼­ ¾²ÀÌ¸ç convertUniBintoDec¿¡¼­ ¹è¿­ÀÇ ±æÀÌ·Î¾²ÀÓ
bool flag = false; // °úÁ¦4 ÇÔ¼ö¿¡¼­ °úÁ¦6 ÇÔ¼ö°¡ ¾²ÀÎ »óÅÂ¶ó¸é (ÃÊ¼º¸¸ + (ÃÊ¼º,Áß¼º,Á¾¼º)) printf¹®À» Ãâ·ÂÇÏ°í ¾È¾²ÀÎ »óÅÂ¶ó¸é Ãâ·Â °á°ú°ªÀÌ ¼ıÀÚ·Î¸¸ ³ª¿À°Ô²û ÇÔ
string isWhat; // ¾î¶°ÇÑ ÇÑ±ÛÄÚµåÀÎÁö ³ªÅ¸³»´Â º¯¼ö
string UTF8 = "ÀÌ ¹®ÀÚ¿­ ÇÑ±ÛÄÚµå´Â UTF-8 ÀÔ´Ï´Ù\n";
string UTF16be = "ÀÌ ¹®ÀÚ¿­ ÇÑ±ÛÄÚµå´Â UTF-16(BE)  ÀÔ´Ï´Ù\n";
string UTF16le = "ÀÌ ¹®ÀÚ¿­ ÇÑ±ÛÄÚµå´Â UTF-16(LE)  ÀÔ´Ï´Ù\n";
string EUCKR = "ÀÌ ¹®ÀÚ¿­ ÇÑ±ÛÄÚµå´Â EUC-KR(¶Ç´Â CP949) ÀÔ´Ï´Ù\n";
bool check16 = false; // ÀÔ·Â¹Ş´Â °ªÀÌ utf-16Á¾·ùÀÌ¸é true ¾Æ´Ï¸é false
bool check16check = true; // check16 ÆÇº°ÇÏ±âÀ§ÇØ ½áÁÜ
char userBuffer[MAX_SIZE] = { NULL };
int userBufferLen;
char dicBuffer[MAX_SIZE] = { NULL };
int dicBufferLen;
bool userFirst = true;
int countPrint = 0;

class Jamo {
public:
	int first;
	int middle;
	int end;
};
class userJamo {
public:
	int first;
	int middle;
	int end;
};

class dicJamo {
public:
	int first;
	int middle;
	int end;
};

userJamo userjamo[MAX_SIZE];
dicJamo dicjamo[MAX_SIZE];

int main() {
	ifstream inFile;
	readchar(inFile);
	for (int i = 0; i < bufferLen; i++)
		bufferNew[i] = buffer[i] - 0xffffff00;

	//cout << endl;
	//printHex(buffer, bufferLen);
	//cout << compareEUCKR(buffer, bufferLen) << endl;
	compareEUCKR(buffer, bufferLen);
	if (!EUC_KR) { // EUC-KRÀÌ ¾Æ´Ï¶ó¸é
		if (checkOnlyOne(buffer, bufferLen))
			cout << compareOnlyOne(buffer, bufferLen);
		else {
			compare(buffer, bufferLen);
			if (isWhat == "EUC-KR");
			//cout << EUCKR;
			else if (isWhat == "UTF-16LE")
				cout << UTF16le;
			else if (isWhat == "UTF-16BE")
				cout << UTF16be;
			else if (isWhat == "UTF-8")
				cout << UTF8;
		}
	}
	//if(!checkNoHanGul(buffer, bufferLen)) { // ÇÑ±ÛÀÌ ¾Æ´Ñ ¹®ÀÚ°¡ ÇÏ³ª¶óµµ µé¾î°¡ ÀÖÀ¸¸é
	//	cout << "ÀÔ·Â¹ŞÀº ¹®ÀÚ¿­ÀÌ ÇÑ±ÛÀÌ ¾Æ´Ñ°ÍÀÌ ÀÖÀ¸¹Ç·Î UTF-16BE·Î º¯È¯À» ÇÏÁö ¾Ê°í Á¾·áÇÕ´Ï´Ù." << endl;
	//	return 0;
	//}
	compare(buffer, bufferLen);
	if (isWhat != "UTF-16BE")  // UTF-16BE°¡ ¾Æ´Ï¶ó¸é UTF-16BE·Î º¯È¯½ÃÄÑÁà¾ßÇÔ
		convert(buffer, bufferLen);
	cout << endl;
	// °úÁ¦ (4¹ø) ÇÔ¼ö È£ÃâºÎºĞ
	homeWork4(); // °úÁ¦(5¹øÀº homeWork4() ÇÔ¼ö¾È¿¡ È£Ãâ ÇÏ´Â °÷ÀÌ ÀÖ½À´Ï´Ù.)
	homeWork6();
	HW2();
	cout << " [°úÁ¦ 2] °úÁ¦ 1¿¡¼­ ±¸ÇöÇÑ ÇÑ±Û¹®Àå Ç®¾î¾²±â ±â´É ¼öÁ¤-º¸¿Ï ½ÃÀÛ\n";
	cout << "»çÀü°ú À¯»çÇÑ ´Ü¾î°¡ ¾ø½À´Ï´Ù." << endl;
	return 0;
}
void readchar(ifstream & instream) {
	instream.open(FILE_NAME);
	if (!instream.is_open()) {
		cout << "input.txt¸¦ ¿­ ¼ö ¾ø½À´Ï´Ù." << endl;
		return;
	}
	//instream.getline(buffer, 100);
	int z = 0;
	while (instream >> buffer[z]) {
		if (buffer[z] == 0)
			continue;
		z++;
	}
	instream.close();
	bufferLen = strlen(buffer);
	cout << "ÀÔ·Â¹ŞÀº ¹®ÀÚ¿­ :   " << endl << buffer << endl;
}

void convertBin(int w) {
	if (w == 0)
		return;
	convertBin(w / 2);
	bufferBin[iBin++] = w % 2;
	bufferBinLen++;
	//printf("%d", w % 2);
}

void printBin(int *w, int len) {
	cout << "À§¿¡¼­ ÀÔ·Â¹ŞÀº ¹®ÀÚ¿­À» 2Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù." << endl;
	for (int i = 0; i < bufferBinLen; i++) {
		if (i % 8 == 0 && i != 0) // 2Áø¼ö ÀÌ»Ú°Ô Ãâ·Â ÇÏ±âÀ§ÇÑ if¹® 8ºñÆ®¸¶´Ù ¶ç¾î¼­ ±¸ºĞ
			cout << " ";
		printf("%d", w[i]);
	}
	cout << endl;
}
void printOct(char *w, int len) {
	cout << "À§ ¹®ÀÚ¿­À» 8Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù." << endl;
	for (int i = 0; i<len; i++)
		printf("O%o ", w[i]);
	cout << endl;
}
void printDec(char *w, int len) {
	cout << "À§ ¹®ÀÚ¿­À» 10Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù." << endl;
	for (int i = 0; i<len; i++)
		printf("%d ", w[i]);
	cout << endl;
}
void printHex(char *w, int len) {
	cout << "À§ ¹®ÀÚ¿­À» 16Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù." << endl;
	for (int i = 0; i<len; i++)
		printf("0x%x ", w[i]);
	cout << endl;
}

string compareOnlyOne(char *w, int len) {
	// ÀÌÁ¦ UTF8ÀÎÁö UTF16ÀÎÁö EUC-KR ÀÎÁö ÆÇº°À» ÇØÁØ´Ù.
	// ¾Æ·¡´Â "°¢"À» °¢°¢ ¸Ş¸ğÀå¿¡¼­ Á¾·ùº°·Î ÀúÀåÈÄ ½ÇÇà½ÃÄÑ º» °á°úÀÌ´Ù.
	/*	utf - 8
	ï»¿ê°
	À§ ¹®ÀÚ¿­À» 16Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù.
	0xffffffef 0xffffffbb 0xffffffbf 0xffffffea 0xffffffb0 0xffffff81

	utf - 16be
	? 
	À§ ¹®ÀÚ¿­À» 16Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù.
	0xfffffffe 0xffffffff 0xffffffac 0x1

	utf - 16le
	ÿ
	À§ ¹®ÀÚ¿­À» 16Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù.
	0xffffffff 0xfffffffe 0x1 0xffffffac


	EUC-KR
	°¢
	À§ ¹®ÀÚ¿­À» 16Áø¼ö·Î ³ªÅ¸³ª°Ú½À´Ï´Ù.
	0xffffffb0 0xffffffa2
	*/
	// À§¿¡¼­ utf-8Àº 16Áø¼ö·Î Ãâ·ÂÇÏ¿´À» ¶§ 6¹ÙÀÌÆ®°¡ ³ª¿Ô°í (BOM3¹ÙÀÌÆ®) 3+3n (nÀº ±ÛÀÚ°¹¼ö)
	// utf-16BE, utf-16LE´Â 4¹ÙÀÌÆ® ±×¸®°í EUC-KRÀº 2¹ÙÀÌÆ®°¡ ³ª¿Ô´Ù (BOM2¹ÙÀÌÆ®) 2+2n, EUC-KRÀº BOM¾øÀ½ 2n
	// ¸ÕÀú ¹ÙÀÌÆ® ¼ö·Î utf-8°ú utf-16, EUC-KRÀ» ±¸ºĞÇØ ÁÖ°í
	// ±×´ÙÀ½ utf-16¿¡¼­ÀÇ LE¿Í BE´Â ¾Õ¿¡ Ã¹ ¹ÙÀÌÆ® fe·Î ³¡³ª´ÂÁö ff·Î ³¡³ª´ÂÁö·Î ºñ±³¸¦ ÇØÁÖ°Ú´Ù.
	if (len == 2) // "°¢" -> 0xffffffb0 0xffffffa2 = EUC-KR
		return EUCKR;
	else if (len == 6) // "°¢" -> 0xffffffef 0xffffffbb 0xffffffbf 0xffffffea 0xffffffb0 0xffffff81 = UTF-8
		return UTF8;
	else if (len == 4) { // "°¢" -> 0xfffffffe 0xffffffff 0xffffffac 0x1 = UTF-16BE , 0xffffffff 0xfffffffe 0x1 0xffffffac = UTF-16LE
		if (w[0] == 0xfffffffe)
			return UTF16be;
		else if (w[0] == 0xffffffff)
			return UTF16le;
	}
}

bool checkOnlyOne(char *w, int len) {
	if (w[0] == 0xfffffffe || w[0] == 0xffffffff) { // UTF-16ÀÏ ¶§
		if (len == 4) // BOM2¹ÙÀÌÆ®, ³ª¸ÓÁö 2¹ÙÀÌÆ® (ÇÑ ±ÛÀÚ ÀÏ ¶§)
			return true;
		else
			return false;
	}
	else if (w[0] == 0xffffffef) { // UTF-8ÀÏ ¶§
		if (len == 6) // BOM3¹ÙÀÌÆ®, ³ª¸ÓÁö 3¹ÙÀÌÆ® (ÇÑ ±ÛÀÚ ÀÏ ¶§)
			return true;
		else
			return false;
	}
	else { // EUC-KRÀÏ ¶§
		if (len == 2) // BOM ¾øÀ½, ³ª¸ÓÁö 2¹ÙÀÌÆ® (ÇÑ ±ÛÀÚ ÀÏ ¶§)
			return true;
		else
			return false;
	}
}

void compare(char *w, int len) {
	if (w[0] == 0xfffffffe) // UTF-16BEÀÏ ¶§
		isWhat = "UTF-16BE";
	else if (w[0] == 0xffffffff) // UTF-16LEÀÏ ¶§
		isWhat = "UTF-16LE";
	else if (w[0] == 0xffffffef) // UTF-8ÀÏ ¶§
		isWhat = "UTF-8";
	else // EUC-KRÀÏ ¶§
		isWhat = "EUC-KR";
}

string compareEUCKR(char *w, int len) {
	// ¾Æ½ºÅ°Ç¥¿¡ ÀÖ´Â ¹üÀ§°¡ µé¾î¿À¸é(ÇÑ±ÛÀÌ ¾Æ´Ñ ¹üÀ§°¡ µé¾î¿À¸é) EUC-KRÀÎ°Í
	bool checkEUC = true;
	compare(w, len);
	if (isWhat == "EUC-KR") {
		for (int i = 0; i < len; i++) {
			if (w[i] < 0 || w[i] > 127) {
				checkEUC = false;
				break;
			}
		}
	}
	else if (isWhat == "UTF-8") {
		for (int i = 3; i < len; i++) {
			if (w[i] < 0 || w[i] > 127) {
				checkEUC = false;
				break;
			}
		}
	}
	else if (isWhat == "UTF-16LE" || isWhat == "UTF-16BE") {
		for (int i = 2; i < len; i++) {
			if (w[i] < 0 || w[i] > 127) {
				checkEUC = false;
				break;
			}
		}
	}
	isWhat = "";
	if (checkEUC) {
		EUC_KR = true;
		return EUCKR;
	}
	else {
		EUC_KR = false;
		return "";
	}
}

void convert(char *w, int len) {
	if (isWhat == "EUC-KR") {
		//cout << "ÀÔ·Â¹ŞÀº ÇÑ±ÛÀº EUC-KRÀÌ¹Ç·Î ";
		//cout << "UTF-16BE·ÎÀÇ º¯È¯À» ½ÃÀÛÇÕ´Ï´Ù." << endl << "EUC-KR¿¡¼­ UTF-16BE·Î ¹Ù·Î º¯È¯Àº ¾î·Æ±â ¶§¹®¿¡\nEUC-KR -> UTF-8 -> UTF-16BE º¯È¯ °úÁ¤À» °ÅÄ¡°Ô µË´Ï´Ù." << endl << endl;
		convertEUCto8(bufferNew, bufferLen);
		for (int i = 0; i < bufferLen*1.5 + 3; i++)
			buffer8[i] = buffer8[i] - 0xffffff00;
		for (int i = 0; i < bufferLen*1.5 + 3; i++)
			convertBin(buffer8[i]); // 2Áø¼ö·Î º¯È¯
		convert8BintoUniBin(uniBin, bufferBinLen);
		convertUniBintoDec(uniBin, start);
	}
	else if (isWhat == "UTF-16LE") {
		cout << "ÀÔ·Â¹ŞÀº ÇÑ±ÛÀº UTF-16LEÀÌ¹Ç·Î ";
		cout << "UTF-16BE·ÎÀÇ º¯È¯À» ½ÃÀÛÇÕ´Ï´Ù." << endl << endl;
		convertLEtoBE(buffer, bufferLen);
	}
	else if (isWhat == "UTF-8") {
		cout << "ÀÔ·Â¹ŞÀº ÇÑ±ÛÀº UTF-8ÀÌ¹Ç·Î ";
		cout << "UTF-16BE·ÎÀÇ º¯È¯À» ½ÃÀÛÇÕ´Ï´Ù." << endl << endl;
		for (int i = 0; i < bufferLen; i++)
			convertBin(bufferNew[i]); // 2Áø¼ö·Î º¯È¯
		printBin(bufferBin, bufferBinLen);
		cout << endl;
		convert8BintoUniBin(uniBin, bufferBinLen);
		convertUniBintoDec(uniBin, start);
	}
	else if (isWhat == "UTF-16BE")
		cout << "ÀÌ¹Ì UTF-16BEÀÌ¹Ç·Î ¹Ù²Ü ÇÊ¿ä°¡ ¾ø½À´Ï´Ù" << endl;
}

void convert8BintoUniBin(int *w, int len) { // 000000-00007F 1ByteÀÎ °æ¿ì
	int next = 24;
	while (next != len) {
		if (bufferBin[next] == 0) { // 000000-00007F 1ByteÀÎ °æ¿ì
			next++; // 0xxxxxxx ÀÌ¹Ç·Î 0 °Ç³Ê¶Ù°í
			w[start++] = 5; // ¿©±â¼­ 5´Â ÀÓÀÇ·Î »õ·Î¿î ¹®ÀÚ°¡ ½ÃÀÛµÊÀ» ¾Ë¸®°Ô ¼³Á¤ÇÑ ¼ıÀÚ
			w[start++] = 7; // ¸î¹øÀ» ÀĞ¾î¾ß ÇÒ °ÍÀÎÁö (7¹ø)
			for (int i = 0; i < 7; i++) // 0xxxxxxx ÀÌ¹Ç·Î 7¹ø ½ÇÇà
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 0) { // 000080-0007FF 2ByteÀÎ °æ¿ì
																								 // 110xxxxx 10xxxxxx
			next += 3; // 110 °Ç³Ê ¶Ù°í 
			w[start++] = 5; // ¿©±â¼­ 5´Â ÀÓÀÇ·Î »õ·Î¿î ¹®ÀÚ°¡ ½ÃÀÛµÊÀ» ¾Ë¸®°Ô ¼³Á¤ÇÑ ¼ıÀÚ
			w[start++] = 11; // ¸î¹øÀ» ÀĞ¾î¾ß ÇÒ °ÍÀÎÁö (11¹ø)
			for (int i = 0; i < 5; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 °Ç³Ê ¶Ù°í
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 1 && bufferBin[next + 3] == 0) { // 000800-00FFFF 3ByteÀÎ °æ¿ì
																															 //1110xxxx 10xxxxxx 10xxxxxx
			next += 4; // 1110 °Ç³Ê ¶Ù°í
			w[start++] = 5; // ¿©±â¼­ 5´Â ÀÓÀÇ·Î »õ·Î¿î ¹®ÀÚ°¡ ½ÃÀÛµÊÀ» ¾Ë¸®°Ô ¼³Á¤ÇÑ ¼ıÀÚ
			w[start++] = 16; // ¸î¹øÀ» ÀĞ¾î¾ß ÇÒ °ÍÀÎÁö (16¹ø)
			for (int i = 0; i < 4; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 °Ç³Ê ¶Ù°í
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 °Ç³Ê ¶Ù°í
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 1 && bufferBin[next + 3] == 1 && bufferBin[next + 4] == 0) { // 010000-10FFFF 4ByteÀÎ °æ¿ì (SMP)
																																						 //11110zzz 10zzxxxx 10xxxxxx 10xxxxxx
			next += 5; // 11110 °Ç³Ê ¶Ù°í
			w[start++] = 5; // ¿©±â¼­ 5´Â ÀÓÀÇ·Î »õ·Î¿î ¹®ÀÚ°¡ ½ÃÀÛµÊÀ» ¾Ë¸®°Ô ¼³Á¤ÇÑ ¼ıÀÚ
			w[start++] = 21; // ¸î¹øÀ» ÀĞ¾î¾ß ÇÒ °ÍÀÎÁö (21¹ø)
			for (int i = 0; i < 3; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10°Ç³Ê ¶Ù°í
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10°Ç³Ê ¶Ù°í
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10°Ç³Ê ¶Ù°í
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else {
			//cout << "ÇÑ±ÛÀÇ ¹®ÀÚ¿­À» ÀÔ·ÂÇØ ÁÖ¼¼¿ä." << endl;
			break;
		}
	}
	// Á¦´ë·Î º¯È¯ÀÌ µÇ¾ú´ÂÁö test¿ë ÇÁ¸°Æ®¹®
	//int remember; // ¸î±ÛÀÚ°¡ µé¾î°¡´ÂÁö ±â¾ïÇÏ´Â ÀÓ½Ãº¯¼ö
	//cout << "¾Æ·¡´Â ¼ıÀÚ¸¦ 2Áø¼ö·Î Ãâ·ÂÇÑ °ÍÀÔ´Ï´Ù. 5´Â »õ ¹®ÀÚ°¡ ½ÃÀÛµÊÀ» ¾Ë¸®¸ç 5 ´ÙÀ½¼ö´Â ¸î±ÛÀÚ ±îÁö°¡ ±× ¹®ÀÚÀÎÁö¸¦ ¾Ë·ÁÁİ´Ï´Ù." << endl;
	//for (int i = 0; i < start; i++) {
	//	if (w[i] == 5)
	//		remember = w[i + 1] + i + 1;
	//	cout << w[i];
	//	if (i == remember)
	//		cout << " ";
	//}
	//cout << endl;
}

void convertUniBintoDec(int *w, int len) {
	int remember = w[1]; // ¸î±ÛÀÚ°¡ µé¾î°¡´ÂÁö ±â¾ïÇÏ´Â ÀÓ½Ãº¯¼ö
	int rere; // ÀÓ½Ãº¯¼ö
	int rememberReal; // ÁøÂ¥ ¸î±ÛÀÚÀÎÁö
	int powNum; // nÁ¦°öÇÒ ¼ö
	int s = 0;
	for (int i = 0; i < len; i++) {
		if (w[i] == 5) {
			rere = remember;
			remember = w[i + 1] + i; // remember±îÁö ÇÑ¹®ÀÚ·Î Ãë±ŞÇÏ¸é µÊ
			rememberReal = w[i + 1];
			powNum = w[i + 1] - 1;
			if (i == rere + 2)
				s++;
			i += 2;
		}
		for (int j = 0; j < rememberReal; j++) {
			if (j == rememberReal - 1)
				bufferUniDec[s] += w[i];
			else
				bufferUniDec[s] += w[i++] * pow(2, powNum--);
		}
	}
	// test¿ë Ãâ·Â¹®
	/*cout << "¾Æ·¡ ³»¿ëÀº À§ÀÇ 2Áø¼ö¸¦ Uni10Áø¼ö·Î Ç¥ÇöÇÑ °ÍÀÔ´Ï´Ù." << endl;
	for (int i = 0; i < s+1; i++) {
	cout << bufferUniDec[i] << " ";
	}*/
	//cout << "UTF-16À¸·Î º¯È¯ ¿Ï·á" << endl << "0xfeff "; //feff´Â UTF-16BEÀÇ BOMÇ¥½Ã
	bufferUniDecLen = s + 1;
	//for (int i = 0; i < s + 1; i++)
	//	printf("0x%x ", bufferUniDec[i]);
}

void convertLEtoBE(char *w, int len) {
	for (int i = 0; i < len; i += 2) {
		buffer16BE[i] = w[i + 1];
		buffer16BE[i + 1] = w[i];
	}
	cout << "UTF-16À¸·Î º¯È¯ ¿Ï·á" << endl;
	buffer16BELen = len;
	for (int i = 0; i < len; i++)
		printf("0x%x ", buffer16BE[i]);
	cout << endl;
}

void convertEUCto8(int *in, int nOut) {
	USES_CONVERSION;
	char *ary = new char[nOut];
	for (int i = 0; i < nOut; i++)
		ary[i] = in[i];
	char *out = new char[nOut*1.5];
	wchar_t *wc = A2W(ary); // ANSI to UCS-2
	WideCharToMultiByte(CP_UTF8, 0, wc, -1, out, nOut*1.5, 0, 0); // UCS-2 to UTF-8
	for (int i = 0; i < nOut*1.5; i++)
		buffer8[i] = out[i];
	// BOM Ãß°¡ÇÏ±â
	for (int i = nOut*1.5 + 2; i >= 3; i--)
		buffer8[i] = buffer8[i - 3];
	buffer8[0] = 0xffffffef; buffer8[1] = 0xffffffbb; buffer8[2] = 0xffffffbf;
	/*cout << "UTF-8·Î º¯È¯µÈ 16Áø¼ö¸¦ Ãâ·ÂÇÏ°Ú½À´Ï´Ù." << endl;
	for (int i = 0; i < nOut*1.5 + 3; i++)
	printf("0x%x ", buffer8[i]);
	cout << endl << endl;*/
}

bool checkNoHanGul(char *w, int len) {
	for (int i = 0; i < len; i++) {
		if (w[i] >= 0 && w[i] <= 127)
			return false;
	}
	return true;
}

void homeWork4() {
	if (!flag) { // ¾ÆÁ÷ °úÁ¦ÇÔ¼ö6ÀÌ ½ÇÇà µÇÁö ¾Ê¾Ò´Ù¸é
		if (isWhat == "UTF-16LE") {
			//cout << endl << "16LE¿¡¼­ ½ÇÇà ½ÃÄ×À» ¶§ : " << endl;
			for (int i = 0; i < buffer16BELen; i++) {
				if (buffer16BE[i] < 0)
					buffer16BE[i] -= 0xffffff00;
			}
		}
		else if (isWhat == "UTF-16BE") {
			//cout << "16BE¿¡¼­ ½ÇÇà½ÃÄ×À» ¶§ : " << endl;
			for (int i = 0; i < bufferLen; i++) {
				if (buffer[i] < 0)
					buffer16BE[i] = buffer[i] - 0xffffff00;
				else
					buffer16BE[i] = buffer[i]; // int·Î Çüº¯È¯
			}
			buffer16BELen = bufferLen;
		}
		else if (isWhat == "EUC-KR" || isWhat == "UTF-8") {
			//cout << "EUCKRÀÌ³ª UTF8¿¡¼­ ½ÇÇà½ÃÄ×À» ¶§ : " << endl;
			for (int i = 0; i < bufferUniDecLen; i++)
				buffer16BE[i] = bufferUniDec[i];
			buffer16BELen = bufferUniDecLen;
		}
		// ¿©±â±îÁö µÚÁ×¹ÚÁ×ÀÌ¿´´ø ¹è¿­À» buffer16BELen ÀÌ°É·Î ÅëÀÏ½ÃÅ´
		/*for (int i = 0; i < buffer16BELen; i++)
		printf("%x ", buffer16BE[i]);
		cout << endl;*/
		if (isWhat == "UTF-16LE" || isWhat == "UTF-16BE") {
			int index = 0;
			for (int i = 0; i < buffer16BELen; i++) {
				if (i % 2 == 0) // index°¡ Â¦¼ö ÀÏ ¶§ ±× indexÀÇ ¼ö * 16 + ´ÙÀ½¼ö
					buffer16BE[index++] = buffer16BE[i] * 16 * 16 + buffer16BE[i + 1];
				else
					continue;
			}
			buffer16BELen /= 2; // ±æÀÌ´Â Àı¹İÀ¸·Î ¹İÅä¸· ³²
								// ¿©±â±îÁö ºÙÀÌ±â ¿Ï·á
		}
		//for (int i = 0; i < buffer16BELen; i++)
		//printf("%x ", buffer16BE[i]);
		//cout << endl;
	}
	Jamo *jamo = new Jamo[buffer16BELen]; // ¹öÆÛÀÇ ±æÀÌ¸¸Å­ µ¿ÀûÇÒ´ç
										  /* ÃÊ¼º, Áß¼º, Á¾¼º ³ª´©´Â °ø½Ä
										  i = ch - 0xac00;
										  cho = i / (21 * 28);
										  jung = (i / 28) % 21;
										  jong = i % 28;
										  printf("\tÃÊ¼º:Áß¼º:Á¾¼º(offset °ª)\t--> %d : %d : %d\n", cho, jung, jong);
										  printf("\tÃÊ¼º:Áß¼º:Á¾¼º(À¯´ÏÄÚµå °ª)\t--> U+%x : U+%x : U+%x\n", cho, jung, jong);
										  */
	for (int i = 0; i < buffer16BELen; i++) {
		int word = buffer16BE[i] - 0xac00;
		jamo[i].first = word / (21 * 28);
		jamo[i].middle = (word / 28) % 21;
		jamo[i].end = word % 28;
	}
	string first[19] = { "¤¡", "¤¢", "¤¤", "¤§", "¤¨", "¤©", "¤±", "¤²", "¤³", "¤µ", "¤¶", "¤·", "¤¸", "¤¹", "¤º", "¤»", "¤¼", "¤½", "¤¾" };
	string middle[21] = { "¤¿", "¤À", "¤Á", "¤Â", "¤Ã", "¤Ä", "¤Å", "¤Æ", "¤Ç", "¤È", "¤É", "¤Ê", "¤Ë", "¤Ì", "¤Í", "¤Î", "¤Ï", "¤Ğ", "¤Ñ", "¤Ò", "¤Ó" };
	string end[28] = { "", "¤¡", "¤¢", "¤£", "¤¤", "¤¥", "¤¦", "¤§", "¤©", "¤ª", "¤«", "¤¬", "¤­", "¤®", "¤¯", "¤°", "¤±", "¤²", "¤´", "¤µ", "¤¶", "¤·", "¤¸", "¤º", "¤»", "¤¼", "¤½", "¤¾" };
	if (!flag) {
		//cout << endl << "°úÁ¦(4¹ø) ÇÑ±Û ¹®ÀÚ¿­À» ÃÊ¼º(0~18), Áß¼º(0~20), Á¾¼º(0~27)À¸·Î ºĞ¸®ÇÑ °á°ú °ª" << endl << endl;
		int i = 1;
		if (isWhat == "EUC-KR" || isWhat == "UTF-8") 
			i = 0;
		else if (check16check) {
			check16 = true;
			check16check = false;
		}
		for (; i < buffer16BELen; i++) { // Ã³À½¿¡´Â BOMÀÌ µé¾îÀÖÀ¸¹Ç·Î 1ºÎÅÍ
			if (countPrint == 0)
				printf("\tÃÊ¼º:Áß¼º:Á¾¼º(offset °ª) : \t%d\t %d\t %d\n", jamo[i].first, jamo[i].middle, jamo[i].end);
			if (userFirst) { // Ã³À½¿£ userjamo class¿¡ ³Ö´Â´Ù
				userjamo[i].first = jamo[i].first;
				userjamo[i].middle = jamo[i].middle;
				userjamo[i].end = jamo[i].end;
				if (i == buffer16BELen - 1) {
					userFirst = false;
					userBufferLen = buffer16BELen;
				}
			}
			else {
				dicjamo[i].first = jamo[i].first;
				dicjamo[i].middle = jamo[i].middle;
				dicjamo[i].end = jamo[i].end;
			}
			int firstBase = 0x1100;		// À¯´ÏÄÚµå ÃÊ¼º '¤¡'
			int middleBase = 0x1161;	// À¯´ÏÄÚµå Áß¼º '¤¿'
			int endBase = 0x11A8;		// À¯´ÏÄÚµå Á¾¼º '¤¡'
			int a = jamo[i].first + firstBase;
			int b = jamo[i].middle + middleBase;
			int c = jamo[i].end + endBase - 1;
			if (countPrint == 0) {
				printf("\tÃÊ¼º:Áß¼º:Á¾¼º(À¯´ÏÄÚµå °ª) : U+%x : U+%x : U+%x\n", a, b, c);
				cout << "\tÃÊ¼º : " << first[jamo[i].first] << "\tÁß¼º : " << middle[jamo[i].middle] << "\tÁ¾¼º : " << end[jamo[i].end] << endl;
			}
		}
		countPrint++;
		homeWork5(jamo);
	}

	//else if (flag) { // °úÁ¦ÇÔ¼ö6ÀÌ ½ÇÇà µÈ ÈÄ¶ó¸é
	//	int i = 1;
	//	if (isWhat == "EUC-KR" || isWhat == "UTF-8")
	//		i = 0;
	//	cout << "6-1 ÃÊ/Áß/Á¾¼ºÀ¸·Î Ç®¾î¾´ ÇüÅÂ" << endl;
	//	for (; i < buffer16BELen; i++)// Ã³À½¿¡´Â BOMÀÌ µé¾îÀÖÀ¸¹Ç·Î 1ºÎÅÍ
	//		cout << first[jamo[i].first] << middle[jamo[i].middle] << end[jamo[i].end] << " ";
	//	cout << endl << "6-2 ÃÊ¼ºµé¸¸ Ãâ·Â" << endl;
	//	i = 1;
	//	if (isWhat == "EUC-KR" || isWhat == "UTF-8")
	//		i = 0;
	//	for (; i < buffer16BELen; i++)// Ã³À½¿¡´Â BOMÀÌ µé¾îÀÖÀ¸¹Ç·Î 1ºÎÅÍ
	//		cout << first[jamo[i].first] << " ";
	//	cout << endl;
	//}
}

void homeWork5(class Jamo *jamo) {
	//cout << "°úÁ¦ (5¹ø) À§ 4¹øÀÇ ±¸Á¶Ã¼ ¹è¿­ ³»¿ëÀ» UTF-16(BE) ÇÑ±Û ½ºÆ®¸µÀ¸·Î Á¶ÇÕÇÑ °á°ú °ª. (¾ÕÀÇ feff´Â UTF-16BEÀÇ BOM)" << endl;
	int *combine = new int[buffer16BELen]; // ÇÕÄ¡´Â°Ç ³ª´³À» ¶§Ã³·³ °Å²Ù·Î ÇØÁÖ¸é µÈ´Ù.
	for (int i = 0; i < buffer16BELen; i++) {
		combine[i] = 0xac00 + 28 * 21 * jamo[i].first + 28 * jamo[i].middle + jamo[i].end;
		//printf("%x ", combine[i]);
	}
	//cout << endl << endl;
}

void homeWork6() {
	//cout << "°úÁ¦ (6¹ø)  ÀÓÀÇÀÇ ÅØ½ºÆ®¸¦ ÃÊ/Áß/Á¾¼ºÀ¸·Î Ç®¾î¾´ ÇüÅÂ·Î Ãâ·Â ¹× ÃÊ¼ºµé¸¸ Ãâ·ÂÇÏ´Â ÇÁ·Î±×·¥ ÀÛ¼º." << endl;
	flag = true;
	homeWork4();
}

void HW2() {
	//int i = 1;
	//if (!check16) 
	//	i = 0;
	//for (; i < buffer16BELen; i++)  // Ã³À½¿¡´Â BOMÀÌ µé¾îÀÖÀ¸¹Ç·Î 1ºÎÅÍ
	//	printf("\tÀÔ·ÂÀÇÃÊ¼º:Áß¼º:Á¾¼º(offset °ª) : \t%d\t %d\t %d\n", userjamo[i].first, userjamo[i].middle, userjamo[i].end);
	ifstream instream("input.txt");
	if (!instream.is_open()) {
		cout << "input.txt¸¦ ¿­ ¼ö ¾ø½À´Ï´Ù." << endl;
		exit(1);
	}
	for (int i = 0; i < MAX_SIZE; i++) {
		buffer[i] = NULL;
		bufferNew[i] = NULL;
		bufferReal[i] = NULL;
		bufferUniDec[i] = NULL;
		bufferUniDecLen = 0;
		buffer16BE[i] = NULL;
		buffer16BELen = 0;
		buffer8[i] = NULL;
		bufferLen = 0;
		bufferBin[i] = NULL;
		bufferBinLen = 0;
		uniBin[i] = NULL;
		iBin = 0;
		start = 0;
		flag = false;
	}
	while (instream.getline(buffer, 100)) {
		bufferLen = strlen(buffer);
		for (int i = 0; i < bufferLen; i++)
			bufferNew[i] = buffer[i] - 0xffffff00;
		//cout << compareEUCKR(buffer, bufferLen) << endl;
		compareEUCKR(buffer, bufferLen);
		if (!EUC_KR) { // EUC-KRÀÌ ¾Æ´Ï¶ó¸é
			if (checkOnlyOne(buffer, bufferLen))
				//cout << compareOnlyOne(buffer, bufferLen);
				compareOnlyOne(buffer, bufferLen);
			else {
				compare(buffer, bufferLen);
				if (isWhat == "EUC-KR");
				//cout << EUCKR;
				else if (isWhat == "UTF-16LE")
					cout << UTF16le;
				else if (isWhat == "UTF-16BE")
					cout << UTF16be;
				else if (isWhat == "UTF-8")
					cout << UTF8;
			}
		}
		//if(!checkNoHanGul(buffer, bufferLen)) { // ÇÑ±ÛÀÌ ¾Æ´Ñ ¹®ÀÚ°¡ ÇÏ³ª¶óµµ µé¾î°¡ ÀÖÀ¸¸é
		//	cout << "ÀÔ·Â¹ŞÀº ¹®ÀÚ¿­ÀÌ ÇÑ±ÛÀÌ ¾Æ´Ñ°ÍÀÌ ÀÖÀ¸¹Ç·Î UTF-16BE·Î º¯È¯À» ÇÏÁö ¾Ê°í Á¾·áÇÕ´Ï´Ù." << endl;
		//	return 0;
		//}
		compare(buffer, bufferLen);
		if (isWhat != "UTF-16BE")  // UTF-16BE°¡ ¾Æ´Ï¶ó¸é UTF-16BE·Î º¯È¯½ÃÄÑÁà¾ßÇÔ
			convert(buffer, bufferLen);

		homeWork4(); // °úÁ¦(5¹øÀº homeWork4() ÇÔ¼ö¾È¿¡ È£Ãâ ÇÏ´Â °÷ÀÌ ÀÖ½À´Ï´Ù.)
		homeWork6();
		
		//for (int i = 0; i < buffer16BELen; i++)  // Ã³À½¿¡´Â BOMÀÌ µé¾îÀÖÀ¸¹Ç·Î 1ºÎÅÍ
		//	printf("\t»çÀüÀÇÃÊ¼º:Áß¼º:Á¾¼º(offset °ª) : \t%d\t %d\t %d\n", dicjamo[i].first, dicjamo[i].middle, dicjamo[i].end);
		int count = 0, realcount = 0;
		
		if (check16) { // UTF-16 °è¿­ÀÏ °æ¿ì ¸Ç¾Õ¿¡ 000ÀÌ ÀÖ±â ¶§¹®¿¡ ÇÑÄ­¾¿ ¾ÕÀ¸·Î ¶¯°ÜÁÖ´Â ÀÛ¾÷À» ÇØÁà¾ßÇÏ°í ³ªÁß¿¡ ÀüÃ¼ ±æÀÌµµ -1 ÇØÁà¾ßÇÔ
			for (int i = 0; i < userBufferLen; i++) {
				userjamo[i].first = userjamo[i + 1].first;
				userjamo[i].middle = userjamo[i + 1].middle;
				userjamo[i].end = userjamo[i + 1].end;
			}
			userBufferLen -= 1;
			check16 = false;
		}
		for (int i = 0; i < userBufferLen; i++) {
			countPrint = i;
			// ¿©±â¼­ºÎÅÍ ÃÊ¼º
			if (dicjamo[i].first == 0) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤¡ÀÏ ¶§
				if (userjamo[i].first != 0) {
					count++;
					if (userjamo[i].first == 3 || userjamo[i].first == 9) { // ¤§,¤µ ÀÏ °æ¿ì
						realcount++;
					}
				}
			}
			if (dicjamo[i].first == 2) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤¤ÀÏ ¶§
				if (userjamo[i].first != 2) {
					count++;
					if (userjamo[i].first == 6 || userjamo[i].first == 11) // ¤±,¤· ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 3) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤§ÀÏ ¶§
				if (userjamo[i].first != 3) {
					count++;
					if (userjamo[i].first == 0 || userjamo[i].first == 12) // ¤¡,¤¸ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 5) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤©ÀÏ ¶§
				if (userjamo[i].first != 5) {
					count++;
					if (userjamo[i].first == 18 || userjamo[i].first == 11) // ¤¾,¤· ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 6) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤±ÀÏ ¶§
				if (userjamo[i].first != 6) {
					count++;
					if (userjamo[i].first == 2) // ¤¤ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 7) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤²ÀÏ ¶§
				if (userjamo[i].first != 7) {
					count++;
					if (userjamo[i].first == 12) // ¤¸ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 9) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤µÀÏ ¶§
				if (userjamo[i].first != 9) {
					count++;
					if (userjamo[i].first == 0) // ¤¡ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 11) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤·ÀÏ ¶§
				if (userjamo[i].first != 11) {
					count++;
					if (userjamo[i].first == 2 || userjamo[i].first == 5) // ¤¤,¤© ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 12) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤¸ÀÏ ¶§
				if (userjamo[i].first != 12) {
					count++;
					if (userjamo[i].first == 7 || userjamo[i].first == 3) // ¤²,¤§ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 14) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤ºÀÏ ¶§
				if (userjamo[i].first != 14) {
					count++;
					if (userjamo[i].first == 16 || userjamo[i].first == 17) // ¤¼,¤½ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 15) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤»ÀÏ ¶§
				if (userjamo[i].first != 15) {
					count++;
					if (userjamo[i].first == 16) // ¤¼ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 16) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤¼ÀÏ ¶§
				if (userjamo[i].first != 16) {
					count++;
					if (userjamo[i].first == 15 || userjamo[i].first == 14) // ¤»,¤º ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 17) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤½ÀÏ ¶§
				if (userjamo[i].first != 17) {
					count++;
					if (userjamo[i].first == 14) // ¤º ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].first == 18) { // »çÀü¿¡ ÃÊ¼ºÀÌ ¤¾ÀÏ ¶§
				if (userjamo[i].first != 18) {
					count++;
					if (userjamo[i].first == 5) // ¤© ÀÏ °æ¿ì
						realcount++;
				}
			}
			// ¿©±â¼­ºÎÅÍ Áß¼º
			if (dicjamo[i].middle == 0) { // »çÀü¿¡ Áß¼ºÀÌ ¤¿ÀÏ ¶§
				if (userjamo[i].middle != 0) {
					count++;
					if (userjamo[i].middle == 4 || userjamo[i].middle == 20) // ¤Ã,¤Ó ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 1) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÀÀÏ ¶§
				if (userjamo[i].middle != 1) {
					count++;
					if (userjamo[i].middle == 2 || userjamo[i].middle == 5) // ¤Á,¤Ä ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 2) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÁÀÏ ¶§
				if (userjamo[i].middle != 2) {
					count++;
					if (userjamo[i].middle == 6 || userjamo[i].middle == 1) // ¤Å,¤À ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 4) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÃÀÏ ¶§
				if (userjamo[i].middle != 4) {
					count++;
					if (userjamo[i].middle == 8 || userjamo[i].middle == 0) // ¤Ç,¤¿ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 5) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÄÀÏ ¶§
				if (userjamo[i].middle != 5) {
					count++;
					if (userjamo[i].middle == 1) // ¤À ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 6) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÅÀÏ ¶§
				if (userjamo[i].middle != 6) {
					count++;
					if (userjamo[i].middle == 12 || userjamo[i].middle == 2) // ¤Ë,¤Á ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 8) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÇÀÏ ¶§
				if (userjamo[i].middle != 8) {
					count++;
					if (userjamo[i].middle == 4) // ¤Ã ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 12) { // »çÀü¿¡ Áß¼ºÀÌ ¤ËÀÏ ¶§
				if (userjamo[i].middle != 12) {
					count++;
					if (userjamo[i].middle == 6) // ¤Å ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 13) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÌÀÏ ¶§
				if (userjamo[i].middle != 13) {
					count++;
					if (userjamo[i].middle == 17 || userjamo[i].middle == 18) // ¤Ğ,¤Ñ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 17) { // »çÀü¿¡ Áß¼ºÀÌ ¤ĞÀÏ ¶§
				if (userjamo[i].middle != 17) {
					count++;
					if (userjamo[i].middle == 13) // ¤Ì ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 18) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÑÀÏ ¶§
				if (userjamo[i].middle != 18) {
					count++;
					if (userjamo[i].middle == 13) // ¤Ì ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].middle == 20) { // »çÀü¿¡ Áß¼ºÀÌ ¤ÓÀÏ ¶§
				if (userjamo[i].middle != 20) {
					count++;
					if (userjamo[i].middle == 0) // ¤¿ ÀÏ °æ¿ì
						realcount++;
				}
			}
			// ¿©±â¼­ºÎÅÍ Á¾¼º
			if (dicjamo[i].end == 1) { // »çÀü¿¡ Á¾¼ºÀÌ ¤¡ÀÏ ¶§
				if (userjamo[i].end != 1) {
					count++;
					if (userjamo[i].end == 7 || userjamo[i].end == 19) // ¤§,¤µ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 4) { // »çÀü¿¡ Á¾¼ºÀÌ ¤¤ÀÏ ¶§
				if (userjamo[i].end != 4) {
					count++;
					if (userjamo[i].end == 16 || userjamo[i].end == 21) // ¤±,¤· ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 7) { // »çÀü¿¡ Á¾¼ºÀÌ ¤§ÀÏ ¶§
				if (userjamo[i].end != 7) {
					count++;
					if (userjamo[i].end == 22 || userjamo[i].end == 1) //¤¸,¤¡ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 8) { // »çÀü¿¡ Á¾¼ºÀÌ ¤©ÀÏ ¶§
				if (userjamo[i].end != 8) {
					count++;
					if (userjamo[i].end == 21 || userjamo[i].end == 27) // ¤·,¤¾ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 16) { // »çÀü¿¡ Á¾¼ºÀÌ ¤±ÀÏ ¶§
				if (userjamo[i].end != 16) {
					count++;
					if (userjamo[i].end == 4) // ¤¤ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 17) { // »çÀü¿¡ Á¾¼ºÀÌ ¤²ÀÏ ¶§
				if (userjamo[i].end != 17) {
					count++;
					if (userjamo[i].end == 22) // ¤¸ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 19) { // »çÀü¿¡ Á¾¼ºÀÌ ¤µÀÏ ¶§
				if (userjamo[i].end != 19) {
					count++;
					if (userjamo[i].end == 1) // ¤¡ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 21) { // »çÀü¿¡ Á¾¼ºÀÌ ¤·ÀÏ ¶§
				if (userjamo[i].end != 21) {
					count++;
					if (userjamo[i].end == 4 || userjamo[i].end == 8) // ¤¤,¤© ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 22) { // »çÀü¿¡ Á¾¼ºÀÌ ¤¸ÀÏ ¶§
				if (userjamo[i].end != 22) {
					count++;
					if (userjamo[i].end == 17 || userjamo[i].end == 7) // ¤²,¤§ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 23) { // »çÀü¿¡ Á¾¼ºÀÌ ¤ºÀÏ ¶§
				if (userjamo[i].end != 23) {
					count++;
					if (userjamo[i].end == 25 || userjamo[i].end == 26) // ¤¼,¤½ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 24) { // »çÀü¿¡ Á¾¼ºÀÌ ¤»ÀÏ ¶§
				if (userjamo[i].end != 24) {
					count++;
					if (userjamo[i].end == 25) // ¤¼ ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 25) { // »çÀü¿¡ Á¾¼ºÀÌ ¤¼ÀÏ ¶§
				if (userjamo[i].end != 25) {
					count++;
					if (userjamo[i].end == 24 || userjamo[i].end == 23) // ¤»,¤º ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 26) { // »çÀü¿¡ Á¾¼ºÀÌ ¤½ÀÏ ¶§
				if (userjamo[i].end != 26) {
					count++;
					if (userjamo[i].end == 23) // ¤º ÀÏ °æ¿ì
						realcount++;
				}
			}
			if (dicjamo[i].end == 27) { // »çÀü¿¡ Á¾¼ºÀÌ ¤¾ÀÏ ¶§
				if (userjamo[i].end != 27) {
					count++;
					if (userjamo[i].end == 8) // ¤© ÀÏ °æ¿ì
						realcount++;
				}
			}
		}
		if (count == 1 && realcount == 1) {
			cout << " [°úÁ¦ 2] °úÁ¦ 1¿¡¼­ ±¸ÇöÇÑ ÇÑ±Û¹®Àå Ç®¾î¾²±â ±â´É ¼öÁ¤-º¸¿Ï ½ÃÀÛ\n";
			cout << "»çÀü¿¡ À¯»çÇÑ ´Ü¾î´Â " << buffer << "ÀÔ´Ï´Ù." << endl;
			exit(1);
		}
		else if (count == 0) {
			cout << " [°úÁ¦ 2] °úÁ¦ 1¿¡¼­ ±¸ÇöÇÑ ÇÑ±Û¹®Àå Ç®¾î¾²±â ±â´É ¼öÁ¤-º¸¿Ï ½ÃÀÛ\n";
			cout << buffer << "´Â »çÀü°ú ¿ÏÀüÈ÷ ÀÏÄ¡ÇÏ´Â ´Ü¾îÀÔ´Ï´Ù." << endl;
			exit(1);
		}

		for (int i = 0; i < MAX_SIZE; i++) {
			buffer[i] = NULL;
			bufferNew[i] = NULL;
			bufferReal[i] = NULL;
			bufferUniDec[i] = NULL;
			bufferUniDecLen = 0;
			buffer16BE[i] = NULL;
			buffer16BELen = 0;
			buffer8[i] = NULL;
			bufferLen = 0;
			bufferBin[i] = NULL;
			bufferBinLen = 0;
			uniBin[i] = NULL;
			iBin = 0;
			start = 0;
			flag = false;
		}

		//cout << buffer << endl;
	}
}