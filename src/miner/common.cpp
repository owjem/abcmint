
#include "common.h"
#include "util.h"
#include <iostream>


int C(int n, int m)
{
    if (m == 0)
        return 1;
    else if (m == 1)
        return n;
    else if (m == 2)
        return n * (n - 1) >> 1;
    else
        return 0;
}

int M(uint64_t* Mask, int index)
{
    if (index < 64)
        return (Mask[0] >> index) & 1;
    else
        return (Mask[1] >> (index - 64)) & 1;
}

uint8_t TM(uint64_t* Mask, int index)
{
    if (index < 64)
        return (uint8_t)((Mask[0] >> index) & 1);
    else
        return (uint8_t)((Mask[1] >> (index - 64)) & 1);
}

void ArrayShiftRight(uint8_t array[], int len, int nShift)
{
    int i = 0;
    uint8_t temp;
    do {
        i = (i + nShift) % len;
        temp = array[i];
        array[i] = array[0];
        array[0] = temp;
    } while (i);
}
std::bitset<8> ByteToBits(unsigned char byte)
{
    return std::bitset<8>(byte);
}
// Define the binomial function to calculate number of terms in different degrees.
int Binomial(int n, int m)
{
    if (m == 0)
        return 1;
    else if (m == 1)
        return n;
    else if (m == 2)
        return n * (n - 1) >> 1;
    else
        return 0;
}
uint8_t*** CreateEquationsUint8(int n, int e)
{
    uint8_t*** Eqs = (uint8_t***)calloc(e, sizeof(uint8_t**)); // Create an array for saving coefficients for exfes.
    if (!Eqs)
        LogPrintf("malloc Eqs failure!\n");
    for (int i = 0; i < e; i++) {
        Eqs[i] = (uint8_t**)calloc(3, sizeof(uint8_t*));
        if (!Eqs[i])
            LogPrintf("malloc Eqs failure!\n");
        for (int j = 0; j < 3; j++) {
            Eqs[i][j] = (uint8_t*)calloc(Binomial(n, j), sizeof(uint8_t));
            if (!Eqs[i][j])
                LogPrintf("malloc Eqs failure!\n");
        }
    }
    return Eqs;
}
// Define a function set all equation elements to zero.
int*** CreateEquations(int n, int e)
{
    int*** Eqs = (int***)calloc(e, sizeof(int**)); // Create an array for saving coefficients for exfes.
    if (!Eqs)
        LogPrintf("malloc Eqs failure!\n");
    for (int i = 0; i < e; i++) {
        Eqs[i] = (int**)calloc(3, sizeof(int*));
        if (!Eqs[i])
            LogPrintf("malloc Eqs failure!\n");
        for (int j = 0; j < 3; j++) {
            Eqs[i][j] = (int*)calloc(Binomial(n, j), sizeof(int));
            if (!Eqs[i][j])
                LogPrintf("malloc Eqs failure!\n");
        }
    }
    return Eqs;
}
void FreeEquations(int e, int*** Eqs)
{
    if (Eqs) {
        for (int i = 0; i < e; i++) {
            for (int j = 0; j < 3; j++) {
                if (Eqs[i][j])
                    free(Eqs[i][j]);
            }
            if (Eqs[i])
                free(Eqs[i]);
        }
        free(Eqs);
    }
}
void FreeEquationsUint8(int e, uint8_t*** Eqs)
{
    if (Eqs) {
        for (int i = 0; i < e; i++) {
            for (int j = 0; j < 3; j++) {
                if (Eqs[i][j])
                    free(Eqs[i][j]);
            }
            if (Eqs[i])
                free(Eqs[i]);
        }
        free(Eqs);
    }
}
// Define a function to print equations for debugging.
void PrintEquation(int n, int e, int*** Eqs)
{
    for (int i = 0; i < e; i++) {
        std::cout << "Eqs[" << i << "] =  ";
        for (int j = 2; j >= 0; j--)
            for (int k = 0; k < Binomial(n, j); k++)
                std::cout << " " << Eqs[i][j][k];
        std::cout << std::endl;
    }
}

