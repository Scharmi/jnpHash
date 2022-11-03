#ifndef HASH_H
#define HASH_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <iostream>

#ifdef __cplusplus 
namespace jnp1 {
extern "C" {
#endif
typedef uint64_t (*hash_function_t) (uint64_t const*, size_t);
unsigned long hash_create(hash_function_t);
void hash_delete(unsigned long);
size_t hash_size(unsigned long);
bool hash_insert(unsigned long, uint64_t const *, size_t);
bool hash_remove(unsigned long, uint64_t const *, size_t);
void hash_clear(unsigned long);
bool hash_test(unsigned long, uint64_t const *, size_t);
#ifdef __cplusplus 
}
}
#endif
#endif