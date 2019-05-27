#include "MYSQLPreparedStatement.h"
#include "SQLCommon.h"

namespace SteerStone
{
    volatile bool MYSQLPreparedStatement::s_Logged = false;

    /// Constructor
    MYSQLPreparedStatement::MYSQLPreparedStatement() : m_PrepareError(false)
    {
    }

    /// Deconstructor
    MYSQLPreparedStatement::~MYSQLPreparedStatement()
    {
        LOG_INFO << "MYSQLPreparedStatement";
        mysql_close(m_Connection);
    }

    /// Connect
    /// @p_Username : Name of user
    /// @p_Password : Password of user
    /// @p_Port     : Port we are connecting to
    /// @p_Host     : Address we are connecting to
    /// @p_Database : Database we are querying to
    /// @p_PoolSize : Amount of MYSQL connections we are spawning
    uint32 MYSQLPreparedStatement::Connect(std::string const p_Username, std::string const p_Password, uint32 const p_Port, std::string const p_Host, std::string const p_Database)
    {
        /// Initialize connection
        MYSQL* l_Connection;
        l_Connection = mysql_init(NULL);

        if (!l_Connection)
        {
            LOG_INFO << ("Could not initialize MySQL connection to database: %s", p_Database);
            return CR_UNKNOWN_ERROR;
        }

        mysql_options(l_Connection, MYSQL_SET_CHARSET_NAME, "utf8");

        /// Connect to database
        m_Connection = mysql_real_connect(l_Connection, p_Host.c_str(), p_Username.c_str(), p_Password.c_str(), p_Database.c_str(), p_Port, NULL, NULL);

        if (m_Connection)
        {
            if (!s_Logged)
            {
                LOG_INFO << "MySQL Client Libary: " << mysql_get_client_info();
                LOG_INFO << "MySQL Server Version: " << mysql_get_server_info(m_Connection);
                LOG_INFO << "Connected to MYSQL Database at " << m_Connection->host;

                s_Logged = true;
            }

            mysql_set_character_set(m_Connection, "utf8");

            return 0;
        }
        else
        {
            /// Free connection and report error
            mysql_close(l_Connection);
            return mysql_errno(l_Connection);
        }
    }

    /// Prepare the statement
    /// @p_Query : Query which will be executed to database
    void MYSQLPreparedStatement::PrepareStatement(char const* p_Query)
    {
        if (!Prepare(p_Query))
        {
            m_PrepareError = true;
            LOG_ERROR << "Failed in Preparing Statement!";
        }
    }

    /// Execute
    /// Execute query
    PreparedResultSet* MYSQLPreparedStatement::Execute()
    {
        if (m_PrepareError)
            return nullptr;

        MYSQL_RES* l_Result = nullptr;
        MYSQL_FIELD* l_Fields = nullptr;
        uint32 l_FieldCount = 0;

        if (!ExecuteStatement(&l_Result, &l_Fields, &l_FieldCount))
            return nullptr;

        return new PreparedResultSet(m_Stmt, l_Result, l_Fields, l_FieldCount);
    }

    /// Prepare
    /// Prepare the query
    /// @p_Query : Query which will be executed to database
    bool MYSQLPreparedStatement::Prepare(char const * p_Query)
    {
        RemoveBinds();

        m_Stmt = mysql_stmt_init(m_Connection);
        m_Query = p_Query;

        if (!m_Stmt)
        {
            LOG_ERROR << "m_Stmt: " << mysql_error(m_Connection);
            return false;
        }

        if (mysql_stmt_prepare(m_Stmt, m_Query.c_str(), m_Query.length()))
        {
            LOG_ERROR << "mysql_stmt_prepare: " << mysql_error(m_Connection) << " ON " << m_Query;
            return false;
        }

        m_ParametersCount = mysql_stmt_param_count(m_Stmt);

        if (m_ParametersCount)
        {
            m_Bind = new MYSQL_BIND[m_ParametersCount];
            memset(m_Bind, 0, sizeof(MYSQL_BIND) * m_ParametersCount);
        }

        return true;
    }

    /// BindParameters
    /// Bind parameters from storage into SQL
    void MYSQLPreparedStatement::BindParameters()
    {
        for (auto l_Itr = m_Binds.cbegin(); l_Itr != m_Binds.cend(); l_Itr++)
        {
            uint8 l_Unsigned = 0;
            m_Bind[l_Itr->first].buffer_type   = l_Itr->second.GetFieldType(l_Unsigned);
            m_Bind[l_Itr->first].is_unsigned   = l_Unsigned;
            m_Bind[l_Itr->first].buffer        = l_Itr->second.GetBuffer();
            m_Bind[l_Itr->first].length        = nullptr;
            m_Bind[l_Itr->first].buffer_length = l_Itr->second.GetSize();
        }

        if (mysql_stmt_bind_param(m_Stmt, m_Bind))
        {
            LOG_ERROR << "mysql_stmt_bind_param: Cannot bind parameters ON " << m_Query;
        }
    }

    bool MYSQLPreparedStatement::ExecuteStatement(MYSQL_RES** p_Result, MYSQL_FIELD** p_Fields, uint32* p_FieldCount)
    {
        BindParameters();

        if (mysql_stmt_execute(m_Stmt))
        {
            LOG_ERROR << "mysql_stmt_execute: " << mysql_stmt_error(m_Stmt);
            return false;
        }

        *p_Result = mysql_stmt_result_metadata(m_Stmt);
        *p_FieldCount = mysql_stmt_field_count(m_Stmt);

        if (!*p_Result)
        {
            mysql_free_result(*p_Result);
            return false;
        }

        *p_Fields = mysql_fetch_fields(*p_Result);

        return true;
    }

    /// RemoveBinds
    /// Remove previous binds
    void MYSQLPreparedStatement::RemoveBinds()
    {
        if (!m_Stmt)
            return;

        delete[] m_Bind;
        m_Binds.clear();
        mysql_stmt_close(m_Stmt);

        m_PrepareError = false;
        m_Bind = nullptr;
        m_Stmt = nullptr;
        m_Query = std::string();
        m_ParametersCount = 0;
    }
}