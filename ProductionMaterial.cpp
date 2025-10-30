#include "ProductionMaterial.h"

// =========================================
// 构造函数：初始化基本成员
// =========================================
ProductionMaterial::ProductionMaterial() {
    // 初始化标量成员
    badTarget_X = 0.0f;
    badTarget_Y = 0.0f;
    childBoardIndex = 0;
    childCodeIndex = 0;
    prinSpeed = 100.0f;
    printType = 0;
    visCoedWidth = visCoedLength = 0.0f;
    dimCodeWidth = dimCoedLength = 0.0f;
    visCoedIndex = dimCodeIndex = 0;
    visCoedNum = dimCodeNum = 0;
    totalBoards = 0;
    totalProductionCount = 0;

    // 清空容器成员
    clearAllData();
}

// =========================================
// 清空所有容器数据
// 在重新加载 ProductionMaterial 时调用
// =========================================
void ProductionMaterial::clearAllData() {
    childBoardprintCodePositions.clear();
    firstExtendedprintCodePositions.clear();
    encodeRuleName.clear();
    encodeRulePrintNum.clear();
    darkCodeType.clear();
    lightCodeType.clear();
    printRuleName.clear();
}

// =========================================
// 打印调试信息（开发调试时使用）
// =========================================
void ProductionMaterial::printDebugInfo() const {
    std::cout << "\n=== ProductionMaterial Debug Info ===" << std::endl;
    std::cout << "总子板数: " << totalBoards << std::endl;
    std::cout << "当前生产计数: " << totalProductionCount << std::endl;
    std::cout << "暗码数量: " << dimCodeNum << " | 明码数量: " << visCoedNum << std::endl;
    std::cout << "不良标识: (" << badTarget_X << ", " << badTarget_Y << ")" << std::endl;
    std::cout << "子板坐标数量: " << childBoardprintCodePositions.size() << std::endl;
    std::cout << "扩展点数量: " << firstExtendedprintCodePositions.size() << std::endl;
    std::cout << "编码规则数量: " << encodeRuleName.size() << std::endl;
    std::cout << "=====================================" << std::endl;
}
