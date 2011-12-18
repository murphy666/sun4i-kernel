/* This is an autogenerated file */
/* Tag: noCheckHeader */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2009. All rights reserved. */
/* */
/*   Refer to LICENSE.txt included with this source for details on the */
/*   license terms. */

#ifndef IE_RSN_H
#define IE_RSN_H

#ifdef __cplusplus
extern "C" {
#endif

/* STANDARD INCLUDES *********************************************************/
#include "ie_utils/ie_utils.h"

/* COMPLEX TYPES DEFINITIONS *************************************************/
typedef struct ie_rsn
{
    struct rsn_header {
        CsrUint8  id;
        CsrUint8  length;
        CsrUint8  version[2];
        CsrUint8  groupCipherOui[3];
        CsrUint8  groupCipherSuiteType;
    }* header; 
    
    struct rsn_pwCipherPair {
        CsrUint8  pwCipherCount[2];
        struct rsn_pwCipherList {
            CsrUint8  pwCipherListOui[3];
            CsrUint8  pwCipherListSuiteType;
        } pwCipherList[1]; 
    }* pwCipherPair; 
    
    struct rsn_akmCipherPair {
        CsrUint8  akmCipherCount[2];
        struct rsn_akmCipherList {
            CsrUint8  akmCipherListOui[3];
            CsrUint8  akmCipherListSuiteType;
        } akmCipherList[1]; 
    }* akmCipherPair; 
    
    struct rsn_body1 {
        CsrUint8  rsnCapability[2];
    }* body1; 
    
    struct rsn_pmkidPair {
        CsrUint8  pmkidCount[2];
        struct rsn_pmkidList {
            CsrUint8  pmkidListData[16];
        } pmkidList[1]; 
    }* pmkidPair; 
}ie_rsn;


/* PUBLIC FUNCTION PROTOTYPES ************************************************/
extern ie_result ie_get_rsn(CsrUint8* elements, CsrUint32 length, ie_rsn* rsn);



/* SUBSIDARY ACCESSORS *******************************************************/
#define IE_RSN__ID_SIZE                                      1
#define IE_RSN__LENGTH_SIZE                                  1
#define IE_RSN__VERSION_SIZE                                 2
#define IE_RSN__GROUP_CIPHER_SIZE                            4
#define IE_RSN__PW_CIPHER_PAIR_SIZE                          6
#define IE_RSN__AKM_CIPHER_PAIR_SIZE                         6
#define IE_RSN__RSN_CAPABILITY_SIZE                          2
#define IE_RSN__PMKID_PAIR_SIZE                              18
#define IE_RSN__TOTAL_SIZE                                   40




#ifdef __cplusplus
}
#endif

#endif /* IE_RSN_H */
