/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

/*
 *    See section 4.5.3 of the User's Guide for database design and
 *    application requirements.
*/

/*
 * This example will show you how to manage 2 dimensional date, in our case fictive points of interest
 * in a map, efficiently using a 1 dimensional index. To do this we’ll be mapping the longitude, latitude
 * of our points into what we define as a Z-value. Since out longitude and latitude values are 32 bit integers
 * the Z-value will be 64 bit’s wide. Our example will only store the 64 bit Z-Value in the database since
 * both the longitude and latitude can be derived from this value. The coding is a simple interleaving of
 * bits from the longitude and latitude.
 *
 * Looking at these three values bit pattern, the interleaving can be explained by:
 * Longitude = x32 x31 x30 … x1
 * Latitude  = y32 y31 y30 … y1
 * Z-Value   = x32 y32 x31 y31 x30 y30 … x1 y1
 *
 * Please visit our examples guide in the documentation set for a comprehensive explanation of this example.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rdm.h"
#include "poi.h"

#define MAP_SIZE  100000

typedef int32_t (*callback)(DB_TASK *);

/* Global Variables */ /*lint -save -e956 */
static uint32_t uPointX;       /* Used in the nearest neighbor reqursive query. */
static uint32_t uPointY;       /* Used in the nearest neighbor reqursive query. */
static double   fDistance;     /* Used in the nearest neighbor reqursive query. */
static DB_ADDR  dbaNeighbor;   /* Used in the nearest neighbor reqursive query. */ /*lint -restore */

extern uint32_t decodeX(uint64_t);               /* Decode X coordinate (longitude) from the interleaved Z-value */
extern uint32_t decodeY(uint64_t);               /* Decode Y coordinate (latitude) from the interleaved Z-value */
extern uint64_t interleave(uint32_t, uint32_t);  /* Interleave X coordinate (longitude) and Y coordinate (latitude) into Z-Value. */
extern void     rectangle_query(uint32_t, uint32_t, uint32_t, uint32_t, callback, DB_TASK *); /* The query function. */

/* Forward Declarations */
void getstring(char *s, int32_t size);

/* Turn an integer between 0 and 720,000,000 into Latitude
*/
static double denormalizeLatitude(
    uint32_t lat)
{
    double dLat;

    /* original values from -90 through 90 */

    dLat = lat / 4000000.0;

    dLat -= 90.0;

    return dLat;
}

/* Turn an integer between 0 and 720,000,000 into Longitude
*/
static double denormalizeLongitude(
    uint32_t lon)
{
    double dLon;

    /* original values from -180 through 180 */

    dLon = lon / 2000000.0;

    if (dLon > 180.0)
        dLon -= 360.0;

    return dLon;
}

static void print_poi(
    struct pointsofinterest *poi)
{
    printf("%s, %s %05d (%6.3lf, %6.3lf)\n", poi->city, poi->state, poi->zip,
            denormalizeLatitude(decodeY(poi->zvalue)),
            denormalizeLongitude(decodeX(poi->zvalue)));
}

/* Print out the cordinates for a point.

   The cordinates for the point will be printed using the last key
   found.

   This is a callback function used by the points_in_rectangle
*/
static int32_t print_point(
    DB_TASK *task)
{
    int32_t iStatus;

    struct pointsofinterest poi;

    iStatus = d_recread(&poi, task, CURR_DB);
    if (iStatus != S_OKAY)
        return iStatus;

    print_poi(&poi);

    return S_OKAY;
}

/* Calculate the distance in miles between two points.
 * Note that this is just an approximation, but a fairly close one.
*/
static double miles_between(
    double x1,
    double y1,
    double x2,
    double y2)
{ /*lint !e578 */
    double avgLat;
    double x;
    double y;
    double absX;
    double absY;

    /* The size of one longitude degree decreases as latitude moved north, so
     * use the average of the latitude to adjust the longitude distance.
   */
    avgLat = (y1 + y2) / 2.0;

    absX = x2 - x1;
    if (absX < 0.0)
        absX = -absX;

    absY = y2 - y1;
    if (absY < 0.0)
        absY = -absY;

    /* NOTES: 69.171 is the earth's circumference at the equator divided by
     *        360 degrees.  69.055 comes from earth's circumference around
     *        the poles.  0.017453292 is pi/180, the conversion from degrees
     *        to radians.
    */
    x = absX * 69.171 * cos(avgLat * 0.017453292);

    y = absY * 69.055;

    return sqrt(pow(x, 2.0) + pow(y, 2.0));
}

