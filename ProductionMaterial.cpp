#include "ProductionMaterial.h"

// =========================================
// ���캯������ʼ��������Ա
// =========================================
ProductionMaterial::ProductionMaterial() {
    // ��ʼ��������Ա
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

    // ���������Ա
    clearAllData();
}

// =========================================
// ���������������
// �����¼��� ProductionMaterial ʱ����
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
// ��ӡ������Ϣ����������ʱʹ�ã�
// =========================================
void ProductionMaterial::printDebugInfo() const {
    std::cout << "\n=== ProductionMaterial Debug Info ===" << std::endl;
    std::cout << "���Ӱ���: " << totalBoards << std::endl;
    std::cout << "��ǰ��������: " << totalProductionCount << std::endl;
    std::cout << "��������: " << dimCodeNum << " | ��������: " << visCoedNum << std::endl;
    std::cout << "������ʶ: (" << badTarget_X << ", " << badTarget_Y << ")" << std::endl;
    std::cout << "�Ӱ���������: " << childBoardprintCodePositions.size() << std::endl;
    std::cout << "��չ������: " << firstExtendedprintCodePositions.size() << std::endl;
    std::cout << "�����������: " << encodeRuleName.size() << std::endl;
    std::cout << "=====================================" << std::endl;
}
