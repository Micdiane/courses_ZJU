import socket


def main():
    # 服务器地址和端口
    server_host = 'example.com'  # 请替换为实际的服务器地址
    server_port = 80  # 请替换为实际的服务器端口

    # 创建一个TCP套接字
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 连接到服务器
    client_socket.connect((server_host, server_port))

    # 获取客户端的本地地址信息
    client_host, client_port = client_socket.getsockname()

    print(f"Connected to server at {server_host}:{server_port}")
    print(f"Client's local address is {client_host}:{client_port}")

    # 关闭套接字
    client_socket.close()
    
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 连接到服务器
    client_socket.connect((server_host, server_port))

    # 获取客户端的本地地址信息
    client_host, client_port = client_socket.getsockname()

    print(f"Connected to server at {server_host}:{server_port}")
    print(f"Client's local address is {client_host}:{client_port}")

    # 关闭套接字
    client_socket.close()
    
    

if __name__ == "__main__":
    main()
当服务端在调用listen和accept之间设置了调试断点并暂停在此断点时，此时客户端调用connect后不会立即连接成功。在TCP套接字通信中，accept是服务端用于接受客户端连接请求的函数，当服务端执行到accept时，它会一直等待，直到一个客户端连接请求到达。

如果服务端暂停在accept之间，客户端的connect调用会在尝试连接到服务端时被阻塞，直到服务端接受连接请求或发生连接超时。只有当服务端调用accept并接受了客户端的连接请求时，客户端的connect才会成功完成连接。如果服务端一直保持在accept之前的断点状态，那么客户端的连接请求将一直处于等待状态。