/* Calculate nearest neigbor.

   The cordinates for the point will be using the last key
   found.

   This is a callback function used by the points_in_rectangle
*/
static int32_t neighbor_calculation(
    DB_TASK *task)
{
    int32_t  iStatus;
    uint64_t uZvalue;
    double   fD;
    double   xVal;
    double   yVal;

    iStatus = d_keyread(&uZvalue, task);

    if (iStatus == S_OKAY)
    {
        xVal = denormalizeLongitude(decodeX(uZvalue));
        yVal = denormalizeLatitude(decodeY(uZvalue));
        fD = miles_between(denormalizeLongitude(uPointX),
                denormalizeLatitude(uPointY), xVal, yVal);

        fDistance = fD;
        iStatus = d_crget(&dbaNeighbor, task, CURR_DB);
    }

    return iStatus;
}

/* Turn Latitude into an integer between 0 and 720,000,000
*/
static uint32_t normalizeLatitude(
    double lat)
{
    /* original values from -90 through 90 */

    /* make from 0 to 180 */
    lat += 90.0;

    /* include digits after decimal point */
    lat *= 4000000.0;

    return (uint32_t) lat;
}

/* Turn Longitude into an integer between 0 and 720,000,000
*/
static uint32_t normalizeLongitude(
    double lon)
{
    /* original values from -180 through 180 */

    /* make from 0 to 360 */
    /* negative values are actually greater than positive values as we start
     * at 0.  Values go like this:
     * 0, 1, 2, ..., 179, 180 (-180), -179, -178, ..., -2, -1
     * Longitude 180 and longitude -180 are the same thing */
    if (lon < 0.0)
        lon += 360.0;

    /* include digits after decimal point */
    lon *= 2000000.0;

    return (uint32_t) lon;
}

/* Get the next element in a comma delimited line of text
*/
static const char *getNextElement(
    char **line)
{
    const char *start;
    char       *end;

    if (*line == NULL)
        return NULL;

    start = *line;
    if ((end = strchr(*line, ',')) == NULL)
        *line = NULL;
    else
    {
        *end = '\0';
        *line = end + 1;
    }

    return start;
}

/* Parse through a line of input, expecting a numeric zip code, a string city
 * name, a string state abbreviation, a numeric latitude, a numeric longitude,
 * and a string zip code
*/
static int32_t parseLine(
    char                    *line,
    struct pointsofinterest *poi)
{
    uint32_t    uLat;
    uint32_t    uLon;
    double      lat;
    double      lon;
    const char *elem;

    /* zip */
    if ((elem = getNextElement(&line)) == NULL)
        return 0;

    sscanf(elem, "%d", &poi->zip);

    /* city */
    if ((elem = getNextElement(&line)) == NULL)
        return 0;

    strcpy(poi->city, elem);

    /* state */
    if ((elem = getNextElement(&line)) == NULL)
        return 0;

    strcpy(poi->state, elem);

    /* latitude */
    if ((elem = getNextElement(&line)) == NULL)
        return 0;

    sscanf(elem, "%lf", &lat);
    uLat = normalizeLatitude(lat);

    /* longitude */
    if ((elem = getNextElement(&line)) == NULL)
        return 0;

    sscanf(elem, "%lf", &lon);
    uLon = normalizeLongitude(lon);

    /* time zone */
    if ((elem = getNextElement(&line)) == NULL)
        return 0;

    sscanf(elem, "%d", &poi->tz);

    /* Calculate the Z-Value based on randomly generated longitude and latitude. Random numbers between 0 and MAP_SIZE. */
    poi->zvalue = interleave(uLon, uLat);

    return 1;
}

