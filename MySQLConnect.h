#pragma once
#ifndef MYSQLCONNECT_H
#define MYSQLCONNECT_H
#include <iostream>
#include <mysql.h>
#include <string> // ȷ������ string ͷ�ļ�
using namespace std;

class MySqlConnect
{
private:
    void freeResult();
    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_result = nullptr;
    MYSQL_ROW m_row = nullptr;
    unsigned int m_num_fields = 0; // ���������浱ǰ�����������

public:
    MySqlConnect();
    ~MySqlConnect();
    bool connect(string user, string passwd, string dbName, string ip, unsigned short port = 3306);
    bool update(string sql); // ���� INSERT, UPDATE, DELETE
    bool query(string sql);  // ���� SELECT

    bool next();
    // �õ�������е��ֶ�ֵ��ȡ��¼�����ֶη���
    // �Ľ������� std::optional<std::string> ��ȷ��������Чֵ���ṩ������־
    string value(int index);

    // ��������ȡ���һ��MySQL�����Ĵ�����Ϣ
    string getError();
};
#endif // !MYSQLCONNECT_H