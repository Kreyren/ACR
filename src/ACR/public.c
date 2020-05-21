/*

    ********** DO NOT REMOVE THIS INFORMATION ************

    ACR - A set of C functions in a git Repository
    Copyright (C) 2018 - 2020 Adam C. Rosenberg

    Please read LICENSE before using this code

    Please read README.md for instructions on including
    this code in your project and where to find examples

    If you contribute to this code, Please add your name
    and email address to the AUTHORS list below so that
    we know who to thank!

    AUTHORS

    Adam Rosenberg <adam@sirspot.com>

    LICENSE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


    ******************************************************

*/
/** \file public.c

    \see ACR/public.h
    
*/
#include "/workspace/ACR/include/ACR/public.h"

//#define ACR_USE_UTF8_LOCALE

#ifdef ACR_USE_UTF8_LOCALE
// included for setlocale
#include <locale.h>
#endif

/** info string lookup table
*/
static char* g_ACRInfoStringLookup[ACR_INFO_COUNT+1] =
{
    //
    // COMPARISON
    //
    ACR_INFO_STR_EQUAL,     // ACR_INFO_EQUAL
    ACR_INFO_STR_NOT_EQUAL, // ACR_INFO_NOT_EQUAL
    ACR_INFO_STR_LESS,      // ACR_INFO_LESS
    ACR_INFO_STR_GREATER,   // ACR_INFO_GREATER
    ACR_INFO_STR_UNKNOWN,   // ACR_INFO_UNKNOWN

    //
    // POSITIVE, NEGATIVE
    //
    ACR_INFO_STR_TRUE,      // ACR_INFO_TRUE
    ACR_INFO_STR_FALSE,     // ACR_INFO_FALSE
    ACR_INFO_STR_OK,        // ACR_INFO_OK
    ACR_INFO_STR_ERROR,     // ACR_INFO_ERROR
    ACR_INFO_STR_ENABLED,   // ACR_INFO_ENABLED
    ACR_INFO_STR_DISABLED,  // ACR_INFO_DISABLED
    ACR_INFO_STR_YES,       // ACR_INFO_YES
    ACR_INFO_STR_NO,        // ACR_INFO_NO
    ACR_INFO_STR_ON,        // ACR_INFO_ON
    ACR_INFO_STR_OFF,       // ACR_INFO_OFF
    ACR_INFO_STR_START,     // ACR_INFO_START
    ACR_INFO_STR_STOP,      // ACR_INFO_STOP
    ACR_INFO_STR_ACCEPT,    // ACR_INFO_ACCEPT
    ACR_INFO_STR_IGNORE,    // ACR_INFO_IGNORE
    ACR_INFO_STR_VALID,     // ACR_INFO_VALID
    ACR_INFO_STR_INVALID,   // ACR_INFO_INVALID

    //
    // POSITION
    //
    ACR_INFO_STR_TOP,       // ACR_INFO_TOP
    ACR_INFO_STR_BOTTOM,    // ACR_INFO_BOTTOM
    ACR_INFO_STR_FIRST,     // ACR_INFO_FIRST
    ACR_INFO_STR_LAST,      // ACR_INFO_LAST
    ACR_INFO_STR_PREVIOUS,  // ACR_INFO_PREVIOUS
    ACR_INFO_STR_NEXT,      // ACR_INFO_NEXT
    ACR_INFO_STR_CURRENT,   // ACR_INFO_CURRENT

    // DIRECTION
    ACR_INFO_STR_UP,        // ACR_INFO_UP
    ACR_INFO_STR_DOWN,      // ACR_INFO_DOWN
    ACR_INFO_STR_LEFT,      // ACR_INFO_LEFT
    ACR_INFO_STR_RIGHT,     // ACR_INFO_RIGHT

    // AGE
    ACR_INFO_STR_OLD,       // ACR_INFO_OLD
    ACR_INFO_STR_NEW,       // ACR_INFO_NEW

    // INSTRUCTION
    ACR_INFO_STR_BEGIN,     // ACR_INFO_BEGIN
    ACR_INFO_STR_READY,     // ACR_INFO_READY
    ACR_INFO_STR_WAIT,      // ACR_INFO_WAIT
    ACR_INFO_STR_GO,        // ACR_INFO_GO
    ACR_INFO_STR_END,       // ACR_INFO_END

    ""                      // ACR_INFO_COUNT
};

