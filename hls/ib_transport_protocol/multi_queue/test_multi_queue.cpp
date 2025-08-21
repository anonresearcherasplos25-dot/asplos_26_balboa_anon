// Anonymous contribution
#include "multi_queue.hpp"
#include <vector>
#include <queue>
//#include <random>

struct TestValue
{
	ap_uint<32> abc;
	ap_uint<32> cba;
};

int main()
{
	hls::stream<mqInsertReq<TestValue> >	multiQueue_push("multiQueue_push");
	hls::stream<mqPopReq>					multiQueue_pop_req("multiQueue_pop_req");
	hls::stream<TestValue>					multiQueue_rsp("multiQueue_rsp");

	const int NUM_QUEUES = 128;
	const int QUEUE_SIZE = 2048;

	ap_uint<16> key = 13;

	int count = 0;
	while (count < 100000)
	{
		if (count < 10)
		{
			TestValue t;
			t.abc = count+16;
			t.cba = count+16;
			mqInsertReq<TestValue> insert = mqInsertReq<TestValue>(key, t);
			multiQueue_push.write(insert);
		}
		if (count >= 10 && count < 30)
		{
			if (count % 2 == 0) {
				multiQueue_pop_req.write(mqPopReq(POP,key));
			} else {
				multiQueue_pop_req.write(mqPopReq(FRONT, key));
			}
		}

		multi_queue<TestValue, NUM_QUEUES, QUEUE_SIZE>(	multiQueue_push,
														multiQueue_pop_req,
														multiQueue_rsp);
		count++;
	}

	TestValue ret;
	while (!multiQueue_rsp.empty())
	{
		multiQueue_rsp.read(ret);
		std::cout << "abc: " << ret.abc << ", cba: " << ret.cba << std::endl;
	}

	return 0;
}
