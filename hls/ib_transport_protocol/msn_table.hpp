// Anonymous contribution
#pragma once

#include "../axi_utils.hpp"
#include <rocev2_config.hpp> //defines MAX_QPS

struct ifMsnReq
{
	ap_uint<16> qpn;
	ap_uint<32> r_key;
	ifMsnReq() {}
	ifMsnReq(ap_uint<16> qpn, ap_uint<32> r_key)
		:qpn(qpn), r_key(r_key) {}
};

struct rxMsnReq
{
	ap_uint<16> qpn;
	ap_uint<24> msn;
	ap_uint<64> vaddr;
	ap_uint<32> dma_length;
    ap_uint<1>  lst;
	bool		write;
	rxMsnReq()
		:write(false) {}
	rxMsnReq(ap_uint<16> qpn)
		:qpn(qpn), write(false) {}
	rxMsnReq(ap_uint<16> qpn, ap_uint<24> msn)
			:qpn(qpn), msn(msn), vaddr(0), dma_length(0), lst(0), write(true) {}
	rxMsnReq(ap_uint<16> qpn, ap_uint<24> msn, ap_uint<64> vaddr, ap_uint<32> len, ap_uint<1> lst)
		:qpn(qpn), msn(msn), vaddr(vaddr), dma_length(len), lst(lst), write(true) {}
};

struct txMsnRsp
{
	ap_uint<24>	msn;
	ap_uint<32> r_key;
	txMsnRsp() {}
	txMsnRsp(ap_uint<24> msn, ap_uint<32> r_key)
		:msn(msn), r_key(r_key) {}
};

/*
 * dma: page 167
 * msn: page 169
 */
struct dmaState
{
	ap_uint<24> msn;
	ap_uint<64> vaddr;
	ap_uint<32> dma_length;
	ap_uint<32> r_key;
    ap_uint<1>  lst;
};


template <int INSTID>
void msn_table(hls::stream<rxMsnReq>& rxExh2msnTable_upd_req,
					hls::stream<ap_uint<16> >& txExh2msnTable_req,
					hls::stream<ifMsnReq>& if2msnTable_init,
					hls::stream<dmaState>& msnTable2rxExh_rsp,
					hls::stream<txMsnRsp>& msnTable2txExh_rsp);

template <int INSTID = 0>
void msn_table(hls::stream<rxMsnReq>&		rxExh2msnTable_upd_req,
					hls::stream<ap_uint<16> >&	txExh2msnTable_req,
					hls::stream<ifMsnReq>&	if2msnTable_init,
					hls::stream<dmaState>&		msnTable2rxExh_rsp,
					hls::stream<txMsnRsp>&	msnTable2txExh_rsp)
{
#pragma HLS PIPELINE II=1
#pragma HLS INLINE off

	static dmaState  msn_table[MAX_QPS];
#if defined( __VITIS_HLS__)
	#pragma HLS bind_storage variable=msn_table type=RAM_2P impl=BRAM
#else
	#pragma HLS RESOURCE variable=msn_table core=RAM_2P_BRAM
#endif

	rxMsnReq rxRequest;
	ifMsnReq ifRequest;
	ap_uint<16> qpn;

	//TODO init channel

	if (!rxExh2msnTable_upd_req.empty())
	{
		rxExh2msnTable_upd_req.read(rxRequest);
		if (rxRequest.write)
		{
			msn_table[rxRequest.qpn].msn = rxRequest.msn;
			msn_table[rxRequest.qpn].vaddr = rxRequest.vaddr;
			msn_table[rxRequest.qpn].dma_length= rxRequest.dma_length;
            msn_table[rxRequest.qpn].lst = rxRequest.lst;
		}
		else
		{
			msnTable2rxExh_rsp.write(dmaState(msn_table[rxRequest.qpn]));
		}
	}
	else if (!txExh2msnTable_req.empty())
	{
		txExh2msnTable_req.read(qpn);
		msnTable2txExh_rsp.write(txMsnRsp(msn_table[qpn].msn, msn_table[qpn].r_key));
	}
	else if (!if2msnTable_init.empty()) //move up??
	{
		if2msnTable_init.read(ifRequest);
		std::cout << std::hex << "[MSN TABLE " << INSTID << "]: MSN init for qpn " << ifRequest.qpn << std::endl;
		msn_table[ifRequest.qpn].msn = 0;
		msn_table[ifRequest.qpn].vaddr = 0; //TODO requried?
		msn_table[ifRequest.qpn].dma_length = 0;  //TODO requried?
		msn_table[ifRequest.qpn].r_key = ifRequest.r_key;
		// msn_table[ifRequest.qpn].r_key = 5; 
        msn_table[ifRequest.qpn].lst = 0;
	}
}
