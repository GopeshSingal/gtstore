#ifndef GTSTORE
#define GTSTORE

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <map>
#include <functional>

#define MAX_KEY_BYTE_PER_REQUEST 20
#define MAX_VALUE_BYTE_PER_REQUEST 1000

using namespace std;

typedef vector<string> val_t;

class GTStoreClient {
		private:
				int client_id;
				val_t value;
		public:
				void init(int id);
				void finalize();
				val_t get(string key);
				bool put(string key, val_t value);
};

class GTStoreManager {
		public:
				void init(int num_nodes);
};

class GTStoreStorage {
		public:
static void RunStorageNode(size_t port);
};

class ConsistentHashing {
		public:
			void init(int virtualNodes);
			size_t addNode(const std::string &node);
			void removeNode(const std::string &node);
			vector<string> getNodeAddress(const std::string &key, int k);
			size_t hash_fn(const std::string &key);
			int virtualNodes_;
			std::map<size_t, std::string> ring_;
};

size_t ConsistentHashing::hash_fn(const std::string &key) {
    std::hash<std::string> hasher;
    return hasher(key);
}

void ConsistentHashing::init(int virtualNodes) {
    // cout << "initing hashring\n";
    virtualNodes_ = virtualNodes;
}

size_t ConsistentHashing::addNode(const std::string &node) {
    // for (int i = 0; i < virtualNodes_; ++i) {
        std::string virtualNodeName = node + "-" + std::to_string(0);
        size_t hash = hash_fn(virtualNodeName);
        ring_[hash] = node;
        return hash;
    // }
}

void ConsistentHashing::removeNode(const std::string &node) {
    // for (int i = 0; i < virtualNodes_; ++i) {
        std::string virtualNodeName = node + "-" + std::to_string(0);
        size_t hash = hash_fn(virtualNodeName);
        ring_.erase(hash);
        for (const auto& pair : ring_) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        }
    // }
}

vector<string> ConsistentHashing::getNodeAddress(const std::string &key, int k) {
    if (ring_.empty()) {
        vector<string> empty;
        return empty;
    }

    size_t hash = hash_fn(key);
    auto it = ring_.lower_bound(hash);

    if (it == ring_.end()) {
        it = ring_.begin();
    }

    vector<string> res;
    int curr = stoi(it->second);
    res.push_back("0.0.0.0:" + it->second);

    for (int i = 0; i < k; i++) {
        curr = curr + 1;
        if (curr >= virtualNodes_) {
            curr = 1;
        }
        res.push_back(("0.0.0.0:" + to_string(curr)));
    }

    return res;
}


#endif

