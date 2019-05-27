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

#ifndef _DATABASE_SQL_CONNECTION_h
#define _DATABASE_SQL_CONNECTION_h
#include "SharedDefines.h"
#endif /* !_DATABASE_SQL_CONNECTION_h */

namespace SteerStone
{
    class SQLConnection
    {
    public:
        /// Constructor
        SQLConnection() {}

        /// Deconstructor
        virtual ~SQLConnection() {}

    public:
        /// Create
        /// @p_Username : Name of user
        /// @p_Password : Password of user
        /// @p_Port     : Port we are connecting to
        /// @p_Host     : Address we are connecting to
        /// @p_Database : Database we are querying to
        virtual uint32 Create(std::string const p_Username, std::string const p_Password,
            uint32 const p_Port, std::string const p_Host, std::string const p_Database) = 0;

    protected:
        static volatile bool s_Logged; ///< Used to show MYSQL details only once
    };
} ///< NAMESPACE STEERSTONE

volatile bool SteerStone::SQLConnection::s_Logged = false;
