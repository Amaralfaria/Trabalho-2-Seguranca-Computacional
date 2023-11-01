Para compilar o programa do zero basta rodar o makefile na pasta src ou realizar os seguintes comandos:
* g++ -std=c++17 -c modules/galoisOperations.cpp -o modules/galoisOperations.o
* g++ -std=c++17 -c modules/aesOperations.cpp -o modules/aesOperations.o
* g++ -std=c++17 -c modules/blocks.cpp -o modules/blocks.o
* g++ -std=c++17 -c modules/fileManipulations.cpp -o modules/fileManipulations.o
* g++ -std=c++17 modules/aesOperations.o modules/blocks.o modules/fileManipulations.o modules/galoisOperations.o aes.cpp -o aes.exe
