// Anonymous contribution
#ifndef ETHERNET_HPP
#define ETHERNET_HPP

#include "../packet.hpp"

const int ETH_HEADER_SIZE = 112;

/**
 * [47:0] MAC destination
 * [95:48] MAC source
 * [111:96] Ethertype
 */ 
template <int W>
class ethHeader : public packetHeader<W, ETH_HEADER_SIZE> {
	using packetHeader<W, ETH_HEADER_SIZE>::header;

public:
    void setDstAddress(ap_uint<48> addr)
    {
        header(47,0) = addr;
    }
    void setSrcAddress(ap_uint<48> addr)
    {
        header(95,48) = addr;
    }
    void setEthertype(ap_uint<16> type)
    {
        header(111,96) = reverse(type);
    }
	ap_uint<16> getEthertype()
	{
		return reverse((ap_uint<16>)header(111,96));
	}
};

#endif
