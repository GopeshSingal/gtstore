#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


#include <grpcpp/grpcpp.h>
#include "demo.grpc.pb.h"
#include "demo.pb.h"
#include "grpcpp/channel.h"

using demo::KeyRequest;
using demo::ValueResponse;
using demo::ManagerService;
using demo::NodeInfo;
using demo::Ack;
using demo::StorageNode;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

std::string target_str = "localhost:50051";
std::string fail_resp = "Failed";
std::string bad_rpc = "RPC failed";
class DemoClient
{
public:
	DemoClient(std::shared_ptr<Channel> channel) : stub_(demo::ManagerService::NewStub(channel)) {}

	std::vector<std::string> getNode(const std::string &user)
	{
		KeyRequest request;
		request.set_key(user);
		NodeInfo reply;
		ClientContext context;

		Status status = stub_->GetNodeForKey(&context, request, &reply);
		std::vector<std::string> x;
		if (status.ok()) {
			for (const auto &id : reply.node_ids()) {
				x.push_back(id);
			}
			// TODO Write service method to retrieve data from node :D
			return x;
		} else {
			std::cout << status.error_code() << ": " << status.error_message()
					<< std::endl;
			std::vector<std::string> x;
			return x;
		}
	}

private:
	std::unique_ptr<demo::ManagerService::Stub> stub_;
};

class NodeClient
{
public:
	NodeClient(std::shared_ptr<Channel> channel) : stub_(StorageNode::NewStub(channel)) {}

	std::string retrieve(const std::string &key)
	{
		demo::KeyRequest request;
		request.set_key(key);
		demo::ValueResponse resp;
		ClientContext context;
		Status status = stub_->Get(&context, request, &resp);
		
		if (status.ok()) {
			if (resp.found()) {
				return resp.value();
			} else {
				return fail_resp;
			}

		} else {
			std::cout << status.error_code() << ": " << status.error_message()
					<< std::endl;
			return bad_rpc;
		}
	}

	std::string store(const std::string &key, const std::string &value)
	{
		demo::KeyValuePair request;
		request.set_key(key);
		request.set_value(value);
		Ack reply;
		ClientContext context;
		
		Status status = stub_->Put(&context, request, &reply);

		if (status.ok()) {
			std::string resp = reply.message() + " " + grpc::to_string(reply.success());
			return reply.message();
		} else {
			std::cout << status.error_code() << ": " << status.error_message()
					<< std::endl;
			return bad_rpc;
		}
	}

private:
	std::unique_ptr<StorageNode::Stub> stub_;
};

std::unique_ptr<DemoClient> manager;

void get(std::string &key) {
	std::vector<std::string> addrs = manager->getNode(key);
	std::string addr = addrs[0];
	std::cout << "Get received: " << addr << std::endl;
	if (addr != bad_rpc) {
		NodeClient node = NodeClient(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials()));
		std::string output = node.retrieve(key);
		if (output != bad_rpc && output != fail_resp) {
			std::cout << "Value: " << output << std::endl;
		} else if (output == fail_resp) {
			std::cout << "Key does not exist. Value could not be found\n";
		} else {
			std::cout << "GET: Node RPC Failed. Node may not exist\n";
		}
	}
}

void put(std::string &key, std::string &value) {
	std::vector<std::string> addrs = manager->getNode(key);
	if (!addrs.empty()) {
		for (const auto& port : addrs) {
			NodeClient node = NodeClient(grpc::CreateChannel(port, grpc::InsecureChannelCredentials()));
			std::string output = node.store(key, value);
			std::cout << port.back() << std::endl;
		}
	} else {
		std::cout << "PUT: Node RPC Failed. Node may not exist\n";
	}
}

void init() {
	manager = std::make_unique<DemoClient>(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
}

int main(int argc, char **argv)
{
	std::string operation;
    std::string key = "";
    std::string value = "";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--put" && i + 1 < argc) {
            operation = "put";
            key = argv[++i];
        } else if (arg == "--val" && i + 1 < argc) {
            value = argv[++i];
        } else if (arg == "--get" && i + 1 < argc) {
            operation = "get";
            key = argv[++i];
        } else {
            std::cerr << "Unknown or incomplete argument: " << arg << std::endl;
            return 1;
        }
    }

	init();

    if (operation == "put" && !key.empty() && !value.empty()) {
        put(key, value);
    } else if (operation == "get" && !key.empty()) {
        get(key);
    }
	return 0;
}