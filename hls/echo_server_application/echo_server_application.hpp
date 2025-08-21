// Anonymous contributor

#include "../axi_utils.hpp"
#include "../toe/toe.hpp"

/** @defgroup echo_server_application Echo Server Application
 *
 */
void echo_server_application(hls::stream<ap_uint<16> >& listenPort, hls::stream<bool>& listenPortStatus,
								hls::stream<appNotification>& notifications, hls::stream<appReadRequest>& readRequest,
								hls::stream<ap_uint<16> >& rxMetaData, hls::stream<net_axis<64> >& rxData,
								hls::stream<ipTuple>& openConnection, hls::stream<openStatus>& openConStatus,
								hls::stream<ap_uint<16> >& closeConnection,
								hls::stream<appTxMeta>& txMetaData, hls::stream<net_axis<64> >& txData,
								hls::stream<appTxRsp>& txStatus);
