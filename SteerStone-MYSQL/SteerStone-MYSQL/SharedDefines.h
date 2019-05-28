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

#ifndef _COMMON_SHARED_DEFINES_h
#define _COMMON_SHARED_DEFINES_h
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <ctype.h>
#include <memory>
#include <queue>
#include <deque>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <assert.h>
#include <stdlib.h>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#endif /* _COMMON_SHARED_DEFINES_h */

typedef std::int64_t int64;
typedef std::int32_t int32;
typedef std::int16_t int16;
typedef std::int8_t int8;
typedef std::uint64_t uint64;
typedef std::uint32_t uint32;
typedef std::uint16_t uint16;
typedef std::uint8_t uint8;

#define MIN_CONNECTION_POOL_SIZE 1
#define MAX_CONNECTION_POOL_SIZE 10 ///< Don't change this
#define MAX_PREPARED_STATEMENTS 50 /// And don't change this
#define MAX_QUERY_LENGTH  (32*1024)