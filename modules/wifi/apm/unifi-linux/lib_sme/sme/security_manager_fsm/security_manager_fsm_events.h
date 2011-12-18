/* This is an autogenerated file */
/* Tag: noCheckHeader */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2009. All rights reserved. */
/* */
/*   Refer to LICENSE.txt included with this source for details on the */
/*   license terms. */

#ifndef SECURITY_MANAGER_XML_SIGNALS_H
#define SECURITY_MANAGER_XML_SIGNALS_H

#ifdef __cplusplus
extern "C" {
#endif

#define SECURITY_JOIN_START_REQ_ID                                   0x3400
#define SECURITY_JOIN_START_CFM_ID                                   0x3401
#define SECURITY_CONNECT_START_REQ_ID                                0x3402
#define SECURITY_CONNECT_START_CFM_ID                                0x3403
#define SECURITY_DISCONNECTED_IND_ID                                 0x3404
#define SECURITY_CONNECT_STOP_REQ_ID                                 0x3405
#define SECURITY_CONNECT_STOP_CFM_ID                                 0x3406
#define SECURITY_SETKEY_REQ_ID                                       0x3407
#define SECURITY_MLME_MICHAEL_MIC_FAILURE_TIMER_ID                   0x3408
#define SECURITY_TERMINATE_REQ_ID                                    0x3409


/* -------------------------- security_connect_start_req -------------------------- */
typedef struct SecurityConnectStartReq_Evt
{
    FsmEvent                        common;
} SecurityConnectStartReq_Evt;

#define send_security_connect_start_req(context, pid) \
{ \
    SecurityConnectStartReq_Evt *evt = (SecurityConnectStartReq_Evt*) CsrPmalloc(sizeof(SecurityConnectStartReq_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_START_REQ_ID); \
}

#define send_security_connect_start_req_external(context, pid) \
{ \
    SecurityConnectStartReq_Evt *evt = (SecurityConnectStartReq_Evt*) CsrPmalloc(sizeof(SecurityConnectStartReq_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_START_REQ_ID); \
}

/* -------------------------- security_connect_start_cfm -------------------------- */
typedef struct SecurityConnectStartCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} SecurityConnectStartCfm_Evt;

#define send_security_connect_start_cfm(context, pid, p_status) \
{ \
    SecurityConnectStartCfm_Evt *evt = (SecurityConnectStartCfm_Evt*) CsrPmalloc(sizeof(SecurityConnectStartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_START_CFM_ID); \
}

#define send_security_connect_start_cfm_external(context, pid, p_status) \
{ \
    SecurityConnectStartCfm_Evt *evt = (SecurityConnectStartCfm_Evt*) CsrPmalloc(sizeof(SecurityConnectStartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_START_CFM_ID); \
}

/* -------------------------- security_connect_stop_req -------------------------- */
typedef struct SecurityConnectStopReq_Evt
{
    FsmEvent                        common;
} SecurityConnectStopReq_Evt;

#define send_security_connect_stop_req(context, pid) \
{ \
    SecurityConnectStopReq_Evt *evt = (SecurityConnectStopReq_Evt*) CsrPmalloc(sizeof(SecurityConnectStopReq_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_STOP_REQ_ID); \
}

#define send_security_connect_stop_req_external(context, pid) \
{ \
    SecurityConnectStopReq_Evt *evt = (SecurityConnectStopReq_Evt*) CsrPmalloc(sizeof(SecurityConnectStopReq_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_STOP_REQ_ID); \
}

/* -------------------------- security_connect_stop_cfm -------------------------- */
typedef struct SecurityConnectStopCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} SecurityConnectStopCfm_Evt;

#define send_security_connect_stop_cfm(context, pid, p_status) \
{ \
    SecurityConnectStopCfm_Evt *evt = (SecurityConnectStopCfm_Evt*) CsrPmalloc(sizeof(SecurityConnectStopCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_STOP_CFM_ID); \
}

#define send_security_connect_stop_cfm_external(context, pid, p_status) \
{ \
    SecurityConnectStopCfm_Evt *evt = (SecurityConnectStopCfm_Evt*) CsrPmalloc(sizeof(SecurityConnectStopCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_CONNECT_STOP_CFM_ID); \
}

/* -------------------------- security_disconnected_ind -------------------------- */
typedef struct SecurityDisconnectedInd_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} SecurityDisconnectedInd_Evt;

#define send_security_disconnected_ind(context, pid, p_status) \
{ \
    SecurityDisconnectedInd_Evt *evt = (SecurityDisconnectedInd_Evt*) CsrPmalloc(sizeof(SecurityDisconnectedInd_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_DISCONNECTED_IND_ID); \
}

#define send_security_disconnected_ind_external(context, pid, p_status) \
{ \
    SecurityDisconnectedInd_Evt *evt = (SecurityDisconnectedInd_Evt*) CsrPmalloc(sizeof(SecurityDisconnectedInd_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_DISCONNECTED_IND_ID); \
}

/* -------------------------- security_join_start_req -------------------------- */
typedef struct SecurityJoinStartReq_Evt
{
    FsmEvent                        common;
} SecurityJoinStartReq_Evt;

#define send_security_join_start_req(context, pid) \
{ \
    SecurityJoinStartReq_Evt *evt = (SecurityJoinStartReq_Evt*) CsrPmalloc(sizeof(SecurityJoinStartReq_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_JOIN_START_REQ_ID); \
}

#define send_security_join_start_req_external(context, pid) \
{ \
    SecurityJoinStartReq_Evt *evt = (SecurityJoinStartReq_Evt*) CsrPmalloc(sizeof(SecurityJoinStartReq_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_JOIN_START_REQ_ID); \
}

/* -------------------------- security_join_start_cfm -------------------------- */
typedef struct SecurityJoinStartCfm_Evt
{
    FsmEvent                        common;
    DataReference                   dataref;
    unifi_Status                    status;
} SecurityJoinStartCfm_Evt;

#define send_security_join_start_cfm(context, pid, p_dataref, p_status) \
{ \
    SecurityJoinStartCfm_Evt *evt = (SecurityJoinStartCfm_Evt*) CsrPmalloc(sizeof(SecurityJoinStartCfm_Evt)); \
    evt->dataref              = p_dataref; \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_JOIN_START_CFM_ID); \
}

#define send_security_join_start_cfm_external(context, pid, p_dataref, p_status) \
{ \
    SecurityJoinStartCfm_Evt *evt = (SecurityJoinStartCfm_Evt*) CsrPmalloc(sizeof(SecurityJoinStartCfm_Evt)); \
    evt->dataref              = p_dataref; \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_JOIN_START_CFM_ID); \
}

/* -------------------------- security_setkey_req -------------------------- */
typedef struct SecuritySetkeyReq_Evt
{
    FsmEvent                        common;
    DataReference                   dataref;
    KeyType                         keyType;
    CipherSuiteSelector             cipherSuiteSelector;
    CsrUint8                        keyIndex;
} SecuritySetkeyReq_Evt;

#define send_security_setkey_req(context, pid, p_dataref, p_keyType, p_cipherSuiteSelector, p_keyIndex) \
{ \
    SecuritySetkeyReq_Evt *evt = (SecuritySetkeyReq_Evt*) CsrPmalloc(sizeof(SecuritySetkeyReq_Evt)); \
    evt->dataref              = p_dataref; \
    evt->keyType              = p_keyType; \
    evt->cipherSuiteSelector  = p_cipherSuiteSelector; \
    evt->keyIndex             = p_keyIndex; \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_SETKEY_REQ_ID); \
}

#define send_security_setkey_req_external(context, pid, p_dataref, p_keyType, p_cipherSuiteSelector, p_keyIndex) \
{ \
    SecuritySetkeyReq_Evt *evt = (SecuritySetkeyReq_Evt*) CsrPmalloc(sizeof(SecuritySetkeyReq_Evt)); \
    evt->dataref              = p_dataref; \
    evt->keyType              = p_keyType; \
    evt->cipherSuiteSelector  = p_cipherSuiteSelector; \
    evt->keyIndex             = p_keyIndex; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_SETKEY_REQ_ID); \
}

/* -------------------------- security_terminate_req -------------------------- */
typedef struct SecurityTerminateReq_Evt
{
    FsmEvent                        common;
} SecurityTerminateReq_Evt;

#define send_security_terminate_req(context, pid) \
{ \
    SecurityTerminateReq_Evt *evt = (SecurityTerminateReq_Evt*) CsrPmalloc(sizeof(SecurityTerminateReq_Evt)); \
    fsm_send_event(context, (FsmEvent*)evt, pid, SECURITY_TERMINATE_REQ_ID); \
}

#define send_security_terminate_req_external(context, pid) \
{ \
    SecurityTerminateReq_Evt *evt = (SecurityTerminateReq_Evt*) CsrPmalloc(sizeof(SecurityTerminateReq_Evt)); \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, SECURITY_TERMINATE_REQ_ID); \
}



/* -------------------------- security_mlme_michael_mic_failure_timer -------------------------- */
typedef struct SecurityMlmeMichaelMicFailureTimer_Evt
{
    FsmTimer                        common;
} SecurityMlmeMichaelMicFailureTimer_Evt;

#define send_security_mlme_michael_mic_failure_timer(context, rettimerid, timeValue, timeExtraValue) \
{ \
    rettimerid = fsm_set_timer((context), (FsmTimer*)NULL, timeValue, timeExtraValue, SECURITY_MLME_MICHAEL_MIC_FAILURE_TIMER_ID);\
}



#ifdef __cplusplus
}
#endif

#endif /* SECURITY_MANAGER_XML_SIGNALS_H */
