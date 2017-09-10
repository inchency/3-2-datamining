#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <atlconv.h> // EUC-KR->UTF-8 ��ȯ�Ҷ� ��
#include <string>

#define FILE_NAME "949.txt"
#define MAX_SIZE 1000
using namespace std;

void readchar(ifstream &); // input.txt �д� �Լ�
void convertBin(int w); // 10������ �̿��� 2������ ��ȯ���ִ� �Լ�
void printBin(int *w, int len);
void printOct(char *w, int len); // 8������ ���
void printDec(char *w, int len); // 10������ ���
void printHex(char *w, int len); // 16������ ���
string compareEUCKR(char *w, int len); // ó���� EUC-KR���� �ƴ��� Ȯ�����ִ� �Լ�
bool checkOnlyOne(char *w, int len); // �ѱ������� ������������ �Ǻ����ִ� �Լ�
string compareOnlyOne(char *w, int len); // ���ڿ��� ���ڷ� �����Ͽ� �� ���ڿ��� �ѱ��ڵ尡 �������� ����Ʈ�� ����� �ڵ����� �ν��ϴ� �Լ� (�� �ѱ��� �϶��� ����)
void compare(char *w, int len); // ��� �ѱ��ڵ����� �Ǻ� (2�����̻� ����, BOM ���)
void convert8BintoUniBin(int *w, int len); // UTF-8 2���� -> Unicode 2������ ��ȯ���ִ� �Լ�
void convertUniBintoDec(int *w, int len); // Unicode 2���� -> 10������ ��ȯ���ִ� �Լ� (= UTF-16BE)
void convert(char *w, int len); // UTF16BE�� �ƴҶ� ��ȯ���ִ� �Լ�
void convertLEtoBE(char *w, int len); // UTF16LE->BE ��ȯ �Լ�
void convertEUCto8(int *in, int nOut); // EUC-KR->UTF-8 ��ȯ �Լ�
bool checkNoHanGul(char *w, int len); // �ѱ��� �ƴ� ���ڰ� �ϳ��� �� ������ false ��ȯ
void homeWork4(); // ����(4��) �Լ� (UTF-16(BE)�� �� �ѱ� ���ڿ��� �ʼ�(0~18), �߼�(0~20), ����(0~27)���� �и��ϴ� �Լ� �ۼ�.)
void homeWork5(class Jamo *jamo); // ����(5��) �� 4���� ����ü �迭 ������ UTF - 16(BE)�ѱ� ��Ʈ������ �����ϴ� �Լ� �ۼ�.
void homeWork6(); // ����(6��) �� �Լ����� �̿��Ͽ� ������ �ѱ��ڵ�� �ۼ��� �ѱ� �ؽ�Ʈ ������ ������ ��/��/�������� Ǯ� ���·� ��� �� �ʼ��鸸 ����ϴ� ���α׷� �ۼ�.
void HW2(); // [���� 2] ���� 1���� ������ "�ѱ۹��� Ǯ���" ��� ����-����

			// �ؿ��� ���ϰ� ����ϱ����� ���������� ����
