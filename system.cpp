

#include"system.h"

void SystemControl::safeGuard(ZMC_HANDLE& handle, IOControl& ioControl, vector<AxisControl*>& axiss) {

	while (1) {

		
		//赋初值
		needAlarm = false;
		buzzerState = true;
		
		//errorCode = generateErrorCode(handle, ioControl, axis);
		 
		//根据通用io生成的错误码
		int ioErrorcode = generateIOErrorCode(handle, ioControl);
		//根据轴错误状态生成的错误码
		int axisErrorcode = generateAxisErrorCode(handle, axiss);

		//根据通用io信息以及轴错误状态生成错误码
		errorCode = ioErrorcode >= axisErrorcode ? ioErrorcode : axisErrorcode;

        //如果没有系统错误，将自动化生产标志为设置为true,允许自动化生产
        if (errorCode == 0000) {
            stopRunAutoRroductionFlag.store(true);
        }

		//判断安全性
		if (errorCode != 0000) {
			//判断是否锁存
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
			//启动线程
			thread alarm(&SystemControl::triggerAlarm, this, ref(handle), ref(ioControl),ref(axiss));
			alarm.detach();
		}
		

		if (alarming) {
			if (ioControl.DIA[4] == 1) {
				//判断是否为最高级别的报警,最高级别的报警点击取消按钮报警不会消失，需要等到错误解决才会消失
				if (errorCode < 3000) {
					//响铃线程关闭
					alarming = false;
				}
				
			}
			//负责控制蜂鸣器
			if (ioControl.DIA[5] == 1) {
				//关闭蜂鸣器
				buzzerState = false;
			}	
		}
		Sleep(500);
	}
}