/** day of week string lookup table
*/
static char* g_ACRDayOfWeekStringLookup[ACR_DAY_COUNT+1] =
{
    ACR_DAY_STR_SUNDAY,     // ACR_DAY_SUNDAY
    ACR_DAY_STR_MONDAY,     // ACR_DAY_MONDAY
    ACR_DAY_STR_TUESDAY,    // ACR_DAY_TUESDAY
    ACR_DAY_STR_WEDNESDAY,  // ACR_DAY_WEDNESDAY
    ACR_DAY_STR_THURSDAY,   // ACR_DAY_THURSDAY
    ACR_DAY_STR_FRIDAY,     // ACR_DAY_FRIDAY
    ACR_DAY_STR_SATURDAY,   // ACR_DAY_SATURDAY 
    ""                      // ACR_DAY_COUNT
};

/** month string lookup table
*/
static char* g_ACRMonthStringLookup[ACR_MONTH_COUNT+1] =
{
    ACR_MONTH_STR_JANUARY,  // ACR_MONTH_JANUARY
    ACR_MONTH_STR_FEBRUARY, // ACR_MONTH_FEBRUARY
    ACR_MONTH_STR_MARCH,    // ACR_MONTH_MARCH
    ACR_MONTH_STR_APRIL,    // ACR_MONTH_APRIL
    ACR_MONTH_STR_MAY,      // ACR_MONTH_MAY
    ACR_MONTH_STR_JUNE,     // ACR_MONTH_JUNE
    ACR_MONTH_STR_JULY,     // ACR_MONTH_JULY
    ACR_MONTH_STR_AUGUST,   // ACR_MONTH_AUGUST
    ACR_MONTH_STR_SEPTEMBER,// ACR_MONTH_SEPTEMBER
    ACR_MONTH_STR_OCTOBER,  // ACR_MONTH_OCTOBER
    ACR_MONTH_STR_NOVEMBER, // ACR_MONTH_NOVEMBER
    ACR_MONTH_STR_DECEMBER, // ACR_MONTH_DECEMBER
    ""                      // ACR_MONTH_COUNT
};

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - TEST
//
////////////////////////////////////////////////////////////

