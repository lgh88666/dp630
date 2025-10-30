#pragma once

// 包含 ZMotion 库和系统相关头文件
#include "zauxdll2.h"
#include "zmotion.h"
#include "io.h"         
#include "system.h"     
#include "axis.h"       
#include "ProductionMaterial.h" 
#include "MySQLConnect.h"

#include <vector>       
#include <string>       
#include <Windows.h>    
#include <iostream>     // For debugging output (如果需要)


// 使用 std 命名空间，方便书写
using namespace std;

// 前向声明，避免循环依赖，因为这些类可能在彼此的头文件中相互引用
class AxisControl;
class ProductionMaterial;
class IOControl;
class SystemControl;

class AutoProduction {
public:
    // --- 实时坐标信息 ---
    float currentX0;        // 当前X轴坐标 (载台0)
    float currentX1;        // 当前X轴坐标 (载台1)
    float currentY1;        // 当前YP轴坐标(喷头)
    float currentY2;        // 当前YM轴坐标(定位相机)
    float currentY3;        // 当前YC轴坐标(读码)
    float Current_angle;    // 当前喷头角度

    // --- 校准和偏移量 ---
    const float adjustFactor = 0.5; // 校准系数，初始设置为0.5
    float codeOffsetX0;     // 上料白框识别后，每个码的横向偏移量
    float codeOffsetY0;     // 上料白框识别后，每个码的纵向偏移量
 

    // --- 生产批次和计数信息 ---
    int childBoardIndex = 0; // 当前处理的子板索引
    int childCodeIndex = 0;  // 当前处理的码索引 (在当前子板中的码索引)
    int childCodeNum;        // 每个子板需要喷印的二维码个数 (单块子板上的码总数)
    int codeNum;             // 喷码总数：该生产资料一块基板上需要喷码的总数（所有子板上的码之和）


    //一些计数counter,每次生产之后自动化生产过程中断后，需要重新读取这些数据
    string productionName;//当前喷印的基板资料名称，绑定数据
    int baseId;     //当前喷印基板id
    
    int currentCode;//当前喷印到第几个码


    // --- 运动和生产流程状态 ---
    int totalStepNum;     // 完成生产总共需要的步骤总数
    int currentStepCount; // 当前正在进行的步骤计数

    // --- 码位及不良信息 ---
    vector<vector<float>> printCodePositions;         // 喷码坐标集合, 维度: [x, y, angle, 子板索引, 码索引]
    vector<vector<float>> codePositions;              // 用于存储所有原始的、未被删不良的码位置。维度: [x, y, angle, 子板索引, 码索引]
    vector<vector<float>> dimprintCodePositions;      // 暗码坐标集合
    vector<vector<float>> printCodeOffsets;      //喷码偏移量集合，维度: (ΔX', ΔY',Δangle'，板索引, 码索引)
    vector<vector<float>> defectiveLabel;        // 不良标识的坐标信息，维度: (ΔX, ΔY, 板索引)
    vector<vector<int>> currentCodeCount;        // 当前正在喷涂的二维码 [板索引, 码索引]
    vector<int> badBabelIndex; // 用于存储识别到的不良子板索引
 
    //新增
    int printType;//打印类型
    int childBoardNum = 0;//子板数

    int okCurrentProductionCount = 0;//本次生产过程中ok基板数量（喷码位置校验后，根据误差结果加1）

    int delay_time;//喷印的延迟时间，单位ms
    float printSpeed;//喷印的速度

    float darkPrintHeight;//暗码喷印高度
    float lightPrintHeight;//明码喷印高度

    // --- 枚举定义，用于管理生产状态和模式 ---
    enum class StepState {
        INIT_STATE,    // 初始化状态
        WAITING_STATE, // 等待操作或事件
        BUSY_STATE,    // 正在执行任务
        DONE_STATE,    // 任务完成
        ERROR_STATE    // 出现错误
    };

    enum class ProductionMode {
        LOOP_CONTINUE, // 循环连续生产
        LOOP_STOP,     // 循环停止（完成当前基板后停止）
        STEP_STOP      // 步进停止（完成当前步骤后停止）
    };

