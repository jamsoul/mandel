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
                        char* result, uint32_t result_length);      }
        }
}

namespace eosio {
   name alt_bn128_add() {
      return name( internal_use_do_not_use::alt_bn128_add() );
   }
   
   name alt_bn128_mul() {
      return name( internal_use_do_not_use::alt_bn128_mul() );
   }
   
   name alt_bn128_pair() {
      return name( internal_use_do_not_use::alt_bn128_pair() );
   }
   
   name mod_exp() {
      return name( internal_use_do_not_use::mod_exp() );
   }
   
   
   
}

class [[eosio::contract]] alt_bn128_test : public eosio::contract {
public:
   using eosio::contract::contract;

   [[eosio::action]]
   void assertsender( eosio::name expected_sender );
   using assertsender_action = eosio::action_wrapper<"assertsender"_n, &get_sender_test::assertsender>;

   [[eosio::action]]
   void sendinline( eosio::name to, eosio::name expected_sender );

   [[eosio::action]]
   void notify( eosio::name to, eosio::name expected_sender, bool send_inline );

   [[eosio::on_notify("*::notify")]]
   void on_notify( eosio::name to, eosio::name expected_sender, bool send_inline );

};
