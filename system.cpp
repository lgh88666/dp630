

#include"system.h"

void SystemControl::safeGuard(ZMC_HANDLE& handle, IOControl& ioControl, vector<AxisControl*>& axiss) {

	while (1) {

		
		//����ֵ
		needAlarm = false;
		buzzerState = true;
		
		//errorCode = generateErrorCode(handle, ioControl, axis);
		 
		//����ͨ��io���ɵĴ�����
		int ioErrorcode = generateIOErrorCode(handle, ioControl);
		//���������״̬���ɵĴ�����
		int axisErrorcode = generateAxisErrorCode(handle, axiss);

		//����ͨ��io��Ϣ�Լ������״̬���ɴ�����
		errorCode = ioErrorcode >= axisErrorcode ? ioErrorcode : axisErrorcode;

        //���û��ϵͳ���󣬽��Զ���������־Ϊ����Ϊtrue,�����Զ�������
        if (errorCode == 0000) {
            stopRunAutoRroductionFlag.store(true);
        }

		//�жϰ�ȫ��
		if (errorCode != 0000) {
			//�ж��Ƿ�����
			if (errorCodeLatch == 0000 || (errorCodeLatch != 0000 && errorCodeLatch != errorCode)) {
				errorCodeLatch = errorCode;
				needAlarm = true;
			}
		}
		else {
			errorCodeLatch = 0;
		}

		if (needAlarm) {
			alarming = true;
			//�����߳�
			thread alarm(&SystemControl::triggerAlarm, this, ref(handle), ref(ioControl),ref(axiss));
			alarm.detach();
		}
		

		if (alarming) {
			if (ioControl.DIA[4] == 1) {
				//�ж��Ƿ�Ϊ��߼���ı���,��߼���ı������ȡ����ť����������ʧ����Ҫ�ȵ��������Ż���ʧ
				if (errorCode < 3000) {
					//�����̹߳ر�
					alarming = false;
				}
				
			}
			//������Ʒ�����
			if (ioControl.DIA[5] == 1) {
				//�رշ�����
				buzzerState = false;
			}	
		}
		Sleep(500);
	}
}

void SystemControl::triggerAlarm(ZMC_HANDLE & handle, IOControl & ioControl, vector<AxisControl*>& axiss) {
	//�������������������ʹ������Ϊ0
	// �������ѹ���ͣ������ᴥ��Ӳ��λ������Ҫ���������ʹ��ȫ����0��
	// �����������λ����������Ҫ���������ʹ����0��
	//����ǰ�ȫ�ű���������Ҫ��ʹ����0
	//if (errorCode == 3000 || errorCode == 2001 || errorCode == 2002) {
 //       for (AxisControl* axis : axiss) {
	//		axis->setIntAxisParams(handle, "axisEnable", 0);
	//	}
	//}
	//if (errorCode == 1001 || errorCode == 1002) {
	//	for (AxisControl* axis : axiss) {
	//		if (axis->axisErrorCode == 1001 || axis->axisErrorCode == 1002) {
	//			axis->setIntAxisParams(handle, "axisEnable", 0);
	//		}	
	//	}
	//}


    //ֱ���ж��Զ����߳�
    stopRunAutoRroductionFlag.store(false);



    //�޸�Ϊ��ֻҪ���ֱ������Ͷ���ʹ�ܽ�������Ϊ0
    for (AxisControl* axis : axiss) {
        axis->setIntAxisParams(handle, "axisEnable", 0);
    }
   

   
	/*while (alarming) {
		//ָʾ�ƣ���ƣ�������
        ioControl.setOutput(handle,225, 0, "DOA");
		ioControl.setOutput(handle, 227, 1, "DOA");
		ioControl.setOutput(handle, 230, buzzerState ? 1: 0, "DOA");
		Sleep(500);
		ioControl.setOutput(handle, 225, 0, "DOA");
		ioControl.setOutput(handle, 227, 0, "DOA");
		ioControl.setOutput(handle, 230, 0, "DOA");
		Sleep(500);
	}*/
}

