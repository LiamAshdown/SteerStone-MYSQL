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

#ifndef _PREPARED_STATEMENT_MYSQL_PREPARED_STATEMENT_h
#define _PREPARED_STATEMENT_MYSQL_PREPARED_STATEMENT_h
#include "SharedDefines.h"
#include "PreparedResultSet.h"
#endif /* !_PREPARED_STATEMENT_MYSQL_PREPARED_STATEMENT_h */

namespace SteerStone
{
    enum Index
    {
        INDEX_0,
        INDEX_1,
        INDEX_2,
        INDEX_3,
        INDEX_4,
        INDEX_5,
        INDEX_6,
        INDEX_7,
        INDEX_8,
        INDEX_9,
        INDEX_10,
        INDEX_11,
        INDEX_12,
        INDEX_13,
        INDEX_14,
        INDEX_15,
    };

    class MYSQLPreparedStatement
    {
    public:
        /// Constructor
        MYSQLPreparedStatement();

        /// Constructor
        ~MYSQLPreparedStatement();

    public:
        /// SetupStatements
        /// @p_Username : Name of user
        /// @p_Password : Password of user
        /// @p_Port     : Port we are connecting to
        /// @p_Host     : Address we are connecting to
        /// @p_Database : Database we are querying to
        /// @p_PoolSize : Amount of MYSQL connections we are spawning
        uint32 Connect(std::string const p_Username, std::string const p_Password,
            uint32 const p_Port, std::string const p_Host, std::string const p_Database);

    public:
        /// PrepareStatement
        /// Prepare the statement
        /// @p_Query : Query which will be executed to database
        void PrepareStatement(char const* p_Query);

        /// Execute
        /// Execute query
        PreparedResultSet* Execute();

    public:
        /// Set our prepared values
        void SetBool(uint8 p_Index, bool p_Value)          { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint8(uint8 p_Index, uint8 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint16(uint8 p_Index, uint16 p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint32(uint8 p_Index, uint32 p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint64(uint8 p_Index, uint64 p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt8(uint8 p_Index, int8 p_Value)          { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt16(uint8 p_Index, int16 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt32(uint8 p_Index, int32 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt64(uint8 p_Index, int64 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetFloat(uint8 p_Index, float p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetDouble(uint8 p_Index, double p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetString(uint8 p_Index, std::string p_Value) { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }

    private:
        /// Prepare
        /// Prepare the query
        /// @p_Query : Query which will be executed to database
        bool Prepare(char const* p_Query);

        /// BindParameters
        /// Bind parameters from storage into SQL
        void BindParameters();

        /// ExecuteStatement
        /// Execute the statement to database
        bool ExecuteStatement(MYSQL_RES** p_Result, MYSQL_FIELD** p_Fields, uint32* p_FieldCount);

        /// RemoveBinds
        /// Remove previous binds
        void RemoveBinds();

    private:
        MYSQL* m_Connection;
        MYSQL_STMT* m_Stmt;
        MYSQL_BIND* m_Bind;
        uint32 m_ParametersCount;

    private:
        std::string m_Query;
        std::vector<std::pair<uint8, SQLBindData>> m_Binds;
        bool m_PrepareError;

    private:
        static volatile bool s_Logged;
    };
} ///< NAMESPACE STEERSTONE