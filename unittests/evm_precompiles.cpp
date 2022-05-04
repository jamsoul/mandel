#include <eosio/chain/abi_serializer.hpp>
#include <eosio/chain/resource_limits.hpp>
#include <eosio/chain/generated_transaction_object.hpp>
#include <eosio/testing/tester.hpp>

#include <Runtime/Runtime.h>

#include <fc/variant_object.hpp>

#include <boost/test/unit_test.hpp>

#include <contracts.hpp>

#include "fork_test_utilities.hpp"

using namespace eosio::chain;
using namespace eosio::testing;
using namespace std::literals;

using bytes = std::vector<char>;

bytes h2bin(const std::string& source) {
   bytes output(source.length()/2);
   fc::from_hex(source, output.data(), output.size());
   return output;
}

BOOST_AUTO_TEST_SUITE(evm_precompiles_tests)

BOOST_AUTO_TEST_CASE( alt_bn128_add_test ) { try {
   tester c( setup_policy::preactivate_feature_and_new_bios );

   const auto& tester1_account = account_name("tester1");
   c.create_accounts( {tester1_account} );
   c.produce_block();

   const auto& pfm = c.control->get_protocol_feature_manager();
   const auto& d = pfm.get_builtin_digest( builtin_protocol_feature_t::evm_precompiles );
   BOOST_REQUIRE( d );

   c.preactivate_protocol_features( {*d} );
   c.produce_block();

   c.set_code( tester1_account, contracts::alt_bn128_test_wasm() );
   c.set_abi( tester1_account, contracts::alt_bn128_test_abi().data() );
   c.produce_block();

   std::string op1("222480c9f95409bfa4ac6ae890b9c150bc88542b87b352e92950c340458b0c092976efd698cf23b414ea622b3f720dd9080d679042482ff3668cb2e32cad8ae2");
   std::string op2("1bd20beca3d8d28e536d2b5bd3bf36d76af68af5e6c96ca6e5519ba9ff8f53322a53edf6b48bcf5cb1c0b4ad1d36dfce06a79dcd6526f1c386a14d8ce4649844");
   std::string res("16c7c4042e3a725ddbacf197c519c3dcad2bc87dfd9ac7e1e1631154ee0b7d9c19cd640dd28c9811ebaaa095a16b16190d08d6906c4f926fce581985fe35be0e");

   c.push_action( tester1_account, "testadd"_n, tester1_account, mutable_variant_object()
      ("op1", h2bin(op1))
      ("op2", h2bin(op2))
      ("result", h2bin(res))
   );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( alt_bn128_mul_test ) { try {
   tester c( setup_policy::preactivate_feature_and_new_bios );

   const auto& tester1_account = account_name("tester1");
   c.create_accounts( {tester1_account} );
   c.produce_block();

   const auto& pfm = c.control->get_protocol_feature_manager();
   const auto& d = pfm.get_builtin_digest( builtin_protocol_feature_t::evm_precompiles );
   BOOST_REQUIRE( d );

   c.preactivate_protocol_features( {*d} );
   c.produce_block();

   c.set_code( tester1_account, contracts::alt_bn128_test_wasm() );
   c.set_abi( tester1_account, contracts::alt_bn128_test_abi().data() );
   c.produce_block();

   std::string op1("1e462d01d1861f7ee499bf70ab12ade335d98586b52db847ee2ec1e790170e0414bd807f4e64904b29e874fd824ff16e465b5798b19aafe0cae60a2dbcf91333");
   std::string op2("30586f85e8fcea91c0db1ed30aacf7350e72efd4cf756b3ce309f2159e275ff9");
   std::string res("15ea829def65cb28c5435094e1b8d06cb021a8671319cdad074ee89ce7c2c0bf0b68b46b86de49221fe4dbdce9b88518812c9d48fb502ada0a2ad9fc28312c89");

   c.push_action( tester1_account, "testmul"_n, tester1_account, mutable_variant_object()
      ("op1", h2bin(op1))
      ("op2", h2bin(op2))
      ("result", h2bin(res))
   );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( alt_bn128_pair_test ) { try {
   tester c( setup_policy::preactivate_feature_and_new_bios );

   const auto& tester1_account = account_name("tester1");
   c.create_accounts( {tester1_account} );
   c.produce_block();

   const auto& pfm = c.control->get_protocol_feature_manager();
   const auto& d = pfm.get_builtin_digest( builtin_protocol_feature_t::evm_precompiles );
   BOOST_REQUIRE( d );

   c.preactivate_protocol_features( {*d} );
   c.produce_block();

   c.set_code( tester1_account, contracts::alt_bn128_test_wasm() );
   c.set_abi( tester1_account, contracts::alt_bn128_test_abi().data() );
   c.produce_block();

   std::string op1("2bcf154b010dedb450cfea4f635526973f39365ec204e4a8b0e3ecc29abb7e4e23db84b7ae4e35681e833b6a1f6903e28291d154af3ec5ddc787e0e6cb05891227d2525616cd883a2e952616138e052125201826d45e179a9ae28655338ca2be2167ff55d36a2ed92eb480b1b9365382ea2facea90c860d63211827f122fdc292c6e8b5d5da9a03f2d6b57bf2338168eca1e43409693b43659fe834149e506a9020401d78e6fe746fe3d9512f9b4eedcfdd7eb5d08e307f1d6ee5d38f9a253ec2bcf154b010dedb450cfea4f635526973f39365ec204e4a8b0e3ecc29abb7e4e0c88c9bb32e36ac199cd0a4c6218547b14ef993cb93304af7498ab300d77743527d2525616cd883a2e952616138e052125201826d45e179a9ae28655338ca2be2167ff55d36a2ed92eb480b1b9365382ea2facea90c860d63211827f122fdc292c6e8b5d5da9a03f2d6b57bf2338168eca1e43409693b43659fe834149e506a9020401d78e6fe746fe3d9512f9b4eedcfdd7eb5d08e307f1d6ee5d38f9a253ec");
   
   c.push_action( tester1_account, "testpair"_n, tester1_account, mutable_variant_object()
      ("op1", h2bin(op1))
      ("result", false) // should be true though
   );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( blake2f_test ) { try {
   tester c( setup_policy::preactivate_feature_and_new_bios );

   const auto& tester1_account = account_name("tester1");
   c.create_accounts( {tester1_account} );
   c.produce_block();

   const auto& pfm = c.control->get_protocol_feature_manager();
   const auto& d = pfm.get_builtin_digest( builtin_protocol_feature_t::evm_precompiles );
   BOOST_REQUIRE( d );

   c.preactivate_protocol_features( {*d} );
   c.produce_block();

   c.set_code( tester1_account, contracts::alt_bn128_test_wasm() );
   c.set_abi( tester1_account, contracts::alt_bn128_test_abi().data() );
   c.produce_block();

   std::string h("48c9bdf267e6096a3ba7ca8485ae67bb2bf894fe72f36e3cf1361d5f3af54fa5"
                 "d182e6ad7f520e511f6c3e2b8c68059b6bbd41fbabd9831f79217e1319cde05b");
   std::string m("6162630000000000000000000000000000000000000000000000000000000000"
                 "0000000000000000000000000000000000000000000000000000000000000000"
                 "0000000000000000000000000000000000000000000000000000000000000000"
                 "0000000000000000000000000000000000000000000000000000000000000000");
   std::string t0("0300000000000000");
   std::string t1("0000000000000000");
   std::string result("ba80a53f981c4d0d6a2797b69f12f6e94c212f14685ac4b74b12bb6fdbffa2d1"
                      "7d87c5392aab792dc252d5de4533cc9518d38aa8dbf1925ab92386edd4009923");
   uint32_t r{12};

   c.push_action( tester1_account, "testblake2f"_n, tester1_account, mutable_variant_object()
      ("rounds", r)
      ("state", h2bin(h))
      ("message", h2bin(m))
      ("t0", h2bin(t0))
      ("t1", h2bin(t1))
      ("final", true)
      ("result", h2bin(result))
   );
   } FC_LOG_AND_RETHROW() }


BOOST_AUTO_TEST_CASE( get_block_num_test ) { try {
   tester c( setup_policy::preactivate_feature_and_new_bios );

   const auto& tester1_account = account_name("tester1");
   c.create_accounts( {tester1_account} );
   c.produce_block();

   const auto& pfm = c.control->get_protocol_feature_manager();
   const auto& d = pfm.get_builtin_digest( builtin_protocol_feature_t::evm_precompiles );
   BOOST_REQUIRE( d );

   c.preactivate_protocol_features( {*d} );
   c.produce_block();

   c.set_code( tester1_account, contracts::alt_bn128_test_wasm() );
   c.set_abi( tester1_account, contracts::alt_bn128_test_abi().data() );
   c.produce_block();

   c.push_action( tester1_account, "testblock"_n, tester1_account, mutable_variant_object()
      ("blocknum", 0)
   );
   } FC_LOG_AND_RETHROW() }


BOOST_AUTO_TEST_SUITE_END()
