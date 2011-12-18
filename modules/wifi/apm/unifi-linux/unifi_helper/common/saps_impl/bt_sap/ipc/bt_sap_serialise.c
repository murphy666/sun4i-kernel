/* This is an autogenerated file */
/* Tag: noCheckHeader */

/*    CONFIDENTIAL */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2008. All rights reserved. */

#include "sme_top_level_fsm/sme_top_level_fsm.h"
#include "event_pack_unpack/event_pack_unpack.h"

CsrUint16 serialise_unifi_bt_active_wifi_channel_req(CsrUint8** resultBuffer, CsrUint16 channelMhz, CsrUint16 bandwidthMhz)
{
    CsrUint16 packedLength = 0;
    CsrUint32 packedSize = 6;
    CsrUint8* evt;
    CsrUint8* buffer;

    /* Calc Dymanic Size */
    packedSize += 2;
    packedSize += 2;

    evt = (CsrUint8*)CsrPmalloc(packedSize);
    buffer = evt;
    packedLength += event_pack_CsrUint16(&buffer, UNIFI_BT_ACTIVE_WIFI_CHANNEL_REQ_ID);
    packedLength += event_pack_CsrUint16(&buffer, 0);
    packedLength += event_pack_CsrUint16(&buffer, 0);
    packedLength += event_pack_CsrUint16(&buffer, channelMhz);
    packedLength += event_pack_CsrUint16(&buffer, bandwidthMhz);

    *resultBuffer = evt;
    return packedLength;
}

CsrUint16 serialise_unifi_bt_wifi_active_req(CsrUint8** resultBuffer)
{
    CsrUint16 packedLength = 0;
    CsrUint32 packedSize = 6;
    CsrUint8* evt;
    CsrUint8* buffer;

    /* Calc Dymanic Size */

    evt = (CsrUint8*)CsrPmalloc(packedSize);
    buffer = evt;
    packedLength += event_pack_CsrUint16(&buffer, UNIFI_BT_WIFI_ACTIVE_REQ_ID);
    packedLength += event_pack_CsrUint16(&buffer, 0);
    packedLength += event_pack_CsrUint16(&buffer, 0);

    *resultBuffer = evt;
    return packedLength;
}

CsrUint16 serialise_unifi_bt_wifi_inactive_req(CsrUint8** resultBuffer)
{
    CsrUint16 packedLength = 0;
    CsrUint32 packedSize = 6;
    CsrUint8* evt;
    CsrUint8* buffer;

    /* Calc Dymanic Size */

    evt = (CsrUint8*)CsrPmalloc(packedSize);
    buffer = evt;
    packedLength += event_pack_CsrUint16(&buffer, UNIFI_BT_WIFI_INACTIVE_REQ_ID);
    packedLength += event_pack_CsrUint16(&buffer, 0);
    packedLength += event_pack_CsrUint16(&buffer, 0);

    *resultBuffer = evt;
    return packedLength;
}