int SystemControl::generateIOErrorCode(ZMC_HANDLE& handle, IOControl& ioControl) {
	//ÿ�γ�ʼ��errorCode
	errorCode = 0000;
	//����һ����߼���Ĵ�����
	int highErrorCode = 0000;
	//��ȡͨ��IO�ź�(DIA��DIBģ�飩
	if (ioControl.getInput(handle, "DIA") && ioControl.getInput(handle, "DIB")) {
		if (ioControl.DIA[0] == 1) {
			cout << "��ѹ���ʹ���" << endl;
			//����ΪĿǰ��߼���
			errorCode = 3000;
			//�жϵ�ǰ���ɵ�errorCode����
			if (errorCode > highErrorCode) {
				highErrorCode = errorCode; 
			}
		}
		if (ioControl.DIA[1] == 0) {
			cout << "��ȫ�Ŵ�! " << endl;;
			errorCode = 1003;
			if (errorCode > highErrorCode) {
				highErrorCode = errorCode;
			}
		}
		//������һЩio����
		//����
	}
	return highErrorCode;
}

int SystemControl::generateAxisErrorCode(ZMC_HANDLE& handle,vector<AxisControl*>& axiss) {
	errorCode = 0000;
	int highErrorCode = 0000;
	//��ȡ��Ĵ������,��ȡ��һ������ֱ������ѭ��
	for (AxisControl*axis : axiss) {
		//��ȡ�������Ӳ��������״̬��Ϣ
		axis->getAxisParams(handle, "fhLimitState");
		axis->getAxisParams(handle, "rhLimitState");
		if (axis->axisStatus == 512) {
			cout << "��" << axis->axisId << "������������λ" << axis->axisErrorCode << endl;
			axis->axisErrorCode = 1001;
			errorCode = axis->axisErrorCode;
		}
		if (axis->axisStatus == 1024) {
			cout << "��" << axis->axisId << "������������λ" << endl;
			axis->axisErrorCode = 1002;
			errorCode = axis->axisErrorCode;
			
		}
		if (axis->fhLimitState == 1) {
			cout << "��" << axis->axisId << "��������Ӳ����λ" << endl;
			axis->axisErrorCode = 2001;
			errorCode = 2001;
		
		}
		if (axis->rhLimitState == 1) {
			cout << "��" << axis->axisId << "��������Ӳ����λ" << endl;
			axis->axisErrorCode = 2002;
			errorCode = 2002;
			
		}	
		if (errorCode > highErrorCode) {
			highErrorCode = errorCode;
		}
	}
	return highErrorCode;
}

