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

#include "fxMath.h"
#include "fxUtils.h"

CHAR *AllocString(const CHAR *pValue)
{
    if (pValue)
    {
        CHAR *value = (CHAR *)MEM_ALLOC(strlen(pValue) + 1);
        strcpy(value, pValue);
        return value;
    }
    else
    {
        return NULL;
    }
}

CHAR *AllocMaxPath(const CHAR *pSource)
{
	CHAR *result;
	UINT len = MAX_PATH * sizeof(CHAR);

	result = (CHAR *)MEM_ALLOC(len);
	if (result != NULL)
	{
		memset(result, 0, len);
	}

	if (pSource != NULL)
	{
        FX_ASSERT(strlen(pSource) < MAX_PATH);
		strcpy(result, pSource);
	}

	return result;
}

VOID AppendSlash(CHAR *pPath)
{
	if (pPath)
    {
	    UINT len = strlen(pPath);
	    if ((len > 0) && (pPath[len-1] != PATH_SLASH_CHAR)) 
	    {
            CHAR pathSlash[2] = {0};
            pathSlash[0] = PATH_SLASH_CHAR;
		    strcat(pPath, pathSlash);
	    }
    }
}

VOID TrimSlash(CHAR *pPath)
{
	if (pPath)
    {
	    UINT len = strlen(pPath);
	    if ((len > 0) && (pPath[len-1] == PATH_SLASH_CHAR)) 
	    {
		    pPath[len-1] = '\0';
	    }
    }
}

UINT StrLenUtf8(const CHAR *pValue) 
{
    UINT i = 0, j = 0;

    if (pValue == NULL)
    {
        return 0;
    }
  
    while (pValue[i]) 
    {
        if ((pValue[i] & 0xc0) != 0x80)
        {
            j++;
        }

        i++;
    }

    return j;
}

CHAR *g_LogFileName = NULL;

VOID LogInitialize(const CHAR *pFileName)
{
    MEM_FREE(g_LogFileName);
    g_LogFileName = AllocString(pFileName);
}

VOID LogError(CHAR *pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);

    CHAR buffer[256];
    vsprintf(buffer, pFormat, args);
    strcat(buffer, "\r\n");

    FxOutputDebugString(buffer);

    if (g_LogFileName)
    {
        HANDLE FileHandle = FileOpen(g_LogFileName, "a");
        if (FileHandle)
        {
            FileWrite(FileHandle, buffer, strlen(buffer));
            FileClose(FileHandle);
        }
    }

    va_end(args);
}

BOOL IsWhiteSpace(CHAR Value)
{
    return (Value == '\n') || (Value == '\r') || (Value == ' ') || (Value == '\t');
}

VOID TrimString(CHAR *pValue)
{
    if (!pValue || !*pValue) 
    {
        return;
    }

    CHAR *tail = pValue + strlen(pValue) - 1;
    
    CHAR *head = pValue;
    while (head < tail)
    {
        if (IsWhiteSpace(*head))
        {
            head++;
        }
        else break;
    }

    while (tail > head)
    {
        if (IsWhiteSpace(*tail))
        {
            tail--;
        }
        else break;
    }
    tail++;

    CHAR *curr = pValue;
    while (head < tail)
    {
        *curr = *head;
        curr++;
        head++;
    }
    *curr = '\0';

    if (pValue && IsWhiteSpace(*pValue))
    {
        *pValue = '\0';
    }
}

VOID Swap(INT *A, INT *B)
{
    INT t = *A;
    *A = *B;
    *B = t;
}

VOID Swap(INT16 *A, INT16 *B)
{
    INT16 t = *A;
    *A = *B;
    *B = t;
}

VOID Swap(DOUBLE *A, DOUBLE *B)
{
    DOUBLE t = *A;
    *A = *B;
    *B = t;
}

BOOL IsRectEmpty(const FXRECT *pRect)
{
    return pRect->Right < pRect->Left || pRect->Bottom < pRect->Top;
}

