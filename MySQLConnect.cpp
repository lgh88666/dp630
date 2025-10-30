#include "MySQLConnect.h"
#include <stdexcept> // 用于 std::runtime_error

void MySqlConnect::freeResult()
{
    if (m_result)
    {
        mysql_free_result(m_result);
        m_result = nullptr;
        m_num_fields = 0; // 结果集释放时，列数也清零
    }
}

MySqlConnect::MySqlConnect()
{
    m_conn = mysql_init(nullptr);
    if (m_conn == nullptr) // 关键：检查 mysql_init 是否成功
    {
        std::cerr << "MySqlConnect Error: Failed to initialize MySQL connection (mysql_init returned nullptr)." << std::endl;
        // 可以在这里抛出异常，或者设置一个内部错误状态，让后续操作提前失败
        // throw std::runtime_error("MySQL initialization failed.");
    }
    else
    {
        // 只有在 m_conn 有效时才设置字符集
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
        m_conn = nullptr; // 避免悬空指针
    }
    freeResult();
}

bool MySqlConnect::connect(string user, string passwd, string dbName, string ip, unsigned short port)
{
    if (m_conn == nullptr) { // 如果 m_conn 之前初始化失败，则无法连接
        std::cerr << "MySqlConnect Error: Cannot connect, MySQL connection object is not initialized." << std::endl;
        return false;
    }
    MYSQL* ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0);
    if (ptr == nullptr) {
        std::cerr << "MySqlConnect Error: Failed to connect to database: " << mysql_error(m_conn) << std::endl;
    }
    return ptr != nullptr;
}

// 用于 INSERT, UPDATE, DELETE 语句
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
    return true; // UPDATE, INSERT, DELETE 成功，返回 true
}

// 用于 SELECT 语句
bool MySqlConnect::query(string sql)
{
    freeResult(); // 清理上次结果
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
        // mysql_store_result 返回 nullptr 可能是因为查询没有结果集 (例如 DDL 语句),
        // 或者查询失败 (但 mysql_query 没报错).
        // 通常对于 SELECT 语句，如果查询成功但没有匹配行，m_result 不为 nullptr，
        // 而是 mysql_num_rows(m_result) 为 0。
        // 如果这里是 SELECT 结果为 nullptr 且 mysql_errno() 不为0，说明有问题。
        if (mysql_errno(m_conn) != 0) {
            std::cerr << "MySqlConnect Error: Failed to store query result: " << mysql_error(m_conn) << std::endl;
            return false;
        }
        // 如果没有错误，且 m_result 为 nullptr，则通常意味着是非 SELECT 语句，或 SELECT 无结果集
        // 但此 query 函数只用于 SELECT，所以这里应视为失败，除非你明确知道为何为 nullptr
    }

    // 只有 SELECT 语句才有字段数
    if (m_result) {
        m_num_fields = mysql_num_fields(m_result);
    }
    else {
        m_num_fields = 0;
    }

    return true; // SELECT 查询本身成功
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
    // 关键：检查索引是否在有效范围内
    if (m_row == nullptr || index < 0 || (unsigned int)index >= m_num_fields || m_row[index] == nullptr)
    {
        // 打印更详细的警告信息，方便调试
        std::cerr << "MySqlConnect Warning: Attempted to access invalid column index " << index
            << ". Current row pointer: " << (m_row ? "valid" : "nullptr")
            << ", Total fields in result: " << m_num_fields
            << ". Returning empty string." << std::endl;
        return string(); // 返回空字符串，防止崩溃
    }
    return string(m_row[index]);
}

string MySqlConnect::getError() {
    if (m_conn) {
        return mysql_error(m_conn);
    }
    return "MySQL connection not initialized.";
}