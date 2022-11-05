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
#define map_find ((mapa().find(id))->second) //wskazuje set od id
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
    void printNameHashTable(const std::string &name, unsigned long id) {
    std::cerr << name << ": " << "hash table #" << id;
    }
    void functionCall(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        std::cerr << name << "(" << id << ", ";
        printArray(seq, size);
        std::cerr << ", " << size << ")\n";
    }
    void functionCall(const std::string &name, unsigned long id) {
        std::cerr << name << "(" << id << ")\n";
    }
    void idNonexist(const std::string &name, unsigned long id) {
        printNameHashTable(name, id);
        std::cerr << " does not exist\n";
    }
    void tableWasEmpty(const std::string &name, unsigned long id) {
        printNameHashTable(name, id);
        std::cerr << " was empty\n";
    }
    void tableSize(const std::string &name, unsigned long id, size_t size) {
        printNameHashTable(name, id);
        std::cerr << " contains " << size << " element(s)\n";
    }
    void hashCreated(unsigned long id){
        std::cerr << "hash_create: hash table #" << id << " created\n";
    }
    void hashCreateStart(jnp1::hash_function_t x){
        std::cerr << "hash_create(" << (void const *) x << ")\n";
    }
    void invalidPointer(const std::string &name) {
        std::cerr << name << ": invalid pointer (NULL)\n";
    }
    void printTableState(const std::string &name, unsigned long id, uint64_t const * seq, size_t size, const std::string &property) {
        printNameHashTable(name, id);
        std::cerr << ", sequence ";
        printArray(seq, size);
        std::cerr << property << "\n";
    }
    void tableInserted(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        printTableState(name, id, seq, size, " inserted");
    }
    void tableRemoved(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        printTableState(name, id, seq, size, " removed");
    }
    void tableWasPresent(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        printTableState(name, id, seq, size, " was present");
    }
    void tableWasNotPresent(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        printTableState(name, id, seq, size, " was not present");
    }
    void tableIsPresent(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        printTableState(name, id, seq, size, " is present");
    }
    void tableIsNotPresent(const std::string &name, unsigned long id, uint64_t const * seq, size_t size) {
        printTableState(name, id, seq, size, " is not present");
    }
    void hashDeleted(const std::string &name, unsigned long id) {
        printNameHashTable(name, id);
        std::cerr << " deleted\n";
    }
    void hashCleared(const std::string &name, unsigned long id) {
        printNameHashTable(name, id);
        std::cerr << " cleared\n";
    }
    void invalidSize(const std::string &name, size_t size) {
        std::cerr << name << ": invalid size (" << size << ")\n";
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
    // //Nieprzetestowane jeszcze
    unsigned long hash_create(jnp1::hash_function_t x) {
        if(debug){
            debugInfo::hashCreateStart(x);
        }
        std::unordered_set<std::vector<uint64_t>, hf> zbiur(0, hf(x));
        std::unordered_set<int> secior(0);
        mapa().insert({id_global(), zbiur});
        id_global()++;
        if(debug){
            debugInfo::hashCreated(id_global() - 1);
        }
        return id_global() - 1;
        //niescislosc byla bo mapa na np. set ma id 0, a bylo zwracane 1
    }
    void hash_delete(unsigned long id) {
        if(debug) {
            debugInfo::functionCall("hash_delete", id);
        }
        bool success = mapa().erase(id);
        if(debug) {
            if(success) {
                debugInfo::hashDeleted("hash_delete", id);
            }
            else {
                debugInfo::idNonexist("hash_delete", id);
            }
        }

    }
    size_t hash_size(unsigned long id) {
        if(debug) {
            debugInfo::functionCall("hash_size", id);
        }
        if(mapa().find(id) == mapa().end()){
            //nie ma klucza
            if(debug)
                debugInfo::idNonexist("hash_size", id);
            return 0;
        }
        else{
            size_t size = map_find.size();
            if(debug)
                debugInfo::tableSize("hash_size", id, size);
            return map_find.size();
        }
    }
    bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
        if(debug) {
            debugInfo::functionCall("hash_insert", id, seq, size);
            if(seq == NULL) debugInfo::invalidPointer("hash_insert");
            if(size == 0) debugInfo::invalidSize("hash_insert", 0);
            if(mapa().find(id) == mapa().end()) debugInfo::idNonexist("hash_insert", id);
        }
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if(hash_test_no_debug(id, seq, size) == true){
                debugInfo::tableWasPresent("hash_insert", id, seq, size);
                return false;
            }
            else{
                map_find.insert(arr_to_vec(seq, size));
                debugInfo::tableInserted("hash_insert", id, seq, size);
                return true;
            }
            //error tylko gdy cos nie powiedzie lub set ma juz ten hash(ciag)
        }
    }
    bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
        if(debug) {
            debugInfo::functionCall("hash_remove", id, seq, size);
            if(seq == NULL) debugInfo::invalidPointer("hash_remove");
            if(size == 0) debugInfo::invalidSize("hash_remove", 0);
            if(mapa().find(id) == mapa().end()) debugInfo::idNonexist("hash_remove", id);
        }
        if(seq == NULL || mapa().find(id) == mapa().end() || size == 0){
            return false;
        }
        else{
            if(hash_test_no_debug(id, seq, size) == false){
                if(debug)
                    debugInfo::tableWasNotPresent("hash_remove", id, seq, size);
                return false;
            }
            else{
                map_find.erase(arr_to_vec(seq, size));
                if(debug)
                    debugInfo::tableRemoved("hash_remove", id, seq, size);
                return true;
            }
            //error tylko gdy cos nie powiedzie lub set nie ma tego hash(ciag)
            return true;
        }
    }
    void hash_clear(unsigned long id) {
        debugInfo::functionCall("hash_clear", id);
        if(mapa().find(id) != mapa().end()){
            if(map_find.size() == 0) debugInfo::tableWasEmpty("hash_clear", id);
            else debugInfo::hashCleared("hash_clear", id);
            map_find.clear();
        }
        else {
            debugInfo::idNonexist("hash_clear", id);
        }
    }
    bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
        if(debug) {
            debugInfo::functionCall("hash_test", id, seq, size);
            if(seq == NULL) debugInfo::invalidPointer("hash_test");
            if(size == 0) debugInfo::invalidSize("hash_test", 0);
            if((seq != NULL) && (size != 0))
                if(mapa().find(id) == mapa().end()) debugInfo::idNonexist("hash_test", id);
        }
        if(hash_test_no_debug(id, seq, size)) {
            if(debug) {
                if((seq != NULL) && (size != 0) && (mapa().find(id) != mapa().end()))
                debugInfo::tableIsPresent("hash_test", id, seq, size);
            }
            return true;
        }
        else {
            if(debug) {
                if((seq != NULL) && (size != 0) && (mapa().find(id) != mapa().end()))
                debugInfo::tableIsNotPresent("hash_test", id, seq, size);
            }
            return false;
        }
    }
}
