// Copyright (c) 2018 The Bitcoin developers

#ifndef BITCOIN_SOLUTION_H
#define BITCOIN_SOLUTION_H

#include <inttypes.h>
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include "idxlut.h"
#include "main.h"

typedef int (*solution_callback_t)(void *, uint64_t, uint64_t*);

#define ALGO_AUTO        0
#define ALGO_ENUMERATION 1
#define ALGO_FFT         2

typedef struct {
    int algorithm;
    int word_size;

    int algo_enum_self_tune;
    int algo_auto_degree_bound;
    int algo_enum_use_sse;
    int verbose;
} wrapper_settings_t;

typedef int*  vector_t ;
typedef vector_t* matrix_t;
typedef uint32_t pck_vector_t;

#define SIMD_CHUNK_SIZE 9
#define IA32_CHUNK_SIZE 9
#define enumerated_degree_bound 10
//#define min(x,y) (((x) > (y)) ? (y) : (x))

typedef struct {
    matrix_t quad;
    vector_t lin;
    int con;
} quadratic_form;

typedef struct {
    int n;
    int degree;
    int n_batches;
    pck_vector_t **G;
    idx_lut_t * testing_LUT;

    solution_callback_t callback;
    void *callback_state;
} wrapper_state_t;

typedef quadratic_form* system_t;
#define unlikely(x)     __builtin_expect(!!(x), 0)

/* In principe, this function should be the single entry point to the library
   The input format is explicit, except for the coefficients of the equations.
   coeffs[e][d][m] = coefficient of the m-th monomial of degree d in the e-th equation
   the monomials of each degree are assumed to be in invlex order (i.e. in lex order with reversed variables)
*/
int exhaustive_search_wrapper(const int n,
                                              int n_eqs,
                                              const int degree,
                                              int ***coeffs,
                                              solution_callback_t callback,
                                              void* callback_state,
                                              CBlockIndex* pindexPrev);

void init_settings(wrapper_settings_t *result);
void choose_settings( wrapper_settings_t *s, int n, int n_eqs, int degree);
vector_t init_vector(int n_rows);
pck_vector_t pack(int n, const vector_t v);
uint64_t to_gray(uint64_t i);
uint64_t rdtsc(void);
pck_vector_t packed_eval(LUT_t LUT, int n, int d, pck_vector_t *F, uint64_t i);
void moebius_transform(int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, CBlockIndex* pindexPrev);
void print_vec(__m128i foo);
void exhaustive_ia32_deg_2(LUT_t LUT,
                                       int n,
                                       pck_vector_t F[],
                                       solution_callback_t callback,
                                       void* callback_state,
                                       int verbose,
                                       CBlockIndex* pindexPrev); // autogenerated_sequential_deg_2.c
pck_vector_t packed_eval_deg_2(LUT_t LUT,
                                      int n,
                                      pck_vector_t F[],
                                      uint64_t i); // autogenerated_tester_deg_2.c

#ifdef HAVE_SSE2
void exhaustive_sse2_deg_2_T_2_el_0(LUT_t LUT,
                                                      int n,
                                                      pck_vector_t nonsimd_F[],
                                                      solution_callback_t callback,
                                                      void* callback_state,
                                                      int verbose); // autogenerated_simd_deg_2_T_2_el_0.c
void exhaustive_sse2_deg_2_T_3_el_0(LUT_t LUT,
                                                  int n,
                                                  pck_vector_t nonsimd_F[],
                                                  solution_callback_t callback,
                                                  void* callback_state,
                                                  int verbose); // autogenerated_simd_deg_2_T_3_el_0.c
void exhaustive_sse2_deg_2_T_4_el_0(LUT_t LUT,
                                                  int n,
                                                  pck_vector_t nonsimd_F[],
                                                  solution_callback_t callback,
                                                  void* callback_state,
                                                  int verbose); // autogenerated_simd_deg_2_T_4_el_0.c
#endif

#ifdef HAVE_64_BITS
extern void func_deg_2_T_2_el_0(__m128i *F, uint64_t *F_sp, void *buf, uint64_t *num, uint64_t idx); // autogenerated_asm_deg_2_T_2_el_0.s
extern void func_deg_2_T_3_el_0(__m128i *F, uint64_t *F_sp, void *buf, uint64_t *num, uint64_t idx); // autogenerated_asm_deg_2_T_3_el_0.s
extern void func_deg_2_T_4_el_0(__m128i *F, uint64_t *F_sp, void *buf, uint64_t *num, uint64_t idx); // autogenerated_asm_deg_2_T_4_el_0.s
#endif

void exfes (int m, int n, int e, uint64_t *Mask, uint64_t maxsol, int ***Eqs, uint64_t **SolArray,CBlockIndex* pindexPrev);

#endif
