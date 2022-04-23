#include "alt_bn128_test.hpp"
#include <eosio/transaction.hpp>

using namespace eosio;

uint8_t from_hex( char c ) {
   if( c >= '0' && c <= '9' )
      return c - '0';
   if( c >= 'a' && c <= 'f' )
         return c - 'a' + 10;
   if( c >= 'A' && c <= 'F' )
         return c - 'A' + 10;
   return 0;
}

size_t from_hex( const std::string& hex_str, char* out_data, size_t out_data_len ) {
   auto i = hex_str.begin();
   uint8_t* out_pos = (uint8_t*)out_data;
   uint8_t* out_end = out_pos + out_data_len;
   while( i != hex_str.end() && out_end != out_pos ) {
      *out_pos = from_hex( *i ) << 4;   
      ++i;
      if( i != hex_str.end() )  {
         *out_pos |= from_hex( *i );
         ++i;
      }
      ++out_pos;
   }
   return out_pos - (uint8_t*)out_data;
}

std::string to_hex( const char* d, uint32_t s ) 
{
   std::string r;
   const char* to_hex="0123456789abcdef";
   uint8_t* c = (uint8_t*)d;
   for( uint32_t i = 0; i < s; ++i )
      (r += to_hex[(c[i]>>4)]) += to_hex[(c[i] &0x0f)];
   return r;
}

std::string to_hex( const std::vector<char>& data )
{
   if( data.size() )
      return to_hex( data.data(), data.size() );
   return "";
}

void alt_bn128_test::testadd( std::string  op1, std::string  op2, std::string  result ) {
   char dop1[64] = { 0 };
   char dop2[64] = { 0 };
   char ret[64] = { 0 };

   from_hex(op1, dop1, 64);
   from_hex(op2, dop2, 64);

   eosio::internal_use_do_not_use::alt_bn128_add( dop1, 64, dop2, 64, (char*)ret, 64);

   std::vector<char> retVector(ret, ret+64);
   std::string retHex = to_hex( retVector );

   check( retHex == result, "Result does not match");
}

[[eosio::action]]
void alt_bn128_test::testmul( std::string op1, std::string op2, std::string result) {
   char dop1[64] = { 0 };
   char dop2[32] = { 0 };
   char ret[64] = { 0 };

   from_hex(op1, dop1, 64);
   from_hex(op2, dop2, 32);

   eosio::internal_use_do_not_use::alt_bn128_mul( dop1, 64, dop2, 32, (char*)ret, 64);

   std::vector<char> retVector(ret, ret+64);
   std::string retHex = to_hex( retVector );

   check( retHex == result, "Result does not match");

}

[[eosio::action]]
void alt_bn128_test::testpair( std::string op1, bool result) {
   char *dop1 = new char[2048];
   char ret[64] = { 0 };

   auto op1_len = from_hex(op1, dop1, 2048);

   eosio::internal_use_do_not_use::alt_bn128_pair( dop1, op1_len, (char*)ret, 64);

   delete []dop1;

   check(  (ret[0]!=0) == result, "Result does not match");
}

[[eosio::action]]
void alt_bn128_test::testmodexp( std::string base, std::string exp, std::string modulo, std::string result) {

}

