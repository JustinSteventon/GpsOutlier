/*************************************************************************************************/
//                                                                                                *
//   Copyright (C) 2005              Software                                                     *
//                                                                                                *
//   Original author: Justin Steventon (justin@steventon.com)                                     *
//                                                                                                *
//   You may retrieve the latest version of this file via the              Software home page,    *
//   located at: http://www.            .com.                                                     *
//                                                                                                *
//   The contents of this file are used subject to the Mozilla Public License Version 1.1         *
//   (the "License"); you may not use this file except in compliance with the License. You may    *
//   obtain a copy of the License from http://www.mozilla.org/MPL.                                *
//                                                                                                *
//   Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY  *
//   OF ANY KIND, either express or implied. See the License for the specific language governing  *
//   rights and limitations under the License.                                                    *
//                                                                                                *
//************************************************************************************************/

#pragma once

//#include "fxBrand.h"

// Headers.

#if defined(QTBUILD)
    #pragma warning( disable : 4100)    // unreferenced parameters
    #pragma warning( disable : 4189)    // variables set but not used
    #pragma warning( disable : 4267)    // size_t -> ulong

    #if defined(VSBUILD)
    #include <windows.h>
    #endif
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <math.h>
    #include <float.h>
    #include <string.h>
    #include <ctype.h>
    #include <stdarg.h>
    #include <time.h>

    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    #include <QFile.h>
    #include <QDir.h>
    #include <QDateTime.h>
    #include <QTimer.h>

    #undef UNALIGNED
    #define UNALIGNED

    #define INT64               qint64
    #define UINT64              quint64

    #define min(a,b)            (((a) < (b)) ? (a) : (b))
    #define max(a,b)            (((a) > (b)) ? (a) : (b))
    #if !defined(VSBUILD)
        #define stricmp(a, b)       strcasecmp(a, b)
        #define itoa(a, b, c)       sprintf(b, "%d" , (int)a)
        #define PATH_SLASH_CHAR     '\\'
    #else
        #define PATH_SLASH_CHAR     '\\'
        #define stricmp(a, b)       _stricmp(a, b)
        #define itoa(a, b, c)       sprintf(b, "%d" , (int)a)
        #pragma warning( disable : 4838)
        #pragma warning( disable : 4267)
        #pragma warning( disable : 4311)
        #pragma warning( disable : 4302)
        #pragma warning( disable : 4312)
    #endif

#elif defined(_WIN32)
    #pragma pack(4)
    #pragma warning(disable: 4103)
    #pragma warning(disable: 4996)
    #include <windows.h>
    #include <stdio.h>
    #include <math.h>
    #include <float.h>

    #define UINT64              unsigned __int64
    #define INT64               __int64

    #define BYTESWAP
    #define itoa(a, b, c)       _itoa(a, b, c)
    #define ultoa(a, b, c)      _ultoa(a, b, c)

    #define PATH_SLASH_CHAR     '\\'

    #if defined(_WIN32_WCE)
        #include <winnetwk.h>
        #include <projects.h>
        #define stricmp(a, b)       _stricmp(a, b)
        #define intptr_t			INT
    #endif

#elif defined(_ANDROID)
    #include <jni.h>
    #include <string.h>
    #include <time.h>
    #include <android/log.h>
    #include <android/bitmap.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <math.h>
    #include <float.h>
    #include <assert.h>
    #include <ctype.h>
    #include <inttypes.h>
    #include <string.h>
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <fnmatch.h>
    #include <dirent.h>

    #define INT64               int64_t
    #define UINT64              uint64_t 

    #define UNALIGNED
    #define min(a,b)            (((a) < (b)) ? (a) : (b))
    #define max(a,b)            (((a) > (b)) ? (a) : (b))
    #define stricmp(a, b)       strcasecmp(a, b)
    #define itoa(a, b, c)       sprintf(b, "%d" , (int)a) 
    #define PATH_SLASH_CHAR     '/'
#endif

//
// Baseline defines.
//

#undef TRUE
#define TRUE     (BYTE)1
#undef FALSE
#define FALSE    (BYTE)0   

#undef NULL
#define NULL     0

