// Anonymous contributor
#include "../axi_utils.hpp"
#include "../fns_config.hpp"

const uint16_t ARP = 0x0806;
const uint16_t IPv4 = 0x0800;
const uint16_t IPv6 = 0x86DD;

const uint8_t ICMP = 0x01;
const uint8_t ICMPv6 = 0x3a;
const uint8_t UDP = 0x11;
const uint8_t TCP = 0x06;

template <int WIDTH>
void ip_handler_core(hls::stream<net_axis<WIDTH> >&		s_axis_raw,
				hls::stream<net_axis<WIDTH> >&		m_axis_ARP,
				hls::stream<net_axis<WIDTH> >&		m_axis_ICMPv6,
				hls::stream<net_axis<WIDTH> >&		m_axis_IPv6UDP,
				hls::stream<net_axis<WIDTH> >&		m_axis_ICMP,
				hls::stream<net_axis<WIDTH> >&		m_axis_UDP,
				hls::stream<net_axis<WIDTH> >&		m_axis_TCP,
				hls::stream<net_axis<WIDTH> >&		m_axis_ROCE,
				ap_uint<32>				myIpAddress);
