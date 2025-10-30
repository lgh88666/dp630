
#ifndef REORDER_H
#define REORDER_H

#include <vector>
#include <iostream>
#include <limits>
using namespace std;

// ���������ݽṹ
struct PointData {
    float x, y, angle, sub_board_id, internal_id, xy_sum;

    // ���캯������ vector<float> ��ʼ��������
    PointData(const vector<float>& data);

    // ��������ת��Ϊ vector<float> ��ʽ
    vector<float> toVector() const;

    // �ж��������Ƿ�����ͬһ���飨��ͬ angle �� internal_id��
    bool isInSameGroup(const PointData& other) const;
};

// ������������������ԭʼ�����ݣ������Ż������ĵ�����
vector<vector<float>> reorder_points(const vector<vector<float>>& points_raw);

#endif // REORDER_H