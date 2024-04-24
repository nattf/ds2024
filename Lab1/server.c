#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFFER_SIZE 1024

void receive_file(SOCKET client_socket) {
    char file_name[BUFFER_SIZE];
    int file_size;

    // Receive file name
    recv(client_socket, file_name, sizeof(file_name), 0);

    // Receive file size
    recv(client_socket, (char*)&file_size, sizeof(file_size), 0);

    // Open the file
    FILE* file = fopen(file_name, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Receive and write file data
    int remaining_bytes = file_size;
    char buffer[BUFFER_SIZE];
    while (remaining_bytes > 0) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        fwrite(buffer, 1, bytes_received, file);
        remaining_bytes -= bytes_received;
    }

    // Close the file
    fclose(file);

    // Send confirmation message
    const char* message = "File received successfully";
    send(client_socket, message, strlen(message), 0);

    // Close the client socket
    closesocket(client_socket);
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int server_port = 12345;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("Failed to initialize Winsock");
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Error creating socket");
        WSACleanup();
        return 1;
    }

    // Bind socket to a specific port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("Error binding socket");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    listen(server_socket, 1);
    printf("Server listening on port %d\n", server_port);

    // Accept incoming connection
    int client_address_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
    if (client_socket == INVALID_SOCKET) {
        perror("Error accepting connection");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Connection established with %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Receive the file
    receive_file(client_socket);

    // Close the server socket
    closesocket(server_socket);
    WSACleanup();

    return 0;
}