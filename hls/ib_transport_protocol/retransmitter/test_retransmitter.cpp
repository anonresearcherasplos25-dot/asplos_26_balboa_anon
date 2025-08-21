// Anonymous contribution
#include "retransmitter.hpp"
#include <iostream>

int main()
{

	static stream<retransRelease>	rx2retrans_release_upd("rx2retrans_release_upd");
	static stream<retransmission> rx2retrans_req("rx2retrans_req");
	static stream<retransmission> timer2retrans_req("timer2retrans_req");
	static stream<retransEntry>	tx2retrans_insertRequest("tx2retrans_insertRequest");
	static stream<event>			retrans2event("retrans2event");

	int count = 0;
	while (count < 1000)
	{
		if (count == 5)
		{
			tx2retrans_insertRequest.write(retransEntry(0x11, 0xabcdea, RC_RDMA_WRITE_ONLY, 0xfeea, 16));
			tx2retrans_insertRequest.write(retransEntry(0x11, 0xabcdeb, RC_RDMA_READ_REQUEST, 0xfeeb, 16));
			tx2retrans_insertRequest.write(retransEntry(0x11, 0xabcdec, RC_RDMA_WRITE_ONLY, 0xfeec, 16));
			tx2retrans_insertRequest.write(retransEntry(0x11, 0xabcded, RC_RDMA_READ_REQUEST, 0xfeed, 16));
			tx2retrans_insertRequest.write(retransEntry(0x11, 0xabcdee, RC_RDMA_WRITE_ONLY, 0xfeec, 16));
			tx2retrans_insertRequest.write(retransEntry(0x11, 0xabcdef, RC_RDMA_READ_REQUEST, 0xfeed, 16));
		}
		if (count == 20)
		{
			timer2retrans_req.write(retransmission(0x11));
		}

		if (count == 25)
		{
			rx2retrans_req.write(retransmission(0x11, 0xabcdec));
		}

		//release
		if (count == 30)
		{
			rx2retrans_release_upd(retransRelease(0x11, 0xabcdec));
		}

		//trigger again
		if (count == 20)
		{
			timer2retrans_req.write(retransmission(0x11));
		}

		if (count == 25)
		{
			rx2retrans_req.write(retransmission(0x11, 0xabcdec));
		}

		if (count == 0)
		void retransmitter(	rx2retrans_release_upd,
							rx2retrans_req,
							timer2retrans_req,
							tx2retrans_insertRequest,
							retrans2event);

		count++;
	}

	event ev;
	while(!retrans2event.empty())
	{
		retrans2event.read(ev);
		std::cout << "Retransmission of qp " << ev.qpn << ", addr: " << ev.addr << ", len: " << ev.length << std::endl;
	}

	return 0;
}