BOOL IntersectRect(FXRECT *pDst, const FXRECT *pSrc1, const FXRECT *pSrc2)
{
    #define MMAX(a, b) ((a) > (b) ? (a) : (b))
    #define MMIN(a, b) ((a) < (b) ? (a) : (b))

    if ((pSrc1->Left > pSrc2->Right) || (pSrc2->Left > pSrc1->Right) ||
        (pSrc1->Top > pSrc2->Bottom) || (pSrc2->Top > pSrc1->Bottom))
    {
        memset(pDst, 0, sizeof(FXRECT));
        return FALSE;
    }

    pDst->Left   = MMAX(pSrc1->Left, pSrc2->Left );
    pDst->Right  = MMIN(pSrc1->Right, pSrc2->Right );
    pDst->Top    = MMAX(pSrc1->Top, pSrc2->Top );
    pDst->Bottom = MMIN(pSrc1->Bottom, pSrc2->Bottom );

    return TRUE;
}

VOID OffsetRect(FXRECT *pRect, INT Dx, INT Dy)
{
    pRect->Left += Dx;
    pRect->Top  += Dy;
    pRect->Right += Dx;
    pRect->Bottom += Dy;
}

VOID InflateRect(FXRECT *pRect, INT Dx, INT Dy)
{
    pRect->Left -= Dx;
    pRect->Top -= Dy;
    pRect->Right += Dx;
    pRect->Bottom += Dy;
}

BOOL PtInRect(const FXRECT *pRect, const FXPOINT *pPoint)
{
    return (pPoint->X >= pRect->Left) && (pPoint->X < pRect->Right) &&
           (pPoint->Y >= pRect->Top) && (pPoint->Y < pRect->Bottom);
}

BOOL PtInRect(const FXRECT *pRect, const INT X, const INT Y)
{
    return (X >= pRect->Left) && (X < pRect->Right) &&
           (Y >= pRect->Top) && (Y < pRect->Bottom);
}

BOOL CompareRect(const FXRECT *pRect1, const FXRECT *pRect2)
{
    return memcmp(pRect1, pRect2, sizeof(FXRECT)) == 0;
}

BOOL CompareGuid(const GUID *pGuid1, const GUID *pGuid2)
{
    return memcmp(pGuid1, pGuid2, sizeof(GUID)) == 0;
}

BOOL CompareGuid(const GUID Guid1, const GUID Guid2)
{
    return memcmp(&Guid1, &Guid2, sizeof(GUID)) == 0;
}

VOID InitGuid(GUID *pGuid)
{
    memset(pGuid, 0, sizeof(GUID));
}

VOID InitGuid(GUID *pDstGuid, const GUID *pSrcGuid)
{
    memmove(pDstGuid, pSrcGuid, sizeof(GUID));
}

BOOL IsNullGuid(GUID *pGuid)
{
    return CompareGuid(pGuid, &GUID_0);
}

BOOL CompareXGuid(const XGUID *pGuid1, const XGUID *pGuid2)
{
    return memcmp(pGuid1, pGuid2, sizeof(XGUID)) == 0;
}

VOID InitXGuid(XGUID *pGuid)
{
    memset(pGuid, 0, sizeof(XGUID));
}

BOOL IsNullXGuid(const XGUID *pGuid)
{
    // NULL_GUID is big enough to handle both GUID and UINT sizes of XGUID
    return (memcmp(pGuid, &GUID_0, sizeof(XGUID)) == 0);
}

GUID ConstructGuid(XGUID Id)
{
    GUID result = {0};
    memcpy(&result, &Id, sizeof(Id));
    return result;
}

