

#include"axis.h"

AxisControl::AxisControl(int axisId) {
	this->axisId = axisId;
}

int AxisControl::getAxisParams(ZMC_HANDLE handle, string paramsName) {
	if (paramsName == "axisStatus") {
		if (ZAux_Direct_GetAxisStatus(handle, axisId, &axisStatus)) {
			return 0;
		}return 1;
	}
	if (paramsName == "fhLimitState") {
		if (ZAux_Direct_GetIn(handle, fhLimitId, &fhLimitState)) {
			return 0;
		}return 1;
	}
	if (paramsName == "rhLimitState") {
		if (ZAux_Direct_GetIn(handle, rhLimitId, &rhLimitState)) {
			return 0;
		}return 1;
	}
	if (paramsName == "idle") {
		if (ZAux_Direct_GetParam(handle, "IDLE", axisId, &idle)) {
			return 0;
		}return 1;
	}
	if (paramsName == "dpos") {
		if (ZAux_Direct_GetDpos(handle, axisId, &dPos)) {
			return 0;
		}return 1;
	}
	if (paramsName == "mPos") {
		if (ZAux_Direct_GetMpos(handle, axisId, &mPos)) {
			return 0;
		}return 1;
	}

}

int AxisControl::setIntAxisParams(ZMC_HANDLE handle, string paramsName, int value) {
	if (paramsName == "type") {
		if (ZAux_Direct_SetAtype(handle, axisId, value)) {
			return 0;
		}this->type = value;
		return 1;
	}
	if (paramsName == "units") {
		if (ZAux_Direct_SetUnits(handle, axisId, value)) {
			return 0;
		}this->units = value;
		return 1;
	}
	if (paramsName == "fhLimitId") {
		if (ZAux_Direct_SetFwdIn(handle, axisId, value)) {
			return 0;
		}this->fhLimitId = value;
		return 1;
	}
	if (paramsName == "rhLimitId") {
		if (ZAux_Direct_SetRevIn(handle, axisId, value)) {
			return 0;
		}this->rhLimitId = value;
		return 1;
	}
	if (paramsName == "homeInId") {
		if (ZAux_Direct_SetDatumIn(handle, axisId, value)) {
			return 0;
		}this->homeInId = value;
		return 1;
	}
	if (paramsName == "axisEnable") {
		if (ZAux_Direct_SetAxisEnable(handle, axisId, value)) {
			return 0;
		}this->axisEnable = value;
		return 1;
	}
	if (paramsName == "fsLimitState") {
		this->fsLimitState = value;
		return 1;
	}
	if (paramsName == "rsLimitState") {
		this->rsLimitState = value;
	}
	if (paramsName == "axisErrorCode") {
		this->axisErrorCode = value;
	}
}

