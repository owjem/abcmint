#include "openssl/sha.h"
#include "uint256.h"
#include <bitset>

int C(int n, int m);
int M(uint64_t* Mask, int index);
uint8_t TM(uint64_t* Mask, int index);

void ArrayShiftRight(uint8_t array[], int len, int nShift);
int Binomial(int n, int m);
std::bitset<8> ByteToBits(unsigned char byte);

// Define a function set all equation elements to zero.
int*** CreateEquations(int n, int e);
uint8_t*** CreateEquationsUint8(int n, int e);

void FreeEquations(int e, int*** Eqs);
void FreeEquationsUint8(int e, uint8_t*** Eqs);

// Define a function to print equations for debugging.
void PrintEquation(int n, int e, int*** Eqs);


void Uint256ToBits(const unsigned char* bytes, std::bitset<256>& bits);
void Uint256ToSolutionBits(uint8_t* x, unsigned int nUnknowns, uint256 nonce);
void NewGenCoeffMatrix(uint256 hash, unsigned int nBits, std::vector<uint8_t>& coeffM);
void GenCoeffMatrix(uint256 hash, unsigned int nBits, std::vector<uint8_t>& coeffM);

void TransformTo3D(int n, int e, std::vector<uint8_t>& coefficientsMatrix, uint8_t*** Eqs);
void TransTo1D(int n, int e, std::vector<uint8_t>& coefficientsMatrix, uint8_t*** Eqs);
void SolutionBitsToUint256(uint8_t* x, unsigned int nUnknowns, uint256& nonce);
void AddQuadraticTerms(std::vector<uint8_t>& src, unsigned int n, unsigned int m, std::vector<uint32_t>& dest);
void LexToGradeReverseLex(std::vector<uint32_t>& src, unsigned int n, unsigned int m, uint32_t* coefficients);

// Transform coefficientsMatrix into the structure required by exfes.
void TransformDataStructure(int n, int e, std::vector<uint8_t>& coefficientsMatrix, int*** Eqs);

// Define a function to print solutions obtained from exfes.
int ReportSolution(uint64_t maxsol, uint64_t** SolArray, uint256& s);

// Define a function set all array elements to zero.
uint64_t** CreateArray(uint64_t maxsol);
void FreeArray(uint64_t maxsol, uint64_t** SolArray);
uint64_t ReverseUint64ByBits(uint64_t num);
