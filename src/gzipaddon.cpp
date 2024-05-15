#include <node_api.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <zlib.h>

#define CHUNK 16384

// Function to unzip gzipped data
napi_value Unzip(napi_env env, napi_callback_info args) {
    size_t argc = 1;
    napi_value argv[1];
    napi_get_cb_info(env, args, &argc, argv, nullptr, nullptr);

    // Get the buffer length
    size_t length;
    napi_get_arraybuffer_info(env, argv[0], nullptr, &length);

    // Get the buffer data
    void* data;
    napi_get_arraybuffer_info(env, argv[0], &data, &length);

    // Prepare for decompression
    z_stream strm  = {0};
    strm.total_in  = strm.avail_in  = length;
    strm.next_in   = (Bytef *)data;

    inflateInit2(&strm, 16+MAX_WBITS);

    // Decompress
    char out[CHUNK];
    std::ostringstream result;

    do {
        strm.avail_out = CHUNK;
        strm.next_out = reinterpret_cast<Bytef *>(out);
        inflate(&strm, Z_NO_FLUSH);
        result.write(out, CHUNK - strm.avail_out);
    } while (strm.avail_out == 0);

    inflateEnd(&strm);

    // Convert the output to a Node.js string
    napi_value result_string;
    napi_create_string_utf8(env, result.str().c_str(), result.str().length(), &result_string);

    return result_string;
}

// Module initialization
napi_value Init(napi_env env, napi_value exports) {
    napi_value fn;
    napi_create_function(env, nullptr, 0, Unzip, nullptr, &fn);
    napi_set_named_property(env, exports, "unzip", fn);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
