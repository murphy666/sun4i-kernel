/* This is an autogenerated file */
/* Tag: noCheckHeader */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2009. All rights reserved. */
/* */
/*   Refer to LICENSE.txt included with this source for details on the */
/*   license terms. */

/* STANDARD INCLUDES *********************************************************/
#include "ie_utils/ie_utils.h"
#include "ie_access_trace/lib_info_element_trace_fields.h"

/* PUBLIC FUNCTION DEFINITIONS ***********************************************/

#ifdef LIB_INFO_ELEMENT_TRACE_ENABLE

CsrUint8* ie_trace_ssid(CsrUint8 traceLevel, CsrUint8 *pBuf)
{
    CsrUint32 value;

    ie_CsrUint8(pBuf, value);         CsrLog(traceLevel, (CsrString*)"", (CsrString*)"id:                        0x%.2x", value );
    ie_CsrUint8(pBuf, value);         CsrLog(traceLevel, (CsrString*)"", (CsrString*)"length:                    0x%.2x", value );
    
    /* defined length, length */
    CsrLogBuffer(traceLevel, (CsrString*)"", pBuf, (CsrUint16)value, (CsrString*)"    ssid");
    pBuf+=value;

    return pBuf;
}


#endif /* LIB_INFO_ELEMENT_TRACE_ENABLE */




