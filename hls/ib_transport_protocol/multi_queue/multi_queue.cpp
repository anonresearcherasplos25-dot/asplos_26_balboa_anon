// Anonymous contribution
#ifdef __SYNTHESIS__

#include "multi_queue.hpp"




void multi_queue_test_top(hls::stream<mqInsertReq<int> >		multiQueue_push,
						hls::stream<mqPopReq>	multiQueue_pop_req,
						hls::stream<int>		multiQueue_rsp)
{
#pragma HLS DATAFLOW

	multi_queue<int, 128, 2048>(multiQueue_push,
								multiQueue_pop_req,
								multiQueue_rsp);
}

#endif