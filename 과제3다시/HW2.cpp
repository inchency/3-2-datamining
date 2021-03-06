#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <atlconv.h> // EUC-KR->UTF-8 변환할때 씀
#include <string>

#define FILE_NAME "949.txt"
#define MAX_SIZE 1000
using namespace std;

void readchar(ifstream &); // input.txt 읽는 함수
void convertBin(int w); // 10진수를 이용해 2진수로 변환해주는 함수
void printBin(int *w, int len);
void printOct(char *w, int len); // 8진수로 출력
void printDec(char *w, int len); // 10진수로 출력
void printHex(char *w, int len); // 16진수로 출력
string compareEUCKR(char *w, int len); // 처음에 EUC-KR인지 아닌지 확인해주는 함수
bool checkOnlyOne(char *w, int len); // 한글자인지 여러글자인지 판별해주는 함수
string compareOnlyOne(char *w, int len); // 문자열을 인자로 전달하여 그 문자열의 한글코드가 무엇인지 바이트를 계산해 자동으로 인식하는 함수 (단 한글자 일때만 가능)
void compare(char *w, int len); // 어떠한 한글코드인지 판별 (2글자이상 가능, BOM 사용)
void convert8BintoUniBin(int *w, int len); // UTF-8 2진수 -> Unicode 2진수로 변환해주는 함수
void convertUniBintoDec(int *w, int len); // Unicode 2진수 -> 10진수로 변환해주는 함수 (= UTF-16BE)
void convert(char *w, int len); // UTF16BE가 아닐때 변환해주는 함수
void convertLEtoBE(char *w, int len); // UTF16LE->BE 변환 함수
void convertEUCto8(int *in, int nOut); // EUC-KR->UTF-8 변환 함수
bool checkNoHanGul(char *w, int len); // 한글이 아닌 문자가 하나라도 들어가 있으면 false 반환
void homeWork4(); // 과제(4번) 함수 (UTF-16(BE)로 된 한글 문자열을 초성(0~18), 중성(0~20), 종성(0~27)으로 분리하는 함수 작성.)
void homeWork5(class Jamo *jamo); // 과제(5번) 위 4번의 구조체 배열 내용을 UTF - 16(BE)한글 스트링으로 조합하는 함수 작성.
void homeWork6(); // 과제(6번) 위 함수들을 이용하여 임의의 한글코드로 작성된 한글 텍스트 파일의 내용을 초/중/종성으로 풀어쓴 형태로 출력 및 초성들만 출력하는 프로그램 작성.
void HW2(); // [과제 2] 과제 1에서 구현한 "한글문장 풀어쓰기" 기능 수정-보완

			// 밑에는 편하게 사용하기위해 전역변수로 선언
