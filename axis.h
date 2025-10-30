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

	//记录回零的状态
	bool hasRunOnce = false;

	//构造函数
	AxisControl(int axisId);

	//读取轴参数
	int getAxisParams(ZMC_HANDLE handle, string paramsName);

	//设置轴参数(int类型轴参数)
	int setIntAxisParams(ZMC_HANDLE handle, string paramsName, int value);

	//设置轴参数(float类型轴参数)
	int setFloatAxisParams(ZMC_HANDLE handle, string paramsName, float value);

	//轴回零运动
	void homeAxis(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss);

	//全轴回原点
	void homeAllAxis(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss);

	//轴的移动逻辑
	void axisMove(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss, int type, int direction, float distance);

private:
	//以下成员方法不允许在类外调用

	//回零完整动作（两步）
	//1、第一步，执行回零指令
	//2、第二步，执行运动指令到待机位置
	void homeFirstAndSecondAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl, float distance);

	//回零动作分解
	//仅第一步，执行回零指令
	void homeFirstAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl);

	//回零动作分解
	//仅第二部，执行运动指令到待机位置
	void homeSecondAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl, float distance);

	//重复代码封装
	//回零类别判断逻辑
	void homeTypeAxis(ZMC_HANDLE& handle, IOControl& ioControl,vector<AxisControl*>& axiss);

	
};
