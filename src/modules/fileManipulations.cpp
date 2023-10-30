#pragma once

#include"fileManipulations.hpp"

vector<array<array<uint8_t, 4>, 4>> generateBlocks(string filepath){
    ifstream arquivo(filepath, std::ios::binary);
    vector<array<array<uint8_t, 4>, 4>> blocks;

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return blocks;
    }

    array<array<uint8_t,4>,4> block;
    for(int i = 0;i<block.size();i++){
        block[i].fill(0);
    }

    int pos = 0;
    bool empty = true;



    while(arquivo.peek() != EOF){
        uint8_t byte = arquivo.get();
        empty = false;
        block[pos%4][pos/4] = byte;
        pos++;

        if(pos>15){
            blocks.push_back(array<array<uint8_t,4>,4>(block));
            
            for(int i = 0;i<block.size();i++){
                block[i].fill(0);
            }

            pos = 0;
            empty = true;
        }
    }

    if(!empty){
        blocks.push_back(array<array<uint8_t,4>,4>(block));
    }

    

    arquivo.close();

    return blocks;
}

int fileSize(string filePath){
    ifstream file(filePath,ios::binary);
    int counter = 0;

    while(file.peek() != EOF){
        file.get();
        counter++;
    }

    file.close();

    return counter;
}

bool writeResult(vector<array<array<uint8_t, 4>, 4>> blocks, string filepath, int fileSize){
    ofstream output(filepath, ios::binary);
    int chars_written = 0;

    if (!output.is_open()) {
        std::cerr << "Erro ao escrever no arquivo." << std::endl;
        return false;
    }



    for(int i = 0;i<blocks.size();i++){
        for(int j = 0;j< blocks[i].size();j++){
            for(int k = 0;k <blocks[i][j].size() && chars_written < fileSize;k++, chars_written++){
                output.put(blocks[i][k][j]);
                // cout << hex << (int) blocks[i][k][j] << " ";
            }
            // cout << "\n";
        }
        // cout << "\n";
    }
    // cout << "\n";

    output.close();
    
    return true;
}