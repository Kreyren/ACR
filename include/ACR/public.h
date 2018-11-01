/*

    ********** DO NOT REMOVE THIS INFORMATION ************

    ACR - A set of C functions in a git Repository
    Copyright (C) 2018 Adam C. Rosenberg

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
/** \file public.h

    All enums and defines are found here. Some of the most
    common functions and typedefs will also be predefined
    by this header.

    Top uses:

    ACR_BYTE_ORDER_16   -   ensures a value is stored
                            in big endian byte order
    ACR_BYTE_ORDER_32   -   \see ACR_IS_BIG_ENDIAN

    ACR_Buffer_t        -   a simple struct with macros
                            to handle allocation and freeing
                            of memory safely.

    ACR_VarBuffer_t     -   a simple struct with macros to 
                            handle allocation of memory
                            safely while freeing memory only
                            when necessary to grow the
                            memory area.

    ACR_String_t        -   a struct for access to strings
                            with support for UTF8 encoding

    DECIMAL_COMPARE     -   compare decimal values within a
                            tolerance

*/
#ifndef _ACR_PUBLIC_H_
#define _ACR_PUBLIC_H_

// included for malloc and free
#include <stdlib.h>

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - SYSTEM LEVEL
//
////////////////////////////////////////////////////////////

/** represents a successful program or thread execution

    example:

    int main()
    {
        return ACR_SUCCESS;
    }
    
*/
#define ACR_SUCCESS 0

/** represents a false boolean result

    example:

    if(ACR_BOOL_FALSE)
    {
        // never reaches this code
    }
    else
    {
        // always reaches this code
    }

*/
#define ACR_BOOL_FALSE 0

/** represents a null pointer.
    use this instead of 0 to clearly indicate the value
    is being used for a null pointer

    for variable init:

        char* ptr = ACR_NULL;

    for comparison:
    
        if(ptr == ACR_NULL)
        {
            // ptr is a null pointer
        }

*/
#define ACR_NULL 0

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - ENDIANNESS
//
// Note: Big Endian means that multibyte values have the
//       bytes stored in memory from least significant 
//       to most significant.
//
// Optional: If you already know the endianess of your
//           system then set the preprocessor define as
//           follows to improve byte order handling
//           performance.
//           Big Endian:    ACR_IS_BIG_ENDIAN=1
//           Little Endian: ACR_IS_BIG_ENDIAN=0
//
// Sample memory:
//
//   type/bytes
//
//   char/1     memory = [ 0x01 ---- ---- ---- ]
//              binary little endian =
//                  MSB ------------------------00000001 LSB
//              decimal big endian = 1
//              decimal little endian = 1
//
//   short/2    memory = [ 0x01 0x00 ---- ---- ]
//              binary little endian = 
//                  MSB ----------------0000000100000000 LSB
//              decimal big endian = 1
//              decimal little endian = 256
//
//   short/2    memory = [ 0x00 0x01 ---- ---- ]
//              binary little endian = 
//                  MSB ----------------0000000000000001 LSB
//              decimal big endian = 256
//              decimal little endian = 1
//
//   long/4     memory = [ 0x00 0x01 0x00 0x00 ]
//              binary little endian = 
//                  MSB 00000000000000010000000000000000 LSB
//              decimal big endian = 256
//              decimal little endian = 65536
//
// "short" Example:
//
//      // given a local variable with value 1
//      short valueInSystemEndian = 1;
//      // convert the value to big endian 
//      short valueInBigEndian = ACR_BYTE_ORDER_16(valueInSystemEndian);
//      // and use the same macro to convert
//      // a big endian value back to system endianness
//      valueInSystemEndian = ACR_BYTE_ORDER_16(valueInBigEndian);
//
////////////////////////////////////////////////////////////

#ifndef ACR_IS_BIG_ENDIAN
#ifdef BIG_ENDIAN
#define ACR_IS_BIG_ENDIAN 1
#else
#ifdef LITTLE_ENDIAN
#define ACR_IS_BIG_ENDIAN 0
#else
#define ACR_IS_BIG_ENDIAN (*(unsigned short *)"\0\xff" < 0x100)
#define ACR_ENDIAN_DYNAMIC
#endif // #ifdef LITTLE_ENDIAN
#endif // #ifdef BIG_ENDIAN
#endif // #ifndef ACR_IS_BIG_ENDIAN