int AxisControl::setFloatAxisParams(ZMC_HANDLE handle, string paramsName, float value) {
	if (paramsName == "sramp") {
		if (ZAux_Direct_SetSramp(handle, axisId, value)) {
			return 0;
		}this->sramp = value;
	}
	if (paramsName == "speed0") {
		//速度超过设定的最大速度，直接返回0
		if (value > maxSpeed[0]) {
			return 0;
		}
		if (ZAux_Direct_SetSpeed(handle, axisId, value)) {
			return 0;
		}this->speed[0] = value;
		return 1;		
	}
	if (paramsName == "speed1") {
		if (value > maxSpeed[1]) {
			return 0;
		}
		if (ZAux_Direct_SetSpeed(handle, axisId, value)) {
			return 0;
		}this->speed[1] = value;
		return 1;
	}
	if (paramsName == "speed2") {
		if (value > maxSpeed[2]) {
			return 0;
		}
		if (ZAux_Direct_SetSpeed(handle, axisId, value)) {
			return 0;
		}this->speed[2] = value;
		return 1;
	}
	if (paramsName == "accel0") {
		if (value > maxAccelDecel[0]) {
			return 0;
		}
		if (ZAux_Direct_SetAccel(handle, axisId, value)) {
			return 0;
		}this->accel[0] = value;
		return 1;
	}
	if (paramsName == "accel1") {
		if (value > maxAccelDecel[1]) {
			return 0;
		}
		if (ZAux_Direct_SetAccel(handle, axisId, value)) {
			return 0;
		}this->accel[1] = value;
		return 1;
	}
	if (paramsName == "accel2") {
		if (value > maxAccelDecel[2]) {
			return 0;
		}
		if (ZAux_Direct_SetDecel(handle, axisId, value)) {
			return 0;
		}this->accel[2] = value;
		return 1;
	}
	if (paramsName == "decel0") {
		if (value > maxAccelDecel[0]) {
			return 0;
		}
		if (ZAux_Direct_SetDecel(handle, axisId, value)) {
			return 0;
		}this->decel[0] = value;
		return 1;
	}
	if (paramsName == "decel1") {
		if (value > maxAccelDecel[1]) {
			return 0;
		}
		if (ZAux_Direct_SetDecel(handle, axisId, value)) {
			return 0;
		}
		this->decel[1] = value;
		return 1;
	}
	if (paramsName == "decel2") {
		if (value > maxAccelDecel[2]) {
			return 0;
		}
		if (ZAux_Direct_SetAccel(handle, axisId, value)) {
			return 0;
		}this->decel[2] = value;
		return 1;
	}
	if (paramsName == "startMoveSpeed0") {
		if (value > maxSpeed[0]) {
			return 0;
		}
		if (ZAux_Direct_SetLspeed(handle, axisId, value)) {
			return 0;
		}this->startMoveSpeed[0] = value;
		return 1;
	}
	if (paramsName == "startMoveSpeed1") {
		if (value > maxSpeed[1]) {
			return 0;
		}
		if (ZAux_Direct_SetLspeed(handle, axisId, value)) {
			return 0;
		}this->startMoveSpeed[1] = value;
		return 1;
	}
	if (paramsName == "startMoveSpeed2") {
		if (value > maxSpeed[2]) {
			return 0;
		}
		if (ZAux_Direct_SetLspeed(handle, axisId, value)) {
			return 0;
		}this->startMoveSpeed[2] = value;
		return 1;
	}
	if (paramsName == "fsLimit") {
		if (ZAux_Direct_SetFsLimit(handle, axisId, value)) {
			return 0;
		}this->fsLimit = value;
		return 1;
	}
	if (paramsName == "rsLimit") {
		if (ZAux_Direct_SetRsLimit(handle, axisId, value)) {
			return 0;
		}this->rsLimit = value;
		return 1;
	}
	if (paramsName == "creep") {
		//速度不能超过最大速度，creep为回零运动爬行速度
		if (value > maxSpeed[1]) {
			return 0;
		}
		if (ZAux_Direct_SetCreep(handle, axisId, value)) {
			return 0;
		}this->creep = value;
		return 1;
	}
	if (paramsName == "homeOffPos") {
		if (ZAux_BusCmd_SetDatumOffpos(handle, axisId, value)) {
			return 0;
		}this->homeOffPos = value;
		return 1;
	}
	if (paramsName == "dPos") {
		if (ZAux_Direct_SetDpos(handle, axisId, value)) {
			return 0;
		}this->dPos = value;
		return 1;
	}
	if (paramsName == "mPos") {
		if (ZAux_Direct_SetMpos(handle, axisId, value)) {
			return 0;
		}this->mPos = value;
		return 1;
	}
	if (paramsName == "maxPos") {
		this->maxPos = value;
		return 1;
	}
	if (paramsName == "standByPos") {
		this->standByPos = value;
		return 1;
	}
	if (paramsName == "maxSpeed0") {
		this->maxSpeed[0] = value;
		return 1;
	}
	if (paramsName == "maxSpeed1") {
		this->maxSpeed[1] = value;
		return 1;
	}
	if (paramsName == "maxSpeed2") {
		this->maxSpeed[2] = value;
		return 1;
	}
	if (paramsName == "maxAccelDecel0") {
		this->maxAccelDecel[0] = value;
		return 1;
	}
	if (paramsName == "maxAccelDecel1") {
		this->maxAccelDecel[1] = value;
		return 1;
	}
	if (paramsName == "maxAccelDecel2") {
		this->maxAccelDecel[2] = value;
		return 1;
	}
	if (paramsName == "ePos") {
		this->ePos = value;
		return 1;
	}

}

