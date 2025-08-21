// Anonymous contributor
#include "../toe.hpp"
#include "../toe_internals.hpp"

struct optionalFieldsMeta
{
	ap_uint<4>	dataOffset;
	ap_uint<1>	syn;
	optionalFieldsMeta() {}
	optionalFieldsMeta(ap_uint<4> offset, ap_uint<1> syn)
		:dataOffset(offset), syn(syn) {}
};

struct pseudoMeta
{
	ap_uint<32> their_address;
	ap_uint<32> our_address;
	ap_uint<16> length;
	pseudoMeta() {}
	pseudoMeta(ap_uint<32> src_addr, ap_uint<32> dst_addr, ap_uint<16> len)
		:their_address(src_addr), our_address(dst_addr), length(len) {}
};

/** @ingroup rx_engine
 *  @TODO check if same as in Tx engine
 */
struct rxEngineMetaData
{
	//ap_uint<16> sessionID;
	ap_uint<32> seqNumb;
	ap_uint<32> ackNumb;
	ap_uint<16> winSize;
//#if (WINDOW_SCALE)
	ap_uint<4>	winScale;
//#endif
	ap_uint<16> length;
	ap_uint<1>	ack;
	ap_uint<1>	rst;
	ap_uint<1>	syn;
	ap_uint<1>	fin;
	ap_uint<4>	dataOffset;
	//ap_uint<16> dstPort;
};

/** @ingroup rx_engine
 *
 */
struct rxFsmMetaData
{
	ap_uint<16>			sessionID;
	ap_uint<32>			srcIpAddress;
	ap_uint<16>			dstIpPort;
	rxEngineMetaData	meta; //check if all needed
	ap_uint<16> 		srcIpPort;
	rxFsmMetaData() {}
	rxFsmMetaData(ap_uint<16> id, ap_uint<32> ipAddr, ap_uint<16> ipPort, rxEngineMetaData meta, ap_uint<16> srcIpPort)
				:sessionID(id), srcIpAddress(ipAddr), dstIpPort(ipPort), meta(meta), srcIpPort(srcIpPort) {}
};

/** @defgroup rx_engine RX Engine
 *  @ingroup tcp_module
 *  RX Engine
 */
template <int WIDTH>
void rx_engine(	stream<net_axis<WIDTH> >&					ipRxData,
				stream<sessionLookupReply>&			sLookup2rxEng_rsp,
				stream<sessionState>&				stateTable2rxEng_upd_rsp,
				stream<bool>&						portTable2rxEng_rsp,
				stream<rxSarEntry>&					rxSar2rxEng_upd_rsp,
				stream<rxTxSarReply>&				txSar2rxEng_upd_rsp,
#if !(RX_DDR_BYPASS)
				stream<mmStatus>&					rxBufferWriteStatus,
#endif
				stream<net_axis<WIDTH> >&					rxBufferWriteData,
				stream<sessionLookupQuery>&			rxEng2sLookup_req,
				stream<stateQuery>&					rxEng2stateTable_upd_req,
				stream<ap_uint<16> >&				rxEng2portTable_req,
				stream<rxSarRecvd>&					rxEng2rxSar_upd_req,
				stream<rxTxSarQuery>&				rxEng2txSar_upd_req,
				stream<rxRetransmitTimerUpdate>&	rxEng2timer_clearRetransmitTimer,
				stream<ap_uint<16> >&				rxEng2timer_clearProbeTimer,
				stream<ap_uint<16> >&				rxEng2timer_setCloseTimer,
				stream<openStatus>&					openConStatusOut, //TODO remove
				stream<extendedEvent>&				rxEng2eventEng_setEvent,
#if !(RX_DDR_BYPASS)
				stream<mmCmd>&						rxBufferWriteCmd,
				stream<appNotification>&			rxEng2rxApp_notification);
#else
				stream<appNotification>&			rxEng2rxApp_notification,
				ap_uint<16>					rxbuffer_data_count,
				ap_uint<16>					rxbuffer_max_data_count);
#endif
