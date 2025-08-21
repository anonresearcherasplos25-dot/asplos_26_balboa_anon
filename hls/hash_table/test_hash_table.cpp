// Anonymous contribution
#include "hash_table.hpp"
#include <map>
#include <random>

std::default_random_engine gen;
std::uniform_int_distribution<uint64_t> distr(0, std::numeric_limits<uint64_t>::max());

uint64_t getRandomKey()
{
   return distr(gen);
}

uint64_t getRandomTuple()
{
   return 0;
}


int main()
{
   hls::stream<htLookupReq<64> >       s_axis_lup_req;
   hls::stream<htUpdateReq<64,16> >    s_axis_upd_req;
   hls::stream<htLookupResp<64,16> >   m_axis_lup_rsp;
   hls::stream<htUpdateResp<64,16> >   m_axis_upd_rsp;
   ap_uint<16> regInsertFailureCount;

   std::map<uint64_t,uint16_t> expectedKV;
   uint32_t numElements = (NUM_TABLES-1) * TABLE_SIZE;
   std::cout << "Testing with " << numElements << ", total number of entries: " << NUM_TABLES * TABLE_SIZE << ", load factor: " << (double) (NUM_TABLES-1) / (double) NUM_TABLES << std::endl;

   //insert elements
   int i =0;
   while (i < numElements)
   {
      uint64_t newKey = getRandomKey();
      //uint64_t<96> newKey = getRandomTuple();
      uint16_t newValue = i;

      //Check if key unique
      if (expectedKV.find(newKey) == expectedKV.end())
      {
         s_axis_upd_req.write(htUpdateReq<64, 16>(KV_INSERT, newKey, newValue, 0));
         expectedKV[newKey] = newValue;
         i++;
      }
   }

   bool success = true;
   int count = 0;
   //Execute Inserts
   int successCounter = 0;
   while (count < numElements+100)
   {
      hash_table( s_axis_lup_req,
                  s_axis_upd_req,
                  m_axis_lup_rsp,
                  m_axis_upd_rsp,
                  regInsertFailureCount);
      if (!m_axis_upd_rsp.empty())
      {
         htUpdateResp<64,16> response = m_axis_upd_rsp.read();
         if (!response.success)
         {
            success = false;
            std::cerr << "[ERROR] insert failed" << std::endl;
         }
         else
         {
            successCounter++;
         }
         
      }
      count++;
   }
   if (successCounter != numElements)
   {
      success = false;
      std::cerr << "[ERROR] not all elements inserted successfully, expected: " << numElements << ", received: " << successCounter << std::endl;

   }

   //Issue lookups
   std::map<uint64_t, uint16_t>::const_iterator it;
   for(it = expectedKV.begin(); it != expectedKV.end(); ++it)
   {
      uint64_t key = it->first;
      s_axis_lup_req.write(htLookupReq<64>(key, 0));
   }

   //Execute lookups
   count = 0;
   int lookupCounter = 0;
   it = expectedKV.begin();
   while (count < numElements+100)
   {
      hash_table( s_axis_lup_req,
                  s_axis_upd_req,
                  m_axis_lup_rsp,
                  m_axis_upd_rsp,
                  regInsertFailureCount);
      if (!m_axis_lup_rsp.empty())
      {
         htLookupResp<64,16> response = m_axis_lup_rsp.read();
         if (!response.hit)
         {
            success = false;
            std::cerr << "[ERROR] lookup did not return hit" << std::endl;
         }
         else
         {
            if (response.key == it->first &&  response.value == it->second)
            {
               lookupCounter++;
            }
            else
            {
               success = false;
               std::cerr << "[ERROR] lookup returned wrong key or value" << std::endl;
            }
            
         }
         it++;
      }
      count++;
   }
   if (lookupCounter != numElements)
   {
      success = false;
      std::cerr << "[ERROR] not all lookups successfully, expected: " << numElements << ", received: " << lookupCounter << std::endl;

   }

   //Deleting half of the keys
   bool even = true;
   uint32_t expectedDeletes = 0;
   for(it = expectedKV.begin(); it != expectedKV.end(); ++it)
   {
      uint64_t key = it->first;
      if (even)
      {
         s_axis_upd_req.write(htUpdateReq<64, 16>(KV_DELETE, key, 0, 0));
         expectedDeletes++;
      }
      even = !even;
   }

   //Execute deletes
   int deleteCounter = 0;
   count = 0;
   while (count < expectedDeletes+100)
   {
      hash_table( s_axis_lup_req,
                  s_axis_upd_req,
                  m_axis_lup_rsp,
                  m_axis_upd_rsp,
                  regInsertFailureCount);
      if (!m_axis_upd_rsp.empty())
      {
         htUpdateResp<64,16> response = m_axis_upd_rsp.read();
         if (!response.success)
         {
            success = false;
            std::cerr << "[ERROR] delete failed" << std::endl;
         }
         else
         {
            deleteCounter++;
         }
         
      }
      count++;
   }
   if (deleteCounter != expectedDeletes)
   {
      success = false;
      std::cerr << "[ERROR] not all elements deletes successfully, expected: " << numElements << ", received: " << successCounter << std::endl;

   }

   //Run lookups again to make sure elements got deleted
   for(it = expectedKV.begin(); it != expectedKV.end(); ++it)
   {
      uint64_t key = it->first;
         s_axis_lup_req.write(htLookupReq<64>(key, 0));
   }

   //Execute lookups
   count = 0;
   lookupCounter = 0;
   it = expectedKV.begin();
   even = true;
   while (count < numElements+100)
   {
      hash_table( s_axis_lup_req,
                  s_axis_upd_req,
                  m_axis_lup_rsp,
                  m_axis_upd_rsp,
                  regInsertFailureCount);
      if (!m_axis_lup_rsp.empty())
      {
         htLookupResp<64,16> response = m_axis_lup_rsp.read();
         if (even)
         {
            if (response.hit)
            {
               success = false;
               std::cerr << "[ERROR] lookup returned deleted item" << std::endl;
            }
         }
         else //!even
         {
            if (!response.hit)
            {
               success = false;
               std::cerr << "[ERROR] lookup did not return hit" << std::endl;
            }
            else
            {
               if (response.key == it->first &&  response.value == it->second)
               {
                  lookupCounter++;
               }
               else
               {
                  success = false;
                  std::cerr << "[ERROR] lookup returned wrong key or value" << std::endl;
               }
            }            
         }
         it++;
         even = !even;
      }
      count++;
   }
   if (lookupCounter != (numElements-expectedDeletes))
   {
      success = false;
      std::cerr << "[ERROR] not all lookups successfully, expected: " << numElements << ", received: " << lookupCounter << std::endl;

   }

   std::cout << "regInsertFailureCount: " << regInsertFailureCount << std::endl;
   return (success) ? 0 : -1;
}