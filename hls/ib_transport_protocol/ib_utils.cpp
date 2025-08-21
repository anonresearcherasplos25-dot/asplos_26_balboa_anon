// Anonymous contribution

#include "ib_transport_protocol.hpp"

bool checkIfResponse(ibOpCode code)
{
	return (code == RC_RDMA_READ_RESP_FIRST || code == RC_RDMA_READ_RESP_MIDDLE ||
			code == RC_RDMA_READ_RESP_LAST  || code == RC_RDMA_READ_RESP_ONLY ||
			code == RC_ACK);
}

bool checkIfWrite(ibOpCode code)
{
	return (code == RC_RDMA_WRITE_FIRST || code == RC_RDMA_WRITE_MIDDLE ||
			code == RC_RDMA_WRITE_LAST  || code == RC_RDMA_WRITE_ONLY);
}

bool checkIfAethHeader(ibOpCode code)
{
	return (code == RC_RDMA_READ_RESP_ONLY || code == RC_RDMA_READ_RESP_FIRST ||
			code == RC_RDMA_READ_RESP_LAST || code == RC_ACK);
}

bool checkIfRethHeader(ibOpCode code)
{
	return (code == RC_RDMA_WRITE_ONLY  || code == RC_RDMA_WRITE_FIRST ||
			code == RC_RDMA_READ_REQUEST);
}
