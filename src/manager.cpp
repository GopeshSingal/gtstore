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
	public:
		ManagerServiceImpl(int virtual_nodes) {
			hash_ring.init(3);
			// for (i in virtual nodes) {
			// 	hash_ring.addNode(i);
			// }
			hash_ring.addNode("1");
			// hash_ring.addNode("2");
			// hash_ring.addNode("3");
			// hash_ring.addNode("4");
			// hash_ring.addNode("5");
		}
		Status GetNodeAddrForKey(ServerContext* context, const KeyRequest* request, NodeInfo* response) {
			// lock_guard<mutex> lock(hr_mutex);
			string node_id = hash_ring.getNodeAddress(request->key());
			if (node_id.empty()) {
				return Status(grpc::StatusCode::NOT_FOUND, "Node not found");
			}
			// ! Check health of node then 
			response->set_node_id(node_id);
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

// void RunServer(int virtual_nodes)
void RunServer()
{

  std::string server_address("0.0.0.0:50051");
  ManagerServiceImpl service = ManagerServiceImpl(1);

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
  RunServer();
  return 0;
}