#define ACR_BYTE_ORDER_SWAP_16(x) (((((unsigned short)(x)) >> 8) & 0x00ff) | ((((unsigned short)(x)) & 0x00ff) << 8))
#define ACR_BYTE_ORDER_SWAP_32(x) ((((unsigned long)(x) >> 24) & 0x000000ff) | (((unsigned long)(x) >> 8) & 0x0000ff00) | (((unsigned long)(x) << 8) & 0x00ff0000) | (((unsigned long)(x) << 24) & 0xff000000))

#ifdef ACR_ENDIAN_DYNAMIC
#define ACR_BYTE_ORDER_16(x) ((ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_BYTE_ORDER_SWAP_16(x):x)
#define ACR_BYTE_ORDER_32(x) ((ACR_IS_BIG_ENDIAN == ACR_BOOL_FALSE)?ACR_BYTE_ORDER_SWAP_32(x):x)
#else
#if ACR_IS_BIG_ENDIAN == 0
#define ACR_BYTE_ORDER_16(x) (ACR_BYTE_ORDER_SWAP_16(x))
#define ACR_BYTE_ORDER_32(x) (ACR_BYTE_ORDER_SWAP_32(x))
#else
#define ACR_BYTE_ORDER_16(x) (x)
#define ACR_BYTE_ORDER_32(x) (x)
#endif
#endif

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - BYTES
//
////////////////////////////////////////////////////////////

/** type for a single byte
*/
typedef unsigned char ACR_Byte_t;

/** number of bits in ACR_Byte_t
*/
#define ACR_BITS_PER_BYTE 8

/** max value that can be stored by the ACR_Byte_t type 
*/
#define ACR_MAX_BYTE 255 // hex value 0xFF

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - MEMORY LENGTHS
//
////////////////////////////////////////////////////////////

/** type for a typical length
    \see ACR_MAX_LENGTH for the maximum value that can be
    stored by this data type
*/
typedef unsigned long ACR_Length_t;

/** represents zero length.
    use this instead of 0 to clearly indicate the value
    is being used for a length

    for variable init:

        int length = ACR_ZERO_LENGTH;

    for comparison:

        if(length == ACR_ZERO_LENGTH)
        {
            // length is zero
        }
    
*/
#define ACR_ZERO_LENGTH 0

/** max value that can be stored by the ACR_Length_t type 
*/
#define ACR_MAX_LENGTH 4294967295UL // hex value 0xFFFF

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - COUNTING
//
////////////////////////////////////////////////////////////

/** type for a typical count
    \see ACR_MAX_COUNT for the maximum value that can be
    stored by this data type
*/
typedef unsigned long ACR_Count_t;

/** max value that can be stored by the ACR_Count_t type 
*/
#define ACR_MAX_COUNT 4294967295UL // hex value 0xFFFF

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - COMMMON VALUES
//
////////////////////////////////////////////////////////////

/** represents an empty value.
    use this instead of 0 to clearly indicate the value
    is being used to identify an empty value

    for memory init:

        char data[64];
        memset(data, ACR_EMPTY_VALUE, sizeof(data));
        
    for variable init:

        int selected = ACR_EMPTY_VALUE;

    for comparison:

        if(data[0] == ACR_EMPTY_VALUE)
        {
            // nothing has been written to the data
        }

        if(selected == ACR_EMPTY_VALUE)
        {
            // nothing has been selected
        }

*/
#define ACR_EMPTY_VALUE 0

/** the value of pie
*/
#define ACR_PI 3.14159265359

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - TIME VALUES
//
////////////////////////////////////////////////////////////

#define ACR_MICRO_PER_MILLI 1000
#define ACR_MILLI_PER_SEC   1000
#define ACR_SEC_PER_MIN     60
#define ACR_MIN_PER_HOUR    60
#define ACR_HOUR_PER_DAY    24

