import socket
import os

def send_file(connection, file_name):
    file_size = os.path.getsize(file_name)
    connection.send(file_name.encode())
    connection.send(str(file_size).encode())

    with open(file_name, 'rb') as file:
        for data in iter(lambda: file.read(1024), b''):
            connection.sendall(data)

    response = connection.recv(1024)
    print(response.decode())

def start_client():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_host = 'localhost'
    server_port = 12345
    client_socket.connect((server_host, server_port))

    file_name = 'lab1.txt' 
    send_file(client_socket, file_name)

    client_socket.close()

start_client()