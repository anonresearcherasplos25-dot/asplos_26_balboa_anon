// Anonymous contributor
#include "echo_server_application.hpp"

int main()
{

	hls::stream<ap_uint<16> > listenPort("listenPort");
	hls::stream<bool> listenPortStatus("listenPortStatus");
	hls::stream<appNotification> notifications;
	hls::stream<appReadRequest> readRequest;
	hls::stream<ap_uint<16> > rxMetaData;
	hls::stream<net_axis<64> > rxData;
	hls::stream<ipTuple> openConnection;
	hls::stream<openStatus> openConStatus;
	hls::stream<ap_uint<16> > closeConnection;
	hls::stream<appTxMeta> txMetaData;
	hls::stream<net_axis<64> > txData;
	hls::stream<appTxRsp>	txStatus;

	int count = 0;
	int portOpened = -1;
	while (count < 50)
	{
		echo_server_application(	listenPort, listenPortStatus,
									notifications, readRequest,
									rxMetaData, rxData,
									openConnection, openConStatus,
									closeConnection,
									txMetaData, txData,
									txStatus);
		if (!listenPort.empty())
		{
			ap_uint<16> port = listenPort.read();
			std::cout << std::dec << "opening port: " << port << std::endl;
			listenPortStatus.write(true);
			portOpened = 0;
		}
		count++;
	}
	return portOpened;
}
