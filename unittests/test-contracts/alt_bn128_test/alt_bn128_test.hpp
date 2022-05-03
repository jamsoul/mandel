#pragma once

#include <eosio/eosio.hpp>

using bytes = std::vector<char>;

namespace eosio {
   namespace internal_use_do_not_use {
    extern "C" {
      __attribute__((eosio_wasm_import)) 
      int32_t alt_bn128_add( const char* op1_data, uint32_t op1_length,
                           const char* op2_data, uint32_t op2_length, 
                           char* result , uint32_t result_length);

      __attribute__((eosio_wasm_import)) 
      int32_t alt_bn128_mul( const char* op1_data, uint32_t op1_length,
                           const char* op2_data, uint32_t op2_length, 
                           char* result , uint32_t result_length);

      __attribute__((eosio_wasm_import)) 
      int32_t alt_bn128_pair( const char* op1_data, uint32_t op1_length,
                           bool* result);

      __attribute__((eosio_wasm_import)) 
      int32_t mod_exp( uint32_t base_len,
                     uint32_t exp_len,
                     uint32_t mod_len, 
                     const char* base_data, uint32_t base_length,
                     const char* exp_data, uint32_t exp_length,
                     const char* mod_data, uint32_t mod_length,
                     char* result, uint32_t result_length);      

      __attribute__((eosio_wasm_import))
      int32_t blake2_f( uint32_t rounds, 
                        const char* state, uint32_t len_state,
                        const char* message, uint32_t len_message, 
                        const char* t0_offset, uint32_t len_t0_offset, 
                        const char* t1_offset, uint32_t len_t1_offset, 
                        bool final, 
                        char* result, uint32_t len_result);
      }
   }
}

class [[eosio::contract]] alt_bn128_test : public eosio::contract {
public:
   using eosio::contract::contract;

   [[eosio::action]]
   void testadd( bytes op1, bytes op2, bytes result);

   [[eosio::action]]
   void testmul( bytes op1, bytes op2, bytes result);

   [[eosio::action]]
   void testpair( bytes op1, bool result);

   [[eosio::action]]
   void testmodexp( bytes base, bytes exp, bytes modulo, bytes result);
         
   [[eosio::action]]
   void testblake2f( uint32_t rounds, bytes state, bytes message, bytes t0, bytes t1, bool final, bytes result);

};