/** internal test of quick test of ACR library functions
    \returns ACR_INFO_OK or ACR_INFO_ERROR
*/
int ACR_Test(void)
{
    // ENDIANNESS
    unsigned short systemEndianValue = 0x0001;
    unsigned short bigEndian = ACR_BYTE_ORDER_16(systemEndianValue);
    unsigned long systemEndianValue32 = 0x00000001UL;
    unsigned long bigEndian32 = ACR_BYTE_ORDER_32(systemEndianValue32);
    #if ACR_USE_64BIT == ACR_BOOL_TRUE
    unsigned long long systemEndianValue64 = 0x0000000000000001ULL;
    unsigned long long bigEndian64 = ACR_BYTE_ORDER_64(systemEndianValue64);
    #endif
    // BYTES
    ACR_Byte_t byte = ACR_MAX_BYTE;
    // MEMORY LENGTHS
    ACR_Length_t length = ACR_MAX_LENGTH;
    // COUNTS
    ACR_Count_t count = ACR_MAX_COUNT;
    // DATE TIME VALUES
    ACR_DayOfWeek_t freedomFriday = ACR_DAY_TUESDAY;
    ACR_String_t dayOfWeekString = ACR_DayOfWeekToString(freedomFriday);
    ACR_Month_t monthOfMarch = ACR_MONTH_MARCH;
    ACR_String_t monthString = ACR_MonthToString(monthOfMarch);
    ACR_DATETIME(dateTime);
    // SIMPLE MEMORY BUFFER
    ACR_BUFFER(buffer);
    // DECIMAL VALUES
    ACR_Decimal_t realNumber = (ACR_Decimal_t)5.1999;
    // SIMPLE UTF8 STRINGS
    ACR_String_t stringForInfoUp = ACR_StringFromMemory((ACR_Byte_t*)"up", ACR_MAX_LENGTH, ACR_MAX_COUNT);
    ACR_Info_t infoFromUp = ACR_InfoFromString(stringForInfoUp);
    ACR_String_t stringForEmojiSmile;

    // other
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    int messageNumber;
    #endif

    #ifdef ACR_USE_UTF8_LOCALE
    setlocale(LC_ALL, "en_US.utf8");
    #endif

    //
    // ENDIANNESS
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 100;
    #endif
    #ifdef ACR_COMPILER_VS2017
	#pragma warning(suppress: 4127)
    #endif
    if(ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)
    {
        // system is little endian
        if(systemEndianValue == bigEndian)
        {
            // ACR_IS_BIG_ENDIAN is not set properly or
            // ACR_BYTE_ORDER_16 is not working properly
            ACR_DEBUG_PRINT(messageNumber+1, "ERROR: ACR_IS_BIG_ENDIAN is %s but ACR_BYTE_ORDER_16 did not work", (ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_INFO_STR_FALSE:ACR_INFO_STR_TRUE);
            return ACR_FAILURE;
        }
        else
        {
            ACR_DEBUG_PRINT(messageNumber+2, "OK: ACR_IS_BIG_ENDIAN is %s", (ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_INFO_STR_FALSE:ACR_INFO_STR_TRUE);
        }
    }
    else
    {
        // system is big endian
        if(systemEndianValue != bigEndian)
        {
            // ACR_IS_BIG_ENDIAN is not set properly or
            // ACR_BYTE_ORDER_16 is not working properly
            ACR_DEBUG_PRINT(messageNumber+3, "ERROR: ACR_IS_BIG_ENDIAN is %s but ACR_BYTE_ORDER_16 did not work", (ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_INFO_STR_FALSE:ACR_INFO_STR_TRUE);
            return ACR_FAILURE;
        }
        else
        {
            ACR_DEBUG_PRINT(messageNumber+4, "OK: ACR_IS_BIG_ENDIAN is %s", (ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_INFO_STR_FALSE:ACR_INFO_STR_TRUE);
        }
    }
    systemEndianValue = ACR_BYTE_ORDER_16(bigEndian);
    if(systemEndianValue != 0x0001)
    {
        // ACR_BYTE_ORDER_16 is not working properly
        ACR_DEBUG_PRINT(messageNumber+5, "ERROR: ACR_IS_BIG_ENDIAN is %s but ACR_BYTE_ORDER_16 did not work", (ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_INFO_STR_FALSE:ACR_INFO_STR_TRUE);
        return ACR_FAILURE;
    }
    systemEndianValue32 = ACR_BYTE_ORDER_32(bigEndian32);
    if(systemEndianValue32 != 0x00000001UL)
    {
        // ACR_BYTE_ORDER_32 is not working properly
        ACR_DEBUG_PRINT(messageNumber+6, "ERROR: ACR_IS_BIG_ENDIAN is %s but ACR_BYTE_ORDER_32 did not work", (ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_INFO_STR_FALSE:ACR_INFO_STR_TRUE);
        return ACR_FAILURE;
    }
    #if ACR_USE_64BIT == ACR_BOOL_TRUE
    systemEndianValue64 = ACR_BYTE_ORDER_32(bigEndian64);
    if(systemEndianValue32 != 0x0000000000000001ULL)
    {
        // ACR_BYTE_ORDER_64 is not working properly
        ACR_DEBUG_PRINT(messageNumber+7, "ERROR: ACR_IS_BIG_ENDIAN is %s but ACR_BYTE_ORDER_64 did not work", (ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_INFO_STR_FALSE:ACR_INFO_STR_TRUE);
        return ACR_FAILURE;
    }
    #endif

    //
    // BYTES
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 200;
    #endif
    byte += 1;
    if(byte != 0)
    {
        // ACR_MAX_BYTE value is incorrect
        ACR_DEBUG_PRINT(messageNumber+1, "ERROR: ACR_MAX_BYTE value is incorrect");
        return ACR_FAILURE;
    }
    else
    {
        ACR_DEBUG_PRINT(messageNumber+2, "OK: ACR_MAX_BYTE value is %u", ACR_MAX_BYTE);
    }

    //
    // MEMORY LENGTHS
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 300;
    #endif
    length += 1;
    if(length != 0)
    {
        // ACR_MAX_LENGTH value is incorrect
        ACR_DEBUG_PRINT(messageNumber+1, "ERROR: ACR_MAX_LENGTH value is incorrect");
        return ACR_FAILURE;
    }
    else
    {
        #if ACR_USE_64BIT == ACR_BOOL_TRUE
        ACR_DEBUG_PRINT(messageNumber+2, "OK: ACR_MAX_LENGTH value is %llu", ACR_MAX_LENGTH);
        #else
        ACR_DEBUG_PRINT(messageNumber+3, "OK: ACR_MAX_LENGTH value is %lu", ACR_MAX_LENGTH);
        #endif
    }

    //
    // COUNTS
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 400;
    #endif
    count += 1;
    if(count > ACR_ZERO_COUNT)
    {
        ACR_DEBUG_PRINT(messageNumber+2, "OK: ACR_MAX_COUNT value is %lu", ACR_MAX_COUNT);
    }
    else
    {
         // ACR_MAX_COUNT value is incorrect
        ACR_DEBUG_PRINT(messageNumber+1, "ERROR: ACR_MAX_COUNT value is incorrect");
        return ACR_FAILURE;
   }

    //
    // DATE AND TIME VALUES
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 500;
    #endif
    if(ACR_DayOfWeekFromString(dayOfWeekString) != ACR_DAY_TUESDAY)
    {
        // ACR_DayOfWeekToString() or ACR_DayOfWeekFromString() are not working
        // or someone didn't know that Freedom Friday is still on a Tuesday
        ACR_DEBUG_PRINT(messageNumber+1, "ERROR: ACR_DayOfWeekFromString(%.*s) did not return ACR_DAY_TUESDAY", (int)dayOfWeekString.m_Buffer.m_Length, (const char*)dayOfWeekString.m_Buffer.m_Pointer);
        return ACR_FAILURE;
    }
    else
    {
        ACR_DEBUG_PRINT(messageNumber+2, "OK: ACR_DayOfWeekFromString(%.*s) is ACR_DAY_TUESDAY", (int)dayOfWeekString.m_Buffer.m_Length, (const char*)dayOfWeekString.m_Buffer.m_Pointer);
    }

    if(ACR_MonthFromString(monthString) != ACR_MONTH_MARCH)
    {
        // ACR_MonthToString() or ACR_MonthFromString() are not working
        ACR_DEBUG_PRINT(messageNumber+3, "ERROR: ACR_MonthFromString(%.*s) did not return ACR_MONTH_MARCH", (int)monthString.m_Buffer.m_Length, (const char*)monthString.m_Buffer.m_Pointer);
        return ACR_FAILURE;
    }
    else
    {
        ACR_DEBUG_PRINT(messageNumber+4, "OK: ACR_MonthFromString(%.*s) is ACR_MONTH_MARCH", (int)monthString.m_Buffer.m_Length, (const char*)monthString.m_Buffer.m_Pointer);
    }

    ACR_DATETIME_NOW(dateTime);
    if(ACR_DATETIME_HAS_TIME(dateTime) == ACR_BOOL_FALSE)
    {
        if(ACR_DATETIME_HAS_DATE(dateTime) == ACR_BOOL_FALSE)
        {
            #if ACR_HAS_RTC == ACR_BOOL_TRUE
            ACR_DEBUG_PRINT(messageNumber+5, "ERROR: ACR_DATETIME_NOW did not provide time or date");
            return ACR_FAILURE;
            #else
            ACR_DEBUG_PRINT(messageNumber+6, "OK: No RTC so ACR_DATETIME_NOW did not provide time or date");
            #endif
        }
        else
        {
            ACR_DEBUG_PRINT(messageNumber+7, "ERROR: ACR_DATETIME_NOW did not provide time");
            return ACR_FAILURE;
        }
    }
    else
    {
        if(ACR_DATETIME_HAS_DATE(dateTime) == ACR_BOOL_FALSE)
        {
            ACR_DEBUG_PRINT(messageNumber+8, "ERROR: ACR_DATETIME_NOW did not provide date");
            return ACR_FAILURE;
        }
        else
        {
            monthString = ACR_MonthToString(ACR_DATETIME_MONTH(dateTime));
            dayOfWeekString = ACR_DayOfWeekToString(ACR_DATETIME_DAY_OF_WEEK(dateTime));
            ACR_DEBUG_PRINT(messageNumber+9, "OK: ACR_DATETIME_NOW has provided date and time of %.*s %.*s %d %d %d:%02d:%02d",
                (int)dayOfWeekString.m_Buffer.m_Length, (const char*)dayOfWeekString.m_Buffer.m_Pointer,
                (int)monthString.m_Buffer.m_Length, (const char*)monthString.m_Buffer.m_Pointer,
                ACR_DATETIME_DAY(dateTime), ACR_DATETIME_YEAR(dateTime),
                ACR_DATETIME_HOUR(dateTime), ACR_DATETIME_MIN(dateTime), ACR_DATETIME_SEC(dateTime));            
        }
    }

    //
    // SIMPLE MEMORY BUFFER
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 600;
    #endif
    if(ACR_BUFFER_GET_LENGTH(buffer) != 0)
    {
        // buffer length should have initialized to zero
        #if ACR_USE_64BIT == ACR_BOOL_TRUE
        ACR_DEBUG_PRINT(messageNumber+1, "ERROR: buffer length is %llu when it should be 0", buffer.m_Length);
        #else
        ACR_DEBUG_PRINT(messageNumber+2, "ERROR: buffer length is %lu when it should be 0", buffer.m_Length);
        #endif // #if ACR_USE_64BIT == ACR_BOOL_TRUE
        return ACR_FAILURE;
    }
    ACR_BUFFER_ALLOC(buffer, 1000);
    if(ACR_BUFFER_GET_LENGTH(buffer) == 0)
    {
        // failed to allocate the buffer
#ifdef ACR_COMPILER_VS2017
		#pragma warning(suppress: 4127)
#endif
        if(ACR_HAS_MALLOC != ACR_BOOL_FALSE)
        {
            ACR_DEBUG_PRINT(messageNumber+3, "ERROR: ACR_BUFFER_ALLOC failed to allocate memory");
            return ACR_FAILURE;
        }
        else
        {
            ACR_DEBUG_PRINT(messageNumber+4, "OK: ACR_HAS_MALLOC is not defined so buffer length is 0");
        }
    }
    else
    {
        #if ACR_USE_64BIT == ACR_BOOL_TRUE
        ACR_DEBUG_PRINT(messageNumber+5, "OK: ACR_BUFFER_ALLOC allocated %llu bytes", buffer.m_Length);
        #else
        ACR_DEBUG_PRINT(messageNumber+6, "OK: ACR_BUFFER_ALLOC allocated %lu bytes", buffer.m_Length);
        #endif // #if ACR_USE_64BIT == ACR_BOOL_TRUE
    }
    ACR_BUFFER_FREE(buffer);
    if(ACR_BUFFER_GET_LENGTH(buffer) != 0)
    {
        ACR_DEBUG_PRINT(messageNumber+7, "ERROR: ACR_BUFFER_FREE failed to free buffer memory");
        return ACR_FAILURE;
    }
    else
    {
        ACR_DEBUG_PRINT(messageNumber+8, "OK: ACR_BUFFER_FREE freed buffer memory");
    }
    
    //
    // DECIMAL VALUES
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 700;
    #endif
    if(ACR_DECIMAL_TOLERANCE_COMPARE(realNumber, (ACR_Decimal_t)5.2, (ACR_Decimal_t)0.0001) != ACR_INFO_EQUAL)
    {
        // 5.1999 and 5.2 should have been equal using the tolerance
        ACR_DEBUG_PRINT(messageNumber+1, "ERROR: ACR_DECIMAL_COMPARE should have found %0.4f to be equal to %0.4f", realNumber, 5.2);
        return ACR_FAILURE;
    }
    else
    {
        ACR_DEBUG_PRINT(messageNumber+2, "OK: ACR_DECIMAL_COMPARE found %0.4f is within %0.4f of %0.4f", realNumber, 0.0001, 5.2);
    }

    //
    // SIMPLE UTF8 STRINGS
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 800;
    #endif
    if(infoFromUp != ACR_INFO_UP)
    {
        // incorrect value returned from "up"
        ACR_DEBUG_PRINT(messageNumber+1, "ERROR: ACR_InfoFromString(%.*s) did not find ACR_INFO_UP", (int)stringForInfoUp.m_Buffer.m_Length, (const char*)stringForInfoUp.m_Buffer.m_Pointer);
        return ACR_FAILURE;
    }
    else
    {
        ACR_DEBUG_PRINT(messageNumber+2, "OK: %.*s is ACR_INFO_UP", (int)stringForInfoUp.m_Buffer.m_Length, (const char*)stringForInfoUp.m_Buffer.m_Pointer);
    }

    stringForEmojiSmile = ACR_StringFromMemory((ACR_Byte_t*)"Smile 🙂", ACR_MAX_LENGTH, ACR_MAX_COUNT);
    if(stringForEmojiSmile.m_Count != 7)
    {
        // incorrect character count for string with emoji smile
        ACR_DEBUG_PRINT(messageNumber+3, "ERROR: ACR_StringFromMemory() says the smile emoji is %lu characters", stringForEmojiSmile.m_Count);
        return ACR_FAILURE;
    }
    else
    {
        ACR_DEBUG_PRINT(messageNumber+4, "OK: %.*s with a single character smile emoji", (int)stringForEmojiSmile.m_Buffer.m_Length, (const char*)stringForEmojiSmile.m_Buffer.m_Pointer);
    }
    
    //
    // ALL TESTS COMPLETE
    //
    #if ACR_IS_DEBUG == ACR_BOOL_TRUE
    messageNumber = 900;
    #endif
    ACR_DEBUG_PRINT(messageNumber+1, "OK: All tests complete");
    return ACR_SUCCESS;
}

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - COMMON INFORMATIONAL VALUES
//
////////////////////////////////////////////////////////////

ACR_String_t ACR_InfoToString(
    ACR_Info_t info)
{
    return ACR_StringFromMemory((ACR_Byte_t*)g_ACRInfoStringLookup[info], ACR_MAX_LENGTH, ACR_MAX_COUNT);
}

ACR_Info_t ACR_InfoFromString(
    ACR_String_t src)
{
    ACR_Count_t i = 0;
    ACR_Info_t x;
    do
    {
        x = ACR_StringCompareToMemory(src, (const ACR_Byte_t*)g_ACRInfoStringLookup[i], ACR_MAX_LENGTH, ACR_MAX_COUNT, ACR_INFO_NO);
        if(x == ACR_INFO_EQUAL)
        {
            return (ACR_Info_t)i;
        }
        i++;
    }
    while(i < ACR_INFO_COUNT);
    return ACR_INFO_UNKNOWN;
}

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - DATE AND TIME VALUES
//
////////////////////////////////////////////////////////////

ACR_String_t ACR_DayOfWeekToString(
    ACR_DayOfWeek_t dayOfWeek)
{
    return ACR_StringFromMemory((ACR_Byte_t*)g_ACRDayOfWeekStringLookup[dayOfWeek], ACR_MAX_LENGTH, ACR_MAX_COUNT);
}

ACR_DayOfWeek_t ACR_DayOfWeekFromString(
    ACR_String_t src)
{
    ACR_Count_t i = 0;
    ACR_Info_t x;
    do
    {
        x = ACR_StringCompareToMemory(src, (const ACR_Byte_t*)g_ACRDayOfWeekStringLookup[i], ACR_MAX_LENGTH, ACR_MAX_COUNT, ACR_INFO_NO);
        if(x == ACR_INFO_EQUAL)
        {
            return (ACR_DayOfWeek_t)i;
        }
        i++;
    }
    while(i < ACR_INFO_COUNT);
    return ACR_DAY_OF_WEEK_UNKNOWN;
}

ACR_String_t ACR_MonthToString(
    ACR_Month_t month)
{
    return ACR_StringFromMemory((ACR_Byte_t*)g_ACRMonthStringLookup[month], ACR_MAX_LENGTH, ACR_MAX_COUNT);
}

ACR_Month_t ACR_MonthFromString(
    ACR_String_t src)
{
    ACR_Count_t i = 0;
    ACR_Info_t x;
    do
    {
        x = ACR_StringCompareToMemory(src, (const ACR_Byte_t*)g_ACRMonthStringLookup[i], ACR_MAX_LENGTH, ACR_MAX_COUNT, ACR_INFO_NO);
        if(x == ACR_INFO_EQUAL)
        {
            return (ACR_Month_t)i;
        }
        i++;
    }
    while(i < ACR_INFO_COUNT);
    return ACR_MONTH_UNKNOWN;
}

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - SIMPLE UTF8 STRINGS
//                    AND UNICODE CHARACTERS
//
////////////////////////////////////////////////////////////

ACR_Info_t ACR_Utf8NextChar(
        const ACR_Byte_t* mem,
        ACR_Length_t memLength,
        ACR_Length_t* pos)
{
    if((*pos) < memLength)
    {
        ACR_Byte_t c = mem[(*pos)];
        if(c != 0)
        {
            ACR_Length_t p = (*pos) + ACR_UTF8_BYTE_COUNT(c);
            if(p < memLength)
            {
                (*pos) = p;
                return ACR_INFO_OK;
            }
        }
        else
        {
            // already at end of string
        }
    }
    return ACR_INFO_ERROR;
}

ACR_Info_t ACR_Utf8PrevChar(
        const ACR_Byte_t* mem,
        ACR_Length_t memLength,
        ACR_Length_t* pos)
{
    if(((*pos) > 0) && ((*pos) <= memLength))
    {
        ACR_Byte_t c;
        (*pos) -= 1;
        c = mem[(*pos)];
        if((c & 0x80) == 0)
        {
            // [0xxxxxxx]
            return ACR_INFO_OK;
        }
        else
        {
            // [1xxxxxxx]
            while((c & 0x40) == 0)
            {
                // [?0xxxxxx]
                if((*pos) == 0)
                {
                    return ACR_INFO_ERROR;
                }
                (*pos) -= 1;
                c = mem[(*pos)];
            }
            // [?1xxxxxx]
            return ACR_INFO_OK;
        }
    }
    return ACR_INFO_ERROR;
}

ACR_Unicode_t ACR_UnicodeToLower(
    ACR_Unicode_t u)
{
    /// \todo use a conversion table instead to handle additional unicode characters
    
    // handle ASCII characters
    if((u >= 65) && (u <= 90))
    {
        u += 32;
    }

    return u;
}

ACR_Unicode_t ACR_UnicodeToUpper(
    ACR_Unicode_t u)
{
    /// \todo use a conversion table instead to handle additional unicode characters
    
    // handle ASCII characters
    if((u >= 97) && (u <= 122))
    {
        u -= 32;
    }

    return u;
}

ACR_Unicode_t ACR_Utf8ToUnicode(
    const ACR_Byte_t* mem,
    int bytes)
{
    int offset = (bytes-1);
    if(offset > 0)
    {
        ACR_Unicode_t c = (((ACR_Unicode_t)(mem[0] & (0x3F>>offset))) << (offset*6));
        int byteNum = 1;
        do
        {
            offset -= 1;
            c |= (((ACR_Unicode_t)(mem[byteNum] & 0x3F)) << (offset*6));
            byteNum += 1;
        }
        while(offset > 0);
        return c;
    }
    return (ACR_Unicode_t)mem[0];
}

ACR_String_t ACR_StringFromMemory(
    ACR_Byte_t* src,
    ACR_Length_t srcLength,
    ACR_Count_t maxCharacters)
{
    ACR_STRING(s);
    if(src != ACR_NULL)
    {
        ACR_Length_t remaining = srcLength;
        ACR_Byte_t* srcPtr = src;
        while((remaining > 0) && (s.m_Count < maxCharacters) && ((*srcPtr) != 0))
        {
			ACR_Length_t bytes = ACR_UTF8_BYTE_COUNT((*srcPtr));
            if(bytes > remaining)
            {
                break;
            }
            s.m_Count++;
            srcPtr += bytes;
            remaining -= bytes;
        }
        ACR_BUFFER_REFERENCE(s.m_Buffer, src, (srcLength - remaining));
    }
    return s;
}

ACR_Info_t ACR_StringCompareToMemory(
    ACR_String_t string,
    const ACR_Byte_t* src,
    ACR_Length_t srcLength,
    ACR_Count_t maxCharacters,
    ACR_Info_t caseSensitive)
{
    if(src != ACR_NULL)
    {
        ACR_Count_t count = 0;
        // 0 = string
        // 1 = src
		ACR_Byte_t* srcPtr[2];
		ACR_Length_t bytes[2];
        int strIndex;
        ACR_Unicode_t c[2];
        srcPtr[0] = (ACR_Byte_t*)string.m_Buffer.m_Pointer;
		srcPtr[1] = (ACR_Byte_t*)src;
        while((srcLength > 0) && ((srcPtr[1][0]) != 0))
        {
            if(count == maxCharacters)
            {
                // finished comparing the max number of characters
                return ACR_INFO_EQUAL;
            }
            // count the next character using UTF8 byte counts
            bytes[1] = ACR_UTF8_BYTE_COUNT(srcPtr[1][0]);
            if(bytes[1] > srcLength)
            {
                // src is malformed UTF8 at the last
                // character so the last character of
                // src will be ignored
                break;
            }
            count++;
            if(count > string.m_Count)
            {
                // src has more characters than string
                return ACR_INFO_GREATER;
            }
            bytes[0] = ACR_UTF8_BYTE_COUNT(srcPtr[0][0]);
            // create the utf8 characters to compare
            strIndex = 0;
            do
            {
                c[strIndex] = ACR_Utf8ToUnicode(srcPtr[strIndex], (int)bytes[strIndex]);
                strIndex++;
            }
            while(strIndex < 2);
            if(caseSensitive != ACR_INFO_YES)
            {
                c[0] = ACR_UnicodeToLower(c[0]);
                c[1] = ACR_UnicodeToLower(c[1]);
            }
            if(c[0] > c[1])
            {
                // src is a smaller value than string
                return ACR_INFO_LESS;
            }
            else if(c[0] < c[1])
            {
                // src is a greater value than string
                return ACR_INFO_GREATER;
            }
            srcPtr[0] += bytes[0];
            srcPtr[1] += bytes[1];
            srcLength -= bytes[1];
        }
        // no more src characters to count
        if(count < string.m_Count)
        {
            // src has less characters than string
            return ACR_INFO_LESS;
        }
        return ACR_INFO_EQUAL;
    }
    return ACR_INFO_INVALID;
}

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - OTHER
//
////////////////////////////////////////////////////////////
