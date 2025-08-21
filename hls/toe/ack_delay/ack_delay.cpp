// Anonymous contributor

#include "ack_delay.hpp"

using namespace hls;

void ack_delay(	stream<extendedEvent>&	input,
				stream<extendedEvent>&	output,
				stream<ap_uint<1> >&	readCountFifo,
				stream<ap_uint<1> >&	writeCountFifo)
{
#pragma HLS PIPELINE II=1

	static ap_uint<12> ack_table[MAX_SESSIONS]; //TODO why is it 12
	#pragma HLS bind_storage variable=ack_table type=RAM_2P impl=BRAM
	#pragma HLS DEPENDENCE variable=ack_table inter false
	static ap_uint<16>	ad_pointer = 0;
	//static ap_uint<4>	ad_readCounter = 0;
	extendedEvent ev;

	if (!input.empty())
	{
		input.read(ev);
		readCountFifo.write(1);
		// Check if there is a delayed ACK
		if (ev.type == ACK && ack_table[ev.sessionID] == 0)
		{
			ack_table[ev.sessionID] = TIME_64us;
		}
		else
		{
			// Assumption no SYN/RST
			ack_table[ev.sessionID] = 0;
			output.write(ev);
			writeCountFifo.write(1);
		}
	}
	else
	{
		if (ack_table[ad_pointer] > 0 && !output.full())
		{
			if (ack_table[ad_pointer] == 1)
			{
				output.write(event(ACK, ad_pointer));
				writeCountFifo.write(1);
			}
			// Decrease value
			ack_table[ad_pointer] -= 1;
		}
		ad_pointer++;
		if (ad_pointer == MAX_SESSIONS)
		{
			ad_pointer = 0;
		}
	}
}
