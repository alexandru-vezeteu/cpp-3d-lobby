//
// Created by alex on 19/05/25.
//

#ifndef THREADSAFEUNORDEREDMAP_HPP
#define THREADSAFEUNORDEREDMAP_HPP

#include <unordered_map>
#include <shared_mutex>
#include <memory>
#include <functional>
#include <mutex>

template<class K, class V>
class ThreadSafeUnorderedMap {
private:
    std::unordered_map<K,std::shared_ptr<V>> map;
    std::shared_mutex mutex;
public:

    ThreadSafeUnorderedMap(const ThreadSafeUnorderedMap& other) = delete;
    ThreadSafeUnorderedMap(const ThreadSafeUnorderedMap&& other) = delete;
    ThreadSafeUnorderedMap& operator=(const ThreadSafeUnorderedMap& other) = delete;
    ThreadSafeUnorderedMap& operator=(const ThreadSafeUnorderedMap&& other) = delete;

    ThreadSafeUnorderedMap():map{}, mutex{} {}

    bool empty() {
        std::lock_guard l{mutex};
        return map.empty();
    }



    int size() {
        std::lock_guard l{mutex};
        return map.size();
    }

    const ThreadSafeUnorderedMap& for_each(std::function<void(const K&, const std::shared_ptr<V>&)> function )
    {
        std::shared_lock l{mutex};
        for (const auto& kv : map)
        {
            function(kv.first, kv.second);
        }
        return *this;
    }

    ThreadSafeUnorderedMap& apply(std::function<void(K&, V&)> function )
    {
        std::unique_lock l{mutex};
        for (auto& kv:map) {
            function(kv.first, kv.second);
        }
        return *this;
    }

    void set(const K& key, const V value)
    {
        std::unique_lock l{mutex};
        map[key] = std::make_shared<V>(std::move(value));
    }
    template<class... Args>
    void emplace(const K& key, Args... args)
    {
        std::unique_lock l{mutex};
        map[key] = std::make_shared<V>(std::forward<Args>(args)...);
    }

    void emplace(const K& key) {
        std::unique_lock l{mutex};
        map[key] = std::make_shared<V>();
    }
    std::shared_ptr<V> get(const K& key)
    {
        std::shared_lock l{mutex};
        return map[key];
    }

    void remove(const K& key)
    {
        std::unique_lock l{mutex};
        map.erase(key);
    }

    bool contains(const K& key)
    {
        std::shared_lock l{mutex};
        return map.contains(key);
    }


    std::shared_ptr<V> operator[](const K& key)
    {
        std::unique_lock l{mutex};
        if (map[key] == nullptr) {
            map[key] = std::make_shared<V>();
        }
        return map[key];

    }

    std::vector<K> keys()
    {
        std::shared_lock l{mutex};
        std::vector<K> keys;
        for (const auto& kv : map) {
            keys.push_back(kv.first);
        }
        return keys;
    }

    std::vector<std::shared_ptr<V>> values()
    {
        std::shared_lock l{mutex};
        std::vector<std::shared_ptr<V>> values;
        for (const auto& kv : map) {
            values.push_back(kv.second);
        }
        return values;
    }

    std::vector<K> keys(std::function<bool(const K&, const std::shared_ptr<V>&)> predicate)
    {
        std::shared_lock l{mutex};
        std::vector<K> keys;
        for (const auto& kv : map) {
            if (predicate(kv.first, kv.second)) {
                keys.push_back(kv.first);
            }
        }
        return keys;
    }


    void erase(const K& key)
    {
        std::unique_lock l{mutex};
        map.erase(key);
    }







};
#endif //THREADSAFEUNORDEREDMAP_HPP