VOID GetBitmapRect(FXBITMAPRESOURCE *Bitmap, UINT Width, UINT Height, BOOL Center, BOOL Stretch, BOOL Proportional, FXRECT *pTargetRect)
{
    INT w = Bitmap->Width;
    INT h = Bitmap->Height;
    INT cw = Width;
    INT ch = Height;

    FLOAT xyaspect;

    if (Stretch || (Proportional && ((w > cw) || (h > ch))))
    { 
        if (Proportional && (w > 0) && (h > 0))
        {
            xyaspect = (FLOAT)w / (FLOAT)h;
            if (w > h)
            {
                w = cw;
                h = (UINT)((FLOAT)cw / xyaspect);
                if (h > ch) // woops, too big
                {
                    h = ch;
                    w = (UINT)((FLOAT)ch * xyaspect);
                }
            }
            else
            {
                h = ch;
                w = (UINT)((FLOAT)ch * xyaspect);
                if (w > cw) // woops, too big
                {
                    w = cw;
                    h = (UINT)((FLOAT)cw / xyaspect);
                }
            }
        }
        else
        {
            w = cw;
            h = ch;
        }
    }

    pTargetRect->Left = 0;
    pTargetRect->Top = 0;
    pTargetRect->Right = max(0, (INT)w-1);
    pTargetRect->Bottom = max(0, (INT)h-1);

    if (Center)
    {
        OffsetRect(pTargetRect, (INT)((cw - w) / 2), (INT)((ch - h) / 2));
    }
}

VOID NumberToText(INT Value, BYTE Digits, BYTE Decimals, BYTE Fraction, BOOL Negative, CHAR *pOutValue, BOOL Spaces)
{
    INT i;
    if (Fraction > 1)
    {
        DOUBLE value = Value;
        for (i=0; i < Decimals; i++, value/=10) {}

        INT intPart = (INT)value;
        INT fraPart = (INT)((value - (DOUBLE)intPart) * Fraction);

        itoa(intPart, pOutValue, 10);

        CHAR fraText1[32];
        itoa(fraPart, fraText1, 10);

        CHAR fraText2[32];
        itoa(Fraction, fraText2, 10);

        if ((fraPart > 0) && (fraPart < Fraction))
        {
            strcat(pOutValue, " ");
            strcat(pOutValue, fraText1);
            strcat(pOutValue, "/");
            strcat(pOutValue, fraText2);
        }
    }
    else
    {
        INT divisor = 1;
        INT runningCount = 0;
        INT actualDigits = 0;
        INT v = Value;
        while (v > 0)
        {
            actualDigits++;
            v /= 10;
        }

        actualDigits = max(Digits, actualDigits);            

        CHAR digit[2] = "0";
        strcpy(pOutValue, "");

        for (i=0; i<actualDigits-1; i++, divisor*=10) {}

        for (i=0; i<actualDigits; i++)
        {
            INT value = (Value - runningCount) / divisor;
            
            runningCount += (value * divisor);
            divisor /= 10;

            if (Spaces && (i > 0))
            {
                strcat(pOutValue, " ");
            }

            digit[0] = '0' + (CHAR)value;
            strcat(pOutValue, digit);

            if ((i == actualDigits - Decimals - 1) && (i < actualDigits-1))
            {
                if (Spaces)
                {
                    strcat(pOutValue, " ");
                }
                strcat(pOutValue, ".");
            }
        }

        if (!Spaces)
        {
            // Skip leading zeros
            CHAR *p = pOutValue;
            for (i=0; i<(INT)strlen(pOutValue); i++)
            {
                if (pOutValue[i] != '0') break;
                p++;
            }

            // So we get "0."
            if (*p == '.')
            {
                p--;
            }

            if (strlen(p) == 0)
            {
                strcat(p, "0");
            }

            if (strchr(p, '.') == NULL)
            {
                strcat(p, ".");
            }
            
            strcpy(pOutValue, p);
        }
    }

    if (Negative)
    {
        CHAR *p = pOutValue + strlen(pOutValue) + 1;
        while (p > pOutValue)
        {
            *p = *(p-1);
            p--;
        }
        *p = '-';
    }
}

