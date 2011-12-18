/* This is an autogenerated file */
/* Tag: noCheckHeader */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2009. All rights reserved. */
/* */
/*   Refer to LICENSE.txt included with this source for details on the */
/*   license terms. */

#ifndef NME_SECURITY_MANAGER_XML_SIGNALS_H
#define NME_SECURITY_MANAGER_XML_SIGNALS_H

#ifdef __cplusplus
extern "C" {
#endif

#define NME_SECURITY_PRECONNECT_REQ_ID                               0x6301
#define NME_SECURITY_PRECONNECT_CFM_ID                               0x6302
#define NME_SECURITY_START_REQ_ID                                    0x6303
#define NME_SECURITY_START_CFM_ID                                    0x6304
#define NME_SECURITY_RESTART_REQ_ID                                  0x6305
#define NME_SECURITY_RESTART_CFM_ID                                  0x6306
#define NME_SECURITY_STOP_REQ_ID                                     0x6307
#define NME_SECURITY_STOP_CFM_ID                                     0x6308
#define NME_SECURITY_LIBSETKEY_IND_ID                                0x6309
#define NME_SECURITY_LIBSETPAC_IND_ID                                0x630A
#define NME_SECURITY_LIBSETSESSION_IND_ID                            0x630B
#define NME_SECURITY_LIBSETCERTIFICATEVALIDATION_IND_ID              0x630C
#define NME_SECURITY_LIBABORT_IND_ID                                 0x630D
#define NME_SECURITY_LIBWPSDONE_IND_ID                               0x630E
#define NME_SECURITY_KEYS_INSTALLED_IND_ID                           0x630F
#define NME_SECURITY_EAP_TIMER_ID                                    0x6310
#define NME_SECURITY_COMPLETE_IND_ID                                 0x6311
#define NME_SECURITY_ABORT_IND_ID                                    0x6312
#define NME_SECURITY_PREWPS_REQ_ID                                   0x6313
#define NME_SECURITY_PREWPS_CFM_ID                                   0x6314
#define NME_SECURITY_TERMINATE_REQ_ID                                0x6315
#define NME_SECURITY_TERMINATE_CFM_ID                                0x6316


/* -------------------------- nme_security_abort_ind -------------------------- */
typedef struct NmeSecurityAbortInd_Evt
{
    FsmEvent                        common;
    nme_SecurityResultCode          result;
} NmeSecurityAbortInd_Evt;

#define send_nme_security_abort_ind(context, pid, p_result) \
{ \
    NmeSecurityAbortInd_Evt *evt = (NmeSecurityAbortInd_Evt*) CsrPmalloc(sizeof(NmeSecurityAbortInd_Evt)); \
    evt->result               = p_result; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_ABORT_IND_ID); \
}

#define send_nme_security_abort_ind_external(context, pid, p_result) \
{ \
    NmeSecurityAbortInd_Evt *evt = (NmeSecurityAbortInd_Evt*) CsrPmalloc(sizeof(NmeSecurityAbortInd_Evt)); \
    evt->result               = p_result; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_ABORT_IND_ID); \
}

/* -------------------------- nme_security_complete_ind -------------------------- */
typedef struct NmeSecurityCompleteInd_Evt
{
    FsmEvent                        common;
    nme_SecurityResultCode          result;
} NmeSecurityCompleteInd_Evt;

#define send_nme_security_complete_ind(context, pid, p_result) \
{ \
    NmeSecurityCompleteInd_Evt *evt = (NmeSecurityCompleteInd_Evt*) CsrPmalloc(sizeof(NmeSecurityCompleteInd_Evt)); \
    evt->result               = p_result; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_COMPLETE_IND_ID); \
}

#define send_nme_security_complete_ind_external(context, pid, p_result) \
{ \
    NmeSecurityCompleteInd_Evt *evt = (NmeSecurityCompleteInd_Evt*) CsrPmalloc(sizeof(NmeSecurityCompleteInd_Evt)); \
    evt->result               = p_result; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_COMPLETE_IND_ID); \
}

/* -------------------------- nme_security_keys_installed_ind -------------------------- */
typedef struct NmeSecurityKeysInstalledInd_Evt
{
    FsmEvent                        common;
} NmeSecurityKeysInstalledInd_Evt;

