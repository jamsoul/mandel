#include "alt_bn128_test.hpp"
#include <eosio/transaction.hpp>

using namespace eosio;


void alt_bn128_test::fun1( eosio::name _op1, eosio::name _op2) {
   unsigned char op1[64] = { 34,36,128,201,249,84,9,191,164,172,106,232,144,185,193,80,188,136,84,43,135,179,82,233,41,80,195,64,69,139,12,9,
                    41,118,239,214,152,207,35,180,20,234,98,43,63,114,13,217,8,13,103,144,66,72,47,243,102,140,178,227,44,173,138,226};
   unsigned char op2[64] = { 27,210,11,236,163,216,210,142,83,109,43,91,211,191,54,215,106,246,138,245,230,201,108,166,229,81,155,169,255,143,83,50,
                    42,83,237,246,180,139,207,92,177,192,180,173,29,54,223,206,6,167,157,205,101,38,241,195,134,161,77,140,228,100,152,68};
   unsigned char ret[64] = { 0 };


   print_f("OP1:"); eosio::printhex(op1, 64); print_f("\n");
   print_f("OP2:"); eosio::printhex(op2, 64); print_f("\n");
   
   eosio::internal_use_do_not_use::alt_bn128_add( (char*)op1, 64, (char*)op2, 64, (char*)ret, 64);

   print_f("OUT:"); eosio::printhex(ret, 64); print_f("\n");

}
/*
void get_sender_test::assertsender( name expected_sender ) {
   check( get_sender() == expected_sender, "sender did not match" );
}

void get_sender_test::sendinline( name to, name expected_sender ) {
   assertsender_action a( to,  std::vector<eosio::permission_level>{} );
   a.send( expected_sender );
}

void get_sender_test::notify( name to, name expected_sender, bool send_inline ) {
   require_recipient( to );
}

void get_sender_test::on_notify( name to, name expected_sender, bool send_inline ) {
   if( send_inline ) {
      assertsender_action a( get_first_receiver(),  std::vector<eosio::permission_level>{} );
      a.send( expected_sender );
   } else {
      check( get_sender() == expected_sender, "sender did not match" );
   }
}
*/