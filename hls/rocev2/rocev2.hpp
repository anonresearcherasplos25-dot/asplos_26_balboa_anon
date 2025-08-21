// Anonymous contribution
#pragma once

#define IP_VERSION 4
// #define RETRANS_EN

#include "../axi_utils.hpp"
#include "../ipv4/ipv4.hpp"
#include "../ipv6/ipv6.hpp"
#include "../udp/udp.hpp"
#include "../crc/crc.hpp"
#include "../ib_transport_protocol/ib_transport_protocol.hpp"
//#include "../pointer_chasing/pointer_chasing.hpp"

#if IP_VERSION == 6
typedef ipv6Meta ipMeta;
#else
typedef ipv4Meta ipMeta;
#endif

template <int WIDTH, int INSTID>
void rocev2(
	hls::stream<net_axis<WIDTH> >& s_axis_rx_data,
	hls::stream<net_axis<WIDTH> >&	m_axis_tx_data,

	// S(R)Q
	hls::stream<txMeta>& s_axis_sq_meta,

	// ACKs
	stream<ackMeta>& m_axis_rx_ack_meta,

	// RDMA
	hls::stream<memCmd>& m_axis_mem_write_cmd,
	hls::stream<memCmd>& m_axis_mem_read_cmd,
	hls::stream<net_axis<WIDTH> >& m_axis_mem_write_data,
	hls::stream<net_axis<WIDTH> >& s_axis_mem_read_data,
	
	// QP
	hls::stream<qpContext>&	s_axis_qp_interface,
	hls::stream<ifConnReq>&	s_axis_qp_conn_interface,
	ap_uint<128> local_ip_address,
	
	// Debug
#ifdef DBG_IBV
	hls::stream<psnPkg>& m_axis_dbg,
#endif 
	ap_uint<32>& regCrcDropPkgCount,
	ap_uint<32>& regInvalidPsnDropCount,
    ap_uint<32>& regRetransCount,
	ap_uint<32>& regIbvCountRx,
    ap_uint<32>& regIbvCountTx
);