char buffer[MAX_SIZE] = { NULL }; // 문자열을 받아 buffer 배열에 저장
int bufferNew[MAX_SIZE] = { NULL }; // buffer[i] - 0xffffff00; 할거임
int bufferReal[MAX_SIZE] = { NULL }; // 8글자를 배열1칸에 넣을 진짜 2진수를 저장할 배열
int bufferUniDec[MAX_SIZE] = { NULL }; // 유니 10진수 저장하는 배열
int bufferUniDecLen = 0;
int buffer16BE[MAX_SIZE] = { NULL }; // 변환된 UTF-16BE 담는 배열
int buffer16BELen = 0;
int buffer8[MAX_SIZE] = { NULL }; // 변환된 UTF-8 담는 배열
int bufferLen; // buffer의 길이
bool EUC_KR;
int bufferBin[MAX_SIZE] = { NULL };
int bufferBinLen; // 2진수buffer의 길이
int uniBin[MAX_SIZE] = { NULL }; // 유니코드 2진수로나타낸 배열
int iBin = 0;
int start = 0; // convert8BintoUniBin에서 쓰이며 convertUniBintoDec에서 배열의 길이로쓰임
bool flag = false; // 과제4 함수에서 과제6 함수가 쓰인 상태라면 (초성만 + (초성,중성,종성)) printf문을 출력하고 안쓰인 상태라면 출력 결과값이 숫자로만 나오게끔 함
string isWhat; // 어떠한 한글코드인지 나타내는 변수
string UTF8 = "이 문자열 한글코드는 UTF-8 입니다\n";
string UTF16be = "이 문자열 한글코드는 UTF-16(BE)  입니다\n";
string UTF16le = "이 문자열 한글코드는 UTF-16(LE)  입니다\n";
string EUCKR = "이 문자열 한글코드는 EUC-KR(또는 CP949) 입니다\n";
bool check16 = false; // 입력받는 값이 utf-16종류이면 true 아니면 false
bool check16check = true; // check16 판별하기위해 써줌
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
	if (!EUC_KR) { // EUC-KR이 아니라면
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
	//if(!checkNoHanGul(buffer, bufferLen)) { // 한글이 아닌 문자가 하나라도 들어가 있으면
	//	cout << "입력받은 문자열이 한글이 아닌것이 있으므로 UTF-16BE로 변환을 하지 않고 종료합니다." << endl;
	//	return 0;
	//}
	compare(buffer, bufferLen);
	if (isWhat != "UTF-16BE")  // UTF-16BE가 아니라면 UTF-16BE로 변환시켜줘야함
		convert(buffer, bufferLen);
	cout << endl;
	// 과제 (4번) 함수 호출부분
	homeWork4(); // 과제(5번은 homeWork4() 함수안에 호출 하는 곳이 있습니다.)
	homeWork6();
	HW2();
	cout << " [과제 2] 과제 1에서 구현한 한글문장 풀어쓰기 기능 수정-보완 시작\n";
	cout << "사전과 유사한 단어가 없습니다." << endl;
	return 0;
}
void readchar(ifstream & instream) {
	instream.open(FILE_NAME);
	if (!instream.is_open()) {
		cout << "input.txt를 열 수 없습니다." << endl;
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
	cout << "입력받은 문자열 :   " << endl << buffer << endl;
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
	cout << "위에서 입력받은 문자열을 2진수로 나타나겠습니다." << endl;
	for (int i = 0; i < bufferBinLen; i++) {
		if (i % 8 == 0 && i != 0) // 2진수 이쁘게 출력 하기위한 if문 8비트마다 띄어서 구분
			cout << " ";
		printf("%d", w[i]);
	}
	cout << endl;
}
void printOct(char *w, int len) {
	cout << "위 문자열을 8진수로 나타나겠습니다." << endl;
	for (int i = 0; i<len; i++)
		printf("O%o ", w[i]);
	cout << endl;
}
void printDec(char *w, int len) {
	cout << "위 문자열을 10진수로 나타나겠습니다." << endl;
	for (int i = 0; i<len; i++)
		printf("%d ", w[i]);
	cout << endl;
}
void printHex(char *w, int len) {
	cout << "위 문자열을 16진수로 나타나겠습니다." << endl;
	for (int i = 0; i<len; i++)
		printf("0x%x ", w[i]);
	cout << endl;
}

string compareOnlyOne(char *w, int len) {
	// 이제 UTF8인지 UTF16인지 EUC-KR 인지 판별을 해준다.
	// 아래는 "각"을 각각 메모장에서 종류별로 저장후 실행시켜 본 결과이다.
	/*	utf - 8
	癤욧컖
	위 문자열을 16진수로 나타나겠습니다.
	0xffffffef 0xffffffbb 0xffffffbf 0xffffffea 0xffffffb0 0xffffff81

	utf - 16be
	? 
	위 문자열을 16진수로 나타나겠습니다.
	0xfffffffe 0xffffffff 0xffffffac 0x1

	utf - 16le
	�
	위 문자열을 16진수로 나타나겠습니다.
	0xffffffff 0xfffffffe 0x1 0xffffffac


	EUC-KR
	각
	위 문자열을 16진수로 나타나겠습니다.
	0xffffffb0 0xffffffa2
	*/
	// 위에서 utf-8은 16진수로 출력하였을 때 6바이트가 나왔고 (BOM3바이트) 3+3n (n은 글자갯수)
	// utf-16BE, utf-16LE는 4바이트 그리고 EUC-KR은 2바이트가 나왔다 (BOM2바이트) 2+2n, EUC-KR은 BOM없음 2n
	// 먼저 바이트 수로 utf-8과 utf-16, EUC-KR을 구분해 주고
	// 그다음 utf-16에서의 LE와 BE는 앞에 첫 바이트 fe로 끝나는지 ff로 끝나는지로 비교를 해주겠다.
	if (len == 2) // "각" -> 0xffffffb0 0xffffffa2 = EUC-KR
		return EUCKR;
	else if (len == 6) // "각" -> 0xffffffef 0xffffffbb 0xffffffbf 0xffffffea 0xffffffb0 0xffffff81 = UTF-8
		return UTF8;
	else if (len == 4) { // "각" -> 0xfffffffe 0xffffffff 0xffffffac 0x1 = UTF-16BE , 0xffffffff 0xfffffffe 0x1 0xffffffac = UTF-16LE
		if (w[0] == 0xfffffffe)
			return UTF16be;
		else if (w[0] == 0xffffffff)
			return UTF16le;
	}
}

bool checkOnlyOne(char *w, int len) {
	if (w[0] == 0xfffffffe || w[0] == 0xffffffff) { // UTF-16일 때
		if (len == 4) // BOM2바이트, 나머지 2바이트 (한 글자 일 때)
			return true;
		else
			return false;
	}
	else if (w[0] == 0xffffffef) { // UTF-8일 때
		if (len == 6) // BOM3바이트, 나머지 3바이트 (한 글자 일 때)
			return true;
		else
			return false;
	}
	else { // EUC-KR일 때
		if (len == 2) // BOM 없음, 나머지 2바이트 (한 글자 일 때)
			return true;
		else
			return false;
	}
}

void compare(char *w, int len) {
	if (w[0] == 0xfffffffe) // UTF-16BE일 때
		isWhat = "UTF-16BE";
	else if (w[0] == 0xffffffff) // UTF-16LE일 때
		isWhat = "UTF-16LE";
	else if (w[0] == 0xffffffef) // UTF-8일 때
		isWhat = "UTF-8";
	else // EUC-KR일 때
		isWhat = "EUC-KR";
}

string compareEUCKR(char *w, int len) {
	// 아스키표에 있는 범위가 들어오면(한글이 아닌 범위가 들어오면) EUC-KR인것
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
		//cout << "입력받은 한글은 EUC-KR이므로 ";
		//cout << "UTF-16BE로의 변환을 시작합니다." << endl << "EUC-KR에서 UTF-16BE로 바로 변환은 어렵기 때문에\nEUC-KR -> UTF-8 -> UTF-16BE 변환 과정을 거치게 됩니다." << endl << endl;
		convertEUCto8(bufferNew, bufferLen);
		for (int i = 0; i < bufferLen*1.5 + 3; i++)
			buffer8[i] = buffer8[i] - 0xffffff00;
		for (int i = 0; i < bufferLen*1.5 + 3; i++)
			convertBin(buffer8[i]); // 2진수로 변환
		convert8BintoUniBin(uniBin, bufferBinLen);
		convertUniBintoDec(uniBin, start);
	}
	else if (isWhat == "UTF-16LE") {
		cout << "입력받은 한글은 UTF-16LE이므로 ";
		cout << "UTF-16BE로의 변환을 시작합니다." << endl << endl;
		convertLEtoBE(buffer, bufferLen);
	}
	else if (isWhat == "UTF-8") {
		cout << "입력받은 한글은 UTF-8이므로 ";
		cout << "UTF-16BE로의 변환을 시작합니다." << endl << endl;
		for (int i = 0; i < bufferLen; i++)
			convertBin(bufferNew[i]); // 2진수로 변환
		printBin(bufferBin, bufferBinLen);
		cout << endl;
		convert8BintoUniBin(uniBin, bufferBinLen);
		convertUniBintoDec(uniBin, start);
	}
	else if (isWhat == "UTF-16BE")
		cout << "이미 UTF-16BE이므로 바꿀 필요가 없습니다" << endl;
}

