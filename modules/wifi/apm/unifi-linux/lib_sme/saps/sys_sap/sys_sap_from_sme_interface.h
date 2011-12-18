/* This is an autogenerated file */
/* Tag: noCheckHeader */

/*    CONFIDENTIAL */
/*    Copyright (C) Cambridge Silicon Radio Ltd 2008. All rights reserved. */

#ifndef SYS_OUTPUT_H
#define SYS_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SME_API_VALIDATE

#define call_unifi_sys_capabilities_req(context, appHandle) CSR_UNUSED(appHandle)
#define call_unifi_sys_configure_power_mode_req(context, mode, wakeHost) CSR_UNUSED(mode), CSR_UNUSED(wakeHost)
#define call_unifi_sys_eapol_req(context, appHandle, subscriptionHandle, frameLength, frame, freeFunction) CSR_UNUSED(appHandle), CSR_UNUSED(subscriptionHandle), CSR_UNUSED(frameLength), CSR_UNUSED(frame), CSR_UNUSED(freeFunction)
#define call_unifi_sys_hip_req(context, mlmeCommandLength, mlmeCommand, dataRef1Length, dataRef1, dataRef2Length, dataRef2) CSR_UNUSED(mlmeCommandLength), CSR_UNUSED(mlmeCommand), CSR_UNUSED(dataRef1Length), CSR_UNUSED(dataRef1), CSR_UNUSED(dataRef2Length), CSR_UNUSED(dataRef2)
#define call_unifi_sys_m4_transmit_req(context) 
#define call_unifi_sys_ma_unitdata_req(context, appHandle, subscriptionHandle, frameLength, frame, freeFunction, priority, serviceClass, reqIdentifier) CSR_UNUSED(appHandle), CSR_UNUSED(subscriptionHandle), CSR_UNUSED(frameLength), CSR_UNUSED(frame), CSR_UNUSED(freeFunction), CSR_UNUSED(priority), CSR_UNUSED(serviceClass), CSR_UNUSED(reqIdentifier)
#define call_unifi_sys_ma_unitdata_rsp(context, subscriptionHandle, result) CSR_UNUSED(subscriptionHandle), CSR_UNUSED(result)
#define call_unifi_sys_ma_unitdata_subscribe_req(context, appHandle, encapsulation, protocol, oui) CSR_UNUSED(appHandle), CSR_UNUSED(encapsulation), CSR_UNUSED(protocol), CSR_UNUSED(oui)
#define call_unifi_sys_ma_unitdata_unsubscribe_req(context, appHandle, subscriptionHandle) CSR_UNUSED(appHandle), CSR_UNUSED(subscriptionHandle)
#define call_unifi_sys_media_status_req(context, mediaStatus, mediaTypeMask) CSR_UNUSED(mediaStatus), CSR_UNUSED(mediaTypeMask)
#define call_unifi_sys_multicast_address_rsp(context, status, action, getAddressesCount, getAddresses) CSR_UNUSED(status), CSR_UNUSED(action), CSR_UNUSED(getAddressesCount), CSR_UNUSED(getAddresses)
#define call_unifi_sys_port_configure_req(context, uncontrolledPortAction, controlledPortAction, macAddress) CSR_UNUSED(uncontrolledPortAction), CSR_UNUSED(controlledPortAction), CSR_UNUSED(macAddress)
#define call_unifi_sys_qos_control_req(context, control) CSR_UNUSED(control)
#define call_unifi_sys_resume_rsp(context, status) CSR_UNUSED(status)
#define call_unifi_sys_suspend_rsp(context, status) CSR_UNUSED(status)
#define call_unifi_sys_tclas_add_req(context, tclasLength, tclas) CSR_UNUSED(tclasLength), CSR_UNUSED(tclas)
#define call_unifi_sys_tclas_del_req(context, tclasLength, tclas) CSR_UNUSED(tclasLength), CSR_UNUSED(tclas)
#define call_unifi_sys_traffic_classification_req(context, trafficType, period) CSR_UNUSED(trafficType), CSR_UNUSED(period)
#define call_unifi_sys_traffic_config_req(context, trafficConfigType, config) CSR_UNUSED(trafficConfigType), CSR_UNUSED(config)
#define call_unifi_sys_wifi_off_req(context, appHandle) CSR_UNUSED(appHandle)
#define call_unifi_sys_wifi_off_rsp(context) 
#define call_unifi_sys_wifi_on_req(context, appHandle) CSR_UNUSED(appHandle)
#define call_unifi_sys_wifi_on_rsp(context, status, stationMacAddress, smeVersions, scheduledInterrupt) CSR_UNUSED(status), CSR_UNUSED(stationMacAddress), CSR_UNUSED(smeVersions), CSR_UNUSED(scheduledInterrupt)