void SystemControl::triggerAlarm(ZMC_HANDLE & handle, IOControl & ioControl, vector<AxisControl*>& axiss) {
	//触发报警，将所有轴的使能设置为0
	// 如果是气压过低，任意轴触发硬限位，则需要将所有轴的使能全部清0。
	// 如果是轴软限位触发，仅需要将自身轴的使能清0。
	//如果是安全门报警，则不需要将使能清0
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


    //直接中断自动化线程
    stopRunAutoRroductionFlag.store(false);



    //修改为，只要出现报警，就对轴使能进行设置为0
    for (AxisControl* axis : axiss) {
        axis->setIntAxisParams(handle, "axisEnable", 0);
    }
   

   
	/*while (alarming) {
		//指示灯，红灯，蜂鸣器
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
	//每次初始化errorCode
	errorCode = 0000;
	//定义一个最高级别的错误码
	int highErrorCode = 0000;
	//读取通用IO信号(DIA与DIB模块）
	if (ioControl.getInput(handle, "DIA") && ioControl.getInput(handle, "DIB")) {
		if (ioControl.DIA[0] == 1) {
			cout << "气压过低错误！" << endl;
			//设置为目前最高级别
			errorCode = 3000;
			//判断当前生成的errorCode级别
			if (errorCode > highErrorCode) {
				highErrorCode = errorCode; 
			}
		}
		if (ioControl.DIA[1] == 0) {
			cout << "安全门打开! " << endl;;
			errorCode = 1003;
			if (errorCode > highErrorCode) {
				highErrorCode = errorCode;
			}
		}
		//其他的一些io错误
		//待定
	}
	return highErrorCode;
}

int SystemControl::generateAxisErrorCode(ZMC_HANDLE& handle,vector<AxisControl*>& axiss) {
	errorCode = 0000;
	int highErrorCode = 0000;
	//读取轴的错误代码,读取到一个错误直接跳出循环
	for (AxisControl*axis : axiss) {
		//读取轴的正负硬件传感器状态信息
		axis->getAxisParams(handle, "fhLimitState");
		axis->getAxisParams(handle, "rhLimitState");
		if (axis->axisStatus == 512) {
			cout << "轴" << axis->axisId << "触发正向软限位" << axis->axisErrorCode << endl;
			axis->axisErrorCode = 1001;
			errorCode = axis->axisErrorCode;
		}
		if (axis->axisStatus == 1024) {
			cout << "轴" << axis->axisId << "触发负向软限位" << endl;
			axis->axisErrorCode = 1002;
			errorCode = axis->axisErrorCode;
			
		}
		if (axis->fhLimitState == 1) {
			cout << "轴" << axis->axisId << "触发正向硬件限位" << endl;
			axis->axisErrorCode = 2001;
			errorCode = 2001;
		
		}
		if (axis->rhLimitState == 1) {
			cout << "轴" << axis->axisId << "触发负向硬件限位" << endl;
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

    // 取出轴0
    AxisControl* axis0 = axiss.at(0);
    axis0->setIntAxisParams(handle, "type", 65);
    axis0->setIntAxisParams(handle, "units", 1000);
    axis0->setIntAxisParams(handle, "fhLimitId", 129);
    axis0->setIntAxisParams(handle, "rhLimitId", 128);
    axis0->setIntAxisParams(handle, "homeInId", 130);
    axis0->setIntAxisParams(handle, "axisEnable", 0);
    axis0->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog

    //赋值加速度曲线时间
    axis0->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis0->setFloatAxisParams(handle, "maxSpeed0", 1500);
    axis0->setFloatAxisParams(handle, "maxSpeed1", 200);
    axis0->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis0->setFloatAxisParams(handle, "speed1", 50);
    axis0->setFloatAxisParams(handle, "speed2", 500);
    axis0->setFloatAxisParams(handle, "speed0", 1000);

    // 赋值起始运动速度123
    axis0->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis0->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis0->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // 赋值回零爬行速度0
    axis0->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis0->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis0->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis0->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis0->setFloatAxisParams(handle, "accel1", 300);
    axis0->setFloatAxisParams(handle, "accel2", 500);
    axis0->setFloatAxisParams(handle, "accel0", 3000);

    // 赋值减速度123
    axis0->setFloatAxisParams(handle, "decel1", 300);
    axis0->setFloatAxisParams(handle, "decel2", 500);
    axis0->setFloatAxisParams(handle, "decel0", 3000);

    // 赋值正负软限位0
    axis0->setFloatAxisParams(handle, "fsLimit", 1087);
    axis0->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis0->setFloatAxisParams(handle, "homeOffPos", 4.5);

    // 设置dPos、mPos、ePos默认值为0
    axis0->setFloatAxisParams(handle, "dPos", 0);
    axis0->setFloatAxisParams(handle, "mPos", 0);
    axis0->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis0->setFloatAxisParams(handle, "standByPos", 5);

    // 设置最大行程
    axis0->setFloatAxisParams(handle, "maxPos", 1097);

    // 设置软限位传感器状态，默认为0
    axis0->setIntAxisParams(handle, "fsLimitState", 0);
    axis0->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴1
    AxisControl* axis1 = axiss.at(1);
    axis1->setIntAxisParams(handle, "type", 65);
    axis1->setIntAxisParams(handle, "units", 1000);
    axis1->setIntAxisParams(handle, "fhLimitId", 137);
    axis1->setIntAxisParams(handle, "rhLimitId", 136);
    axis1->setIntAxisParams(handle, "homeInId", 138);
    axis1->setIntAxisParams(handle, "axisEnable", 0);
    axis1->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog
    
    //赋值加速度曲线时间
    axis1->setFloatAxisParams(handle, "sramp", 100);


    // 赋值速度之前，先赋值最大速度123
    axis1->setFloatAxisParams(handle, "maxSpeed0", 1500);
    axis1->setFloatAxisParams(handle, "maxSpeed1", 200);
    axis1->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis1->setFloatAxisParams(handle, "speed1", 50);
    axis1->setFloatAxisParams(handle, "speed2", 500);
    axis1->setFloatAxisParams(handle, "speed0", 1000);

    // 赋值起始运动速度123
    axis1->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis1->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis1->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // 赋值回零爬行速度0
    axis1->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis1->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis1->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis1->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis1->setFloatAxisParams(handle, "accel1", 300);
    axis1->setFloatAxisParams(handle, "accel2", 500);
    axis1->setFloatAxisParams(handle, "accel0", 3000);

    // 赋值减速度123
    axis1->setFloatAxisParams(handle, "decel1", 300);
    axis1->setFloatAxisParams(handle, "decel2", 500);
    axis1->setFloatAxisParams(handle, "decel0", 3000);

    // 赋值正负软限位0
    axis1->setFloatAxisParams(handle, "fsLimit", 1087);
    axis1->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis1->setFloatAxisParams(handle, "homeOffPos", 5);

    // 设置dPos、mPos、ePos默认值为0
    axis1->setFloatAxisParams(handle, "dPos", 0);
    axis1->setFloatAxisParams(handle, "mPos", 0);
    axis1->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis1->setFloatAxisParams(handle, "standByPos", 5);

    // 设置最大行程
    axis1->setFloatAxisParams(handle, "maxPos", 1097);

    // 设置软限位传感器状态，默认为0
    axis1->setIntAxisParams(handle, "fsLimitState", 0);
    axis1->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴2
    AxisControl* axis2 = axiss.at(2);
    axis2->setIntAxisParams(handle, "type", 65);
    axis2->setIntAxisParams(handle, "units", 1000);
    axis2->setIntAxisParams(handle, "fhLimitId", 145);
    axis2->setIntAxisParams(handle, "rhLimitId", 144);
    axis2->setIntAxisParams(handle, "homeInId", 146);
    axis2->setIntAxisParams(handle, "axisEnable", 0);
    axis2->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog

    //赋值加速度曲线时间
    axis2->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis2->setFloatAxisParams(handle, "maxSpeed0", 1500);
    axis2->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis2->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis2->setFloatAxisParams(handle, "speed1", 50);
    axis2->setFloatAxisParams(handle, "speed2", 500);
    axis2->setFloatAxisParams(handle, "speed0", 1000);

    // 赋值起始运动速度123
    axis2->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis2->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis2->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // 赋值回零爬行速度0
    axis2->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis2->setFloatAxisParams(handle, "maxAccelDecel0", 3000);
    axis2->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis2->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis2->setFloatAxisParams(handle, "accel1", 300);
    axis2->setFloatAxisParams(handle, "accel2", 500);
    axis2->setFloatAxisParams(handle, "accel0", 3000);

    // 赋值减速度123
    axis2->setFloatAxisParams(handle, "decel1", 300);
    axis2->setFloatAxisParams(handle, "decel2", 500);
    axis2->setFloatAxisParams(handle, "decel0", 3000);

    // 赋值正负软限位0
    axis2->setFloatAxisParams(handle, "fsLimit", 742);
    axis2->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis2->setFloatAxisParams(handle, "homeOffPos", 5);

    // 设置dPos、mPos、ePos默认值为0
    axis2->setFloatAxisParams(handle, "dPos", 0);
    axis2->setFloatAxisParams(handle, "mPos", 0);
    axis2->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis2->setFloatAxisParams(handle, "standByPos", 250);

    // 设置最大行程
    axis2->setFloatAxisParams(handle, "maxPos", 752);

    // 设置软限位传感器状态，默认为0
    axis2->setIntAxisParams(handle, "fsLimitState", 0);
    axis2->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴3
    AxisControl* axis3 = axiss.at(3);
    axis3->setIntAxisParams(handle, "type", 65);
    axis3->setIntAxisParams(handle, "units", 1000);
    axis3->setIntAxisParams(handle, "fhLimitId", 153);
    axis3->setIntAxisParams(handle, "rhLimitId", 152);
    axis3->setIntAxisParams(handle, "homeInId", 154);
    axis3->setIntAxisParams(handle, "axisEnable", 0);
    axis3->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog

    //赋值加速度曲线时间
    axis3->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis3->setFloatAxisParams(handle, "maxSpeed0", 3000);
    axis3->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis3->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis3->setFloatAxisParams(handle, "speed1", 50);
    axis3->setFloatAxisParams(handle, "speed2", 500);
    axis3->setFloatAxisParams(handle, "speed0", 1200);

    // 赋值起始运动速度123
    axis3->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis3->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis3->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // 赋值回零爬行速度0
    axis3->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis3->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis3->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis3->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis3->setFloatAxisParams(handle, "accel1", 300);
    axis3->setFloatAxisParams(handle, "accel2", 500);
    axis3->setFloatAxisParams(handle, "accel0", 4000);

    // 赋值减速度123
    axis3->setFloatAxisParams(handle, "decel1", 300);
    axis3->setFloatAxisParams(handle, "decel2", 500);
    axis3->setFloatAxisParams(handle, "decel0", 4000);

    // 赋值正负软限位0
    axis3->setFloatAxisParams(handle, "fsLimit", 748);
    axis3->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis3->setFloatAxisParams(handle, "homeOffPos", 5);

    // 设置dPos、mPos、ePos默认值为0
    axis3->setFloatAxisParams(handle, "dPos", 0);
    axis3->setFloatAxisParams(handle, "mPos", 0);
    axis3->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis3->setFloatAxisParams(handle, "standByPos", 250);

    // 设置最大行程
    axis3->setFloatAxisParams(handle, "maxPos", 758);

    // 设置软限位传感器状态，默认为0
    axis3->setIntAxisParams(handle, "fsLimitState", 0);
    axis3->setIntAxisParams(handle, "rsLimitState", 0);


    // 取出轴4
    AxisControl* axis4 = axiss.at(4);
    axis4->setIntAxisParams(handle, "type", 65);
    axis4->setIntAxisParams(handle, "units", 1000);
    axis4->setIntAxisParams(handle, "fhLimitId", 161);
    axis4->setIntAxisParams(handle, "rhLimitId", 160);
    axis4->setIntAxisParams(handle, "homeInId", 162);
    axis4->setIntAxisParams(handle, "axisEnable", 0);
    axis4->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1joc 2原点

    //赋值加速度曲线时间
    axis4->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis4->setFloatAxisParams(handle, "maxSpeed0", 3000);
    axis4->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis4->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis4->setFloatAxisParams(handle, "speed1", 50);
    axis4->setFloatAxisParams(handle, "speed2", 500);
    axis4->setFloatAxisParams(handle, "speed0", 1200);

    // 赋值起始运动速度123
    axis4->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis4->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis4->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // 赋值回零爬行速度0
    axis4->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis4->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis4->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis4->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis4->setFloatAxisParams(handle, "accel1", 300);
    axis4->setFloatAxisParams(handle, "accel2", 500);
    axis4->setFloatAxisParams(handle, "accel0", 4000);

    // 赋值减速度123
    axis4->setFloatAxisParams(handle, "decel1", 300);
    axis4->setFloatAxisParams(handle, "decel2", 500);
    axis4->setFloatAxisParams(handle, "decel0", 4000);

    // 赋值正负软限位0
    axis4->setFloatAxisParams(handle, "fsLimit", 748);
    axis4->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis4->setFloatAxisParams(handle, "homeOffPos", 5);

    // 设置dPos、mPos、ePos默认值为0
    axis4->setFloatAxisParams(handle, "dPos", 0);
    axis4->setFloatAxisParams(handle, "mPos", 0);
    axis4->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis4->setFloatAxisParams(handle, "standByPos", 250);

    // 设置最大行程
    axis4->setFloatAxisParams(handle, "maxPos", 758);

    // 设置软限位传感器状态，默认为0
    axis4->setIntAxisParams(handle, "fsLimitState", 0);
    axis4->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴5
    AxisControl* axis5 = axiss.at(5);
    axis5->setIntAxisParams(handle, "type", 65);
    axis5->setIntAxisParams(handle, "units", 1000);
    axis5->setIntAxisParams(handle, "fhLimitId", 169);
    axis5->setIntAxisParams(handle, "rhLimitId", 168);
    axis5->setIntAxisParams(handle, "homeInId", 170);
    axis5->setIntAxisParams(handle, "axisEnable", 0);
    axis5->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog

    //赋值加速度曲线时间
    axis5->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis5->setFloatAxisParams(handle, "maxSpeed0", 1800);
    axis5->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis5->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis5->setFloatAxisParams(handle, "speed1", 50);
    axis5->setFloatAxisParams(handle, "speed2", 500);
    axis5->setFloatAxisParams(handle, "speed0", 800);

    // 赋值起始运动速度123
    axis5->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis5->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis5->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // 赋值回零爬行速度0
    axis5->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis5->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis5->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis5->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis5->setFloatAxisParams(handle, "accel1", 300);
    axis5->setFloatAxisParams(handle, "accel2", 500);
    axis5->setFloatAxisParams(handle, "accel0", 2000);

    // 赋值减速度123
    axis5->setFloatAxisParams(handle, "decel1", 300);
    axis5->setFloatAxisParams(handle, "decel2", 500);
    axis5->setFloatAxisParams(handle, "decel0", 2000);

    // 赋值正负软限位0
    axis5->setFloatAxisParams(handle, "fsLimit", 950);
    axis5->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis5->setFloatAxisParams(handle, "homeOffPos", 5);

    // 设置dPos、mPos、ePos默认值为0
    axis5->setFloatAxisParams(handle, "dPos", 0);
    axis5->setFloatAxisParams(handle, "mPos", 0);
    axis5->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis5->setFloatAxisParams(handle, "standByPos", 524.477);

    // 设置最大行程
    axis5->setFloatAxisParams(handle, "maxPos", 960);

    // 设置软限位传感器状态，默认为0
    axis5->setIntAxisParams(handle, "fsLimitState", 0);
    axis5->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴6
    AxisControl* axis6 = axiss.at(6);
    axis6->setIntAxisParams(handle, "type", 65);
    axis6->setIntAxisParams(handle, "units", 1000);
    axis6->setIntAxisParams(handle, "fhLimitId", 177);
    axis6->setIntAxisParams(handle, "rhLimitId", 176);
    axis6->setIntAxisParams(handle, "homeInId", 178);
    axis6->setIntAxisParams(handle, "axisEnable", 0);
    axis6->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog

    //赋值加速度曲线时间
    axis6->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis6->setFloatAxisParams(handle, "maxSpeed0", 1800);
    axis6->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis6->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis6->setFloatAxisParams(handle, "speed1", 50);
    axis6->setFloatAxisParams(handle, "speed2", 500);
    axis6->setFloatAxisParams(handle, "speed0", 800);

    // 赋值起始运动速度123
    axis6->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis6->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis6->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // 赋值回零爬行速度0
    axis6->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis6->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis6->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis6->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis6->setFloatAxisParams(handle, "accel1", 300);
    axis6->setFloatAxisParams(handle, "accel2", 500);
    axis6->setFloatAxisParams(handle, "accel0", 2000);

    // 赋值减速度123
    axis6->setFloatAxisParams(handle, "decel1", 300);
    axis6->setFloatAxisParams(handle, "decel2", 500);
    axis6->setFloatAxisParams(handle, "decel0", 2000);

    // 赋值正负软限位0
    axis6->setFloatAxisParams(handle, "fsLimit", 950);
    axis6->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis6->setFloatAxisParams(handle, "homeOffPos", 5);

    // 设置dPos、mPos、ePos默认值为0
    axis6->setFloatAxisParams(handle, "dPos", 0);
    axis6->setFloatAxisParams(handle, "mPos", 0);
    axis6->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis6->setFloatAxisParams(handle, "standByPos", 524.475);

    // 设置最大行程
    axis6->setFloatAxisParams(handle, "maxPos", 960);

    // 设置软限位传感器状态，默认为0
    axis6->setIntAxisParams(handle, "fsLimitState", 0);
    axis6->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴7
    AxisControl* axis7 = axiss.at(7);
    axis7->setIntAxisParams(handle, "type", 65);
    axis7->setIntAxisParams(handle, "units", 1000);
    axis7->setIntAxisParams(handle, "fhLimitId", 185);
    axis7->setIntAxisParams(handle, "rhLimitId", 184);
    axis7->setIntAxisParams(handle, "homeInId", 186);
    axis7->setIntAxisParams(handle, "axisEnable", 0);
    axis7->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog

    //赋值加速度曲线时间
    axis7->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis7->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis7->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis7->setFloatAxisParams(handle, "maxSpeed2", 500);

    // 赋值速度123
    axis7->setFloatAxisParams(handle, "speed1", 50);
    axis7->setFloatAxisParams(handle, "speed2", 200);
    axis7->setFloatAxisParams(handle, "speed0", 800);

    // 赋值起始运动速度123
    axis7->setFloatAxisParams(handle, "startMoveSpeed1", 25);
    axis7->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis7->setFloatAxisParams(handle, "startMoveSpeed0", 100);

    // 赋值回零爬行速度0
    axis7->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis7->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis7->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis7->setFloatAxisParams(handle, "maxAccelDecel2", 500);

    // 赋值加速度123
    axis7->setFloatAxisParams(handle, "accel1", 300);
    axis7->setFloatAxisParams(handle, "accel2", 200);
    axis7->setFloatAxisParams(handle, "accel0", 2000);

    // 赋值减速度123
    axis7->setFloatAxisParams(handle, "decel1", 300);
    axis7->setFloatAxisParams(handle, "decel2", 200);
    axis7->setFloatAxisParams(handle, "decel0", 500);

    // 赋值正负软限位0
    axis7->setFloatAxisParams(handle, "fsLimit", 1298.5);
    axis7->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis7->setFloatAxisParams(handle, "homeOffPos", 5);

    // 设置dPos、mPos、ePos默认值为0
    axis7->setFloatAxisParams(handle, "dPos", 0);
    axis7->setFloatAxisParams(handle, "mPos", 0);
    axis7->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis7->setFloatAxisParams(handle, "standByPos", 472.336);

    // 设置最大行程
    axis7->setFloatAxisParams(handle, "maxPos", 1308.5);

    // 设置软限位传感器状态，默认为0
    axis7->setIntAxisParams(handle, "fsLimitState", 0);
    axis7->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴8
    AxisControl* axis8 = axiss.at(8);
    axis8->setIntAxisParams(handle, "type", 65);
    axis8->setIntAxisParams(handle, "units", 1000);
    axis8->setIntAxisParams(handle, "fhLimitId", 193);
    axis8->setIntAxisParams(handle, "rhLimitId", 192);
    axis8->setIntAxisParams(handle, "homeInId", 194);
    axis8->setIntAxisParams(handle, "axisEnable", 0);
    axis8->setIntAxisParams(handle, "axisErrorCode", 0);

    //0点位 1原点 2jog

    //赋值加速度曲线时间
    axis8->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度123
    axis8->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis8->setFloatAxisParams(handle, "maxSpeed1", 100);
    axis8->setFloatAxisParams(handle, "maxSpeed2", 1000);

    // 赋值速度123
    axis8->setFloatAxisParams(handle, "speed1", 20);
    axis8->setFloatAxisParams(handle, "speed2", 200);
    axis8->setFloatAxisParams(handle, "speed0", 200);

    // 赋值起始运动速度123
    axis8->setFloatAxisParams(handle, "startMoveSpeed1", 5);
    axis8->setFloatAxisParams(handle, "startMoveSpeed2", 100);
    axis8->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // 赋值回零爬行速度0
    axis8->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速123
    axis8->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis8->setFloatAxisParams(handle, "maxAccelDecel1", 300);
    axis8->setFloatAxisParams(handle, "maxAccelDecel2", 1000);

    // 赋值加速度123
    axis8->setFloatAxisParams(handle, "accel1", 300);
    axis8->setFloatAxisParams(handle, "accel2", 500);
    axis8->setFloatAxisParams(handle, "accel0", 500);

    // 赋值减速度123
    axis8->setFloatAxisParams(handle, "decel1", 300);
    axis8->setFloatAxisParams(handle, "decel2", 500);
    axis8->setFloatAxisParams(handle, "decel0", 500);

    // 赋值正负软限位0
    axis8->setFloatAxisParams(handle, "fsLimit", 72);
    axis8->setFloatAxisParams(handle, "rsLimit", -5);

    // 设置回零偏移量0
    axis8->setFloatAxisParams(handle, "homeOffPos", 44);

    // 设置dPos、mPos、ePos默认值为0
    axis8->setFloatAxisParams(handle, "dPos", 0);
    axis8->setFloatAxisParams(handle, "mPos", 0);
    axis8->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis8->setFloatAxisParams(handle, "standByPos", 10);

    // 设置最大行程
    axis8->setFloatAxisParams(handle, "maxPos", 82);

    // 设置软限位传感器状态，默认为0
    axis8->setIntAxisParams(handle, "fsLimitState", 0);
    axis8->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴9
    AxisControl* axis9 = axiss.at(9);
    axis9->setIntAxisParams(handle, "type", 65);
    axis9->setIntAxisParams(handle, "units", 1000);
    axis9->setIntAxisParams(handle, "fhLimitId", 201);
    axis9->setIntAxisParams(handle, "rhLimitId", 200);
    axis9->setIntAxisParams(handle, "homeInId", 202);
    axis9->setIntAxisParams(handle, "axisEnable", 0);
    axis9->setIntAxisParams(handle, "axisErrorCode", 0);


    //赋值加速度曲线时间
    axis9->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度
    axis9->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis9->setFloatAxisParams(handle, "maxSpeed1", 200);
    axis9->setFloatAxisParams(handle, "maxSpeed2", 1200);

    // 赋值速度
    axis9->setFloatAxisParams(handle, "speed1", 50);
    axis9->setFloatAxisParams(handle, "speed2", 1000);
    axis9->setFloatAxisParams(handle, "speed0", 800);

    // 赋值起始运动速度
    axis9->setFloatAxisParams(handle, "startMoveSpeed1", 15);
    axis9->setFloatAxisParams(handle, "startMoveSpeed2", 200);
    axis9->setFloatAxisParams(handle, "startMoveSpeed0", 200);

    // 赋值回零爬行速度
    axis9->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速
    axis9->setFloatAxisParams(handle, "maxAccelDecel0", 2000);
    axis9->setFloatAxisParams(handle, "maxAccelDecel1", 500);
    axis9->setFloatAxisParams(handle, "maxAccelDecel2", 2000);

    // 赋值加速度
    axis9->setFloatAxisParams(handle, "accel1", 400);
    axis9->setFloatAxisParams(handle, "accel2", 1000);
    axis9->setFloatAxisParams(handle, "accel0", 1000);

    // 赋值减速度
    axis9->setFloatAxisParams(handle, "decel1", 400);
    axis9->setFloatAxisParams(handle, "decel2", 1000);
    axis9->setFloatAxisParams(handle, "decel0", 1000);

    // 赋值正负软限位
    axis9->setFloatAxisParams(handle, "fsLimit", 340);
    axis9->setFloatAxisParams(handle, "rsLimit", -60);

    // 设置回零偏移量
    axis9->setFloatAxisParams(handle, "homeOffPos", 13.78);

    // 设置dPos、mPos、ePos默认值为0
    axis9->setFloatAxisParams(handle, "dPos", 0);
    axis9->setFloatAxisParams(handle, "mPos", 0);
    axis9->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis9->setFloatAxisParams(handle, "standByPos", 0);

    // 设置最大行程
    axis9->setFloatAxisParams(handle, "maxPos", 360);

    // 设置软限位传感器状态，默认为0
    axis9->setIntAxisParams(handle, "fsLimitState", 0);
    axis9->setIntAxisParams(handle, "rsLimitState", 0);

    // 取出轴10
    AxisControl* axis10 = axiss.at(10);
    axis10->setIntAxisParams(handle, "type", 65);
    axis10->setIntAxisParams(handle, "units", 1000);
    axis10->setIntAxisParams(handle, "fhLimitId", 209);
    axis10->setIntAxisParams(handle, "rhLimitId", 208);
    axis10->setIntAxisParams(handle, "homeInId", 210);
    axis10->setIntAxisParams(handle, "axisEnable", 0);
    axis10->setIntAxisParams(handle, "axisErrorCode", 0);

    //赋值加速度曲线时间
    axis10->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度
    axis10->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis10->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis10->setFloatAxisParams(handle, "maxSpeed2", 500);

    // 赋值速度
    axis10->setFloatAxisParams(handle, "speed1", 20);
    axis10->setFloatAxisParams(handle, "speed2", 100);
    axis10->setFloatAxisParams(handle, "speed0", 800);

    // 赋值起始运动速度
    axis10->setFloatAxisParams(handle, "startMoveSpeed1", 5);
    axis10->setFloatAxisParams(handle, "startMoveSpeed2", 20);
    axis10->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // 赋值回零爬行速度
    axis10->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速
    axis10->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis10->setFloatAxisParams(handle, "maxAccelDecel1", 200);
    axis10->setFloatAxisParams(handle, "maxAccelDecel2", 2000);

    // 赋值加速度
    axis10->setFloatAxisParams(handle, "accel1", 200);
    axis10->setFloatAxisParams(handle, "accel2", 300);
    axis10->setFloatAxisParams(handle, "accel0", 3000);

    // 赋值减速度
    axis10->setFloatAxisParams(handle, "decel1", 200);
    axis10->setFloatAxisParams(handle, "decel2", 1500);
    axis10->setFloatAxisParams(handle, "decel0", 3000);

    // 赋值正负软限位
    axis10->setFloatAxisParams(handle, "fsLimit", 202);
    axis10->setFloatAxisParams(handle, "rsLimit", -3);

    // 设置回零偏移量
    axis10->setFloatAxisParams(handle, "homeOffPos", 3);

    // 设置dPos、mPos、ePos默认值为0
    axis10->setFloatAxisParams(handle, "dPos", 0);
    axis10->setFloatAxisParams(handle, "mPos", 0);
    axis10->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis10->setFloatAxisParams(handle, "standByPos", 5);

    // 设置最大行程
    axis10->setFloatAxisParams(handle, "maxPos", 205);

    // 设置软限位传感器状态，默认为0
    axis10->setIntAxisParams(handle, "fsLimitState", 0);
    axis10->setIntAxisParams(handle, "rsLimitState", 0);


    // 取出轴11
    AxisControl* axis11 = axiss.at(11);
    axis11->setIntAxisParams(handle, "type", 65);
    axis11->setIntAxisParams(handle, "units", 1000);
    axis11->setIntAxisParams(handle, "fhLimitId", 217);
    axis11->setIntAxisParams(handle, "rhLimitId", 216);
    axis11->setIntAxisParams(handle, "homeInId", 218);
    axis11->setIntAxisParams(handle, "axisEnable", 0);
    axis11->setIntAxisParams(handle, "axisErrorCode", 0);

    //赋值加速度曲线时间
    axis11->setFloatAxisParams(handle, "sramp", 100);

    // 赋值速度之前，先赋值最大速度
    axis11->setFloatAxisParams(handle, "maxSpeed0", 2000);
    axis11->setFloatAxisParams(handle, "maxSpeed1", 50);
    axis11->setFloatAxisParams(handle, "maxSpeed2", 500);

    // 赋值速度
    axis11->setFloatAxisParams(handle, "speed1", 20);
    axis11->setFloatAxisParams(handle, "speed2", 100);
    axis11->setFloatAxisParams(handle, "speed0", 800);

    // 赋值起始运动速度
    axis11->setFloatAxisParams(handle, "startMoveSpeed1", 5);
    axis11->setFloatAxisParams(handle, "startMoveSpeed2", 20);
    axis11->setFloatAxisParams(handle, "startMoveSpeed0", 50);

    // 赋值回零爬行速度
    axis11->setFloatAxisParams(handle, "creep", 5);

    // 赋值加减速度之前，先赋值最大加减速
    axis11->setFloatAxisParams(handle, "maxAccelDecel0", 5000);
    axis11->setFloatAxisParams(handle, "maxAccelDecel1", 200);
    axis11->setFloatAxisParams(handle, "maxAccelDecel2", 2000);

    // 赋值加速度
    axis11->setFloatAxisParams(handle, "accel1", 200);
    axis11->setFloatAxisParams(handle, "accel2", 300);
    axis11->setFloatAxisParams(handle, "accel0", 3000);

    // 赋值减速度
    axis11->setFloatAxisParams(handle, "decel1", 200);
    axis11->setFloatAxisParams(handle, "decel2", 1500);
    axis11->setFloatAxisParams(handle, "decel0", 3000);

    // 赋值正负软限位
    axis11->setFloatAxisParams(handle, "fsLimit", 202);
    axis11->setFloatAxisParams(handle, "rsLimit", -3);

    // 设置回零偏移量
    axis11->setFloatAxisParams(handle, "homeOffPos", 3);

    // 设置dPos、mPos、ePos默认值为0
    axis11->setFloatAxisParams(handle, "dPos", 0);
    axis11->setFloatAxisParams(handle, "mPos", 0);
    axis11->setFloatAxisParams(handle, "ePos", 0);

    // 设置起始位置
    axis11->setFloatAxisParams(handle, "standByPos", 5);

    // 设置最大行程
    axis11->setFloatAxisParams(handle, "maxPos", 205);

    // 设置软限位传感器状态，默认为0
    axis11->setIntAxisParams(handle, "fsLimitState", 0);
    axis11->setIntAxisParams(handle, "rsLimitState", 0);


}