void convert8BintoUniBin(int *w, int len) { // 000000-00007F 1Byte인 경우
	int next = 24;
	while (next != len) {
		if (bufferBin[next] == 0) { // 000000-00007F 1Byte인 경우
			next++; // 0xxxxxxx 이므로 0 건너뛰고
			w[start++] = 5; // 여기서 5는 임의로 새로운 문자가 시작됨을 알리게 설정한 숫자
			w[start++] = 7; // 몇번을 읽어야 할 것인지 (7번)
			for (int i = 0; i < 7; i++) // 0xxxxxxx 이므로 7번 실행
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 0) { // 000080-0007FF 2Byte인 경우
																								 // 110xxxxx 10xxxxxx
			next += 3; // 110 건너 뛰고 
			w[start++] = 5; // 여기서 5는 임의로 새로운 문자가 시작됨을 알리게 설정한 숫자
			w[start++] = 11; // 몇번을 읽어야 할 것인지 (11번)
			for (int i = 0; i < 5; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 건너 뛰고
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 1 && bufferBin[next + 3] == 0) { // 000800-00FFFF 3Byte인 경우
																															 //1110xxxx 10xxxxxx 10xxxxxx
			next += 4; // 1110 건너 뛰고
			w[start++] = 5; // 여기서 5는 임의로 새로운 문자가 시작됨을 알리게 설정한 숫자
			w[start++] = 16; // 몇번을 읽어야 할 것인지 (16번)
			for (int i = 0; i < 4; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 건너 뛰고
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10 건너 뛰고
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else if (bufferBin[next] == 1 && bufferBin[next + 1] == 1 && bufferBin[next + 2] == 1 && bufferBin[next + 3] == 1 && bufferBin[next + 4] == 0) { // 010000-10FFFF 4Byte인 경우 (SMP)
																																						 //11110zzz 10zzxxxx 10xxxxxx 10xxxxxx
			next += 5; // 11110 건너 뛰고
			w[start++] = 5; // 여기서 5는 임의로 새로운 문자가 시작됨을 알리게 설정한 숫자
			w[start++] = 21; // 몇번을 읽어야 할 것인지 (21번)
			for (int i = 0; i < 3; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10건너 뛰고
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10건너 뛰고
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
			next += 2; // 10건너 뛰고
			for (int i = 0; i < 6; i++)
				w[start++] = bufferBin[next++];
		}
		else {
			//cout << "한글의 문자열을 입력해 주세요." << endl;
			break;
		}
	}
	// 제대로 변환이 되었는지 test용 프린트문
	//int remember; // 몇글자가 들어가는지 기억하는 임시변수
	//cout << "아래는 숫자를 2진수로 출력한 것입니다. 5는 새 문자가 시작됨을 알리며 5 다음수는 몇글자 까지가 그 문자인지를 알려줍니다." << endl;
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
	int remember = w[1]; // 몇글자가 들어가는지 기억하는 임시변수
	int rere; // 임시변수
	int rememberReal; // 진짜 몇글자인지
	int powNum; // n제곱할 수
	int s = 0;
	for (int i = 0; i < len; i++) {
		if (w[i] == 5) {
			rere = remember;
			remember = w[i + 1] + i; // remember까지 한문자로 취급하면 됨
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
	// test용 출력문
	/*cout << "아래 내용은 위의 2진수를 Uni10진수로 표현한 것입니다." << endl;
	for (int i = 0; i < s+1; i++) {
	cout << bufferUniDec[i] << " ";
	}*/
	//cout << "UTF-16으로 변환 완료" << endl << "0xfeff "; //feff는 UTF-16BE의 BOM표시
	bufferUniDecLen = s + 1;
	//for (int i = 0; i < s + 1; i++)
	//	printf("0x%x ", bufferUniDec[i]);
}

void convertLEtoBE(char *w, int len) {
	for (int i = 0; i < len; i += 2) {
		buffer16BE[i] = w[i + 1];
		buffer16BE[i + 1] = w[i];
	}
	cout << "UTF-16으로 변환 완료" << endl;
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
	// BOM 추가하기
	for (int i = nOut*1.5 + 2; i >= 3; i--)
		buffer8[i] = buffer8[i - 3];
	buffer8[0] = 0xffffffef; buffer8[1] = 0xffffffbb; buffer8[2] = 0xffffffbf;
	/*cout << "UTF-8로 변환된 16진수를 출력하겠습니다." << endl;
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
	if (!flag) { // 아직 과제함수6이 실행 되지 않았다면
		if (isWhat == "UTF-16LE") {
			//cout << endl << "16LE에서 실행 시켰을 때 : " << endl;
			for (int i = 0; i < buffer16BELen; i++) {
				if (buffer16BE[i] < 0)
					buffer16BE[i] -= 0xffffff00;
			}
		}
		else if (isWhat == "UTF-16BE") {
			//cout << "16BE에서 실행시켰을 때 : " << endl;
			for (int i = 0; i < bufferLen; i++) {
				if (buffer[i] < 0)
					buffer16BE[i] = buffer[i] - 0xffffff00;
				else
					buffer16BE[i] = buffer[i]; // int로 형변환
			}
			buffer16BELen = bufferLen;
		}
		else if (isWhat == "EUC-KR" || isWhat == "UTF-8") {
			//cout << "EUCKR이나 UTF8에서 실행시켰을 때 : " << endl;
			for (int i = 0; i < bufferUniDecLen; i++)
				buffer16BE[i] = bufferUniDec[i];
			buffer16BELen = bufferUniDecLen;
		}
		// 여기까지 뒤죽박죽이였던 배열을 buffer16BELen 이걸로 통일시킴
		/*for (int i = 0; i < buffer16BELen; i++)
		printf("%x ", buffer16BE[i]);
		cout << endl;*/
		if (isWhat == "UTF-16LE" || isWhat == "UTF-16BE") {
			int index = 0;
			for (int i = 0; i < buffer16BELen; i++) {
				if (i % 2 == 0) // index가 짝수 일 때 그 index의 수 * 16 + 다음수
					buffer16BE[index++] = buffer16BE[i] * 16 * 16 + buffer16BE[i + 1];
				else
					continue;
			}
			buffer16BELen /= 2; // 길이는 절반으로 반토막 남
								// 여기까지 붙이기 완료
		}
		//for (int i = 0; i < buffer16BELen; i++)
		//printf("%x ", buffer16BE[i]);
		//cout << endl;
	}
	Jamo *jamo = new Jamo[buffer16BELen]; // 버퍼의 길이만큼 동적할당
										  /* 초성, 중성, 종성 나누는 공식
										  i = ch - 0xac00;
										  cho = i / (21 * 28);
										  jung = (i / 28) % 21;
										  jong = i % 28;
										  printf("\t초성:중성:종성(offset 값)\t--> %d : %d : %d\n", cho, jung, jong);
										  printf("\t초성:중성:종성(유니코드 값)\t--> U+%x : U+%x : U+%x\n", cho, jung, jong);
										  */
	for (int i = 0; i < buffer16BELen; i++) {
		int word = buffer16BE[i] - 0xac00;
		jamo[i].first = word / (21 * 28);
		jamo[i].middle = (word / 28) % 21;
		jamo[i].end = word % 28;
	}
	string first[19] = { "ㄱ", "ㄲ", "ㄴ", "ㄷ", "ㄸ", "ㄹ", "ㅁ", "ㅂ", "ㅃ", "ㅅ", "ㅆ", "ㅇ", "ㅈ", "ㅉ", "ㅊ", "ㅋ", "ㅌ", "ㅍ", "ㅎ" };
	string middle[21] = { "ㅏ", "ㅐ", "ㅑ", "ㅒ", "ㅓ", "ㅔ", "ㅕ", "ㅖ", "ㅗ", "ㅘ", "ㅙ", "ㅚ", "ㅛ", "ㅜ", "ㅝ", "ㅞ", "ㅟ", "ㅠ", "ㅡ", "ㅢ", "ㅣ" };
	string end[28] = { "", "ㄱ", "ㄲ", "ㄳ", "ㄴ", "ㄵ", "ㄶ", "ㄷ", "ㄹ", "ㄺ", "ㄻ", "ㄼ", "ㄽ", "ㄾ", "ㄿ", "ㅀ", "ㅁ", "ㅂ", "ㅄ", "ㅅ", "ㅆ", "ㅇ", "ㅈ", "ㅊ", "ㅋ", "ㅌ", "ㅍ", "ㅎ" };
	if (!flag) {
		//cout << endl << "과제(4번) 한글 문자열을 초성(0~18), 중성(0~20), 종성(0~27)으로 분리한 결과 값" << endl << endl;
		int i = 1;
		if (isWhat == "EUC-KR" || isWhat == "UTF-8") 
			i = 0;
		else if (check16check) {
			check16 = true;
			check16check = false;
		}
		for (; i < buffer16BELen; i++) { // 처음에는 BOM이 들어있으므로 1부터
			if (countPrint == 0)
				printf("\t초성:중성:종성(offset 값) : \t%d\t %d\t %d\n", jamo[i].first, jamo[i].middle, jamo[i].end);
			if (userFirst) { // 처음엔 userjamo class에 넣는다
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
			int firstBase = 0x1100;		// 유니코드 초성 'ㄱ'
			int middleBase = 0x1161;	// 유니코드 중성 'ㅏ'
			int endBase = 0x11A8;		// 유니코드 종성 'ㄱ'
			int a = jamo[i].first + firstBase;
			int b = jamo[i].middle + middleBase;
			int c = jamo[i].end + endBase - 1;
			if (countPrint == 0) {
				printf("\t초성:중성:종성(유니코드 값) : U+%x : U+%x : U+%x\n", a, b, c);
				cout << "\t초성 : " << first[jamo[i].first] << "\t중성 : " << middle[jamo[i].middle] << "\t종성 : " << end[jamo[i].end] << endl;
			}
		}
		countPrint++;
		homeWork5(jamo);
	}

	//else if (flag) { // 과제함수6이 실행 된 후라면
	//	int i = 1;
	//	if (isWhat == "EUC-KR" || isWhat == "UTF-8")
	//		i = 0;
	//	cout << "6-1 초/중/종성으로 풀어쓴 형태" << endl;
	//	for (; i < buffer16BELen; i++)// 처음에는 BOM이 들어있으므로 1부터
	//		cout << first[jamo[i].first] << middle[jamo[i].middle] << end[jamo[i].end] << " ";
	//	cout << endl << "6-2 초성들만 출력" << endl;
	//	i = 1;
	//	if (isWhat == "EUC-KR" || isWhat == "UTF-8")
	//		i = 0;
	//	for (; i < buffer16BELen; i++)// 처음에는 BOM이 들어있으므로 1부터
	//		cout << first[jamo[i].first] << " ";
	//	cout << endl;
	//}
}

void homeWork5(class Jamo *jamo) {
	//cout << "과제 (5번) 위 4번의 구조체 배열 내용을 UTF-16(BE) 한글 스트링으로 조합한 결과 값. (앞의 feff는 UTF-16BE의 BOM)" << endl;
	int *combine = new int[buffer16BELen]; // 합치는건 나눴을 때처럼 거꾸로 해주면 된다.
	for (int i = 0; i < buffer16BELen; i++) {
		combine[i] = 0xac00 + 28 * 21 * jamo[i].first + 28 * jamo[i].middle + jamo[i].end;
		//printf("%x ", combine[i]);
	}
	//cout << endl << endl;
}

void homeWork6() {
	//cout << "과제 (6번)  임의의 텍스트를 초/중/종성으로 풀어쓴 형태로 출력 및 초성들만 출력하는 프로그램 작성." << endl;
	flag = true;
	homeWork4();
}

void HW2() {
	//int i = 1;
	//if (!check16) 
	//	i = 0;
	//for (; i < buffer16BELen; i++)  // 처음에는 BOM이 들어있으므로 1부터
	//	printf("\t입력의초성:중성:종성(offset 값) : \t%d\t %d\t %d\n", userjamo[i].first, userjamo[i].middle, userjamo[i].end);
	ifstream instream("input.txt");
	if (!instream.is_open()) {
		cout << "input.txt를 열 수 없습니다." << endl;
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
		if (!EUC_KR) { // EUC-KR이 아니라면
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
		//if(!checkNoHanGul(buffer, bufferLen)) { // 한글이 아닌 문자가 하나라도 들어가 있으면
		//	cout << "입력받은 문자열이 한글이 아닌것이 있으므로 UTF-16BE로 변환을 하지 않고 종료합니다." << endl;
		//	return 0;
		//}
		compare(buffer, bufferLen);
		if (isWhat != "UTF-16BE")  // UTF-16BE가 아니라면 UTF-16BE로 변환시켜줘야함
			convert(buffer, bufferLen);

		homeWork4(); // 과제(5번은 homeWork4() 함수안에 호출 하는 곳이 있습니다.)
		homeWork6();
		
		//for (int i = 0; i < buffer16BELen; i++)  // 처음에는 BOM이 들어있으므로 1부터
		//	printf("\t사전의초성:중성:종성(offset 값) : \t%d\t %d\t %d\n", dicjamo[i].first, dicjamo[i].middle, dicjamo[i].end);
		int count = 0, realcount = 0;
		
		if (check16) { // UTF-16 계열일 경우 맨앞에 000이 있기 때문에 한칸씩 앞으로 땡겨주는 작업을 해줘야하고 나중에 전체 길이도 -1 해줘야함
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
			// 여기서부터 초성
			if (dicjamo[i].first == 0) { // 사전에 초성이 ㄱ일 때
				if (userjamo[i].first != 0) {
					count++;
					if (userjamo[i].first == 3 || userjamo[i].first == 9) { // ㄷ,ㅅ 일 경우
						realcount++;
					}
				}
			}
			if (dicjamo[i].first == 2) { // 사전에 초성이 ㄴ일 때
				if (userjamo[i].first != 2) {
					count++;
					if (userjamo[i].first == 6 || userjamo[i].first == 11) // ㅁ,ㅇ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 3) { // 사전에 초성이 ㄷ일 때
				if (userjamo[i].first != 3) {
					count++;
					if (userjamo[i].first == 0 || userjamo[i].first == 12) // ㄱ,ㅈ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 5) { // 사전에 초성이 ㄹ일 때
				if (userjamo[i].first != 5) {
					count++;
					if (userjamo[i].first == 18 || userjamo[i].first == 11) // ㅎ,ㅇ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 6) { // 사전에 초성이 ㅁ일 때
				if (userjamo[i].first != 6) {
					count++;
					if (userjamo[i].first == 2) // ㄴ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 7) { // 사전에 초성이 ㅂ일 때
				if (userjamo[i].first != 7) {
					count++;
					if (userjamo[i].first == 12) // ㅈ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 9) { // 사전에 초성이 ㅅ일 때
				if (userjamo[i].first != 9) {
					count++;
					if (userjamo[i].first == 0) // ㄱ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 11) { // 사전에 초성이 ㅇ일 때
				if (userjamo[i].first != 11) {
					count++;
					if (userjamo[i].first == 2 || userjamo[i].first == 5) // ㄴ,ㄹ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 12) { // 사전에 초성이 ㅈ일 때
				if (userjamo[i].first != 12) {
					count++;
					if (userjamo[i].first == 7 || userjamo[i].first == 3) // ㅂ,ㄷ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 14) { // 사전에 초성이 ㅊ일 때
				if (userjamo[i].first != 14) {
					count++;
					if (userjamo[i].first == 16 || userjamo[i].first == 17) // ㅌ,ㅍ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 15) { // 사전에 초성이 ㅋ일 때
				if (userjamo[i].first != 15) {
					count++;
					if (userjamo[i].first == 16) // ㅌ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 16) { // 사전에 초성이 ㅌ일 때
				if (userjamo[i].first != 16) {
					count++;
					if (userjamo[i].first == 15 || userjamo[i].first == 14) // ㅋ,ㅊ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 17) { // 사전에 초성이 ㅍ일 때
				if (userjamo[i].first != 17) {
					count++;
					if (userjamo[i].first == 14) // ㅊ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].first == 18) { // 사전에 초성이 ㅎ일 때
				if (userjamo[i].first != 18) {
					count++;
					if (userjamo[i].first == 5) // ㄹ 일 경우
						realcount++;
				}
			}
			// 여기서부터 중성
			if (dicjamo[i].middle == 0) { // 사전에 중성이 ㅏ일 때
				if (userjamo[i].middle != 0) {
					count++;
					if (userjamo[i].middle == 4 || userjamo[i].middle == 20) // ㅓ,ㅣ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 1) { // 사전에 중성이 ㅐ일 때
				if (userjamo[i].middle != 1) {
					count++;
					if (userjamo[i].middle == 2 || userjamo[i].middle == 5) // ㅑ,ㅔ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 2) { // 사전에 중성이 ㅑ일 때
				if (userjamo[i].middle != 2) {
					count++;
					if (userjamo[i].middle == 6 || userjamo[i].middle == 1) // ㅕ,ㅐ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 4) { // 사전에 중성이 ㅓ일 때
				if (userjamo[i].middle != 4) {
					count++;
					if (userjamo[i].middle == 8 || userjamo[i].middle == 0) // ㅗ,ㅏ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 5) { // 사전에 중성이 ㅔ일 때
				if (userjamo[i].middle != 5) {
					count++;
					if (userjamo[i].middle == 1) // ㅐ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 6) { // 사전에 중성이 ㅕ일 때
				if (userjamo[i].middle != 6) {
					count++;
					if (userjamo[i].middle == 12 || userjamo[i].middle == 2) // ㅛ,ㅑ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 8) { // 사전에 중성이 ㅗ일 때
				if (userjamo[i].middle != 8) {
					count++;
					if (userjamo[i].middle == 4) // ㅓ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 12) { // 사전에 중성이 ㅛ일 때
				if (userjamo[i].middle != 12) {
					count++;
					if (userjamo[i].middle == 6) // ㅕ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 13) { // 사전에 중성이 ㅜ일 때
				if (userjamo[i].middle != 13) {
					count++;
					if (userjamo[i].middle == 17 || userjamo[i].middle == 18) // ㅠ,ㅡ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 17) { // 사전에 중성이 ㅠ일 때
				if (userjamo[i].middle != 17) {
					count++;
					if (userjamo[i].middle == 13) // ㅜ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 18) { // 사전에 중성이 ㅡ일 때
				if (userjamo[i].middle != 18) {
					count++;
					if (userjamo[i].middle == 13) // ㅜ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].middle == 20) { // 사전에 중성이 ㅣ일 때
				if (userjamo[i].middle != 20) {
					count++;
					if (userjamo[i].middle == 0) // ㅏ 일 경우
						realcount++;
				}
			}
			// 여기서부터 종성
			if (dicjamo[i].end == 1) { // 사전에 종성이 ㄱ일 때
				if (userjamo[i].end != 1) {
					count++;
					if (userjamo[i].end == 7 || userjamo[i].end == 19) // ㄷ,ㅅ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 4) { // 사전에 종성이 ㄴ일 때
				if (userjamo[i].end != 4) {
					count++;
					if (userjamo[i].end == 16 || userjamo[i].end == 21) // ㅁ,ㅇ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 7) { // 사전에 종성이 ㄷ일 때
				if (userjamo[i].end != 7) {
					count++;
					if (userjamo[i].end == 22 || userjamo[i].end == 1) //ㅈ,ㄱ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 8) { // 사전에 종성이 ㄹ일 때
				if (userjamo[i].end != 8) {
					count++;
					if (userjamo[i].end == 21 || userjamo[i].end == 27) // ㅇ,ㅎ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 16) { // 사전에 종성이 ㅁ일 때
				if (userjamo[i].end != 16) {
					count++;
					if (userjamo[i].end == 4) // ㄴ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 17) { // 사전에 종성이 ㅂ일 때
				if (userjamo[i].end != 17) {
					count++;
					if (userjamo[i].end == 22) // ㅈ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 19) { // 사전에 종성이 ㅅ일 때
				if (userjamo[i].end != 19) {
					count++;
					if (userjamo[i].end == 1) // ㄱ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 21) { // 사전에 종성이 ㅇ일 때
				if (userjamo[i].end != 21) {
					count++;
					if (userjamo[i].end == 4 || userjamo[i].end == 8) // ㄴ,ㄹ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 22) { // 사전에 종성이 ㅈ일 때
				if (userjamo[i].end != 22) {
					count++;
					if (userjamo[i].end == 17 || userjamo[i].end == 7) // ㅂ,ㄷ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 23) { // 사전에 종성이 ㅊ일 때
				if (userjamo[i].end != 23) {
					count++;
					if (userjamo[i].end == 25 || userjamo[i].end == 26) // ㅌ,ㅍ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 24) { // 사전에 종성이 ㅋ일 때
				if (userjamo[i].end != 24) {
					count++;
					if (userjamo[i].end == 25) // ㅌ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 25) { // 사전에 종성이 ㅌ일 때
				if (userjamo[i].end != 25) {
					count++;
					if (userjamo[i].end == 24 || userjamo[i].end == 23) // ㅋ,ㅊ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 26) { // 사전에 종성이 ㅍ일 때
				if (userjamo[i].end != 26) {
					count++;
					if (userjamo[i].end == 23) // ㅊ 일 경우
						realcount++;
				}
			}
			if (dicjamo[i].end == 27) { // 사전에 종성이 ㅎ일 때
				if (userjamo[i].end != 27) {
					count++;
					if (userjamo[i].end == 8) // ㄹ 일 경우
						realcount++;
				}
			}
		}
		if (count == 1 && realcount == 1) {
			cout << " [과제 2] 과제 1에서 구현한 한글문장 풀어쓰기 기능 수정-보완 시작\n";
			cout << "사전에 유사한 단어는 " << buffer << "입니다." << endl;
			exit(1);
		}
		else if (count == 0) {
			cout << " [과제 2] 과제 1에서 구현한 한글문장 풀어쓰기 기능 수정-보완 시작\n";
			cout << buffer << "는 사전과 완전히 일치하는 단어입니다." << endl;
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