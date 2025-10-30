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
	//��е����ϵͳ״̬��0���ܲ�������ʼ״̬ / ����״̬��1Ready״̬�����Բ�����2���ڵ�������״̬��3�����Զ�����״
	atomic<int> sysState = 0;

	atomic<int> errorCode = 0000;

	atomic<int> axisState[12];

	atomic<bool> needAlarm;

	atomic<bool> alarming;

	atomic<bool> startButtonPressed;

	atomic<bool> homeButtonPressed;

	atomic<bool> cancelButtonPressed;

	//������״̬
	atomic<bool> buzzerState;

	int errorCodeLatch;

	//����һ�����Ʊ����ж��Զ��������̵߳ı���
	atomic<bool> stopRunAutoRroductionFlag = true;

	//�ػ��̺߳�������ȫ�ػ�
	void safeGuard(ZMC_HANDLE& handle,IOControl& ioControl,vector<AxisControl*>& axiss);
	//�շ������̺߳���
	void triggerAlarm(ZMC_HANDLE& handle, IOControl& ioControl, vector<AxisControl*>& axiss);

	//����ͨ��io����io�Ĵ�����
	int generateIOErrorCode(ZMC_HANDLE& handle, IOControl& ioControl);

	//������״̬������Ĵ�����
	int generateAxisErrorCode(ZMC_HANDLE& handle,vector<AxisControl*>& axiss);

	//ϵͳ������ĳ�ʼ��
	void initialAxisParams(ZMC_HANDLE& handle, vector<AxisControl*>& axiss);


















};

