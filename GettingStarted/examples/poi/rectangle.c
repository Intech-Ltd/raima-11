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
 * This example will show you how to manage 2 dimensional date, in our case
 * fictitious points of interest in a map, efficiently using a 1 dimensional
 * index. To do this we’ll be mapping the latitude, longitude of our points
 * into what we define as a Z-value. Since out latitude and longitude values
 * are 32 bit integers the Z-value will be 64 bit’s wide. Our example will
 * only store the 64 bit Z-Value in the database since both the latitude and
 * longitude can be derived from this value. The coding is a simple
 * interleaving of bits from the latitude and longitude.
 *
 * Looking at these three values bit pattern, the interleaving can be
 * explained by:
 *    Latitude  = x32 x31 x30 ... x1
 *    Longitude = y32 y31 y30 ... y1
 *    Z-Value   = x32 y32 x31 y31 x30 y30 ... x1 y1
 *
 * Please visit our examples guide in the documentation set for a comprehensive
 * explanation of this example.
*/

#include <stdlib.h>
#include <stdio.h>
#include "rdm.h"
#include "poi.h"
#include <assert.h> /*lint !e451 -emacro(944,assert) */

#if defined(RDM_WINDOWS)
#include <windows.h>
#endif

typedef int32_t (*callback)(DB_TASK *);

/* Global Variables */
static const uint64_t uXes = ULL(0xAAAAAAAAAAAAAAAA); /* Bitmap for all X's in the Z-Value. */
static const uint64_t uYes = ULL(0x5555555555555555); /* Bitmap for all Y's in the Z-Value. */

uint32_t decodeX(uint64_t);
uint32_t decodeY(uint64_t);
uint64_t interleave(uint32_t, uint32_t);
void     rectangle_query(uint32_t, uint32_t, uint32_t, uint32_t, callback, DB_TASK *);

/* Decode X coordinate (longitude) from the interleaved Z-value
 *
 * Longitude = x32 x31 x30 … x1
 * Latitude  = y32 y31 y30 … y1
 * Z-Value   = x32 y32 x31 y31 x30 y30 … x1 y1
 *
 * Since the Z-Value is a interleaving of Longitude and Latitude this function
 * extracts all the x bits from the Z-Value and shifts them down into a 32 bit
 * datatype to re-build the Longitude data.
 */

uint32_t decodeX(
    uint64_t z)
{
    uint64_t uZx8 =
            ((z & ULL(0x4000400040004000)) >> 7) |
            ((z & ULL(0x1000100010001000)) >> 6) |
            ((z & ULL(0x0400040004000400)) >> 5) |
            ((z & ULL(0x0100010001000100)) >> 4) |
            ((z & ULL(0x0040004000400040)) >> 3) |
            ((z & ULL(0x0010001000100010)) >> 2) |
            ((z & ULL(0x0004000400040004)) >> 1) |
            (z & ULL(0x0001000100010001));
    uint32_t x = (uint32_t) (((uZx8 & ULL(0x00FF000000000000)) >> 24) |
            ((uZx8 & ULL(0x000000FF00000000)) >> 16) |
            ((uZx8 & ULL(0x0000000000FF0000)) >> 8) |
            (uZx8 & ULL(0x00000000000000FF)));

    return x;
}


/* Decode Y coordinate (latitude) from the interleaved Z-value
 *
 * Longitude = x32 x31 x30 … x1
 * Latitude  = y32 y31 y30 … y1
 * Z-Value   = x32 y32 x31 y31 x30 y30 … x1 y1
 *
 * Since the Z-Value is a interleaving of Longitude and Latitude this function
 * extracts all the y bits from the Z-Value and shifts them down into a 32 bit
 * datatype to re-build the Latitude data.
 */

