#include "ProductionMaterialManager.h"

// =========================================
// 构造函数
// 保存数据库连接参数（仅保存，不连接）
// =========================================
ProductionMaterialManager::ProductionMaterialManager(const std::string& host,
    const std::string& user,
    const std::string& password,
    const std::string& database,
    unsigned short port)
    : m_host(host), m_user(user), m_password(password), m_database(database), m_port(port) {
}

// =========================================
// 创建数据库连接（封装 MySQLConnect）
// 每次查询都临时创建连接，防止长期占用连接
// =========================================
std::unique_ptr<MySqlConnect> ProductionMaterialManager::createConnection() {
    auto conn = std::make_unique<MySqlConnect>();
    if (!conn->connect(m_user, m_password, m_database, m_host, m_port)) {
        std::cerr << "数据库连接失败: " << conn->getError() << std::endl;
        return nullptr;
    }
    return conn;
}

// 安全类型转换函数，防止 stoi/stof 抛异常导致崩溃
float ProductionMaterialManager::parseFloat(const std::string& str, float def) {
    try { return std::stof(str); }
    catch (...) { return def; }
}
int ProductionMaterialManager::parseInt(const std::string& str, int def) {
    try { return std::stoi(str); }
    catch (...) { return def; }
}

// =========================================
// 从数据库读取 子板码数据
// 表：subboard_and_boardedge_code_table
// =========================================
std::vector<ChildBoardInfo> ProductionMaterialManager::getChildBoardCodes(const std::string& modelName, bool locateMode) {
    std::vector<ChildBoardInfo> results;
    auto conn = createConnection();
    if (!conn) return results;

    std::stringstream sql;
    sql << "SELECT soonid, cipherX, cipherY, plainCodeX, plainCodeY, "
        << "cipherWide, cipherHigh, plainCodeWide, plainCodeHigh, "
        << "printContent, printSource, cipherClause, plainCodeClause, angle "
        << "FROM subboard_and_boardedge_code_table "
        << "WHERE modelName = '" << modelName << "' "
        << "AND locateMode = " << (locateMode ? "1" : "0");

    if (conn->query(sql.str())) {
        while (conn->next()) {
            ChildBoardInfo info;
            // 从结果中逐列提取数据
            info.soonId = parseInt(conn->value(0));
            info.cipherX = parseFloat(conn->value(1));
            info.cipherY = parseFloat(conn->value(2));
            info.plainCodeX = parseFloat(conn->value(3));
            info.plainCodeY = parseFloat(conn->value(4));
            info.cipherWide = parseFloat(conn->value(5));
            info.cipherHigh = parseFloat(conn->value(6));
            info.plainCodeWide = parseFloat(conn->value(7));
            info.plainCodeHigh = parseFloat(conn->value(8));
            info.printContent = conn->value(9);
            info.printSource = conn->value(10);
            info.cipherClause = parseInt(conn->value(11));
            info.plainCodeClause = parseInt(conn->value(12));
            info.angle = parseFloat(conn->value(13));
            info.locateMode = locateMode;
            results.push_back(info);
        }
    }
    return results;
}

// =========================================
// 从数据库读取 扩展点数据
// 表：subboard_expansion_point_tab
// =========================================
std::vector<ExpansionPointInfo> ProductionMaterialManager::getExpansionPoints(const std::string& modelName, bool locateMode) {
    std::vector<ExpansionPointInfo> results;
    auto conn = createConnection();
    if (!conn) return results;

    std::stringstream sql;
    sql << "SELECT X, Y, angle, expandid FROM subboard_expansion_point_tab "
        << "WHERE modelName = '" << modelName << "' "
        << "AND locateMode = " << (locateMode ? "1" : "0");

    if (conn->query(sql.str())) {
        while (conn->next()) {
            ExpansionPointInfo info;
            info.x = parseFloat(conn->value(0));
            info.y = parseFloat(conn->value(1));
            info.angle = parseFloat(conn->value(2));
            info.expandId = parseInt(conn->value(3));
            info.locateMode = locateMode;
            results.push_back(info);
        }
    }
    return results;
}

