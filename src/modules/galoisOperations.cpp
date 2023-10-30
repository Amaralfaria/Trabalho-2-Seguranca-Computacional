#pragma once

#include"galoisOperations.hpp"

uint8_t multiplyInGF(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    uint8_t hi_bit_set;
    for (int i = 0; i < 8; i++) {
        if ((b & 1) == 1) {
            result ^= a;
        }
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80) {
            a ^= 0x1B; // 0x1B é o polinômio irreduzível 0x11B em GF(2^8)
        }
        b >>= 1;
    }
    return result;
}

uint8_t addInGF(uint8_t a, uint8_t b) {
    return a ^ b;
}

uint8_t operationGF(const array<uint8_t, 4> &elements, const int linha,const array<array<uint8_t, 4>, 4> &columnTable){
    uint8_t sum = 0;
    for(int i = 0;i<elements.size();i++){
        sum = addInGF(sum, multiplyInGF(elements[i], columnTable[linha][i]));
    }

    return sum;
}