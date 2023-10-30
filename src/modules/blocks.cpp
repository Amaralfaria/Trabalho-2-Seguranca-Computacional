#pragma once

#include"blocks.hpp"


string desiredSizeString(string s){
    string block = s;

    int desired_characters = 32;
    int zeros_to_add = desired_characters - block.length();
    if(zeros_to_add > 0){
        string zeros(zeros_to_add,'0');
        block += zeros;
    }

    return block;
}

array<array<uint8_t, 4>, 4> stringToBlock(string block){
    block = desiredSizeString(block);
    uint64_t upper = stoull(block.substr(0,16),0,16);
    uint64_t lower = stoull(block.substr(16,16),0,16);
    array<array<uint8_t, 4>, 4> matrix;
    

    int shift = 56;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<4;j++){
            matrix[j][i] = (uint8_t) (upper >> (shift)) & 0XFF;
            shift-=8;
        }
    }

    shift = 56;

    for(int i = 2;i<4;i++){
        for(int j = 0;j<4;j++){
            matrix[j][i] = (uint8_t) (lower >> (shift)) & 0XFF;
            shift-=8;
        }
    }

    // for(int i = 0;i<matrix.size();i++){
    //     for(int j = 0;j<matrix[i].size();j++){
    //         cout << hex << (int) matrix[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";


    return matrix;
}

void printBlock(const array<array<uint8_t, 4>, 4> &block, int size){
    int chars_written = 0;

    for(int i = 0;i<block.size();i++){
        for(int j = 0;j<block[i].size() && chars_written < size;j++,chars_written++){
            cout << hex << (int) block[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}