typedef BYTE DayTable[12];
const DayTable MonthDays[2] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
const UINT DateDelta   = 693594;
const UINT HoursPerDay = 24;
const UINT MinsPerHour = 60;
const UINT SecsPerMin  = 60;
const UINT MSecsPerSec = 1000;
const UINT MinsPerDay  = HoursPerDay * MinsPerHour;
const UINT SecsPerDay  = MinsPerDay * SecsPerMin;
const UINT MSecsPerDay = SecsPerDay * MSecsPerSec;


BOOL IsLeapYear(UINT Year)
{
    return (Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0));
}

DOUBLE EncodeDateTime(const FXDATETIME *pDateTime)
{
    const DayTable *dayTable = &MonthDays[IsLeapYear(pDateTime->Year)];

    if ((pDateTime->Year >= 1) && (pDateTime->Year <= 9999) && (pDateTime->Month >= 1) && (pDateTime->Month <= 12) &&
        (pDateTime->Day >= 1) && (pDateTime->Day <= (*dayTable)[pDateTime->Month-1]))
    {
        // Encode Date
        UINT day = pDateTime->Day;
        for (UINT i=1; i<pDateTime->Month; i++)
        {
            day = day + (*dayTable)[i-1];
        }

        UINT days = pDateTime->Year - 1;
        days = days * 365 + days/4 - days/100 + days/400 + day - DateDelta;
 
        // Encode time
		UINT16 h  = min(pDateTime->Hour, (UINT16)HoursPerDay-1);
		UINT16 m  = min(pDateTime->Minute, (UINT16)MinsPerHour-1);
		UINT16 s  = min(pDateTime->Second, (UINT16)SecsPerMin-1);
		UINT16 ms = min(pDateTime->Milliseconds, (UINT16)MSecsPerSec-1);

        return (DOUBLE)days + EncodeTime(h, m, s, ms);
    }
	else
	{
		//FX_ASSERT(FALSE);    // Bad dates just fail silently.
		return 0;
	}
}

DOUBLE EncodeTime(const UINT16 Hours, const UINT16 Minutes, const UINT16 Seconds, const UINT16 Milliseconds)
{
    return (DOUBLE)((UINT)Hours * (MinsPerHour * SecsPerMin * MSecsPerSec) + 
                    (UINT)Minutes * (SecsPerMin * MSecsPerSec) +
                    (UINT)Seconds * MSecsPerSec +
                    (UINT)Milliseconds) / (DOUBLE)MSecsPerDay;
}

VOID DecodeDate(DOUBLE EncodedDate, FXDATETIME *pDateTime)
{
    const DOUBLE DateDelta = 693594;

    memset(pDateTime, 0, sizeof(FXDATETIME));

    if (EncodedDate > -DateDelta)  
    {
        UINT di = (UINT)EncodedDate;
        DOUBLE j = (((DOUBLE)di + 693900) * 4) - 1;
        UINT ly = (UINT)(j / 146097);
        
        j = j - 146097 * ly;
        UINT ld = (UINT)(j / 4);
        j  = (ld * 4 + 3) / 1461;
        
        ld = (UINT)((ld * 4 + 7 - 1461 * j) / 4);
        DOUBLE lm = (5 * ld - 3) / 153;
        ld = (UINT)((5 * ld + 2 - 153 * lm) / 5);
        ly = 100 * ly + (UINT)j;
        if (lm < 10) 
        {
            lm += 3;
        }
        else
        {
            lm -= 9;
            ly ++;
        }
    
        pDateTime->Year = (UINT16)ly;
        pDateTime->Month = (UINT16)lm;
        pDateTime->Day = (UINT16)ld;
    }
}

