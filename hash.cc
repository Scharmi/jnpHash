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
    //Nieprzetestowane jeszcze
    unsigned long hash_create(jnp1::hash_function_t x) {
        std::unordered_set<std::vector<uint64_t>, hf> zbiur(0, hf(x));
        std::unordered_set<int> secior(0);
        mapa().insert({id(), zbiur});
        id()++;
        return id() - 1;
        //niescislosc byla bo mapa na np. set ma id 0, a bylo zwracane 1
    }
    void hash_delete(unsigned long id) {
        mapa().erase(id);
    }
    size_t hash_size(unsigned long id) {
        if(mapa().find(id) == mapa().end()){
            //nie ma klucza
            return 0;
        }
        else{
            return (mapa())[id].size();
        }
    }
    bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if(hash_test(id, seq, size) == true){
                return false;
            }
            else{
                (mapa())[id].insert(arr_to_vec(seq, size));
                return true;
            }
            //error tylko gdy cos nie powiedzie lub set ma juz ten hash(ciag)
        }
    }
    bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if(hash_test(id, seq, size) == false){
                return false;
            }
            else{
                (mapa())[id].erase(arr_to_vec(seq, size));
                return true;
            }
            //error tylko gdy cos nie powiedzie lub set nie ma tego hash(ciag)
            return true;
        }
    }
    void hash_clear(unsigned long id) {
        if(mapa().find(id) != mapa().end()){
            (mapa())[id].clear();
        }
    }
    bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if((mapa()[id]).find(arr_to_vec(seq, size)) == (mapa()[id]).end())
                return false;
            else
                return true;
        }
    }
}


int main() {
    std::cout << "Hello World\n";
}