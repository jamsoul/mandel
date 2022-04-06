#pragma once

namespace eosio { namespace chain { namespace webassembly { namespace error_codes {

   enum alt_bn128 : int32_t {
      undefined = -1, ///< undefined error
      none = 0, ///< succeed
      operand_size_error,
      invalid_signature_format,
      unactivated_key_type, 
      invalid_signature_data, 
      insufficient_output_buffer, 
   };

}}}} 