VOID DecodeDateTime(DOUBLE EncodedDateTime, FXDATETIME *pDateTime)
{
    if (EncodedDateTime == 0.0)
    {
        memset(pDateTime, 0, sizeof(FXDATETIME));
        return;
    }

    DecodeDate(EncodedDateTime, pDateTime);

    UINT time;
    time = (UINT)((EncodedDateTime - (UINT)EncodedDateTime) * MSecsPerDay);

    UINT minCount, msecCount;
    minCount = time / (SecsPerMin * MSecsPerSec);
    msecCount = time % (SecsPerMin * MSecsPerSec);

    pDateTime->Hour = (UINT16)(minCount / MinsPerHour);
    pDateTime->Minute = (UINT16)(minCount % MinsPerHour);
    pDateTime->Second = (UINT16)(msecCount / MSecsPerSec);
    pDateTime->Milliseconds = (UINT16)(msecCount % MSecsPerSec);
}

DOUBLE CalcDistance(DOUBLE Lat1, DOUBLE Lon1, DOUBLE Lat2, DOUBLE Lon2)
{
    DOUBLE rLat1 = DEG2RAD(Lat1);
    DOUBLE rLon1 = DEG2RAD(Lon1);
    DOUBLE rLat2 = DEG2RAD(Lat2);
    DOUBLE rLon2 = DEG2RAD(Lon2);
    
    DOUBLE dLon = rLon2 - rLon1;
    DOUBLE dLat = rLat2 - rLat1;

    return acos(sin(rLat1) * sin(rLat2) + cos(rLat1) * cos(rLat2) * cos(dLon));
}

DOUBLE CalcDistanceKm(DOUBLE Lat1, DOUBLE Lon1, DOUBLE Lat2, DOUBLE Lon2)
{
    return 6378.7 * CalcDistance(Lat1, Lon1, Lat2, Lon2);
}

DOUBLE CalcDistanceMi(DOUBLE Lat1, DOUBLE Lon1, DOUBLE Lat2, DOUBLE Lon2)
{
    return 3963.0 * CalcDistance(Lat1, Lon1, Lat2, Lon2);
}

DOUBLE CalcDistanceNaMi(DOUBLE Lat1, DOUBLE Lon1, DOUBLE Lat2, DOUBLE Lon2)
{
    return 3437.74667 * CalcDistance(Lat1, Lon1, Lat2, Lon2);
}

INT CalcHeading(DOUBLE Lat1, DOUBLE Lon1, DOUBLE Lat2, DOUBLE Lon2)
{
    DOUBLE rLat1 = DEG2RAD(Lat1);
    DOUBLE rLon1 = DEG2RAD(Lon1);
    DOUBLE rLat2 = DEG2RAD(Lat2);
    DOUBLE rLon2 = DEG2RAD(Lon2);
    
    DOUBLE dLon = rLon2 - rLon1;
    DOUBLE dLat = rLat2 - rLat1;

    DOUBLE y = sin(dLon) * cos(rLat2);
    DOUBLE x = cos(rLat1) * sin(rLat2) - sin(rLat1) * cos(rLat2) * cos(dLon);

    INT heading;

    if (y > 0)
    {
        if (x > 0) 
        {
            heading = (INT)RAD2DEG(atan(y / x));
        }
        else if (x < 0) 
        {
            heading = 180 - (INT)RAD2DEG(atan(-y / x));
        }
        else if (x == 0) 
        {
            heading = 90;
        }
    }
    else if (y < 0)
    {
        if (x > 0)
        {
            heading = (INT)RAD2DEG(-atan(-y / x));
        }
        else if (x < 0)
        {
            heading = (INT)RAD2DEG(atan(y / x)) - 180;
        }
        else if (x == 0) 
        {
            heading = 270;
        }
    }
    else if (y == 0)
    {
        if (x > 0)
        {
            heading = 0;
        }
        else if (x < 0) 
        {
            heading = 180;
        }
        else if (x == 0)
        {
            heading = 0; //[the 2 points are the same]
        }
    }

    return (heading + 360) % 360;
}

DOUBLE KnotsToKmHour(DOUBLE Knots)
{
	return Knots * 1.88;
}

