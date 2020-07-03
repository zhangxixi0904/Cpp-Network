# Cpp-Network



## 编译

```bash
gcc tcp_net_socket.c tcp_net_server.c -o tcp_net_server
gcc tcp_net_socket.c tcp_net_client.c -o tcp_net_client
```

## 运行

```bash
# 服务端
./tcp_net_server 本机ip 端口号
# 客户端
./tcp_net_client 服务端ip 端口号
```