#else

#include "sys_sap/sys_sap.h"

#ifdef FSM_TRANSITION_LOCK

#define call_unifi_sys_capabilities_req(context, p_appHandle) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_CAPABILITIES_REQ appHandle = %p) To(env)",\
        fsm_current_process_name((context)), \
        p_appHandle    ));\
    unifi_sys_capabilities_req(context->externalContext, p_appHandle); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_configure_power_mode_req(context, p_mode, p_wakeHost) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_CONFIGURE_POWER_MODE_REQ mode = %s wakeHost = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_LowPowerMode(p_mode), \
        p_wakeHost?"TRUE":"FALSE"    ));\
    unifi_sys_configure_power_mode_req(context->externalContext, p_mode, p_wakeHost); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_eapol_req(context, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_EAPOL_REQ appHandle = %p) To(env)",\
        fsm_current_process_name((context)), \
        p_appHandle    ));\
    unifi_sys_eapol_req(context->externalContext, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_hip_req(context, p_mlmeCommandLength, p_mlmeCommand, p_dataRef1Length, p_dataRef1, p_dataRef2Length, p_dataRef2) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_HIP_REQ) To(env)",\
        fsm_current_process_name((context))    ));\
    unifi_sys_hip_req(context->externalContext, p_mlmeCommandLength, p_mlmeCommand, p_dataRef1Length, p_dataRef1, p_dataRef2Length, p_dataRef2); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_m4_transmit_req(context) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_M4_TRANSMIT_REQ) To(env)",\
        fsm_current_process_name((context))    ));\
    unifi_sys_m4_transmit_req(context->externalContext); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_ma_unitdata_req(context, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction, p_priority, p_serviceClass, p_reqIdentifier) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_REQ appHandle = %p priority = %s serviceClass = %s) To(env)",\
        fsm_current_process_name((context)), \
        p_appHandle, \
        trace_unifi_Priority(p_priority), \
        trace_unifi_ServiceClass(p_serviceClass)    ));\
    unifi_sys_ma_unitdata_req(context->externalContext, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction, p_priority, p_serviceClass, p_reqIdentifier); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_ma_unitdata_rsp(context, p_subscriptionHandle, p_result) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_RSP result = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_Status(p_result)    ));\
    unifi_sys_ma_unitdata_rsp(context->externalContext, p_subscriptionHandle, p_result); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_ma_unitdata_subscribe_req(context, p_appHandle, p_encapsulation, p_protocol, p_oui) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_SUBSCRIBE_REQ appHandle = %p encapsulation = %s) To(env)",\
        fsm_current_process_name((context)), \
        p_appHandle, \
        trace_unifi_Encapsulation(p_encapsulation)    ));\
    unifi_sys_ma_unitdata_subscribe_req(context->externalContext, p_appHandle, p_encapsulation, p_protocol, p_oui); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_ma_unitdata_unsubscribe_req(context, p_appHandle, p_subscriptionHandle) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_UNSUBSCRIBE_REQ appHandle = %p) To(env)",\
        fsm_current_process_name((context)), \
        p_appHandle    ));\
    unifi_sys_ma_unitdata_unsubscribe_req(context->externalContext, p_appHandle, p_subscriptionHandle); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_media_status_req(context, p_mediaStatus, p_mediaTypeMask) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MEDIA_STATUS_REQ mediaStatus = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_MediaStatus(p_mediaStatus)    ));\
    unifi_sys_media_status_req(context->externalContext, p_mediaStatus, p_mediaTypeMask); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_multicast_address_rsp(context, p_status, p_action, p_getAddressesCount, p_getAddresses) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MULTICAST_ADDRESS_RSP status = %s action = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_Status(p_status), \
        trace_unifi_ListAction(p_action)    ));\
    unifi_sys_multicast_address_rsp(context->externalContext, p_status, p_action, p_getAddressesCount, p_getAddresses); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_port_configure_req(context, p_uncontrolledPortAction, p_controlledPortAction, p_macAddress) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_PORT_CONFIGURE_REQ uncontrolledPortAction = %s controlledPortAction = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_PortAction(p_uncontrolledPortAction), \
        trace_unifi_PortAction(p_controlledPortAction)    ));\
    unifi_sys_port_configure_req(context->externalContext, p_uncontrolledPortAction, p_controlledPortAction, p_macAddress); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_qos_control_req(context, p_control) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_QOS_CONTROL_REQ control = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_QoSControl(p_control)    ));\
    unifi_sys_qos_control_req(context->externalContext, p_control); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_resume_rsp(context, p_status) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_RESUME_RSP status = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_Status(p_status)    ));\
    unifi_sys_resume_rsp(context->externalContext, p_status); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_suspend_rsp(context, p_status) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_SUSPEND_RSP status = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_Status(p_status)    ));\
    unifi_sys_suspend_rsp(context->externalContext, p_status); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_tclas_add_req(context, p_tclasLength, p_tclas) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TCLAS_ADD_REQ) To(env)",\
        fsm_current_process_name((context))    ));\
    unifi_sys_tclas_add_req(context->externalContext, p_tclasLength, p_tclas); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_tclas_del_req(context, p_tclasLength, p_tclas) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TCLAS_DEL_REQ) To(env)",\
        fsm_current_process_name((context))    ));\
    unifi_sys_tclas_del_req(context->externalContext, p_tclasLength, p_tclas); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_traffic_classification_req(context, p_trafficType, p_period) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TRAFFIC_CLASSIFICATION_REQ trafficType = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_TrafficType(p_trafficType)    ));\
    unifi_sys_traffic_classification_req(context->externalContext, p_trafficType, p_period); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_traffic_config_req(context, p_trafficConfigType, p_config) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TRAFFIC_CONFIG_REQ trafficConfigType = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_TrafficConfigType(p_trafficConfigType)    ));\
    unifi_sys_traffic_config_req(context->externalContext, p_trafficConfigType, p_config); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_wifi_off_req(context, p_appHandle) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_OFF_REQ appHandle = %p) To(env)",\
        fsm_current_process_name((context)), \
        p_appHandle    ));\
    unifi_sys_wifi_off_req(context->externalContext, p_appHandle); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_wifi_off_rsp(context) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_OFF_RSP) To(env)",\
        fsm_current_process_name((context))    ));\
    unifi_sys_wifi_off_rsp(context->externalContext); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_wifi_on_req(context, p_appHandle) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_ON_REQ appHandle = %p) To(env)",\
        fsm_current_process_name((context)), \
        p_appHandle    ));\
    unifi_sys_wifi_on_req(context->externalContext, p_appHandle); \
    (void)CsrMutexLock(context->transitionLock);