char *replace(const char *str, const char *pSrc, const char *pDst) 
{ 
    char *ret, *r; 
    const char *p, *q; 
    UINT srclen = strlen(pSrc); 
    UINT count, retlen, dstlen = strlen(pDst); 

    if (srclen != dstlen) 
    { 
        for (count = 0, p = str; (q = strstr(p, pSrc)) != NULL; p = q + srclen) 
        {
            count++; 
        }
        retlen = p - str + strlen(p) + count * (dstlen - srclen); 
    } 
    else 
    {
        retlen = strlen(str); 
    }

    ret = (CHAR *)MEM_ALLOC(retlen + 1); 
    if (ret == NULL)
    {
        return NULL;
    }

    for (r = ret, p = str; (q = strstr(p, pSrc)) != NULL; p = q + srclen) 
    { 
        UINT l = q - p; 
        memcpy(r, p, l); 
        r += l; 
        memcpy(r, pDst, dstlen); 
        r += dstlen; 
    } 

    strcpy(r, p); 

    return ret; 
} 

BOOL StringReplace(CHAR **pString, const CHAR *pFrom, const CHAR *pTo)
{
    if (*pString == NULL)
    {
        return TRUE;
    }

    CHAR *newStr;
    newStr = replace(*pString, pFrom, pTo);
    if (newStr == NULL)
    {
        return FALSE;
    }

    if (newStr == *pString)
    {
        return TRUE;
    }

    MEM_FREE(*pString);
    *pString = newStr;

    return TRUE;
}

BOOL TestGPS(FXGPS *pGPS, DOUBLE Accuracy)
{
    return (pGPS->State == dsConnected) && 
           (pGPS->Position.Quality != fqNone) &&
           (pGPS->Position.Accuracy <= Accuracy);
}

BOOL TestGPS(FXGPS_POSITION *pGPS, DOUBLE Accuracy)
{
    return (pGPS->Quality != fqNone) &&
           (pGPS->Accuracy <= Accuracy) &&
           (pGPS->Accuracy > 0);
}

DOUBLE EncodeDouble(INT IValue, INT IPoint, BOOL INegative, DOUBLE OutMin, DOUBLE OutMax)
{
    DOUBLE v = IValue;

    for (INT i=0; i<IPoint; i++, v/=10) {}

    v = max(OutMin, v);
    v = min(OutMax, v);

    return INegative ? -v : v;
}

VOID DecodeDouble(DOUBLE Value, BYTE Decimals, INT *pIValue, INT *pIPoint, BOOL *pINegative, BOOL Pack)
{
    INT v = 0;
    INT p = -1;
    INT i = 0;
    DOUBLE value = 0;

    if (Value == 0) 
    {
        goto Done;
    }

    value = FxAbs(Value);
    for (i=0; i<Decimals; i++, value*=10) {}
    value += 0.5;   // round properly

    v = (INT)value;
    p = Decimals;

    if (Pack && (p > 0))
    {
        while ((v > 0) && (v % 10 == 0))
        {
            v /= 10;
            p--;
        }
    }

    if (p == 0)
    {
        p = -1;
    }

Done:

    *pIValue = v;
    *pIPoint = p;
    *pINegative = (Value < 0);
}

VOID InitCOM(BYTE *pPortId, FX_COMPORT *pComPort)
{
    *pPortId = 1;
    pComPort->Baud = CBR_19200;
    pComPort->Parity = NOPARITY;
    pComPort->Data = DATABITS_8;
    pComPort->Stop = ONESTOPBIT;
    pComPort->FlowControl = FALSE;
}

CHAR *AllocGuidName(GUID *pGuid, CHAR *pExtension)
{
    CHAR *result = NULL;
    UINT len = 40 + ((pExtension != NULL) ? strlen(pExtension) : 0);

    result = (CHAR *)MEM_ALLOC(len);
    if (result == NULL)
    {
        goto cleanup;
    }

    sprintf(result, 
            "%08lx%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x%s",
            pGuid->Data1, pGuid->Data2, pGuid->Data3, 
            pGuid->Data4[0], pGuid->Data4[1], pGuid->Data4[2], pGuid->Data4[3], 
            pGuid->Data4[4], pGuid->Data4[5], pGuid->Data4[6], pGuid->Data4[7],
            pExtension ? pExtension : ""); 

cleanup:

    return result;
}