/* Insert some data into the database.
*/
static void insert_data(
    DB_TASK *task)
{
    int32_t                 iStatus;
    uint32_t                ii;
    struct pointsofinterest poi;
    FILE                   *fp;
    char                    line[100];

    fp = fopen("latlongdata.csv", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open 'latlongdata.csv'.  Please make sure it is in the current directory.\n");
        return;
    }

    printf("Inserting city data, please wait...");
    fflush(stdout);

    d_trbegin("datapoints", task);

    ii = 0;
    while (fgets(line, (int32_t) sizeof(line), fp))
    {
        if (!parseLine(line, &poi))
            return;

        /* new transaction every 1000 records */
        if (ii && ii % 5000 == 0)
        {
            d_trend(task);
            d_trbegin("datapoints", task);
        }

        ii++;

        /* Create the record. */
        iStatus = d_fillnew(POINTSOFINTEREST, &poi, task, CURR_DB);
        if (iStatus != S_OKAY)
            printf("Failed to create a record, return = %d\n", iStatus);
    }

    d_trend(task);

    printf(" done.\n");
    printf("NOTE: Latitude ranges between 17.1 and 71.3.\n");
    printf("      Longitude ranges between -176.7 and -64.7.\n");
    printf("      No points will be found outside of these ranges.\n");
    fflush(stdout);
}

typedef enum { LATITUDE, LONGITUDE } AXIS;

static void getBoundaryInfo(
    AXIS         axis,
    const char **name,
    const char **xy,
    double      *max,
    uint32_t  (**fcn)(double))
{
    if (axis == LONGITUDE)
    {
        *name = "Longitude";
        *xy = "X";
        *max = 180.0;
        *fcn = normalizeLongitude;
    }
    else
    {
        *name = "Latitude";
        *xy = "Y";
        *max = 90.0;
        *fcn = normalizeLatitude;
    }
}

/* Get a boundary (longitude or latitude) doing all necessary checking
*/
static uint32_t getBoundary(
    const char     *prompt,
    AXIS            axis,
    uint32_t       *val,
    const uint32_t *prevval)
{
    double      dVal;
    double      max;
    const char *xy;
    const char *name;
    char        cmd[20];
    uint32_t  (*fcn)(double);

    getBoundaryInfo(axis, &name, &xy, &max, &fcn);

    for (;;)
    {
        printf("%s %s (%s) (-%.1f through %.1f): ", prompt, name, xy, max, max);
        getstring(cmd, sizeof(cmd));
        if (*cmd == '\0')
            return 0;

        sscanf(cmd, "%lf", &dVal);
        if (dVal < -max || dVal > max)
        {
            printf("%s must be between -%.1f and %.1f\n", name, max, max);
            continue;
        }

        *val = (fcn)(dVal);

        if (prevval && *val == *prevval)
        {
            printf("Second %s must be different than First %s.\n", name, name);
            continue;
        }

        break;
    }

    return 1;
}


/* Print all points of interest within a rectangle.
*/
static void records_in_rectangle(
    DB_TASK *task)
{
    uint32_t uTmp;
    uint32_t uMinX = 0;
    uint32_t uMinY = 0;
    uint32_t uMaxX = 0;
    uint32_t uMaxY = 0;

    /* Ask the user for the query bounding box. */
    printf("\nEnter bounding box values\n");

    if (!getBoundary("First", LATITUDE, &uMinY, NULL))
        return;

    if (!getBoundary("Second", LATITUDE, &uMaxY, &uMinY))
        return;

    if (uMaxY < uMinY)
    {
        uTmp = uMaxY;
        uMaxY = uMinY;
        uMinY = uTmp;
    }

    if (!getBoundary("First", LONGITUDE, &uMinX, NULL))
        return;

    if (!getBoundary("Second", LONGITUDE, &uMaxX, &uMinX))
        return;

    if (uMaxX < uMinX)
    {
        uTmp = uMaxX;
        uMaxX = uMinX;
        uMinX = uTmp;
    }

    printf("\n");

    /* points_in_rectangle will do a range query across the entered values,
     * decide if the rectangle entered needs to be broken up in smaller pieces
     * and recursively scan these smaler rectangles. Any point that's within
     * the given rengle will be printed by the callback function print_point.
     */

    rectangle_query(uMinX, uMinY, uMaxX, uMaxY, print_point, task);
}

