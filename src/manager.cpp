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

using demo::ManagerService;
using demo::NodeInfo;
using demo::Ack;
using demo::KeyRequest;
using demo::NodeInfo;
using demo::KeyValuePair;

class ManagerServiceImpl final : public ManagerService::Service {
	private:
		ConsistentHashing hash_ring;
		mutex hr_mutex;
		int k;
		int n;
	public:
		ManagerServiceImpl(int storage_nodes, int replica_factor) {
			n = storage_nodes;
			hash_ring.init(storage_nodes);
			for (int i = 1; i < n; i++) {
				hash_ring.addNode(to_string(i));
			}
			k = replica_factor;
		}
		Status GetNodeAddrForKey(ServerContext* context, const KeyRequest* request, NodeInfo* response) {
			// lock_guard<mutex> lock(hr_mutex);
			vector<string> node_ids = hash_ring.getNodeAddress(request->key(), k);
			// ! Check health of node then 
			for (const auto&id : node_ids) {
				response->add_node_ids(id);
			}
			return Status(grpc::StatusCode::OK, "Here's a Node");
		}
		Status RemoveNode(ServerContext* context, const NodeInfo* request, Ack* response) {
			// lock_guard<mutex> lock(hr_mutex);
			string node_id = request->node_id();
			hash_ring.removeNode(node_id);
			response->set_success(true);
			response->set_message("Node removed successfully");
			// cout << "Node removed: " << node_id << endl;
			return Status::OK;
			// ! Probably add some load balancing to make it all hunky dory
		}

};

void RunServer(int storage_nodes, int replica_factor)
{

  std::string server_address("0.0.0.0:50051");
  ManagerServiceImpl service = ManagerServiceImpl(storage_nodes, replica_factor);

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Manager listening on " << server_address << std::endl;

  server->Wait();
//   virtual_nodes += 0;
}

int main(int argc, char **argv)
{
	int storage_nodes = 5;
	int replica_factor = 3;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--nodes" && i + 1 < argc) {
            storage_nodes = std::stoi(argv[++i]);
			storage_nodes += 1;
        } else if (arg == "--rep" && i + 1 < argc) {
            replica_factor = std::stoi(argv[++i]);
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1; 
        }
    }
    RunServer(storage_nodes, replica_factor);
    return 0;
}