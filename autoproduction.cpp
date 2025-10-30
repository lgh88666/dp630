#include "autoproduction.h"
#include "reorder.h"            // 引入新的头文件
#include <winsock.h> // 在Windows上，通常需要在mysql.h之前
#include <mysql.h>
#include <iostream>
#include <cmath>     // 用于三角函数计算
#include <iostream>  // For debugging output (重复)
#include <thread>    // For this_thread::sleep_for
#include <chrono>    // For chrono::milliseconds
#include <map>         // 用于 map
#include <sstream>     // 用于 stringstream
#include <string>      // 用于 string

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define DEG_TO_RAD(x) ((x) * M_PI / 180.0)
#endif


//读取坐标函数
void AutoProduction::getCurrentCoordinate(ZMC_HANDLE& handle, vector<AxisControl*>& axiss) {

	// 实际硬件读取操作（示例伪代码）
	AxisControl* axis0 = axiss.at(0);
	AxisControl* axis1 = axiss.at(1);
	AxisControl* axis2 = axiss.at(2);
	AxisControl* axis3 = axiss.at(3);
	AxisControl* axis4 = axiss.at(4);
	//AxisControl* axis8 = axiss.at(8);
		// 假设轴0为X，轴3为Y（根据实际硬件调整）
	ZAux_Direct_GetDpos(handle, 0, &currentX0);
	ZAux_Direct_GetDpos(handle, 0, &currentX1);
	ZAux_Direct_GetDpos(handle, 2, &currentY1);
	ZAux_Direct_GetDpos(handle, 3, &currentY2);
	ZAux_Direct_GetDpos(handle, 4, &currentY3);
	ZAux_Direct_GetDpos(handle, 4, &Current_angle);
}

//点击移动函数
void clickMove(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& sysParam, vector<AxisControl*> axiss)
{
	AxisControl* axis0 = axiss.at(0); // 载台X轴
	AxisControl* axis3 = axiss.at(3); // 定位相机Y轴

	// 斜向移动（优先判断）
	if (GetAsyncKeyState('1') & 0x8000) { // 左上
		axis0->axisMove(handle, ioControl, sysParam, axiss, 2, -1, 200);
		axis3->axisMove(handle, ioControl, sysParam, axiss, 2, -1, 200);
	}
	else if (GetAsyncKeyState('2') & 0x8000) { // 右上
		axis0->axisMove(handle, ioControl, sysParam, axiss, 2, 1, 200);
		axis3->axisMove(handle, ioControl, sysParam, axiss, 2, -1, 200);
	}
	else if (GetAsyncKeyState('3') & 0x8000) { // 左下
		axis0->axisMove(handle, ioControl, sysParam, axiss, 2, -1, 200);
		axis3->axisMove(handle, ioControl, sysParam, axiss, 2, 1, 200);
	}
	else if (GetAsyncKeyState('4') & 0x8000) { // 右下
		axis0->axisMove(handle, ioControl, sysParam, axiss, 2, 1, 200);
		axis3->axisMove(handle, ioControl, sysParam, axiss, 2, 1, 200);
	}
	// 添加数字键 '5' 的回零逻辑
	else if (GetAsyncKeyState('5') & 0x8000) { // 按下数字五
		// 载台X轴回零
		axis0->homeAxis(handle, ioControl, sysParam, axiss);
		// 定位相机Y轴回零
		axis3->homeAxis(handle, ioControl, sysParam, axiss);
		while (1) {
			axis0->getAxisParams(handle, "idle");
			axis3->getAxisParams(handle, "idle");
			if (axis0->idle < 0 && axis3->idle < 0)break;
		}
	}
	// 方向键控制 (只有当没有按下其他数字键时才执行)
	else {
		// 轴0左右控制
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			axis0->axisMove(handle, ioControl, sysParam, axiss, 2, -1, 200);
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			axis0->axisMove(handle, ioControl, sysParam, axiss, 2, 1, 200);
		}
		else {
			ZAux_Direct_Single_Cancel(handle, 0, 2); // 停止轴0
		}

		// 轴3上下控制
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			axis3->axisMove(handle, ioControl, sysParam, axiss, 2, -1, 200);
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			axis3->axisMove(handle, ioControl, sysParam, axiss, 2, 1, 200);
		}
		else {
			ZAux_Direct_Single_Cancel(handle, 3, 2); // 停止轴3
		}
	}
}

//点到点移动函数
void AutoProduction::moveTo(ZMC_HANDLE& handle, vector<AxisControl*>& axiss, int xAxisIdx, float targetX, int yAxisIdx, float targetY) {

	AxisControl* axisX = axiss.at(xAxisIdx);
	AxisControl* axisY = axiss.at(yAxisIdx);

	// 直接访问 axisId 成员变量
	int axisXID = axisX->axisId;
	int axisYID = axisY->axisId; // 修正了这里的错误

	ZAux_Direct_Single_MoveAbs(handle, axisXID, targetX);
	ZAux_Direct_Single_MoveAbs(handle, axisYID, targetY);

	// 判断轴运动是否完成
	// 注意：这里的判断逻辑依赖于 ZMotion 控制器在轴运动完成后将 'idle' 设置为负值
	bool x_done = false;
	bool y_done = false;

	while (true) {
    axisX->getAxisParams(handle, "idle");
    axisY->getAxisParams(handle, "idle");
    // ...判断...
    if (x_done && y_done) break;//检查X、Y轴是否空闲
    //if (systemControl.errorCode.load() != 0) return; // 插入
    this_thread::sleep_for(chrono::milliseconds(10));
}
}