#define VOID     void
#define CHAR     char
#define BOOL     unsigned char
#define BOOLEAN  unsigned char
#define BYTE     unsigned char
#define UINT8    unsigned char
#define UINT16   unsigned short
#define UINT32   unsigned long  
#define UINT     unsigned long
#define INT8     char
#define INT16    short
#define INT32    long
#define INT      long
#define LONG     long
#define PCHAR    char *
#define INSTANCE void *
#define VOID     void
#define CHAR     char
#define DATE     double
#define TIME     double
#define DATETIME double
#define DOUBLE   double
#define COLOR    unsigned long
#define IEEE4    float
#define IEEE8    double

#define DBID     unsigned long

#define MAX_INT32           (2147483648.)   
#define INVALID_DBID        (0xFFFFFFFF)
#define INVALID_INDEX       (0xFFFFFFFF)
#define SIGN(X)             (((X) == 0) ? 0 : ((X) > 0) ? 1 : -1) 

#ifndef MAX_PATH
    #define MAX_PATH 256
#endif

#ifndef GUID_DEFINED
    #define GUID_DEFINED
    struct GUID 
    {
        UINT Data1;
        UINT16 Data2;
        UINT16 Data3;
        BYTE   Data4[8];
    };
#endif

#ifndef ARRAYSIZE
    #define ARRAYSIZE(X) (sizeof(X)/sizeof(X[0]))
#endif

#define FX_ASSERT(Condition)    if (!(Condition)) FxHardFault(__FILE__, __LINE__)

#define MEM_ALLOC(Size)         FxAlloc(Size)
#define MEM_REALLOC(Data, Size) FxReAlloc(Data, Size)
#undef MEM_FREE
#define MEM_FREE(Data)          if (Data) FxFree(Data)
#define MEM_SIZE(Data)          FxSize(Data)
#define MEM_CHECK(Data)         FxCheck(Data)

// NonAndroid
#if !defined(WIN32) || defined(QTBUILD)
    #define HANDLE				VOID *
    #define FLOAT				float
    #define DOUBLE				double
    #define WCHAR               UINT16

    #define CBR_110             110
    #define CBR_300             300
    #define CBR_600             600
    #define CBR_1200            1200
    #define CBR_2400            2400
    #define CBR_4800            4800
    #define CBR_9600            9600
    #define CBR_14400           14400
    #define CBR_19200           19200
    #define CBR_38400           38400
    #define CBR_56000           56000
    #define CBR_57600           57600
    #define CBR_115200          115200
    #define CBR_128000          128000
    #define CBR_256000          256000

    #define NOPARITY            0
    #define ODDPARITY           1
    #define EVENPARITY          2
    #define MARKPARITY          3
    #define SPACEPARITY         4

    #define ONESTOPBIT          0
    #define ONE5STOPBITS        1
    #define TWOSTOPBITS         2

    #if !defined(DATABITS_5)
        #define DATABITS_5          0x0001
        #define DATABITS_6          0x0002
        #define DATABITS_7          0x0004
        #define DATABITS_8          0x0008
        #define DATABITS_16         0x0010
        #define DATABITS_16X        0x0020
    #endif
#endif

#if defined(CLIENT_DLL)
    #define XGUID     GUID
    #define XFONT     FXFONT
    #define XSOUND    FXSOUND
    #define XBITMAP   FXBITMAP
    #define XBINARY   FXBINARY
    #define freeX(X)  MEM_FREE(X)

    #include <ClientDll.h>
    #define ICON(x)   IDB_CONTROL_##x
#else
    #define XGUID     UINT
    #define XFONT     UINT
    #define XSOUND    UINT
    #define XBITMAP   UINT
    #define XBINARY   UINT
    #define freeX(X)

    #define ICON(x)   0
#endif

VOID FxHardFault(CHAR *pFile, int Line);

VOID *FxAlloc(UINT Bytes);
VOID *FxReAlloc(VOID *pData, UINT Bytes);
VOID FxFree(VOID *pData);
UINT FxSize(VOID *pData);
BOOL FxCheck(VOID *pData);

#if defined(QTBUILD) && !defined(VSBUILD)
#undef _WIN32
#undef WIN32
#endif
