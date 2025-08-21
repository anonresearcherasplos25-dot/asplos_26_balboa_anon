// Anonymous contribution
#pragma once

#include <iostream>
#include "../axi_utils.hpp"
#include "../packet.hpp"
#include "rocev2_config.hpp"

using namespace hls;

// Disable CRC on the RX path - no check of the CRC of incoming packets 
#define DISABLE_CRC_CHECK

// Disable CRC on the TX path - no calculation of the CRC for outgoing packets
#define DISABLE_CRC_CALCULATION

// Definition of the CRC-module as inserted in the datapath of the networking stack
template <int WIDTH, int INSTID>
void crc(	
	// RX - incoming packets. Received through s_axis, passed on through m_axis. 
    hls::stream<net_axis<WIDTH> >&  s_axis_rx_data,
    hls::stream<net_axis<WIDTH> >&  m_axis_rx_data,
    
    // TX - outgoing packets. Received through s_axis, passed on through m_axis. 
    hls::stream<net_axis<WIDTH> >&  s_axis_tx_data,
    hls::stream<net_axis<WIDTH> >&  m_axis_tx_data,
    
    // Debug
    ap_uint<32>& 	                regCrcDropPkgCount
);
