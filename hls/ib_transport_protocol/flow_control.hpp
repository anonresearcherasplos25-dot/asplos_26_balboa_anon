// Anonymous contribution
#pragma once

#include "../../axi_utils.hpp"
#include "../ib_transport_protocol.hpp"
#include "hls_math.h"
using namespace hls;

const uint32_t FLOW_ORDER = 5;
const uint32_t FLOW_OUTSTANDING = 32;//exp2(FLOW_OUTSTANDING_ORDER);

struct flowTableEntry
{
	ap_uint<FLOW_ORDER> head;
    ap_uint<FLOW_ORDER> tail; 
    ap_uint<1> issued;
    ap_uint<FLOW_OUTSTANDING> rd_active;
    ap_uint<24> curr_ssn;
    ap_uint<24> curr_msn;
};

template <int INSTID = 0>
void flow_control(	
    stream<flowUpdReq>&	req2flow_upd,
    stream<flowUpdAck>&	ack2flow_upd,
    stream<flowTableEntry>& flow2req_rsp,
    stream<flowTableEntry>&	flow2ack_rsp
) {
#pragma HLS PIPELINE II=1
#pragma HLS INLINE off

	static flowTableEntry flow_table[MAX_QPS];
#if defined( __VITIS_HLS__)
	#pragma HLS bind_storage variable=flow_table type=RAM_2P impl=BRAM
#else
	#pragma HLS RESOURCE variable=flow_table core=RAM_2P_BRAM
#endif

    flowUpdAck ackUpd;

    if (!ack2flow_upd.empty()) {
        ack2flow_upd.read(ackUpd);
        if(ackUpd.write) {
            flow_table[ackUpd.qpn].tail = ackUpd.tail;
            flow_table[ackUpd.qpn].curr_msn = ackUpd.curr_msn;
            if(ackUpd.tail == flow_table[ackUpd.qpn].head)
                flow_table[ackUpd.qpn].issued = 0;
        } else {
            flow2ack_rsp.write(flow_table[ackUpd.qpn]);
        }
    } else if (!req2flow_upd.empty()) {
        req2flow_upd.read(reqUpd);
        if(reqUpd.write) {

        } else {

        }
    }
}