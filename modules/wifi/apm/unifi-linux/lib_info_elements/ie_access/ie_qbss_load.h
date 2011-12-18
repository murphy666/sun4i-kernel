/* This is an autogenerated file */
/* Tag: noCheckHeader */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2009. All rights reserved. */
/* */
/*   Refer to LICENSE.txt included with this source for details on the */
/*   license terms. */

#ifndef IE_QBSS_LOAD_H
#define IE_QBSS_LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* STANDARD INCLUDES *********************************************************/
#include "ie_utils/ie_utils.h"

/* COMPLEX TYPES DEFINITIONS *************************************************/
typedef struct ie_qbss_load
{
    CsrUint8  id;
    CsrUint8  length;
    CsrUint8  stationCount[2];
    CsrUint8  channelUtilization;
    CsrUint8  AvailableAdmissionCap[2];
}ie_qbss_load;


/* PUBLIC FUNCTION PROTOTYPES ************************************************/
#define ie_get_qbss_load(elements,length, qbss_load) ie_find(IE_ID_QBSS_LOAD, elements, length, (CsrUint8**)&qbss_load);


/* SUBSIDARY ACCESSORS *******************************************************/
#define IE_QBSS_LOAD__ID_SIZE                                1
#define IE_QBSS_LOAD__LENGTH_SIZE                            1
#define IE_QBSS_LOAD__STATION_COUNT_SIZE                     2
#define IE_QBSS_LOAD__CHANNEL_UTILIZATION_SIZE               1
#define IE_QBSS_LOAD__AVAILABLE_ADMISSION_CAP_SIZE           2
#define IE_QBSS_LOAD__TOTAL_SIZE                             7




#ifdef __cplusplus
}
#endif

#endif /* IE_QBSS_LOAD_H */
