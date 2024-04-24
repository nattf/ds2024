#include <iostream>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include "file_transfer.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using filetransfer::FileTransferRequest;
using filetransfer::FileTransferResponse;
using filetransfer::FileTransferService;

class FileTransferServiceImpl final : public FileTransferService::Service {
    Status UploadFile(ServerContext* context, const FileTransferRequest* request,
                      FileTransferResponse* response) override {
        std::string file_name = request->file_name();
        std::string file_path = "path/to/store/" + file_name;

        std::ofstream file(file_path, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to create file: " << file_path << std::endl;
            return Status(grpc::StatusCode::INTERNAL, "Failed to create file");
        }

        const std::string& file_data = request->file_data();
        file.write(file_data.data(), file_data.size());
        file.close();

        std::cout << "File received: " << file_path << std::endl;

        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    FileTransferServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();

    return 0;
}