    StepState stepState;            // 当前生产步骤状态
    ProductionMode productionMode;  // 当前生产模式
    vector<float> axisParameters;   // 轴参数，这可能是一个通用的参数集合，或来自数据库的配置
    vector<vector<float>> boardPositionAdjust; // 板子位置调整量，可能由视觉识别得出
    vector<vector<float>> printCodePositionsPrint;  // 实际用于喷印的码坐标（可能经过校准或偏移量调整）


public:
    //自动化流程
    void autoProductionRun(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl,
        vector<AxisControl*>& axiss, ProductionMaterial& pm);

    // 移动指定的X轴和Y轴到目标位置并等待完成
    void moveTo(ZMC_HANDLE& handle, vector<AxisControl*>& axiss, int xAxisIdx, float targetX, int yAxisIdx, float targetY);

    // 坐标获取
    void getCurrentCoordinate(ZMC_HANDLE& handle, vector<AxisControl*>& axiss);

    // 移动操作
    void clickMove(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss);

    //识别板子上料误差
    vector<vector<float>>identifyPositions(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, std::vector<AxisControl*>& axiss, ProductionMaterial& pm);

    // 生成后续子板扩展码
    std::vector<std::vector<float>>generateChildBoardCodes(ProductionMaterial& pm);

    //生成不良标识的位置信息
    vector<vector<float>> generateDefectiveLabel(ProductionMaterial& pm);

    //识别不良标识，返回不良子板索引
    vector<int>recognizeBadBabel(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss, const vector<vector<float>> defectiveLabel);

    //删掉不良标识的码
    vector<vector<float>> deleteBadBabel(vector<int> badBabelIndex);

    //喷码函数
    void printProduction(ZMC_HANDLE& handle, IOControl& ioControl, SystemControl& systemControl, vector<AxisControl*>& axiss, ProductionMaterial& pm, MySqlConnect mc);

    //检查喷码
    void checkCode(
        ZMC_HANDLE& handle,
        IOControl& ioControl,
        SystemControl& systemControl,
        std::vector<AxisControl*>& axiss,
        ProductionMaterial& pm,
        const std::vector<int>& badBabelIndex,
        MySqlConnect& mc // ✅ 新增这个参数
    );
    //调整喷码坐标函数
    void adjustprintCodePositions(ProductionMaterial& pm);

    //根据角度，明暗码类型，明暗码宽度，生成轴运动需要移动的距离，以及速度，并返回一个vector数组
    vector<vector<float>> calculateMotion(float angle, ProductionMaterial& pm, int type, float v);


    //应用上一次偏移量到码点坐标
    void applyOffsetsFromDatabase(MySqlConnect& mc);


    /////////////////////////////////////////////以下为运动指令///////////////////////////////////////
    //指令1:全部吸盘 - 上升到等待位置
    void runCommand1(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss);

    //指令2:上料吸盘 - 下降到上料盒的底部吸着面
    void runCommand2(ZMC_HANDLE handle, SystemControl& systemControl,vector<AxisControl*>& axiss);

    //指令3:上料吸盘 - 从产品盒吸取【PCB】
    void runCommand3(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss);

    //指令4:上料吸盘 - 从产品盒取【PCB】放至平台1
    void runCommand4(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss);

    //指令5:上料吸盘 - 从产品盒取【PCB】放至平台2
    void runCommand5(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss);

    //指令6:上料吸盘 - 从产品盒取【隔纸】放至纸盒
    void runCommand6(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss);

    //指令7:下料吸盘 - 下降到平台2的吸取面
    void runCommand7(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss);

    //指令8:下料吸盘 - 下降到平台2的吸取面 (请确认是否与指令7重复)
    void runCommand8(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss);

    //指令9:下料吸盘 - 从平台1上吸取【PCB】
    void runCommand9(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss);

    //指令10:下料吸盘 - 从平台2上吸取【PCB】
    void runCommand10(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss);

    //指令11:下料吸盘 - 将【PCB】放至OK盒
    void runCommand11(ZMC_HANDLE handle, SystemControl& systemControl, IOControl& ioControl, vector<AxisControl*>& axiss);

    //指令12:下料吸盘 - 将【PCB】放至NG盒
    void runCommand12(ZMC_HANDLE handle, SystemControl& systemControl, vector<AxisControl*>& axiss);
};