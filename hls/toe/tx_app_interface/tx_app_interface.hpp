// Anonymous contributor
#include "../toe.hpp"
#include "../toe_internals.hpp"
#include "../tx_app_if/tx_app_if.hpp"
#include "../tx_app_stream_if/tx_app_stream_if.hpp"

using namespace hls;

struct txAppTableEntry
{
	ap_uint<WINDOW_BITS>		ackd;
	ap_uint<WINDOW_BITS>		mempt;
#if (TCP_NODELAY)
	ap_uint<WINDOW_BITS> 	min_window;
#endif
	txAppTableEntry() {}
};

template <int WIDTH>
void tx_app_interface(	stream<appTxMeta>&			appTxDataReqMetadata,
					stream<net_axis<WIDTH> >&				appTxDataReq,
					stream<sessionState>&			stateTable2txApp_rsp,
					stream<txSarAckPush>&			txSar2txApp_ack_push,
					stream<mmStatus>&				txBufferWriteStatus,

					stream<ipTuple>&				appOpenConnReq,
					stream<ap_uint<16> >&			appCloseConnReq,
					stream<sessionLookupReply>&		sLookup2txApp_rsp,
					stream<ap_uint<16> >&			portTable2txApp_port_rsp,
					stream<sessionState>&			stateTable2txApp_upd_rsp,
					stream<openStatus>&				conEstablishedFifo,

					stream<appTxRsp>&			appTxDataRsp,
					stream<ap_uint<16> >&				txApp2stateTable_req,
					stream<mmCmd>&					txBufferWriteCmd,
					stream<net_axis<WIDTH> >&				txBufferWriteData,
#if (TCP_NODELAY)
					stream<net_axis<WIDTH> >&				txApp2txEng_data_stream,
#endif
					stream<txAppTxSarPush>&			txApp2txSar_push,

					stream<openStatus>&				appOpenConnRsp,
					stream<fourTuple>&				txApp2sLookup_req,
					stream<stateQuery>&				txApp2stateTable_upd_req,
					stream<event>&					txApp2eventEng_setEvent,
					stream<openStatus>&				rtTimer2txApp_notification,
					ap_uint<32>						myIpAddress);
