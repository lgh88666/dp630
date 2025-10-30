#pragma once
#include"zauxdll2.h"
#include"zmotion.h"
#include"io.h"
#include"system.h"
#include<string>
#include<Windows.h>
using namespace std;
class SystemControl;
class AxisControl {
public:
	int axisId;
	int type;
	int units;
	float speed[3];
	float accel[3]; 
	float decel[3];
	float startMoveSpeed[3];
	int axisStatus;
	float fsLimit;
	float rsLimit;
	float sramp;
	int fhLimitId;
	int rhLimitId;
	float creep;
	float homeOffPos;
	int homeInId;
	int axisEnable;
	float idle = -1;
	float maxPos;
	float standByPos;
	float maxSpeed[3];
	float maxAccelDecel[3];
	float dPos;
	float mPos;
	float ePos;
	int fsLimitState;
	int rsLimitState;
	uint32 fhLimitState;
	uint32 rhLimitState;
	int axisErrorCode = 0000;

	//��¼�����״̬
	bool hasRunOnce = false;

	//���캯��
	AxisControl(int axisId);

	//��ȡ�����
	int getAxisParams(ZMC_HANDLE handle, string paramsName);

	//���������(int���������)
	int setIntAxisParams(ZMC_HANDLE handle, string paramsName, int value);

	//���������(float���������)
	int setFloatAxisParams(ZMC_HANDLE handle, string paramsName, float value);

	//������˶�
	void homeAxis(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss);

	//ȫ���ԭ��
	void homeAllAxis(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss);

	//����ƶ��߼�
	void axisMove(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss, int type, int direction, float distance);

private:
	//���³�Ա�������������������

	//��������������������
	//1����һ����ִ�л���ָ��
	//2���ڶ�����ִ���˶�ָ�����λ��
	void homeFirstAndSecondAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl, float distance);

	//���㶯���ֽ�
	//����һ����ִ�л���ָ��
	void homeFirstAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl);

	//���㶯���ֽ�
	//���ڶ�����ִ���˶�ָ�����λ��
	void homeSecondAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl, float distance);

	//�ظ������װ
	//��������ж��߼�
	void homeTypeAxis(ZMC_HANDLE& handle, IOControl& ioControl,vector<AxisControl*>& axiss);

	
};
