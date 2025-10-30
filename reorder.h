
#ifndef REORDER_H
#define REORDER_H

#include <vector>
#include <iostream>
#include <limits>
using namespace std;

// 定义点的数据结构
struct PointData {
    float x, y, angle, sub_board_id, internal_id, xy_sum;

    // 构造函数：从 vector<float> 初始化点数据
    PointData(const vector<float>& data);

    // 将点数据转换为 vector<float> 格式
    vector<float> toVector() const;

    // 判断两个点是否属于同一分组（相同 angle 和 internal_id）
    bool isInSameGroup(const PointData& other) const;
};

// 主排序函数声明：输入原始点数据，返回优化排序后的点序列
vector<vector<float>> reorder_points(const vector<vector<float>>& points_raw);

#endif // REORDER_H