uint32_t decodeY(
    uint64_t z)
{
    uint64_t uZx8 =
            ((z & ULL(0x8000800080008000)) >> 8) |
            ((z & ULL(0x2000200020002000)) >> 7) |
            ((z & ULL(0x0800080008000800)) >> 6) |
            ((z & ULL(0x0200020002000200)) >> 5) |
            ((z & ULL(0x0080008000800080)) >> 4) |
            ((z & ULL(0x0020002000200020)) >> 3) |
            ((z & ULL(0x0008000800080008)) >> 2) |
            ((z & ULL(0x0002000200020002)) >> 1);
    uint32_t y = (uint32_t) (((uZx8 & ULL(0x00FF000000000000)) >> 24) |
            ((uZx8 & ULL(0x000000FF00000000)) >> 16) |
            ((uZx8 & ULL(0x0000000000FF0000)) >> 8) |
            (uZx8 & ULL(0x00000000000000FF)));

    return y;
}

/* Create the Z-Value based on the X coordinate (longitude) and Y coordinate (latitude).
 *
 * Longitude = x32 x31 x30 … x1
 * Latitude  = y32 y31 y30 … y1
 * Z-Value   = x32 y32 x31 y31 x30 y30 … x1 y1
 *
 * This function takes two 32 bit values, namely the longitude (x) and latitude (y)
 * and generates the bit interleaved Z-Value.
 */

uint64_t interleave(
    uint32_t x,
    uint32_t y)
{
    uint64_t uZx8 = ((uint64_t) (x & 0xFFUL)) +
            (((uint64_t) (x & 0xFF00UL)) << 8) +
            (((uint64_t) (x & 0xFF0000UL)) << 16) +
            (((uint64_t) (x & 0xFF000000UL)) << 24);
    uint64_t uZx = (uZx8 & ULL(0x0101010101010101)) |
            ((uZx8 & ULL(0x0202020202020202)) << 1) |
            ((uZx8 & ULL(0x0404040404040404)) << 2) |
            ((uZx8 & ULL(0x0808080808080808)) << 3) |
            ((uZx8 & ULL(0x1010101010101010)) << 4) |
            ((uZx8 & ULL(0x2020202020202020)) << 5) |
            ((uZx8 & ULL(0x4040404040404040)) << 6) |
            ((uZx8 & ULL(0x8080808080808080)) << 7);
    uint64_t uZy8 = ((uint64_t) (y & 0xFFUL)) +
            (((uint64_t) (y & 0xFF00UL)) << 8) +
            (((uint64_t) (y & 0xFF0000UL)) << 16) +
            (((uint64_t) (y & 0xFF000000UL)) << 24);
    uint64_t uZy = ((uZy8 & ULL(0x0101010101010101)) << 1) |
            ((uZy8 & ULL(0x0202020202020202)) << 2) |
            ((uZy8 & ULL(0x0404040404040404)) << 3) |
            ((uZy8 & ULL(0x0808080808080808)) << 4) |
            ((uZy8 & ULL(0x1010101010101010)) << 5) |
            ((uZy8 & ULL(0x2020202020202020)) << 6) |
            ((uZy8 & ULL(0x4040404040404040)) << 7) |
            ((uZy8 & ULL(0x8080808080808080)) << 8);
    uint64_t z = uZx | uZy;

    assert(x == decodeX(z));
    assert(y == decodeY(z));

    return z;
}


/* Break up a rectangle into two smaller rectangles for minimizing the
   nodes that needs to be visited.

   This method can only be called for rectangles that actually can
   be divided into two rectangles.
   */

