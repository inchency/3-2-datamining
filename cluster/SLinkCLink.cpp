#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define MAX_SIZE 5000
#define NULL -1

class cluster {
public:
	int ary[MAX_SIZE];
	int min = 999999999;
	int max = 0;
	int minIndex;
	int maxIndex;
};

void makeSimTable();
void sLink(int **T, int num);
void cLink(int **T, int num);

int main() {
	makeSimTable();
	return 0;
}

void makeSimTable() {
	int num;
	cout << "몇개의 개체를 만들까요?" << endl;
	cin >> num;
	int **simTable = new int*[num];
	for (int i = 0; i < num; i++)
		simTable[i] = new int[num];
	for (int i = 0; i < num; i++)
		for (int j = 0; j < num; j++)
			simTable[i][j] = NULL;
	// make upper triangulur matrix
	srand((unsigned int)time(0)); // 매번 다른 난수를 사용 위해 시간 사용
	cout << "난수를 생성해서 upper triangulur matrix 형태로 각 개체간의 유사도를 보여드리겠습니다." << endl;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			if (j > i)
				simTable[i][j] = rand();
		}
	}
	cout << "\t";
	for (int i = 0; i < num; i++)
		cout << "d" << i << "\t";
	cout << endl;
	for (int i = 0; i < num; i++) {
		cout << "d" << i << "\t";
		for (int j = 0; j < num; j++) {
			if (j > i)
				cout << simTable[i][j] << "\t";
			else
				cout << "\t";
		}
		cout << endl;
	}
	while (1) {
		cout << endl << "1. SLink로 Cluster 나누기" << endl << "2. CLink로 Cluster 나누기" << endl << "3. 프로그램 종료" << endl;
		int menu;
		cin >> menu;
		switch (menu) {
		case 1:
			sLink(simTable, num);
			break;
		case 2:
			cLink(simTable, num);
			break;
		case 3:
			cout << "프로그램을 종료합니다." << endl;
			exit(1);
		default:
			cout << "1~3에 알맞은 숫자를 다시 입력해 주세요." << endl;
		}
	}
}

