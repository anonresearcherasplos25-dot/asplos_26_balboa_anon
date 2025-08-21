// Anonymous contribution
#pragma once

#include "axi_utils.hpp"

typedef enum {
	ROUTE_DMA = 0x0,
	ROUTE_CUSTOM = 0x1,
} axiRoute;


struct dmCmd
{
	ap_uint<23>	bbt;
	ap_uint<1>	type;
	ap_uint<6>	dsa;
	ap_uint<1>	eof;
	ap_uint<1>	drr;
	ap_uint<32>	saddr;
	ap_uint<4>	tag;
	ap_uint<4>	rsvd;
	dmCmd() {}
	//dmCmd(const dmCmd& cmd)
		//:bbt(cmd.bbt), type(cmd.type), dsa(cmd.dsa), eof(cmd.eof), drr(cmd.drr), saddr(cmd.saddr), tag(cmd.tag), rsvd(cmd.rsvd) {}
	dmCmd(ap_uint<32> addr, ap_uint<16> len) //TODO length depends on datamover config
		:bbt(len), type(1), dsa(0), eof(1), drr(0), saddr(addr), tag(0), rsvd(0) {}
	dmCmd(ap_uint<32> addr, ap_uint<16> len, ap_uint<4> tag)
		:bbt(len), type(1), dsa(0), eof(1), drr(0), saddr(addr), tag(tag), rsvd(0) {}
};

struct routedDmCmd
{
	ap_uint<23>	bbt;
	ap_uint<32>	saddr;
	ap_uint<4>	tag;
	ap_uint<1>	dest;
	routedDmCmd() {}
	routedDmCmd(ap_uint<32> addr, ap_uint<16> len, ap_uint<1> dest=0)
		:bbt(len), saddr(addr), tag(0), dest(dest) {}
	routedDmCmd(ap_uint<32> addr, ap_uint<16> len, ap_uint<4> tag, ap_uint<1> dest=0)
		:bbt(len), saddr(addr), tag(tag), dest(dest) {}
};

struct dmStatus
{
	ap_uint<4>	tag;
	ap_uint<1>	interr;
	ap_uint<1>	decerr;
	ap_uint<1>	slverr;
	ap_uint<1>	okay;
	dmStatus() {}
	dmStatus(bool okay)
		:okay(okay) {}
	dmStatus(bool okay, ap_uint<4> tag)
		:okay(okay), tag(tag) {}
};

struct memCmd
{
	ap_uint<64> addr;
	ap_uint<32> len;
	ap_uint<4>  local_reg;
	ap_uint<1>  ctl;
	ap_uint<1>  host; 
	memCmd() {}
	memCmd(ap_uint<64> addr, ap_uint<32> len, ap_uint<4> local_reg, ap_uint<1> ctl, ap_uint<1> host)
		:addr(addr), len(len), local_reg(local_reg), ctl(ctl), host(host) {}
};

struct routedMemCmd
{
	memCmd      data;
	routedMemCmd() {}
	routedMemCmd(ap_uint<64> addr, ap_uint<32> len, ap_uint<4> local_reg, ap_uint<1> ctl, ap_uint<1> host)
		:data(addr, len, local_reg, ctl, host) {}
};