//自动生产函数
void AutoProduction::autoProductionRun(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl,
	vector<AxisControl*>& axiss, ProductionMaterial& pm) {

	if (axiss.empty()) {

		cout << "[测试模式] 无运动轴，跳过所有运动相关流程，只跑数据流和数据库。" << endl;
		cout << "[DEBUG] 进入详细测试模式分支" << endl;
		// 1. 生成所有码坐标
		cout << "[测试模式] 1. 生成所有码坐标..." << endl;
		auto codes = generateChildBoardCodes(pm);
		for (size_t i = 0; i < codes.size(); ++i) {
			cout << "  码点" << i << ": x=" << codes[i][0] << ", y=" << codes[i][1]
				<< ", angle=" << codes[i][2] << ", 板=" << codes[i][3] << ", 码=" << codes[i][4] << endl;
		}

		// 2. 排序
		cout << "[测试模式] 2. 对所有码点排序..." << endl;
		reorder_points(printCodePositions);

		// 3. 生成不良标识点
		cout << "[测试模式] 3. 生成不良标识点..." << endl;
		auto badLabels = generateDefectiveLabel(pm);
		for (size_t i = 0; i < badLabels.size(); ++i) {
			cout << "  不良点" << i << ": x=" << badLabels[i][0] << ", y=" << badLabels[i][1]
				<< ", 板=" << badLabels[i][2] << endl;
		}

		// 4. 连接数据库
		cout << "[测试模式] 4. 连接数据库..." << endl;
		MySqlConnect mc;
		if (!mc.connect("root", "134679852", "autoproduction", "127.0.0.1", 3306)) {
			cerr << "[测试模式] 数据库连接失败！" << endl;
			return;
		}
		cout << "[测试模式] 数据库连接成功。" << endl;

		// 5. 查询 autoproduction_counter 表
		cout << "[测试模式] 5. 查询 autoproduction_counter 表..." << endl;
		string sql = "select * from autoproduction_counter where productionName = '" + productionName + "'";
		if (mc.query(sql) && mc.next()) {
			cout << "  当前基板ID: " << mc.value(2) << ", 总基板数: " << mc.value(3) << ", 当前码: " << mc.value(4) << endl;
		}
		else {
			cout << "  未查到 productionName=" << productionName << " 的记录。" << endl;
		}

		// 6. 写入补偿表
		cout << "[测试模式] 6. 写入 autoproduction_offsets 表（模拟）..." << endl;
		stringstream ss;
		ss << "INSERT INTO autoproduction_offsets (production_name, board_id, code_id, x_offset, y_offset, angle_offset) VALUES ('"
			<< productionName << "', 0, 0, 0.1, -0.2, 0.05) ON DUPLICATE KEY UPDATE x_offset=VALUES(x_offset), y_offset=VALUES(y_offset), angle_offset=VALUES(angle_offset)";
		if (mc.query(ss.str())) {
			cout << "  补偿表写入成功。" << endl;
		}
		else {
			cout << "  补偿表写入失败。" << endl;
		}

		// 7. 读取补偿表
		cout << "[测试模式] 7. 读取 autoproduction_offsets 表..." << endl;
		sql = "SELECT board_id, code_id, x_offset, y_offset, angle_offset FROM autoproduction_offsets WHERE production_name = '" + productionName + "'";
		if (mc.query(sql)) {
			int count = 0;
			while (mc.next()) {
				cout << "  补偿记录: 板 " << mc.value(0) << ", 码 " << mc.value(1)
					<< ", x=" << mc.value(2) << ", y=" << mc.value(3) << ", angle=" << mc.value(4) << endl;
				++count;
			}
			if (count == 0) cout << "  没有补偿记录。" << endl;
		}

		cout << "[测试模式] 数据流和数据库流程已执行完毕。" << endl;
		return;
	}

	// 需要的参数有：1、第一块子板上的不良标识坐标
				  // 2、第一块子板码上的信息，n个码，参数形式：5D -> [x, y, angle, childBoardIndex,codeIndex(应为0)]
				  // 3、第一个子板的扩展参考点坐标 ，参数形式：5D -> [x, y, childBoardIndex(应为0),codeIndex] 

	////////////////////////////////////////////////初始化////////////////////////////////////////////////////////

	const float initialDistance = 100;//移动到目标相机的距离

	AxisControl* axis0 = axiss.at(0);

	//生成所有码坐标并存入了printCodePositions与codePositions
	generateChildBoardCodes(pm);

	//对所有码的排序
	reorder_points(printCodePositions);

	//生成所有子板的不良标识点的坐标
	generateDefectiveLabel(pm);

	//连接数据库
	MySqlConnect mc;
	mc.connect("root", "134679852", "autoproduction", "127.0.0.1", 3306);

	//读取当前数据库的信息
	string sql = "select * from autoproduction_counter where productionName = '" + productionName + "'";
	mc.query(sql);
	//取出本次生产的信息
	mc.next();
	//对相关参数赋值
	baseId = stoi(mc.value(2));		//当前喷印基板id
	pm.totalBoards = stoi(mc.value(3));//此资料需要喷印的总基板数
	currentCode = stoi(mc.value(4));//当前喷印到第几个码

	//在数据库读取上一次喷印的补偿值并应用到码点位置
	applyOffsetsFromDatabase(mc);

	////////////////////////////////////////////////生产循环/////////////////////////////////////////////////

	while (systemControl.errorCode.load() == 0 && baseId <= pm.totalBoards) { // 用errorCode替换stopRunAutoRroductionFlag

		//每个生产步骤之前都检测一下是否需要停止生产,以提高实时性。
		//if (systemControl.errorCode.load() != 0) return;
	
		//runCommand4(handle, ioControl, axiss);//将板子放在轴一

		//ZAux_Direct_Single_MoveAbs(handle, 0, initialDistance);
		//判断轴是否运动完成
		/*while (1) {
			axis0->getAxisParams(handle, "idle");

			if (axis0->idle < 0)break;
		}*/

		//上料误差识别
		identifyPositions(handle, ioControl, systemControl, axiss, pm);

		//生产步骤1
		//if (systemControl.errorCode.load() != 0) return;
		//识别不良标识，返回不良子板索引
		recognizeBadBabel(handle, ioControl, systemControl, axiss, defectiveLabel);

		//生产步骤2
		//if (systemControl.errorCode.load() != 0) return;
		//删除不良板上的所有码
		deleteBadBabel(badBabelIndex);

		//生产步骤3
		//if (systemControl.errorCode.load() != 0) return;
		//喷码函数
		printProduction(handle, ioControl, systemControl, axiss, pm, mc);

		//生产步骤4
		//if (systemControl.errorCode.load() != 0) return;
		//喷码位置校验函数
		checkCode(handle, ioControl,systemControl,axiss, pm, badBabelIndex,mc);
		////生产步骤5
		//if (systemControl.errorCode.load() != 0) return;
		////生产步骤6
		//if (systemControl.errorCode.load() != 0) return;
		////生产步骤7
		//if (systemControl.errorCode.load() != 0) return;
		////生产步骤8...........

	}

}

// 生成后续子板扩展码
// 函数内部会同时更新 this->codePositions 和 this->printCodePositions，并返回 codePositions 的副本。
vector<vector<float>> AutoProduction::generateChildBoardCodes(ProductionMaterial& pm) {
	this->codePositions.clear();

	// 校验基本输入
	if (pm.childBoardprintCodePositions.empty() || pm.firstExtendedprintCodePositions.size() < 1) {
		return this->codePositions;
	}

	// 1. 找出模板子板（boardID == 0）的所有码
	vector<vector<float>> blueprintCodes;
	for (const auto& code : pm.childBoardprintCodePositions) {
		if (code.size() >= 5 && static_cast<int>(code[3]) == 0) {
			blueprintCodes.push_back(code);
		}
	}

	if (blueprintCodes.empty()) return this->codePositions;

	// 2. 使用 firstExtendedprintCodePositions[0] 作为子板 0 的第一个码基准点
	const auto& refCode = pm.firstExtendedprintCodePositions[0];
	float refX = refCode[0];
	float refY = refCode[1];
	float refAngle = refCode[2];

	// 3. 计算结构偏移（相对于基准点）
	vector<tuple<float, float, float, int>> relativeOffsets;  // dx, dy, dAngle, codeID

	for (const auto& code : blueprintCodes) {
		int codeID = static_cast<int>(code[4]);
		float dx = code[0] - refX;
		float dy = code[1] - refY;
		float dAngle = code[2] - refAngle;

		relativeOffsets.emplace_back(dx, dy, dAngle, codeID);
	}

	// 4. 遍历每个扩展码（每块子板的第一个码）
	for (const auto& base : pm.firstExtendedprintCodePositions) {
		if (base.size() < 5) continue;

		float baseX = base[0];
		float baseY = base[1];
		float baseAngle = base[2];
		int boardID = static_cast<int>(base[3]);

		float rad = baseAngle * static_cast<float>(M_PI) / 180.0f;
		float cosA = cos(rad);
		float sinA = sin(rad);

		// 推导整块子板的所有码
		for (const auto& offset : relativeOffsets) {
			float dx = get<0>(offset);
			float dy = get<1>(offset);
			float dA = get<2>(offset);
			int codeID = get<3>(offset);

			float x = baseX + dx * cosA - dy * sinA;
			float y = baseY + dx * sinA + dy * cosA;
			float angle = baseAngle + dA;

			this->codePositions.push_back({
				x, y, angle,
				static_cast<float>(boardID),
				static_cast<float>(codeID)
				});
		}
	}

	this->printCodePositions = this->codePositions;
	return this->codePositions;
}

