#include <iostream>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include "file_transfer.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using filetransfer::FileTransferRequest;
using filetransfer::FileTransferResponse;
using filetransfer::FileTransferService;

class FileTransferClient {
public:
    FileTransferClient(std::shared_ptr<Channel> channel)
        : stub_(FileTransferService::NewStub(channel)) {
    }

    bool UploadFile(const std::string& file_name, const std::string& file_path) {
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (!file) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return false;
        }

        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(file_size);
        if (!file.read(buffer.data(), file_size)) {
            std::cerr << "Failed to read file: " << file_path << std::endl;
            return false;
        }

        file.close();

        FileTransferRequest request;
        request.set_file_name(file_name);
        request.set_file_data(buffer.data(), buffer.size());

        FileTransferResponse response;
        ClientContext context;

        Status status = stub_->UploadFile(&context, request, &response);

        if (status.ok()) {
            std::cout << "File uploaded successfully." << std::endl;
            return true;
        } else {
            std::cerr << "RPC failed: " << status.error_message() << std::endl;
            return false;
        }
    }

private:
    std::unique_ptr<FileTransferService::Stub> stub_;
};

int main() {
    std::string server_address("localhost:50051");
    FileTransferClient client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    std::string file_name = "example.txt";
    std::string file_path = "path/to/example.txt";

    if (!client.UploadFile(file_name, file_path)) {
        return 1;
    }

    return 0;
}