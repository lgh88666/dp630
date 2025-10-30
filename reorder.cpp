

#include "reorder.h"
#include <algorithm>
#include <cmath>
using namespace std;

// ================= PointData ��Ա����ʵ�� =================
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

// ================= ��������ʵ�� =================
// ����ȽϺ���
static bool compareXYAsc(const PointData& a, const PointData& b) {
    return a.xy_sum < b.xy_sum;
}

// ����ȽϺ���
static bool compareXYDesc(const PointData& a, const PointData& b) {
    return a.xy_sum > b.xy_sum;
}

// ��ȡ����
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

// ������СXY�͵�������
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

// ================= ��������ʵ�� =================
vector<vector<float>> reorder_points(const vector<vector<float>>& points_raw) {
    vector<PointData> points;
    for (const auto& p : points_raw) {
        points.emplace_back(p);
    }

    // ����1: ��angle��internal_id�������
    sort(points.begin(), points.end(), [](const PointData& a, const PointData& b) {
        if (a.angle == b.angle) {
            return a.internal_id < b.internal_id;
        }
        return a.angle < b.angle;
        });

    // ����2: ��ȡ���鲢��������
    auto groups = extractGroups(points);
    for (auto& group : groups) {
        sort(group.begin(), group.end(), compareXYAsc);
    }

    // ����3: �ҵ���ʼ�飨��СXY�ͣ�
    int first_group_idx = findMinXYGroup(groups);
    if (first_group_idx == -1) return {}; // ��������Ч���ݵ����

    vector<PointData> result;
    vector<bool> used(groups.size(), false);

    // �����ʼ�飨����
    result.insert(result.end(), groups[first_group_idx].begin(), groups[first_group_idx].end());
    used[first_group_idx] = true;
    float last_xy = result.back().xy_sum;

    // ����4: ��̬ѡ�������
    while (result.size() < points.size()) {
        int next_group = -1;
        bool use_ascending = true;
        float min_diff = numeric_limits<float>::max();

        // Ѱ����ӽ�����
        for (size_t i = 0; i < groups.size(); ++i) {
            if (used[i]) continue;

            // �����������Ӳ���
            float diff_asc = abs(groups[i].front().xy_sum - last_xy);
            // ���㽵�����Ӳ���
            float diff_desc = abs(groups[i].back().xy_sum - last_xy);

            // ������С����
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

        // ���ѡ�е���
        if (use_ascending) {
            result.insert(result.end(), groups[next_group].begin(), groups[next_group].end());
        }
        else {
            result.insert(result.end(), groups[next_group].rbegin(), groups[next_group].rend());
        }
        used[next_group] = true;
        last_xy = result.back().xy_sum;
    }

    // ת����vector<vector<float>>
    vector<vector<float>> output;
    for (const auto& p : result) {
        output.push_back(p.toVector());
    }

    return output;
}