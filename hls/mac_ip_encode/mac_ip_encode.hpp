// Anonymous contributor

#include "../axi_utils.hpp"
#include "../fns_config.hpp"

struct arpTableReply
{
	ap_uint<48>	macAddress;
	bool		hit;
	arpTableReply() {}
	arpTableReply(ap_uint<48> macAdd, bool hit)
			:macAddress(macAdd), hit(hit) {}
};

template <int WIDTH>
void mac_ip_encode_core( hls::stream<net_axis<WIDTH> >&			dataIn,
					hls::stream<arpTableReply>&		arpTableIn,
					hls::stream<net_axis<WIDTH> >&			dataOut,
					hls::stream<ap_uint<32> >&		arpTableOut,
					ap_uint<48>					myMacAddress,
					ap_uint<32>					regSubNetMask,
					ap_uint<32>					regDefaultGateway);
