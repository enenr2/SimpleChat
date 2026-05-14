# SimpleChat 
 
基于 Qt 和 C++ 的聊天室项目，包含服务端和客户端。 
 
## 项目结构 
 
``` 
SimpleChat/ 
 ├── server/      # 服务端（ChatServer, GateSaver, StatusServer, VerifyServer） 
 └── client/      # Qt 客户端 
``` 
 
## 服务端 
 
- **ChatServer** - 主聊天服务器（C++, gRPC, MySQL, Redis） 
- **GateSaver** - 网关服务 
- **StatusServer** - 状态服务 
- **VerifyServer** - 验证服务（Node.js） 
 
## 客户端 
 
Qt C++ 桌面聊天客户端。 
 
## 构建 
 
服务端使用 Visual Studio 打开对应 .sln 文件编译。 
客户端使用 Qt Creator 打开 client/file.pro 编译。
