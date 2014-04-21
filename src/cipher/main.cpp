#include <wbcontainer.h>

#include <iostream>
#include <fstream>


int processFile(std::string inFilename, std::string outFilename, WBContainer *container){
    std::ofstream ofs(outFilename, std::ios::binary);
    if (! ofs){
        std::cout << "ERROR: can't create/open output file.";
        return -1;
    }

    std::ifstream ifs(inFilename, std::ios::binary);
    if (! ifs){
        std::cout << "ERROR: can't open input file.";
        return -1;
    }

    auto *data = (std::istream::char_type*) malloc(64);
    auto *block = (std::istream::char_type*) malloc(64);
    while (true){
        ifs.read(data, 64);
        if (!ifs){
            if (ifs.gcount() != 0){
                container->processBlock((byte*)data, (byte*)block);
                ofs.write(block, 64);
                break;
            }
            break;
        }
        container->processBlock((byte*)data, (byte*)block);
        ofs.write(block, 64);
    }

    /*for (size_t i=0; i<1000000; ++i){
        container->processBlock((byte*)data, (byte*)block);
    }*/
    free(data);
    free(block);

    return 0;
}


int main(){
    /*byte key[64] = {
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
    };*/

    byte key[64] = {
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
    };

    /*byte key[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };*/

    /*byte data[64] = {
        1, 2, 3, 4, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 2,
    };
    byte *block = (byte*)malloc(64);*/

    std::string inputFile = "zero";
    std::string outputFile = "zero.enc";

    /*std::string inputFile = "source.bin";
    std::string outputFile = "wbc.bin.enc";*/

    auto container = WBContainer(&key[0]);
    return processFile(inputFile, outputFile, &container);
}
