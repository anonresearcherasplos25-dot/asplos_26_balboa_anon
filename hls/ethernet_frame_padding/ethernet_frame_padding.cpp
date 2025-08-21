// Anonymous contribution
#include "ethernet_frame_padding.hpp"

void ethernet_frame_padding(
  hls::stream<ap_axiu<DATA_WIDTH, 0, 0, 0>>& dataIn,
  hls::stream<ap_axiu<DATA_WIDTH, 0, 0, 0>>& dataOut)
{
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS INTERFACE axis register port=dataIn name=s_axis
#pragma HLS INTERFACE axis register port=dataOut name=m_axis

  static ap_uint<1> state = 0;

  switch(state)
  {
    case 0:
      if (!dataIn.empty())
      {
        ap_axiu<DATA_WIDTH, 0, 0, 0> currWord = dataIn.read();
        if (currWord.last)
        {
          for (int i = 0; i < DATA_WIDTH/8; ++i)
          {
            #pragma HLS UNROLL
            if (currWord.keep[i] == 0)
            {
              currWord.keep[i] = 1;
              currWord.data(i*8+7, i*8) = 0;
            }
          }
        }
        else
        {
          state = 1;
        }
        dataOut.write(currWord);
      }
      break;
    case 1:
      if (!dataIn.empty())
      {
        ap_axiu<DATA_WIDTH, 0, 0, 0> currWord = dataIn.read();
        dataOut.write(currWord);

        if (currWord.last)
        {
          state = 0;
        }
      }
      break;
  } //switch
}