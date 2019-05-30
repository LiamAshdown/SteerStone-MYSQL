/*
* Liam Ashdown
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PreparedStatements.h"

namespace SteerStone
{
    /// Constructor
    PreparedStatements::PreparedStatements()
    {
    }

    /// Deconstructor
    PreparedStatements::~PreparedStatements()
    {
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

    /// GetPrepareStatement
    /// GetPrepareStatement a PrepareStatement
    PreparedStatement * PreparedStatements::GetPrepareStatement()
    {
        std::unique_lock<std::mutex> l_Guard(m_Mutex);

        for (auto const& l_Itr : m_Pool)
        {
            MYSQLPreparedStatement* l_PreparedStatement = l_Itr;

            for (uint32 l_I = 0; l_I < MAX_PREPARED_STATEMENTS; l_I++)
            {
                PreparedStatement* l_PreparedStatementHolder = l_PreparedStatement->m_Statements[l_I];

                if (l_PreparedStatementHolder->TryLock())
                    return l_PreparedStatementHolder;
            }
        }

        /// If we get here then there's a design logic flaw, it should be impossible for all statements to be used
        /// unless we are running with thousands of players querying at same time which is unlikely
        throw std::runtime_error("GetPrepareStatement: Could not get a prepare statement. All statements are taken.");

        return nullptr;
    }

    /// FreePrepareStatement
    /// Release Prepare statement to be used again
    void PreparedStatements::FreePrepareStatement(PreparedStatement* p_PrepareStatement)
    {
        std::unique_lock<std::mutex> l_Guard(m_Mutex);

        /// Statement must be locked - if not something went wrong and we must investigate
        assert(!p_PrepareStatement->TryLock());

        if (!p_PrepareStatement->TryLock())
            p_PrepareStatement->Unlock();
    }
} ///< NAMESPACE STEERSTONE
