// Anonymous contributor
#include "rx_sar_table.hpp"

using namespace hls;

/** @ingroup rx_sar_table
 * 	This data structure stores the RX(receiving) sliding window
 *  and handles concurrent access from the @ref rx_engine, @ref rx_app_if
 *  and @ref tx_engine
 *  @param[in]		rxEng2rxSar_upd_req
 *  @param[in]		rxApp2rxSar_upd_req
 *  @param[in]		txEng2rxSar_upd_req
 *  @param[out]		rxSar2rxEng_upd_rsp
 *  @param[out]		rxSar2rxApp_upd_rsp
 *  @param[out]		rxSar2txEng_upd_rsp
 */
void rx_sar_table(	stream<rxSarRecvd>&			rxEng2rxSar_upd_req,
					stream<rxSarAppd>&			rxApp2rxSar_upd_req,
					stream<ap_uint<16> >&		txEng2rxSar_req, //read only
					stream<rxSarEntry>&			rxSar2rxEng_upd_rsp,
					stream<rxSarAppd>&			rxSar2rxApp_upd_rsp,
					stream<rxSarReply>&			rxSar2txEng_rsp)
{

	static rxSarEntry rx_table[MAX_SESSIONS];

#pragma HLS PIPELINE II=1

#pragma HLS bind_storage variable=rx_table type=RAM_1P impl=BRAM
#pragma HLS DEPENDENCE variable=rx_table inter false

	// Read only access from the Tx Engine
	if(!txEng2rxSar_req.empty())
	{
		ap_uint<16> addr = txEng2rxSar_req.read();
		rxSarEntry entry = rx_table[addr];
		rxSarReply reply (entry);

		//Pre-calculated usedLength, windowSize to improve timing in metaLoader
#if (WINDOW_SCALE)
				ap_uint<WINDOW_BITS> actualWindowSize = (entry.appd - ((ap_uint<WINDOW_BITS>)entry.recvd)) - 1; // This works even for wrap around
				reply.windowSize = actualWindowSize >> entry.win_shift;
#else
				//This works even for wrap around
				reply.windowSize = (entry.appd - ((ap_uint<16>)entry.recvd)) - 1; // This works even for wrap around
#endif

		rxSar2txEng_rsp.write(reply);
	}
	// Read or Write access from the Rx App I/F to update the application pointer
	else if(!rxApp2rxSar_upd_req.empty())
	{
		rxSarAppd in_appd = rxApp2rxSar_upd_req.read();
		if(in_appd.write)
		{
			rx_table[in_appd.sessionID].appd = in_appd.appd;
		}
		else
		{
			rxSar2rxApp_upd_rsp.write(rxSarAppd(in_appd.sessionID, rx_table[in_appd.sessionID].appd));
		}
	}
	// Read or Write access from the Rx Engine
	else if(!rxEng2rxSar_upd_req.empty())
	{
		rxSarRecvd in_recvd = rxEng2rxSar_upd_req.read();
		if (in_recvd.write)
		{
			rx_table[in_recvd.sessionID].recvd = in_recvd.recvd;
			rx_table[in_recvd.sessionID].head = in_recvd.head;
			rx_table[in_recvd.sessionID].offset = in_recvd.offset;
			rx_table[in_recvd.sessionID].gap = in_recvd.gap;
			if (in_recvd.init)
			{
				rx_table[in_recvd.sessionID].appd = in_recvd.recvd;
#if (WINDOW_SCALE)
				rx_table[in_recvd.sessionID].win_shift = in_recvd.win_shift;
#endif
			}
		}
		else
		{
			rxSar2rxEng_upd_rsp.write(rx_table[in_recvd.sessionID]);
		}
	}
}
