#include "MySQLConnect.h"
#include <stdexcept> // ���� std::runtime_error

void MySqlConnect::freeResult()
{
    if (m_result)
    {
        mysql_free_result(m_result);
        m_result = nullptr;
        m_num_fields = 0; // ������ͷ�ʱ������Ҳ����
    }
}

MySqlConnect::MySqlConnect()
{
    m_conn = mysql_init(nullptr);
    if (m_conn == nullptr) // �ؼ������ mysql_init �Ƿ�ɹ�
    {
        std::cerr << "MySqlConnect Error: Failed to initialize MySQL connection (mysql_init returned nullptr)." << std::endl;
        // �����������׳��쳣����������һ���ڲ�����״̬���ú���������ǰʧ��
        // throw std::runtime_error("MySQL initialization failed.");
    }
    else
    {
        // ֻ���� m_conn ��Чʱ�������ַ���
        if (mysql_set_character_set(m_conn, "utf8"))
        {
            std::cerr << "MySqlConnect Warning: Failed to set MySQL character set to utf8." << std::endl;
        }
    }
}

MySqlConnect::~MySqlConnect()
{
    if (m_conn != nullptr)
    {
        mysql_close(m_conn);
        m_conn = nullptr; // ��������ָ��
    }
    freeResult();
}

bool MySqlConnect::connect(string user, string passwd, string dbName, string ip, unsigned short port)
{
    if (m_conn == nullptr) { // ��� m_conn ֮ǰ��ʼ��ʧ�ܣ����޷�����
        std::cerr << "MySqlConnect Error: Cannot connect, MySQL connection object is not initialized." << std::endl;
        return false;
    }
    MYSQL* ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0);
    if (ptr == nullptr) {
        std::cerr << "MySqlConnect Error: Failed to connect to database: " << mysql_error(m_conn) << std::endl;
    }
    return ptr != nullptr;
}

// ���� INSERT, UPDATE, DELETE ���
bool MySqlConnect::update(string sql)
{
    if (m_conn == nullptr) {
        std::cerr << "MySqlConnect Error: Cannot update, MySQL connection is not initialized." << std::endl;
        return false;
    }
    if (mysql_query(m_conn, sql.c_str()))
    {
        std::cerr << "MySqlConnect Error: SQL UPDATE failed: " << mysql_error(m_conn) << " SQL: " << sql << std::endl;
        return false;
    };
    return true; // UPDATE, INSERT, DELETE �ɹ������� true
}

// ���� SELECT ���
bool MySqlConnect::query(string sql)
{
    freeResult(); // �����ϴν��
    if (m_conn == nullptr) {
        std::cerr << "MySqlConnect Error: Cannot query, MySQL connection is not initialized." << std::endl;
        return false;
    }
    if (mysql_query(m_conn, sql.c_str()))
    {
        std::cerr << "MySqlConnect Error: SQL SELECT failed: " << mysql_error(m_conn) << " SQL: " << sql << std::endl;
        return false;
    };

    m_result = mysql_store_result(m_conn);
    if (m_result == nullptr) {
        // mysql_store_result ���� nullptr ��������Ϊ��ѯû�н���� (���� DDL ���),
        // ���߲�ѯʧ�� (�� mysql_query û����).
        // ͨ������ SELECT ��䣬�����ѯ�ɹ���û��ƥ���У�m_result ��Ϊ nullptr��
        // ���� mysql_num_rows(m_result) Ϊ 0��
        // ��������� SELECT ���Ϊ nullptr �� mysql_errno() ��Ϊ0��˵�������⡣
        if (mysql_errno(m_conn) != 0) {
            std::cerr << "MySqlConnect Error: Failed to store query result: " << mysql_error(m_conn) << std::endl;
            return false;
        }
        // ���û�д����� m_result Ϊ nullptr����ͨ����ζ���Ƿ� SELECT ��䣬�� SELECT �޽����
        // ���� query ����ֻ���� SELECT����������Ӧ��Ϊʧ�ܣ���������ȷ֪��Ϊ��Ϊ nullptr
    }

    // ֻ�� SELECT �������ֶ���
    if (m_result) {
        m_num_fields = mysql_num_fields(m_result);
    }
    else {
        m_num_fields = 0;
    }

    return true; // SELECT ��ѯ����ɹ�
}

bool MySqlConnect::next()
{
    if (m_result == nullptr)
    {
        return false;
    }
    m_row = mysql_fetch_row(m_result);
    return m_row != nullptr;
}

string MySqlConnect::value(int index)
{
    // �ؼ�����������Ƿ�����Ч��Χ��
    if (m_row == nullptr || index < 0 || (unsigned int)index >= m_num_fields || m_row[index] == nullptr)
    {
        // ��ӡ����ϸ�ľ�����Ϣ���������
        std::cerr << "MySqlConnect Warning: Attempted to access invalid column index " << index
            << ". Current row pointer: " << (m_row ? "valid" : "nullptr")
            << ", Total fields in result: " << m_num_fields
            << ". Returning empty string." << std::endl;
        return string(); // ���ؿ��ַ�������ֹ����
    }
    return string(m_row[index]);
}

string MySqlConnect::getError() {
    if (m_conn) {
        return mysql_error(m_conn);
    }
    return "MySQL connection not initialized.";
}