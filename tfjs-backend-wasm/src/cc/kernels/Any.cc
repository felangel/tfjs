/* Copyright 2021 Google LLC. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ===========================================================================*/

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <cstddef>

#include "tfjs-backend-wasm/src/cc/backend.h"

namespace tfjs {
namespace wasm {
// We use C-style API to interface with Javascript.
extern "C" {

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void Any(const size_t x_id, const size_t reduce_size, const size_t out_id) {
  auto& x_info = backend::get_tensor_info(x_id);
  auto& out_info = backend::get_tensor_info_out(out_id);

  const bool* x_buf = x_info.b();
  const size_t x_size = x_info.size;

  bool* out_buf = out_info.b_write();
  const size_t out_size = out_info.size;

  const bool* x_offset = x_buf;

  for (size_t i = 0; i < out_size; ++i) {
    const size_t offset = i * reduce_size;
    bool any = x_buf[offset];

    const bool* x_iter_end = x_offset + reduce_size;

    for (const bool* x = x_offset; x < x_iter_end; ++x) {
      bool value = *x;
      any = any || value;
    }

    x_offset += reduce_size;

    out_buf[i] = any;
  }
}

}  // extern "C"
}  // namespace wasm
}  // namespace tfjs