BOOL ClearDirectory(const CHAR *pPath)
{
    BOOL result = FALSE;

    FXFILELIST fileList;
    CHAR *path = AllocString(pPath);
    if (path == NULL)
    {
        LogError("Out of memory");
    	goto cleanup;
    }

    TrimSlash(path);

    if (FxBuildFileList(path, "*", &fileList))
    {
    	for (UINT i = 0; i < fileList.GetCount(); i++)
    	{
    		FxDeleteFile(fileList.GetPtr(i)->FullPath);
    	}
    }

    result = TRUE;

  cleanup:

    return result;
}

BOOL ChangeFileExtension(CHAR *pFileName, CHAR *pNewExtension)
{
    CHAR *pDot = strrchr(pFileName, '.');
    if (!pDot) return FALSE;
    {
        strcpy(pDot + 1, pNewExtension);
        return TRUE;
    }
}

CHAR *ExtractFileName(CHAR *pFullFileName)
{
    CHAR *pTemp = strrchr(pFullFileName, PATH_SLASH_CHAR);
    if (pTemp)
    {
        return pTemp + 1;
    }
    else
    {
        return NULL;
    }
}

UINT FileListGetTotalSize(FXFILELIST *pFileList)
{
    UINT result = 0;
    for (UINT i = 0; i < pFileList->GetCount(); i++)
    {
        FXFILE file = pFileList->Get(i);
        result += file.Size;
    }

    return result;
}

VOID FileListClear(FXFILELIST *pFileList)
{
    for (UINT i = 0; i < pFileList->GetCount(); i++)
    {
        FXFILE file = pFileList->Get(i);
        MEM_FREE(file.Name);
        MEM_FREE(file.FullPath);
        if (file.FileHandle != 0)
        {
            FileClose(file.FileHandle);
        }
    }

    pFileList->Clear();
}

BOOL FileListRemove(FXFILELIST *pFileList, CHAR *pFileName)
{
    BOOL Result = FALSE;

    for (UINT i = 0; i < pFileList->GetCount(); i++)
    {
        FXFILE file = pFileList->Get(i);

        if ((stricmp(pFileName, file.FullPath) == 0) ||
            (stricmp(pFileName, file.Name) == 0))
        {
            MEM_FREE(file.Name);
            MEM_FREE(file.FullPath);
            if (file.FileHandle != 0)
            {
                FileClose(file.FileHandle);
            }

            pFileList->Delete(i);

            Result = TRUE;
            break;
        }
    }

    return Result;
}

HANDLE FileOpen(const CHAR *pFilename, const CHAR *pMode)
{
	return (HANDLE)fopen(pFilename, pMode);
}

VOID FileClose(HANDLE Handle)
{
    if (Handle) 
    {
        fclose((FILE *)Handle);
    }
}

BOOL FileRead(HANDLE Handle, VOID *pBuffer, UINT Length)
{
    return fread(pBuffer, 1, Length, (FILE *)Handle) == Length;
}

BOOL FileSeek(HANDLE Handle, UINT Position)
{
    return fseek((FILE *)Handle, Position, SEEK_SET) == 0;
}

BOOL FileWrite(HANDLE Handle, VOID *pBuffer, UINT Length)
{
    return fwrite(pBuffer, 1, Length, (FILE *)Handle) == Length;
}

UINT FileGetSize(HANDLE Handle)
{
    UINT result = 0;
    FILE *f = (FILE *)Handle;
    long original = ftell(f);

    if (fseek(f, 0, SEEK_END) == 0)
    {
        result = ftell(f);
        fseek(f, original, SEEK_SET);
    }

    return result;
}

UINT FileGetLastError(HANDLE Handle)
{
    return ferror((FILE *)Handle);
}