#define call_unifi_sys_wifi_on_rsp(context, p_status, p_stationMacAddress, p_smeVersions, p_scheduledInterrupt) \
    (void)CsrMutexUnlock(context->transitionLock); \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_ON_RSP status = %s scheduledInterrupt = %s) To(env)",\
        fsm_current_process_name((context)), \
        trace_unifi_Status(p_status), \
        p_scheduledInterrupt?"TRUE":"FALSE"    ));\
    unifi_sys_wifi_on_rsp(context->externalContext, p_status, p_stationMacAddress, p_smeVersions, p_scheduledInterrupt); \
    (void)CsrMutexLock(context->transitionLock);


#else

#define call_unifi_sys_capabilities_req(context, p_appHandle) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_CAPABILITIES_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_capabilities_req(context->externalContext, p_appHandle);

#define call_unifi_sys_configure_power_mode_req(context, p_mode, p_wakeHost) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_CONFIGURE_POWER_MODE_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_configure_power_mode_req(context->externalContext, p_mode, p_wakeHost);

#define call_unifi_sys_eapol_req(context, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_EAPOL_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_eapol_req(context->externalContext, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction);

#define call_unifi_sys_hip_req(context, p_mlmeCommandLength, p_mlmeCommand, p_dataRef1Length, p_dataRef1, p_dataRef2Length, p_dataRef2) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_HIP_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_hip_req(context->externalContext, p_mlmeCommandLength, p_mlmeCommand, p_dataRef1Length, p_dataRef1, p_dataRef2Length, p_dataRef2);

