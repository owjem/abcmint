#ifndef BITCOIN_GPU_MINING_H
#define BITCOIN_GPU_MINING_H
uint64_t GPUSearchSolution(uint32_t* coefficients, unsigned int number_of_variables,
                           unsigned int number_of_equations);
#endif