// MINUTES
#define ACR_MIN_PER_DAY (ACR_MIN_PER_HOUR*ACR_HOUR_PER_DAY)
// SECONDS
#define ACR_SEC_PER_HOUR (ACR_SEC_PER_MIN*ACR_MIN_PER_HOUR)
#define ACR_SEC_PER_DAY (ACR_SEC_PER_HOUR*ACR_HOUR_PER_DAY)
// MILLISECONDS
#define ACR_MILLI_PER_MIN (ACR_MILLI_PER_SEC*ACR_SEC_PER_MIN)
#define ACR_MILLI_PER_HOUR (ACR_MILLI_PER_MIN*ACR_MIN_PER_HOUR)
// MICROSECONDS
#define ACR_MICRO_PER_SEC (ACR_MICRO_PER_MILLI*ACR_MILLI_PER_SEC)
#define ACR_MICRO_PER_MIN (ACR_MICRO_PER_SEC*ACR_SEC_PER_MIN)

/** days of the week
*/
typedef enum ACR_DayOfWeek_e
{
    ACR_DAY_SUNDAY = 0,
    ACR_DAY_MONDAY,
    ACR_DAY_TUESDAY,
    ACR_DAY_WEDNESDAY,
    ACR_DAY_THURSDAY,
    ACR_DAY_FRIDAY,
    ACR_DAY_SATURDAY,
    ACR_DAY_COUNT
} ACR_DayOfWeek_t;

#define ACR_DAY_OF_WEEK_UNKNOWN ACR_DAY_COUNT
#define ACR_DAY_PER_WEEK ACR_DAY_COUNT
#define ACR_MIN_PER_WEEK (ACR_MIN_PER_DAY*ACR_DAY_PER_WEEK)

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - SIMPLE MEMORY BUFFER
//
////////////////////////////////////////////////////////////

/** type for reference to a buffer.
    - never use malloc or free directly.
    - never use void pointers.
    - always use ACR_Buffer_t and \see ACR_BUFFER defines 
      for easy and safe usage

    example:

        int main()
        {
            ACR_BUFFER(text);
            ACR_BUFFER_ALLOC(text, 255);
            if(ACR_BUFFER_GET_LENGTH(text) > ACR_ZERO_LENGTH)
            {
                // this buffer is safe to use with
                // up to ACR_BUFFER_GET_LENGTH() bytes
            }
            ACR_BUFFER_FREE(text);
            return ACR_SUCCESS;
        }
*/
typedef struct ACR_Buffer_s
{
    /** points to a memory location or ACR_NULL
    */
    ACR_Byte_t* m_Pointer;

    /** length of the memory in bytes
        that m_Pointer points to
    */
    ACR_Length_t m_Length;

} ACR_Buffer_t;

/** define a buffer on the stack with the specified name
*/
#define ACR_BUFFER(name) ACR_Buffer_t name = {ACR_EMPTY_VALUE};

/** get the length of the buffer
*/
#define ACR_BUFFER_GET_LENGTH(name) name.m_Length

/** free memory used by the buffer
*/
#define ACR_BUFFER_FREE(name) if(name.m_Pointer != ACR_NULL){ free(name.m_Pointer); name.m_Pointer = ACR_NULL; } name.m_Length = ACR_ZERO_LENGTH;

/** assign memory to the buffer
    IMPORTANT: do not use ACR_BUFFER_FREE() after using
               ACR_BUFFER_REFERENCE()
*/
#define ACR_BUFFER_REFERENCE(name, memory, length) name.m_Pointer = (ACR_Byte_t*)memory; if(name.m_Pointer != ACR_NULL){ name.m_Length = length; }else{ name.m_Length = ACR_ZERO_LENGTH; };

/** allocate memory for the buffer
    IMPORTANT: do not use ACR_BUFFER_REFERENCE() after using
               ACR_BUFFER_ALLOC()
*/
#define ACR_BUFFER_ALLOC(name, length) if(name.m_Pointer != ACR_NULL){ free(name.m_Pointer); } name.m_Pointer = (ACR_Byte_t*)malloc(length); if(name.m_Pointer != ACR_NULL){ name.m_Length = length; }else{ name.m_Length = ACR_ZERO_LENGTH; };

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - VARIABLE LENGTH MEMORY BUFFER
//
////////////////////////////////////////////////////////////

