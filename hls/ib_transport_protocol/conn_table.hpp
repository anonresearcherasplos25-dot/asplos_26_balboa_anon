// Anonymous contribution
#pragma once

#include "../axi_utils.hpp"
#include "../ib_transport_protocol/ib_transport_protocol.hpp"
#include <rocev2_config.hpp> //defines MAX_QPS

struct connTableEntry
{
	//ap_uint<24> local_qpn;
	ap_uint<24> remote_qpn;
	ap_uint<128> remote_ip_address; //TODO make variable
	ap_uint<16> remote_udp_port; //TODO what is this used for
};

template <int INSTID>
void conn_table(	hls::stream<ap_uint<16> >&	tx_ibhconnTable_req,
						hls::stream<ifConnReq>&		qpi2connTable_req,
						hls::stream<connTableEntry>&	tx_connTable2ibh_rsp);


template <int INSTID>
void conn_table(	hls::stream<ap_uint<16> >&	tx_ibhconnTable_req,
						hls::stream<ifConnReq>&		qpi2connTable_req,
						hls::stream<connTableEntry>&	tx_connTable2ibh_rsp)
{
#pragma HLS PIPELINE II=1
#pragma HLS INLINE off

	static connTableEntry conn_table[MAX_QPS];
	//#pragma HLS RESOURCE variable=conn_table core=RAM_2P_BRAM

	ap_uint<16> txRequest;
	ifConnReq ifRequest;

	if (!tx_ibhconnTable_req.empty())
	{
		tx_ibhconnTable_req.read(txRequest);
		//std::cout << "Requested conn data for: " << txRequest << std::endl;
		tx_connTable2ibh_rsp.write(conn_table[txRequest]);
	}
	else if (!qpi2connTable_req.empty())
	{
		qpi2connTable_req.read(ifRequest);
		//std::cout << "Storing conn data for: " << ifRequest.qpn << std::endl;
		conn_table[ifRequest.qpn].remote_qpn = ifRequest.remote_qpn;
		conn_table[ifRequest.qpn].remote_ip_address = ifRequest.remote_ip_address;
		conn_table[ifRequest.qpn].remote_udp_port = ifRequest.remote_udp_port;
	}
}