// 生成不良标识的坐标,基于第一个子板上的第一个码进行推断,并与子板id进行绑定
// 返回一个包含不良标识坐标的向量，这些坐标是根据第一个子板上的第一个码推断出来的
vector<vector<float>> AutoProduction::generateDefectiveLabel(ProductionMaterial& pm) {
	vector<vector<float>> localDefectiveLabels;

	// 取模板子板第一个码的位置（作为结构参考点）
	const vector<float>& templateCode = pm.firstExtendedprintCodePositions[0];
	float baseX = templateCode[0];
	float baseY = templateCode[1];

	// 获取原始不良标识（在模板子板上）
	float badX = pm.badTarget_X;
	float badY = pm.badTarget_Y;

	// 计算相对偏移（结构坐标系下）
	float dx = badX - baseX;
	float dy = badY - baseY;

	// 添加模板子板原始不良位置
	localDefectiveLabels.push_back({ badX, badY, 0.0f });

	// 遍历扩展码（即每块子板的第一个码）
	for (size_t i = 1; i < pm.firstExtendedprintCodePositions.size(); ++i) {
		const vector<float>& ext = pm.firstExtendedprintCodePositions[i];
		if (ext.size() < 5) continue;

		float ex = ext[0];
		float ey = ext[1];
		float ea = ext[2]; // 角度（单位：度）
		int boardID = static_cast<int>(ext[3]);

		// 将偏移向量 (dx, dy) 旋转 ea 度
		float rad = ea * static_cast<float>(M_PI) / 180.0f;
		float cosA = cos(rad);
		float sinA = sin(rad);

		float rot_dx = dx * cosA - dy * sinA;
		float rot_dy = dx * sinA + dy * cosA;

		// 应用旋转偏移后的位置
		float newX = ex + rot_dx;
		float newY = ey + rot_dy;

		localDefectiveLabels.push_back({ newX, newY, static_cast<float>(boardID) });
	}

	return localDefectiveLabels;
}

// 识别板子上料误差
// 函数将识别到的偏移量应用于内部的 printCodePositions 成员变量，并返回修改后的副本。
vector<vector<float>> AutoProduction::identifyPositions(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss, ProductionMaterial& pm) {

	// =====================================================================
	// 相机和图像处理逻辑（如果需要，请取消注释）
	// =====================================================================
	// 
	// =====================================================================
	// 相机和图像处理逻辑结束
	// =====================================================================

	// 为了演示，假设 codeOffsetX0 和 codeOffsetY0 已经设置
	// （要么通过上面未注释的相机逻辑，要么通过一些默认/模拟值）。
	// 如果相机逻辑被注释，这些值可能未初始化，除非你在调用此函数前设置它们或在此处提供默认值。
	// 示例默认值（如果相机逻辑关闭）：
	 this->codeOffsetX0 = 0.1f; // 示例 X 偏移
	 this->codeOffsetY0 = -0.05f; // 示例 Y 偏移


	// 遍历 printCodePositions，将计算出的偏移量应用到每个位置
	for (size_t i = 0; i < this->printCodePositions.size(); ++i) {

		// 获取当前位置
		float currentX = this->printCodePositions[i][0];
		float currentY = this->printCodePositions[i][1];

		// 获取对应的偏移量（使用成员变量 codeOffsetX0 和 codeOffsetY0）
		float offsetX = this->codeOffsetX0;
		float offsetY = this->codeOffsetY0;

		// 根据偏移量调整位置
		float newX = currentX + offsetX;
		float newY = currentY + offsetY;

		// 更新 printCodePositions 中的坐标，保持角度、子板ID和码ID不变
		this->printCodePositions[i][0] = newX;
		this->printCodePositions[i][1] = newY;
		// 其他元素（例如角度、子板ID、码ID）保持不变
	}

	// 返回修改后的 printCodePositions 的副本
	return this->printCodePositions;
}

// 识别不良标识，返回不良子板索引
vector<int> AutoProduction::recognizeBadBabel(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss, const vector<vector<float>> defectiveLabel) {

	int xAxisCarrierIdx = 0;   // 载台X轴的索引 (例如，负责板子X方向移动的轴)
	int yAxisLocateCamIdx = 3; // 定位相机Y轴的索引 (例如，定位相机自身的Y轴)

	// *** 核心逻辑：遍历每一个不良标识点 ***
	for (const auto& label : defectiveLabel) {
		if (label.size() < 3) {
			cerr << "Warning: Defective label data incomplete (missing X, Y, or child board index). Skipping entry." << endl;
			continue; // 跳过不完整的条目
		}

		// 从当前不良标识数据中提取坐标和子板索引
		float targetBadX = label[0];          // 不良标识的X坐标
		float targetBadY = label[1];          // 不良标识的Y坐标
		int childBoardIdx = static_cast<int>(label[2]); // 对应的子板索引

		// ====== 轴运动部分 ======
		// 运动到相机位置：将载台X轴移动到当前不良标识的targetBadX
		// 同时将定位相机的Y轴移动到当前不良标识的 targetBadY
		// 调用 moveTo 函数执行运动，将两个轴同时移动到目标位置
		//moveTo(handle, axiss, xAxisCarrierIdx, targetBadX, yAxisLocateCamIdx, targetBadY);

		// ========================

		// 相机操作和不良标识检测逻辑将在这里实现。
		// 对于每一个不良标识点，轴移动到位后，会在这里执行相机捕获和图像处理。
		// 实际实现时，请根据相机检测结果决定是否将 childBoardIdx 加入 badBoardIndices。
		// 例如：
		// 1. 捕获图像：optional<cv::Mat> capturedImageOpt = imageOperate.captureImageToMat();
		// 2. 检测不良：if (capturedImageOpt && !capturedImageOpt->empty()) {
		//                int detectionResult = imageOperate.CheckdetectCircle(*capturedImageOpt);
		//                if (detectionResult == 0) { // 假设 0 表示检测到不良
		//                    isBadLabelDetected = true;
		//                }
		//            }

		bool isBadLabelDetected = false; // 伪代码：假设一个检测结果

		if (isBadLabelDetected) {
			badBabelIndex.push_back(childBoardIdx);
			cout << "[INFO] (Placeholder) Bad label detected on child board index: " << childBoardIdx << endl;
		}
		else {
			cout << "[INFO] (Placeholder) No bad label detected on child board index: " << childBoardIdx << endl;
		}
	}

	return badBabelIndex; // 返回所有识别到的不良子板索引
}