/** type for reference to a buffer with variable length
    - never use malloc or free directly.
    - never use void pointers.
    - always use ACR_VarBuffer_t and \see ACR_VAR_BUFFER
      defines for easy and safe usage

    example:

        int main()
        {
            ACR_VAR_BUFFER(text);

            // example text length will never exceed 32 bytes
            ACR_VAR_BUFFER_ALLOC(text, 32);
            
            // just use 1 byte without changing allocated memory
            ACR_VAR_BUFFER_ALLOC(text, 1);
            if(ACR_VAR_BUFFER_GET_MAX_LENGTH(text) >= 1)
            {
                // this buffer is safe to use with up to ACR_VAR_BUFFER_GET_MAX_LENGTH() bytes
                // use ACR_VAR_BUFFER_GET_LENGTH() to get the number of bytes in the buffer
                // copy some data to the buffer
                text.m_Buffer.m_Pointer[0] = 'a';
            }

            // now use 2 bytes without changing allocated memory
            ACR_VAR_BUFFER_ALLOC(text, 2);
            if(ACR_VAR_BUFFER_GET_MAX_LENGTH(text) >= 2)
            {
                // this buffer is safe to use with up to ACR_VAR_BUFFER_GET_MAX_LENGTH() bytes
                // use ACR_VAR_BUFFER_GET_LENGTH() to get the number of bytes in the buffer
                // copy some data to the buffer
                text.m_Buffer.m_Pointer[0] = 'a';
                text.m_Buffer.m_Pointer[1] = 'b';
            }

            // use ACR_VAR_BUFFER_GET_LENGTH() to get the number of bytes in the buffer

            ACR_BUFFER_FREE(text);
            return ACR_SUCCESS;
        }
*/
typedef struct ACRVarBuffer_s
{
    ACR_Buffer_t m_Buffer;
    ACR_Length_t m_Length;
} ACR_VarBuffer_t;

/** define a variable sized buffer on the stack with the
    specified name
*/
#define ACR_VAR_BUFFER(name) ACR_VarBuffer_t name = {ACR_EMPTY_VALUE};

/** get the max length of the buffer
*/
#define ACR_VAR_BUFFER_GET_MAX_LENGTH(name) name.m_Buffer.m_Length

/** get the length of the buffer
*/
#define ACR_VAR_BUFFER_GET_LENGTH(name) name.m_Length

/** free memory used by the buffer
*/
#define ACR_VAR_BUFFER_FREE(name) if(name.m_Buffer.m_Pointer != ACR_NULL){ free(name.m_Buffer.m_Pointer); name.m_Buffer.m_Pointer = ACR_NULL; } name.m_Buffer.m_Length = ACR_ZERO_LENGTH; name.m_Length = ACR_ZERO_LENGTH;

/** allocate memory for the buffer only if needed
*/
#define ACR_VAR_BUFFER_ALLOC(name, length) if(name.m_Buffer.m_Length < length){ if(name.m_Buffer.m_Pointer != ACR_NULL){ free(name.m_Buffer.m_Pointer); } name.m_Buffer.m_Pointer = (ACR_Byte_t*)malloc(length); if(name.m_Buffer.m_Pointer != ACR_NULL){ name.m_Buffer.m_Length = length; }else{ name.m_Buffer.m_Length = ACR_ZERO_LENGTH; } name.m_Length = name.m_Buffer.m_Length; }else{ name.m_Length = length; };

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - COMMON INFORMATIONAL VALUES
//
////////////////////////////////////////////////////////////

