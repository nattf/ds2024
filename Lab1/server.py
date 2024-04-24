import socket

def receive_file(connection):
    file_name = connection.recv(1024).decode()
    file_size = int(connection.recv(1024).decode())

    with open(file_name, 'wb') as file:
        while file_size > 0:
            data = connection.recv(1024)
            file.write(data)
            file_size -= len(data)

    connection.send(b'File received successfully')
    connection.close()

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_host = 'localhost'
    server_port = 12345
    server_socket.bind((server_host, server_port))
    server_socket.listen(1)

    print(f'Server listening on {server_host}:{server_port}')

    while True:
        connection, address = server_socket.accept()
        print(f'Connection established with {address[0]}:{address[1]}')
        receive_file(connection)

start_server()