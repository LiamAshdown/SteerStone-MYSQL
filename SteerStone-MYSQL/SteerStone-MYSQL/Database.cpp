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

#include "Database.h"

namespace SteerStone
{
    /// Constructor
    Database::Database()
    {
    }

    /// Deconstructor
    Database::~Database()
    {
    }

    /// Initialize
    /// @p_Username : Name of user
    /// @p_Password : Password of user
    /// @p_Port     : Port we are connecting to
    /// @p_Host     : Address we are connecting to
    /// @p_Database : Database we are querying to
    /// @p_PoolSize : Amount of MYSQL connections we are spawning
    void Database::StartUp(std::string const p_Username, std::string const p_Password, uint32 const p_Port, std::string const p_Host, std::string const p_Database, uint32 const p_PoolSize)
    {
        /// Check if pool size is within our requirements
        if (p_PoolSize < MIN_CONNECTION_POOL_SIZE)
            m_PoolSize = MIN_CONNECTION_POOL_SIZE;
        else if (p_PoolSize > MAX_CONNECTION_POOL_SIZE)
            m_PoolSize = MAX_CONNECTION_POOL_SIZE;
        else
            m_PoolSize = p_PoolSize;

        m_PreparedStatements.SetUp(p_Username, p_Password, p_Port, p_Host, p_Database, p_PoolSize);
    }

    /// GetPreparedStatement
    /// Returns a Prepare Statement from Pool
    MYSQLPreparedStatement* Database::GetPrepareStatement()
    {
       return m_PreparedStatements.Borrow();
    }

    /// FreePrepareStatement
    /// Free Prepare Statement
    /// @p_PreparedStatement : Connection we are freeing
    void Database::FreePrepareStatement(MYSQLPreparedStatement* p_PreparedStatement)
    {
        m_PreparedStatements.UnBorrow(p_PreparedStatement);
    }
} ///< NAMESPACE STEERSTONE