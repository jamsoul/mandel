#pragma once

#include <eosio/eosio.hpp>

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
                           char* result , uint32_t result_length);

      __attribute__((eosio_wasm_import)) 
      int32_t mod_exp( uint32_t base_len,
                     uint32_t exp_len,
                     uint32_t mod_len, 
                     const char* base_data, uint32_t base_length,
                     const char* exp_data, uint32_t exp_length,
                     const char* mod_data, uint32_t mod_length,
                     char* result, uint32_t result_length);      
      }
   }
}

class [[eosio::contract]] alt_bn128_test : public eosio::contract {
public:
   using eosio::contract::contract;

   [[eosio::action]]
   void fun1( eosio::name op1, eosio::name op2);

 /*  [[eosio::action]]
   void test_alt_bn128_mul( eosio::name op1, eosio::name op2);

   [[eosio::action]]
   void test_alt_bn128_pair( eosio::name op1);

   [[eosio::action]]
   void test_mod_exp( eosio::name base, eosio::name exp, eosio::name modulus );
   */

};
