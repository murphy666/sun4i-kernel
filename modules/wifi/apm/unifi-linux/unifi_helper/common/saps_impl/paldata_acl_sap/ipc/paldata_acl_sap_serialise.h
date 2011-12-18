/* This is an autogenerated file */
/* Tag: noCheckHeader */

/*    CONFIDENTIAL */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2008. All rights reserved. */

#ifndef __PALDATA_ACL_SAP_SERIALIZE_H__
#define __PALDATA_ACL_SAP_SERIALIZE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "paldata_top_level_fsm/paldata_top_level_fsm.h"

CsrUint16 serialise_pal_acl_data_ind(CsrUint8** resultBuffer, CsrUint16 dataLength, const CsrUint8 *data, CsrUint8 physicalLinkHandle, CsrUint16 aclOffset, unifi_FrameFreeFunction freeFunction);


#ifdef __cplusplus
}
#endif

#endif /* __PALDATA_ACL_SAP_SERIALIZE_H__ */