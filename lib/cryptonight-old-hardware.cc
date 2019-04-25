// Copyright (c) 2018 Gerik Bonaert <dev@keyhash.eu>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <nan.h>
#include <node.h>
#include <node_buffer.h>
#include <stdint.h>
#include <v8.h>

#if defined(__ARM_ARCH)
    #define XMRIG_ARM 1
    #include "vendor/crypto/CryptoNight_arm.h"
#else
    #include "vendor/extra.h"
    #include "vendor/crypto/CryptoNight_x86.h"
#endif

#include "vendor/Mem.h"

#define SOFT_AES true

static struct cryptonight_ctx* ctx = NULL;

void init_ctx() {
    if (ctx) return;
    Mem::create(&ctx, xmrig::CRYPTONIGHT_HEAVY, 1);
}

#define THROW_ERROR_EXCEPTION(x) Nan::ThrowError(x)

void callback(char* data, void* hint) {
    free(data);
}

using namespace node;
using namespace v8;
using namespace Nan;

NAN_METHOD(hash) {
        Local<Object> buf;
        uint32_t variant = 0;
        uint32_t height = 0;

    switch (info.Length()) {
        case 3:
			if (info[2]->IsUint32())
				height = info[2]->Int32Value();
			else
				return Nan::ThrowError("Third argument must be a number");	
		case 2:
            if (info[1]->IsUint32()) {
				variant = info[1]->Int32Value();
				if (variant >= 4 && info.Length() == 2) {
					return Nan::ThrowError("When variant is 4 or higher, height is required.");
				}
			} else {
				return Nan::ThrowError("Second argument must be a number");
			}
		case 1:
			buf = info[0]->ToObject();
			if (!Buffer::HasInstance(buf))
				return Nan::ThrowError("The first argument must be a buffer");
			if (variant > 0 && Buffer::Length(buf) < 43)
				return Nan::ThrowError("Buffer must be at least 43 bytes when variant > 0 is defined");
			break;
		case 0:
			return Nan::ThrowError("You must provide at least a buffer");
		default:
			return Nan::ThrowError("Invalid number of arguments.");
	}

    char output[32];
    init_ctx();
    switch (variant) {
       case 0:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_0>  (reinterpret_cast<const uint8_t*>(Buffer::Data(buf)), Buffer::Length(buf), reinterpret_cast<uint8_t*>(output), &ctx, height);
                break;
       case 1:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_1>  (reinterpret_cast<const uint8_t*>(Buffer::Data(buf)), Buffer::Length(buf), reinterpret_cast<uint8_t*>(output), &ctx, height);
                break;
       case 2:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_2>  (reinterpret_cast<const uint8_t*>(Buffer::Data(buf)), Buffer::Length(buf), reinterpret_cast<uint8_t*>(output), &ctx, height);
                break;
       case 4:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_4>  (reinterpret_cast<const uint8_t*>(Buffer::Data(buf)), Buffer::Length(buf), reinterpret_cast<uint8_t*>(output), &ctx, height);
                break;
       default: cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_2>  (reinterpret_cast<const uint8_t*>(Buffer::Data(buf)), Buffer::Length(buf), reinterpret_cast<uint8_t*>(output), &ctx, height);
    }

    info.GetReturnValue().Set(CopyBuffer(output, 32).ToLocalChecked());
}

class CCryptonightAsync : public Nan::AsyncWorker {

    private:

        struct cryptonight_ctx* m_ctx;
        const char* const m_input;
        const uint32_t m_input_len;
        const int m_variant;
        const int m_height;
        char m_output[32];
        MemInfo m_info;

    public:

        CCryptonightAsync(Nan::Callback* const callback, const char* const input, const uint32_t input_len, const int variant, const int height)
            : Nan::AsyncWorker(callback), m_ctx(static_cast<cryptonight_ctx *>(_mm_malloc(sizeof(cryptonight_ctx), 16))),
              m_input(input), m_input_len(input_len), m_variant(variant), m_height(height) {
            m_info = Mem::create(&m_ctx, xmrig::CRYPTONIGHT, 1);
        }

        ~CCryptonightAsync() {
            Mem::release(&m_ctx, 1, m_info);
        }

        void Execute () {
            switch (m_variant) {
                case 0:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_0>  (reinterpret_cast<const uint8_t*>(m_input), m_input_len, reinterpret_cast<uint8_t*>(m_output), &m_ctx, m_height);
                    break;
                case 1:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_1>  (reinterpret_cast<const uint8_t*>(m_input), m_input_len, reinterpret_cast<uint8_t*>(m_output), &m_ctx, m_height);
                    break;
                case 2:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_2>  (reinterpret_cast<const uint8_t*>(m_input), m_input_len, reinterpret_cast<uint8_t*>(m_output), &m_ctx, m_height);
                    break;
                case 4:  cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_4>  (reinterpret_cast<const uint8_t*>(m_input), m_input_len, reinterpret_cast<uint8_t*>(m_output), &m_ctx, m_height);
                    break;
                default: cryptonight_single_hash<xmrig::CRYPTONIGHT, SOFT_AES, xmrig::VARIANT_2>  (reinterpret_cast<const uint8_t*>(m_input), m_input_len, reinterpret_cast<uint8_t*>(m_output), &m_ctx, m_height);
            }
        }

        void HandleOKCallback () {
            Local<Value> argv[] = {
                CopyBuffer(m_output, 32).ToLocalChecked()
            };

            callback->Call(1, argv);
        }
};

NAN_METHOD(asyncHash) {
    Local<Object> buf;
	uint32_t variant = 0;
    uint32_t height = 0;
	Callback *callback;

	switch (info.Length()) {
        case 4:
			if (info[3]->IsFunction())
				callback = new Callback(To<Function>(info[3]).ToLocalChecked());
			else
				return Nan::ThrowError("Fourth argument must be a callback");
		case 3:
			if (info.Length() >= 4) {
				if (info[2]->IsUint32())
					height = info[2]->Int32Value();
				else
					return Nan::ThrowError("Third argument must be a number");
			} else {
				if (info[2]->IsFunction())
					callback = new Callback(To<Function>(info[2]).ToLocalChecked());
				else
					return Nan::ThrowError("Third argument must be a callback");
			}
		case 2:
			if (info.Length() >= 3) {
				if (info[1]->IsUint32())
					variant = info[1]->Int32Value();
				else 
					return Nan::ThrowError("Second argument must be a number");
			} else {
				callback = new Callback(To<Function>(info[1]).ToLocalChecked());
			}
			buf = info[0]->ToObject();
			if (!Buffer::HasInstance(buf))
				return Nan::ThrowError("The first argument must be a buffer");
			if (variant > 0 && Buffer::Length(buf) < 43)
				return Nan::ThrowError("Buffer must be at least 43 bytes when variant > 0 is defined");
			break;
		case 1:
		case 0:
			return Nan::ThrowError("You must provide at least a buffer and a callback");
		default:
			return Nan::ThrowError("Invalid number of arguments.");
	}

    Nan::AsyncQueueWorker(new CCryptonightAsync(callback, Buffer::Data(buf), Buffer::Length(buf), variant, height));
}

NAN_MODULE_INIT(init) {
    Nan::Set(target, Nan::New("hash").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(hash)).ToLocalChecked());
    Nan::Set(target, Nan::New("asyncHash").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(asyncHash)).ToLocalChecked());
}

NODE_MODULE(cryptonight, init)
