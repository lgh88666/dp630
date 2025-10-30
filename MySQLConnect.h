#pragma once
#ifndef MYSQLCONNECT_H
#define MYSQLCONNECT_H
#include <iostream>
#include <mysql.h>
#include <string> // 确保包含 string 头文件
using namespace std;

class MySqlConnect
{
private:
    void freeResult();
    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_result = nullptr;
    MYSQL_ROW m_row = nullptr;
    unsigned int m_num_fields = 0; // 新增：保存当前结果集的列数

public:
    MySqlConnect();
    ~MySqlConnect();
    bool connect(string user, string passwd, string dbName, string ip, unsigned short port = 3306);
    bool update(string sql); // 用于 INSERT, UPDATE, DELETE
    bool query(string sql);  // 用于 SELECT

    bool next();
    // 得到结果集中的字段值，取记录里面字段方法
    // 改进：返回 std::optional<std::string> 或确保返回有效值并提供错误日志
    string value(int index);

    // 新增：获取最近一次MySQL操作的错误信息
    string getError();
};
#endif // !MYSQLCONNECT_H