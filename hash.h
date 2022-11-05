#ifndef HASH_H
#define HASH_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus 
#include <iostream>
namespace jnp1 {
extern "C" {
#endif
typedef uint64_t (*hash_function_t) (uint64_t const*, size_t);
unsigned long hash_create(hash_function_t);
void hash_delete(unsigned long id);
size_t hash_size(unsigned long id);
bool hash_insert(unsigned long id, uint64_t const * seq, size_t size);
bool hash_remove(unsigned long id, uint64_t const * seq, size_t size);
void hash_clear(unsigned long id);
bool hash_test(unsigned long id, uint64_t const * seq, size_t size);
#ifdef __cplusplus 
}
}
#endif
#endif