char buffer[MAX_SIZE] = { NULL }; // ���ڿ��� �޾� buffer �迭�� ����
int bufferNew[MAX_SIZE] = { NULL }; // buffer[i] - 0xffffff00; �Ұ���
int bufferReal[MAX_SIZE] = { NULL }; // 8���ڸ� �迭1ĭ�� ���� ��¥ 2������ ������ �迭
int bufferUniDec[MAX_SIZE] = { NULL }; // ���� 10���� �����ϴ� �迭
int bufferUniDecLen = 0;
int buffer16BE[MAX_SIZE] = { NULL }; // ��ȯ�� UTF-16BE ��� �迭
int buffer16BELen = 0;
int buffer8[MAX_SIZE] = { NULL }; // ��ȯ�� UTF-8 ��� �迭
int bufferLen; // buffer�� ����
bool EUC_KR;
int bufferBin[MAX_SIZE] = { NULL };
int bufferBinLen; // 2����buffer�� ����
int uniBin[MAX_SIZE] = { NULL }; // �����ڵ� 2�����γ�Ÿ�� �迭
int iBin = 0;
int start = 0; // convert8BintoUniBin���� ���̸� convertUniBintoDec���� �迭�� ���̷ξ���
bool flag = false; // ����4 �Լ����� ����6 �Լ��� ���� ���¶�� (�ʼ��� + (�ʼ�,�߼�,����)) printf���� ����ϰ� �Ⱦ��� ���¶�� ��� ������� ���ڷθ� �����Բ� ��
string isWhat; // ��� �ѱ��ڵ����� ��Ÿ���� ����
string UTF8 = "�� ���ڿ� �ѱ��ڵ�� UTF-8 �Դϴ�\n";
string UTF16be = "�� ���ڿ� �ѱ��ڵ�� UTF-16(BE)  �Դϴ�\n";
string UTF16le = "�� ���ڿ� �ѱ��ڵ�� UTF-16(LE)  �Դϴ�\n";
string EUCKR = "�� ���ڿ� �ѱ��ڵ�� EUC-KR(�Ǵ� CP949) �Դϴ�\n";
bool check16 = false; // �Է¹޴� ���� utf-16�����̸� true �ƴϸ� false
bool check16check = true; // check16 �Ǻ��ϱ����� ����
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
	if (!EUC_KR) { // EUC-KR�� �ƴ϶��
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
	//if(!checkNoHanGul(buffer, bufferLen)) { // �ѱ��� �ƴ� ���ڰ� �ϳ��� �� ������
	//	cout << "�Է¹��� ���ڿ��� �ѱ��� �ƴѰ��� �����Ƿ� UTF-16BE�� ��ȯ�� ���� �ʰ� �����մϴ�." << endl;
	//	return 0;
	//}
	compare(buffer, bufferLen);
	if (isWhat != "UTF-16BE")  // UTF-16BE�� �ƴ϶�� UTF-16BE�� ��ȯ���������
		convert(buffer, bufferLen);
	cout << endl;
	// ���� (4��) �Լ� ȣ��κ�
	homeWork4(); // ����(5���� homeWork4() �Լ��ȿ� ȣ�� �ϴ� ���� �ֽ��ϴ�.)
	homeWork6();
	HW2();
	cout << " [���� 2] ���� 1���� ������ �ѱ۹��� Ǯ��� ��� ����-���� ����\n";
	cout << "������ ������ �ܾ �����ϴ�." << endl;
	return 0;
}
void readchar(ifstream & instream) {
	instream.open(FILE_NAME);
	if (!instream.is_open()) {
		cout << "input.txt�� �� �� �����ϴ�." << endl;
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
	cout << "�Է¹��� ���ڿ� :   " << endl << buffer << endl;
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
	cout << "������ �Է¹��� ���ڿ��� 2������ ��Ÿ���ڽ��ϴ�." << endl;
	for (int i = 0; i < bufferBinLen; i++) {
		if (i % 8 == 0 && i != 0) // 2���� �̻ڰ� ��� �ϱ����� if�� 8��Ʈ���� �� ����
			cout << " ";
		printf("%d", w[i]);
	}
	cout << endl;
}
void printOct(char *w, int len) {
	cout << "�� ���ڿ��� 8������ ��Ÿ���ڽ��ϴ�." << endl;
	for (int i = 0; i<len; i++)
		printf("O%o ", w[i]);
	cout << endl;
}
void printDec(char *w, int len) {
	cout << "�� ���ڿ��� 10������ ��Ÿ���ڽ��ϴ�." << endl;
	for (int i = 0; i<len; i++)
		printf("%d ", w[i]);
	cout << endl;
}
void printHex(char *w, int len) {
	cout << "�� ���ڿ��� 16������ ��Ÿ���ڽ��ϴ�." << endl;
	for (int i = 0; i<len; i++)
		printf("0x%x ", w[i]);
	cout << endl;
}

string compareOnlyOne(char *w, int len) {
	// ���� UTF8���� UTF16���� EUC-KR ���� �Ǻ��� ���ش�.
	// �Ʒ��� "��"�� ���� �޸��忡�� �������� ������ ������� �� ����̴�.
	/*	utf - 8
	﻿각
	�� ���ڿ��� 16������ ��Ÿ���ڽ��ϴ�.
	0xffffffef 0xffffffbb 0xffffffbf 0xffffffea 0xffffffb0 0xffffff81

	utf - 16be
	? 
	�� ���ڿ��� 16������ ��Ÿ���ڽ��ϴ�.
	0xfffffffe 0xffffffff 0xffffffac 0x1

	utf - 16le
	�
	�� ���ڿ��� 16������ ��Ÿ���ڽ��ϴ�.
	0xffffffff 0xfffffffe 0x1 0xffffffac


	EUC-KR
	��
	�� ���ڿ��� 16������ ��Ÿ���ڽ��ϴ�.
	0xffffffb0 0xffffffa2
	*/
	// ������ utf-8�� 16������ ����Ͽ��� �� 6����Ʈ�� ���԰� (BOM3����Ʈ) 3+3n (n�� ���ڰ���)
	// utf-16BE, utf-16LE�� 4����Ʈ �׸��� EUC-KR�� 2����Ʈ�� ���Դ� (BOM2����Ʈ) 2+2n, EUC-KR�� BOM���� 2n
	// ���� ����Ʈ ���� utf-8�� utf-16, EUC-KR�� ������ �ְ�
	// �״��� utf-16������ LE�� BE�� �տ� ù ����Ʈ fe�� �������� ff�� ���������� �񱳸� ���ְڴ�.
	if (len == 2) // "��" -> 0xffffffb0 0xffffffa2 = EUC-KR
		return EUCKR;
	else if (len == 6) // "��" -> 0xffffffef 0xffffffbb 0xffffffbf 0xffffffea 0xffffffb0 0xffffff81 = UTF-8
		return UTF8;
	else if (len == 4) { // "��" -> 0xfffffffe 0xffffffff 0xffffffac 0x1 = UTF-16BE , 0xffffffff 0xfffffffe 0x1 0xffffffac = UTF-16LE
		if (w[0] == 0xfffffffe)
			return UTF16be;
		else if (w[0] == 0xffffffff)
			return UTF16le;
	}
}

bool checkOnlyOne(char *w, int len) {
	if (w[0] == 0xfffffffe || w[0] == 0xffffffff) { // UTF-16�� ��
		if (len == 4) // BOM2����Ʈ, ������ 2����Ʈ (�� ���� �� ��)
			return true;
		else
			return false;
	}
	else if (w[0] == 0xffffffef) { // UTF-8�� ��
		if (len == 6) // BOM3����Ʈ, ������ 3����Ʈ (�� ���� �� ��)
			return true;
		else
			return false;
	}
	else { // EUC-KR�� ��
		if (len == 2) // BOM ����, ������ 2����Ʈ (�� ���� �� ��)
			return true;
		else
			return false;
	}
}

void compare(char *w, int len) {
	if (w[0] == 0xfffffffe) // UTF-16BE�� ��
		isWhat = "UTF-16BE";
	else if (w[0] == 0xffffffff) // UTF-16LE�� ��
		isWhat = "UTF-16LE";
	else if (w[0] == 0xffffffef) // UTF-8�� ��
		isWhat = "UTF-8";
	else // EUC-KR�� ��
		isWhat = "EUC-KR";
}

string compareEUCKR(char *w, int len) {
	// �ƽ�Űǥ�� �ִ� ������ ������(�ѱ��� �ƴ� ������ ������) EUC-KR�ΰ�
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
		//cout << "�Է¹��� �ѱ��� EUC-KR�̹Ƿ� ";
		//cout << "UTF-16BE���� ��ȯ�� �����մϴ�." << endl << "EUC-KR���� UTF-16BE�� �ٷ� ��ȯ�� ��Ʊ� ������\nEUC-KR -> UTF-8 -> UTF-16BE ��ȯ ������ ��ġ�� �˴ϴ�." << endl << endl;
		convertEUCto8(bufferNew, bufferLen);
		for (int i = 0; i < bufferLen*1.5 + 3; i++)
			buffer8[i] = buffer8[i] - 0xffffff00;
		for (int i = 0; i < bufferLen*1.5 + 3; i++)
			convertBin(buffer8[i]); // 2������ ��ȯ
		convert8BintoUniBin(uniBin, bufferBinLen);
		convertUniBintoDec(uniBin, start);
	}
	else if (isWhat == "UTF-16LE") {
		cout << "�Է¹��� �ѱ��� UTF-16LE�̹Ƿ� ";
		cout << "UTF-16BE���� ��ȯ�� �����մϴ�." << endl << endl;
		convertLEtoBE(buffer, bufferLen);
	}
	else if (isWhat == "UTF-8") {
		cout << "�Է¹��� �ѱ��� UTF-8�̹Ƿ� ";
		cout << "UTF-16BE���� ��ȯ�� �����մϴ�." << endl << endl;
		for (int i = 0; i < bufferLen; i++)
			convertBin(bufferNew[i]); // 2������ ��ȯ
		printBin(bufferBin, bufferBinLen);
		cout << endl;
		convert8BintoUniBin(uniBin, bufferBinLen);
		convertUniBintoDec(uniBin, start);
	}
	else if (isWhat == "UTF-16BE")
		cout << "�̹� UTF-16BE�̹Ƿ� �ٲ� �ʿ䰡 �����ϴ�" << endl;
}

