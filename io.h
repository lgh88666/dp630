#pragma once
#include"zmotion.h"
#include"zauxdll2.h"
#include<string>
using namespace std;

class IOControl {
public:
	int DIA[16];
	
	int DIB[16];

	int DOA[16];

	int DOB[16];
	//��ȡ�����������ź�
	int getInput(ZMC_HANDLE handle, string interface1);
	
	//���ö�Ӧģ�������ź�
	int setOutput(ZMC_HANDLE handle, int ionum, int value, string interface1);

	int getOutPut(ZMC_HANDLE handle, string interface1);
};