void AxisControl::homeAxis(ZMC_HANDLE& handle, IOControl& ioControl,SystemControl& systemControl,vector<AxisControl*>& axiss) {
	//回零完成后，每个轴需要运动到初始位置的距离数组
	float distance[12] = { 5,5,250,250,250,524.477,524.475,472.336,10,0,5,5 };
	//获取安全线程中轴异常的errorCode
	systemControl.generateAxisErrorCode(handle, axiss);
	int ioErrorcode = systemControl.generateIOErrorCode(handle,ioControl);
	//如果安全门打开，不允许回零操作
	if (ioErrorcode == 1003) {
		//报警
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
		alarm.detach();
		//返回
		return;

	}
	//如果有其他轴出现负向硬限位触发，静止轴的回零操作
	for (AxisControl* axisOne : axiss) {
		if (axisOne->axisErrorCode == 2002) {
			//报警
			systemControl.alarming = true;
			thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl),ref(axiss));
			alarm.detach();
			//返回
			return;
		}
	}
	
	//触发正向硬限位，允许回零，回零操作需要二次进行。
	if (axisErrorCode == 2001 ) {
			if (!hasRunOnce) {
				homeTypeAxis(handle, ioControl, axiss);
				/*if (axisId == 0 || axisId == 1) {
					//相关关联轴回零,轴10和轴11回零
					AxisControl axis10 = axis.at(10);
					AxisControl axis11 = axis.at(11);
					thread axis10Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
					thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
					axis10Move.join();
					axis11Move.join();
				}
				if (axisId == 5) {
					AxisControl axis10 = axis.at(10);
					thread axisMove(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
					axisMove.join();
					Sleep(1000);
				}
				if (axisId == 6) {
					AxisControl axis11 = axis.at(11);
					thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
					axis11Move.join();
					Sleep(1000);
				}*/
				//相关轴回完，需要报警，进行二次回零
				//启动报警线程
				systemControl.alarming = true;
				thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
				alarm.detach();
				hasRunOnce = true;
				return;
			}
			else {
				homeTypeAxis(handle, ioControl, axiss);
				/*if (axisId == 0 || axisId == 1) {
					//相关关联轴回零,轴10和轴11回零
					AxisControl axis10 = axis.at(10);
					AxisControl axis11 = axis.at(11);
					thread axis10Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
					thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
					axis10Move.join();
					axis11Move.join();
				}
				if (axisId == 5) {
					AxisControl axis10 = axis.at(10);
					thread axisMove(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
					axisMove.join();
					Sleep(1000);
				}
				if (axisId == 6) {
					AxisControl axis11 = axis.at(11);
					thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
					axis11Move.join();
					Sleep(1000);
				}*/
				//轴自身回零
				AxisControl* axisOne = axiss.at(axisId);
				homeFirstAndSecondAxis(handle, ioControl, *axisOne, distance[axisId]);
				axisErrorCode = 0000;
				hasRunOnce = false;
			}

	}
	//触发轴的负向硬限位，单轴回零没有效果，全轴回零没有效果，仅报警。
	// 但其他轴的回零不影响
	/*if (axisErrorCode == 2002) {
		//启动报警线程
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl));
		alarm.detach();
		//回零没有效果，直接返回
		return;	
	}*/
	cout << "axisId" << axisId << endl;
	//轴触发正负软限位，允许回零。
	// 需要两次回零操作，第一次回零，报警，相关轴运动，无相关轴的仅报警。
	// 第二次点击回零运动后，轴回零动作开始。
	if (axisErrorCode == 1001 || axisErrorCode == 1002) {
		//对于轴九，触发正负软限位，仅需要一次回零
		if (axisId == 9) {
			AxisControl axisOne = *axiss.at(axisId);
			//执行轴9的回零操作
			homeFirstAndSecondAxis(handle, ioControl, axisOne, distance[axisId]);
			//回零完成后，直接退出
			return;
		}
		//对于其他轴，触发正负软限位，需要两次回零操作
		//hasRunOnce记录轴的回零是否是第一次，默认为false
		if (!hasRunOnce) {
			homeTypeAxis(handle, ioControl, axiss);
			//对于轴0和轴1，两个载台，相关轴先运动一次
			/*if (axisId == 0 || axisId == 1) {
				//相关关联轴回零,轴10和轴11回零
				AxisControl axis10 = axis.at(10);
				AxisControl axis11 = axis.at(11);
				thread axis10Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
				thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
				axis10Move.join();
				axis11Move.join();	
			}
			//对于轴5，上料吸盘前后移动，相关轴先运动一次
			if (axisId == 5 ) {
				//相关轴10开始回零
				AxisControl axis10 = axis.at(10);
				thread axisMove(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
				axisMove.join();
			}
			//对于轴6，下料吸盘前后移动，相关轴先运动一次
			if (axisId == 6) {
				//相关轴11开始回零
				AxisControl axis11 = axis.at(11);
				thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
				axis11Move.join();
			}*/
			//其他轴不涉及相关轴，仅让其报警即可
			//相关轴回完，需要报警，进行二次回零
			//启动报警线程
			systemControl.alarming = true;
			thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
			hasRunOnce = true;
			return;
		}
		//已进行一次回零操作
		else {
			homeTypeAxis(handle, ioControl, axiss);
			//执行轴0或轴1的第二次回零
			/*if (axisId == 0 || axisId == 1) {
				//相关关联轴回零,轴10和轴11回零
				AxisControl axis10 = axis.at(10);
				AxisControl axis11 = axis.at(11);
				thread axis10Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
				thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
				axis10Move.join();
				axis11Move.join();
			}
			
			if (axisId == 5) {
				AxisControl axis10 = axis.at(10);
				thread axisMove(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
				axisMove.join();
				Sleep(1000);
			}

			if (axisId == 6) {
				AxisControl axis11 = axis.at(11);
				thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
				axis11Move.join();
				Sleep(1000);
			}*/
			//轴自身回零
			AxisControl axisOne = *axiss.at(axisId);
			homeFirstAndSecondAxis(handle, ioControl, axisOne,distance[axisId]);
		}	
	}
	//如果没有正负软限位错误，正常回零
	else {
		homeTypeAxis(handle, ioControl, axiss);
		/*if (axisId == 0 || axisId == 1) {
			//相关关联轴回零,轴10和轴11回零
			AxisControl axis10 = axis.at(10);
			AxisControl axis11 = axis.at(11);
			thread axis10Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
			thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
			axis10Move.join();
			axis11Move.join();
		}

		if (axisId == 5) {
			AxisControl axis10 = axis.at(10);
			thread axis10Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
			axis10Move.join();
			Sleep(1000);
		}

		if (axisId == 6) {
			AxisControl axis11 = axis.at(11);
			thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
			axis11Move.join();
			Sleep(1000);
		}*/
		//除轴0和轴1外，其余轴回零不牵扯其他轴的运动
		AxisControl* axisOne = axiss.at(axisId);
		homeFirstAndSecondAxis(handle, ioControl, *axisOne, distance[axisId]);
		hasRunOnce = false;
	
	}
	

}	
	
