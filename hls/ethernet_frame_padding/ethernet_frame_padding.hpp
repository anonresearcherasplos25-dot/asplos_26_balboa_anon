// Anonymous contribution
#pragma once
#include "../fns_config.hpp"
#include "../axi_utils.hpp"

void ethernet_frame_padding(
  hls::stream<ap_axiu<DATA_WIDTH, 0, 0, 0>>& dataIn,
  hls::stream<ap_axiu<DATA_WIDTH, 0, 0, 0>>& dataOut);