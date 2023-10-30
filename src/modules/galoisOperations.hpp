#pragma once

#include<bits/stdc++.h>
using namespace std;

uint8_t multiplyInGF(uint8_t a, uint8_t b);

uint8_t addInGF(uint8_t a, uint8_t b);

uint8_t operationGF(const array<uint8_t, 4> &elements, const int linha,const array<array<uint8_t, 4>, 4> &columnTable);