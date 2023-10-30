#pragma once

#include<bits/stdc++.h>
using namespace std;

uint8_t subByte(const uint8_t byte, const array<array<uint8_t, 16>, 16> &sBox);

void subBytes(array<array<uint8_t, 4>, 4> &block, const array<array<uint8_t, 16>, 16> &sBox);

array<uint8_t,4> g_function(const array<uint8_t,4> &w, int round);

array<array<uint8_t, 4>, 4> keyExpansion(const array<array<uint8_t, 4>, 4> &key, int round);


void mixColumns(array<array<uint8_t, 4>, 4> &block, const array<array<uint8_t, 4>, 4> &columnTable);



void addRoundKey(const array<array<uint8_t, 4>, 4> &key, array<array<uint8_t, 4>, 4> &block);


void shiftRows(array<array<uint8_t, 4>, 4> &block);

void inverseShiftRows(array<array<uint8_t, 4>, 4> &block);



array<array<uint8_t, 4>, 4> aesEncript(array<array<uint8_t, 4>, 4> block, array<array<uint8_t, 4>, 4> key, int n_rounds);

array<array<uint8_t, 4>, 4> aesDecript(array<array<uint8_t, 4>, 4> block, array<array<uint8_t, 4>, 4> key, int n_rounds);

array<array<uint8_t, 4>, 4> generate_IV_matrix();

array<array<uint8_t, 4>, 4> getEncriptedCTRIV(const array<array<uint8_t, 4>, 4> &IV, const array<array<uint8_t, 4>, 4> &key, uint64_t contador, int rounds);


array<array<uint8_t, 4>, 4> getEncriptedCipherBlock(const array<array<uint8_t, 4>, 4> &encriptedCTRIV, const array<array<uint8_t, 4>, 4> &block);

vector<array<array<uint8_t, 4>, 4>> aesCTR(array<array<uint8_t, 4>, 4> key, string filepath, int rounds, array<array<uint8_t, 4>, 4> IV);
