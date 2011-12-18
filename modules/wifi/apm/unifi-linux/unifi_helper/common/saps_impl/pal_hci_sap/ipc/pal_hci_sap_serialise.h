/***************************************************************************
 *
 * This is an AUTOGENERATED file from genio_pack_interface_c.pl
 *
 *    CONFIDENTIAL
 *    Copyright (C) Cambridge Silicon Radio Ltd 2007. All rights reserved.
 */

/***************************************************************************
 * Description:
 *       The following functions are used to convert primitives, commands or
 *       in a packed stream of bytes and send it to a choosen destination.
 *
 * Options used to generate this file:
 * perl genio_sap_serialise.pl
 *       -o /home/bfsw/host/releases/unifi_hostsoftware_212/build/unifi-driver-212/host/unifi_helper/unifi_sme/../common/saps_impl/pal_hci_sap/ipc/pal_hci_sap_serialise.h
 *       -x ../../common/hciio/../amp_hci/bt_amp_hci.xml
 *       -d up
 *       -c Event_Code
 *       -i PAL_HCI
 *       -h HCI_Event_Header
 *
 ***************************************************************************/
#include "fsm/csr_wifi_fsm.h" 
#include "sme_trace/sme_trace.h"
#include "sme_trace/sme_pbc.h"
#include "pal_hci_sap/pal_hci_sap_types.h" 
#include "pal_hci_sap/pal_hci_sap_signals.h" 

CsrUint16 serialise_hci_command_complete_evt(CsrUint8** resultBuffer, CsrUint8 numHciCommandPackets, const ReturnParameters* returnParameters);
CsrUint16 serialise_hci_command_status_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint8 numHciCommandPackets, HciOpcode commandOpcode);
CsrUint16 serialise_hci_hardware_error_evt(CsrUint8** resultBuffer, CsrUint8 hardwareCode);
CsrUint16 serialise_hci_flush_occurred_evt(CsrUint8** resultBuffer, CsrUint16 handle);
CsrUint16 serialise_hci_loopback_command_evt(CsrUint8** resultBuffer, HciCommandPacket hciCommandPacket);
CsrUint16 serialise_hci_data_buffer_overflow_evt(CsrUint8** resultBuffer, HciLinkType linkType);
CsrUint16 serialise_hci_qos_violation_evt(CsrUint8** resultBuffer, CsrUint16 handle);
CsrUint16 serialise_hci_enhanced_flush_complete_evt(CsrUint8** resultBuffer, CsrUint16 handle);
CsrUint16 serialise_hci_physical_link_complete_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint8 physicalLinkHandle);
CsrUint16 serialise_hci_channel_select_evt(CsrUint8** resultBuffer, CsrUint8 physicalLinkHandle);
CsrUint16 serialise_hci_disconnect_physical_link_complete_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint8 physicalLinkHandle, CsrUint8 reason);
CsrUint16 serialise_hci_physical_link_loss_early_warning_evt(CsrUint8** resultBuffer, CsrUint8 physicalLinkHandle, LinkLossReasonCode linkLossReason);
CsrUint16 serialise_hci_physical_link_recovery_evt(CsrUint8** resultBuffer, CsrUint8 physicalLinkHandle);
CsrUint16 serialise_hci_logical_link_complete_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint16 logicalLinkHandle, CsrUint8 physicalLinkHandle, CsrUint8 txFlowSpecId);
CsrUint16 serialise_hci_disconnect_logical_link_complete_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint16 logicalLinkHandle, CsrUint8 reason);
CsrUint16 serialise_hci_flow_spec_modify_complete_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint16 handle);
CsrUint16 serialise_hci_number_of_completed_data_blocks_evt(CsrUint8** resultBuffer, CsrUint16 totalNumDataBlocks, CsrUint8 numberOfHandles, CsrUint16 handle, CsrUint16 numberOfCompletedPackets, CsrUint16 numberOfCompletedBlocks);
CsrUint16 serialise_hci_short_range_mode_change_complete_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint8 physicalLinkHandle, ShortRangeMode shortRangeModeState);
CsrUint16 serialise_hci_amp_status_change_evt(CsrUint8** resultBuffer, HciStatusCode status, AmpStatus ampStatus);
CsrUint16 serialise_hci_generic_amp_link_key_nt(CsrUint8** resultBuffer, const BdAddr* bdAddr, const GenericAmpLinkKey* genericAmpLinkKey, LinkKeyType keyType);
CsrUint16 serialise_amp_transmit_power_evt(CsrUint8** resultBuffer, HciStatusCode status, PowerLevel powerLevel);
CsrUint16 serialise_amp_receiver_report_evt(CsrUint8** resultBuffer, CsrUint8 ampReport);
CsrUint16 serialise_amp_start_transmitter_test_evt(CsrUint8** resultBuffer, HciStatusCode status, CsrUint8 powerLevel);
CsrUint16 serialise_amp_start_receiver_test_evt(CsrUint8** resultBuffer, HciStatusCode status);

