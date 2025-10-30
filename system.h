#pragma once
#include<iostream>
#include<Windows.h>
#include<vector>
#include<atomic>
#include"io.h"
#include"axis.h"
#include<thread>

using namespace std;

class AxisControl;
class SystemControl {
public:
	//机械控制系统状态：0不能操作，初始状态 / 错误状态、1Ready状态，可以操作、2正在单步运行状态、3正在自动生产状
	atomic<int> sysState = 0;

	atomic<int> errorCode = 0000;

	atomic<int> axisState[12];

	atomic<bool> needAlarm;

	atomic<bool> alarming;

	atomic<bool> startButtonPressed;

	atomic<bool> homeButtonPressed;

	atomic<bool> cancelButtonPressed;

	//蜂鸣器状态
	atomic<bool> buzzerState;

	int errorCodeLatch;

	//定义一个控制报警中断自动化生产线程的变量
	atomic<bool> stopRunAutoRroductionFlag = true;

	//守护线程函数，安全守护
	void safeGuard(ZMC_HANDLE& handle,IOControl& ioControl,vector<AxisControl*>& axiss);
	//诱发报警线程函数
	void triggerAlarm(ZMC_HANDLE& handle, IOControl& ioControl, vector<AxisControl*>& axiss);

	//根据通用io生成io的错误码
	int generateIOErrorCode(ZMC_HANDLE& handle, IOControl& ioControl);

	//根据轴状态生成轴的错误码
	int generateAxisErrorCode(ZMC_HANDLE& handle,vector<AxisControl*>& axiss);

	//系统轴参数的初始化
	void initialAxisParams(ZMC_HANDLE& handle, vector<AxisControl*>& axiss);


















};

