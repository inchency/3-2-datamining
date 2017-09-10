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
	cout << "��� ��ü�� ������?" << endl;
	cin >> num;
	int **simTable = new int*[num];
	for (int i = 0; i < num; i++)
		simTable[i] = new int[num];
	for (int i = 0; i < num; i++)
		for (int j = 0; j < num; j++)
			simTable[i][j] = NULL;
	// make upper triangulur matrix
	srand((unsigned int)time(0)); // �Ź� �ٸ� ������ ��� ���� �ð� ���
	cout << "������ �����ؼ� upper triangulur matrix ���·� �� ��ü���� ���絵�� �����帮�ڽ��ϴ�." << endl;
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
		cout << endl << "1. SLink�� Cluster ������" << endl << "2. CLink�� Cluster ������" << endl << "3. ���α׷� ����" << endl;
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
			cout << "���α׷��� �����մϴ�." << endl;
			exit(1);
		default:
			cout << "1~3�� �˸��� ���ڸ� �ٽ� �Է��� �ּ���." << endl;
		}
	}
}

void sLink(int **T, int num) {
	int k;
	cout << "�� ���� Ŭ����Ʈ�� ������?" << endl;
	cin >> k;
	cout << k << "���� Ŭ����Ʈ�� ����ڽ��ϴ�." << endl << "Ŭ����Ʈ�� ���� ������ ��ü" << k << "���� �Է� �� �ּ���." << endl;
	int *ranNum = new int[k];
	cluster *clust = new cluster[k];
	for (int i = 0; i < k; i++)
		for (int j = 0; j < MAX_SIZE; j++)
			clust[i].ary[j] = NULL;

	for (int i = 0; i < k; i++) {
		cout << "���� �Է� �� �ּ���." << endl;
		cin >> ranNum[i];
		if (ranNum[0] >= num) {
			cout << num - 1 << "���Ϸ� �ٽ� �Է��� �ּ���.";
			i--;
		}
		for (int j = 0; j<i; j++)
			if (ranNum[j] == ranNum[i]) {
				cout << "�ߺ��Ǵ� ���� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ���!!" << endl;
				i--;
			}
	}

	// ���̺� ��¹�
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
	for (int i = 0; i < num; i++) // check �ʱ�ȭ
		check[i] = true;
	for (int i = 0; i < k; i++)
		check[ranNum[i]] = false;
	// clust�� �ʱⰪ �ֱ�
	for (int i = 0; i < k; i++)
		clust[i].ary[0] = ranNum[i];
	cout << "SLink �ʱⰪ �Դϴ�." << endl;
	for (int j = 0; j < k; j++) {
		for (int l = 0; l < MAX_SIZE; l++) {
			if (clust[j].ary[l] == NULL)
				break;
			cout << clust[j].ary[l] << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < num; i++) { // i�� ��ü�� ���������� ���س����� �ε���
		if (!check[i])
			continue;
		for (int j = 0; j < k; j++) { // j�� class clust�� index
			for (int l = 0; l < MAX_SIZE; l++) { // l�� clust ���ι迭ary�� �ε���
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
		cout << "SLink �߰��Ǵ� ���� �����ֱ�" << endl;
		for (int j = 0; j < k; j++) {
			for (int l = 0; l < MAX_SIZE; l++) {
				if (clust[j].ary[l] == NULL)
					break;
				cout << clust[j].ary[l] << " ";
			}
			cout << endl;
		}
	}
	cout << "------------------SLink ������-----------------------" << endl;
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
	cout << "�� ���� Ŭ����Ʈ�� ������?" << endl;
	cin >> k;
	cout << k << "���� Ŭ����Ʈ�� ����ڽ��ϴ�." << endl << "Ŭ����Ʈ�� ���� ������ ��ü" << k << "���� �Է� �� �ּ���." << endl;
	int *ranNum = new int[k];
	cluster *clust = new cluster[k];
	for (int i = 0; i < k; i++)
		for (int j = 0; j < MAX_SIZE; j++)
			clust[i].ary[j] = NULL;

	for (int i = 0; i < k; i++) {
		cout << "���� �Է� �� �ּ���." << endl;
		cin >> ranNum[i];
		if (ranNum[0] >= num) {
			cout << num - 1 << "���Ϸ� �ٽ� �Է��� �ּ���.";
			i--;
		}
		for (int j = 0; j<i; j++)
			if (ranNum[j] == ranNum[i]) {
				cout << "�ߺ��Ǵ� ���� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ���!!" << endl;
				i--;
			}
	}

	// ���̺� ��¹�
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
	for (int i = 0; i < num; i++) // check �ʱ�ȭ
		check[i] = true;
	for (int i = 0; i < k; i++)
		check[ranNum[i]] = false;
	// clust�� �ʱⰪ �ֱ�
	for (int i = 0; i < k; i++)
		clust[i].ary[0] = ranNum[i];
	cout << "CLink �ʱⰪ �Դϴ�." << endl;
	for (int j = 0; j < k; j++) {
		for (int l = 0; l < MAX_SIZE; l++) {
			if (clust[j].ary[l] == NULL)
				break;
			cout << clust[j].ary[l] << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < num; i++) { // i�� ��ü�� ���������� ���س����� �ε���
		if (!check[i])
			continue;
		for (int j = 0; j < k; j++) { // j�� class clust�� index
			for (int l = 0; l < MAX_SIZE; l++) { // l�� clust ���ι迭ary�� �ε���
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
		int min = clust[0].max; // max �߿��� �׳��� ���� �������� min�̶� ����
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
		cout << "CLink �߰��Ǵ� ���� �����ֱ�" << endl;
		for (int j = 0; j < k; j++) {
			for (int l = 0; l < MAX_SIZE; l++) {
				if (clust[j].ary[l] == NULL)
					break;
				cout << clust[j].ary[l] << " ";
			}
			cout << endl;
		}
	}
	cout << "------------------CLink ������-----------------------" << endl;
	for (int j = 0; j < k; j++) {
		for (int l = 0; l < MAX_SIZE; l++) {
			if (clust[j].ary[l] == NULL)
				break;
			cout << clust[j].ary[l] << " ";
		}
		cout << endl;
	}
}