void Uint256ToBits(const unsigned char* bytes, std::bitset<256>& bits)
{
    for (int i = 0; i < 32; i++) {
        std::bitset<8> tempbits = ByteToBits(bytes[i]);
        for (int j = 0; j < 8; j++) {
            bits[i * 8 + j] = tempbits[j];
        }
    }
}
void Uint256ToSolutionBits(uint8_t* x, unsigned int nUnknowns, uint256 nonce)
{
    std::bitset<256> bitnonce;
    Uint256ToBits(nonce.begin(), bitnonce);
    if (nUnknowns < 256) {
        for (unsigned int i = 0; i < nUnknowns; i++) {
            x[i] = (uint8_t)bitnonce[i];
        }
    } else {
        for (unsigned int i = 0; i < 256; i++) {
            x[i] = (uint8_t)bitnonce[i];
        }
    }
}
void NewGenCoeffMatrix(uint256 hash, unsigned int nBits, std::vector<uint8_t>& coeffM)
{
    unsigned int mEquations = nBits;
    unsigned int nUnknowns = nBits + 8;
    unsigned int nTerms = 1 + (nUnknowns + 1) * (nUnknowns) / 2;
    // generate the first polynomial coefficients.
    unsigned char in[32], out[32];
    unsigned int count = 0, i, j, k;
    uint8_t g[nTerms];
    std::bitset<256> bits;
    SHA256(hash.begin(), 32, in);
    for (i = 0; i < mEquations; i++) {
        count = 0;
        do {
            SHA256(in, 32, out);
            Uint256ToBits(out, bits);
            for (k = 0; k < 256; k++) {
                if (count < nTerms) {
                    g[count++] = (uint8_t)bits[k];
                } else {
                    break;
                }
            }
            for (j = 0; j < 32; j++) {
                in[j] = out[j];
            }
        } while (count < nTerms);
        for (j = 0; j < nTerms; j++) {
            coeffM[i * nTerms + j] = g[j];
        }
    }
}
void GenCoeffMatrix(uint256 hash, unsigned int nBits, std::vector<uint8_t>& coeffM)
{
    unsigned int mEquations = nBits;
    unsigned int nUnknowns = nBits + 8;
    unsigned int nTerms = 1 + (nUnknowns + 1) * (nUnknowns) / 2;

    // generate the first polynomial coefficients.
    unsigned char in[32], out[32];
    unsigned int count = 0, i, j, k;
    uint8_t g[nTerms];
    std::bitset<256> bits;
    SHA256(hash.begin(), 32, in);

    do {
        SHA256(in, 32, out);
        Uint256ToBits(out, bits);
        for (k = 0; k < 256; k++) {
            if (count < nTerms) {
                g[count++] = (uint8_t)bits[k];
            } else {
                break;
            }
        }
        for (j = 0; j < 32; j++) {
            in[j] = out[j];
        }
    } while (count < nTerms);

    // generate the rest polynomials coefficients by shiftint f[0] one bit
    for (i = 0; i < mEquations; i++) {
        ArrayShiftRight(g, nTerms, 1);
        for (j = 0; j < nTerms; j++)
            coeffM[i * nTerms + j] = g[j];
    }
}