/* Print all points of interest within a rectangle.
*/
static void nearest_neighbor(
    DB_TASK *task)
{
    uint32_t                minX;
    uint32_t                minY;
    uint32_t                maxX;
    uint32_t                maxY;
    struct pointsofinterest poi;

    /* Ask the user for the query bounding box. */
    printf("\nEnter point values\n");
    if (!getBoundary("Point", LATITUDE, &uPointY, NULL))
        return;

    if (!getBoundary("Point", LONGITUDE, &uPointX, NULL))
        return;

    printf("\n");

    /* points_in_rectangle will do a range query accross the entered values, decide
     * if the rectangle entered needs to be broken up in smaler peices and recursively
     * scan these smaler rectangles. Any point that's within the given rengle will be
     * handed of to the neighbor_calculation which will set the dbaNeighbor if a
     * neighbor is found, or not. If not we'll expand our rectangle and requery until
     * we find our point.
     */

    minX = uPointX;
    minY = uPointY;
    maxX = uPointX;
    maxY = uPointY;
    fDistance = -1.0;
    dbaNeighbor = NULL_DBA;
    do
    {
        minX -= min(minX, 500);
        minY -= min(minY, 500);
        maxX += min(MAP_SIZE - maxX, 500);
        maxY += min(MAP_SIZE - maxY, 500);

        rectangle_query(minX, minY, maxX, maxY, neighbor_calculation, task);
    }
    while (DBA_ISNULL(dbaNeighbor) && !(minX == 0 && minY == 0 && maxX == MAP_SIZE && maxY == MAP_SIZE));

    /* Only print if dbaNeighbor is found */
    if (!DBA_ISNULL(dbaNeighbor))
    {
        if (d_crset(&dbaNeighbor, task, CURR_DB) != S_OKAY)
            return;

        if (d_recread(&poi, task, CURR_DB) != S_OKAY)
            return;

        printf("Nearest Neighbor:\n\n");
        printf("%.3lf miles to ", fDistance);
        print_poi(&poi);
    }
}

static int32_t get_zip(
    const char              *prompt,
    struct pointsofinterest *poi,
    DB_TASK                 *task)
{
    int32_t zip;
    int32_t stat;
    char    cmd[20];

    for (;;)
    {
        printf("%s: ", prompt);
        getstring(cmd, sizeof(cmd));
        if (*cmd == '\0')
            return 0;

        sscanf(cmd, "%d", &zip);
        if (zip <= 100 || zip >= 100000)
        {
            printf("Invalid zip - must be between 00210 and 99950.\n");
            continue;
        }

        if ((stat = d_keyfind(POINTSOFINTEREST_ZIP, &zip, task, CURR_DB)) == S_OKAY)
        {
            /* Found the zip, read the record and return */
            d_recread(poi, task, CURR_DB);
            break;
        }
        /* a system error would typically indicate a non-initialized database here */
        if (stat <= S_SYSTEM_FIRST )
            return 0;

        printf("Zip %05d doesn't exist.  Nearest zips are:\n", zip);

        /* find next closest */
        if (d_keynext(POINTSOFINTEREST_ZIP, task, CURR_DB) == S_NOTFOUND)
            d_keynext(POINTSOFINTEREST_ZIP, task, CURR_DB);

        d_recread(poi, task, CURR_DB);
        printf("    ");
        print_poi(poi);

        /* find previous closest */
        if (d_keyprev(POINTSOFINTEREST_ZIP, task, CURR_DB) == S_NOTFOUND)
            d_keyprev(POINTSOFINTEREST_ZIP, task, CURR_DB);

        d_recread(poi, task, CURR_DB);
        printf("    ");
        print_poi(poi);
    }

    return 1;
}

static void between_zips(
    DB_TASK *task)
{
    struct pointsofinterest poi1;
    struct pointsofinterest poi2;

    printf("\nEnter zip codes\n");
    if (!get_zip("From", &poi1, task))
        return;

    if (!get_zip("To  ", &poi2, task))
        return;

    /* compute and print distance */
    printf("Distance between\n    ");
    print_poi(&poi1);

    printf("and\n    ");
    print_poi(&poi2);

    printf("is %.3lf miles (as the crow flies).\n",
            miles_between(denormalizeLongitude(decodeX(poi1.zvalue)),
                denormalizeLatitude(decodeY(poi1.zvalue)),
                denormalizeLongitude(decodeX(poi2.zvalue)),
                denormalizeLatitude(decodeY(poi2.zvalue))));
}