void AxisControl::homeFirstAndSecondAxis(ZMC_HANDLE & handle, IOControl & ioControl,AxisControl& axisControl,float distance)
{
	//设置对应的回零运动参数
	axisControl.setFloatAxisParams(handle, "startMoveSpeed1", axisControl.startMoveSpeed[1]);
	axisControl.setFloatAxisParams(handle, "speed1", axisControl.speed[1]);
	axisControl.setFloatAxisParams(handle, "accel1", axisControl.accel[1]);
	axisControl.setFloatAxisParams(handle, "decel1", axisControl.decel[1]);

	//设置单轴使能为1
	axisControl.setIntAxisParams(handle, "axisEnable", 1);

	//开始回零运动,回零模式设置为4
	ZAux_Direct_Single_Datum(handle, axisControl.axisId, 4);

	//检测回零运动状态是否完成
	uint32 homestatus = 0;
	while (1) {
		if (homestatus == 0) {
			//设置回零指示灯来回闪烁
			ioControl.setOutput(handle, 226, 1, "DOA");
			Sleep(500);
			ioControl.setOutput(handle, 226, 0, "DOA");
			Sleep(500);
			ZAux_Direct_GetHomeStatus(handle, axisControl.axisId, &homestatus);//获取回零运动完成状态
			if (homestatus == 1) {
				//回零状态为1，开始执行下一步(运动到待机位置)
				axisControl.setFloatAxisParams(handle, "startMoveSpeed0", axisControl.startMoveSpeed[0]);
				axisControl.setFloatAxisParams(handle, "speed0", axisControl.speed[0]);
				axisControl.setFloatAxisParams(handle, "accel0", axisControl.accel[0]);
				axisControl.setFloatAxisParams(handle, "decel0", axisControl.decel[0]);
				ZAux_Direct_Single_MoveAbs(handle, axisControl.axisId, distance);
				break;
			}
		}
	}
	while (1) {
		//设置回零指示灯来回闪烁
		ioControl.setOutput(handle, 226, 1, "DOA");
		Sleep(500);
		ioControl.setOutput(handle, 226, 0, "DOA");
		Sleep(500);
		getAxisParams(handle, "idle");
		if (idle < 0) {
			ioControl.setOutput(handle, 226, 0, "DOA");
			break;
		}
	}
}
	
