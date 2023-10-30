#pragma once

#include<bits/stdc++.h>
using namespace std;

vector<array<array<uint8_t, 4>, 4>> generateBlocks(string filepath);

int fileSize(string filePath);

bool writeResult(vector<array<array<uint8_t, 4>, 4>> blocks, string filepath, int fileSize);