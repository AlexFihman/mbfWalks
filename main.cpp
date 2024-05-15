#include <iostream>
#include <fstream>
#include <random>
#include <immintrin.h>
#include <stdint.h>
#include <sys/time.h>

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

bool cmp(uint64_t x, uint64_t y) {
    return (x | y) == y;
}

//bool cmpR(Record& r1, Record& r2) {
//    for (int i=0; i<8; i++)
//       if (!cmp(r1.data[i], r2.data[i])) return false;
//    return true;
//}

bool cmpR(Record& r1, Record& r2) {
    // Load data into AVX-512 registers
    __m512i r1_data = _mm512_loadu_si512((__m512i*) r1.data);
    __m512i r2_data = _mm512_loadu_si512((__m512i*) r2.data);

    // Compute (x | y)
    __m512i or_data = _mm512_or_si512(r1_data, r2_data);

    // Compare (x | y) == y
    __mmask8 cmp_mask = _mm512_cmpeq_epu64_mask(or_data, r2_data);

    // Check if all comparisons are true
    return cmp_mask == 0xFF;
}

static struct timeval _tstart, _tend;
static struct timezone tz;

double TimeMillis()
{
    gettimeofday(&_tstart, &tz);
	return (double)_tstart.tv_sec + (double)_tstart.tv_usec/(1000*1000);
};

int main() {
    // Create a random number generator
    std::random_device rd;
    std::mt19937 rng(rd());
    const int dim = 9;

    int size  = 1 << dim;
    int rsize = (1 << dim) / 64;
    Record* records = new Record[20000];
    
    //std::ofstream outputFile(generateRandomFilename().c_str(), std::ios::binary);
    //if (!outputFile.is_open()) {
    //    std::cerr << "Error: Failed to open the file for writing." << std::endl;
    //    return 1;
    //}    

    MonotoneBooleanFunction *mbfs[20000];
    for (int i = 0; i < 20000; i++) {
        mbfs[i] = new MonotoneBooleanFunction(dim, rng);
        for (int j=0; j<10000; j++) {
            mbfs[i]->flipRandom();
        }
        if (i%1000 == 0) std::cout << "initialized mbf: " << i << std::endl;
    }

    int num_rec = 0;
    while (true) {
       double t1 = TimeMillis();
       for (int i=0; i<20000; i++) {
           mbfs[i] -> step();
           mbfs[i] -> toRecord(records[i]);
           //if (i%1000 == 0) std::cout << "got mbf9 record: " << i << std::endl;
       }

       int cnt = 0;
       for (int i=0; i<10000; i++)
       for (int j=0; j<10000; j++)
           if (cmpR(records[i], records[j + 10000])) cnt++;
       //for (int i=0; i<20000; i++)
       //for (int j=0; j<20000; j++)
       //    if (i != j)
       //        if (cmpR(records[i], records[j])) cnt++;

       double t2 = TimeMillis();
       std::cout << num_rec << "\t" << cnt << "\t" << (t2-t1) << std::endl;
       num_rec++;    
    }
    //outputFile.close();

    return 0;
}
