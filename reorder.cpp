

#include "reorder.h"
#include <algorithm>
#include <cmath>
using namespace std;

// ================= PointData 成员函数实现 =================
PointData::PointData(const vector<float>& data) {
    if (data.size() >= 5) {
        x = data[0];
        y = data[1];
        angle = data[2];
        sub_board_id = data[3];
        internal_id = data[4];
        xy_sum = x + y;
    }
    else {
        x = y = angle = sub_board_id = internal_id = numeric_limits<float>::quiet_NaN();
        xy_sum = numeric_limits<float>::infinity();
        cerr << "Warning: Invalid point data (size < 5)!" << endl;
    }
}

vector<float> PointData::toVector() const {
    return { x, y, angle, sub_board_id, internal_id };
}

bool PointData::isInSameGroup(const PointData& other) const {
    return (angle == other.angle) && (internal_id == other.internal_id);
}

// ================= 辅助函数实现 =================
// 升序比较函数
static bool compareXYAsc(const PointData& a, const PointData& b) {
    return a.xy_sum < b.xy_sum;
}

// 降序比较函数
static bool compareXYDesc(const PointData& a, const PointData& b) {
    return a.xy_sum > b.xy_sum;
}

// 提取分组
static vector<vector<PointData>> extractGroups(const vector<PointData>& points) {
    vector<vector<PointData>> groups;
    if (points.empty()) return groups;

    groups.push_back({ points[0] });
    for (size_t i = 1; i < points.size(); ++i) {
        if (!points[i].isInSameGroup(points[i - 1])) {
            groups.push_back({});
        }
        groups.back().push_back(points[i]);
    }
    return groups;
}

// 查找最小XY和的组索引
static int findMinXYGroup(const vector<vector<PointData>>& groups) {
    float min_sum = numeric_limits<float>::max();
    int group_index = -1;

    for (size_t i = 0; i < groups.size(); ++i) {
        if (!groups[i].empty() && groups[i].front().xy_sum < min_sum) {
            min_sum = groups[i].front().xy_sum;
            group_index = i;
        }
    }
    return group_index;
}

// ================= 主排序函数实现 =================
vector<vector<float>> reorder_points(const vector<vector<float>>& points_raw) {
    vector<PointData> points;
    for (const auto& p : points_raw) {
        points.emplace_back(p);
    }

    // 步骤1: 按angle和internal_id排序分组
    sort(points.begin(), points.end(), [](const PointData& a, const PointData& b) {
        if (a.angle == b.angle) {
            return a.internal_id < b.internal_id;
        }
        return a.angle < b.angle;
        });

    // 步骤2: 提取分组并组内排序
    auto groups = extractGroups(points);
    for (auto& group : groups) {
        sort(group.begin(), group.end(), compareXYAsc);
    }

    // 步骤3: 找到起始组（最小XY和）
    int first_group_idx = findMinXYGroup(groups);
    if (first_group_idx == -1) return {}; // 处理无有效数据的情况

    vector<PointData> result;
    vector<bool> used(groups.size(), false);

    // 添加起始组（升序）
    result.insert(result.end(), groups[first_group_idx].begin(), groups[first_group_idx].end());
    used[first_group_idx] = true;
    float last_xy = result.back().xy_sum;

    // 步骤4: 动态选择后续组
    while (result.size() < points.size()) {
        int next_group = -1;
        bool use_ascending = true;
        float min_diff = numeric_limits<float>::max();

        // 寻找最接近的组
        for (size_t i = 0; i < groups.size(); ++i) {
            if (used[i]) continue;

            // 计算升序连接差异
            float diff_asc = abs(groups[i].front().xy_sum - last_xy);
            // 计算降序连接差异
            float diff_desc = abs(groups[i].back().xy_sum - last_xy);

            // 更新最小差异
            if (diff_asc < min_diff) {
                min_diff = diff_asc;
                next_group = i;
                use_ascending = true;
            }
            if (diff_desc < min_diff) {
                min_diff = diff_desc;
                next_group = i;
                use_ascending = false;
            }
        }

        // 添加选中的组
        if (use_ascending) {
            result.insert(result.end(), groups[next_group].begin(), groups[next_group].end());
        }
        else {
            result.insert(result.end(), groups[next_group].rbegin(), groups[next_group].rend());
        }
        used[next_group] = true;
        last_xy = result.back().xy_sum;
    }

    // 转换回vector<vector<float>>
    vector<vector<float>> output;
    for (const auto& p : result) {
        output.push_back(p.toVector());
    }

    return output;
}