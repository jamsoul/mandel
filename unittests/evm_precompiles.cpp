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

BOOST_AUTO_TEST_SUITE(evm_precompiles_tests)

BOOST_AUTO_TEST_CASE( alt_bn128_add_test ) { try {
   tester c( setup_policy::preactivate_feature_and_new_bios );

   const auto& tester1_account = account_name("tester1");
   c.create_accounts( {tester1_account} );
   c.produce_block();

   BOOST_CHECK_EXCEPTION(  c.set_code( tester1_account, contracts::alt_bn128_test_wasm() ),
                           wasm_exception,
                           fc_exception_message_is( "env.alt_bn128_add unresolveable" ) );

   const auto& pfm = c.control->get_protocol_feature_manager();
   const auto& d = pfm.get_builtin_digest( builtin_protocol_feature_t::evm_precompiles );
   BOOST_REQUIRE( d );

   c.preactivate_protocol_features( {*d} );
   c.produce_block();

   c.set_code( tester1_account, contracts::alt_bn128_test_wasm() );
   c.set_abi( tester1_account, contracts::alt_bn128_test_abi().data() );
   c.produce_block();

   c.push_action( tester1_account, "fun1"_n, tester1_account, mutable_variant_object()
      ("op1", tester1_account.to_string())
      ("op2", tester1_account.to_string())
   );

/*
   BOOST_CHECK_EXCEPTION(  c.push_action( tester1_account, "sendinline"_n, tester1_account, mutable_variant_object()
                                             ("to", tester2_account.to_string())
                                             ("expected_sender", account_name{}) ),
                           eosio_assert_message_exception,
                           eosio_assert_message_is( "sender did not match" ) );

   c.push_action( tester1_account, "sendinline"_n, tester1_account, mutable_variant_object()
      ("to", tester2_account.to_string())
      ("expected_sender", tester1_account.to_string())
   );

   c.push_action( tester1_account, "notify"_n, tester1_account, mutable_variant_object()
      ("to", tester2_account.to_string())
      ("expected_sender", tester1_account.to_string())
      ("send_inline", false)
   );

   c.push_action( tester1_account, "notify"_n, tester1_account, mutable_variant_object()
      ("to", tester2_account.to_string())
      ("expected_sender", tester2_account.to_string())
      ("send_inline", true)
   );

   c.push_action( tester1_account, "assertsender"_n, tester1_account, mutable_variant_object()
      ("expected_sender", account_name{})
   );*/
} FC_LOG_AND_RETHROW() }


BOOST_AUTO_TEST_SUITE_END()
