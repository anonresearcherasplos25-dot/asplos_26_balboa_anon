// Anonymous contribution
#pragma once

#include "../axi_utils.hpp"
#include <rocev2_config.hpp> //defines MAX_QPS
using namespace hls;

struct txReadReqUpdate
{
	ap_uint<16> qpn;
	ap_uint<24> max_fwd_readreq;
	txReadReqUpdate() {}
	txReadReqUpdate(ap_uint<16> qpn, ap_uint<24> maxf)
		:qpn(qpn), max_fwd_readreq(maxf) {}
};

struct rxReadReqUpdate
{
	ap_uint<16> qpn;
	ap_uint<24> oldest_outstanding_readreq;
	bool write;
	rxReadReqUpdate() : write(false) {}
	rxReadReqUpdate(ap_uint<16> qpn)
		:qpn(qpn), oldest_outstanding_readreq(0), write(false) {}
	rxReadReqUpdate(ap_uint<16> qpn, ap_uint<24> old)
		:qpn(qpn), oldest_outstanding_readreq(old), write(true) {}
};

struct rxReadReqRsp
{
	ap_uint<24> oldest_outstanding_readreq;
	bool valid;
	rxReadReqRsp() {}
	rxReadReqRsp(ap_uint<24> old, bool valid)
		:oldest_outstanding_readreq(old), valid(valid) {}

};

struct readReqTableEntry
{
	ap_uint<24> oldest_outstanding_readreq;
	ap_uint<24> max_fwd_readreq;
};

template <int INSTID>
void read_req_table(stream<txReadReqUpdate>&	tx_readReqTable_upd,
#ifdef RETRANS_EN
					stream<rxReadReqUpdate>&	rx_readReqTable_upd_req,
					stream<rxReadReqRsp>&		rx_readReqTable_upd_rsp);
#else
					stream<rxReadReqUpdate>&	rx_readReqTable_upd_req);
#endif

template <int INSTID = 0>
void read_req_table(stream<txReadReqUpdate>&	tx_readReqTable_upd,
#ifdef RETRANS_EN
					stream<rxReadReqUpdate>&	rx_readReqTable_upd_req,
					stream<rxReadReqRsp>&		rx_readReqTable_upd_rsp)
#else
					stream<rxReadReqUpdate>&	rx_readReqTable_upd_req)
#endif
{
#pragma HLS PIPELINE II=1
#pragma HLS INLINE off

	static readReqTableEntry  req_table[MAX_QPS];
#if defined( __VITIS_HLS__)
	#pragma HLS bind_storage variable=req_table type=RAM_2P impl=BRAM
#else
	#pragma HLS RESOURCE variable=req_table core=RAM_2P_BRAM
#endif

	txReadReqUpdate update;
	rxReadReqUpdate request;

	if (!tx_readReqTable_upd.empty())
	{
		tx_readReqTable_upd.read(update);
		req_table[update.qpn].max_fwd_readreq = update.max_fwd_readreq;
	}
	else if (!rx_readReqTable_upd_req.empty())
	{
		rx_readReqTable_upd_req.read(request);
		if (request.write)
		{
			req_table[request.qpn].oldest_outstanding_readreq = request.oldest_outstanding_readreq;
		}
#ifdef RETRANS_EN
		else
		{
			bool valid = (req_table[request.qpn].oldest_outstanding_readreq < req_table[request.qpn].max_fwd_readreq);
			rx_readReqTable_upd_rsp.write(rxReadReqRsp(req_table[request.qpn].oldest_outstanding_readreq, valid));
		}
#endif
	}
}
