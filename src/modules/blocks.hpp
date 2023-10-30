#pragma once

#include<bits/stdc++.h>
using namespace std;

string desiredSizeString(string s);

array<array<uint8_t, 4>, 4> stringToBlock(string block);

void printBlock(const array<array<uint8_t, 4>, 4> &block, int size);