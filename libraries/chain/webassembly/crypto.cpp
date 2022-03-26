#include <eosio/chain/webassembly/interface.hpp>
#include <eosio/chain/protocol_state_object.hpp>
#include <eosio/chain/transaction_context.hpp>
#include <eosio/chain/apply_context.hpp>


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

   void interface::alt_bn128_add(span<char> op1, span<char> op2, span<char> result) const {
      datastream<const char*> ds( op1.data(), op1.size() );
     // fc::raw::unpack(ds, s);
      fc::snark::fc_span _op1;
      fc::snark::fc_span _op2;
      fc::snark::fc_span _result;

      int32_t retCode = fc::snark::alt_bn128_add(_op1, _op2, &_result);
   }

   void interface::alt_bn128_mul(span<char> g1_point, span<char> scalar, span<char> result) const {
   }

   void interface::alt_bn128_pair(span<char> g1_pairs, span<char> g2_pairs, bool result) const {
   }

   void interface::mod_exp(uint32_t len_base, 
                           uint32_t len_exp, 
                           uint32_t len_modulus,
                           span<char> base, 
                           span<char> exp, 
                           span<char> modulus, 
                           span<char> out) const {

   }

}}} // ns eosio::chain::webassembly

/*

   int32_t interface::recover_key_safe( span<const char> digest,
                                        span<const char> sig,
                                        span<char> pub,
                                        uint32_t* publen) const {
      using error_code = eosio::chain::webassembly::error_codes::recover_key_safe;

      try {
         fc::crypto::signature s;
         try {
            datastream<const char*> ds( sig.data(), sig.size() );
            fc::raw::unpack(ds, s);
         } catch ( fc::exception& ) {
            return error_code::invalid_signature_format;
         }

         if( static_cast<unsigned>(s.which()) >= context.db.get<protocol_state_object>().num_supported_key_types ) {
            return error_code::unactivated_key_type;
         }

         if(context.control.is_producing_block())
            EOS_ASSERT(s.variable_size() <= context.control.configured_subjective_signature_length_limit(),
                       sig_variable_size_limit_exception, "signature variable length component size greater than subjective maximum");

         fc::sha256 _digest;
         try {
           _digest = fc::sha256(digest.data(), digest.size());
         } catch ( fc::exception& ) {
            return error_code::invalid_message_digest;
         }

         fc::crypto::public_key recovered;
         try {
            recovered = fc::crypto::public_key(s, _digest, false);
         } catch ( fc::exception& ) {
            return error_code::invalid_signature_data;
         }

         auto packed_pubkey = fc::raw::pack(recovered);
         if( pub.size() < packed_pubkey.size() ) {
            return error_code::insufficient_output_buffer;
         }
         std::memcpy(pub.data(), packed_pubkey.data(), packed_pubkey.size());
         *publen = packed_pubkey.size();
      } catch( const eosio::chain::sig_variable_size_limit_exception& ) {
         throw;
      } catch ( fc::exception& ) {
         return error_code::undefined;
      }
      return error_code::none;
   }

*/