void SystemControl::initialAxisParams(ZMC_HANDLE& handle, vector<AxisControl*>& axiss) {

    // ȡ����0
    AxisControl* axis0 = axiss.at(0);
    axis0->setIntAxisParams(handle, "type", 65);
    axis0->setIntAxisParams(handle, "units", 1000);
    axis0->setIntAxisParams(handle, "fhLimitId", 129);
    axis0->setIntAxisParams(handle, "rhLimitId", 128);
    axis0->setIntAxisParams(handle, "homeInId", 130);
    axis0->setIntAxisParams(handle, "axisEnable", 0);
    axis0->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog

    //��ֵ���ٶ�����ʱ��
    axis0->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis0->setFloatAxisParams(handle, "maxSpeed0", 1500);
    axis0->setFloatAxisParams(handle, "maxSpeed1", 200);
    axis0->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis0->setFloatAxisParams(handle, "speed1", 50);
    axis0->setFloatAxisParams(handle, "speed2", 500);
    axis0->setFloatAxisParams(handle, "speed0", 1000);

    // ��ֵ��ʼ�˶��ٶ�123
    axis0->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis0->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis0->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // ��ֵ���������ٶ�0
    axis0->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis0->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis0->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis0->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis0->setFloatAxisParams(handle, "accel1", 300);
    axis0->setFloatAxisParams(handle, "accel2", 500);
    axis0->setFloatAxisParams(handle, "accel0", 3000);

    // ��ֵ���ٶ�123
    axis0->setFloatAxisParams(handle, "decel1", 300);
    axis0->setFloatAxisParams(handle, "decel2", 500);
    axis0->setFloatAxisParams(handle, "decel0", 3000);

    // ��ֵ��������λ0
    axis0->setFloatAxisParams(handle, "fsLimit", 1087);
    axis0->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis0->setFloatAxisParams(handle, "homeOffPos", 4.5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis0->setFloatAxisParams(handle, "dPos", 0);
    axis0->setFloatAxisParams(handle, "mPos", 0);
    axis0->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis0->setFloatAxisParams(handle, "standByPos", 5);

    // ��������г�
    axis0->setFloatAxisParams(handle, "maxPos", 1097);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis0->setIntAxisParams(handle, "fsLimitState", 0);
    axis0->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����1
    AxisControl* axis1 = axiss.at(1);
    axis1->setIntAxisParams(handle, "type", 65);
    axis1->setIntAxisParams(handle, "units", 1000);
    axis1->setIntAxisParams(handle, "fhLimitId", 137);
    axis1->setIntAxisParams(handle, "rhLimitId", 136);
    axis1->setIntAxisParams(handle, "homeInId", 138);
    axis1->setIntAxisParams(handle, "axisEnable", 0);
    axis1->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog
    
    //��ֵ���ٶ�����ʱ��
    axis1->setFloatAxisParams(handle, "sramp", 100);


    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis1->setFloatAxisParams(handle, "maxSpeed0", 1500);
    axis1->setFloatAxisParams(handle, "maxSpeed1", 200);
    axis1->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis1->setFloatAxisParams(handle, "speed1", 50);
    axis1->setFloatAxisParams(handle, "speed2", 500);
    axis1->setFloatAxisParams(handle, "speed0", 1000);

    // ��ֵ��ʼ�˶��ٶ�123
    axis1->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis1->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis1->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // ��ֵ���������ٶ�0
    axis1->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis1->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis1->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis1->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis1->setFloatAxisParams(handle, "accel1", 300);
    axis1->setFloatAxisParams(handle, "accel2", 500);
    axis1->setFloatAxisParams(handle, "accel0", 3000);

    // ��ֵ���ٶ�123
    axis1->setFloatAxisParams(handle, "decel1", 300);
    axis1->setFloatAxisParams(handle, "decel2", 500);
    axis1->setFloatAxisParams(handle, "decel0", 3000);

    // ��ֵ��������λ0
    axis1->setFloatAxisParams(handle, "fsLimit", 1087);
    axis1->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis1->setFloatAxisParams(handle, "homeOffPos", 5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis1->setFloatAxisParams(handle, "dPos", 0);
    axis1->setFloatAxisParams(handle, "mPos", 0);
    axis1->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis1->setFloatAxisParams(handle, "standByPos", 5);

    // ��������г�
    axis1->setFloatAxisParams(handle, "maxPos", 1097);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis1->setIntAxisParams(handle, "fsLimitState", 0);
    axis1->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����2
    AxisControl* axis2 = axiss.at(2);
    axis2->setIntAxisParams(handle, "type", 65);
    axis2->setIntAxisParams(handle, "units", 1000);
    axis2->setIntAxisParams(handle, "fhLimitId", 145);
    axis2->setIntAxisParams(handle, "rhLimitId", 144);
    axis2->setIntAxisParams(handle, "homeInId", 146);
    axis2->setIntAxisParams(handle, "axisEnable", 0);
    axis2->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog

    //��ֵ���ٶ�����ʱ��
    axis2->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis2->setFloatAxisParams(handle, "maxSpeed0", 1500);
    axis2->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis2->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis2->setFloatAxisParams(handle, "speed1", 50);
    axis2->setFloatAxisParams(handle, "speed2", 500);
    axis2->setFloatAxisParams(handle, "speed0", 1000);

    // ��ֵ��ʼ�˶��ٶ�123
    axis2->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis2->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis2->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // ��ֵ���������ٶ�0
    axis2->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis2->setFloatAxisParams(handle, "maxAccelDecel0", 3000);
    axis2->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis2->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis2->setFloatAxisParams(handle, "accel1", 300);
    axis2->setFloatAxisParams(handle, "accel2", 500);
    axis2->setFloatAxisParams(handle, "accel0", 3000);

    // ��ֵ���ٶ�123
    axis2->setFloatAxisParams(handle, "decel1", 300);
    axis2->setFloatAxisParams(handle, "decel2", 500);
    axis2->setFloatAxisParams(handle, "decel0", 3000);

    // ��ֵ��������λ0
    axis2->setFloatAxisParams(handle, "fsLimit", 742);
    axis2->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis2->setFloatAxisParams(handle, "homeOffPos", 5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis2->setFloatAxisParams(handle, "dPos", 0);
    axis2->setFloatAxisParams(handle, "mPos", 0);
    axis2->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis2->setFloatAxisParams(handle, "standByPos", 250);

    // ��������г�
    axis2->setFloatAxisParams(handle, "maxPos", 752);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis2->setIntAxisParams(handle, "fsLimitState", 0);
    axis2->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����3
    AxisControl* axis3 = axiss.at(3);
    axis3->setIntAxisParams(handle, "type", 65);
    axis3->setIntAxisParams(handle, "units", 1000);
    axis3->setIntAxisParams(handle, "fhLimitId", 153);
    axis3->setIntAxisParams(handle, "rhLimitId", 152);
    axis3->setIntAxisParams(handle, "homeInId", 154);
    axis3->setIntAxisParams(handle, "axisEnable", 0);
    axis3->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog

    //��ֵ���ٶ�����ʱ��
    axis3->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis3->setFloatAxisParams(handle, "maxSpeed0", 3000);
    axis3->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis3->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis3->setFloatAxisParams(handle, "speed1", 50);
    axis3->setFloatAxisParams(handle, "speed2", 500);
    axis3->setFloatAxisParams(handle, "speed0", 1200);

    // ��ֵ��ʼ�˶��ٶ�123
    axis3->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis3->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis3->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // ��ֵ���������ٶ�0
    axis3->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis3->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis3->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis3->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis3->setFloatAxisParams(handle, "accel1", 300);
    axis3->setFloatAxisParams(handle, "accel2", 500);
    axis3->setFloatAxisParams(handle, "accel0", 4000);

    // ��ֵ���ٶ�123
    axis3->setFloatAxisParams(handle, "decel1", 300);
    axis3->setFloatAxisParams(handle, "decel2", 500);
    axis3->setFloatAxisParams(handle, "decel0", 4000);

    // ��ֵ��������λ0
    axis3->setFloatAxisParams(handle, "fsLimit", 748);
    axis3->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis3->setFloatAxisParams(handle, "homeOffPos", 5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis3->setFloatAxisParams(handle, "dPos", 0);
    axis3->setFloatAxisParams(handle, "mPos", 0);
    axis3->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis3->setFloatAxisParams(handle, "standByPos", 250);

    // ��������г�
    axis3->setFloatAxisParams(handle, "maxPos", 758);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis3->setIntAxisParams(handle, "fsLimitState", 0);
    axis3->setIntAxisParams(handle, "rsLimitState", 0);


    // ȡ����4
    AxisControl* axis4 = axiss.at(4);
    axis4->setIntAxisParams(handle, "type", 65);
    axis4->setIntAxisParams(handle, "units", 1000);
    axis4->setIntAxisParams(handle, "fhLimitId", 161);
    axis4->setIntAxisParams(handle, "rhLimitId", 160);
    axis4->setIntAxisParams(handle, "homeInId", 162);
    axis4->setIntAxisParams(handle, "axisEnable", 0);
    axis4->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1joc 2ԭ��

    //��ֵ���ٶ�����ʱ��
    axis4->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis4->setFloatAxisParams(handle, "maxSpeed0", 3000);
    axis4->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis4->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis4->setFloatAxisParams(handle, "speed1", 50);
    axis4->setFloatAxisParams(handle, "speed2", 500);
    axis4->setFloatAxisParams(handle, "speed0", 1200);

    // ��ֵ��ʼ�˶��ٶ�123
    axis4->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis4->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis4->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // ��ֵ���������ٶ�0
    axis4->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis4->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis4->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis4->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis4->setFloatAxisParams(handle, "accel1", 300);
    axis4->setFloatAxisParams(handle, "accel2", 500);
    axis4->setFloatAxisParams(handle, "accel0", 4000);

    // ��ֵ���ٶ�123
    axis4->setFloatAxisParams(handle, "decel1", 300);
    axis4->setFloatAxisParams(handle, "decel2", 500);
    axis4->setFloatAxisParams(handle, "decel0", 4000);

    // ��ֵ��������λ0
    axis4->setFloatAxisParams(handle, "fsLimit", 748);
    axis4->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis4->setFloatAxisParams(handle, "homeOffPos", 5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis4->setFloatAxisParams(handle, "dPos", 0);
    axis4->setFloatAxisParams(handle, "mPos", 0);
    axis4->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis4->setFloatAxisParams(handle, "standByPos", 250);

    // ��������г�
    axis4->setFloatAxisParams(handle, "maxPos", 758);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis4->setIntAxisParams(handle, "fsLimitState", 0);
    axis4->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����5
    AxisControl* axis5 = axiss.at(5);
    axis5->setIntAxisParams(handle, "type", 65);
    axis5->setIntAxisParams(handle, "units", 1000);
    axis5->setIntAxisParams(handle, "fhLimitId", 169);
    axis5->setIntAxisParams(handle, "rhLimitId", 168);
    axis5->setIntAxisParams(handle, "homeInId", 170);
    axis5->setIntAxisParams(handle, "axisEnable", 0);
    axis5->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog

    //��ֵ���ٶ�����ʱ��
    axis5->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis5->setFloatAxisParams(handle, "maxSpeed0", 1800);
    axis5->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis5->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis5->setFloatAxisParams(handle, "speed1", 50);
    axis5->setFloatAxisParams(handle, "speed2", 500);
    axis5->setFloatAxisParams(handle, "speed0", 800);

    // ��ֵ��ʼ�˶��ٶ�123
    axis5->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis5->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis5->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // ��ֵ���������ٶ�0
    axis5->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis5->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis5->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis5->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis5->setFloatAxisParams(handle, "accel1", 300);
    axis5->setFloatAxisParams(handle, "accel2", 500);
    axis5->setFloatAxisParams(handle, "accel0", 2000);

    // ��ֵ���ٶ�123
    axis5->setFloatAxisParams(handle, "decel1", 300);
    axis5->setFloatAxisParams(handle, "decel2", 500);
    axis5->setFloatAxisParams(handle, "decel0", 2000);

    // ��ֵ��������λ0
    axis5->setFloatAxisParams(handle, "fsLimit", 950);
    axis5->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis5->setFloatAxisParams(handle, "homeOffPos", 5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis5->setFloatAxisParams(handle, "dPos", 0);
    axis5->setFloatAxisParams(handle, "mPos", 0);
    axis5->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis5->setFloatAxisParams(handle, "standByPos", 524.477);

    // ��������г�
    axis5->setFloatAxisParams(handle, "maxPos", 960);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis5->setIntAxisParams(handle, "fsLimitState", 0);
    axis5->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����6
    AxisControl* axis6 = axiss.at(6);
    axis6->setIntAxisParams(handle, "type", 65);
    axis6->setIntAxisParams(handle, "units", 1000);
    axis6->setIntAxisParams(handle, "fhLimitId", 177);
    axis6->setIntAxisParams(handle, "rhLimitId", 176);
    axis6->setIntAxisParams(handle, "homeInId", 178);
    axis6->setIntAxisParams(handle, "axisEnable", 0);
    axis6->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog

    //��ֵ���ٶ�����ʱ��
    axis6->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis6->setFloatAxisParams(handle, "maxSpeed0", 1800);
    axis6->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis6->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis6->setFloatAxisParams(handle, "speed1", 50);
    axis6->setFloatAxisParams(handle, "speed2", 500);
    axis6->setFloatAxisParams(handle, "speed0", 800);

    // ��ֵ��ʼ�˶��ٶ�123
    axis6->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis6->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis6->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // ��ֵ���������ٶ�0
    axis6->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis6->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis6->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis6->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis6->setFloatAxisParams(handle, "accel1", 300);
    axis6->setFloatAxisParams(handle, "accel2", 500);
    axis6->setFloatAxisParams(handle, "accel0", 2000);

    // ��ֵ���ٶ�123
    axis6->setFloatAxisParams(handle, "decel1", 300);
    axis6->setFloatAxisParams(handle, "decel2", 500);
    axis6->setFloatAxisParams(handle, "decel0", 2000);

    // ��ֵ��������λ0
    axis6->setFloatAxisParams(handle, "fsLimit", 950);
    axis6->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis6->setFloatAxisParams(handle, "homeOffPos", 5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis6->setFloatAxisParams(handle, "dPos", 0);
    axis6->setFloatAxisParams(handle, "mPos", 0);
    axis6->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis6->setFloatAxisParams(handle, "standByPos", 524.475);

    // ��������г�
    axis6->setFloatAxisParams(handle, "maxPos", 960);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis6->setIntAxisParams(handle, "fsLimitState", 0);
    axis6->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����7
    AxisControl* axis7 = axiss.at(7);
    axis7->setIntAxisParams(handle, "type", 65);
    axis7->setIntAxisParams(handle, "units", 1000);
    axis7->setIntAxisParams(handle, "fhLimitId", 185);
    axis7->setIntAxisParams(handle, "rhLimitId", 184);
    axis7->setIntAxisParams(handle, "homeInId", 186);
    axis7->setIntAxisParams(handle, "axisEnable", 0);
    axis7->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog

    //��ֵ���ٶ�����ʱ��
    axis7->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis7->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis7->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis7->setFloatAxisParams(handle, "maxSpeed2", 500);

    // ��ֵ�ٶ�123
    axis7->setFloatAxisParams(handle, "speed1", 50);
    axis7->setFloatAxisParams(handle, "speed2", 200);
    axis7->setFloatAxisParams(handle, "speed0", 800);

    // ��ֵ��ʼ�˶��ٶ�123
    axis7->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis7->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis7->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // ��ֵ���������ٶ�0
    axis7->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis7->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis7->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis7->setFloatAxisParams(handle, "maxAccelDecel2", 500);

    // ��ֵ���ٶ�123
    axis7->setFloatAxisParams(handle, "accel1", 300);
    axis7->setFloatAxisParams(handle, "accel2", 200);
    axis7->setFloatAxisParams(handle, "accel0", 2000);

    // ��ֵ���ٶ�123
    axis7->setFloatAxisParams(handle, "decel1", 300);
    axis7->setFloatAxisParams(handle, "decel2", 200);
    axis7->setFloatAxisParams(handle, "decel0", 500);

    // ��ֵ��������λ0
    axis7->setFloatAxisParams(handle, "fsLimit", 1298.5);
    axis7->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis7->setFloatAxisParams(handle, "homeOffPos", 5);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis7->setFloatAxisParams(handle, "dPos", 0);
    axis7->setFloatAxisParams(handle, "mPos", 0);
    axis7->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis7->setFloatAxisParams(handle, "standByPos", 472.336);

    // ��������г�
    axis7->setFloatAxisParams(handle, "maxPos", 1308.5);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis7->setIntAxisParams(handle, "fsLimitState", 0);
    axis7->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����8
    AxisControl* axis8 = axiss.at(8);
    axis8->setIntAxisParams(handle, "type", 65);
    axis8->setIntAxisParams(handle, "units", 1000);
    axis8->setIntAxisParams(handle, "fhLimitId", 193);
    axis8->setIntAxisParams(handle, "rhLimitId", 192);
    axis8->setIntAxisParams(handle, "homeInId", 194);
    axis8->setIntAxisParams(handle, "axisEnable", 0);
    axis8->setIntAxisParams(handle, "axisErrorCode", 0);

    //0��λ 1ԭ�� 2jog

    //��ֵ���ٶ�����ʱ��
    axis8->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�123
    axis8->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis8->setFloatAxisParams(handle, "maxSpeed1", 100);
    axis8->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // ��ֵ�ٶ�123
    axis8->setFloatAxisParams(handle, "speed1", 20);
    axis8->setFloatAxisParams(handle, "speed2", 200);
    axis8->setFloatAxisParams(handle, "speed0", 200);

    // ��ֵ��ʼ�˶��ٶ�123
    axis8->setFloatAxisParams(handle, "startMoveSpeed1", 5);
    axis8->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis8->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // ��ֵ���������ٶ�0
    axis8->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���123
    axis8->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis8->setFloatAxisParams(handle, "maxAccelDecel1", 300);
    axis8->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // ��ֵ���ٶ�123
    axis8->setFloatAxisParams(handle, "accel1", 300);
    axis8->setFloatAxisParams(handle, "accel2", 500);
    axis8->setFloatAxisParams(handle, "accel0", 500);

    // ��ֵ���ٶ�123
    axis8->setFloatAxisParams(handle, "decel1", 300);
    axis8->setFloatAxisParams(handle, "decel2", 500);
    axis8->setFloatAxisParams(handle, "decel0", 500);

    // ��ֵ��������λ0
    axis8->setFloatAxisParams(handle, "fsLimit", 72);
    axis8->setFloatAxisParams(handle, "rsLimit", -5);

    // ���û���ƫ����0
    axis8->setFloatAxisParams(handle, "homeOffPos", 44);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis8->setFloatAxisParams(handle, "dPos", 0);
    axis8->setFloatAxisParams(handle, "mPos", 0);
    axis8->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis8->setFloatAxisParams(handle, "standByPos", 10);

    // ��������г�
    axis8->setFloatAxisParams(handle, "maxPos", 82);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis8->setIntAxisParams(handle, "fsLimitState", 0);
    axis8->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����9
    AxisControl* axis9 = axiss.at(9);
    axis9->setIntAxisParams(handle, "type", 65);
    axis9->setIntAxisParams(handle, "units", 1000);
    axis9->setIntAxisParams(handle, "fhLimitId", 201);
    axis9->setIntAxisParams(handle, "rhLimitId", 200);
    axis9->setIntAxisParams(handle, "homeInId", 202);
    axis9->setIntAxisParams(handle, "axisEnable", 0);
    axis9->setIntAxisParams(handle, "axisErrorCode", 0);


    //��ֵ���ٶ�����ʱ��
    axis9->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�
    axis9->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis9->setFloatAxisParams(handle, "maxSpeed1", 200);
    axis9->setFloatAxisParams(handle, "maxSpeed2", 1200);

    // ��ֵ�ٶ�
    axis9->setFloatAxisParams(handle, "speed1", 50);
    axis9->setFloatAxisParams(handle, "speed2", 1000);
    axis9->setFloatAxisParams(handle, "speed0", 800);

    // ��ֵ��ʼ�˶��ٶ�
    axis9->setFloatAxisParams(handle, "startMoveSpeed1", 15);
    axis9->setFloatAxisParams(handle, "startMoveSpeed2", 200);
    axis9->setFloatAxisParams(handle, "startMoveSpeed0", 200);

    // ��ֵ���������ٶ�
    axis9->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���
    axis9->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis9->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis9->setFloatAxisParams(handle, "maxAccelDecel2", 2000);

    // ��ֵ���ٶ�
    axis9->setFloatAxisParams(handle, "accel1", 400);
    axis9->setFloatAxisParams(handle, "accel2", 1000);
    axis9->setFloatAxisParams(handle, "accel0", 1000);

    // ��ֵ���ٶ�
    axis9->setFloatAxisParams(handle, "decel1", 400);
    axis9->setFloatAxisParams(handle, "decel2", 1000);
    axis9->setFloatAxisParams(handle, "decel0", 1000);

    // ��ֵ��������λ
    axis9->setFloatAxisParams(handle, "fsLimit", 340);
    axis9->setFloatAxisParams(handle, "rsLimit", -60);

    // ���û���ƫ����
    axis9->setFloatAxisParams(handle, "homeOffPos", 13.78);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis9->setFloatAxisParams(handle, "dPos", 0);
    axis9->setFloatAxisParams(handle, "mPos", 0);
    axis9->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis9->setFloatAxisParams(handle, "standByPos", 0);

    // ��������г�
    axis9->setFloatAxisParams(handle, "maxPos", 360);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis9->setIntAxisParams(handle, "fsLimitState", 0);
    axis9->setIntAxisParams(handle, "rsLimitState", 0);

    // ȡ����10
    AxisControl* axis10 = axiss.at(10);
    axis10->setIntAxisParams(handle, "type", 65);
    axis10->setIntAxisParams(handle, "units", 1000);
    axis10->setIntAxisParams(handle, "fhLimitId", 209);
    axis10->setIntAxisParams(handle, "rhLimitId", 208);
    axis10->setIntAxisParams(handle, "homeInId", 210);
    axis10->setIntAxisParams(handle, "axisEnable", 0);
    axis10->setIntAxisParams(handle, "axisErrorCode", 0);

    //��ֵ���ٶ�����ʱ��
    axis10->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�
    axis10->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis10->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis10->setFloatAxisParams(handle, "maxSpeed2", 500);

    // ��ֵ�ٶ�
    axis10->setFloatAxisParams(handle, "speed1", 20);
    axis10->setFloatAxisParams(handle, "speed2", 100);
    axis10->setFloatAxisParams(handle, "speed0", 800);

    // ��ֵ��ʼ�˶��ٶ�
    axis10->setFloatAxisParams(handle, "startMoveSpeed1", 5);
    axis10->setFloatAxisParams(handle, "startMoveSpeed2", 20);
    axis10->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // ��ֵ���������ٶ�
    axis10->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���
    axis10->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis10->setFloatAxisParams(handle, "maxAccelDecel1", 200);
    axis10->setFloatAxisParams(handle, "maxAccelDecel2", 2000);

    // ��ֵ���ٶ�
    axis10->setFloatAxisParams(handle, "accel1", 200);
    axis10->setFloatAxisParams(handle, "accel2", 300);
    axis10->setFloatAxisParams(handle, "accel0", 3000);

    // ��ֵ���ٶ�
    axis10->setFloatAxisParams(handle, "decel1", 200);
    axis10->setFloatAxisParams(handle, "decel2", 1500);
    axis10->setFloatAxisParams(handle, "decel0", 3000);

    // ��ֵ��������λ
    axis10->setFloatAxisParams(handle, "fsLimit", 202);
    axis10->setFloatAxisParams(handle, "rsLimit", -3);

    // ���û���ƫ����
    axis10->setFloatAxisParams(handle, "homeOffPos", 3);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis10->setFloatAxisParams(handle, "dPos", 0);
    axis10->setFloatAxisParams(handle, "mPos", 0);
    axis10->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis10->setFloatAxisParams(handle, "standByPos", 5);

    // ��������г�
    axis10->setFloatAxisParams(handle, "maxPos", 205);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis10->setIntAxisParams(handle, "fsLimitState", 0);
    axis10->setIntAxisParams(handle, "rsLimitState", 0);


    // ȡ����11
    AxisControl* axis11 = axiss.at(11);
    axis11->setIntAxisParams(handle, "type", 65);
    axis11->setIntAxisParams(handle, "units", 1000);
    axis11->setIntAxisParams(handle, "fhLimitId", 217);
    axis11->setIntAxisParams(handle, "rhLimitId", 216);
    axis11->setIntAxisParams(handle, "homeInId", 218);
    axis11->setIntAxisParams(handle, "axisEnable", 0);
    axis11->setIntAxisParams(handle, "axisErrorCode", 0);

    //��ֵ���ٶ�����ʱ��
    axis11->setFloatAxisParams(handle, "sramp", 100);

    // ��ֵ�ٶ�֮ǰ���ȸ�ֵ����ٶ�
    axis11->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis11->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis11->setFloatAxisParams(handle, "maxSpeed2", 500);

    // ��ֵ�ٶ�
    axis11->setFloatAxisParams(handle, "speed1", 20);
    axis11->setFloatAxisParams(handle, "speed2", 100);
    axis11->setFloatAxisParams(handle, "speed0", 800);

    // ��ֵ��ʼ�˶��ٶ�
    axis11->setFloatAxisParams(handle, "startMoveSpeed1", 5);
    axis11->setFloatAxisParams(handle, "startMoveSpeed2", 20);
    axis11->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // ��ֵ���������ٶ�
    axis11->setFloatAxisParams(handle, "creep", 5);

    // ��ֵ�Ӽ��ٶ�֮ǰ���ȸ�ֵ���Ӽ���
    axis11->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis11->setFloatAxisParams(handle, "maxAccelDecel1", 200);
    axis11->setFloatAxisParams(handle, "maxAccelDecel2", 2000);

    // ��ֵ���ٶ�
    axis11->setFloatAxisParams(handle, "accel1", 200);
    axis11->setFloatAxisParams(handle, "accel2", 300);
    axis11->setFloatAxisParams(handle, "accel0", 3000);

    // ��ֵ���ٶ�
    axis11->setFloatAxisParams(handle, "decel1", 200);
    axis11->setFloatAxisParams(handle, "decel2", 1500);
    axis11->setFloatAxisParams(handle, "decel0", 3000);

    // ��ֵ��������λ
    axis11->setFloatAxisParams(handle, "fsLimit", 202);
    axis11->setFloatAxisParams(handle, "rsLimit", -3);

    // ���û���ƫ����
    axis11->setFloatAxisParams(handle, "homeOffPos", 3);

    // ����dPos��mPos��ePosĬ��ֵΪ0
    axis11->setFloatAxisParams(handle, "dPos", 0);
    axis11->setFloatAxisParams(handle, "mPos", 0);
    axis11->setFloatAxisParams(handle, "ePos", 0);

    // ������ʼλ��
    axis11->setFloatAxisParams(handle, "standByPos", 5);

    // ��������г�
    axis11->setFloatAxisParams(handle, "maxPos", 205);

    // ��������λ������״̬��Ĭ��Ϊ0
    axis11->setIntAxisParams(handle, "fsLimitState", 0);
    axis11->setIntAxisParams(handle, "rsLimitState", 0);


}