static void find_city(
    DB_TASK *task)
{
    char                     city[50];
    char                     ans[5];
    size_t                   cityLen;
    int32_t                  iStatus;
    int32_t                  cityCount;
    struct  pointsofinterest poi;

    for (;;)
    {
        printf("Enter letters of city's name (case sensitive), or ENTER to exit: ");
        getstring(city, 50);
        if (*city == '\0')
            return;

        if ((iStatus = d_keyfind(POINTSOFINTEREST_CITY, city, task, CURR_DB)) == S_NOTFOUND)
            iStatus = d_keynext(POINTSOFINTEREST_CITY, task, CURR_DB);

        cityLen = strlen(city);
        cityCount = 0;

        while (iStatus == S_OKAY)
        {
            d_recread(&poi, task, CURR_DB);
            if (strncmp(city, poi.city, cityLen) != 0)
                break;

            print_poi(&poi);
            cityCount++;
            if (cityCount%25 == 0) {
                printf("Continue? (y/n) ");
                getstring(ans, 5);
                if (*ans == 'n' || *ans == 'N')
                    return;
            }

            iStatus = d_keynext(POINTSOFINTEREST_CITY, task, CURR_DB);
        }

        if (cityCount == 0)
            printf("No city name found starting with %s.\n", city);
    }
}

static void EXTERNAL_FCN MyErr(
    int32_t err,
    char   *errmsg)
{ /*lint !e403 */
    /* anticipate a d_open before the database exists */
    if (err == S_NOFILE)
        printf("Database not initialized!\n");

    if (err != S_INVDB)
        printf("Database Error: %s\n", errmsg);
}

static const char *const ddlp_poi_argv[] =
{
    "ddlp", "-B", "-q", "poi.ddl"
};

int32_t main(
    void)
{
    int32_t  iStatus;
    char     cmd[20];
    DB_TASK *task;

    d_opentask(&task);
    d_setpages(3100, 0, task);
    d_set_dberr(MyErr, task);

    iStatus = d_open("poi", "x", task);
    if (iStatus == S_INVDB)
    {
        /* create it from scratch */
        iStatus = ddlp_main(RLEN(ddlp_poi_argv), ddlp_poi_argv);
        if (iStatus == S_OKAY)
        {
            iStatus = d_open("poi", "x", task);
            if (iStatus == S_OKAY)
            {
                /* initialize the database */
                iStatus = d_initialize(task, CURR_DB);
                if (iStatus == S_OKAY)
                    printf("New poi database created and initialized\n");
            }
        }
    }

    if (iStatus != S_OKAY)
    {
        printf("d_open returned %d\n", iStatus);
        return iStatus;
    }

    for (;;)
    {
        /* display command menu */
        printf("\nPOI Commands:\n");
        printf("   1 - Initialize and load U.S. zipcode data.\n");
        printf("   2 - Print cities in a map rectangle.\n");
        printf("   3 - Find nearest city of a given point.\n");
        printf("   4 - Compute distance between two zip codes.\n");
        printf("   5 - Find cities.\n");
        printf("   q - Quit\n");
        printf("enter command: ");

        getstring(cmd, sizeof(cmd));
        switch (cmd[0])
        {
            case 'q':
            case 'Q': d_close(task); /*lint !e534 */
                      d_closetask(task); /*lint !e534 */
                      return 0;

            case '1':   d_initialize(task, CURR_DB); /*lint !e534 */
                        insert_data(task);
                        break;
            case '2': records_in_rectangle(task); break;
            case '3': nearest_neighbor(task);     break;
            case '4': between_zips(task);         break;
            case '5': find_city(task);            break;

            default:  printf("*** incorrect command -- re-enter\n");
                      break;
        }
    }

    return 0; /*lint !e527 */
}

/* safe gets() */
void getstring(
    char   *s,
    int32_t size)
{
    size_t len;

    if (fgets(s, size, stdin) == NULL)
        return;

    len = strlen(s);
    if (s[len-1] == '\n')
        s[len-1] = '\0';
}

