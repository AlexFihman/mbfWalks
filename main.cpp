#include <iostream>
#include <fstream>
#include <random>
#include "ShortList.h"
#include "MonotoneBooleanFunction.h"

std::string generateRandomFilename() {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int length = 10;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    std::string filename;
    for (int i = 0; i < length; ++i) {
        filename += chars[dis(gen)];
    }
    filename += ".bin"; // Add the file extension
    return filename;
}

int main() {
    // Create a random number generator
    std::random_device rd;
    std::mt19937 rng(rd());
    const int dim = 9;

    int size  = 1 << dim;
    int rsize = (1 << dim) / 64;
    uint64_t record[rsize];

    std::ofstream outputFile(generateRandomFilename().c_str(), std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Failed to open the file for writing." << std::endl;
        return 1;
    }

    MonotoneBooleanFunction mbf(dim, rng);
    for (long int i=0; i<10000; i++) {
        int mc = mbf.getRandomMinCut();
        mbf.flip(mc);
    }

    int num_rec = 0;
    while (num_rec < 1000000) {
        int mc = mbf.getRandomMinCut();
        mbf.flip(mc);

        if (rng() % mbf.minCutSize() == 0) {
           for (int j = 0; j < rsize; j++) record[j] = 0;
           for (int j = 0; j < size; j++) {
               if (mbf.getFunctionValue(j)) {
                   int p = (size - j - 1) / 64;
                   int k = (size - j - 1) % 64;
                   record[p] ^= (1ULL << k);
               }
           }
           outputFile.write(reinterpret_cast<const char*>(record), sizeof(record));
           num_rec++;
        }
    }
    outputFile.close();
    return 0;
}