#define send_nme_security_keys_installed_ind(context, pid) \
{ \
    NmeSecurityKeysInstalledInd_Evt *evt = (NmeSecurityKeysInstalledInd_Evt*) CsrPmalloc(sizeof(NmeSecurityKeysInstalledInd_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_KEYS_INSTALLED_IND_ID); \
}

#define send_nme_security_keys_installed_ind_external(context, pid) \
{ \
    NmeSecurityKeysInstalledInd_Evt *evt = (NmeSecurityKeysInstalledInd_Evt*) CsrPmalloc(sizeof(NmeSecurityKeysInstalledInd_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_KEYS_INSTALLED_IND_ID); \
}

/* -------------------------- nme_security_libabort_ind -------------------------- */
typedef struct NmeSecurityLibabortInd_Evt
{
    FsmEvent                        common;
} NmeSecurityLibabortInd_Evt;

#define send_nme_security_libabort_ind(context, pid) \
{ \
    NmeSecurityLibabortInd_Evt *evt = (NmeSecurityLibabortInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibabortInd_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBABORT_IND_ID); \
}

#define send_nme_security_libabort_ind_external(context, pid) \
{ \
    NmeSecurityLibabortInd_Evt *evt = (NmeSecurityLibabortInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibabortInd_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBABORT_IND_ID); \
}

/* -------------------------- nme_security_libsetcertificatevalidation_ind -------------------------- */
typedef struct NmeSecurityLibsetcertificatevalidationInd_Evt
{
    FsmEvent                        common;
} NmeSecurityLibsetcertificatevalidationInd_Evt;

#define send_nme_security_libsetcertificatevalidation_ind(context, pid) \
{ \
    NmeSecurityLibsetcertificatevalidationInd_Evt *evt = (NmeSecurityLibsetcertificatevalidationInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetcertificatevalidationInd_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETCERTIFICATEVALIDATION_IND_ID); \
}

#define send_nme_security_libsetcertificatevalidation_ind_external(context, pid) \
{ \
    NmeSecurityLibsetcertificatevalidationInd_Evt *evt = (NmeSecurityLibsetcertificatevalidationInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetcertificatevalidationInd_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETCERTIFICATEVALIDATION_IND_ID); \
}

/* -------------------------- nme_security_libsetkey_ind -------------------------- */
typedef struct NmeSecurityLibsetkeyInd_Evt
{
    FsmEvent                        common;
    unifi_Key                       key;
} NmeSecurityLibsetkeyInd_Evt;

#define send_nme_security_libsetkey_ind(context, pid, p_key) \
{ \
    NmeSecurityLibsetkeyInd_Evt *evt = (NmeSecurityLibsetkeyInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetkeyInd_Evt)); \
    evt->key                  = p_key; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETKEY_IND_ID); \
}

#define send_nme_security_libsetkey_ind_external(context, pid, p_key) \
{ \
    NmeSecurityLibsetkeyInd_Evt *evt = (NmeSecurityLibsetkeyInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetkeyInd_Evt)); \
    evt->key                  = p_key; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETKEY_IND_ID); \
}

/* -------------------------- nme_security_libsetpac_ind -------------------------- */
typedef struct NmeSecurityLibsetpacInd_Evt
{
    FsmEvent                        common;
} NmeSecurityLibsetpacInd_Evt;

#define send_nme_security_libsetpac_ind(context, pid) \
{ \
    NmeSecurityLibsetpacInd_Evt *evt = (NmeSecurityLibsetpacInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetpacInd_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETPAC_IND_ID); \
}

#define send_nme_security_libsetpac_ind_external(context, pid) \
{ \
    NmeSecurityLibsetpacInd_Evt *evt = (NmeSecurityLibsetpacInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetpacInd_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETPAC_IND_ID); \
}

/* -------------------------- nme_security_libsetsession_ind -------------------------- */
typedef struct NmeSecurityLibsetsessionInd_Evt
{
    FsmEvent                        common;
} NmeSecurityLibsetsessionInd_Evt;

#define send_nme_security_libsetsession_ind(context, pid) \
{ \
    NmeSecurityLibsetsessionInd_Evt *evt = (NmeSecurityLibsetsessionInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetsessionInd_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETSESSION_IND_ID); \
}

#define send_nme_security_libsetsession_ind_external(context, pid) \
{ \
    NmeSecurityLibsetsessionInd_Evt *evt = (NmeSecurityLibsetsessionInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibsetsessionInd_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBSETSESSION_IND_ID); \
}

/* -------------------------- nme_security_libwpsdone_ind -------------------------- */
typedef struct NmeSecurityLibwpsdoneInd_Evt
{
    FsmEvent                        common;
    nme_SecurityResultCode          result;
} NmeSecurityLibwpsdoneInd_Evt;

