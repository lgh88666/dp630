

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
		//�ٶȳ����趨������ٶȣ�ֱ�ӷ���0
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
		//�ٶȲ��ܳ�������ٶȣ�creepΪ�����˶������ٶ�
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
	//������ɺ�ÿ������Ҫ�˶�����ʼλ�õľ�������
	float distance[12] = { 5,5,250,250,250,524.477,524.475,472.336,10,0,5,5 };
	//��ȡ��ȫ�߳������쳣��errorCode
	systemControl.generateAxisErrorCode(handle, axiss);
	int ioErrorcode = systemControl.generateIOErrorCode(handle,ioControl);
	//�����ȫ�Ŵ򿪣�������������
	if (ioErrorcode == 1003) {
		//����
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
		alarm.detach();
		//����
		return;

	}
	//�������������ָ���Ӳ��λ��������ֹ��Ļ������
	for (AxisControl* axisOne : axiss) {
		if (axisOne->axisErrorCode == 2002) {
			//����
			systemControl.alarming = true;
			thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl),ref(axiss));
			alarm.detach();
			//����
			return;
		}
	}
	
	//��������Ӳ��λ��������㣬���������Ҫ���ν��С�
	if (axisErrorCode == 2001 ) {
			if (!hasRunOnce) {
				homeTypeAxis(handle, ioControl, axiss);
				/*if (axisId == 0 || axisId == 1) {
					//��ع��������,��10����11����
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
				//�������꣬��Ҫ���������ж��λ���
				//���������߳�
				systemControl.alarming = true;
				thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
				alarm.detach();
				hasRunOnce = true;
				return;
			}
			else {
				homeTypeAxis(handle, ioControl, axiss);
				/*if (axisId == 0 || axisId == 1) {
					//��ع��������,��10����11����
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
				//���������
				AxisControl* axisOne = axiss.at(axisId);
				homeFirstAndSecondAxis(handle, ioControl, *axisOne, distance[axisId]);
				axisErrorCode = 0000;
				hasRunOnce = false;
			}

	}
	//������ĸ���Ӳ��λ���������û��Ч����ȫ�����û��Ч������������
	// ��������Ļ��㲻Ӱ��
	/*if (axisErrorCode == 2002) {
		//���������߳�
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl));
		alarm.detach();
		//����û��Ч����ֱ�ӷ���
		return;	
	}*/
	cout << "axisId" << axisId << endl;
	//�ᴥ����������λ��������㡣
	// ��Ҫ���λ����������һ�λ��㣬������������˶����������Ľ�������
	// �ڶ��ε�������˶�������㶯����ʼ��
	if (axisErrorCode == 1001 || axisErrorCode == 1002) {
		//������ţ�������������λ������Ҫһ�λ���
		if (axisId == 9) {
			AxisControl axisOne = *axiss.at(axisId);
			//ִ����9�Ļ������
			homeFirstAndSecondAxis(handle, ioControl, axisOne, distance[axisId]);
			//������ɺ�ֱ���˳�
			return;
		}
		//���������ᣬ������������λ����Ҫ���λ������
		//hasRunOnce��¼��Ļ����Ƿ��ǵ�һ�Σ�Ĭ��Ϊfalse
		if (!hasRunOnce) {
			homeTypeAxis(handle, ioControl, axiss);
			//������0����1��������̨����������˶�һ��
			/*if (axisId == 0 || axisId == 1) {
				//��ع��������,��10����11����
				AxisControl axis10 = axis.at(10);
				AxisControl axis11 = axis.at(11);
				thread axis10Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
				thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
				axis10Move.join();
				axis11Move.join();	
			}
			//������5����������ǰ���ƶ�����������˶�һ��
			if (axisId == 5 ) {
				//�����10��ʼ����
				AxisControl axis10 = axis.at(10);
				thread axisMove(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis10), ref(distance[10]));
				axisMove.join();
			}
			//������6����������ǰ���ƶ�����������˶�һ��
			if (axisId == 6) {
				//�����11��ʼ����
				AxisControl axis11 = axis.at(11);
				thread axis11Move(&AxisControl::homeNormalAxis, this, ref(handle), ref(ioControl), ref(axis11), ref(distance[11]));
				axis11Move.join();
			}*/
			//�����᲻�漰����ᣬ�����䱨������
			//�������꣬��Ҫ���������ж��λ���
			//���������߳�
			systemControl.alarming = true;
			thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
			hasRunOnce = true;
			return;
		}
		//�ѽ���һ�λ������
		else {
			homeTypeAxis(handle, ioControl, axiss);
			//ִ����0����1�ĵڶ��λ���
			/*if (axisId == 0 || axisId == 1) {
				//��ع��������,��10����11����
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
			//���������
			AxisControl axisOne = *axiss.at(axisId);
			homeFirstAndSecondAxis(handle, ioControl, axisOne,distance[axisId]);
		}	
	}
	//���û����������λ������������
	else {
		homeTypeAxis(handle, ioControl, axiss);
		/*if (axisId == 0 || axisId == 1) {
			//��ع��������,��10����11����
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
		//����0����1�⣬��������㲻ǣ����������˶�
		AxisControl* axisOne = axiss.at(axisId);
		homeFirstAndSecondAxis(handle, ioControl, *axisOne, distance[axisId]);
		hasRunOnce = false;
	
	}
	

}	
	
void AxisControl::homeFirstAndSecondAxis(ZMC_HANDLE & handle, IOControl & ioControl,AxisControl& axisControl,float distance)
{
	//���ö�Ӧ�Ļ����˶�����
	axisControl.setFloatAxisParams(handle, "startMoveSpeed1", axisControl.startMoveSpeed[1]);
	axisControl.setFloatAxisParams(handle, "speed1", axisControl.speed[1]);
	axisControl.setFloatAxisParams(handle, "accel1", axisControl.accel[1]);
	axisControl.setFloatAxisParams(handle, "decel1", axisControl.decel[1]);

	//���õ���ʹ��Ϊ1
	axisControl.setIntAxisParams(handle, "axisEnable", 1);

	//��ʼ�����˶�,����ģʽ����Ϊ4
	ZAux_Direct_Single_Datum(handle, axisControl.axisId, 4);

	//�������˶�״̬�Ƿ����
	uint32 homestatus = 0;
	while (1) {
		if (homestatus == 0) {
			//���û���ָʾ��������˸
			ioControl.setOutput(handle, 226, 1, "DOA");
			Sleep(500);
			ioControl.setOutput(handle, 226, 0, "DOA");
			Sleep(500);
			ZAux_Direct_GetHomeStatus(handle, axisControl.axisId, &homestatus);//��ȡ�����˶����״̬
			if (homestatus == 1) {
				//����״̬Ϊ1����ʼִ����һ��(�˶�������λ��)
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
		//���û���ָʾ��������˸
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
	//���������ǰ��Ҫ������״̬
	//������ָ���Ӳ��λ��������ֹ�������
	//�����������Ӳ��λ����������ִ���������
	//����λ������Ӱ����Щ����
	float distance[12] = { 5,5,250,250,250,524.477,524.475,472.336,10,0,5,5 };
	int ioErrorcode = systemControl.generateIOErrorCode(handle, ioControl);
	//�����ȫ�Ŵ򿪣�������������
	if (ioErrorcode == 1001) {
		//����
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
		alarm.detach();
		//����
		return;
	}
	//������ᴥ����Ӳ��λ��Ҳ��ֹ�����˶�
	for (AxisControl* axisOne : axiss) {
		if (axisOne->axisErrorCode == 2002) {
			//����
			systemControl.alarming = true;
			thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
			alarm.detach();
			//����
			return;
		}
	}
	systemControl.generateAxisErrorCode(handle, axiss);
	vector<int> errorAxisId;
	for (AxisControl* axisOne : axiss) {
		if (axisOne->axisErrorCode == 1001 || axisOne->axisErrorCode == 1002 || axisOne->axisErrorCode == 2001) {
			//�õ���������
			errorAxisId.push_back(axisOne->axisId);
		}
	}
	//�Դ���������ͳһһ�λ���
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
		//�����߳�����
		systemControl.alarming = true;
		thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
		alarm.detach();
		hasRunOnce = true;
		return;
	}
	//���л����˶�
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
	//���������߳�
	systemControl.alarming = true;
	thread alarm(&SystemControl::triggerAlarm, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));
	alarm.detach();
}

void AxisControl::homeFirstAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl) {


	//���ö�Ӧ�Ļ����˶�����
	axisControl.setFloatAxisParams(handle, "startMoveSpeed1", axisControl.startMoveSpeed[1]);
	axisControl.setFloatAxisParams(handle, "speed1", axisControl.speed[1]);
	axisControl.setFloatAxisParams(handle, "accel1", axisControl.accel[1]);
	axisControl.setFloatAxisParams(handle, "decel1", axisControl.decel[1]);

	//���õ���ʹ��Ϊ1
	axisControl.setIntAxisParams(handle, "axisEnable", 1);


	//��ʼ�����˶�,����ģʽ����Ϊ4
	ZAux_Direct_Single_Datum(handle, axisControl.axisId, 4);
	//�������˶�״̬�Ƿ����
	uint32 homestatus;

	while (1) {
		

			//���û���ָʾ��������˸
			ioControl.setOutput(handle, 226, 1, "DOA");
			Sleep(500);
			ioControl.setOutput(handle, 226, 0, "DOA");
			Sleep(500);

			ZAux_Direct_GetHomeStatus(handle, axisControl.axisId, &homestatus);//��ȡ�����˶����״̬
			if (homestatus == 1) {
				break;
			}	
		
	}
	//���õ�λ�˶��Ĳ���
	axisControl.setFloatAxisParams(handle, "startMoveSpeed0", axisControl.startMoveSpeed[0]);
	axisControl.setFloatAxisParams(handle, "speed0", axisControl.speed[0]);
	axisControl.setFloatAxisParams(handle, "accel0", axisControl.accel[0]);
	axisControl.setFloatAxisParams(handle, "decel0", axisControl.decel[0]);



}

void AxisControl::homeSecondAxis(ZMC_HANDLE& handle, IOControl& ioControl, AxisControl& axisControl, float distance) {
	
	ZAux_Direct_Single_MoveAbs(handle, axisControl.axisId, distance);
	//��ȡ����˶����״̬
	while (1) {
		//���û���ָʾ��������˸
		ioControl.setOutput(handle, 226, 1, "DOA");
		Sleep(500);
		ioControl.setOutput(handle, 226, 0, "DOA");
		Sleep(500);
		getAxisParams(handle, "idle");
		if (idle < 0) {
			//�ر�ָʾ��
			ioControl.setOutput(handle, 226, 0, "DOA");
			break;
		}
	}
	

}

void  AxisControl::homeTypeAxis(ZMC_HANDLE&handle, IOControl& ioControl, vector<AxisControl*>& axiss) {
	float distance[12] = { 5,5,250,250,250,524.477,524.475,472.336,10,0,5,5 };
	if (axisId == 0 || axisId == 1) {
		//��ع��������,��10����11����
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
	//��һ����⵱ǰϵͳ�İ�ȫ״̬
	if (systemControl.errorCode == 2002) {
		//����
		MessageBox(NULL, L"�ᴥ������Ӳ��λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (systemControl.errorCode == 2001) {
		//����
		MessageBox(NULL, L"�ᴥ������Ӳ��λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (systemControl.errorCode == 1003) {
		//����
		MessageBox(NULL, L"��ȫ�Ŵ򿪣�����", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	//����λ��������Ҫ�ж������ɣ�����λ�����������᲻Ӱ���˶�
	if (axisErrorCode == 1002) {
		//����
		MessageBox(NULL, L"�ᴥ����������λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (axisErrorCode == 1001) {
		//����
		MessageBox(NULL, L"�ᴥ������������λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}

	


	//�ڶ�������������,�����ʹ�ܷ����仯Ϊ1
	if (axisEnable == 0) {
		//����
		MessageBox(NULL, L"���Ƚ�����㣡����", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	//�����������ײ����
	//�������̵�ǰ���ƶ�����ֽ�������ƶ�����������ǰ���ƶ���
	if (axisId == 5 || axisId == 7) {
		//�ж��������̵���ֱ�߶�
		AxisControl axis10 = *axiss.at(10);
		axis10.getAxisParams(handle, "dPos");
		if (axis10.dPos >= 17) {
			//������ʾ��
			MessageBox(NULL, L"�����˶������ܻᷢ����ײ���գ��뽫���������ƶ�����ֽ���Ϸ�������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			return;
		}
	}

	if (axisId == 6 || axisId == 7) {
		//�ж��������̵���ֱ�߶�
		AxisControl axis11 = *axiss.at(11);
		axis11.getAxisParams(handle, "dPos");
		if (axis11.dPos >= 17) {
			//������ʾ��
			MessageBox(NULL, L"�����˶������ܻᷢ����ײ���գ��뽫���������ƶ�����ֽ���Ϸ�������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			return;
		}
	}

	//�鿴�˶����
	//1��Ϊ�����˶�
	//2��Ϊ�����˶�
	//directionΪ1�������˶���directionΪ-1�������˶�
	if (type == 1) {
		if (direction == 1) {
			ZAux_Direct_Single_Move(handle, axisId,distanceOrSpeed);
		}
		if (direction == -1) {
			ZAux_Direct_Single_Move(handle, axisId, 0-distanceOrSpeed);
		}
	}
	//�����˶�
	if (type == 2) {
		if (direction == 1) {
			//�����˶�
			setFloatAxisParams(handle, "speed0", distanceOrSpeed);
			ZAux_Direct_Single_Vmove(handle, axisId, 1);
			//���˶�ֹͣ�󣬼����dPosλ�ã����Ƿ�ﵽ��λ
			//while (1)//�ȴ����˶���� 
			//{
			//	Sleep(100);
			//	getAxisParams(handle, "idle");
			//	if (idle < 0)break;
			//}
			////�˶���ɺ󣬶�ȡ���״̬
			//getAxisParams(handle, "axisStatus");
			//getAxisParams(handle, "fhLimitState");
			//if (axisStatus == 512) {
			//	//����
			//	MessageBox(NULL, L"�ᳬ����������λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
			//if (fhLimitState == 1) {
			//	//����
			//	MessageBox(NULL, L"�ᳬ������Ӳ��λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
			//
		}
		if (direction == -1) {
			//�����˶�
			setFloatAxisParams(handle, "speed0", distanceOrSpeed);
			ZAux_Direct_Single_Vmove(handle, axisId, -1);
			//���˶�ֹͣ�󣬼����dPosλ�ã����Ƿ�ﵽ��λ
			//while (1)//�ȴ���0�˶���� 
			//{
			//	Sleep(100);
			//	getAxisParams(handle, "idle");
			//	if (idle < 0)break;
			//}
			////�˶���ɺ󣬶�ȡdPosλ��
			//getAxisParams(handle, "axisStatus");
			//getAxisParams(handle, "rhLimitState");
			//if (axisStatus == 1024) {
			//	//����
			//	MessageBox(NULL, L"�ᳬ����������λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
			//if (rhLimitState == 1) {
			//	//����
			//	MessageBox(NULL, L"�ᳬ������Ӳ��λ������", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			//	return;
			//}
		}
	}


	
	


	
}