// =========================================
// 从数据库读取 编码规则
// 表：encode_rule
// =========================================
std::vector<EncodeRuleInfo> ProductionMaterialManager::getEncodeRules(const std::string& prodName) {
    std::vector<EncodeRuleInfo> results;
    auto conn = createConnection();
    if (!conn) return results;

    std::stringstream sql;
    sql << "SELECT mode, position, encodeType, length FROM encode_rule "
        << "WHERE prodName = '" << prodName << "' ORDER BY position";

    if (conn->query(sql.str())) {
        while (conn->next()) {
            EncodeRuleInfo info;
            info.mode = parseInt(conn->value(0));
            info.position = parseInt(conn->value(1));
            info.encodeType = parseInt(conn->value(2));
            info.length = parseInt(conn->value(3));
            info.ruleName = "Rule_" + std::to_string(info.position);
            results.push_back(info);
        }
    }
    return results;
}

// =========================================
// 从数据库读取 不良标识点坐标
// 表：defective_identification_tab
// =========================================
std::pair<float, float> ProductionMaterialManager::getDefectiveCoordinates(const std::string& modelName, bool locateMode) {
    auto conn = createConnection();
    if (!conn) return { 0, 0 };

    std::stringstream sql;
    sql << "SELECT X, Y FROM defective_identification_tab "
        << "WHERE modelName='" << modelName << "' "
        << "AND locateMode=" << (locateMode ? "1" : "0") << " LIMIT 1";

    if (conn->query(sql.str()) && conn->next())
        return { parseFloat(conn->value(0)), parseFloat(conn->value(1)) };
    return { 0, 0 };
}

// =========================================
// 读取生产计数器
// 表：autoproduction_counter
// =========================================
ProductionCounterInfo ProductionMaterialManager::getProductionCounter(const std::string& modelName) {
    ProductionCounterInfo info{ 0,0 };
    auto conn = createConnection();
    if (!conn) return info;

    std::stringstream sql;
    sql << "SELECT totalBoards, currentCode FROM autoproduction_counter "
        << "WHERE productionName='" << modelName << "' LIMIT 1";

    if (conn->query(sql.str()) && conn->next()) {
        info.totalBoards = parseInt(conn->value(0));
        info.currentCode = parseInt(conn->value(1));
    }
    return info;
}

// =========================================
// 更新生产计数器（写数据库）
// =========================================
bool ProductionMaterialManager::updateProductionCounter(const std::string& modelName, int currentCode) {
    auto conn = createConnection();
    if (!conn) return false;

    std::stringstream sql;
    sql << "UPDATE autoproduction_counter SET currentCode=" << currentCode
        << " WHERE productionName='" << modelName << "'";
    return conn->update(sql.str());
}

// =========================================
// 测试数据库连接是否可用
// =========================================
bool ProductionMaterialManager::testConnection() {
    auto conn = createConnection();
    return conn != nullptr;
}

// =========================================
// 获取所有生产名称（用于下拉选择等）
// =========================================
std::vector<std::string> ProductionMaterialManager::getAllProductionNames() {
    std::vector<std::string> names;
    auto conn = createConnection();
    if (!conn) return names;
    if (conn->query("SELECT DISTINCT productionName FROM autoproduction_counter"))
        while (conn->next()) names.push_back(conn->value(0));
    return names;
}

// =========================================
// 创建完整 ProductionMaterial 对象
// 包括所有子板、扩展点、编码规则、计数器
// =========================================
std::unique_ptr<ProductionMaterial> ProductionMaterialManager::createProductionMaterial(
    const std::string& modelName, const std::string& prodName, bool locateMode)
{
    auto material = std::make_unique<ProductionMaterial>();
    if (loadProductionMaterialFromDatabase(*material, modelName, prodName, locateMode))
        return material;
    return nullptr;
}