void convert8BintoUniBin(int *w, int len) { // 000000-00007F 1Byte�� ���
	int next = 24;
	while (next != len) {
		if (bufferBin[next] == 0) { // 000000-00007F 1Byte�� ���
			next++; // 0xxxxxxx �̹Ƿ� 0 �ǳʶٰ�
			w[start++] = 5; // ���⼭ 5�� ���Ƿ� ���ο� ���ڰ� ���۵��� �˸��� ������ ����
			w[start++] = 7; // ����� �о�� �� ������ (7��)
			for (int i = 0; i < 7; i++) // 0xxxxxxx �̹Ƿ� 7�� ����
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 0) { // 000080-0007FF 2Byte�� ���
																								 // 110xxxxx 10xxxxxx
			next += 3; // 110 �ǳ� �ٰ� 
			w[start++] = 5; // ���⼭ 5�� ���Ƿ� ���ο� ���ڰ� ���۵��� �˸��� ������ ����
			w[start++] = 11; // ����� �о�� �� ������ (11��)
			for (int i = 0; i < 5; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 �ǳ� �ٰ�
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 1 && bufferBin[next + 3] == 0) { // 000800-00FFFF 3Byte�� ���
																															 //1110xxxx 10xxxxxx 10xxxxxx
			next += 4; // 1110 �ǳ� �ٰ�
			w[start++] = 5; // ���⼭ 5�� ���Ƿ� ���ο� ���ڰ� ���۵��� �˸��� ������ ����
			w[start++] = 16; // ����� �о�� �� ������ (16��)
			for (int i = 0; i < 4; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 �ǳ� �ٰ�
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 �ǳ� �ٰ�
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 1 && bufferBin[next + 3] == 1 && bufferBin[next + 4] == 0) { // 010000-10FFFF 4Byte�� ��� (SMP)
																																						 //11110zzz 10zzxxxx 10xxxxxx 10xxxxxx
			next += 5; // 11110 �ǳ� �ٰ�
			w[start++] = 5; // ���⼭ 5�� ���Ƿ� ���ο� ���ڰ� ���۵��� �˸��� ������ ����
			w[start++] = 21; // ����� �о�� �� ������ (21��)
			for (int i = 0; i < 3; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10�ǳ� �ٰ�
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10�ǳ� �ٰ�
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10�ǳ� �ٰ�
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else {
			//cout << "�ѱ��� ���ڿ��� �Է��� �ּ���." << endl;
			break;
		}
	}
	// ����� ��ȯ�� �Ǿ����� test�� ����Ʈ��
	//int remember; // ����ڰ� ������ ����ϴ� �ӽú���
	//cout << "�Ʒ��� ���ڸ� 2������ ����� ���Դϴ�. 5�� �� ���ڰ� ���۵��� �˸��� 5 �������� ����� ������ �� ���������� �˷��ݴϴ�." << endl;
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
	int remember = w[1]; // ����ڰ� ������ ����ϴ� �ӽú���
	int rere; // �ӽú���
	int rememberReal; // ��¥ ���������
	int powNum; // n������ ��
	int s = 0;
	for (int i = 0; i < len; i++) {
		if (w[i] == 5) {
			rere = remember;
			remember = w[i + 1] + i; // remember���� �ѹ��ڷ� ����ϸ� ��
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
	// test�� ��¹�
	/*cout << "�Ʒ� ������ ���� 2������ Uni10������ ǥ���� ���Դϴ�." << endl;
	for (int i = 0; i < s+1; i++) {
	cout << bufferUniDec[i] << " ";
	}*/
	//cout << "UTF-16���� ��ȯ �Ϸ�" << endl << "0xfeff "; //feff�� UTF-16BE�� BOMǥ��
	bufferUniDecLen = s + 1;
	//for (int i = 0; i < s + 1; i++)
	//	printf("0x%x ", bufferUniDec[i]);
}

void convertLEtoBE(char *w, int len) {
	for (int i = 0; i < len; i += 2) {
		buffer16BE[i] = w[i + 1];
		buffer16BE[i + 1] = w[i];
	}
	cout << "UTF-16���� ��ȯ �Ϸ�" << endl;
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
	// BOM �߰��ϱ�
	for (int i = nOut*1.5 + 2; i >= 3; i--)
		buffer8[i] = buffer8[i - 3];
	buffer8[0] = 0xffffffef; buffer8[1] = 0xffffffbb; buffer8[2] = 0xffffffbf;
	/*cout << "UTF-8�� ��ȯ�� 16������ ����ϰڽ��ϴ�." << endl;
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
	if (!flag) { // ���� �����Լ�6�� ���� ���� �ʾҴٸ�
		if (isWhat == "UTF-16LE") {
			//cout << endl << "16LE���� ���� ������ �� : " << endl;
			for (int i = 0; i < buffer16BELen; i++) {
				if (buffer16BE[i] < 0)
					buffer16BE[i] -= 0xffffff00;
			}
		}
		else if (isWhat == "UTF-16BE") {
			//cout << "16BE���� ��������� �� : " << endl;
			for (int i = 0; i < bufferLen; i++) {
				if (buffer[i] < 0)
					buffer16BE[i] = buffer[i] - 0xffffff00;
				else
					buffer16BE[i] = buffer[i]; // int�� ����ȯ
			}
			buffer16BELen = bufferLen;
		}
		else if (isWhat == "EUC-KR" || isWhat == "UTF-8") {
			//cout << "EUCKR�̳� UTF8���� ��������� �� : " << endl;
			for (int i = 0; i < bufferUniDecLen; i++)
				buffer16BE[i] = bufferUniDec[i];
			buffer16BELen = bufferUniDecLen;
		}
		// ������� ���׹����̿��� �迭�� buffer16BELen �̰ɷ� ���Ͻ�Ŵ
		/*for (int i = 0; i < buffer16BELen; i++)
		printf("%x ", buffer16BE[i]);
		cout << endl;*/
		if (isWhat == "UTF-16LE" || isWhat == "UTF-16BE") {
			int index = 0;
			for (int i = 0; i < buffer16BELen; i++) {
				if (i % 2 == 0) // index�� ¦�� �� �� �� index�� �� * 16 + ������
					buffer16BE[index++] = buffer16BE[i] * 16 * 16 + buffer16BE[i + 1];
				else
					continue;
			}
			buffer16BELen /= 2; // ���̴� �������� ���丷 ��
								// ������� ���̱� �Ϸ�
		}
		//for (int i = 0; i < buffer16BELen; i++)
		//printf("%x ", buffer16BE[i]);
		//cout << endl;
	}
	Jamo *jamo = new Jamo[buffer16BELen]; // ������ ���̸�ŭ �����Ҵ�
										  /* �ʼ�, �߼�, ���� ������ ����
										  i = ch - 0xac00;
										  cho = i / (21 * 28);
										  jung = (i / 28) % 21;
										  jong = i % 28;
										  printf("\t�ʼ�:�߼�:����(offset ��)\t--> %d : %d : %d\n", cho, jung, jong);
										  printf("\t�ʼ�:�߼�:����(�����ڵ� ��)\t--> U+%x : U+%x : U+%x\n", cho, jung, jong);
										  */
	for (int i = 0; i < buffer16BELen; i++) {
		int word = buffer16BE[i] - 0xac00;
		jamo[i].first = word / (21 * 28);
		jamo[i].middle = (word / 28) % 21;
		jamo[i].end = word % 28;
	}
	string first[19] = { "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };
	string middle[21] = { "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };
	string end[28] = { "", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };
	if (!flag) {
		//cout << endl << "����(4��) �ѱ� ���ڿ��� �ʼ�(0~18), �߼�(0~20), ����(0~27)���� �и��� ��� ��" << endl << endl;
		int i = 1;
		if (isWhat == "EUC-KR" || isWhat == "UTF-8") 
			i = 0;
		else if (check16check) {
			check16 = true;
			check16check = false;
		}
		for (; i < buffer16BELen; i++) { // ó������ BOM�� ��������Ƿ� 1����
			if (countPrint == 0)
				printf("\t�ʼ�:�߼�:����(offset ��) : \t%d\t %d\t %d\n", jamo[i].first, jamo[i].middle, jamo[i].end);
			if (userFirst) { // ó���� userjamo class�� �ִ´�
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
			int firstBase = 0x1100;		// �����ڵ� �ʼ� '��'
			int middleBase = 0x1161;	// �����ڵ� �߼� '��'
			int endBase = 0x11A8;		// �����ڵ� ���� '��'
			int a = jamo[i].first + firstBase;
			int b = jamo[i].middle + middleBase;
			int c = jamo[i].end + endBase - 1;
			if (countPrint == 0) {
				printf("\t�ʼ�:�߼�:����(�����ڵ� ��) : U+%x : U+%x : U+%x\n", a, b, c);
				cout << "\t�ʼ� : " << first[jamo[i].first] << "\t�߼� : " << middle[jamo[i].middle] << "\t���� : " << end[jamo[i].end] << endl;
			}
		}
		countPrint++;
		homeWork5(jamo);
	}

	//else if (flag) { // �����Լ�6�� ���� �� �Ķ��
	//	int i = 1;
	//	if (isWhat == "EUC-KR" || isWhat == "UTF-8")
	//		i = 0;
	//	cout << "6-1 ��/��/�������� Ǯ� ����" << endl;
	//	for (; i < buffer16BELen; i++)// ó������ BOM�� ��������Ƿ� 1����
	//		cout << first[jamo[i].first] << middle[jamo[i].middle] << end[jamo[i].end] << " ";
	//	cout << endl << "6-2 �ʼ��鸸 ���" << endl;
	//	i = 1;
	//	if (isWhat == "EUC-KR" || isWhat == "UTF-8")
	//		i = 0;
	//	for (; i < buffer16BELen; i++)// ó������ BOM�� ��������Ƿ� 1����
	//		cout << first[jamo[i].first] << " ";
	//	cout << endl;
	//}
}

void homeWork5(class Jamo *jamo) {
	//cout << "���� (5��) �� 4���� ����ü �迭 ������ UTF-16(BE) �ѱ� ��Ʈ������ ������ ��� ��. (���� feff�� UTF-16BE�� BOM)" << endl;
	int *combine = new int[buffer16BELen]; // ��ġ�°� ������ ��ó�� �Ųٷ� ���ָ� �ȴ�.
	for (int i = 0; i < buffer16BELen; i++) {
		combine[i] = 0xac00 + 28 * 21 * jamo[i].first + 28 * jamo[i].middle + jamo[i].end;
		//printf("%x ", combine[i]);
	}
	//cout << endl << endl;
}

void homeWork6() {
	//cout << "���� (6��)  ������ �ؽ�Ʈ�� ��/��/�������� Ǯ� ���·� ��� �� �ʼ��鸸 ����ϴ� ���α׷� �ۼ�." << endl;
	flag = true;
	homeWork4();
}

void HW2() {
	//int i = 1;
	//if (!check16) 
	//	i = 0;
	//for (; i < buffer16BELen; i++)  // ó������ BOM�� ��������Ƿ� 1����
	//	printf("\t�Է����ʼ�:�߼�:����(offset ��) : \t%d\t %d\t %d\n", userjamo[i].first, userjamo[i].middle, userjamo[i].end);
	ifstream instream("input.txt");
	if (!instream.is_open()) {
		cout << "input.txt�� �� �� �����ϴ�." << endl;
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
		if (!EUC_KR) { // EUC-KR�� �ƴ϶��
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
		//if(!checkNoHanGul(buffer, bufferLen)) { // �ѱ��� �ƴ� ���ڰ� �ϳ��� �� ������
		//	cout << "�Է¹��� ���ڿ��� �ѱ��� �ƴѰ��� �����Ƿ� UTF-16BE�� ��ȯ�� ���� �ʰ� �����մϴ�." << endl;
		//	return 0;
		//}
		compare(buffer, bufferLen);
		if (isWhat != "UTF-16BE")  // UTF-16BE�� �ƴ϶�� UTF-16BE�� ��ȯ���������
			convert(buffer, bufferLen);

		homeWork4(); // ����(5���� homeWork4() �Լ��ȿ� ȣ�� �ϴ� ���� �ֽ��ϴ�.)
		homeWork6();
		
		//for (int i = 0; i < buffer16BELen; i++)  // ó������ BOM�� ��������Ƿ� 1����
		//	printf("\t�������ʼ�:�߼�:����(offset ��) : \t%d\t %d\t %d\n", dicjamo[i].first, dicjamo[i].middle, dicjamo[i].end);
		int count = 0, realcount = 0;
		
		if (check16) { // UTF-16 �迭�� ��� �Ǿտ� 000�� �ֱ� ������ ��ĭ�� ������ �����ִ� �۾��� ������ϰ� ���߿� ��ü ���̵� -1 �������
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
			// ���⼭���� �ʼ�
			if (dicjamo[i].first == 0) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 0) {
					count++;
					if (userjamo[i].first == 3 || userjamo[i].first == 9) { // ��,�� �� ���
						realcount++;
					}
				}
			}
			if (dicjamo[i].first == 2) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 2) {
					count++;
					if (userjamo[i].first == 6 || userjamo[i].first == 11) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 3) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 3) {
					count++;
					if (userjamo[i].first == 0 || userjamo[i].first == 12) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 5) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 5) {
					count++;
					if (userjamo[i].first == 18 || userjamo[i].first == 11) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 6) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 6) {
					count++;
					if (userjamo[i].first == 2) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 7) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 7) {
					count++;
					if (userjamo[i].first == 12) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 9) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 9) {
					count++;
					if (userjamo[i].first == 0) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 11) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 11) {
					count++;
					if (userjamo[i].first == 2 || userjamo[i].first == 5) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 12) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 12) {
					count++;
					if (userjamo[i].first == 7 || userjamo[i].first == 3) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 14) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 14) {
					count++;
					if (userjamo[i].first == 16 || userjamo[i].first == 17) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 15) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 15) {
					count++;
					if (userjamo[i].first == 16) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 16) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 16) {
					count++;
					if (userjamo[i].first == 15 || userjamo[i].first == 14) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 17) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 17) {
					count++;
					if (userjamo[i].first == 14) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].first == 18) { // ������ �ʼ��� ���� ��
				if (userjamo[i].first != 18) {
					count++;
					if (userjamo[i].first == 5) // �� �� ���
						realcount++;
				}
			}
			// ���⼭���� �߼�
			if (dicjamo[i].middle == 0) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 0) {
					count++;
					if (userjamo[i].middle == 4 || userjamo[i].middle == 20) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 1) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 1) {
					count++;
					if (userjamo[i].middle == 2 || userjamo[i].middle == 5) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 2) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 2) {
					count++;
					if (userjamo[i].middle == 6 || userjamo[i].middle == 1) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 4) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 4) {
					count++;
					if (userjamo[i].middle == 8 || userjamo[i].middle == 0) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 5) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 5) {
					count++;
					if (userjamo[i].middle == 1) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 6) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 6) {
					count++;
					if (userjamo[i].middle == 12 || userjamo[i].middle == 2) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 8) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 8) {
					count++;
					if (userjamo[i].middle == 4) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 12) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 12) {
					count++;
					if (userjamo[i].middle == 6) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 13) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 13) {
					count++;
					if (userjamo[i].middle == 17 || userjamo[i].middle == 18) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 17) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 17) {
					count++;
					if (userjamo[i].middle == 13) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 18) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 18) {
					count++;
					if (userjamo[i].middle == 13) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].middle == 20) { // ������ �߼��� ���� ��
				if (userjamo[i].middle != 20) {
					count++;
					if (userjamo[i].middle == 0) // �� �� ���
						realcount++;
				}
			}
			// ���⼭���� ����
			if (dicjamo[i].end == 1) { // ������ ������ ���� ��
				if (userjamo[i].end != 1) {
					count++;
					if (userjamo[i].end == 7 || userjamo[i].end == 19) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 4) { // ������ ������ ���� ��
				if (userjamo[i].end != 4) {
					count++;
					if (userjamo[i].end == 16 || userjamo[i].end == 21) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 7) { // ������ ������ ���� ��
				if (userjamo[i].end != 7) {
					count++;
					if (userjamo[i].end == 22 || userjamo[i].end == 1) //��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 8) { // ������ ������ ���� ��
				if (userjamo[i].end != 8) {
					count++;
					if (userjamo[i].end == 21 || userjamo[i].end == 27) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 16) { // ������ ������ ���� ��
				if (userjamo[i].end != 16) {
					count++;
					if (userjamo[i].end == 4) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 17) { // ������ ������ ���� ��
				if (userjamo[i].end != 17) {
					count++;
					if (userjamo[i].end == 22) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 19) { // ������ ������ ���� ��
				if (userjamo[i].end != 19) {
					count++;
					if (userjamo[i].end == 1) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 21) { // ������ ������ ���� ��
				if (userjamo[i].end != 21) {
					count++;
					if (userjamo[i].end == 4 || userjamo[i].end == 8) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 22) { // ������ ������ ���� ��
				if (userjamo[i].end != 22) {
					count++;
					if (userjamo[i].end == 17 || userjamo[i].end == 7) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 23) { // ������ ������ ���� ��
				if (userjamo[i].end != 23) {
					count++;
					if (userjamo[i].end == 25 || userjamo[i].end == 26) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 24) { // ������ ������ ���� ��
				if (userjamo[i].end != 24) {
					count++;
					if (userjamo[i].end == 25) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 25) { // ������ ������ ���� ��
				if (userjamo[i].end != 25) {
					count++;
					if (userjamo[i].end == 24 || userjamo[i].end == 23) // ��,�� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 26) { // ������ ������ ���� ��
				if (userjamo[i].end != 26) {
					count++;
					if (userjamo[i].end == 23) // �� �� ���
						realcount++;
				}
			}
			if (dicjamo[i].end == 27) { // ������ ������ ���� ��
				if (userjamo[i].end != 27) {
					count++;
					if (userjamo[i].end == 8) // �� �� ���
						realcount++;
				}
			}
		}
		if (count == 1 && realcount == 1) {
			cout << " [���� 2] ���� 1���� ������ �ѱ۹��� Ǯ��� ��� ����-���� ����\n";
			cout << "������ ������ �ܾ�� " << buffer << "�Դϴ�." << endl;
			exit(1);
		}
		else if (count == 0) {
			cout << " [���� 2] ���� 1���� ������ �ѱ۹��� Ǯ��� ��� ����-���� ����\n";
			cout << buffer << "�� ������ ������ ��ġ�ϴ� �ܾ��Դϴ�." << endl;
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