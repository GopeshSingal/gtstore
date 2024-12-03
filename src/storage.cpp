#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "grpcpp/security/server_credentials.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "gtstore.hpp"

#include "demo.grpc.pb.h"
#include "demo.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using demo::StorageNode;
using demo::KeyRequest;
using demo::ValueResponse;
using demo::KeyValuePair;
using demo::Ack;

class StorageNodeImpl final : public StorageNode::Service {
	unordered_map<string, string> kv_store;
	mutex kv_mutex;
	Status Put(ServerContext* context, const KeyValuePair* request, Ack* response) override {
		lock_guard<mutex> lock(kv_mutex);
		kv_store[request->key()] = request->value();
		response->set_success(true);
		response->set_message("Key-value pair stored successfully!");
		return Status::OK;
	}
	Status Get(ServerContext* context, const KeyRequest* request, ValueResponse* response) override {
		lock_guard<mutex> lock(kv_mutex);
		auto it = kv_store.find(request->key());
		if (it != kv_store.end()) {
			response->set_found(true);
			response->set_value(it->second);
		} else {
			response->set_found(false);
		}
		return Status::OK;
	}
};

void RunStorageNode(int port) {
	string server_address("0.0.0.0:" + to_string(port));
	StorageNodeImpl node;
	ServerBuilder builder;
	
	grpc::EnableDefaultHealthCheckService(true);
  	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	builder.RegisterService(&node);
	unique_ptr<Server> server(builder.BuildAndStart());
	cout << "Server listening on " << server_address << endl;

	server->Wait();
}

int main(int argc, char **argv) {
	RunStorageNode(5);
	return 0;
}