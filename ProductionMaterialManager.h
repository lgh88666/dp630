#pragma once
#pragma once
#include "MySQLConnect.h"
#include "ProductionMaterial.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>

//
// ===============================
// ?? 数据结构定义区
// 这些结构对应数据库里的各个表字段
// ===============================
//

// 子板信息（来自 subboard_and_boardedge_code_table）
struct ChildBoardInfo {
    int boardId;        // 子板编号
    int soonId;         // 顺序号（逻辑顺序）
    float cipherX, cipherY;     // 暗码坐标
    float plainCodeX, plainCodeY; // 明码坐标
    float cipherWide, cipherHigh; // 暗码宽高
    float plainCodeWide, plainCodeHigh; // 明码宽高
    std::string printContent;   // 打印内容（用于判断明码/暗码）
    std::string printSource;    // 打印来源（文件/自动生成等）
    int cipherClause;           // 暗码段落号
    int plainCodeClause;        // 明码段落号
    float angle;                // 旋转角度
    bool locateMode;            // A面(0)/B面(1)
};

// 扩展点信息（来自 subboard_expansion_point_tab）
struct ExpansionPointInfo {
    float x, y;     // 坐标
    float angle;    // 旋转角度
    int expandId;   // 扩展点编号
    bool locateMode;// A/B面
};

// 编码规则（来自 encode_rule）
struct EncodeRuleInfo {
    std::string ruleName; // 规则名
    int encodeType;       // 0=暗码，1=明码
    int length;           // 长度
    int position;         // 位置索引
    int mode;             // 编码模式
};

// 生产计数器（来自 autoproduction_counter）
struct ProductionCounterInfo {
    int totalBoards;  // 总子板数
    int currentCode;  // 当前码号
};

//
// ===============================
// ?? ProductionMaterialManager
// 将 DAO（数据库访问） + Service（业务逻辑）合并成一个类
// ===============================
//

class ProductionMaterialManager {
private:
    // MySQL 连接信息
    std::string m_host, m_user, m_password, m_database;
    unsigned short m_port;

public:
    // 构造函数：保存数据库连接参数
    ProductionMaterialManager(const std::string& host,
        const std::string& user,
        const std::string& password,
        const std::string& database,
        unsigned short port = 3306);

    // ======= 数据访问层函数 =======
    // 这些函数直接访问数据库表，执行 SQL 查询
    std::vector<ChildBoardInfo> getChildBoardCodes(const std::string& modelName, bool locateMode);
    std::vector<ExpansionPointInfo> getExpansionPoints(const std::string& modelName, bool locateMode);
    std::vector<EncodeRuleInfo> getEncodeRules(const std::string& prodName);
    std::pair<float, float> getDefectiveCoordinates(const std::string& modelName, bool locateMode);
    ProductionCounterInfo getProductionCounter(const std::string& modelName);
    bool updateProductionCounter(const std::string& modelName, int currentCode);
    std::vector<std::string> getAllProductionNames();
    bool testConnection();

    // ======= 业务逻辑层函数 =======
    // 这些函数组合多张表数据，生成完整的 ProductionMaterial
    std::unique_ptr<ProductionMaterial> createProductionMaterial(
        const std::string& modelName,
        const std::string& prodName,
        bool locateMode = false);

    bool loadProductionMaterialFromDatabase(
        ProductionMaterial& pm,
        const std::string& modelName,
        const std::string& prodName,
        bool locateMode = false);

private:
    // ======= 内部工具函数 =======
    // 用于简化数据库操作、数据转换和数据填充
    std::unique_ptr<MySqlConnect> createConnection();  // 创建数据库连接
    float parseFloat(const std::string& str, float defaultValue = 0.0f); // 安全转float
    int parseInt(const std::string& str, int defaultValue = 0);          // 安全转int

    // 以下函数是“数据填充逻辑”：把查询结果放进 ProductionMaterial 对象中
    void populateChildBoardPositions(ProductionMaterial* material,
        const std::vector<ChildBoardInfo>& childBoards);
    void populateExpansionPoints(ProductionMaterial* material,
        const std::vector<ExpansionPointInfo>& expansionPoints);
    void populateEncodeRules(ProductionMaterial* material,
        const std::vector<EncodeRuleInfo>& encodeRules);
    void calculateCodeStatistics(ProductionMaterial* material,
        const std::vector<ChildBoardInfo>& childBoards);
    void setDefaultPrintParameters(ProductionMaterial* material);
};
