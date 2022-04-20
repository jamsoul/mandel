#pragma once


namespace eosio { namespace chain { namespace webassembly { namespace error_codes {

   enum evm_precompiles : int32_t {
      undefined = -1, ///< undefined error
      none = 0, ///< succeed
      output_buffer_size_error, ///< output buffer size is not enough to hold actual results
      operand_decode_error, ///< one of the operands is not valid
      pair_list_size_error, ///< that
   };

}}}} // ns eosio::chain::webassembly::error_codes