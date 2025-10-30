#include"system.h"
#include<iostream>
#include<thread>
#include<tchar.h>
#include"autoproduction.h"
#include "ProductionMaterial.h"
#include <sstream>
#include <map>         // 用于 std::map
#include <string>      // 用于 std::string
#include "ProductionMaterialManager.h"

int test_autoproduction_dataflow();

int main() {

	// 函数声明
	int setupProductionMaterialFromDatabase();

	ZMC_HANDLE handle = NULL;  //连接返回的句柄 
	uint32 cardnumcardnum = 0;   //PCI0 

	int ret = ZAux_OpenPci(cardnumcardnum, &handle); ////以太网连接控制器以太网连接控制

	if (ERR_SUCCESS != ret)
	{
		printf("PCI连接失败!\n");
		handle = NULL;
		
	}
	printf("PCI连接成功!\n");

	//测试
	SystemControl systemControl;
	AutoProduction autoProduction;
	IOControl ioControl;
	AxisControl axis0(0);
	AxisControl axis1(1);
	AxisControl axis2(2);
	AxisControl axis3(3);
	AxisControl axis4(4);
	AxisControl axis5(5);
	AxisControl axis6(6);
	AxisControl axis7(7);
	AxisControl axis8(8);
	AxisControl axis9(9);
	AxisControl axis10(10);
	AxisControl axis11(11);
	autoProduction.axisParameters = { 9.505,524.477,863.631,5.000,5.599,540.293,540.293,524.475,863.632,0.005,354.055,143.093,30.450,472.233,457.427,190.000,93.000,17.000,189.300,93.000,28.000,820.293 };
	vector<AxisControl*>axiss = {&axis0,&axis1 ,&axis2 ,&axis3 ,&axis4,&axis5,&axis6,&axis7,&axis8,&axis9,&axis10,&axis11};

	//参数初始化
	systemControl.initialAxisParams(handle, axiss);
	//创建生产资料对象
	ProductionMaterial pm;

	

	//创建守护线程
	thread safe(&SystemControl::safeGuard, ref(systemControl), ref(handle), ref(ioControl), ref(axiss));

	// 多轴运动指令集合，存储所有待执行的轴运动指令
	///*vector<AxisMotionCommand> cmds;*/
	//// 当前用户输入的轴编号（按设备配置映射，如1=横轴/X轴，2=纵轴/Y轴）
	//int axis;
	//// 目标轴需要移动到的绝对位置（单位：mm，需根据设备实际行程范围校验）
	//double pos;
	
	

	while (true) {

		//测试轴的回零运动
		cout << "回零运动测试" << endl;
		cout << "输0--------轴0回零" << endl;
		cout << "输1--------轴1回零" << endl;
		cout << "输2--------轴2回零" << endl;
		cout << "输3--------轴3回零" << endl;
		cout << "输4--------轴4回零" << endl;
		cout << "输5--------轴5回零" << endl;
		cout << "输6--------轴6回零" << endl;
		cout << "输7--------轴7回零" << endl;
		cout << "输8--------轴8回零" << endl;
		cout << "输9--------轴9回零" << endl;
		cout << "输10-------轴10回零" << endl;
		cout << "输11-------轴11回零" << endl;
		cout << "输12-------全轴回零" << endl;
		cout << "输入13------退出！！！" << endl;
		cout << "输入14，轴0单步运动向右10mm" << endl;
		cout << "输入15，轴0单步运动向左10mm" << endl;
		cout << "输入16，轴0连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入17，轴1单步运动向右10mm" << endl;
		cout << "输入18，轴1单步运动向左10mm" << endl;
		cout << "输入19，轴1连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入20，轴2单步运动向右10mm" << endl;
		cout << "输入21，轴2单步运动向左10mm" << endl;
		cout << "输入22，轴2连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入23，轴3单步运动向右10mm" << endl;
		cout << "输入24，轴3单步运动向左10mm" << endl;
		cout << "输入25，轴3连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入26，轴4单步运动向右10mm" << endl;
		cout << "输入27，轴4单步运动向左10mm" << endl;
		cout << "输入28，轴4连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入29，轴5单步运动向右10mm" << endl;
		cout << "输入30，轴5单步运动向左10mm" << endl;
		cout << "输入31，轴5连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入32，轴6单步运动向右10mm" << endl;
		cout << "输入33，轴6单步运动向左10mm" << endl;
		cout << "输入34，轴6连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入35，轴7单步运动向右10mm" << endl;
		cout << "输入36，轴7单步运动向左10mm" << endl;
		cout << "输入37，轴7连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入38，轴8单步运动向右10mm" << endl;
		cout << "输入39，轴8单步运动向左10mm" << endl;
		cout << "输入40，轴8连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入41，轴9单步运动向右10mm" << endl;
		cout << "输入42，轴9单步运动向左10mm" << endl;
		cout << "输入43，轴9连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入44，轴10单步运动向右10mm" << endl;
		cout << "输入45，轴10单步运动向左10mm" << endl;
		cout << "输入46，轴10连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;
		cout << "输入47，轴11单步运动向右10mm" << endl;
		cout << "输入48，轴11单步运动向左10mm" << endl;
		cout << "输入49，轴11连续运动,速度为200。键盘左键向左运动，键盘右键向右运动" << endl;

		cout << "输入50，全部吸盘-上升并达到等待位置" << endl;
		cout << "输入51，上料吸盘-下降到上料盒的底部吸着面" << endl;
		cout << "输入52，上料吸盘-从产品盒吸取PCB" << endl;
		cout << "输入53，上料吸盘-从产品盒取PCB放至平台1" << endl;
		cout << "输入54，上料吸盘-从产品盒取PCB放至平台2" << endl;
		cout << "输入55，上料吸盘-从产品盒取隔纸放至纸盒" << endl;
		cout << "输入56，下料吸盘-下降到平台1的吸取面" << endl;
		cout << "输入57，下料吸盘-下降到平台2的吸取面" << endl;
		cout << "输入58，下料吸盘-从平台1上吸取PCB" << endl;
		cout << "输入59，下料吸盘-从平台2上吸取PCB" << endl;
		cout << "输入60，下料吸盘-将PCB放至OK盒" << endl;
		cout << "输入61，下料吸盘-将PCB放至NG盒" << endl;
		cout << "输入62，" << endl;
		cout << "--------------------------" << endl;
		cout << "请输入你的操作！！！！！！！！" << endl;
		cout << "输入70，打开机种资料" << endl;
		cout << "输入72，载台1--定位相机识别操作" << endl;
		cout << "输入73，载台2--定位相机识别操作" << endl;
		cout << "输入74，移动到定位相机中心" << endl;
		cout << "输入75，多轴传参移动" << endl;
		cout << "输入76，单轴传参移动" << endl;
		std::cout << "输入77 - 手动写入测试补偿值 (printCodeOffsets 模拟)" << std::endl;
		std::cout << "输入78 - 执行 checkCode 校验流程（读取+更新）" << std::endl;
		std::cout << "输入79 - 查询数据库中历史补偿记录 (getLastOffsetsFromDatabase)" << std::endl;
		std::cout << "输入80 - 清空补偿表 + 日志表，并重置主键 ID" << std::endl;
		std::cout << "输入81 - 完整数据流测试" << std::endl;
		int number;
		cin >> number;
		if (number == 0) {
			axis0.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 1) {
			axis1.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 2) {
			axis2.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 3) {
			axis3.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 4) {
			axis4.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 5) {
			axis5.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 6) {
			axis6.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 7) {
			axis7.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 8) {
			axis8.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 9) {
			axis9.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 10) {
			axis10.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 11) {
			axis11.homeAxis(handle, ioControl, systemControl, axiss);
		}
		if (number == 12) {
			axis0.homeAllAxis(handle, ioControl, systemControl, axiss);;
		}
		if (number == 13) {
			break;
		}

		if (number == 14) {
			axis0.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 15) {
			axis0.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 16) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);

				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
				ZAux_Direct_Single_Cancel(handle, 0, 2);//取消轴0的运动
			}
			//按下Esc按键退出
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				break;  // 跳出循环
				}
			}
		}

		if (number == 17) {
			axis1.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 18) {
			axis1.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 19) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 1, 2);//取消轴1的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
				
			}
		}

		if (number == 20) {
			axis2.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 21) {
			axis2.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 22) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis2.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis2.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 2, 2);//取消轴2的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
			}
		}

		if (number == 23) {
			axis3.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 24) {
			axis3.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 25) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 3, 2);//取消轴3的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
		
			}
		}

		if (number == 26) {
			axis4.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 27) {
			axis4.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 28) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis4.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis4.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 4, 2);//取消轴4的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
			}
		}

		if (number == 29) {
			axis5.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 30) {
			axis5.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 31) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis5.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis5.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 5, 2);//取消轴5的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
		
			}
		}

		if (number == 32) {
			axis6.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 33) {
			axis6.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 34) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis6.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis6.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 6, 2);//取消轴6的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
				
			}
		}

		if (number == 35) {
			axis7.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 36) {
			axis7.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 37) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis7.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis7.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 7, 2);//取消轴7的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
			
			}
		}

		if (number == 38) {
			axis8.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 39) {
			axis8.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 40) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis8.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis8.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 8, 2);//取消轴8的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
			
			}
		}

		if (number == 41) {
			axis9.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 42) {
			axis9.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 43) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis9.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis9.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 9, 2);//取消轴9的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
		
			}
		}

		if (number == 44) {
			axis10.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 45) {
			axis10.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 46) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis10.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis10.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 10, 2);//取消轴10的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
			
			}
		}

		if (number == 47) {
			axis11.axisMove(handle, ioControl, systemControl, axiss, 1, 1, 10);
		}
		if (number == 48) {
			axis11.axisMove(handle, ioControl, systemControl, axiss, 1, -1, 10);
		}
		if (number == 49) {
			cout << "请选择键盘左键，或者右键，ESC按键" << endl;
			while (true)
			{
				//向左运动
				if (GetAsyncKeyState(VK_LEFT) && 0x8000)
				{
					axis11.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200);
				}
				//向右运动
				else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
				{
					axis11.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);
				}
				else
				{
					ZAux_Direct_Single_Cancel(handle, 11, 2);//取消轴11的运动
				}
				//按下Esc按键退出
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					break;  // 跳出循环
				}
	
			}
		}

		if (number == 50) {

			autoProduction.runCommand1(handle, systemControl,axiss);

		}
		if (number == 51) {
			
			autoProduction.runCommand2(handle, systemControl, axiss);

		}
		if (number == 52) {
			autoProduction.runCommand3(handle, systemControl, ioControl, axiss);
		}
		if (number == 53) {
			
			autoProduction.runCommand4(handle, systemControl, ioControl, axiss);
		

		}
		if (number == 54) {
		
			autoProduction.runCommand5(handle, systemControl, ioControl, axiss);

		}
		if (number == 55) {
			
			autoProduction.runCommand6(handle, systemControl, ioControl, axiss);

		}
		if (number == 56) {

			autoProduction.runCommand7(handle, systemControl, axiss);
		

		}
		if (number == 57) {
			autoProduction.runCommand8(handle, systemControl, axiss);
		
		}
		if (number == 58) {

			autoProduction.runCommand9(handle, systemControl, ioControl, axiss);
		

		}
		if (number == 59) {
			
			autoProduction.runCommand10(handle, systemControl, ioControl, axiss);
		}
		if (number == 60) {

			autoProduction.runCommand11(handle, systemControl, ioControl, axiss);
		}

		if (number == 61) {
		/*	autoProduction.runCommand12(handle, axiss);*/
			//真空破坏//真空破坏
		
		}

		if (number == 70) {
			//打开载台_抽风机变频器，打开载台1_管道电磁阀通断
			ioControl.setOutput(handle, 240, 1, "DOB");
			ioControl.setOutput(handle, 241, 1, "DOB");

		}

		if (number == 71) {
			//关闭载台_抽风机变频器，关闭载台1_管道电磁阀通断
			//关闭上料吸盘真空供给阀，
			ioControl.setOutput(handle, 240, 0, "DOB");
			ioControl.setOutput(handle, 241, 0, "DOB");
			ioControl.setOutput(handle, 234, 0, "DOA");
			ioControl.setOutput(handle, 236, 0, "DOA");
			ioControl.setOutput(handle, 237, 0, "DOA");
			ioControl.setOutput(handle, 242, 0, "DOB");
		}

		if (number == 72) {
			cout << "手动模式：←左 →右 ↑上 ↓下 | 1:↖ 2:↗ 3:↙ 4:↘ | ESC退出" << endl;
			while (true) {
				// 方向键控制（原逻辑）
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 左移
				}
				else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 右移
				}
				else {
					ZAux_Direct_Single_Cancel(handle, 0, 2); // 停止轴0
				}

				if (GetAsyncKeyState(VK_UP) & 0x8000) {
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 上移
				}
				else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 下移
				}
				else {
					ZAux_Direct_Single_Cancel(handle, 3, 2); // 停止轴3
				}

				// 新增斜向移动（数字键1-4）
				if (GetAsyncKeyState('1') & 0x8000) { // ↖ 左上
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 左
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 上
				}
				else if (GetAsyncKeyState('2') & 0x8000) { // ↗ 右上
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 右
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 上
				}
				else if (GetAsyncKeyState('3') & 0x8000) { // ↙ 左下
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 左
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 下
				}
				else if (GetAsyncKeyState('4') & 0x8000) { // ↘ 右下
					axis0.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 右
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 下
				}

				// 退出检测
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					ZAux_Direct_Single_Cancel(handle, 0, 2); // 停轴0
					ZAux_Direct_Single_Cancel(handle, 3, 2); // 停轴3
					break;
				}

				Sleep(10); // 维持低CPU占用
			}
		}

		if (number == 73) {
			cout << "手动模式：←左 →右 ↑上 ↓下 | 1:↖ 2:↗ 3:↙ 4:↘ | ESC退出" << endl;
			while (true) {
				// 方向键控制（原逻辑）
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 左移
				}
				else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 右移
				}
				else {
					ZAux_Direct_Single_Cancel(handle, 1, 2); // 停止轴1
				}

				if (GetAsyncKeyState(VK_UP) & 0x8000) {
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 上移
				}
				else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 下移
				}
				else {
					ZAux_Direct_Single_Cancel(handle, 3, 2); // 停止轴3
				}

				// 新增斜向移动（数字键1-4）
				if (GetAsyncKeyState('1') & 0x8000) { // ↖ 左上
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 左
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 上
				}
				else if (GetAsyncKeyState('2') & 0x8000) { // ↗ 右上
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 右
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 上
				}
				else if (GetAsyncKeyState('3') & 0x8000) { // ↙ 左下
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, -1, 200); // 左
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 下
				}
				else if (GetAsyncKeyState('4') & 0x8000) { // ↘ 右下
					axis1.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 右
					axis3.axisMove(handle, ioControl, systemControl, axiss, 2, 1, 200);  // 下
				}

				// 退出检测
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					ZAux_Direct_Single_Cancel(handle, 1, 2); // 停轴0
					ZAux_Direct_Single_Cancel(handle, 3, 2); // 停轴3
					break;
				}

				Sleep(10); // 维持低CPU占用
			}
		}

		// 

		//if (number == 75) {
		//	cmds.clear(); //必须清空之前的指令(clear是vector容器内部函数)
		//	// 多轴模式
		//	cout << "输入轴号和位置（格式：轴号 位置），输入-1结束:\n";
		//	while (cin >> axis && axis != -1) {// 1. 持续读取轴号，直到输入-1
		//		cin >> pos;                    // 2. 读取该轴的目标位置
		//		cmds.push_back({ axis, pos }); // 3. 将轴号+位置存入指令列表(push_back是vector容器内部函数)
		//	}// 结构体初始化
		//	AutoProduction::MoveAxes(handle, cmds);
		//}

		//if (number == 76) {
		//	// 单轴模式
		//	cout << "输入轴号和位置（格式：轴号 位置）:";
		//	cin >> axis >> pos;
		//	// 显式构造结构体对象
		//	AutoProduction::MoveAxes(handle, AxisMotionCommand{ axis, pos });
		//}
		//手动写入数据
		if (number == 77) {
			AutoProduction testProd;
			testProd.productionName = "TEST_PRODUCT";

			// 手动填充 printCodeOffsets，用于模拟补偿值写入数据库
			testProd.printCodeOffsets = {
				{0.12f, -0.25f, 0.05f, 1.0f, 0.0f},
				{0.10f, -0.20f, 0.02f, 1.0f, 1.0f},
				{0.00f,  0.00f, 0.00f, 2.0f, 0.0f}
			};

			MySqlConnect mc;
			if (!mc.connect("root", "134679852", "autoproduction", "127.0.0.1", 3306)) {
				std::cerr << " 无法连接数据库，请检查账号/密码/IP/库名" << std::endl;
				return -1;
			}

			// 构造 SQL 插入语句
			std::stringstream ss_update;
			ss_update << "INSERT INTO autoproduction_offsets (production_name, board_id, code_id, x_offset, y_offset, angle_offset) VALUES ";
			for (size_t i = 0; i < testProd.printCodeOffsets.size(); ++i) {
				const auto& offset = testProd.printCodeOffsets[i];
				ss_update << "('" << testProd.productionName << "', "
					<< (int)offset[3] << ", " << (int)offset[4] << ", "
					<< offset[0] << ", " << offset[1] << ", " << offset[2] << ")";
				if (i < testProd.printCodeOffsets.size() - 1) ss_update << ",";
			}
			ss_update << " ON DUPLICATE KEY UPDATE "
				<< "x_offset=VALUES(x_offset), y_offset=VALUES(y_offset), angle_offset=VALUES(angle_offset)";

			std::cout << "[调试] 执行 SQL:\n" << ss_update.str() << std::endl;

			if (mc.query(ss_update.str())) {
				std::cout << "手动补偿值已成功写入数据库 autoproduction_offsets 表。" << std::endl;
			}
			else {
				std::cerr << "写入失败，请检查 SQL 拼接或表结构。" << std::endl;
			}

			std::cout << "按回车退出..." << std::endl;
			std::cin.get(); std::cin.get();
		}

		// ✅ 78: 测试 checkCode（执行写入）
		if (number == 78) {
			std::cout << "\n=== 测试喷码校验与补偿逻辑 (checkCode) ===\n" << std::endl;

			AutoProduction testProd;
			testProd.productionName = "TEST_PRODUCT";

			testProd.codePositions = {
				{ 10.0f, 20.0f, 0.0f, 1.0f, 0.0f },
				{ 15.0f, 25.0f, 0.0f, 1.0f, 1.0f },
				{ 30.0f, 40.0f, 0.0f, 2.0f, 0.0f }
			};

			std::vector<int> badBoards = { 2 };

			MySqlConnect mc;
			if (!mc.connect("root", "134679852", "autoproduction", "127.0.0.1", 3306)) {
				std::cerr << " 数据库连接失败，请检查配置" << std::endl;
				return -1;
			}

			testProd.checkCode(handle, ioControl, systemControl, axiss, pm, badBoards, mc);

			std::cout << "\n 测试完成，请查看 autoproduction_offsets 和 production_check_logs。" << std::endl;
			std::cin.get(); std::cin.get();
		}

		// ---------- 79: 读取并打印 autoproduction_offsets 表内容 ----------
		if (number == 79) {
			std::cout << "\n=== 读取数据库中已有补偿记录 (79) ===\n" << std::endl;

			const char* db_host = "127.0.0.1";
			const char* db_user = "root";
			const char* db_pass = "134679852";
			const char* db_name = "autoproduction";
			unsigned int db_port = 3306;

			// 初始化并连接
			MYSQL* conn = mysql_init(nullptr);
			if (!conn) {
				std::cerr << "mysql_init() failed\n";
				return -1;
			}

			if (mysql_real_connect(conn, db_host, db_user, db_pass, db_name, db_port, nullptr, 0) == nullptr) {
				std::cerr << "❌ 数据库连接失败: " << mysql_error(conn) << std::endl;
				mysql_close(conn);
				return -1;
			}

			// 构造查询（你可以修改 production_name）
			std::string productionName = "TEST_PRODUCT";
			std::string sql = "SELECT id, production_name, board_id, code_id, x_offset, y_offset, angle_offset "
				"FROM autoproduction_offsets "
				"WHERE production_name = '" + productionName + "' "
				"ORDER BY board_id, code_id;";


			if (mysql_query(conn, sql.c_str())) {
				std::cerr << "❌ 查询失败: " << mysql_error(conn) << std::endl;
				mysql_close(conn);
				return -1;
			}

			MYSQL_RES* result = mysql_store_result(conn);
			if (result == nullptr) {
				std::cerr << "❌ 获取结果失败: " << mysql_error(conn) << std::endl;
				mysql_close(conn);
				return -1;
			}

			// 打印列名
			unsigned int num_fields = mysql_num_fields(result);
			MYSQL_FIELD* fields = mysql_fetch_fields(result);
			for (unsigned int i = 0; i < num_fields; ++i) {
				std::cout << fields[i].name;
				if (i + 1 < num_fields) std::cout << " | ";
			}
			std::cout << std::endl;
			std::cout << std::string(80, '-') << std::endl;

			// 遍历行并打印
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(result))) {
				// 注意 row 列可能为 NULL
				for (unsigned int i = 0; i < num_fields; ++i) {
					if (row[i]) std::cout << row[i];
					else std::cout << "NULL";
					if (i + 1 < num_fields) std::cout << " | ";
				}
				std::cout << std::endl;
			}

			// 清理
			mysql_free_result(result);
			mysql_close(conn);

			std::cout << "\n=== 打印完成 ===\n按回车退出..." << std::endl;
			std::cin.get(); std::cin.get();
		}





		if (number == 80) {
			std::cout << "\n=== 清空两张表并重置自增 ID ===\n" << std::endl;

			MySqlConnect mc;
			if (!mc.connect("root", "134679852", "autoproduction", "127.0.0.1", 3306)) {
				std::cerr << " 数据库连接失败，请检查配置" << std::endl;
				return -1;
			}

			bool ok1 = mc.query("TRUNCATE TABLE autoproduction_offsets");
			bool ok2 = mc.query("TRUNCATE TABLE production_check_logs");

			if (ok1 && ok2) {
				std::cout << " 两张表已清空，ID 已重置从 1 开始。" << std::endl;
			}
			else {
				std::cerr << " 清空失败，请检查权限或表名。" << std::endl;
			}

			std::cout << "按回车退出..." << std::endl;
			std::cin.get(); std::cin.get();
		}

		if (number == 81) {
			setupProductionMaterialFromDatabase();
			std::cout << "测试流程已结束，按回车键退出..." << std::endl;
			std::cin.get(); std::cin.get();
		}
	}
	safe.join();
	return 0;
	
}
//// =====================
//// 自动生产数据流与数据库交互测试主函数（改进版）
//// =====================
//int test_autoproduction_dataflow() {
//
//	ZMC_HANDLE handle = NULL;  //连接返回的句柄 
//
//	// 1️⃣ 构造假生产资料参数
//	ProductionMaterial pm;
//	pm.totalBoards = 2; // 假设需要喷2块基板
//
//	pm.childBoardprintCodePositions = {
//		{10.0f, 20.0f, 0.0f, 0.0f, 0.0f},
//		{15.0f, 25.0f, 0.0f, 0.0f, 1.0f},
//	
//	};
//	pm.firstExtendedprintCodePositions = {
//		{10.0f, 20.0f, 0.0f, 0.0f, 0.0f},
//		{12.0f, 22.0f, 0.0f, 1.0f, 0.0f}
//	};
//	pm.childBoardCodeInformation = { {2, 2} };
//	pm.printRuleName = { "ruleA", "ruleB" };
//	pm.badTarget_X = 30.0f;
//	pm.badTarget_Y = 40.0f;
//
//	// 2️⃣ 构造必要的对象
//	SystemControl systemControl;
//	AutoProduction autoProduction;
//	IOControl ioControl;
//	std::vector<AxisControl*> axiss;
//
//	autoProduction.productionName = "TEST_PRODUCT"; // ✅ 用同一名字方便数据库测试
//
//	// 3️⃣ 跑自动生产主流程（会自动跳过所有运动和喷码）
//	autoProduction.autoProductionRun(handle, ioControl, systemControl, axiss, pm);
//
//	std::cout << "\n自动生产数据流测试完成！开始写入数据库..." << std::endl;
//
//	// 4️⃣ 建立数据库连接
//	MySqlConnect mc;
//	if (!mc.connect("root", "134679852", "autoproduction", "127.0.0.1", 3306)) {
//		std::cerr << "❌ 数据库连接失败，请检查账号/密码/IP/库名" << std::endl;
//		return -1;
//	}
//
//	// 5️⃣ 写入 autoproduction_offsets 表
//	std::stringstream ss1;
//	ss1 << "INSERT INTO autoproduction_offsets (production_name, board_id, code_id, x_offset, y_offset, angle_offset) VALUES "
//		<< "('TEST_PRODUCT', 1, 0, 0.12, -0.25, 0.05),"
//		<< "('TEST_PRODUCT', 1, 1, 0.10, -0.20, 0.02),"
//		<< "('TEST_PRODUCT', 2, 0, 0.00, 0.00, 0.00) "
//		<< "ON DUPLICATE KEY UPDATE "
//		<< "x_offset=VALUES(x_offset), y_offset=VALUES(y_offset), angle_offset=VALUES(angle_offset);";
//
//	if (mc.query(ss1.str()))
//		std::cout << "✅ autoproduction_offsets 写入成功" << std::endl;
//	else
//		std::cerr << "❌ 写入 autoproduction_offsets 失败" << std::endl;
//
//	// 6️⃣ 写入 autoproduction_counter 表
//	std::stringstream ss2;
//	ss2 << "INSERT INTO autoproduction_counter (productionName, baseId, totalBoards, currentCode) VALUES "
//		<< "('TEST_PRODUCT', 1, 2, 10) "
//		<< "ON DUPLICATE KEY UPDATE "
//		<< "baseId=VALUES(baseId), totalBoards=VALUES(totalBoards), currentCode=VALUES(currentCode);";
//
//	if (mc.query(ss2.str()))
//		std::cout << "✅ autoproduction_counter 写入成功" << std::endl;
//	else
//		std::cerr << "❌ 写入 autoproduction_counter 失败" << std::endl;
//
//
//	// --------------------
//	// ✅ production_check_logs
//	// --------------------
//	std::stringstream ss3;
//	ss3 << "INSERT INTO production_check_logs "
//		<< "(production_name, board_id, code_id, vision_offset_x, vision_offset_y, vision_offset_angle, is_bad_board) VALUES "
//		<< "('TEST_PRODUCT', 1, 0, 0.1, 0.2, 0.0, 0),"
//		<< "('TEST_PRODUCT', 1, 1, 0.15, 0.25, 0.0, 0),"
//		<< "('TEST_PRODUCT', 2, 0, 0.12, 0.22, 0.0, 1);";
//
//	if (mc.query(ss3.str()))
//		std::cout << "✅ production_check_logs 写入成功" << std::endl;
//	else
//		std::cerr << "❌ 写入 production_check_logs 失败" << std::endl;
//
//	std::cout << "\n=== ✅ 测试完成！请在 Navicat 查看三张表数据 ===" << std::endl;
//	std::cout << "按回车退出..." << std::endl;
//	std::cin.get(); std::cin.get();
//	return 0;
//}
int setupProductionMaterialFromDatabase() {
	ZMC_HANDLE handle = NULL;  //连接返回的句柄 
	AutoProduction autoProduction;

	std::cout << "=== 从数据库读取生产资料 ===" << std::endl;

	// 创建 ProductionMaterialManager（替代原来的 DAO 和 Service）
	ProductionMaterialManager manager("localhost", "root", "134679852", "printing_machine", 3306);

	// 检查数据库连接
	if (!manager.testConnection()) {
		std::cerr << "数据库连接失败！" << std::endl;
		return -1;
	}

	// 检查可用的生产名称
	std::cout << " 检查数据库中的生产名称..." << std::endl;
	auto productions = manager.getAllProductionNames();
	std::cout << "   可用的生产名称:" << std::endl;
	for (const auto& name : productions) {
		std::cout << "   - '" << name << "'" << std::endl;
	}

	// 使用 Manager 创建 ProductionMaterial 对象
	auto pm = manager.createProductionMaterial("NEW12", "text", false);

	if (pm) {
		std::cout << "生产资料加载成功！" << std::endl;

		// 打印子板坐标
		if (!pm->childBoardprintCodePositions.empty()) {
			std::cout << "第一个子板码坐标:" << std::endl;
			for (size_t i = 0; i < (std::min)(pm->childBoardprintCodePositions.size(), size_t(3)); ++i) {
				const auto& pos = pm->childBoardprintCodePositions[i];
				if (pos.size() >= 5) {
					std::cout << " X, Y" << "(" << pos[0] << ", " << pos[1]
						<< "), 角度: " << pos[2] << ", 板索引: " << pos[3] << "  码索引" << pos[4] << std::endl;
				}
			}
		}

		// 打印拓展码坐标
		if (!pm->firstExtendedprintCodePositions.empty()) {
			std::cout << "拓展码坐标:" << std::endl;
			for (size_t i = 0; i < (std::min)(pm->firstExtendedprintCodePositions.size(), size_t(3)); ++i) {
				const auto& pos = pm->firstExtendedprintCodePositions[i];
				if (pos.size() >= 5) {
					std::cout << " X, Y" << "(" << pos[0] << ", " << pos[1]
						<< "), 角度: " << pos[2] << ", 板索引: " << pos[4] << "  码索引" << pos[3] << std::endl;
				}
			}
		}

		// 生产完成后保存计数
		std::cout << "更新生产计数..." << std::endl;
		pm->totalProductionCount += 1;
		bool saveSuccess = manager.updateProductionCounter("NEW12", pm->totalProductionCount);

		if (saveSuccess) {
			std::cout << "生产计数更新成功！当前计数: " << pm->totalProductionCount << std::endl;
		}
		else {
			std::cout << "生产计数更新失败！" << std::endl;
		}

		// 2. 构造必要的对象
		SystemControl systemControl;
		IOControl ioControl;
		std::vector<AxisControl*> axiss; // 可以为空

		autoProduction.productionName = "NEW12"; // 对应数据库里的 productionName

		// 3. 跑自动生产主流程（会自动跳过所有运动和喷码）
		autoProduction.autoProductionRun(handle, ioControl, systemControl, axiss, *pm);

		// 打印每个码位的信息
		if (!autoProduction.codePositions.empty()) {
			std::cout << "生成的所有码坐标:" << std::endl;
			for (size_t i = 0; i < autoProduction.codePositions.size(); ++i) {
				const auto& position = autoProduction.codePositions[i];
				if (position.size() >= 5) {
					std::cout << "码点 " << i << ": "
						<< "x=" << position[0] << ", "
						<< "y=" << position[1] << ", "
						<< "角度=" << position[2] << ", "
						<< "子板=" << static_cast<int>(position[3]) << ", "
						<< "码索引=" << static_cast<int>(position[4]) << std::endl;
				}
			}
		}
		else {
			std::cout << "警告: codePositions 为空！" << std::endl;
		}

		// 打印调试信息
		pm->printDebugInfo();

	}
	else {
		std::cerr << "生产资料加载失败！" << std::endl;
		return -1;
	}

	std::cout << "自动生产数据流测试完成！请检查数据库内容。" << std::endl;
	return 0;
}