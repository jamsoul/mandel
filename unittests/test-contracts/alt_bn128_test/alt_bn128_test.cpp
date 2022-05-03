#include "alt_bn128_test.hpp"
#include <eosio/transaction.hpp>

using namespace eosio;

void alt_bn128_test::testadd( bytes op1, bytes op2, bytes result ) {
   bytes ret(64);
   uint32_t errorCode = eosio::internal_use_do_not_use::alt_bn128_add( op1.data(), op1.size(), op2.data(), op2.size(), (char*)ret.data(), 64);
   check( ret == result, "Result does not match");
}

[[eosio::action]]
void alt_bn128_test::testmul( bytes  op1, bytes  op2, bytes  result) {
   bytes ret(64);
   uint32_t errorCode = eosio::internal_use_do_not_use::alt_bn128_mul( op1.data(), op1.size(), op2.data(), op2.size(), (char*)ret.data(), 64);
   check( ret == result, "Result does not match");
}

[[eosio::action]]
void alt_bn128_test::testpair( bytes  op1, bool result) {
   bool ret;
   uint32_t errorCode = eosio::internal_use_do_not_use::alt_bn128_pair( op1.data(), op1.size(), &ret);
   check( ret == result, "Result does not match");
}

[[eosio::action]]
void alt_bn128_test::testmodexp( bytes  base, bytes  exp, bytes  modulo, bytes  result) {

}

[[eosio::action]]
void alt_bn128_test::testblake2f( uint32_t rounds, bytes  state, bytes  message, bytes  t0, bytes  t1, bool final, bytes  result) {
   bytes ret(64);

   check(state.size() == 64, "arg( h ) size error");
   check(message.size() == 128, "arg( m ) size error");
   check(t0.size() == 8, "arg( t0 ) size error");
   check(t1.size() == 8, "arg( t1 ) size error");
   check(result.size() == 64, "arg( result ) size error");
   
   uint32_t errorCode = eosio::internal_use_do_not_use::blake2_f( rounds, 
                                             state.data(), state.size(),
                                             message.data(), message.size(),
                                             t0.data(), t0.size(),
                                             t1.data(), t1.size(), 
                                             final,
                                             (char*)ret.data(), 64);

   check( ret == result, "Result does not match");
}