static void zdivide(
    uint64_t  uMin,
    uint64_t  uMax,
    uint64_t *puLitMax,
    uint64_t *puBigMin)
{
    uint64_t uBD = uMin^uMax;
    uint64_t uMSMask;
    uint64_t uLSMask;
    uint64_t uBSCommon;
    uint64_t uLitMax;
    uint64_t uBigMin;
    uint64_t uMSBD = ((((uint64_t) 0x80000000UL) << 32) |
            ((uint64_t) 0x00000000UL));

    assert (uMin < uMax);

    /* uMin and uMax as follows:
       +---+
       uMin     x   y   ...   z | 0 | a   b   c   d   e   ...
       +---+
       uMax     x   y   ...   z | 1 | s   t   u   v   w   ...
       +---+
       gives the following:

       uMSBD    0   0   ...   0   1   0   0   0   0   0   ...

       uMSMask  0   0   ...   0   0   0   1   0   1   0   ...

       uLSMask  0   0   ...   0   0   1   0   1   0   1   ...

       uBSComm  x   y   ...   z   0   0   0   0   0   0   ...

       uLitMax  x   y   ...   z   0   s   1   u   1   w   ...

       uBigMin  x   y   ...   z   1   0   b   0   d   0   ...
       */

    /* Find the most significant bit that differ between uMin and uMax */
    while ((uMSBD & uBD) == 0)
    {
        uMSBD = uMSBD >> 1;
    }

    /* Find the mask for the most significant bits and the least significant bits */
    if (uMSBD & uXes)
    {
        uMSMask = (uMSBD - 1) & uXes;
        uLSMask = (uMSBD - 1) & uYes;
    }
    else
    {
        uMSMask = (uMSBD - 1) & uYes;
        uLSMask = (uMSBD - 1) & uXes;
    }

    uBSCommon= uMin & (~(uMSBD + uMSBD - 1));
    uLitMax = uBSCommon | uMSMask | (uLSMask & uMax);
    uBigMin = uBSCommon | uMSBD | (uLSMask & uMin);

    assert(uMin <= uLitMax);
    assert(uLitMax < uBigMin);
    assert(uBigMin <= uMax);

    *puLitMax = uLitMax;
    *puBigMin = uBigMin;
}

/* Is the point within the specified rectangle.

   THIS FUNCTION IS FOR INTERNAL USE ONLY.
   */
static int32_t in_rectangle(
    uint64_t uPoint,
    uint64_t uMinRect,
    uint64_t uMaxRect)
{
    if ((uPoint & uXes) <= (uMaxRect & uXes) &&
            (uMinRect & uXes) <= (uPoint & uXes) &&
            (uPoint & uYes) <= (uMaxRect & uYes) &&
            (uMinRect & uYes) <= (uPoint & uYes))
        return TRUE;

    return FALSE;
}

void getstring(char *, int32_t);

/* Call pDo for some points within the rectangle.

   THIS FUNCTION IS FOR INTERNAL USE ONLY.

   pDo is not allowed to change the currency for the current record or
   the cordinates for this point.

   Start the scan for points at uFrom and continue as long as we find
   points within the rectangle without having to scan by two many
   points that are outside the rectangle.

   How many points we are allowed to scan by that are outside the
   rectangle without giving up, is an implementation detail that
   affects performance

   There are no interpretable HRESULT.
   */

static int32_t pointCount; /*lint !e956 */

static int32_t some_points_in_rectangle(
    uint64_t uFrom,
    uint64_t uMin,
    uint64_t uMax,
    uint64_t *puLast,
    callback pDo,
    DB_TASK *task)
{
    int32_t iStatus;

    struct pointsofinterest segMin;
    uint64_t                uPoint = uMax;
    int32_t                 misses = 0;
    char                    ans[5];

    segMin.zvalue = uFrom;

    iStatus = d_keyfind(POINTSOFINTEREST_ZVALUE, &segMin, task, CURR_DB);

    if (iStatus == S_NOTFOUND)
        iStatus = d_keynext(POINTSOFINTEREST_ZVALUE, task, CURR_DB);

    while (iStatus == S_OKAY)
    {
        iStatus = d_keyread(&uPoint, task);
        if (iStatus == S_OKAY)
        {
            if (uPoint > uMax)
                break;

            if (in_rectangle(uPoint, uMin, uMax))
            {
                if (pDo)
                {
                    iStatus = pDo(task);
                    pointCount++;
                    if (pointCount % 25 == 0)
                    {
                        printf("Continue? (y/n) ");
                        getstring(ans, 5);
                        if (*ans == 'n' || *ans == 'N')
                            return S_EOS;
                    }
                }

                misses = 0;
            }
            else
            {
                misses++;
                /* How many misses we allow is an implementation
                   detail that affects performance. */
                if (misses == 3)
                    break;
            }

            if (iStatus == S_OKAY)
                iStatus = d_keynext(POINTSOFINTEREST_ZVALUE, task, CURR_DB);
        }
    }

    if (iStatus == S_NOTFOUND)
    {
        uPoint = uMax;
        iStatus = S_OKAY;
    }

    assert (uPoint >= uFrom);
    *puLast= uPoint;

    return iStatus;
}

