#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstddef>
#include "hash.h"
#ifdef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif
#define map_find ((mapa().find(id))->second)
namespace debugInfo {
    void printArray(uint64_t const * arr, size_t size) {
        if(arr != NULL) {
            std::cerr << "\"";
            for(size_t i = 0; i < size; i++) {
                std::cerr << arr[i];
                if(i + 1 != size) std::cerr << " ";
            }
            std::cerr << "\"";
        }
        else {
            std::cerr << "NULL";
        }
    }
    void functionCall(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        std::cerr << name << ":" << "(" << id << ", ";
        printArray(seq, size);
        std::cerr << ", " << size << ")\n";
    }
    void functionCall(const std::string &name, unsigned long id) {
        std::cerr << name << ":" << "(" << id << ")\n";
    }
    void idNonexist(const std::string &name, unsigned long id) {
        std::cerr << name << ": " << "hash table #" << id << " does not exist\n";
    }
    void tableWasEmpty(const std::string &name, unsigned long id) {
        std::cerr << name << ": " << "hash table #" << id << " was empty\n";
    }
    void tableSize(const std::string &name, unsigned long id, size_t size) {
        std::cerr << name << ": " << "hash table #" << id << " contains " << size << " element(s)\n";
    }
    void hashCreated(unsigned long id){
        std::cerr << "hash_create: hash table #" << id << " created\n";
    }
    void hashCreateStart(jnp1::hash_function_t x){
        std::cerr << "hash_create(" << (void const *) x << ")\n";
    }
};
namespace jnp1 {
    unsigned long &id_global() {
        static unsigned long id_global = 0;
        return id_global;
    };
    struct hf {
        hash_function_t funkcja;
        hf(hash_function_t funkcjaZWejscia) {
            funkcja = funkcjaZWejscia;
        }
        size_t operator () (const std::vector<uint64_t> &wektor) const {
            return funkcja(wektor.data(), wektor.size());
        }
    };
    std::unordered_map< unsigned long, std::unordered_set< std::vector<uint64_t>, hf >  > &mapa() {
        static std::unordered_map<unsigned long, std::unordered_set<std::vector<uint64_t>, hf> > mapa;
        return mapa;
    };
    std::vector<uint64_t> arr_to_vec(uint64_t const * seq, size_t size){
        std::vector<uint64_t> vec;
        for(size_t i = 0 ; i < size ; i++)
            vec.push_back(seq[i]);
        return vec;
    }
    unsigned long hash_create(jnp1::hash_function_t x) {
        if(debug){
            debugInfo::hashCreateStart(x);
        }
        std::unordered_set<std::vector<uint64_t>, hf> newSet(0, hf(x));
        mapa().insert({id_global(), newSet});
        id_global()++;
        if(debug){
            debugInfo::hashCreated(id_global() - 1);
        }
        return id_global() - 1;
    }
    void hash_delete(unsigned long id) {
        if(debug) {
            debugInfo::functionCall("hash_delete", id);
        }
        bool success = mapa().erase(id);
        if(!success && debug) {
            debugInfo::idNonexist("hash_delete", id);
        }
    }
    size_t hash_size(unsigned long id) {
        if(debug) {
            debugInfo::functionCall("hash_size", id);
        }
        if(mapa().find(id) == mapa().end()){
            debugInfo::idNonexist("hash_size", id);
            return 0;
        }
        else{
            size_t size = map_find.size();
            debugInfo::tableSize("hash_size", id, size);
            return map_find.size();
        }
    }
    bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
        if(debug) {
            debugInfo::functionCall("hash_insert", id, seq, size);
        }
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if(hash_test(id, seq, size) == true){
                return false;
            }
            else{
                map_find.insert(arr_to_vec(seq, size));
                return true;
            }
        }
    }
    bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
        if(debug) {
            debugInfo::functionCall("hash_remove", id, seq, size);
        }
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if(hash_test(id, seq, size) == false){
                return false;
            }
            else{
                map_find.erase(arr_to_vec(seq, size));
                return true;
            }
            return true;
        }
    }
    void hash_clear(unsigned long id) {
        debugInfo::functionCall("hash_clear", id);
        if(mapa().find(id) != mapa().end()){
            if(map_find.size() == 0) debugInfo::tableWasEmpty("hash_clear", id);
            map_find.clear();
        }
        else {
            debugInfo::idNonexist("hash_clear", id);
        }
    }
    bool hash_test_no_debug(unsigned long id, uint64_t const * seq, size_t size) {
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if(map_find.find(arr_to_vec(seq, size))  == map_find.end())
                return false;
            else
                return true;
        }
    }
    bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
        if(debug) {
            debugInfo::functionCall("hash_test", id, seq, size);
        }
        return hash_test_no_debug(id, seq, size);
    }
}
