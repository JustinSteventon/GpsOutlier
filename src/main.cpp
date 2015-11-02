#include <QCoreApplication>
#include "fxNMEA.h"
#include "fxUtils.h"

#define ACCURACY_THRESHOLD 100
#define SPEED_THRESHOLD 100
#define DISTANCE_THRESHOLD 100

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

DOUBLE distance(FXGPS* gps1, FXGPS* gps2) {
    DOUBLE d1 = sqrt(pow(gps1->Position.Longitude,2) + pow(gps1->Position.Latitude,2));
    DOUBLE d2 = sqrt(pow(gps2->Position.Longitude,2) + pow(gps2->Position.Latitude,2));
    return fabs(d1-d2);
}

VOID AnalyzeData(TList<FXGPS*> &gpsList)
{
    // We need to evalate the dataset provided and see how well the algorithm works.
    //
    // 1. How often do we throw out good data
    // 2. Do we correctly throw out bad data, regardless of which reading it is?
    //

    GpsFilter gpsFilter;
    DOUBLE d[3] = {
                    distance(gpsList.Get(0), gpsList.Get(1)),
                    distance(gpsList.Get(1), gpsList.Get(2)),
                    distance(gpsList.Get(2), gpsList.Get(3))
                  };
    DOUBLE a[3] = {
                    gpsList.Get(0)->Position.Accuracy,
                    gpsList.Get(1)->Position.Accuracy,
                    gpsList.Get(2)->Position.Accuracy
                  };
    DOUBLE s[3] = {
                    gpsList.Get(0)->Speed,
                    gpsList.Get(1)->Speed,
                    gpsList.Get(2)->Speed
                  };



    for (UINT i = 3; i < gpsList.GetCount(); i++)
    {
        //Calc deltas
        DOUBLE dd = (d[0] + d[1] + d[2]) / 3;
        DOUBLE da = (a[0] + a[1] + a[2]) / 3;
        DOUBLE ds = (s[0] + s[1] + s[2]) / 3;

        FXGPS *gps = gpsList.Get(i);
        FXGPS *prevGps = gpsList.Get( i-1 );
        if ( /*gps->Position.Accuracy / ACCURACY_THRESHOLD > da  &&*/
             ds >= gps->Speed / SPEED_THRESHOLD &&
             dd >= distance(prevGps, gps) / DISTANCE_THRESHOLD) {
            qDebug(" dD=%f dA=%f dS=%f Lat=%f, Lon=%f, PDOP=%f, Speed=%f",dd, da, ds, gps->Position.Latitude, gps->Position.Longitude, gps->Position.Accuracy, gps->Speed);

            //shifting ahead
            d[0] = d[1];
            d[1] = d[2];
            d[2] = distance(prevGps,gps);

            s[0] = s[1];
            s[1] = s[2];
            s[2] = gps->Speed;

            a[0] = a[1];
            a[1] = a[2];
            a[2] = gps->Position.Accuracy;

        }else{
            qDebug("SKIPED: dD=%f dA=%f dS=%f Lat=%f, Lon=%f, PDOP=%f, Speed=%f",dd,da,ds, gps->Position.Latitude, gps->Position.Longitude, gps->Position.Accuracy, gps->Speed);
            d[0] = d[1];
            d[1] = d[2];
            d[2] = dd;

            s[0] = s[1];
            s[1] = s[2];
            s[2] = ds;

            a[0] = a[1];
            a[1] = a[2];
            a[2] = da;
        }



    }
}

int main(int argc, char *argv[])
{
    TList<FXGPS *> gpsList;
    if (!ParseNmeaFile("../data/Juno NMEA 2015-Sep-3 06-20-24PM.nmea", gpsList)) {
        return -1;
    }

    qDebug("Found %ld locations", gpsList.GetCount());

    AnalyzeData(gpsList);

    return 0;
}

