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
// ?? ���ݽṹ������
// ��Щ�ṹ��Ӧ���ݿ���ĸ������ֶ�
// ===============================
//

// �Ӱ���Ϣ������ subboard_and_boardedge_code_table��
struct ChildBoardInfo {
    int boardId;        // �Ӱ���
    int soonId;         // ˳��ţ��߼�˳��
    float cipherX, cipherY;     // ��������
    float plainCodeX, plainCodeY; // ��������
    float cipherWide, cipherHigh; // ������
    float plainCodeWide, plainCodeHigh; // ������
    std::string printContent;   // ��ӡ���ݣ������ж�����/���룩
    std::string printSource;    // ��ӡ��Դ���ļ�/�Զ����ɵȣ�
    int cipherClause;           // ��������
    int plainCodeClause;        // ��������
    float angle;                // ��ת�Ƕ�
    bool locateMode;            // A��(0)/B��(1)
};

// ��չ����Ϣ������ subboard_expansion_point_tab��
struct ExpansionPointInfo {
    float x, y;     // ����
    float angle;    // ��ת�Ƕ�
    int expandId;   // ��չ����
    bool locateMode;// A/B��
};

// ����������� encode_rule��
struct EncodeRuleInfo {
    std::string ruleName; // ������
    int encodeType;       // 0=���룬1=����
    int length;           // ����
    int position;         // λ������
    int mode;             // ����ģʽ
};

// ���������������� autoproduction_counter��
struct ProductionCounterInfo {
    int totalBoards;  // ���Ӱ���
    int currentCode;  // ��ǰ���
};

//
// ===============================
// ?? ProductionMaterialManager
// �� DAO�����ݿ���ʣ� + Service��ҵ���߼����ϲ���һ����
// ===============================
//

class ProductionMaterialManager {
private:
    // MySQL ������Ϣ
    std::string m_host, m_user, m_password, m_database;
    unsigned short m_port;

public:
    // ���캯�����������ݿ����Ӳ���
    ProductionMaterialManager(const std::string& host,
        const std::string& user,
        const std::string& password,
        const std::string& database,
        unsigned short port = 3306);

    // ======= ���ݷ��ʲ㺯�� =======
    // ��Щ����ֱ�ӷ������ݿ��ִ�� SQL ��ѯ
    std::vector<ChildBoardInfo> getChildBoardCodes(const std::string& modelName, bool locateMode);
    std::vector<ExpansionPointInfo> getExpansionPoints(const std::string& modelName, bool locateMode);
    std::vector<EncodeRuleInfo> getEncodeRules(const std::string& prodName);
    std::pair<float, float> getDefectiveCoordinates(const std::string& modelName, bool locateMode);
    ProductionCounterInfo getProductionCounter(const std::string& modelName);
    bool updateProductionCounter(const std::string& modelName, int currentCode);
    std::vector<std::string> getAllProductionNames();
    bool testConnection();

    // ======= ҵ���߼��㺯�� =======
    // ��Щ������϶��ű����ݣ����������� ProductionMaterial
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
    // ======= �ڲ����ߺ��� =======
    // ���ڼ����ݿ����������ת�����������
    std::unique_ptr<MySqlConnect> createConnection();  // �������ݿ�����
    float parseFloat(const std::string& str, float defaultValue = 0.0f); // ��ȫתfloat
    int parseInt(const std::string& str, int defaultValue = 0);          // ��ȫתint

    // ���º����ǡ���������߼������Ѳ�ѯ����Ž� ProductionMaterial ������
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
