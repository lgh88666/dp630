#pragma once
#include <vector>
#include <string>
#include <iostream>

// ===============================
// 📦 ProductionMaterial 类
// 用于存储数据库读取后的生产资料数据
// ===============================
class ProductionMaterial {
public:
    // ===== 不良标识坐标 =====
    float badTarget_X = 0.0f;
    float badTarget_Y = 0.0f;

    // ===== 子板与喷码索引 =====
    int childBoardIndex = 0; // 当前子板索引
    int childCodeIndex = 0;  // 当前喷码索引

    // ===== 喷码参数 =====
    float prinSpeed = 100.0f; // 喷码速度
    int printType = 0;        // 喷码类型：0=暗码，1=明码

    // ===== 明/暗码尺寸信息 =====
    float visCoedWidth = 0.0f;   // 明码宽度
    float visCoedLength = 0.0f;  // 明码长度
    float dimCodeWidth = 0.0f;   // 暗码宽度
    float dimCoedLength = 0.0f;  // 暗码长度

    // ===== 明码暗码计数信息 =====
    int visCoedIndex = 0;  // 明码索引
    int dimCodeIndex = 0;  // 暗码索引
    int visCoedNum = 0;    // 明码数量
    int dimCodeNum = 0;    // 暗码数量

    // ===== 生产计数信息 =====
    int totalBoards = 0;           // 总子板数
    int totalProductionCount = 0;  // 当前生产总计数（会写回数据库）

    // ===== 从数据库读取的结构数据 =====
    std::vector<std::vector<float>> childBoardprintCodePositions;   // 子板坐标 [x, y, angle, boardIndex, codeIndex]
    std::vector<std::vector<float>> firstExtendedprintCodePositions; // 扩展点坐标 [x, y, angle, expandIndex, codeIndex]

    // 编码规则（由 encode_rule 表提供）
    std::vector<std::string> encodeRuleName;    // 编码规则名称
    std::vector<int> encodeRulePrintNum;        // 每个规则对应码数
    std::vector<int> darkCodeType;              // 暗码条款号
    std::vector<int> lightCodeType;             // 明码条款号
    std::vector<std::string> printRuleName;     // 当前打印规则名

    //子板码其他信息
    // 仅需要存储第一块子板上的信息，扩展板与其信息一致
    //[printType，darkWidth,darkHeight,lightWidth,lightHeight]
    //[喷印类型,暗码宽度,暗码高度,明码宽度,明码高度]
    std::vector<std::vector<float>> childBoardCodeInformation; // 修复：添加 std:: 限定符

    // ===== 构造与工具方法 =====
    ProductionMaterial();
    void clearAllData();        // 清空所有数据
    void printDebugInfo() const;// 打印调试信息
};

//#pragma once
//#include"zauxdll2.h"
//#include"zmotion.h"
//#include"io.h"
//#include"system.h"
//#include"axis.h"
//#include<vector>
//#include <winsock.h>
//#include <iostream>
//using namespace std;
//
//
//class ProductionMaterial {
//
//public:
//
//    float badTarget_X;   //不良标识X坐标
//    float badTarget_Y;   //不良标识Y坐标
//
//    int childBoardIndex = 0; //子板索引
//    int childCodeIndex = 0;  //码索引
//
//    float prinSpeed;//喷码时载台移动速度
//    int printType;   //码类型 0为暗码，1为明码
//
//    float visCoedWidth;  //明码宽度
//    float visCoedLength; //明码长度
//    float dimCodeWidth;  //暗码宽度
//    float dimCoedLength; //暗码长度
//
//    int visCoedIndex;        //明码索引
//    int dimCodeIndex;        //暗码索引
//    int visCoedNum;          //子板码上的明码个数,由明码索引推出来
//    int dimCodeNum;          //子板码上的暗码个数,由暗码索引推出来
//
//    int totalBoards;//此资料需要喷印的总基板数
//
//
//    //计数部分
//    int totalProductionCount;//该生产资料总共生产的计数，每次初始值应从数据库读取，结束生产后需写入数据库
//
//    // 输入1: 子板码坐标
//    // 维度: 5D -> [x, y, angle, childBoardIndex,codeIndex(应为0)]
//    vector<vector<float>> childBoardprintCodePositions;
//
//    // 输入2: 第一个子板的扩展参考点坐标
//    // 维度: 5D -> [x, y, angle,childBoardIndex(应为0),codeIndex]
//    vector<vector<float>> firstExtendedprintCodePositions;
//
//    //新增
//    vector<string> encodeRuleName;//规则名称(喷印来源)
//    vector<int> encodeRulePrintNum;//每个规则对应码数。索引号与规则名索引号对应
//    vector<int> darkCodeType;//暗码条款号
//    vector<int> lightCodeType;//明码条款号
//
//
//    vector<string> printRuleName;//当前喷印的规则内容，第一块子板上的规则名称，子板上是一样的
//
//    //子板码其他信息
//    // 仅需要存储第一块子板上的信息，扩展板与其信息一致
//    //[printType，darkWidth,darkHeight,lightWidth,lightHeight]
//    //[喷印类型,暗码宽度,暗码高度,明码宽度,明码高度]
//    vector<vector<float>> childBoardCodeInformation;
//
//
//    vector<float> axisParameters;   // 轴参数，这可能是一个通用的参数集合，或来自数据库的配置
//
//
//    ProductionMaterial();
//
//};
