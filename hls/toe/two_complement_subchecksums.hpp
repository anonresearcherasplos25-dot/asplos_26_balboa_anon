// Anonymous contribution
#pragma once

#include "../axi_utils.hpp"
#include "../ipv4/ipv4.hpp"

template <int WIDTH, int DUMMY>
void two_complement_subchecksums(hls::stream<net_axis<WIDTH> >&		dataIn,
                                 hls::stream<net_axis<WIDTH> >&		dataOut,
                                 hls::stream<subSums<WIDTH/16> >&		txEng_subChecksumsFifoOut)
{
   #pragma HLS PIPELINE II=1
   #pragma HLS INLINE off

	static subSums<WIDTH/16> tcts_tcp_sums;

	if (!dataIn.empty())
	{
		net_axis<WIDTH> currWord = dataIn.read();
		dataOut.write(currWord);

		for (int i = 0; i < WIDTH/16; i++)
		{
         #pragma HLS UNROLL

			ap_uint<16> temp;
			if (currWord.keep(i*2+1, i*2) == 0x3)
			{
				temp(7, 0) = currWord.data(i*16+15, i*16+8);
				temp(15, 8) = currWord.data(i*16+7, i*16);
				tcts_tcp_sums.sum[i] += temp;
				tcts_tcp_sums.sum[i] = (tcts_tcp_sums.sum[i] + (tcts_tcp_sums.sum[i] >> 16)) & 0xFFFF;
			}
			else if (currWord.keep[i*2] == 0x1)
			{
				temp(7, 0) = 0;
				temp(15, 8) = currWord.data(i*16+7, i*16);
				tcts_tcp_sums.sum[i] += temp;
				tcts_tcp_sums.sum[i] = (tcts_tcp_sums.sum[i] + (tcts_tcp_sums.sum[i] >> 16)) & 0xFFFF;
			}
		}
		if(currWord.last == 1)
		{
			txEng_subChecksumsFifoOut.write(tcts_tcp_sums);
			for (int i = 0; i < WIDTH/16; i++)
			{
				#pragma HLS UNROLL
				tcts_tcp_sums.sum[i] = 0;
			}
		}
	}
}