void AxisControl::homeAllAxis(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss) {
	//所有轴回零前需要检测轴的状态
	//如果出现负向硬限位触发，禁止这个操作
	//如果出现正向硬限位触发，可以执行这个操作
	//软限位触发不影响这些操作
	float distance[12] = { 5,5,250,250,250,524.477,524.475,472.336,10,0,5,5 };
	int ioErrorcode = systemControl.generateIOErrorCode(handle, ioControl);
	//如果安全门打开，不允许回零操作
	if (ioErrorcode == 1001) {
		//报警
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
		alarm.detach();
		//返回
		return;
	}
	//如果有轴触碰到硬限位，也禁止回零运动
	for (AxisControl* axisOne : axiss) {
		if (axisOne->axisErrorCode == 2002) {
			//报警
			systemControl.alarming = true;
			thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
			alarm.detach();
			//返回
			return;
		}
	}
	systemControl.generateAxisErrorCode(handle, axiss);
	vector<int> errorAxisId;
	for (AxisControl* axisOne : axiss) {
		if (axisOne->axisErrorCode == 1001 || axisOne->axisErrorCode == 1002 || axisOne->axisErrorCode == 2001) {
			//拿到错误的轴号
			errorAxisId.push_back(axisOne->axisId);
		}
	}
	//对错误的轴进行统一一次回零
	bool moveAxis10 = false;
	bool moveAxis11 = false;
	thread axis10Move;
	thread axis11Move;
	if (!errorAxisId.empty() && !hasRunOnce) {

		for (int& id : errorAxisId) {
			if (id == 0 || id == 1 && !moveAxis10 && !moveAxis11) {
				AxisControl* axis10 = axiss.at(10);
				AxisControl* axis11 = axiss.at(11);
				axis10Move = thread(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis10), ref(distance[10]));
				axis11Move = thread(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis11), ref(distance[11]));
				moveAxis10 = true;
				moveAxis11 = true;
			}
			if (id == 5 && !moveAxis10) {
				AxisControl* axis10 = axiss.at(10);
				axis10Move = thread(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis10), ref(distance[10]));
			}
			if (id == 6 && !moveAxis11) {
				AxisControl* axis11 = axiss.at(11);
				axis11Move = thread(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis11), ref(distance[11]));
			}
		}
		if (axis10Move.joinable()) {
			axis10Move.join();
		}
		if (axis11Move.joinable()) {
			axis11Move.join();
		}
		//报警线程启动
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
		alarm.detach();
		hasRunOnce = true;
		return;
	}
	//进行回零运动
	if (!(systemControl.errorCode == 2002)) {
		
		AxisControl* axis3 = axiss.at(3);
		AxisControl* axis4 = axiss.at(4);
		AxisControl* axis8 = axiss.at(8);
		AxisControl* axis9 = axiss.at(9);
		AxisControl* axis10 = axiss.at(10);
		AxisControl* axis11 = axiss.at(11);
		AxisControl* axis0 = axiss.at(0);
		AxisControl* axis1 = axiss.at(1);
		AxisControl* axis2 = axiss.at(2);
		AxisControl* axis5 = axiss.at(5);
		AxisControl* axis6 = axiss.at(6);
		AxisControl* axis7 = axiss.at(7);

		thread axis3Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis3), ref(distance[3]));
		thread axis4Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis4), ref(distance[4]));
		thread axis8Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis8), ref(distance[8]));
		thread axis10Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis10), ref(distance[10]));
		thread axis11Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis11), ref(distance[11]));

		axis3Move.join();
		axis4Move.join();
		axis8Move.join();
		axis10Move.join();
		axis11Move.join();

		Sleep(1000);
		thread axis9Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis9), ref(distance[9]));
		axis9Move.join();


		thread axis0Move(&AxisControl::homeFirstAxis, this, ref(handle), ref(ioControl), ref(*axis0));
		thread axis1Move(&AxisControl::homeFirstAxis, this, ref(handle), ref(ioControl), ref(*axis1));
		thread axis2Move(&AxisControl::homeFirstAxis, this, ref(handle), ref(ioControl), ref(*axis2));
		thread axis5Move(&AxisControl::homeFirstAxis, this, ref(handle), ref(ioControl), ref(*axis5));
		thread axis6Move(&AxisControl::homeFirstAxis, this, ref(handle), ref(ioControl), ref(*axis6));
		thread axis7Move(&AxisControl::homeFirstAxis, this, ref(handle), ref(ioControl), ref(*axis7));

		axis0Move.join();
		axis1Move.join();
		axis2Move.join();
		axis5Move.join();
		axis6Move.join();
		axis7Move.join();

		thread axis00Move(&AxisControl::homeSecondAxis, this, ref(handle), ref(ioControl), ref(*axis0), ref(distance[0]));
		thread axis01Move(&AxisControl::homeSecondAxis, this, ref(handle), ref(ioControl), ref(*axis1), ref(distance[1]));
		thread axis02Move(&AxisControl::homeSecondAxis, this, ref(handle), ref(ioControl), ref(*axis2), ref(distance[2]));
		thread axis05Move(&AxisControl::homeSecondAxis, this, ref(handle), ref(ioControl), ref(*axis5), ref(distance[5]));
		thread axis06Move(&AxisControl::homeSecondAxis, this, ref(handle), ref(ioControl), ref(*axis6), ref(distance[6]));
		thread axis07Move(&AxisControl::homeSecondAxis, this, ref(handle), ref(ioControl), ref(*axis7), ref(distance[7]));

		axis00Move.join();
		axis01Move.join();
		axis02Move.join();
		axis05Move.join();
		axis06Move.join();
		axis07Move.join();
		return;
	}
	//启动报警线程
	systemControl.alarming = true;
	thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
	alarm.detach();
}

