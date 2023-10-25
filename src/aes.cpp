#include<bits/stdc++.h>

using namespace std;

static const array<array<uint8_t, 16>, 16> sBox = {{
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
}};

static const array<array<uint8_t, 4>, 4> columnTable = {{
    {2,3,1,1},
    {1,2,3,1},
    {1,1,2,3},
    {3,1,1,2}
}};

static const array<uint8_t,15> roundConstants = {
    {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D}
};

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

uint8_t operationGF(const array<uint8_t, 4> &elements, const int linha){
    uint8_t sum = 0;
    for(int i = 0;i<elements.size();i++){
        sum = addInGF(sum, multiplyInGF(elements[i], columnTable[linha][i]));
    }

    return sum;
}


uint8_t subByte(const uint8_t byte) {
    return sBox[byte >> 4][byte & 0x0F];
}

void subBytes(array<array<uint8_t, 4>, 4> &block){
    for(int i = 0;i<block.size();i++){
        for(int j = 0;j<block[i].size();j++){
            block[i][j] = subByte(block[i][j]);
        }
    }
}

array<uint8_t,4> g_function(const array<uint8_t,4> &w, int round){
    array<uint8_t,4> g_operation(w);

    rotate(g_operation.begin(),g_operation.begin() + 1, g_operation.end());

    for(int i = 0;i<g_operation.size();i++){
        g_operation[i] = subByte(g_operation[i]);
    }
    g_operation[0] ^= roundConstants[round]; 

    return g_operation;
}

array<array<uint8_t, 4>, 4> keyExpansion(const array<array<uint8_t, 4>, 4> &key, int round){
    array<array<uint8_t, 4>, 4> current_key;
    array<array<uint8_t, 4>, 4> next_key;
    array<array<uint8_t, 4>, 4> result;


    for(int coluna = 0;coluna<key.size();coluna++){
        for(int linha = 0;linha<key.size();linha++){
            current_key[coluna][linha] = key[linha][coluna];
        }
    }

    array<uint8_t,4> g_operation = g_function(current_key[current_key.size()-1], round);



    for(int i = 0;i<current_key.size();i++){
        for(int j = 0;j<current_key[i].size();j++){
            if(i != 0){
                next_key[i][j] = (current_key[i][j] ^ next_key[i-1][j]);
            }else{
                next_key[i][j] = (current_key[i][j] ^ g_operation[j]);
            }
        }
    }

    for(int i = 0;i<next_key.size();i++){
        for(int j = 0;j<next_key[i].size();j++){
            result[j][i] = next_key[i][j];
        }
    }




    return result;
}





void mixColumns(array<array<uint8_t, 4>, 4> &block){ // OK
    for(int coluna = 0;coluna<block.size();coluna++){
        array<uint8_t,4> elementos;
        for(int linha = 0;linha<block[coluna].size();linha++){
            elementos[linha] = block[linha][coluna];
        }
        
        for(int linha = 0;linha<block[coluna].size();linha++){
            uint8_t sum = operationGF(elementos,linha);
            block[linha][coluna] = sum;
        }
    }
}



void addRoundKey(const array<array<uint8_t, 4>, 4> &key, array<array<uint8_t, 4>, 4> &block){
    for(int i = 0;i<block.size();i++){
        for(int j = 0;j<block[i].size();j++){
            block[i][j] = (block[i][j] ^ key[i][j]);
        }
    }
}


void shiftRows(array<array<uint8_t, 4>, 4> &block){
    for(int i = 1;i<block.size();i++){
        rotate(block[i].begin(), block[i].begin() + i, block[i].end());
    }
}

array<array<uint8_t, 4>, 4> aesEncript(array<array<uint8_t, 4>, 4> block, array<array<uint8_t, 4>, 4> key, int n_rounds){
    //Initial round
    addRoundKey(key,block);

    //1 : n-1 rounds
    for(int i = 1;i<n_rounds;i++){
        subBytes(block);
        shiftRows(block);
        mixColumns(block);
        key = keyExpansion(key,i);
        addRoundKey(key,block);


        // cout << "Round: " << i+1 << "\n";
    }

    //Final round
    subBytes(block);
    shiftRows(block);
    key = keyExpansion(key,n_rounds);
    addRoundKey(key,block);

    return block;
}





int main(){
    array<array<uint8_t, 4>, 4> key = {
        {
        {0x2b,0x28,0xab,0x09},
        {0x7e,0xae,0xf7,0xcf},
        {0x15,0xd2,0x15,0x4f},
        {0x16,0xa6,0x88,0x3c}
        }
    };

    array<array<uint8_t, 4>, 4> block = {
        {
        {0x32,0x88,0x31,0xe0},
        {0x43,0x5a,0x31,0x37},
        {0xf6,0x30,0x98,0x07},
        {0xa8,0x8d,0xa2,0x34}
        }
    };

    array<array<uint8_t, 4>, 4> encripted_block = aesEncript(block,key,10);


    for(int i = 0;i<encripted_block.size();i++){
        for(int j = 0;j<encripted_block.size();j++){
            cout << hex << (int) encripted_block[i][j] << " ";
        }
        cout << "\n";
    }



    return 0;
}