/** info values that can be used instead of
    true or false for options and return values

    for variable init:

        ACR_Info_t result = ACR_INFO_UNKNOWN;

    for comparison:
        
        if(result == ACR_INFO_YES)
        {
            // the result is yes
        }
        else if(result == ACR_INFO_NO)
        {
            // the result is no
        }
        else
        {
            // an unexpected result
        }

    special case for boolean values

        only use ACR_INFO_TRUE and ACR_INFO_FALSE to check
        explicitly for those values. see notes on
        ACR_BOOL_FALSE for other boolean comparisons

        good example:

        ACR_Info_t success = ACR_INFO_FALSE;
        if(success == ACR_INFO_TRUE)
        {
            // success
        }
        else
        {
            // no success
        }

        bad example:

        ACR_Info_t success = ACR_INFO_FALSE;
        // Note: this is a bad example because this non-zero
        //       success value will be interpreted as "true"
        //       even though the intention was for it to be
        //       a false value
        if(success)
        {
            // success
        }
        else
        {
            // no success
        }

    special case for program or thread return value:

        use ACR_SUCCESS for successful execution
        and any other return codes can be used as
        an error.

        do not use 0, ACR_INFO_EQUAL, or ACR_INFO_OK as
        a program or thread return code because they are
        either confusing or will not have the intended
        results

        good example:        
        
        // program or thread exit with no error
        return ACR_SUCCESS;

        good example:

        // program or thread exit error
        return ACR_INFO_ERROR;
        
        bad example:

        // program or thread exit with no error
        // Note: this is a bad example because this non-zero
        //       return value will be interpreted as an
        //       error
        return ACR_INFO_OK;

*/
typedef enum ACR_Info_e
{
    //
    // COMPARISON
    //
    ACR_INFO_EQUAL = 0,
    ACR_INFO_NOT_EQUAL,
    ACR_INFO_LESS,
    ACR_INFO_GREATER,
    ACR_INFO_UNKNOWN,

    //
    // POSITIVE,       NEGATIVE
    //
    ACR_INFO_TRUE,     ACR_INFO_FALSE,
    ACR_INFO_OK,       ACR_INFO_ERROR,
    ACR_INFO_ENABLED,  ACR_INFO_DISABLED,
    ACR_INFO_YES,      ACR_INFO_NO,
    ACR_INFO_ON,       ACR_INFO_OFF,
    ACR_INFO_START,    ACR_INFO_STOP,
    ACR_INFO_ACCEPT,   ACR_INFO_IGNORE,
    ACR_INFO_VALID,    ACR_INFO_INVALID,

    //
    // POSITION
    //
    ACR_INFO_TOP,      ACR_INFO_BOTTOM,
    ACR_INFO_FIRST,    ACR_INFO_LAST,
    ACR_INFO_PREVIOUS, ACR_INFO_NEXT,
    ACR_INFO_CURRENT,

    // DIRECTION
    ACR_INFO_UP,       ACR_INFO_DOWN,
    ACR_INFO_LEFT,     ACR_INFO_RIGHT,

    // AGE
    ACR_INFO_OLD,      ACR_INFO_NEW,

    // INSTRUCTION
    ACR_INFO_BEGIN,
    ACR_INFO_READY,
    ACR_INFO_WAIT,
    ACR_INFO_GO,
    ACR_INFO_END,

    ACR_INFO_COUNT

} ACR_Info_t;

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - DECIMAL VALUES
//
////////////////////////////////////////////////////////////

/** stores a decimal value
*/
typedef float ACR_Decimal_t;

/** default tolerance for comparison of decimal values
    \see ACR_DecimalCompare()
*/
#define ACR_DEFAULT_TOLERANCE 0.0001

/** compare two decimal values
    \param a
    \param b
    \param t the absolute value of the difference
            between a and b where they will be assumed
            to be equal values. use ACR_DEFAULT_TOLERANCE
            if unsure
    \returns - ACR_INFO_EQUAL if the decimals are equal
               within the specified tolerance (t)
             - ACR_INFO_LESS if a is less than b
             - ACR_INFO_GREATER a is greater than b
*/
#define ACR_DECIMAL_TOLERANCE_COMPARE(a,b,t) (((a-t)>b)?ACR_INFO_GREATER:((a+t)<b)?ACR_INFO_LESS:ACR_INFO_EQUAL)
#define ACR_DECIMAL_COMPARE(a,b) (ACR_DECIMAL_TOLERANCE_COMPARE(a,b,ACR_DEFAULT_TOLERANCE))

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - SIMPLE FORMULAS
//
////////////////////////////////////////////////////////////

