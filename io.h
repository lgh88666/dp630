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
	//获取传感器输入信号
	int getInput(ZMC_HANDLE handle, string interface1);
	
	//设置对应模块的输出信号
	int setOutput(ZMC_HANDLE handle, int ionum, int value, string interface1);

	int getOutPut(ZMC_HANDLE handle, string interface1);
};