void AxisControl::homeFirstAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl) {


	//设置对应的回零运动参数
	axisControl.setFloatAxisParams(handle, "startMoveSpeed1", axisControl.startMoveSpeed[1]);
	axisControl.setFloatAxisParams(handle, "speed1", axisControl.speed[1]);
	axisControl.setFloatAxisParams(handle, "accel1", axisControl.accel[1]);
	axisControl.setFloatAxisParams(handle, "decel1", axisControl.decel[1]);

	//设置单轴使能为1
	axisControl.setIntAxisParams(handle, "axisEnable", 1);


	//开始回零运动,回零模式设置为4
	ZAux_Direct_Single_Datum(handle, axisControl.axisId, 4);
	//检测回零运动状态是否完成
	uint32 homestatus;

	while (1) {
		

			//设置回零指示灯来回闪烁
			ioControl.setOutput(handle, 226, 1, "DOA");
			Sleep(500);
			ioControl.setOutput(handle, 226, 0, "DOA");
			Sleep(500);

			ZAux_Direct_GetHomeStatus(handle, axisControl.axisId, &homestatus);//获取回零运动完成状态
			if (homestatus == 1) {
				break;
			}	
		
	}
	//设置点位运动的参数
	axisControl.setFloatAxisParams(handle, "startMoveSpeed0", axisControl.startMoveSpeed[0]);
	axisControl.setFloatAxisParams(handle, "speed0", axisControl.speed[0]);
	axisControl.setFloatAxisParams(handle, "accel0", axisControl.accel[0]);
	axisControl.setFloatAxisParams(handle, "decel0", axisControl.decel[0]);



}

