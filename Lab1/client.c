#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFFER_SIZE 1024

void send_file(SOCKET server_socket, const char* file_name) {
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Send file name
    send(server_socket, file_name, strlen(file_name), 0);

    // Send file size
    send(server_socket, (const char*)&file_size, sizeof(file_size), 0);

    // Send file data
    char buffer[BUFFER_SIZE];
    while (!feof(file)) {
        int bytes_read = fread(buffer, 1, sizeof(buffer), file);
        send(server_socket, buffer, bytes_read, 0);
    }

    // Close the file
    fclose(file);

    // Receive confirmation message
    char confirmation[BUFFER_SIZE];
    recv(server_socket, confirmation, sizeof(confirmation), 0);
    printf("Server response: %s\n", confirmation);

    // Close the server socket
    closesocket(server_socket);
    WSACleanup();
}

int main() {
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server_address;
    const char* server_ip = "127.0.0.1";
    int server_port = 12345;
    const char* file_name = "lab1.txt";  
    
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

    // Set server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // Connect to the server
    if (connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("Error connecting to server");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Send the file
    send_file(server_socket, file_name);

    // Close the server socket
    closesocket(server_socket);
    WSACleanup();

    return 0;
}