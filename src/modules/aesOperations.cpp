#include"aesOperations.hpp"
#include"tables.cpp"
#include"galoisOperations.hpp"
#include"fileManipulations.hpp"

uint8_t subByte(const uint8_t byte, const array<array<uint8_t, 16>, 16> &sBox) {
    return sBox[byte >> 4][byte & 0x0F];
}

void subBytes(array<array<uint8_t, 4>, 4> &block, const array<array<uint8_t, 16>, 16> &sBox){
    for(int i = 0;i<block.size();i++){
        for(int j = 0;j<block[i].size();j++){
            block[i][j] = subByte(block[i][j], sBox);
        }
    }
}

array<uint8_t,4> g_function(const array<uint8_t,4> &w, int round){
    array<uint8_t,4> g_operation(w);

    rotate(g_operation.begin(),g_operation.begin() + 1, g_operation.end());

    for(int i = 0;i<g_operation.size();i++){
        g_operation[i] = subByte(g_operation[i], sBox);
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





void mixColumns(array<array<uint8_t, 4>, 4> &block, const array<array<uint8_t, 4>, 4> &columnTable){ // OK
    for(int coluna = 0;coluna<block.size();coluna++){
        array<uint8_t,4> elementos;
        for(int linha = 0;linha<block[coluna].size();linha++){
            elementos[linha] = block[linha][coluna];
        }
        
        for(int linha = 0;linha<block[coluna].size();linha++){
            uint8_t sum = operationGF(elementos,linha,columnTable);
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

void inverseShiftRows(array<array<uint8_t, 4>, 4> &block){
    for(int i = 1;i<block.size();i++){
        rotate(block[i].rbegin(), block[i].rbegin() + i, block[i].rend());
    }
}



array<array<uint8_t, 4>, 4> aesEncript(array<array<uint8_t, 4>, 4> block, array<array<uint8_t, 4>, 4> key, int n_rounds){

    //Initial round
    addRoundKey(key,block);



    //1 : n-1 rounds
    for(int i = 1;i<n_rounds;i++){
        subBytes(block, sBox);
        shiftRows(block);
        mixColumns(block, columnTable);


        key = keyExpansion(key,i);
        addRoundKey(key,block);
    }

    //Final round
    subBytes(block, sBox);
    shiftRows(block);
    key = keyExpansion(key,n_rounds);
    addRoundKey(key,block);

    return block;
}

array<array<uint8_t, 4>, 4> aesDecript(array<array<uint8_t, 4>, 4> block, array<array<uint8_t, 4>, 4> key, int n_rounds){
    vector<array<array<uint8_t, 4>, 4>> roundKeys;
    roundKeys.push_back(key);

    for(int i = 1;i<=n_rounds;i++){
        roundKeys.push_back(keyExpansion(roundKeys[i-1],i));
    }

    addRoundKey(roundKeys[roundKeys.size()-1],block);
    roundKeys.pop_back();

    for(int i = 1;i<n_rounds;i++){
        inverseShiftRows(block);
        subBytes(block,invSBox);
        addRoundKey(roundKeys[roundKeys.size()-1],block);
        mixColumns(block,invColumnTable);
        roundKeys.pop_back();
    }

    inverseShiftRows(block);
    subBytes(block,invSBox);
    addRoundKey(roundKeys[roundKeys.size()-1],block);
    roundKeys.pop_back();

    return block;
}

array<array<uint8_t, 4>, 4> generate_IV_matrix(){
    array<array<uint8_t, 4>, 4> matrix;


    // uint64_t n = generateRandom();
    uint64_t n = 0xa0dab4844e1a894b;

    
    int shift = 56;

    for(int i = 0;i<matrix.size();i++){
        matrix[i].fill(0);
    }

    for(int i = 0;i<2;i++){
        for(int j = 0;j<4;j++){
            matrix[j][i] = (uint8_t) (n >> (shift)) & 0XFF;
            shift-=8;
        }
    }


    return matrix;
}

array<array<uint8_t, 4>, 4> getEncriptedCTRIV(const array<array<uint8_t, 4>, 4> &IV, const array<array<uint8_t, 4>, 4> &key, uint64_t contador, int rounds){
    array<array<uint8_t,4>,4> IVRound(IV);

    int shift = 56;
    for(int i = 2;i<IV.size();i++){
        for(int j = 0;j<IV[i].size();j++){
            uint8_t xored_number = (contador >> (shift)) & 0xFF;
            // IVRound[j][i] = (IV[i][j]^(xored_number));
            IVRound[j][i] = xored_number;
            shift -= 8;
        }
    }
    // for(int i = 0;i<IV.size();i++){
    //     for(int j = 0;j<IV[i].size();j++){
    //         cout << hex << (int)IVRound[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";

    

    array<array<uint8_t,4>,4> encriptedCTRIV(aesEncript(IVRound,key,rounds));

    return encriptedCTRIV;
}

array<array<uint8_t, 4>, 4> getEncriptedCipherBlock(const array<array<uint8_t, 4>, 4> &encriptedCTRIV, const array<array<uint8_t, 4>, 4> &block){
    array<array<uint8_t, 4>, 4> cipherBlock;

    for(int i = 0;i<encriptedCTRIV.size();i++){
        for(int j = 0;j<encriptedCTRIV[i].size();j++){
            cipherBlock[i][j] = (encriptedCTRIV[i][j]^block[i][j]);
        }
    }

    return cipherBlock;
}

vector<array<array<uint8_t, 4>, 4>> aesCTR(array<array<uint8_t, 4>, 4> key, string filepath, int rounds, array<array<uint8_t, 4>, 4> IV){
    // array<array<uint8_t,4>,4> IV = generate_IV_matrix();

    vector<array<array<uint8_t,4>,4>> encriptedFile(generateBlocks(filepath));

    for(uint64_t i = 0;i<encriptedFile.size();i++){
        array<array<uint8_t,4>,4> encriptedCTRIV = getEncriptedCTRIV(IV,key,i,rounds);
        array<array<uint8_t,4>,4> cipherBlock = getEncriptedCipherBlock(encriptedCTRIV,encriptedFile[i]);

        encriptedFile[i] = cipherBlock;
    }
    
    return encriptedFile;
}