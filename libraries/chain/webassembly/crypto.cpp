#include <eosio/chain/webassembly/interface.hpp>
#include <eosio/chain/protocol_state_object.hpp>
#include <eosio/chain/transaction_context.hpp>
#include <eosio/chain/apply_context.hpp>
#include <eosio/chain/webassembly/error_codes.hpp>

namespace eosio { namespace chain { namespace webassembly {

   void interface::assert_recover_key( legacy_ptr<const fc::sha256> digest,
                                       legacy_span<const char> sig,
                                       legacy_span<const char> pub ) const {
      fc::crypto::signature s;
      fc::crypto::public_key p;
      datastream<const char*> ds( sig.data(), sig.size() );
      datastream<const char*> pubds ( pub.data(), pub.size() );

      fc::raw::unpack( ds, s );
      fc::raw::unpack( pubds, p );

      EOS_ASSERT(static_cast<unsigned>(s.which()) < context.db.get<protocol_state_object>().num_supported_key_types, unactivated_signature_type,
        "Unactivated signature type used during assert_recover_key");
      EOS_ASSERT(static_cast<unsigned>(p.which()) < context.db.get<protocol_state_object>().num_supported_key_types, unactivated_key_type,
        "Unactivated key type used when creating assert_recover_key");

      if(context.control.is_producing_block())
         EOS_ASSERT(s.variable_size() <= context.control.configured_subjective_signature_length_limit(),
                    sig_variable_size_limit_exception, "signature variable length component size greater than subjective maximum");

      auto check = fc::crypto::public_key( s, *digest, false );
      EOS_ASSERT( check == p, crypto_api_exception, "Error expected key different than recovered key" );
   }

   int32_t interface::recover_key( legacy_ptr<const fc::sha256> digest,
                                   legacy_span<const char> sig,
                                   legacy_span<char> pub ) const {
      fc::crypto::signature s;
      datastream<const char*> ds( sig.data(), sig.size() );
      fc::raw::unpack(ds, s);

      EOS_ASSERT(static_cast<unsigned>(s.which()) < context.db.get<protocol_state_object>().num_supported_key_types, unactivated_signature_type,
                 "Unactivated signature type used during recover_key");

      if(context.control.is_producing_block())
         EOS_ASSERT(s.variable_size() <= context.control.configured_subjective_signature_length_limit(),
                    sig_variable_size_limit_exception, "signature variable length component size greater than subjective maximum");


      auto recovered = fc::crypto::public_key(s, *digest, false);

      // the key types newer than the first 2 may be varible in length
      if (static_cast<unsigned>(s.which()) >= config::genesis_num_supported_key_types ) {
         EOS_ASSERT(pub.size() >= 33, wasm_execution_error,
                    "destination buffer must at least be able to hold an ECC public key");
         auto packed_pubkey = fc::raw::pack(recovered);
         auto copy_size = std::min<size_t>(pub.size(), packed_pubkey.size());
         std::memcpy(pub.data(), packed_pubkey.data(), copy_size);
         return packed_pubkey.size();
      } else {
         // legacy behavior, key types 0 and 1 always pack to 33 bytes.
         // this will do one less copy for those keys while maintaining the rules of
         //    [0..33) dest sizes: assert (asserts in fc::raw::pack)
         //    [33..inf) dest sizes: return packed size (always 33)
         datastream<char*> out_ds( pub.data(), pub.size() );
         fc::raw::pack(out_ds, recovered);
         return out_ds.tellp();
      }
   }

   void interface::assert_sha256(legacy_span<const char> data, legacy_ptr<const fc::sha256> hash_val) const {
      auto result = context.trx_context.hash_with_checktime<fc::sha256>( data.data(), data.size() );
      EOS_ASSERT( result == *hash_val, crypto_api_exception, "hash mismatch" );
   }

   void interface::assert_sha1(legacy_span<const char> data, legacy_ptr<const fc::sha1> hash_val) const {
      auto result = context.trx_context.hash_with_checktime<fc::sha1>( data.data(), data.size() );
      EOS_ASSERT( result == *hash_val, crypto_api_exception, "hash mismatch" );
   }

   void interface::assert_sha512(legacy_span<const char> data, legacy_ptr<const fc::sha512> hash_val) const {
      auto result = context.trx_context.hash_with_checktime<fc::sha512>( data.data(), data.size() );
      EOS_ASSERT( result == *hash_val, crypto_api_exception, "hash mismatch" );
   }

   void interface::assert_ripemd160(legacy_span<const char> data, legacy_ptr<const fc::ripemd160> hash_val) const {
      auto result = context.trx_context.hash_with_checktime<fc::ripemd160>( data.data(), data.size() );
      EOS_ASSERT( result == *hash_val, crypto_api_exception, "hash mismatch" );
   }

