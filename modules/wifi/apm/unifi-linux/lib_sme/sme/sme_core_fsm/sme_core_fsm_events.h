/* This is an autogenerated file */
/* Tag: noCheckHeader */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2009. All rights reserved. */
/* */
/*   Refer to LICENSE.txt included with this source for details on the */
/*   license terms. */

#ifndef SME_CORE_XML_SIGNALS_H
#define SME_CORE_XML_SIGNALS_H

#ifdef __cplusplus
extern "C" {
#endif

#define CORE_START_REQ_ID                                            0x3901
#define CORE_START_CFM_ID                                            0x3902
#define CORE_STOP_REQ_ID                                             0x3903
#define CORE_STOP_CFM_ID                                             0x3904
#define INTERNAL_STOP_REQ_ID                                         0x3905
#define D3_TRANSISTION_COMPLETE_REQ_ID                               0x3906


/* -------------------------- core_start_req -------------------------- */
typedef struct CoreStartReq_Evt
{
    FsmEvent                        common;
    CsrBool                         resume;
} CoreStartReq_Evt;

#define send_core_start_req(context, pid, p_resume) \
{ \
    CoreStartReq_Evt *evt = (CoreStartReq_Evt*) CsrPmalloc(sizeof(CoreStartReq_Evt)); \
    evt->resume               = p_resume; \
    fsm_send_event(context, (FsmEvent*)evt, pid, CORE_START_REQ_ID); \
}

#define send_core_start_req_external(context, pid, p_resume) \
{ \
    CoreStartReq_Evt *evt = (CoreStartReq_Evt*) CsrPmalloc(sizeof(CoreStartReq_Evt)); \
    evt->resume               = p_resume; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, CORE_START_REQ_ID); \
}

/* -------------------------- core_start_cfm -------------------------- */
typedef struct CoreStartCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} CoreStartCfm_Evt;

#define send_core_start_cfm(context, pid, p_status) \
{ \
    CoreStartCfm_Evt *evt = (CoreStartCfm_Evt*) CsrPmalloc(sizeof(CoreStartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, CORE_START_CFM_ID); \
}

#define send_core_start_cfm_external(context, pid, p_status) \
{ \
    CoreStartCfm_Evt *evt = (CoreStartCfm_Evt*) CsrPmalloc(sizeof(CoreStartCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, CORE_START_CFM_ID); \
}

/* -------------------------- core_stop_req -------------------------- */
typedef struct CoreStopReq_Evt
{
    FsmEvent                        common;
    CsrBool                         suspend;
} CoreStopReq_Evt;

#define send_core_stop_req(context, pid, p_suspend) \
{ \
    CoreStopReq_Evt *evt = (CoreStopReq_Evt*) CsrPmalloc(sizeof(CoreStopReq_Evt)); \
    evt->suspend              = p_suspend; \
    fsm_send_event(context, (FsmEvent*)evt, pid, CORE_STOP_REQ_ID); \
}

#define send_core_stop_req_external(context, pid, p_suspend) \
{ \
    CoreStopReq_Evt *evt = (CoreStopReq_Evt*) CsrPmalloc(sizeof(CoreStopReq_Evt)); \
    evt->suspend              = p_suspend; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, CORE_STOP_REQ_ID); \
}

/* -------------------------- core_stop_cfm -------------------------- */
typedef struct CoreStopCfm_Evt
{
    FsmEvent                        common;
    unifi_Status                    status;
} CoreStopCfm_Evt;

#define send_core_stop_cfm(context, pid, p_status) \
{ \
    CoreStopCfm_Evt *evt = (CoreStopCfm_Evt*) CsrPmalloc(sizeof(CoreStopCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event(context, (FsmEvent*)evt, pid, CORE_STOP_CFM_ID); \
}

#define send_core_stop_cfm_external(context, pid, p_status) \
{ \
    CoreStopCfm_Evt *evt = (CoreStopCfm_Evt*) CsrPmalloc(sizeof(CoreStopCfm_Evt)); \
    evt->status               = p_status; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, CORE_STOP_CFM_ID); \
}

/* -------------------------- d3_transistion_complete_req -------------------------- */
typedef struct D3TransistionCompleteReq_Evt
{
    FsmEvent                        common;
    CsrBool                         suspend;
} D3TransistionCompleteReq_Evt;

#define send_d3_transistion_complete_req(context, pid, p_suspend) \
{ \
    D3TransistionCompleteReq_Evt *evt = (D3TransistionCompleteReq_Evt*) CsrPmalloc(sizeof(D3TransistionCompleteReq_Evt)); \
    evt->suspend              = p_suspend; \
    fsm_send_event(context, (FsmEvent*)evt, pid, D3_TRANSISTION_COMPLETE_REQ_ID); \
}

#define send_d3_transistion_complete_req_external(context, pid, p_suspend) \
{ \
    D3TransistionCompleteReq_Evt *evt = (D3TransistionCompleteReq_Evt*) CsrPmalloc(sizeof(D3TransistionCompleteReq_Evt)); \
    evt->suspend              = p_suspend; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, D3_TRANSISTION_COMPLETE_REQ_ID); \
}

/* -------------------------- internal_stop_req -------------------------- */
typedef struct InternalStopReq_Evt
{
    FsmEvent                        common;
    unifi_ControlIndication         reason;
} InternalStopReq_Evt;

#define send_internal_stop_req(context, pid, p_reason) \
{ \
    InternalStopReq_Evt *evt = (InternalStopReq_Evt*) CsrPmalloc(sizeof(InternalStopReq_Evt)); \
    evt->reason               = p_reason; \
    fsm_send_event(context, (FsmEvent*)evt, pid, INTERNAL_STOP_REQ_ID); \
}

#define send_internal_stop_req_external(context, pid, p_reason) \
{ \
    InternalStopReq_Evt *evt = (InternalStopReq_Evt*) CsrPmalloc(sizeof(InternalStopReq_Evt)); \
    evt->reason               = p_reason; \
    fsm_send_event_external(context, (FsmEvent*)evt, pid, INTERNAL_STOP_REQ_ID); \
}





#ifdef __cplusplus
}
#endif

#endif /* SME_CORE_XML_SIGNALS_H */
