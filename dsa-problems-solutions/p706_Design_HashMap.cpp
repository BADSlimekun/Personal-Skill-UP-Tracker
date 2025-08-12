//https://leetcode.com/problems/design-hashmap/description/ 

#include <vector>
#include <list>
#include <functional> // hash
#include <utility> // pair<>


class MyHashMap {
private: // default for class
    static constexpr double kMaxLoad = 0.75;

    std::vector<std::list<std::pair<int,int>>> buckets;
    size_t sz = 0;

    size_t index(int key) const {
        return static_cast<size_t>(std::hash<int>{}(key)) % buckets.size();
    }

    void rehash(size_t newCap) {
        std::vector<std::list<std::pair<int,int>>> nb(newCap);
        for (auto &bucket : buckets) {
            for (auto &kv : bucket) {
                size_t i = static_cast<size_t>(std::hash<int>{}(kv.first)) % newCap;
                nb[i].push_front(std::move(kv));
            }
        }
        buckets.swap(nb);
    }

public:
    MyHashMap() : buckets(1024) {}
    
    void put(int key, int value) {
        size_t i = index(key);
        for (auto &kv : buckets[i]) {
            if (kv.first == key) { kv.second = value; return; } // update
        }
        buckets[i].push_front({key, value}); // insert
        ++sz;
        if (static_cast<double>(sz) / buckets.size() > kMaxLoad)
            rehash(buckets.size() * 2);
    }
    
    int get(int key) {
        size_t i = index(key);
        for (auto &kv : buckets[i]) {
            if (kv.first == key) return kv.second;
        }
        return -1; // not found
    }
    
    void remove(int key) {
        size_t i = index(key);
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it) {
            if (it->first == key) { buckets[i].erase(it); --sz; return; }
        }
    }
};