void TransformTo3D(int n, int e, std::vector<uint8_t>& coefficientsMatrix, uint8_t*** Eqs)
{
    uint64_t offset = 0;
    for (int i = 0; i < e; i++) {
        for (int j = 0; j < Binomial(n, 2); j++)
            Eqs[i][2][j] = (uint8_t)coefficientsMatrix[offset + j];
        offset += Binomial(n, 2);
        for (int j = 0; j < n; j++)
            Eqs[i][1][j] = (uint8_t)coefficientsMatrix[offset + j];
        offset += n;
        Eqs[i][0][0] = (uint8_t)coefficientsMatrix[offset];
        offset += 1;
    }
}
void TransTo1D(int n, int e, std::vector<uint8_t>& coefficientsMatrix, uint8_t*** Eqs)
{
    uint64_t offset = 0;
    for (int i = 0; i < e; i++) {
        for (int j = 0; j < Binomial(n, 2); j++)
            coefficientsMatrix[offset + j] = Eqs[i][2][j];
        offset += Binomial(n, 2);
        for (int j = 0; j < n; j++)
            coefficientsMatrix[offset + j] = Eqs[i][1][j];
        offset += n;
        coefficientsMatrix[offset] = Eqs[i][0][0];
        offset += 1;
    }
}
void SolutionBitsToUint256(uint8_t* x, unsigned int nUnknowns, uint256& nonce)
{
    nonce = 0;
    uint256 base = 1;
    for (unsigned int i = 0; i < nUnknowns; i++) {
        if (x[i]) {
            nonce = (base << i) | nonce;
        }
    }
}
void AddQuadraticTerms(std::vector<uint8_t>& src, unsigned int n, unsigned int m, std::vector<uint32_t>& dest)
{
    int offset = 0, count = 0;
    for (unsigned int k = 0; k < m; k++) {
        for (unsigned int i = 0; i < n; i++) {
            for (unsigned int j = i; j < n; j++) {
                if (i == j) {
                    dest[offset++] = 0;
                } else {
                    dest[offset++] = src[count++];
                }
            }
        }
        for (unsigned int i = 0; i < n; i++) {
            dest[offset++] = src[count++];
        }
        dest[offset++] = src[count++];
    }
}
void LexToGradeReverseLex(std::vector<uint32_t>& src, unsigned int n, unsigned int m, uint32_t* coefficients)
{
    unsigned int nTerms = 1 + (((n + 1) * n) >> 1) + n;
    int offset = 0;
    for (unsigned int k = 0; k < m; k++) {
        int count = 0;
        for (unsigned int i = 0; i < n; i++) {
            for (unsigned int j = 0; j <= i; j++) {
                coefficients[offset++] = (uint32_t)src[k * nTerms + j * n - (((j + 1) * j) >> 1) + i];
                count++;
            }
        }
        for (unsigned int i = 0; i < n; i++) {
            coefficients[offset++] = (uint32_t)src[k * nTerms + count + i];
        }
        coefficients[offset++] = (uint32_t)src[k * nTerms + count + n];
    }
}

// Transform coefficientsMatrix into the structure required by exfes.
void TransformDataStructure(int n, int e, std::vector<uint8_t>& coefficientsMatrix, int*** Eqs)
{
    uint64_t offset = 0;
    for (int i = 0; i < e; i++) {
        for (int j = 0; j < Binomial(n, 2); j++)
            Eqs[i][2][j] = (int)coefficientsMatrix[offset + j];
        offset += Binomial(n, 2);
        for (int j = 0; j < n; j++)
            Eqs[i][1][j] = (int)coefficientsMatrix[offset + j];
        offset += n;
        Eqs[i][0][0] = (int)coefficientsMatrix[offset];
        offset += 1;
    }
}

// Define a function set all array elements to zero.
uint64_t** CreateArray(uint64_t maxsol)
{
    uint64_t** SolArray = (uint64_t**)calloc(maxsol, sizeof(uint64_t*)); // Create an array for exfes to store solutions.
    for (uint64_t i = 0; i < maxsol; i++) {
        SolArray[i] = (uint64_t*)calloc(4, sizeof(uint64_t));
    }
    return SolArray;
}

void FreeArray(uint64_t maxsol, uint64_t** SolArray)
{
    for (uint64_t i = 0; i < maxsol; i++) {
        free(SolArray[i]);
    }
    free(SolArray);
}

uint64_t ReverseUint64ByBits(uint64_t num)
{
    unsigned int count = sizeof(num) * 8 - 1;
    uint64_t reverse_num = num;
    num >>= 1;
    while (num) {
        reverse_num <<= 1;
        reverse_num |= num & 1;
        num >>= 1;
        count--;
    }
    reverse_num <<= count;
    return reverse_num;
}

// Define a function to print solutions obtained from exfes.
int ReportSolution(uint64_t maxsol, uint64_t** SolArray, uint256& s)
{
    for (uint64_t i = 0; i < maxsol; i++) {
        for (int j = 3; j >= 0; j--) {
            s = s << 64;
            s ^= SolArray[i][j];
        }
    }
    return 0;
}
