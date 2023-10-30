#pragma once

#include<bits/stdc++.h>

using namespace std;

static const array<array<uint8_t, 16>, 16> sBox;

static const array<array<uint8_t, 16>, 16> invSBox;

static const array<array<uint8_t, 4>, 4> columnTable;

static const array<array<uint8_t, 4>, 4> invColumnTable;
static const array<uint8_t,15> roundConstants;