#define call_unifi_sys_m4_transmit_req(context) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_M4_TRANSMIT_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_m4_transmit_req(context->externalContext);

#define call_unifi_sys_ma_unitdata_req(context, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction, p_priority, p_serviceClass, p_reqIdentifier) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_ma_unitdata_req(context->externalContext, p_appHandle, p_subscriptionHandle, p_frameLength, p_frame, p_freeFunction, p_priority, p_serviceClass, p_reqIdentifier);

#define call_unifi_sys_ma_unitdata_rsp(context, p_subscriptionHandle, p_result) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_RSP) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_ma_unitdata_rsp(context->externalContext, p_subscriptionHandle, p_result);

#define call_unifi_sys_ma_unitdata_subscribe_req(context, p_appHandle, p_encapsulation, p_protocol, p_oui) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_SUBSCRIBE_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_ma_unitdata_subscribe_req(context->externalContext, p_appHandle, p_encapsulation, p_protocol, p_oui);

#define call_unifi_sys_ma_unitdata_unsubscribe_req(context, p_appHandle, p_subscriptionHandle) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MA_UNITDATA_UNSUBSCRIBE_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_ma_unitdata_unsubscribe_req(context->externalContext, p_appHandle, p_subscriptionHandle);

#define call_unifi_sys_media_status_req(context, p_mediaStatus, p_mediaTypeMask) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MEDIA_STATUS_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_media_status_req(context->externalContext, p_mediaStatus, p_mediaTypeMask);

#define call_unifi_sys_multicast_address_rsp(context, p_status, p_action, p_getAddressesCount, p_getAddresses) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_MULTICAST_ADDRESS_RSP) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_multicast_address_rsp(context->externalContext, p_status, p_action, p_getAddressesCount, p_getAddresses);

#define call_unifi_sys_port_configure_req(context, p_uncontrolledPortAction, p_controlledPortAction, p_macAddress) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_PORT_CONFIGURE_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_port_configure_req(context->externalContext, p_uncontrolledPortAction, p_controlledPortAction, p_macAddress);

#define call_unifi_sys_qos_control_req(context, p_control) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_QOS_CONTROL_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_qos_control_req(context->externalContext, p_control);

#define call_unifi_sys_resume_rsp(context, p_status) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_RESUME_RSP) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_resume_rsp(context->externalContext, p_status);

#define call_unifi_sys_suspend_rsp(context, p_status) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_SUSPEND_RSP) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_suspend_rsp(context->externalContext, p_status);

#define call_unifi_sys_tclas_add_req(context, p_tclasLength, p_tclas) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TCLAS_ADD_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_tclas_add_req(context->externalContext, p_tclasLength, p_tclas);

#define call_unifi_sys_tclas_del_req(context, p_tclasLength, p_tclas) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TCLAS_DEL_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_tclas_del_req(context->externalContext, p_tclasLength, p_tclas);

#define call_unifi_sys_traffic_classification_req(context, p_trafficType, p_period) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TRAFFIC_CLASSIFICATION_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_traffic_classification_req(context->externalContext, p_trafficType, p_period);

#define call_unifi_sys_traffic_config_req(context, p_trafficConfigType, p_config) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_TRAFFIC_CONFIG_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_traffic_config_req(context->externalContext, p_trafficConfigType, p_config);

#define call_unifi_sys_wifi_off_req(context, p_appHandle) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_OFF_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_wifi_off_req(context->externalContext, p_appHandle);

#define call_unifi_sys_wifi_off_rsp(context) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_OFF_RSP) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_wifi_off_rsp(context->externalContext);

#define call_unifi_sys_wifi_on_req(context, p_appHandle) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_ON_REQ) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_wifi_on_req(context->externalContext, p_appHandle);

#define call_unifi_sys_wifi_on_rsp(context, p_status, p_stationMacAddress, p_smeVersions, p_scheduledInterrupt) \
    sme_trace_msc((TR_MSC, "MSC MESSAGE :: From(%s) Event(UNIFI_SYS_WIFI_ON_RSP) To(env)", \
                   fsm_current_process_name((context)))); \
    unifi_sys_wifi_on_rsp(context->externalContext, p_status, p_stationMacAddress, p_smeVersions, p_scheduledInterrupt);


#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* SYS_OUTPUT_H */