#define send_nme_security_libwpsdone_ind(context, pid, p_result) \
{ \
    NmeSecurityLibwpsdoneInd_Evt *evt = (NmeSecurityLibwpsdoneInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibwpsdoneInd_Evt)); \
    evt->result               = p_result; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBWPSDONE_IND_ID); \
}

#define send_nme_security_libwpsdone_ind_external(context, pid, p_result) \
{ \
    NmeSecurityLibwpsdoneInd_Evt *evt = (NmeSecurityLibwpsdoneInd_Evt*) CsrPmalloc(sizeof(NmeSecurityLibwpsdoneInd_Evt)); \
    evt->result               = p_result; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_LIBWPSDONE_IND_ID); \
}

/* -------------------------- nme_security_preconnect_req -------------------------- */
typedef struct NmeSecurityPreconnectReq_Evt
{
    FsmEvent                        common;
    unifi_SSID                      ssid;
    unifi_MACAddress                stationMacAddress;
    unifi_Credentials               credentials;
} NmeSecurityPreconnectReq_Evt;

#define send_nme_security_preconnect_req(context, pid, p_ssid, p_stationMacAddress, p_credentials) \
{ \
    NmeSecurityPreconnectReq_Evt *evt = (NmeSecurityPreconnectReq_Evt*) CsrPmalloc(sizeof(NmeSecurityPreconnectReq_Evt)); \
    evt->ssid                 = p_ssid; \
    evt->stationMacAddress    = p_stationMacAddress; \
    evt->credentials          = p_credentials; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_PRECONNECT_REQ_ID); \
}

#define send_nme_security_preconnect_req_external(context, pid, p_ssid, p_stationMacAddress, p_credentials) \
{ \
    NmeSecurityPreconnectReq_Evt *evt = (NmeSecurityPreconnectReq_Evt*) CsrPmalloc(sizeof(NmeSecurityPreconnectReq_Evt)); \
    evt->ssid                 = p_ssid; \
    evt->stationMacAddress    = p_stationMacAddress; \
    evt->credentials          = p_credentials; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_PRECONNECT_REQ_ID); \
}

/* -------------------------- nme_security_preconnect_cfm -------------------------- */
typedef struct NmeSecurityPreconnectCfm_Evt
{
    FsmEvent                        common;
    nme_SecurityResultCode          status;
    unifi_80211PrivacyMode          privacyMode;
    CsrUint16                       authModeMask;
    CsrUint16                       encryptionModeMask;
} NmeSecurityPreconnectCfm_Evt;

#define send_nme_security_preconnect_cfm(context, pid, p_status, p_privacyMode, p_authModeMask, p_encryptionModeMask) \
{ \
    NmeSecurityPreconnectCfm_Evt *evt = (NmeSecurityPreconnectCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityPreconnectCfm_Evt)); \
    evt->status               = p_status; \
    evt->privacyMode          = p_privacyMode; \
    evt->authModeMask         = p_authModeMask; \
    evt->encryptionModeMask   = p_encryptionModeMask; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_PRECONNECT_CFM_ID); \
}

#define send_nme_security_preconnect_cfm_external(context, pid, p_status, p_privacyMode, p_authModeMask, p_encryptionModeMask) \
{ \
    NmeSecurityPreconnectCfm_Evt *evt = (NmeSecurityPreconnectCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityPreconnectCfm_Evt)); \
    evt->status               = p_status; \
    evt->privacyMode          = p_privacyMode; \
    evt->authModeMask         = p_authModeMask; \
    evt->encryptionModeMask   = p_encryptionModeMask; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_PRECONNECT_CFM_ID); \
}

/* -------------------------- nme_security_prewps_req -------------------------- */
typedef struct NmeSecurityPrewpsReq_Evt
{
    FsmEvent                        common;
    unifi_SSID                      ssid;
    unifi_MACAddress                stationMacAddress;
    CsrUint8                        pin[8];
} NmeSecurityPrewpsReq_Evt;

#define send_nme_security_prewps_req(context, pid, p_ssid, p_stationMacAddress, p_pin) \
{ \
    NmeSecurityPrewpsReq_Evt *evt = (NmeSecurityPrewpsReq_Evt*) CsrPmalloc(sizeof(NmeSecurityPrewpsReq_Evt)); \
    evt->ssid                 = p_ssid; \
    evt->stationMacAddress    = p_stationMacAddress; \
    CsrMemCpy(evt->pin, p_pin, 8); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_PREWPS_REQ_ID); \
}