void AxisControl::homeSecondAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl, float distance) {
	
	ZAux_Direct_Single_MoveAbs(handle, axisControl.axisId, distance);
	//获取轴的运动完成状态
	while (1) {
		//设置回零指示灯来回闪烁
		ioControl.setOutput(handle, 226, 1, "DOA");
		Sleep(500);
		ioControl.setOutput(handle, 226, 0, "DOA");
		Sleep(500);
		getAxisParams(handle, "idle");
		if (idle < 0) {
			//关闭指示灯
			ioControl.setOutput(handle, 226, 0, "DOA");
			break;
		}
	}
	

}

void  AxisControl::homeTypeAxis(ZMC_HANDLE&handle, IOControl& ioControl, vector<AxisControl*>& axiss) {
	float distance[12] = { 5,5,250,250,250,524.477,524.475,472.336,10,0,5,5 };
	if (axisId == 0 || axisId == 1) {
		//相关关联轴回零,轴10和轴11回零
		AxisControl* axis10 = axiss.at(10);
		AxisControl* axis11 = axiss.at(11);
		thread axis10Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis10), ref(distance[10]));
		thread axis11Move(&AxisControl::homeFirstAndSecondAxis, this, ref(handle), ref(ioControl), ref(*axis11), ref(distance[11]));
		axis10Move.join();
		axis11Move.join();
	}
	if (axisId == 5) {
		AxisControl* axis10 = axiss.at(10);
		axis10->homeFirstAndSecondAxis(handle, ioControl, *axis10, distance[10]);
		
	}
	if (axisId == 6) {
		AxisControl* axis11 = axiss.at(11);
		axis11->homeFirstAndSecondAxis(handle, ioControl, *axis11, distance[11]);
	}
}

