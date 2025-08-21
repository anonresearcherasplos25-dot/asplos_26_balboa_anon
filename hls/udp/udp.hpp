// Anonymous contribution
#ifndef UDP_HPP
#define UDP_HPP

#include "../axi_utils.hpp"
#include "../packet.hpp"
#include "../ipv4/ipv4.hpp"
//#include "../ipv6/ipv6.hpp"
#include "../fns_config.hpp"

#define IP_VERSION 4

#if IP_VERSION == 6
typedef ipv6Meta ipMeta;
#else
typedef ipv4Meta ipMeta;
#endif

const uint32_t UDP_HEADER_SIZE = 64;
const uint16_t UDP_PROTOCOL = 0x11;

struct ipUdpMeta
{
	ap_uint<128> their_address;
	ap_uint<16> their_port;
	ap_uint<16> my_port;
	ap_uint<16>	length;
	ipUdpMeta() {}
	ipUdpMeta(ap_uint<128> addr, ap_uint<16> tport, ap_uint<16> mport, ap_uint<16> len)
		:their_address(addr), their_port(tport), my_port(mport), length(len) {}
};

struct udpMeta
{
	ap_uint<16> their_port;
	ap_uint<16> my_port;
	ap_uint<16> length;
	bool 		valid;
	udpMeta()
		:valid(true) {}
	udpMeta(ap_uint<16> tport, ap_uint<16> mport, ap_uint<16> len)
		:their_port(tport), my_port(mport), length(len), valid(true) {}
	udpMeta(ap_uint<16> tport, ap_uint<16> mport, ap_uint<16> len, bool valid)
		:their_port(tport), my_port(mport), length(len), valid(valid) {}
};


/**
 * [15, 0] = srcPort;
 * [31, 16] = dstPort;
 * [47, 32] = length;
 * [63, 48] = checksum;
 */
template <int W>
class udpHeader : public packetHeader<W, UDP_HEADER_SIZE> {
	using packetHeader<W, UDP_HEADER_SIZE>::header;

public:
	udpHeader() {}

	void setSrcPort(const ap_uint<16> port)
	{
		header(15,0) = reverse(port);
	}
	ap_uint<16> getSrcPort()
	{
		return reverse((ap_uint<16>)header(15,0));
	}
	void setDstPort(const ap_uint<16> port)
	{
		header(31,16) = reverse(port);
	}
	ap_uint<16> getDstPort()
	{
		return reverse((ap_uint<16>)header(31,16));
	}
	void setLength(ap_uint<16> len)
	{
		header(47,32) = reverse(len);
	}
	ap_uint<16> getLength()
	{
		return reverse((ap_uint<16>)header(47,32));
	}
};

template <int WIDTH>

// Renamed from udp_core to just udp
void udp(		hls::stream<ipMeta>&		s_axis_rx_meta,
				hls::stream<net_axis<WIDTH> >&	s_axis_rx_data,
				hls::stream<ipUdpMeta>&	m_axis_rx_meta,
				hls::stream<net_axis<WIDTH> >&	m_axis_rx_data,
				hls::stream<ipUdpMeta>&	s_axis_tx_meta,
				hls::stream<net_axis<WIDTH> >&	s_axis_tx_data,
				hls::stream<ipMeta>&		m_axis_tx_meta,
				hls::stream<net_axis<WIDTH> >&	m_axis_tx_data,
				ap_uint<128>		reg_ip_address,
				ap_uint<16>			reg_listen_port);

#endif