   void interface::sha1(legacy_span<const char> data, legacy_ptr<fc::sha1> hash_val) const {
      *hash_val = context.trx_context.hash_with_checktime<fc::sha1>( data.data(), data.size() );
   }

   void interface::sha256(legacy_span<const char> data, legacy_ptr<fc::sha256> hash_val) const {
      *hash_val = context.trx_context.hash_with_checktime<fc::sha256>( data.data(), data.size() );
   }

   void interface::sha512(legacy_span<const char> data, legacy_ptr<fc::sha512> hash_val) const {
      *hash_val = context.trx_context.hash_with_checktime<fc::sha512>( data.data(), data.size() );
   }

   void interface::ripemd160(legacy_span<const char> data, legacy_ptr<fc::ripemd160> hash_val) const {
      *hash_val = context.trx_context.hash_with_checktime<fc::ripemd160>( data.data(), data.size() );
   }

   int32_t interface::alt_bn128_add(span<const char> op1, span<const char> op2, span<char> result ) const {
      using error_code = eosio::chain::webassembly::error_codes::evm_precompiles;

      fc::snark::bytes _op1(op1.data(), op1.data() + op1.size());
      fc::snark::bytes _op2(op2.data(), op2.data() + op2.size());

      try {
         auto retCall = fc::snark::alt_bn128_add(_op1, _op2);

         if (retCall.first == fc::snark::error_codes::none) {
            auto &response = retCall.second;
            if (result.size()>=response.size()) {
               std::copy(response.begin(), response.end(), result.data());
            } else {
               return error_code::output_buffer_size_error;
            }
         } else {
            return error_code::operand_decode_error;
         }

      } catch ( fc::exception& ) {
         return error_code::undefined;
      }


      return error_code::none;
   }

   int32_t interface::alt_bn128_mul(span<const char> g1_point, span<const char> scalar, span<char> result) const {
      using error_code = eosio::chain::webassembly::error_codes::evm_precompiles;

      fc::snark::bytes _g1_point(g1_point.data(), g1_point.data() + g1_point.size());
      fc::snark::bytes _scalar(scalar.data(), scalar.data() + scalar.size());

      try {
         auto retCall = fc::snark::alt_bn128_mul(_g1_point, _scalar);

         if (retCall.first == fc::snark::error_codes::none) {
            auto & response = retCall.second;
            
            if (result.size()>=response.size()) {
               std::copy(response.begin(), response.end(), result.data());
            } else {
               return error_code::output_buffer_size_error;
            }
         } else {
            return error_code::operand_decode_error;
         }

      } catch ( fc::exception& ) {
         return error_code::undefined;
      }

      return error_code::none;
   }

   int32_t interface::alt_bn128_pair(span<const char> g1_g2_pairs, span<char> result) const {
      using error_code = eosio::chain::webassembly::error_codes::evm_precompiles;

      size_t constexpr pairSize = 2 * 32 + 2 * 64;
      size_t const pairs = g1_g2_pairs.size() / pairSize;
      if (pairs * pairSize != g1_g2_pairs.size())
         return error_code::pair_list_size_error; // buffer size mismatch pairs length

      fc::snark::bytes _g1_g2_pairs(g1_g2_pairs.data(), g1_g2_pairs.data() + g1_g2_pairs.size());

      try {
         auto retCall = fc::snark::alt_bn128_pair(_g1_g2_pairs);
         if (retCall.first == fc::snark::error_codes::none) {
            result.data()[0] = retCall.second?1:0;
         } else {
            return error_code::undefined; // no other significant errors than pair size check
         }

      } catch ( fc::exception& ) {
         return error_code::undefined;
      }

      return error_code::none;
   }

   int32_t interface::mod_exp(span<const char> base, 
                              span<const char> exp, 
                              span<const char> modulus, 
                              span<char> out) const {
      using error_code = eosio::chain::webassembly::error_codes::evm_precompiles;

      fc::snark::bytes _base(base.data(), base.data() + base.size());
      fc::snark::bytes _exp(exp.data(), exp.data() + exp.size());
      fc::snark::bytes _mod(modulus.data(), modulus.data() + modulus.size());

      try {
         auto retCall = fc::snark::modexp(base.size(), exp.size(), modulus.size(), _base, _exp, _mod);

         if (retCall.first == fc::snark::error_codes::none) {
            auto &response = retCall.second;
            if (out.size()>=response.size()) {
               std::copy(response.begin(), response.end(), out.data());
            } else {
               return error_code::output_buffer_size_error;
            }
         } else {
            return error_code::undefined; // no other significant error than modulus len zero
         }

      } catch ( fc::exception& ) {
         return error_code::undefined;
      }
      
      return error_code::none;
   }

}}}