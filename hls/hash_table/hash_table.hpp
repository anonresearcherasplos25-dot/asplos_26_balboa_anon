// Anonymous contribution
#pragma once
#include "../axi_utils.hpp"
#include <math.h>

const uint32_t MAX_NUMBER_OF_ENTRIES = TCP_STACK_MAX_SESSIONS;

constexpr unsigned long ConstLog2(unsigned long val) {
  return val == 1 ? 0 : 1 + ConstLog2(val >> 1);
}

const uint32_t MAX_KEY_SIZE = 64;
const uint32_t MAX_ADDRESS_BITS = 16;

const uint32_t NUM_TABLES = 9;
const uint32_t TABLE_ADDRESS_BITS = ConstLog2(MAX_NUMBER_OF_ENTRIES/(NUM_TABLES-1));
const uint32_t TABLE_SIZE = (1 << TABLE_ADDRESS_BITS);

const uint32_t KEY_SIZE = 64;
const uint32_t VALUE_SIZE = 16;
const uint32_t MAX_TRIALS = 12;

//The hash table can easily support NUM_TABLES-1 * TABLE_SIZE
//for NUM_TABLES = 9 -> this equals to a load factor of 0.88
enum lookupSource {RX, TX_APP};

template <int K>
struct htLookupReq
{
   ap_uint<K>  key;
   lookupSource  source;
   htLookupReq<K>() {}
   htLookupReq<K>(ap_uint<K> key, lookupSource source)
      :key(key), source(source) {}
};

template <int K, int V>
struct htLookupResp
{
   ap_uint<K>  key;
   ap_uint<V>  value;
   bool        hit;
   lookupSource  source;
};

typedef enum {KV_INSERT, KV_DELETE} kvOperation;

template <int K, int V>
struct htUpdateReq
{
   kvOperation op;
   ap_uint<K>  key;
   ap_uint<V>  value;
   lookupSource  source;
   htUpdateReq<K,V>() {}
   htUpdateReq<K,V>(kvOperation op, ap_uint<K> key, ap_uint<V> value, lookupSource source)
      :op(op), key(key), value(value), source(source) {}
};

template <int K, int V>
struct htUpdateResp
{
   kvOperation op;
   ap_uint<K>  key;
   ap_uint<V>  value;
   bool        success;
   lookupSource  source;
};

template <int K, int V>
struct htEntry
{
   ap_uint<K>  key;
   ap_uint<V>  value;
   bool        valid;
   htEntry<K,V>() {}
   htEntry<K,V>(ap_uint<K> key, ap_uint<V> value)
      :key(key), value(value), valid(true) {}
};


template <int K, int V>
void hash_table_core(hls::stream<htLookupReq<K> >&      s_axis_lup_req,
               hls::stream<htUpdateReq<K,V> >&     s_axis_upd_req,
               hls::stream<htLookupResp<K,V> >&    m_axis_lup_rsp,
               hls::stream<htUpdateResp<K,V> >&    m_axis_upd_rsp,
               ap_uint<16>&                        regInsertFailureCount);