void AxisControl::axisMove(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss,int type,int direction,float distanceOrSpeed) {
	//第一步检测当前系统的安全状态
	if (systemControl.errorCode == 2002) {
		//弹窗
		MessageBox(NULL, L"轴触发负向硬限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (systemControl.errorCode == 2001) {
		//弹窗
		MessageBox(NULL, L"轴触发正向硬限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (systemControl.errorCode == 1003) {
		//弹窗
		MessageBox(NULL, L"安全门打开！！！", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	//软限位触发仅需要判断自身即可，软限位触发，其他轴不影响运动
	if (axisErrorCode == 1002) {
		//弹窗
		MessageBox(NULL, L"轴触发负向软限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (axisErrorCode == 1001) {
		//弹窗
		MessageBox(NULL, L"轴触发正向向软限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	


	//第二步检查回零锁存,回零后使能发生变化为1
	if (axisEnable == 0) {
		//弹窗
		MessageBox(NULL, L"请先将轴回零！！！", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	//第三步检查碰撞条件
	//上料吸盘的前后移动，隔纸盒左右移动，下料吸盘前后移动。
	if (axisId == 5 || axisId == 7) {
		//判断上料吸盘的竖直高度
		AxisControl axis10 = *axiss.at(10);
		axis10.getAxisParams(handle, "dPos");
		if (axis10.dPos >= 17) {
			//弹出提示窗
			MessageBox(NULL, L"继续运动，可能会发生碰撞风险，请将上料吸盘移动到隔纸盒上方！！！", L"提示", MB_OK | MB_ICONINFORMATION);
			return;
		}
	}

	if (axisId == 6 || axisId == 7) {
		//判断下料吸盘的竖直高度
		AxisControl axis11 = *axiss.at(11);
		axis11.getAxisParams(handle, "dPos");
		if (axis11.dPos >= 17) {
			//弹出提示框
			MessageBox(NULL, L"继续运动，可能会发生碰撞风险，请将下料吸盘移动到隔纸盒上方！！！", L"提示", MB_OK | MB_ICONINFORMATION);
			return;
		}
	}

	//查看运动类别
	//1、为单步运动
	//2、为连续运动
	//direction为1，正向运动，direction为-1，反向运动
	if (type == 1) {
		if (direction == 1) {
			ZAux_Direct_Single_Move(handle, axisId,distanceOrSpeed);
		}
		if (direction == -1) {
			ZAux_Direct_Single_Move(handle, axisId, 0-distanceOrSpeed);
		}
	}
	//连续运动
	if (type == 2) {
		if (direction == 1) {
			//正向运动
			setFloatAxisParams(handle, "speed0", distanceOrSpeed);
			ZAux_Direct_Single_Vmove(handle, axisId, 1);
			//当运动停止后，检测其dPos位置，看是否达到限位
			//while (1)//等待轴运动完成 
			//{
			//	Sleep(100);
			//	getAxisParams(handle, "idle");
			//	if (idle < 0)break;
			//}
			////运动完成后，读取轴的状态
			//getAxisParams(handle, "axisStatus");
			//getAxisParams(handle, "fhLimitState");
			//if (axisStatus == 512) {
			//	//弹窗
			//	MessageBox(NULL, L"轴超出正向软限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
			//if (fhLimitState == 1) {
			//	//弹窗
			//	MessageBox(NULL, L"轴超出正向硬限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
			//
		}
		if (direction == -1) {
			//反向运动
			setFloatAxisParams(handle, "speed0", distanceOrSpeed);
			ZAux_Direct_Single_Vmove(handle, axisId, -1);
			//当运动停止后，检测其dPos位置，看是否达到限位
			//while (1)//等待轴0运动完成 
			//{
			//	Sleep(100);
			//	getAxisParams(handle, "idle");
			//	if (idle < 0)break;
			//}
			////运动完成后，读取dPos位置
			//getAxisParams(handle, "axisStatus");
			//getAxisParams(handle, "rhLimitState");
			//if (axisStatus == 1024) {
			//	//弹窗
			//	MessageBox(NULL, L"轴超出负向软限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
			//if (rhLimitState == 1) {
			//	//弹窗
			//	MessageBox(NULL, L"轴超出负向硬限位！！！", L"提示", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
		}
	}


	
	


	
}