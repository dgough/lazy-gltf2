/// C++11 wrapper for a base64 decoding algorithm
/// This is part of the libb64 project, and has been placed in the public domain.
/// For details, see http://sourceforge.net/projects/libb64

#pragma once
#ifndef LIB64_HPP
#define LIB64_HPP
#include <iostream>
#ifndef BASE64_BUFFERSIZE
#define BASE64_BUFFERSIZE 1024
#endif
namespace lib64 {

    typedef enum {
        step_a, step_b, step_c, step_d
    } base64_decodestep;

    typedef struct {
        base64_decodestep step;
        char plainchar;
    } base64_decodestate;

    inline void base64_init_decodestate(base64_decodestate* state_in) {
        state_in->step = step_a;
        state_in->plainchar = 0;
    }

    static int base64_decode_value(char value_in) {
        static const char decoding[] = { 62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51 };
        static const char decoding_size = sizeof(decoding);
        value_in -= 43;
        if (value_in < 0 || value_in > decoding_size) return -1;
        return decoding[(int)value_in];
    }

    static int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in) {
        const char* codechar = code_in;
        char* plainchar = plaintext_out;
        char fragment;

        *plainchar = state_in->plainchar;

        switch (state_in->step) {
            while (1) {
        case step_a:
            do {
                if (codechar == code_in + length_in) {
                    state_in->step = step_a;
                    state_in->plainchar = *plainchar;
                    return static_cast<int>(plainchar - plaintext_out);
                }
                fragment = (char)base64_decode_value(*codechar++);
            } while (fragment < 0);
            *plainchar = (fragment & 0x03f) << 2;
        case step_b:
            do {
                if (codechar == code_in + length_in) {
                    state_in->step = step_b;
                    state_in->plainchar = *plainchar;
                    return static_cast<int>(plainchar - plaintext_out);
                }
                fragment = (char)base64_decode_value(*codechar++);
            } while (fragment < 0);
            *plainchar++ |= (fragment & 0x030) >> 4;
            *plainchar = (fragment & 0x00f) << 4;
        case step_c:
            do {
                if (codechar == code_in + length_in) {
                    state_in->step = step_c;
                    state_in->plainchar = *plainchar;
                    return static_cast<int>(plainchar - plaintext_out);
                }
                fragment = (char)base64_decode_value(*codechar++);
            } while (fragment < 0);
            *plainchar++ |= (fragment & 0x03c) >> 2;
            *plainchar = (fragment & 0x003) << 6;
        case step_d:
            do {
                if (codechar == code_in + length_in)
                {
                    state_in->step = step_d;
                    state_in->plainchar = *plainchar;
                    return static_cast<int>(plainchar - plaintext_out);
                }
                fragment = (char)base64_decode_value(*codechar++);
            } while (fragment < 0);
            *plainchar++ |= (fragment & 0x03f);
            }
        }
        /* control should not reach here */
        return static_cast<int>(plainchar - plaintext_out);
    }

    struct decoder {
        base64_decodestate _state;
        int _buffersize;

        decoder(int buffersize_in = BASE64_BUFFERSIZE) : _buffersize(buffersize_in) {}

        int decode(char value_in) {
            return base64_decode_value(value_in);
        }

        int decode(const char* code_in, const int length_in, char* plaintext_out) {
            return base64_decode_block(code_in, length_in, plaintext_out, &_state);
        }

        void decode(std::istream& istream_in, std::ostream& ostream_in) {
            base64_init_decodestate(&_state);
            //
            const int N = _buffersize;
            char* code = new char[N];
            char* plaintext = new char[N];
            int codelength;
            int plainlength;

            do {
                istream_in.read((char*)code, N);
                codelength = static_cast<int>(istream_in.gcount());
                plainlength = decode(code, codelength, plaintext);
                ostream_in.write((const char*)plaintext, plainlength);
            } while (istream_in.good() && codelength > 0);
            //
            base64_init_decodestate(&_state);

            delete[] code;
            delete[] plaintext;
        }
    };
}
#endif
