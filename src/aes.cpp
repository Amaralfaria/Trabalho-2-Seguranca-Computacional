#include"modules/aesOperations.hpp"
#include"modules/fileManipulations.hpp"
#include"modules/blocks.hpp"

#include<bits/stdc++.h>
using namespace std;


int main(){
    // 3243f6a8885a308d313198a2e0370734 plain block
    // 2b7e151628aed2a6abf7158809cf4f3c chave

    //3925841d02dc09fbdc118597196a0b32 bloco_cifrado

    // arquivos_cifracao_decifracao/texto_teste.txt input
    // arquivos_cifracao_decifracao/output_ciphered.txt output
    // a0dab4844e1a894b IV

    int operacao,rounds;
    string chave;
    array<array<uint8_t, 4>, 4> key;

    cout << "Escolha uma operação:" << "\n";
    cout << "1 - Cifrar bloco unico" << "\n";
    cout << "2 - Decifrar bloco unico" << "\n";
    cout << "3 - Cifrar arquivo txt com AES-CTR" << "\n";

    cin >> operacao;

    cout << "Digite o numero de rodadas\n";
    cin >> rounds;

    cout << "Digite uma chave de 128 bits em hexadecimal\n";
    cin >> chave;

    key = stringToBlock(chave);
    

    if(operacao == 1 || operacao == 2){
        string sBloco;
        array<array<uint8_t, 4>, 4> block;

        cout << "Digite um bloco de 128 bits\n";
        cin >> sBloco;

        block = stringToBlock(sBloco);

        array<array<uint8_t, 4>, 4> result;

        result = operacao == 1?aesEncript(block,key,rounds):aesDecript(block,key,rounds);

        cout << "\nResultado:\n";
        printBlock(result, 16);
    }else if(operacao == 3){
        string inputPath,outputPath;
        string sIV;
        array<array<uint8_t, 4>, 4> IV;
        int n_characters;

        cout << "Digite o caminho do arquivo a ser cifrado\n";
        cin >> inputPath;

        cout << "Digite o caminho do arquivo destinado ao resultado da decifracao\n";
        cin >> outputPath;

        cout << "Digite um IV de 64 bits em hexadecimal\n";
        cin >> sIV;

        IV = stringToBlock(sIV);

        vector<array<array<uint8_t, 4>, 4>> ciphered = aesCTR(key,inputPath,rounds,IV);
        n_characters = fileSize(inputPath);
        writeResult(ciphered,outputPath,n_characters);
        cout << "Arquivo de output criado\n";
        cout << "Blocos gerados:\n";


        for(int i = 0;i<ciphered.size();i++, n_characters-=16){
            printBlock(ciphered[i],n_characters);
        }
    }



    std::ifstream opensslResult("output/arquivos_cifracao_decifracao/encryptedfile_ctr.txt", std::ios::binary);


    while(opensslResult.peek() != EOF){
        cout << hex << (int) opensslResult.get() << " ";
    }
    cout << "\n";

    opensslResult.close();

    return 0;
}


// openssl enc -aes-128-ctr -e -in texto_teste.txt -out encryptedfile_ctr.txt -K 2b7e151628aed2a6abf7158809cf4f3c -iv a0dab4844e1a894b -p -nosalt -nopad