#define send_nme_security_prewps_req_external(context, pid, p_ssid, p_stationMacAddress, p_pin) \
{ \
    NmeSecurityPrewpsReq_Evt *evt = (NmeSecurityPrewpsReq_Evt*) CsrPmalloc(sizeof(NmeSecurityPrewpsReq_Evt)); \
    evt->ssid                 = p_ssid; \
    evt->stationMacAddress    = p_stationMacAddress; \
    CsrMemCpy(evt->pin, p_pin, 8); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_PREWPS_REQ_ID); \
}

/* -------------------------- nme_security_prewps_cfm -------------------------- */
typedef struct NmeSecurityPrewpsCfm_Evt
{
    FsmEvent                        common;
    nme_SecurityResultCode          status;
} NmeSecurityPrewpsCfm_Evt;

#define send_nme_security_prewps_cfm(context, pid, p_status) \
{ \
    NmeSecurityPrewpsCfm_Evt *evt = (NmeSecurityPrewpsCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityPrewpsCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_PREWPS_CFM_ID); \
}

#define send_nme_security_prewps_cfm_external(context, pid, p_status) \
{ \
    NmeSecurityPrewpsCfm_Evt *evt = (NmeSecurityPrewpsCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityPrewpsCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_PREWPS_CFM_ID); \
}

/* -------------------------- nme_security_restart_req -------------------------- */
typedef struct NmeSecurityRestartReq_Evt
{
    FsmEvent                        common;
    CsrUint16                       ieLength;
    CsrUint8                       *ie;
    unifi_SSID                      peerSsid;
    unifi_MACAddress                peerMacAddress;
} NmeSecurityRestartReq_Evt;

#define send_nme_security_restart_req(context, pid, p_ieLength, p_ie, p_peerSsid, p_peerMacAddress) \
{ \
    NmeSecurityRestartReq_Evt *evt = (NmeSecurityRestartReq_Evt*) CsrPmalloc(sizeof(NmeSecurityRestartReq_Evt)); \
    evt->ieLength             = p_ieLength; \
    evt->ie                   = p_ie; \
    evt->peerSsid             = p_peerSsid; \
    evt->peerMacAddress       = p_peerMacAddress; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_RESTART_REQ_ID); \
}

#define send_nme_security_restart_req_external(context, pid, p_ieLength, p_ie, p_peerSsid, p_peerMacAddress) \
{ \
    NmeSecurityRestartReq_Evt *evt = (NmeSecurityRestartReq_Evt*) CsrPmalloc(sizeof(NmeSecurityRestartReq_Evt)); \
    evt->ieLength             = p_ieLength; \
    evt->ie                   = p_ie; \
    evt->peerSsid             = p_peerSsid; \
    evt->peerMacAddress       = p_peerMacAddress; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_RESTART_REQ_ID); \
}

/* -------------------------- nme_security_restart_cfm -------------------------- */
typedef struct NmeSecurityRestartCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} NmeSecurityRestartCfm_Evt;

#define send_nme_security_restart_cfm(context, pid, p_status) \
{ \
    NmeSecurityRestartCfm_Evt *evt = (NmeSecurityRestartCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityRestartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_RESTART_CFM_ID); \
}

#define send_nme_security_restart_cfm_external(context, pid, p_status) \
{ \
    NmeSecurityRestartCfm_Evt *evt = (NmeSecurityRestartCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityRestartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_RESTART_CFM_ID); \
}

/* -------------------------- nme_security_start_req -------------------------- */
typedef struct NmeSecurityStartReq_Evt
{
    FsmEvent                        common;
    CsrUint16                       ieLength;
    CsrUint8                       *ie;
    unifi_SSID                      peerSsid;
    unifi_MACAddress                peerMacAddress;
} NmeSecurityStartReq_Evt;

#define send_nme_security_start_req(context, pid, p_ieLength, p_ie, p_peerSsid, p_peerMacAddress) \
{ \
    NmeSecurityStartReq_Evt *evt = (NmeSecurityStartReq_Evt*) CsrPmalloc(sizeof(NmeSecurityStartReq_Evt)); \
    evt->ieLength             = p_ieLength; \
    evt->ie                   = p_ie; \
    evt->peerSsid             = p_peerSsid; \
    evt->peerMacAddress       = p_peerMacAddress; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_START_REQ_ID); \
}

#define send_nme_security_start_req_external(context, pid, p_ieLength, p_ie, p_peerSsid, p_peerMacAddress) \
{ \
    NmeSecurityStartReq_Evt *evt = (NmeSecurityStartReq_Evt*) CsrPmalloc(sizeof(NmeSecurityStartReq_Evt)); \
    evt->ieLength             = p_ieLength; \
    evt->ie                   = p_ie; \
    evt->peerSsid             = p_peerSsid; \
    evt->peerMacAddress       = p_peerMacAddress; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_START_REQ_ID); \
}