void sLink(int **T, int num) {
	int k;
	cout << "몇 개의 클러스트로 만들까요?" << endl;
	cin >> k;
	cout << k << "개의 클러스트로 만들겠습니다." << endl << "클러스트를 만들 임의의 개체" << k << "개를 입력 해 주세요." << endl;
	int *ranNum = new int[k];
	cluster *clust = new cluster[k];
	for (int i = 0; i < k; i++)
		for (int j = 0; j < MAX_SIZE; j++)
			clust[i].ary[j] = NULL;

	for (int i = 0; i < k; i++) {
		cout << "수를 입력 해 주세요." << endl;
		cin >> ranNum[i];
		if (ranNum[0] >= num) {
			cout << num - 1 << "이하로 다시 입력해 주세요.";
			i--;
		}
		for (int j = 0; j<i; j++)
			if (ranNum[j] == ranNum[i]) {
				cout << "중복되는 수를 입력하셨습니다. 다시 입력하세요!!" << endl;
				i--;
			}
	}

	// 테이블 출력문
	cout << "\t";
	for (int i = 0; i < num; i++)
		cout << "d" << i << "\t";
	cout << endl;
	for (int i = 0; i < num; i++) {
		cout << "d" << i << "\t";
		for (int j = 0; j < num; j++) {
			if (j > i)
				cout << T[i][j] << "\t";
			else
				cout << "\t";
		}
		cout << endl;
	}
	bool *check = new bool[num];
	for (int i = 0; i < num; i++) // check 초기화
		check[i] = true;
	for (int i = 0; i < k; i++)
		check[ranNum[i]] = false;
	// clust에 초기값 넣기
	for (int i = 0; i < k; i++)
		clust[i].ary[0] = ranNum[i];
	cout << "SLink 초기값 입니다." << endl;
	for (int j = 0; j < k; j++) {
		for (int l = 0; l < MAX_SIZE; l++) {
			if (clust[j].ary[l] == NULL)
				break;
			cout << clust[j].ary[l] << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < num; i++) { // i는 개체를 순차적으로 비교해나가는 인덱스
		if (!check[i])
			continue;
		for (int j = 0; j < k; j++) { // j는 class clust의 index
			for (int l = 0; l < MAX_SIZE; l++) { // l은 clust 내부배열ary의 인덱스
				if (clust[j].ary[l] == NULL)
					break;
				if (clust[j].ary[l] < i) {
					if (T[clust[j].ary[l]][i] <= clust[j].min) {
						clust[j].min = T[clust[j].ary[l]][i];
						clust[j].minIndex = i;
					}
				}
				else if (clust[j].ary[l] > i) {
					if (T[i][clust[j].ary[l]] <= clust[j].min) {
						clust[j].min = T[i][clust[j].ary[l]];
						clust[j].minIndex = clust[j].ary[l];
					}
				}
			}
		}
		int min = clust[0].min;
		int si, sj = 0;
		for (int j = 1; j < k; j++) {
			if (min >= clust[j].min) {
				min = clust[j].min;
				si = clust[j].minIndex;
				sj = j;
			}
		}
		for (int j = 0; j < MAX_SIZE; j++) {
			if (clust[sj].ary[j] != NULL)
				continue;
			clust[sj].ary[j] = i;
			break;
		}
		cout << "SLink 추가되는 순서 보여주기" << endl;
		for (int j = 0; j < k; j++) {
			for (int l = 0; l < MAX_SIZE; l++) {
				if (clust[j].ary[l] == NULL)
					break;
				cout << clust[j].ary[l] << " ";
			}
			cout << endl;
		}
	}
	cout << "------------------SLink 최종값-----------------------" << endl;
	for (int j = 0; j < k; j++) {
		for (int l = 0; l < MAX_SIZE; l++) {
			if (clust[j].ary[l] == NULL)
				break;
			cout << clust[j].ary[l] << " ";
		}
		cout << endl;
	}
}

void cLink(int **T, int num) {
	int k;
	cout << "몇 개의 클러스트로 만들까요?" << endl;
	cin >> k;
	cout << k << "개의 클러스트로 만들겠습니다." << endl << "클러스트를 만들 임의의 개체" << k << "개를 입력 해 주세요." << endl;
	int *ranNum = new int[k];
	cluster *clust = new cluster[k];
	for (int i = 0; i < k; i++)
		for (int j = 0; j < MAX_SIZE; j++)
			clust[i].ary[j] = NULL;

	for (int i = 0; i < k; i++) {
		cout << "수를 입력 해 주세요." << endl;
		cin >> ranNum[i];
		if (ranNum[0] >= num) {
			cout << num - 1 << "이하로 다시 입력해 주세요.";
			i--;
		}
		for (int j = 0; j<i; j++)
			if (ranNum[j] == ranNum[i]) {
				cout << "중복되는 수를 입력하셨습니다. 다시 입력하세요!!" << endl;
				i--;
			}
	}

	// 테이블 출력문
	cout << "\t";
	for (int i = 0; i < num; i++)
		cout << "d" << i << "\t";
	cout << endl;
	for (int i = 0; i < num; i++) {
		cout << "d" << i << "\t";
		for (int j = 0; j < num; j++) {
			if (j > i)
				cout << T[i][j] << "\t";
			else
				cout << "\t";
		}
		cout << endl;
	}
	bool *check = new bool[num];
	for (int i = 0; i < num; i++) // check 초기화
		check[i] = true;
	for (int i = 0; i < k; i++)
		check[ranNum[i]] = false;
	// clust에 초기값 넣기
	for (int i = 0; i < k; i++)
		clust[i].ary[0] = ranNum[i];
	cout << "CLink 초기값 입니다." << endl;
	for (int j = 0; j < k; j++) {
		for (int l = 0; l < MAX_SIZE; l++) {
			if (clust[j].ary[l] == NULL)
				break;
			cout << clust[j].ary[l] << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < num; i++) { // i는 개체를 순차적으로 비교해나가는 인덱스
		if (!check[i])
			continue;
		for (int j = 0; j < k; j++) { // j는 class clust의 index
			for (int l = 0; l < MAX_SIZE; l++) { // l은 clust 내부배열ary의 인덱스
				if (clust[j].ary[l] == NULL)
					break;
				if (clust[j].ary[l] < i) {
					if (T[clust[j].ary[l]][i] >= clust[j].max) {
						clust[j].max = T[clust[j].ary[l]][i];
						clust[j].maxIndex = i;
					}
				}
				else if (clust[j].ary[l] > i) {
					if (T[i][clust[j].ary[l]] >= clust[j].max) {
						clust[j].max = T[i][clust[j].ary[l]];
						clust[j].maxIndex = clust[j].ary[l];
					}
				}
			}
		}
		int min = clust[0].max; // max 중에서 그나마 제일 작은것을 min이라 정의
		int si, sj = 0;
		for (int j = 1; j < k; j++) {
			if (min >= clust[j].max) {
				min = clust[j].max;
				si = clust[j].maxIndex;
				sj = j;
			}
		}
		for (int j = 0; j < MAX_SIZE; j++) {
			if (clust[sj].ary[j] != NULL)
				continue;
			clust[sj].ary[j] = i;
			break;
		}
		cout << "CLink 추가되는 순서 보여주기" << endl;
		for (int j = 0; j < k; j++) {
			for (int l = 0; l < MAX_SIZE; l++) {
				if (clust[j].ary[l] == NULL)
					break;
				cout << clust[j].ary[l] << " ";
			}
			cout << endl;
		}
	}
	cout << "------------------CLink 최종값-----------------------" << endl;
	for (int j = 0; j < k; j++) {
		for (int l = 0; l < MAX_SIZE; l++) {
			if (clust[j].ary[l] == NULL)
				break;
			cout << clust[j].ary[l] << " ";
		}
		cout << endl;
	}
}