/* Call pDo for each point withing the inner rectangle (uMin, uMax)
   and some points within the outer rectangle (uMinRect uMaxRect).

   THIS FUNCTION IS FOR INTERNAL USE ONLY.

   pDo is not allowed to change the currency for the current record or
   the cordinates for this point.

   How many points we handle outside the inner rectangle is an
   implementation detail.
   */

static int32_t points_in_rectangle(
    uint64_t uMin,
    uint64_t uMax,
    uint64_t uFrom,
    uint64_t *puLast,
    uint64_t uMinRect,
    uint64_t uMaxRect,
    callback pDo,
    DB_TASK *task)
{
    int32_t iStatus= S_OKAY;

    uint64_t uLitMax, uBigMin;
    uint64_t uLast;

    assert (uMin < uMax);
    assert (uMin < uFrom);
    assert (uFrom < uMax);

    zdivide(uMin, uMax, &uLitMax, &uBigMin);

    assert (uMin <= uLitMax);
    assert (uLitMax < uBigMin);
    assert (uBigMin <= uMax);

    if (uFrom < uLitMax)
    {
        iStatus = points_in_rectangle(uMin, uLitMax, uFrom, &uLast, uMinRect,
                uMaxRect, pDo, task);
        if (iStatus == S_EOS)
            return iStatus;
    }
    else
        uLast = uFrom;

    if (iStatus == S_OKAY && uLast < uMax)
    {
        if (uLast < uBigMin)
        {
            iStatus = some_points_in_rectangle(uBigMin, uMinRect, uMaxRect,
                    &uLast, pDo, task);
            if (iStatus == S_EOS)
                return iStatus;

            assert (uBigMin <= uLast);
        }

        if (iStatus == S_OKAY && uLast < uMax)
        {
            iStatus = points_in_rectangle(uBigMin, uMax, uLast, &uLast,
                    uMinRect, uMaxRect, pDo, task);
            if (iStatus == S_EOS)
                return iStatus;
        }
    }

    if (iStatus == S_OKAY)
        *puLast= uLast;

    return iStatus;
}

/* Call pDo for each point withing the rectangle.

   pDo is not allowed to change the currency for the current record or
   the cordinates for this point.

   pDo is not allowed to change the currency for the current record or
   the ZVALUE key.
   */

void rectangle_query(
    uint32_t uMinLongitude,
    uint32_t uMinLatitude,
    uint32_t uMaxLongitude,
    uint32_t uMaxLatitude,
    callback pDo,
    DB_TASK *task)
{
    int32_t iStatus;

    uint64_t uMin = interleave(uMinLongitude, uMinLatitude);
    uint64_t uMax = interleave(uMaxLongitude, uMaxLatitude);
    uint64_t uLast;

    assert (uMinLongitude <= uMaxLongitude);
    assert (uMinLatitude <= uMaxLatitude);
    assert (uMin <= uMax);

    pointCount = 0;

    iStatus = some_points_in_rectangle(uMin, uMin, uMax, &uLast, pDo, task);
    if (iStatus == S_EOS)
        return;

    if (iStatus == S_OKAY && uLast < uMax)
        points_in_rectangle (uMin, uMax, uLast, &uLast, uMin, uMax, pDo, task);   /*lint !e534 */
}
