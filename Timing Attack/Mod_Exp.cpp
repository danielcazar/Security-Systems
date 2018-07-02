#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
 
extern inline unsigned long square_multiply(unsigned long base, unsigned long exponent, unsigned long modulus);
 
 
// This cpp file contains two implementations of modular exponentiation.
// The data types are tailored to support 32-bit values for base and exponent
 
int main(int argc, char *argv[])
{
    std::vector<std::vector<unsigned long> > input_vector;
    FILE *input_fp;
    int method_index;
    int repeat_count;
 
    if (argc != 4) {
        std::cout << "Error: incorrect argument" << std::endl;
        std::cout << "Usage: ./mod_exp [input file] [method index] [repeat count]" << std::endl;
        return 1;
    }
 
    // 1. Read test vectors from the input file
    input_fp = fopen(argv[1], "r");
 
    while (true) {
        char input_line[256];
 
        // Read a line from input_fp
        fgets(input_line, 256, input_fp);
        if (feof(input_fp))
            break;
 
        // Parse the line into three tokens
        char tokens[3][256];
        int curr_token_index = 0;
        int index_within_token = 0;
        for (int index = 0; input_line[index] != 0; index++) {
            if (input_line[index] == ',') {
                tokens[curr_token_index][index_within_token] = 0;
                curr_token_index++;
                index_within_token = 0;
                if (curr_token_index >= 3) {
                    break;
                }
            } else {
                tokens[curr_token_index][index_within_token] = input_line[index];
                index_within_token++;
            }
        }
        tokens[curr_token_index][index_within_token] = 0;
 
        if (curr_token_index != 2) {
            // Ignore invalid line
            std::cout << "Warning! Input line ignored:" << input_line;
        }
        else {
            // Add the parsed three tokens as a new test vector
            std::vector<unsigned long> new_test_vector;
            new_test_vector.push_back(atol(tokens[0]));
            new_test_vector.push_back(atol(tokens[1]));
            new_test_vector.push_back(atol(tokens[2]));
            input_vector.push_back(new_test_vector);
        }
    }
 
    fclose(input_fp);
 
#ifdef DEBUG
    // Print test vectors
    std::cout << "### Test vectors" << std::endl;
    for (int vector_index = 0; vector_index < (int) input_vector.size(); vector_index++) {
        assert(input_vector[vector_index].size() == 3);
        std::cout << input_vector[vector_index][0] << " " << input_vector[vector_index][1] << " " << input_vector[vector_index][2] << std::endl;
    }
#endif
 
    // Run the exponentiation method
    method_index = atoi(argv[2]);
    repeat_count = atoi(argv[3]);
 
    for (int vector_index = 0; vector_index < (int) input_vector.size(); vector_index++) {
        unsigned long base = input_vector[vector_index][0];
        unsigned long exponent = input_vector[vector_index][1];
        unsigned long modulus = input_vector[vector_index][2];
        struct timeval time_start, time_end;
        long elapsed_time = 0;
        unsigned long result;
 
        gettimeofday(&time_start, NULL);
 
        for (int trial = 0; trial < repeat_count; trial++) {
            if (method_index == 0) {
                result = square_multiply(base, exponent, modulus);
        }      
        else {
                std::cout << "Error: the argument [method index] is out of range" << std::endl;
                return 1;
            }
        }
 
        gettimeofday(&time_end, NULL);
        elapsed_time += (time_end.tv_sec * 1000000 + time_end.tv_usec) - (time_start.tv_sec * 1000000 + time_start.tv_usec);
 
        std::cout << "Test vector " << (vector_index + 1) << " (" << base << "^" << exponent << "%" << modulus << "=" << result << ") / Total elapsed time " << elapsed_time << " usec (average " << (elapsed_time / repeat_count) << " usec)" << std::endl;
    }
 
    return 0;
}
 
inline unsigned long square_multiply(unsigned long base, unsigned long exponent, unsigned long modulus)
{
    // Compute modular exponentiation using square-and-multiply
 
    unsigned long product = 1;
    const int bitwidth = 32;
 
    for (int count = bitwidth - 1; count >= 0; count--) {
        product = (product * product) % modulus;
        if (((exponent >> count) & 0x1) == 0x1) {
            product = (product * base) % modulus;
        }
    }
 
    return product;
}