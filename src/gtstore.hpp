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
				void init(int id); // Add id to set up distinct ports for gRPC
};

class ConsistentHashing {
		public:
			void init(int virtualNodes);
			void addNode(const std::string &node);
			void removeNode(const std::string &node);
			std::string getNode(const std::string &key);
			size_t hash_fn(const std::string &key);

		private:
			int virtualNodes_;
			std::map<size_t, std::string> ring_;
};

size_t ConsistentHashing::hash_fn(const std::string &key) {
    std::hash<std::string> hasher;
    return hasher(key);
}

void ConsistentHashing::init(int virtualNodes) {
    virtualNodes_ = virtualNodes;
}

void ConsistentHashing::addNode(const std::string &node) {
    for (int i = 0; i < virtualNodes_; ++i) {
        std::string virtualNodeName = node + "-" + std::to_string(i);
        size_t hash = hash_fn(virtualNodeName);
        ring_[hash] = node;
    }
}

void ConsistentHashing::removeNode(const std::string &node) {
    for (int i = 0; i < virtualNodes_; ++i) {
        std::string virtualNodeName = node + "-" + std::to_string(i);
        size_t hash = hash_fn(virtualNodeName);
        ring_.erase(hash);
    }
}

std::string ConsistentHashing::getNode(const std::string &key) {
    if (ring_.empty()) {
        return "";
    }

    size_t hash = hash_fn(key);
    auto it = ring_.lower_bound(hash);

    if (it == ring_.end()) {
        it = ring_.begin();
    }

    return it->second;
}


#endif

