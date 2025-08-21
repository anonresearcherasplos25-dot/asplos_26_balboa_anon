// Anonymous contributor
#include "../toe.hpp"
#include "../toe_internals.hpp"

using namespace hls;

static const ap_uint<2> NO_ERROR			= 0;
static const ap_uint<2> ERROR_NOCONNCECTION		= 1;
static const ap_uint<2> ERROR_NOSPACE			= 2;

/** @ingroup tx_app_stream_if
 *
 */
struct eventMeta
{
	ap_uint<16> sessionID;
	ap_uint<16> address;
	ap_uint<16> length;
	eventMeta() {}
	eventMeta(ap_uint<16> id, ap_uint<16> addr, ap_uint<16> len)
				:sessionID(id), address(addr), length(len) {}
};

/** @defgroup tx_app_stream_if TX Application Stream Interface
 *  @ingroup app_if
 */
template <int WIDTH>
void tx_app_stream_if(	stream<appTxMeta>&				appTxDataReqMetaData,
						stream<net_axis<WIDTH> >&				appTxDataReq,
						stream<sessionState>&			stateTable2txApp_rsp,
						stream<txAppTxSarReply>&		txSar2txApp_upd_rsp, //TODO rename
						stream<appTxRsp>&			appTxDataRsp,
						stream<ap_uint<16> >&			txApp2stateTable_req,
						stream<txAppTxSarQuery>&		txApp2txSar_upd_req, //TODO rename
						stream<mmCmd>&					txBufferWriteCmd,
						stream<net_axis<WIDTH> >&				txBufferWriteData,
#if (TCP_NODELAY)
						stream<net_axis<WIDTH> >&				txApp2txEng_data_stream,
#endif
						stream<event>&					txAppStream2eventEng_setEvent);