// 删除不良标识的码并返回更新后的坐标
vector<vector<float>> AutoProduction::deleteBadBabel(vector<int> badBabelIndex) {

	// Iterate through the indices of bad labels to be deleted.
	for (int i = 0; i < badBabelIndex.size(); ++i) {
		// Iterate backwards through printCodePositions to safely delete elements.
		for (int j = printCodePositions.size() - 1; j >= 0; --j) {
			// Check if the childBoardIndex (at index 3) matches the current badBabelIndex.
			if (printCodePositions[j].at(3) == static_cast<float>(badBabelIndex.at(i))) {
				// Erase the element.
				printCodePositions.erase(printCodePositions.begin() + j);
			}
		}
	}

	// Return the modified printCodePositions.
	return this->printCodePositions;
}

//喷码参数函数
//根据角度，明暗码类型，明暗码宽度，生成轴运动需要移动的距离，以及速度，并返回一个vector数组
//用于喷码角度计算
vector<vector<float>> AutoProduction::calculateMotion(float angle, ProductionMaterial& pm, int type, float v) {
	vector<vector<float>> results;
	//type为0，生成暗码需要运动的距离，和速度
	if (type == 0) {
		float x_move, y_move, vx, vy;
		vector<float> result;
		float darkWidth = pm.childBoardCodeInformation.at(0).at(1);
		//将角度转化为弧度
		float rad = DEG_TO_RAD(angle);
		x_move = darkWidth * cos(rad);
		y_move = darkWidth * sin(rad);
		vx = v * cos(rad);
		vy = v * sin(rad);
		result = { x_move,y_move,vx,vy };
		results.push_back(result);
		return  results;

	}
	//type为1，生成明码需要运动的距离，和速度
	if (type == 1) {
		float x_move, y_move, vx, vy;
		vector<float> result;
		float lightWidth = pm.childBoardCodeInformation.at(0).at(3);
		//将角度转化为弧度
		float rad = DEG_TO_RAD(angle);
		x_move = lightWidth * cos(rad);
		y_move = lightWidth * sin(rad);
		vx = v * cos(rad);
		vy = v * sin(rad);
		result = { x_move,y_move,vx,vy };
		results.push_back(result);
		return  results;
	}
	//type为2，生成两种
	if (type == 2) {
		float x1_move, y1_move, vx1, vy1;
		vector<float> result1;
		float darkWidth = pm.childBoardCodeInformation.at(0).at(1);
		//将角度转化为弧度
		float rad = DEG_TO_RAD(angle);
		x1_move = darkWidth * cos(rad);
		y1_move = darkWidth * sin(rad);
		vx1 = v * cos(rad);
		vy1 = v * sin(rad);
		result1 = { x1_move,y1_move,vx1,vy1 };
		results.push_back(result1);

		float x2_move, y2_move, vx2, vy2;
		vector<float> result2;
		float lightWidth = pm.childBoardCodeInformation.at(0).at(3);
		//将角度转化为弧度
		//float rad = DEG_TO_RAD(angle);
		x2_move = lightWidth * cos(rad);
		y2_move = lightWidth * sin(rad);
		vx2 = v * cos(rad);
		vy2 = v * sin(rad);
		result2 = { x2_move,y2_move,vx2,vy2 };
		results.push_back(result2);
		return  results;
	}

}

//喷码函数
void AutoProduction::printProduction(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss, ProductionMaterial& pm, MySqlConnect mc) {

	//以下喷码的整体思路是
	//1、让轴运动到指定坐标位置
	//2、根据传递过来的角度值以及码的类型来计算出各个轴需要运动的距离以及运动的速度，calculateMotion(angle, pm, type, printSpeed);
	//3、拿到以上数值后，设定好对应轴的速度，开启打印机，设置延迟，相关轴运动，即完成一次码的喷印

	//相关参数要存取，一些counter

	//第三步根据printCodePositions容器中坐标，移动到指定位置，进行喷码，n个码
	//这里以轴0为例,轴2为喷码头前后移动，轴8为喷码头上下，轴9为喷码头旋转
	AxisControl* axis0 = axiss.at(0);
	AxisControl* axis2 = axiss.at(2);
	AxisControl* axis8 = axiss.at(8);
	AxisControl* axis9 = axiss.at(9);
	for (int i = currentCode; i < printCodePositions.size(); i++) {
		////读取当前的位置
		//axis0->getAxisParams(handle, "dpos");
		//axis8->getAxisParams(handle, "dpos");
		//axis9->getAxisParams(handle, "dpos");
		////计算轴0需要运动的距离
		//float x_move = printCodePositions.at(i).at(0) - axis0->dPos;
		////计算喷码头前后移动需要运动的距离
		//float y_move = printCodePositions.at(i).at(1) - axis8->dPos;
		////计算喷码头旋转的角度
		//float angle_move = printCodePositions.at(i).at(2) - axis9->dPos;

		////移动以上三个轴到指定位置
		//axis0->axisMove(handle, ioControl, systemControl, axiss, 1, x_move > 0 ? 1 : -1, abs(x_move));
		//axis2->axisMove(handle, ioControl, systemControl, axiss, 1, y_move > 0 ? 1 : -1, abs(y_move));
		//axis9->axisMove(handle, ioControl, systemControl, axiss, 1, angle_move > 0 ? 1 : -1, abs(angle_move));

		////判断轴是否运动完成
		//while (1) {
		//	axis0->getAxisParams(handle, "idle");
		//	axis2->getAxisParams(handle, "idle");
		//	axis9->getAxisParams(handle, "idle");
		//	if (axis0->idle < 0 && axis2->idle < 0 && axis9->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
		//}

		//打印机参数准备
		childBoardIndex = printCodePositions.at(i).at(3);
		childCodeIndex = printCodePositions.at(i).at(4);
		string printRuleName = pm.printRuleName.at(childCodeIndex);

		//根据类型喷码
		//判断需要打印的码的类型
		//0仅打印暗码
		//1仅打印明码
		//2两者都需要打印
		//根据角度值，做相关运动
		float angle = printCodePositions.at(i).at(2);

		if (pm.childBoardCodeInformation.at(0).at(childCodeIndex) == 2) {

			vector<vector<float>> results = calculateMotion(angle, pm, 2, printSpeed);
			//先喷暗码

			////设置喷印暗码时候，喷码头的高度
			//ZAux_Direct_Single_MoveAbs(handle, 8, darkPrintHeight);
			//while (1) {
			//	axis8->getAxisParams(handle, "idle");
			//	if (axis8->idle < 0)break;
			// 检查是否有错误码，若有则立即终止自动生产
			//if (systemControl.errorCode.load() != 0) return;
			//}

			float x1_move = results.at(0).at(0);
			float y1_move = results.at(0).at(1);
			float vx1 = results.at(0).at(2);
			float vy1 = results.at(0).at(3);
			//设置运动速度
			axis0->setFloatAxisParams(handle, "speed0", abs(vx1));
			axis2->setFloatAxisParams(handle, "speed0", abs(vy1));
			//暗码打印
			printType = 0;

			//每次喷码之前都进行检测
			//if (systemControl.errorCode.load() != 0) return;
			Sleep(delay_time);
			/*axis0->axisMove(handle, ioControl, systemControl, axiss, 1, x1_move > 0 ? 1 : -1, abs(x1_move));
			axis2->axisMove(handle, ioControl, systemControl, axiss, 1, y1_move > 0 ? 1 : -1, abs(y1_move));*/

			//暗码喷印完毕，喷印明码
			//需要先将相关轴运动回去
			/*axis0->axisMove(handle, ioControl, systemControl, axiss, 1, -x1_move > 0 ? 1 : -1, abs(x1_move));
			axis2->axisMove(handle, ioControl, systemControl, axiss, 1, -y1_move > 0 ? 1 : -1, abs(y1_move));*/

			//设置喷印明码时候，喷码头的高度
			/*ZAux_Direct_Single_MoveAbs(handle, 8, lightPrintHeight);
			while (1) {
				axis8->getAxisParams(handle, "idle");
				if (axis8->idle < 0)break;
				// 检查是否有错误码，若有则立即终止自动生产
				//	if (systemControl.errorCode.load() != 0) return;
			}*/

			float x2_move = results.at(1).at(0);
			float y2_move = results.at(1).at(1);
			float vx2 = results.at(1).at(2);
			float vy2 = results.at(1).at(3);

			//设置运动速度
			axis0->setFloatAxisParams(handle, "speed0", abs(vx2));
			axis2->setFloatAxisParams(handle, "speed0", abs(vy2));
			//暗码打印
			printType = 1;

			//每次喷码之前都进行检测
			//if (systemControl.errorCode.load() != 0) return;
			Sleep(delay_time);
			/*axis0->axisMove(handle, ioControl, systemControl, axiss, 1, x2_move > 0 ? 1 : -1, abs(x2_move));
			axis2->axisMove(handle, ioControl, systemControl, axiss, 1, y2_move > 0 ? 1 : -1, abs(y2_move));*/

		}

		if (pm.childBoardCodeInformation.at(0).at(childCodeIndex) == 0 || pm.childBoardCodeInformation.at(0).at(childCodeIndex) == 1) {
			//0只喷印暗码
			//1只喷印明码
			int type = pm.childBoardCodeInformation.at(0).at(childCodeIndex);

			vector<vector<float>> results = calculateMotion(angle, pm, type, printSpeed);

			float x_move = results.at(0).at(0);
			float y_move = results.at(0).at(1);
			float vx = results.at(0).at(2);
			float vy = results.at(0).at(3);

			//设置运动速度
			axis0->setFloatAxisParams(handle, "speed0", abs(vx));
			axis2->setFloatAxisParams(handle, "speed0", abs(vy));

			//码打印
			printType = type;

			////设置喷印码时候，喷码头的高度
			//ZAux_Direct_Single_MoveAbs(handle, 2, type == 0 ? darkPrintHeight : lightPrintHeight);
			//while (1) {
			//	axis8->getAxisParams(handle, "idle");
			//	if (axis8->idle < 0)break;
			// 检查是否有错误码，若有则立即终止自动生产
			//if (systemControl.errorCode.load() != 0) return;
			//}

			//每次喷码之前都进行检测，喷码之前有错误，直接返回
			//if (systemControl.errorCode.load() != 0) return;
			Sleep(delay_time);
			/*axis0->axisMove(handle, ioControl, systemControl, axiss, 1, x_move > 0 ? 1 : -1, abs(x_move));
			axis2->axisMove(handle, ioControl, systemControl, axiss, 1, y_move > 0 ? 1 : -1, abs(y_move));*/

		}

		//记录喷到第几个码
		currentCode++;
		//存储到mysql
		string sql = "update autoproduction_counter set currentCode =" + to_string(currentCode) + "where productionName = '" + productionName + "'";
		mc.update(sql);

	}

	//程序运行到这里，表示一个基板已经打印完毕
	//对基板数累加
	baseId++;
	//存储在mysql
	string sql = "update autoproduction_counter set baseId =" + to_string(baseId) + "where productionName = '" + productionName + "'";
	mc.update(sql);
	//将当前码置为0
	currentCode = 0;

}

