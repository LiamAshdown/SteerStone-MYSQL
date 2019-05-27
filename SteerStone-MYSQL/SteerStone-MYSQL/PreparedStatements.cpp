#include "PreparedStatements.h"

namespace SteerStone
{

    PreparedStatements::PreparedStatements()
    {
    }


    PreparedStatements::~PreparedStatements()
    {
        LOG_INFO << "PreparedStatements";
    }

    /// SetUp
    /// @p_Username : Name of user
    /// @p_Password : Password of user
    /// @p_Port     : Port we are connecting to
    /// @p_Host     : Address we are connecting to
    /// @p_Database : Database we are querying to
    /// @p_PoolSize : Amount of MYSQL connections we are spawning
    uint32 SteerStone::PreparedStatements::SetUp(std::string const p_Username, std::string const p_Password, uint32 const p_Port, std::string const p_Host, std::string const p_Database, uint32 const p_PoolSize)
    {
        for (uint32 l_I = 0; l_I < p_PoolSize; l_I++)
        {
            MYSQLPreparedStatement* l_PreparedStatement = new MYSQLPreparedStatement();

            uint32 l_Success = l_PreparedStatement->Connect(p_Username, p_Password, p_Port, p_Host, p_Database);
            
            /// If > 0 - Failed to connect
            if (l_Success)
            {
                LOG_ERROR << "Failed to create connection. MySQL Error: " << l_Success << ". Please refer to MYSQL Documentation.";
                return l_Success;
            }

            m_Pool.push_back(l_PreparedStatement);
        }

        return 0;
    }

    /// Borrow
    /// Borrow a connection
    MYSQLPreparedStatement * PreparedStatements::Borrow()
    {
        std::unique_lock<std::mutex> l_Guard(m_Mutex);

        if (m_Pool.empty())
        {
            LOG_FATAL << "ALL CONNECTIONS ARE BORROWED! CRASHING...";
            assert(!m_Pool.empty());
            return nullptr;
        }

        /// Declare our connection from Pool
        MYSQLPreparedStatement* l_PreparedStatement = m_Pool.front();
        
        /// Insert our borrowed connection into the borrowed pool
        m_Borrowed.insert(l_PreparedStatement);

        /// Remove connection from Pool
        m_Pool.pop_front();

        return l_PreparedStatement;
    }

    /// UnBorrow
    /// Remove connection from UnBorrowed pool and insert back into pool
    /// @p_PreparedStatement : the connection we are handling
    void PreparedStatements::UnBorrow(MYSQLPreparedStatement* p_PreparedStatement)
    {
        std::unique_lock<std::mutex> l_Guard(m_Mutex);

        m_Borrowed.erase(p_PreparedStatement);

        m_Pool.push_back(p_PreparedStatement);
    }
}