/** the circumference of a circle with the specified radius
*/
#define ACR_CIRCUMFERENCE(radius) (2*ACR_PI*radius)

////////////////////////////////////////////////////////////
//
// TYPES AND DEFINES - SIMPLE UTF8 STRINGS 
//                     AND UNICODE CHARACTERS
//
////////////////////////////////////////////////////////////

/** type for string data
*/
typedef struct ACR_String_s
{
    ACR_Buffer_t m_Buffer;
    ACR_Count_t m_Count;
} ACR_String_t;

/** define a string on the stack with the specified name
*/
#define ACR_STRING(name) ACR_String_t name = {ACR_EMPTY_VALUE};

/** type for unicode characters
*/
typedef unsigned long ACR_Unicode_t;

/** given the first UTF8 byte value determine the number of
    bytes uses to encode the character
*/
#define ACR_UTF8_BYTE_COUNT(c) (((c & 0x80) != 0)?((c & 0x40) != 0)?((c & 0x20) != 0)?((c & 0x10) != 0)?4:3:2:1:1)

/////////////////////////////////////////////////////////
//                                                     //
// TYPES AND DEFINES - UNIQUE STRING VALUES            //
//                                                     //
// note: keep these in alphabetical order so that it   //
//       is easy to determine which have been used     //
//       and no duplicates will be created by accident //
//                                                     //
/////////////////////////////////////////////////////////

// *** A ***
#define ACR_INFO_STR_ACCEPT "accept"

// *** B ***
#define ACR_INFO_STR_BEGIN "begin"
#define ACR_INFO_STR_BOTTOM "bottom"

// *** C ***
#define ACR_INFO_STR_CURRENT "current"

// *** D ***
#define ACR_INFO_STR_DISABLED "disabled"
#define ACR_INFO_STR_DOWN "down"

// *** E ***
#define ACR_INFO_STR_ENABLED "enabled"
#define ACR_INFO_STR_END "end"
#define ACR_INFO_STR_EQUAL "equal"
#define ACR_INFO_STR_ERROR "error"

// *** F ***
#define ACR_INFO_STR_FALSE "false"
#define ACR_INFO_STR_FIRST "first"
#define ACR_DAY_STR_FRIDAY "fri"

// *** G ***
#define ACR_INFO_STR_GO "go"
#define ACR_INFO_STR_GREATER "greater"

// *** H ***

// *** I ***
#define ACR_INFO_STR_IGNORE "ignore"
#define ACR_INFO_STR_INVALID "invalid"

// *** J ***

// *** K ***

// *** L ***
#define ACR_INFO_STR_LAST "last"
#define ACR_INFO_STR_LEFT "left"
#define ACR_INFO_STR_LESS "less"

// *** M ***
#define ACR_DAY_STR_MONDAY "mon"

// *** N ***
#define ACR_INFO_STR_NEW "new"
#define ACR_INFO_STR_NEXT "next"
#define ACR_INFO_STR_NO "no"
#define ACR_INFO_STR_NOT_EQUAL "not_equal"

// *** O ***
#define ACR_INFO_STR_OFF "off"
#define ACR_INFO_STR_OK "ok"
#define ACR_INFO_STR_OLD "old"
#define ACR_INFO_STR_ON "on"

// *** P ***
#define ACR_INFO_STR_PREVIOUS "previous"

// *** Q ***

// *** R ***
#define ACR_INFO_STR_READY "ready"
#define ACR_INFO_STR_RIGHT "right"

// *** S ***
#define ACR_DAY_STR_SATURDAY "sat"
#define ACR_INFO_STR_START "start"
#define ACR_INFO_STR_STOP "stop"
#define ACR_DAY_STR_SUNDAY "sun"

// *** T ***
#define ACR_DAY_STR_THURSDAY "thurs"
#define ACR_INFO_STR_TOP "top"
#define ACR_INFO_STR_TRUE "true"
#define ACR_DAY_STR_TUESDAY "tues"

// *** U ***
#define ACR_INFO_STR_UNKNOWN "unknown"
#define ACR_INFO_STR_UP "up"

// *** V ***
#define ACR_INFO_STR_VALID "valid"

