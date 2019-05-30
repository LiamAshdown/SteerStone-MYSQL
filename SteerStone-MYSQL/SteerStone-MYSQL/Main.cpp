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

#include "SharedDefines.h"
#include "DatabaseTypes.h"

using namespace SteerStone;

DatabaseType RoomDatabase;
 
int main()
{
    /// Initialize our logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    RoomDatabase.StartUp("root", "carbon12", 3306, "127.0.0.1", "rooms", 1);

    OperatorProcessor l_Processor;

    {
        PreparedStatement* l_PreparedStatementHolder = RoomDatabase.GetPrepareStatement();
        l_PreparedStatementHolder->PrepareStatement("INSERT INTO room_rating(account_id, room_id) VALUES (?,?)");
        l_PreparedStatementHolder->SetUint32(INDEX_0, 32);
        l_PreparedStatementHolder->SetUint32(INDEX_1, 32);
        //PreparedResultSet* l_PreparedResultSet = l_PreparedStatementHolder->ExecuteStatement();

        l_Processor.AddOperator(RoomDatabase.PrepareOperator(l_PreparedStatementHolder));

        while (true)
        {
            l_Processor.ProcessOperators();
        }


      //  RoomDatabase.FreePrepareStatement(l_PreparedStatementHolder);
    }

    system("pause");
    return 0;
}