//喷码校验函数
void AutoProduction::checkCode(ZMC_HANDLE& handle,IOControl& ioControl,SystemControl& systemControl,
	vector<AxisControl*>& axiss,ProductionMaterial& pm,const vector<int>& badBabelIndex,MySqlConnect& mc) {

	// 存储每个码点本次检测得到的原始视觉偏移（用于记录日志）
	vector<vector<float>> checkCodeOffsets;
	const float checkXOffsetForPlatform = 100.0f; // 喷码平台到校验点的物理偏移（静态值）
	this->printCodeOffsets.clear(); // 清空上一次的补偿结果，准备新一轮

	// 定义结构体：描述历史补偿值（X、Y、角度）
	struct AppliedOffset {
		float x = 0.0f;
		float y = 0.0f;
		float angle = 0.0f;
	};

	// ✅【新增】从数据库读取所有历史补偿值
	map<pair<int, int>, AppliedOffset> lastOffsetMap; // key=(板号,码号)，value=补偿
	string sql = "SELECT board_id, code_id, x_offset, y_offset, angle_offset FROM autoproduction_offsets WHERE production_name = '" + this->productionName + "'";
	if (mc.query(sql)) {
		while (mc.next()) {
			int b = stoi(mc.value(0));
			int c = stoi(mc.value(1));
			AppliedOffset offset;
			offset.x = stof(mc.value(2));
			offset.y = stof(mc.value(3));
			offset.angle = stof(mc.value(4));
			lastOffsetMap[{b, c}] = offset; // 存入map
		}
	}

	// 遍历所有理论上的码点（codePositions）进行校验与补偿计算
	for (int i = 0; i < this->codePositions.size(); ++i) {
		const vector<float>& codeInfo = this->codePositions[i];
		if (codeInfo.size() < 5) continue; // 格式不对直接跳过

		// 提取码点信息
		float codePrintAbsX = codeInfo[0];
		float codeGlobalY = codeInfo[1];
		float codeAngle = codeInfo[2];
		int subBoardIndex = static_cast<int>(codeInfo[3]);
		int codeIndex = static_cast<int>(codeInfo[4]);

		// 判断当前子板是否是不良板
		bool isBadBoard = find(badBabelIndex.begin(), badBabelIndex.end(), subBoardIndex) != badBabelIndex.end();

		// 获取该码点上一次的补偿值
		AppliedOffset previousAppliedOffset;
		auto it = lastOffsetMap.find({ subBoardIndex, codeIndex });
		if (it != lastOffsetMap.end()) {
			previousAppliedOffset = it->second;
		}
		else {
			cout << "⚠️  子板 " << subBoardIndex << " 码 " << codeIndex << " 没有上一次补偿记录，默认使用 {0, 0, 0}" << endl;
		}

		if (isBadBoard) {
			// 不良板：保留旧补偿，不进行视觉检测
			checkCodeOffsets.push_back({ 0, 0, 0, (float)subBoardIndex, (float)codeIndex });
			this->printCodeOffsets.push_back({ previousAppliedOffset.x, previousAppliedOffset.y, previousAppliedOffset.angle, (float)subBoardIndex, (float)codeIndex });
			cout << "子板 " << subBoardIndex << " (码 " << codeIndex << ") 为不良板，已跳过检测并保留上一次的补偿值。" << endl;
		}
		else {
			// 良品板：进行检测 + 补偿计算
			float targetX = codePrintAbsX + checkXOffsetForPlatform;
			float targetY = codeGlobalY;

			// 模拟视觉检测结果（真实项目中应替换为视觉系统返回）
			float visionDetectedOffsetX = 0.1f;
			float visionDetectedOffsetY = -0.2f;
			float visionDetectedOffsetAngle = 0.5f;

			// 保存原始检测偏移
			checkCodeOffsets.push_back({ visionDetectedOffsetX, visionDetectedOffsetY, visionDetectedOffsetAngle, (float)subBoardIndex, (float)codeIndex });

			// 使用迭代公式：本次新补偿 = 旧补偿 + k * 当前视觉误差
			float actualAppliedOffsetX = this->adjustFactor * visionDetectedOffsetX + previousAppliedOffset.x;
			float actualAppliedOffsetY = this->adjustFactor * visionDetectedOffsetY + previousAppliedOffset.y;
			float actualAppliedOffsetAngle = this->adjustFactor * visionDetectedOffsetAngle + previousAppliedOffset.angle;

			// 保存用于更新的补偿值
			this->printCodeOffsets.push_back({ actualAppliedOffsetX, actualAppliedOffsetY, actualAppliedOffsetAngle, (float)subBoardIndex, (float)codeIndex });
		}
	}

	cout << "所有码点校验与补偿计算完成。" << endl;

	// ✅【新增】写入视觉偏移日志表
	if (!checkCodeOffsets.empty()) {
		stringstream ss_log;
		ss_log << "INSERT INTO production_check_logs (production_name, board_id, code_id, vision_offset_x, vision_offset_y, vision_offset_angle, is_bad_board) VALUES ";
		for (size_t i = 0; i < checkCodeOffsets.size(); ++i) {
			const auto& row = checkCodeOffsets[i];
			ss_log << "('" << this->productionName << "', " << (int)row[3] << ", " << (int)row[4] << ", " << row[0] << ", " << row[1] << ", " << row[2] << ", " << (row[0] == 0 && row[1] == 0 && row[2] == 0 ? "TRUE" : "FALSE") << ")";
			if (i < checkCodeOffsets.size() - 1) ss_log << ",";
		}
		mc.query(ss_log.str());
	}

	// ✅【新增】写入/更新补偿值表（供下轮使用）
	if (!this->printCodeOffsets.empty()) {
		stringstream ss_update;
		ss_update << "INSERT INTO autoproduction_offsets (production_name, board_id, code_id, x_offset, y_offset, angle_offset) VALUES ";
		for (size_t i = 0; i < this->printCodeOffsets.size(); ++i) {
			const auto& offset = this->printCodeOffsets[i];
			ss_update << "('" << this->productionName << "', " << (int)offset[3] << ", " << (int)offset[4] << ", " << offset[0] << ", " << offset[1] << ", " << offset[2] << ")";
			if (i < this->printCodeOffsets.size() - 1) ss_update << ",";
		}
		ss_update << " ON DUPLICATE KEY UPDATE x_offset=VALUES(x_offset), y_offset=VALUES(y_offset), angle_offset=VALUES(angle_offset)";
		mc.query(ss_update.str());
	}

	cout << "函数 checkCode 执行完毕，数据已准备就绪。" << endl;
}

