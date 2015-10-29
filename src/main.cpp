#include <QCoreApplication>
#include "fxNMEA.h"
#include "fxUtils.h"

class GpsFilter
{
    UINT _minAccuracy, _minReadings, _maxSpeed;
    FXGPS_POSITION _position;
public:
   GpsFilter()
   {
       Reset();
   }

   ~GpsFilter()
   {

   }

   VOID Reset()
   {
       _minAccuracy = 50;   // 0 = Perfect, 50 = Max (see PDOP)
       _minReadings = 1;    // Minimum number of readings required
       _maxSpeed = 1000;    // Maximum speed or reading is bad
       memset(&_position, 0, sizeof(FXGPS_POSITION));
   }

   VOID Configure(UINT minReadings, UINT minAccuracy, UINT maxSpeed)
   {
       Reset();
       _minReadings = minReadings;
       _minAccuracy = minAccuracy;
       _maxSpeed = maxSpeed;
   }

   VOID AddGPS(FXGPS *gps)
   {
       // TODO:
       // GPS reading is received from the system

       // apply accuracy filter
       // apply speed filter
       // Average GPS reading using Kalman filter or some other mechanism
       //
   }

   BOOL GetGPS(FXGPS_POSITION *pPosition)
   {
       // TODO:
       // Returns TRUE + GPS position if criteria are met
       // Returns FALSE otherwise
       *pPosition = _position;
       return FALSE;
   }
};

bool ParseNmeaFile(CHAR *fileName, TList<FXGPS*> &gpsList)
{
    FXFILEMAP fileMap = {0};

    if (!FxMapFile(fileName, &fileMap))
    {
        qDebug("Failed to map file %s", fileName);
        return FALSE;
    }

    CfxNMEAParser nmeaParser;

    for (UINT i = 0; i < fileMap.FileSize; i++)
    {
        nmeaParser.ParseBuffer((CHAR *)fileMap.BasePointer + i, 1);

        if ((nmeaParser._ggaCount >= 1) &&
            (nmeaParser._gsaCount >= 1) &&
            (nmeaParser._gsvCount >= 3) &&
            (nmeaParser._rmcCount >= 1) &&
            (nmeaParser._zdaCount >= 1))
        {

            FXGPS *gps = (FXGPS *)MEM_ALLOC(sizeof(FXGPS));
            memset(gps, 0, sizeof(FXGPS));
            nmeaParser.GetGPS(gps, 5000);
            if (TestGPS(gps))
            {
                gpsList.Add(gps);
            }

            nmeaParser.Reset();
        }
    }

    FxUnmapFile(&fileMap);

    return TRUE;
}

VOID AnalyzeData(TList<FXGPS*> &gpsList)
{
    // We need to evalate the dataset provided and see how well the algorithm works.
    //
    // 1. How often do we throw out good data
    // 2. Do we correctly throw out bad data, regardless of which reading it is?
    //

    GpsFilter gpsFilter;

    for (UINT i = 0; i < gpsList.GetCount(); i++)
    {
        FXGPS *gps = gpsList.Get(i);
        qDebug("Lat=%f, Lon=%f, PDOP=%f, Speed=%f", gps->Position.Latitude, gps->Position.Longitude, gps->Position.Accuracy, gps->Speed);
    }
}

int main(int argc, char *argv[])
{
    TList<FXGPS *> gpsList;
    if (!ParseNmeaFile("e:\\github\\GpsOutlier\\data\\Juno NMEA 2015-Sep-3 06-20-24PM.nmea", gpsList)) {
        return -1;
    }

    qDebug("Found %ld locations", gpsList.GetCount());

    AnalyzeData(gpsList);

    return 0;
}