// *** W ***
#define ACR_INFO_STR_WAIT "wait"
#define ACR_DAY_STR_WEDNESDAY "wed"

// *** X ***
// *** Y ***
#define ACR_INFO_STR_YES "yes"

// *** Z ***

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - TEST
//
////////////////////////////////////////////////////////////

/** internal test of ACR library functions
    \returns ACR_INFO_OK or ACR_INFO_ERROR
*/
ACR_Info_t ACR_Test(void);

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - COMMON INFORMATIONAL VALUES
//
////////////////////////////////////////////////////////////

/** get the info value as a string
    \param info the info value \see enum ACR_Info_e
    \returns a reference to a string
*/
ACR_String_t ACR_InfoToString(
    ACR_Info_t info);

/** get the info value from a string
    \param src a reference to a string
    \returns an info value from \see enum ACR_Info_e or
             ACR_INFO_UNKNOWN if not found
*/
ACR_Info_t ACR_InfoFromString(
    ACR_String_t src);

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - TIME VALUES
//
////////////////////////////////////////////////////////////

/** get the day of week value as a string
    \param dayOfWeek the day of week value
           \see enum ACR_DayOfWeek_e
    \returns a reference to a string
*/
ACR_String_t ACR_DayOfWeekToString(
    ACR_DayOfWeek_t dayOfWeek);

/** get the day of week value from a string
    \param src a reference to a string
    \returns an info value from \see enum ACR_DayOfWeek_e or
             ACR_DAY_OF_WEEK_UNKNOWN if not found
*/
ACR_DayOfWeek_t ACR_DayOfWeekFromString(
    ACR_String_t src);

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - SIMPLE UTF8 STRINGS
//                    AND UNICODE CHARACTERS
//
////////////////////////////////////////////////////////////

/** convert a unicode charcter to its lower-case
    representation
*/
ACR_Unicode_t ACR_UnicodeToLower(
    ACR_Unicode_t u);

/** convert UTF8 encoded data to a unicode value
    \param mem a valid pointer to the UTF8 encoded character
           in memory
    \param bytes the number of bytes for this UTF8 encoded
           unicode value. use ACR_UTF8_BYTE_COUNT(mem[0])
           if unsure
    \returns the unicode value
*/
ACR_Unicode_t ACR_Utf8ToUnicode(
    ACR_Byte_t* mem,
    int bytes);

/** get a reference to a null-terminated string in memory 
    with support for UTF8 encoding
    \param src a pointer to a null-terminated string in
           memory
    \param srcLength the max number of bytes from src this
           function will search. use ACR_MAX_LENGTH if
           unsure
    \param maxCharacters the max number of characters this
           function will count. use ACR_MAX_COUNT if unsure
    \returns a string referennce
*/
ACR_String_t ACR_StringFromMemory(
    void* src,
    ACR_Length_t srcLength,
    ACR_Count_t maxCharacters);

/** compare a string to a null-terminated string in memory
    with support for UTF8 encoding
    \param string the known string to compare to
    \param src a pointer to a null-terminated string in
           memory
    \param srcLength the max number of bytes from mem this
           function will search. use ACR_MAX_LENGTH if 
           unsure
    \param maxCharacters the max number of characters this
           function will compare. use ACR_MAX_COUNT if 
           unsure
    \param caseSensitive set to ACR_INFO_YES for 
           case-sensitive comparison (faster) or ACR_INFO_NO
           for case-insensitive (slower)
    \returns - ACR_INFO_EQUAL if the strings are equal up
               to maxCharacters
             - ACR_INFO_LESS if src has less characters or
               is less than string
             - ACR_INFO_GREATER if src has more characters
               or is greater than string
             - ACR_INFO_INVALID if src is invalid
*/
ACR_Info_t ACR_StringCompareToMemory(
    ACR_String_t string,
    void* src,
    ACR_Length_t srcLength,
    ACR_Count_t maxCharacters,
    ACR_Info_t caseSensitive);

////////////////////////////////////////////////////////////
//
// PUBLIC FUNCTIONS - OTHER
//
////////////////////////////////////////////////////////////


#endif
