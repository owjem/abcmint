#ifndef BITCOIN_GPU_MINING_H
#define BITCOIN_GPU_MINING_H
#include "idxlut.h"
uint64_t GPUSearchSolution(uint32_t* coefficients, unsigned int number_of_variables,
    unsigned int number_of_equations);
int GetDeviceCount();
void SetDevice(int device);
#endif