// =========================================
// 核心函数：从数据库加载所有信息到 ProductionMaterial
// =========================================
bool ProductionMaterialManager::loadProductionMaterialFromDatabase(
    ProductionMaterial& pm, const std::string& modelName, const std::string& prodName, bool locateMode)
{
    pm.clearAllData(); // 先清空旧数据

    // 分别读取各类信息
    auto childBoards = getChildBoardCodes(modelName, locateMode);
    auto expansions = getExpansionPoints(modelName, locateMode);
    auto rules = getEncodeRules(prodName);
    auto defect = getDefectiveCoordinates(modelName, locateMode);
    auto counter = getProductionCounter(modelName);

    // 填充到 pm 对象中
    pm.badTarget_X = defect.first;
    pm.badTarget_Y = defect.second;
    pm.totalBoards = counter.totalBoards;
    pm.totalProductionCount = counter.currentCode;

    populateChildBoardPositions(&pm, childBoards);
    populateExpansionPoints(&pm, expansions);
    populateEncodeRules(&pm, rules);
    calculateCodeStatistics(&pm, childBoards);
    setDefaultPrintParameters(&pm);

    return true;
}

// =========================================
// 以下是数据封装辅助函数
// 把从数据库读取的结构体转成 ProductionMaterial 成员数据
// =========================================

// 1️⃣ 填充子板坐标
void ProductionMaterialManager::populateChildBoardPositions(ProductionMaterial* material,
    const std::vector<ChildBoardInfo>& boards)
{
    material->childBoardprintCodePositions.clear();
    for (size_t i = 0; i < boards.size(); ++i) {
        const auto& b = boards[i];
        material->childBoardprintCodePositions.push_back({
            b.cipherX, b.cipherY, b.angle,
            static_cast<float>(b.soonId - 1),
            static_cast<float>(i)
            });
    }
}

// 2️⃣ 填充扩展点
void ProductionMaterialManager::populateExpansionPoints(ProductionMaterial* material,
    const std::vector<ExpansionPointInfo>& pts)
{
    material->firstExtendedprintCodePositions.clear();
    for (size_t i = 0; i < pts.size(); ++i) {
        const auto& p = pts[i];
        material->firstExtendedprintCodePositions.push_back({
            p.x, p.y, p.angle,
            static_cast<float>(i),
            static_cast<float>(p.expandId - 1)
            });
    }
}

// 3️⃣ 填充编码规则
void ProductionMaterialManager::populateEncodeRules(ProductionMaterial* material,
    const std::vector<EncodeRuleInfo>& rules)
{
    material->encodeRuleName.clear();
    material->encodeRulePrintNum.clear();
    material->darkCodeType.clear();
    material->lightCodeType.clear();

    for (const auto& r : rules) {
        material->encodeRuleName.push_back(r.ruleName);
        material->encodeRulePrintNum.push_back(r.length);

        // 区分明码与暗码
        if (r.encodeType == 0) material->darkCodeType.push_back(r.position);
        else if (r.encodeType == 1) material->lightCodeType.push_back(r.position);

        material->printRuleName.push_back(r.ruleName);
    }
}

// 4️⃣ 统计明码/暗码数量
void ProductionMaterialManager::calculateCodeStatistics(ProductionMaterial* material,
    const std::vector<ChildBoardInfo>& boards)
{
    material->visCoedNum = 0;
    material->dimCodeNum = 0;

    for (const auto& b : boards) {
        if (b.printContent.find("暗码") != std::string::npos) material->dimCodeNum++;
        else if (b.printContent.find("明码") != std::string::npos) material->visCoedNum++;
    }

    // 默认取第一块子板的码尺寸
    if (!boards.empty()) {
        const auto& f = boards[0];
        material->dimCodeWidth = f.cipherWide;
        material->dimCoedLength = f.cipherHigh;
        material->visCoedWidth = f.plainCodeWide;
        material->visCoedLength = f.plainCodeHigh;
    }
}

// 5️⃣ 设置默认打印参数
void ProductionMaterialManager::setDefaultPrintParameters(ProductionMaterial* material) {
    material->prinSpeed = 100.0f;   // 默认打印速度
    material->printType = 0;        // 默认暗码模式
    material->visCoedIndex = 0;
    material->dimCodeIndex = 0;
    material->childBoardIndex = 0;
    material->childCodeIndex = 0;
}