// 应用数据库中的补偿值到码点位置
void AutoProduction::applyOffsetsFromDatabase(MySqlConnect& mc) {
    map<pair<int, int>, vector<float>> offsetMap;
    string sql = "SELECT board_id, code_id, x_offset, y_offset, angle_offset FROM autoproduction_offsets WHERE production_name = '" + productionName + "'";
    if (mc.query(sql)) {
        while (mc.next()) {
            int b = stoi(mc.value(0));
            int c = stoi(mc.value(1));
            float x = stof(mc.value(2));
            float y = stof(mc.value(3));
            float a = stof(mc.value(4));
            offsetMap[{b, c}] = {x, y, a};
        }
    }
    for (auto& code : codePositions) {
        int b = static_cast<int>(code[3]);
        int c = static_cast<int>(code[4]);
        auto it = offsetMap.find({b, c});
        if (it != offsetMap.end()) {
            code[0] += it->second[0]; // x
            code[1] += it->second[1]; // y
            code[2] += it->second[2]; // angle
        }
    }
    printCodePositions = codePositions;
}

//////////////////////////////////////////////////////////////////////
//指令1ok
void AutoProduction::runCommand1(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss) {
	//取出对应的轴
	AxisControl* axis5 = axiss.at(5);
	AxisControl* axis6 = axiss.at(6);
	AxisControl* axis10 = axiss.at(10);
	AxisControl* axis11 = axiss.at(11);

	//让轴10，轴11一起运动到初始高度位置
	ZAux_Direct_Single_MoveAbs(handle, 10, axis10->standByPos);//轴10初始位置
	ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);//轴11初始位置
	//判断这两个轴是否运动完成
	while (1) {
		axis10->getAxisParams(handle, "idle");
		axis11->getAxisParams(handle, "idle");
		if (axis10->idle < 0 && axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
	//if (systemControl.errorCode.load() != 0) return;
	}
	//让轴5，轴6一起运动到初始水平位置
	//ZAux_Direct_Single_MoveAbs(handle, 5, axis5->standByPos);//524.477
	//ZAux_Direct_Single_MoveAbs(handle, 6, axis6->standByPos);//524.475

	ZAux_Direct_Single_MoveAbs(handle, 5, axisParameters.at(1));//524.477,上料吸盘在平台1放置产品时的坐标，偏移1
	ZAux_Direct_Single_MoveAbs(handle, 6, axisParameters.at(7));//524.475，下料吸盘在平台1放置产品时的坐标，偏移7
	//判断这两个轴是否运动完成
	while (1) {
		axis5->getAxisParams(handle, "idle");
		axis6->getAxisParams(handle, "idle");
		if (axis5->idle < 0 && axis6->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}
//指令2ok
void AutoProduction::runCommand2(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss) {
	//取出对应的轴
	AxisControl* axis10 = axiss.at(10);
	AxisControl* axis5 = axiss.at(5);
	//让轴10运动到初始高度位置
	ZAux_Direct_Single_MoveAbs(handle, 10, axis10->standByPos);
	//判断轴10运动是否完成
	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//让轴5运动到上料盒上方9.505
	//ZAux_Direct_Single_MoveAbs(handle, 5, 9.505);
	ZAux_Direct_Single_MoveAbs(handle, 5, axisParameters.at(0));//9.505，上料吸盘在产品盒吸取产品时的坐标，偏移0

	//判断轴5运动是否完成
	while (1) {
		axis5->getAxisParams(handle, "idle");
		if (axis5->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//轴10下降到指定位置180
	ZAux_Direct_Single_MoveAbs(handle, 10, axisParameters.at(15));//固定位置未找到对应参数
	//确保轴10运动完成
	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}

void AutoProduction::runCommand3(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl,vector<AxisControl*>& axiss) {
	//取出对应的轴
	AxisControl* axis10 = axiss.at(10);
	AxisControl* axis5 = axiss.at(5);
	runCommand2(handle, systemControl,axiss);

	//打开上料吸盘真空供给
	ioControl.setOutput(handle, 234, 1, "DOA");
	//检测上料吸盘真空检测信号则吸取到
	while (1) {
		ioControl.getInput(handle, "DIA");
		if (ioControl.DIA[14] == 1)break;
	}
	//上料吸盘移动到初始高度位置
	ZAux_Direct_Single_MoveAbs(handle, 10, axis10->standByPos);	
	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

}

void AutoProduction::runCommand4(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss) {

	ioControl.getInput(handle, "DIA");
	if (ioControl.DIA[14] == 0) {
		//执行指令3
		runCommand3(handle, systemControl, ioControl, axiss);
	}
	//获取对应的轴
	AxisControl* axis5 = axiss.at(5);
	AxisControl* axis0 = axiss.at(0);
	AxisControl* axis10 = axiss.at(10);
	//载台1，上料吸盘移动到对应位置(左右移动先设置为600，后续调整)
	//ZAux_Direct_Single_MoveAbs(handle, 0, axis0->standByPos);//5
	//ZAux_Direct_Single_MoveAbs(handle, 5,axis5->standByPos);//524.477

	ZAux_Direct_Single_MoveAbs(handle, 0, axisParameters.at(3));//5,平台1在接收产品时的坐标，偏移3
	ZAux_Direct_Single_MoveAbs(handle, 5, axisParameters.at(1));//524.477，上料吸盘在平台1放置产品时的坐标，偏移1

	//判断这两个轴是否运动完成
	while (1) {
		axis5->getAxisParams(handle, "idle");
		axis0->getAxisParams(handle, "idle");
		if (axis5->idle < 0 && axis0->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//上料吸盘下降到指定位置190
	//ZAux_Direct_Single_MoveAbs(handle, 10,190);//190

	ZAux_Direct_Single_MoveAbs(handle, 10, axisParameters.at(15));//190，平台上放置物料固定高度，偏移15
	//判断轴10是否运动完成
	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//关闭真空供给
	ioControl.setOutput(handle, 234, 0, "DOA");
	//真空破坏
	ioControl.setOutput(handle, 233, 1, "DOA");
	while (1) {
		ioControl.getInput(handle, "DIA");
		if (ioControl.DIA[14] == 0)break;
	}
	//关闭真空破坏
	ioControl.setOutput(handle, 233, 0, "DOA");
	//上料吸盘回到初始高度
	ZAux_Direct_Single_MoveAbs(handle, 10, axis10->standByPos);//轴10初始位置	

	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}

void AutoProduction::runCommand5(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl,  vector<AxisControl*>& axiss) {
	if (ioControl.DIA[14] == 0) {
		//执行指令3
		runCommand3(handle, systemControl, ioControl, axiss);
	}
	//获取对应的轴
	AxisControl* axis5 = axiss.at(5);
	AxisControl* axis1 = axiss.at(1);
	AxisControl* axis10 = axiss.at(10);

	//关闭载台1_管道电磁阀通断DOB[1]为0（由1变为0），打开载台2_管道电磁阀通断DOB[3]为0（由0变为1）
	ioControl.setOutput(handle, 241, 0, "DOB");
	ioControl.setOutput(handle, 243, 1, "DOB");
	//载台2，上料吸盘移动到对应位置(左右移动先设置为863.631，后续调整)
	//ZAux_Direct_Single_MoveAbs(handle, 1, axis1->standByPos);
	//ZAux_Direct_Single_MoveAbs(handle, 5,863.631);	
	
	ZAux_Direct_Single_MoveAbs(handle, 1, axisParameters.at(4));//5.599，平台2在接收产品时的坐标，偏移4
	ZAux_Direct_Single_MoveAbs(handle, 5, axisParameters.at(2));//863.631,上料吸盘在平台2上放置产品时的坐标，偏移2

	//判断这两个轴是否运动完成
	while (1) {
		axis5->getAxisParams(handle, "idle");
		axis1->getAxisParams(handle, "idle");
		if (axis5->idle < 0 && axis1->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//上料吸盘下降到指定位置190
	//ZAux_Direct_Single_MoveAbs(handle, 10, 190);

	ZAux_Direct_Single_MoveAbs(handle, 10, axisParameters.at(15));//190，平台上放置物料固定高度，偏移15
	//判断轴10是否运动完成
	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//关闭真空供给
	ioControl.setOutput(handle, 234, 0, "DOA");
	//真空破坏
	ioControl.setOutput(handle, 233, 1, "DOA");
	while (1) {
		ioControl.getInput(handle, "DIA");
		if (ioControl.DIA[14] == 0)break;
	}
	//关闭真空破坏
	ioControl.setOutput(handle, 233, 0, "DOA");
	//上料吸盘回到初始高度
	ZAux_Direct_Single_MoveAbs(handle, 10, axis10->standByPos);
}

void AutoProduction::runCommand6(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss) {
	//执行指令3
	runCommand3(handle, systemControl, ioControl, axiss);
	//取出对应的轴(隔纸盒)
	AxisControl* axis7 = axiss.at(7);
	AxisControl* axis10 = axiss.at(10);
	//移动隔纸盒子到上方
	//ZAux_Direct_Single_MoveAbs(handle, 7,30.450);

	ZAux_Direct_Single_MoveAbs(handle, 7,axisParameters.at(12));//30.450，隔纸盒-上料区位置，偏移12
	//判断运动是否完成
	while (1) {
		axis7->getAxisParams(handle, "idle");
		if (axis7->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//上料吸盘下降57
	ZAux_Direct_Single_MoveAbs(handle, 10,57);//
	//判断运动是否完成
	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	////真空破坏
	//ioControl.setOutput(handle, 233, 1, "DOA");
	//上料吸盘回到初始高度
	ZAux_Direct_Single_MoveAbs(handle, 10, axis10->standByPos);
	//判断运动是否完成
	while (1) {
		axis10->getAxisParams(handle, "idle");
		if (axis10->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//隔纸盒回到原始位置
	//ZAux_Direct_Single_MoveAbs(handle,7, axis7->standByPos);

	ZAux_Direct_Single_MoveAbs(handle,7, axisParameters.at(13));//472.233，隔纸盒-等待位置，偏移13
	while (1) {
		axis7->getAxisParams(handle, "idle");
		if (axis7->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}

void AutoProduction::runCommand7(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss) {
	//取出对应的轴
	AxisControl* axis0 = axiss.at(0);
	AxisControl* axis6 = axiss.at(6);
	AxisControl* axis11 = axiss.at(11);

	//让轴11运动到初始高度位置
	ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);

	//判断轴11运动是否完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//让轴0运动到下料位置（后续需要调整）820.293
	ZAux_Direct_Single_MoveAbs(handle, 0, axisParameters.at(21));///未在21个坐标范围内
	//让轴6运动到下料位置（后续需要调整）
	ZAux_Direct_Single_MoveAbs(handle, 6, axisParameters.at(7));

	//判断轴0运动是否完成
	while (1) {
		axis0->getAxisParams(handle, "idle");
		if (axis0->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//判断轴6运动是否完成
	while (1) {
		axis6->getAxisParams(handle, "idle");
		if (axis6->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

	//轴11下降到指定位置
	ZAux_Direct_Single_MoveAbs(handle, 11, axisParameters.at(18));

	//确保轴11运动完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}

void AutoProduction::runCommand8(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss) {
	//取出对应的轴
	AxisControl* axis1 = axiss.at(1);
	AxisControl* axis6 = axiss.at(6);
	AxisControl* axis11 = axiss.at(11);

	//让轴11运动到初始高度位置
	ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);

	//判断轴11运动是否完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
	//让轴0运动到下料位置（后续需要调整）
	ZAux_Direct_Single_MoveAbs(handle, 1, axisParameters.at(21));
	//让轴6运动到下料位置（后续需要调整）
	ZAux_Direct_Single_MoveAbs(handle, 6, axisParameters.at(8));

	//判断轴1运动是否完成
	while (1) {
		axis1->getAxisParams(handle, "idle");
		axis6->getAxisParams(handle, "idle");
		if (axis1->idle < 0 && axis6->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

	//轴11下降到指定位置（下降的位置，先设置为190,后续调整）
	ZAux_Direct_Single_MoveAbs(handle, 11, axisParameters.at(18));

	//确保轴11运动完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}

void AutoProduction::runCommand9(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss) {
	//取出对应的轴
	AxisControl* axis0 = axiss.at(0);
	AxisControl* axis6 = axiss.at(6);
	AxisControl* axis11 = axiss.at(11);


	//让轴11运动到初始高度位置
	ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);
	//判断轴11运动是否完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

	//让轴0运动到下料位置（后续需要调整）
	ZAux_Direct_Single_MoveAbs(handle, 0, axisParameters.at(21));
	//让轴6运动到下料位置（后续需要调整）
	ZAux_Direct_Single_MoveAbs(handle, 6, axisParameters.at(7));

	//判断轴1和轴6运动是否完成
	while (1) {
		axis0->getAxisParams(handle, "idle");
		axis6->getAxisParams(handle, "idle");
		if (axis0->idle < 0 && axis6->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
		}

	//打开载台1_真空破坏阀DOB[2]由0变为1
	ioControl.setOutput(handle, 242, 1, "DOB");

	////设置载台1_管道电磁阀通断DOB[1]为0
	ioControl.setOutput(handle, 241, 0, "DOB");

	//设置载台2_管道电磁阀通断DOB[3]为1
	ioControl.setOutput(handle, 243, 1, "DOB");

	//打开下料吸盘真空供给DOA[13]由0变为1
	ioControl.setOutput(handle, 237, 1, "DOA");

	//轴11下降到指定位置（下降的位置，先设置为190,后续调整）
	ZAux_Direct_Single_MoveAbs(handle, 11, axisParameters.at(18));

	//确保轴11运动完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;	
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

	//检测下料吸盘真空检测信号则吸取到
	while (1) {
		ioControl.getInput(handle, "DIB");
		if (ioControl.DIB[2] == 1)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

	//关闭载台1_真空破坏阀DOB[2]由1变为0
	ioControl.setOutput(handle, 242, 0, "DOB");

	//下料吸盘回到初始高度
	ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);

	//判断轴11是否运动完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}

void AutoProduction::runCommand10(ZMC_HANDLE handle,SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss) {
	//取出对应的轴
	AxisControl* axis1 = axiss.at(1);
	AxisControl* axis6 = axiss.at(6);
	AxisControl* axis11 = axiss.at(11);

	//让轴11运动到初始高度位置
	ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);

	//判断轴11运动是否完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		if (systemControl.errorCode.load() != 0) return;
	}

	//让轴1运动到下料位置（后续需要调整）
	ZAux_Direct_Single_MoveAbs(handle, 1, axisParameters.at(21));//未在21个坐标范围内

	//让轴6运动到下料位置（后续需要调整）
	ZAux_Direct_Single_MoveAbs(handle, 6, axisParameters.at(8));

	//判断轴1和轴6运动是否完成
	while (1) {
		axis1->getAxisParams(handle, "idle");
		axis6->getAxisParams(handle, "idle");
		if (axis1->idle < 0 && axis6->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

	//打开载台2_真空破坏阀DOB[4]由0变为1
	ioControl.setOutput(handle, 244, 1, "DOB");

	////设置载台2_管道电磁阀通断DOB[3]为0
	ioControl.setOutput(handle, 243, 0, "DOB");

	//设置载台1_管道电磁阀通断DOB[1]为1
	ioControl.setOutput(handle, 241, 1, "DOB");

	//轴11下降到指定位置（下降的位置，先设置为190,后续调整）
	ZAux_Direct_Single_MoveAbs(handle, 11, axisParameters.at(18));

	//确保轴11运动完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}

	//打开下料吸盘真空供给DOA[13]由0变为1
	ioControl.setOutput(handle, 237, 1, "DOA");

	//检测下料吸盘真空检测信号则吸取到
	/*while (1) {
		ioControl.getInput(handle, "DIB");
		if (ioControl.DIB[2] == 1)break;
	}*/

	//关闭载台2_真空破坏阀DOB[4]由1变为0
	ioControl.setOutput(handle, 244, 0, "DOB");

	//下料吸盘回到初始高度
	ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);
	//判断轴11是否运动完成
	while (1) {
		axis11->getAxisParams(handle, "idle");
		if (axis11->idle < 0)break;
		// 检查是否有错误码，若有则立即终止自动生产
		//if (systemControl.errorCode.load() != 0) return;
	}
}

void AutoProduction::runCommand11(ZMC_HANDLE handle, SystemControl& systemControl, IOControl & ioControl, vector<AxisControl*>&axiss){

		AxisControl* axis6 = axiss.at(6);
		AxisControl* axis10 = axiss.at(10);
		AxisControl* axis11 = axiss.at(11);

		//让轴6运动到OK盒位置（后续需要调整）
		ZAux_Direct_Single_MoveAbs(handle, 6, axisParameters.at(9));

		//判断轴6运动是否完成
		while (1) {
			axis6->getAxisParams(handle, "idle");
			if (axis6->idle < 0)break;
			// 检查是否有错误码，若有则立即终止自动生产
			//if (systemControl.errorCode.load() != 0) return;
		}

		// 轴11下降到指定位置（下降的位置，先设置为20, 后续调整）
		ZAux_Direct_Single_MoveAbs(handle, 11, axisParameters.at(19));

		//确保轴11运动完成
		while (1) {
			axis11->getAxisParams(handle, "idle");
			if (axis11->idle < 0)break;
			// 检查是否有错误码，若有则立即终止自动生产
			//if (systemControl.errorCode.load() != 0) return;
			}

		//关闭下料吸盘真空供给DOA[13]
		ioControl.setOutput(handle, 237, 0, "DOA");
		//打开下料吸盘真空破坏DOA[12]
		ioControl.setOutput(handle, 236, 1, "DOA");
		////下料吸盘真空信号检测
		//while (1) {
		//	ioControl.getInput(handle, "DIB");
		//	if (ioControl.DIB[2] == 0)break;
		//}
		// 
		//关闭下料吸盘真空破坏DOA[12]
		ioControl.setOutput(handle, 236, 0, "DOA");

		//下料吸盘回到初始高度
		ZAux_Direct_Single_MoveAbs(handle, 11, axis11->standByPos);

		while (1) {
			axis10->getAxisParams(handle, "idle");
			if (axis10->idle < 0)break;
			// 检查是否有错误码，若有则立即终止自动生产
			//if (systemControl.errorCode.load() != 0) return;
		}

	}