/* -------------------------- nme_security_start_cfm -------------------------- */
typedef struct NmeSecurityStartCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} NmeSecurityStartCfm_Evt;

#define send_nme_security_start_cfm(context, pid, p_status) \
{ \
    NmeSecurityStartCfm_Evt *evt = (NmeSecurityStartCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityStartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_START_CFM_ID); \
}

#define send_nme_security_start_cfm_external(context, pid, p_status) \
{ \
    NmeSecurityStartCfm_Evt *evt = (NmeSecurityStartCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityStartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_START_CFM_ID); \
}

/* -------------------------- nme_security_stop_req -------------------------- */
typedef struct NmeSecurityStopReq_Evt
{
    FsmEvent                        common;
} NmeSecurityStopReq_Evt;

#define send_nme_security_stop_req(context, pid) \
{ \
    NmeSecurityStopReq_Evt *evt = (NmeSecurityStopReq_Evt*) CsrPmalloc(sizeof(NmeSecurityStopReq_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_STOP_REQ_ID); \
}

#define send_nme_security_stop_req_external(context, pid) \
{ \
    NmeSecurityStopReq_Evt *evt = (NmeSecurityStopReq_Evt*) CsrPmalloc(sizeof(NmeSecurityStopReq_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_STOP_REQ_ID); \
}

/* -------------------------- nme_security_stop_cfm -------------------------- */
typedef struct NmeSecurityStopCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} NmeSecurityStopCfm_Evt;

#define send_nme_security_stop_cfm(context, pid, p_status) \
{ \
    NmeSecurityStopCfm_Evt *evt = (NmeSecurityStopCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityStopCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_STOP_CFM_ID); \
}

#define send_nme_security_stop_cfm_external(context, pid, p_status) \
{ \
    NmeSecurityStopCfm_Evt *evt = (NmeSecurityStopCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityStopCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_STOP_CFM_ID); \
}

/* -------------------------- nme_security_terminate_req -------------------------- */
typedef struct NmeSecurityTerminateReq_Evt
{
    FsmEvent                        common;
} NmeSecurityTerminateReq_Evt;

#define send_nme_security_terminate_req(context, pid) \
{ \
    NmeSecurityTerminateReq_Evt *evt = (NmeSecurityTerminateReq_Evt*) CsrPmalloc(sizeof(NmeSecurityTerminateReq_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_TERMINATE_REQ_ID); \
}

#define send_nme_security_terminate_req_external(context, pid) \
{ \
    NmeSecurityTerminateReq_Evt *evt = (NmeSecurityTerminateReq_Evt*) CsrPmalloc(sizeof(NmeSecurityTerminateReq_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_TERMINATE_REQ_ID); \
}

/* -------------------------- nme_security_terminate_cfm -------------------------- */
typedef struct NmeSecurityTerminateCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} NmeSecurityTerminateCfm_Evt;

#define send_nme_security_terminate_cfm(context, pid, p_status) \
{ \
    NmeSecurityTerminateCfm_Evt *evt = (NmeSecurityTerminateCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityTerminateCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, NME_SECURITY_TERMINATE_CFM_ID); \
}

#define send_nme_security_terminate_cfm_external(context, pid, p_status) \
{ \
    NmeSecurityTerminateCfm_Evt *evt = (NmeSecurityTerminateCfm_Evt*) CsrPmalloc(sizeof(NmeSecurityTerminateCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, NME_SECURITY_TERMINATE_CFM_ID); \
}



/* -------------------------- nme_security_eap_timer -------------------------- */
typedef struct NmeSecurityEapTimer_Evt
{
    FsmTimer                        common;
    CsrUint8                        timeoutIndex;
} NmeSecurityEapTimer_Evt;

#define send_nme_security_eap_timer(context, rettimerid, timeValue, timeExtraValue, p_timeoutIndex) \
{ \
    NmeSecurityEapTimer_Evt *evt = (NmeSecurityEapTimer_Evt*) CsrPmalloc(sizeof(NmeSecurityEapTimer_Evt)); \
    evt->timeoutIndex         = p_timeoutIndex; \
    rettimerid = fsm_set_timer((context), (FsmTimer*)evt, timeValue, timeExtraValue, NME_SECURITY_EAP_TIMER_ID);\
}



#ifdef __cplusplus
}
#endif

#endif /* NME_SECURITY_MANAGER_XML_SIGNALS_H */
