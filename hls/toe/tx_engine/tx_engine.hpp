// Anonymous contributor
#include "../toe.hpp"
#include "../toe_internals.hpp"

using namespace hls;

/** @ingroup tx_engine
 *
 */
struct tx_engine_meta //same as rxEngine
{
	ap_uint<32> seqNumb;
	ap_uint<32> ackNumb;
	ap_uint<16> window_size;
#if (WINDOW_SCALE)
	ap_uint<4>	win_shift;
#endif
	ap_uint<16> length;
	ap_uint<1>	ack;
	ap_uint<1>	rst;
	ap_uint<1>	syn;
	ap_uint<1>	fin;
	tx_engine_meta() {}
	tx_engine_meta(ap_uint<1> ack, ap_uint<1> rst, ap_uint<1> syn, ap_uint<1> fin)
			:seqNumb(0), ackNumb(0), window_size(0), length(0), ack(ack), rst(rst), syn(syn), fin(fin) {}
	tx_engine_meta(ap_uint<32> seqNumb, ap_uint<32> ackNumb, ap_uint<1> ack, ap_uint<1> rst, ap_uint<1> syn, ap_uint<1> fin)
			:seqNumb(seqNumb), ackNumb(ackNumb), window_size(0), length(0), ack(ack), rst(rst), syn(syn), fin(fin) {}
};

/** @ingroup tx_engine
 *
 */
struct twoTuple
{
	ap_uint<32> srcIp;
	ap_uint<32> dstIp;
	twoTuple() {}
	twoTuple(ap_uint<32> srcIp, ap_uint<32> dstIp)
				:srcIp(srcIp), dstIp(dstIp) {}
};

/** @defgroup tx_engine TX Engine
 *  @ingroup tcp_module
 *  @image html tx_engine.png
 *  Explain the TX Engine
 *  The @ref tx_engine contains a state machine with a state for each Event Type.
 *  It then loads and generates the necessary metadata to construct the packet. If the packet
 *  contains any payload the data is retrieved from the Memory and put into the packet. The
 *  complete packet is then streamed out of the @ref tx_engine.
 */
template <int WIDTH>
void tx_engine(	stream<extendedEvent>&			eventEng2txEng_event,
				stream<rxSarReply>&				rxSar2txEng_upd_rsp,
				stream<txTxSarReply>&			txSar2txEng_upd_rsp,
				stream<net_axis<WIDTH> >&			txBufferReadData,
#if (TCP_NODELAY)
				stream<net_axis<WIDTH> >&			txApp2txEng_data_stream,
#endif
				stream<fourTuple>&				sLookup2txEng_rev_rsp,
				stream<ap_uint<16> >&			txEng2rxSar_upd_req,
				stream<txTxSarQuery>&			txEng2txSar_upd_req,
				stream<txRetransmitTimerSet>&	txEng2timer_setRetransmitTimer,
				stream<ap_uint<16> >&			txEng2timer_setProbeTimer,
				stream<mmCmd>&					txBufferReadCmd,
				stream<ap_uint<16> >&			txEng2sLookup_rev_req,
				stream<net_axis<WIDTH> >&			ipTxData,
				stream<ap_uint<1> >&			readCountFifo);
