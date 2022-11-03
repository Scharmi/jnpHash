#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "hash.h"

namespace jnp1 {
    unsigned long &id() {
        static unsigned long id = 0;
        return id;
    }
    struct hf {
        hash_function_t funkcja;
        hf(hash_function_t funkcjaZWejscia) {
            funkcja = funkcjaZWejscia;
        }
        size_t operator () (const std::vector<uint32_t> &wektor) const {
            return funkcja(wektor.data(), wektor.size());
        }
    }
    std::unordered_map<unsigned long, unordered_set<std::vector<uint32_t>, hf> &mapa() {
        static std::unordered_map<unsigned long, unordered_set<std::vector<uint32_t>, hf> mapa;
        return mapa;
    }
    unsigned long hash_create(jnp1::hash_function_t x) {
        std::unordered_set<std::vector<uint32_t>, hf> zbiur(0, hf(x));
        mapa().insert(id, zbiur);
        id()++;
        return id();
    }
    void hash_delete(unsigned long) {

    }
    size_t hash_size(unsigned long) {

    }
    bool hash_insert(unsigned long, uint64_t const *, size_t) {

    }
    bool hash_remove(unsigned long, uint64_t const *, size_t) {

    }
    void hash_clear(unsigned long) {

    }
    bool hash_test(unsigned long, uint64_t const *, size_t) {

    }
}

int main() {
    std::cout << "Hello World\n";
}