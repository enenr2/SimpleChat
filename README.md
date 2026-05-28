# SimpleChat

基于 Qt 6 + C++ 的聊天室项目，包含分布式服务端和桌面客户端。

## 项目结构

```
SimpleChat/
 ├── server/              # 服务端（微服务架构）
 │    ├── ChatServer/     # 主聊天服务器（C++, gRPC, MySQL, Redis）
 │    ├── GateSaver/      # 网关服务
 │    ├── StatusServer/   # 状态服务
 │    └── VerifyServer/   # 验证服务（Node.js）
 └── client/              # Qt 桌面客户端
      ├── *.cpp / *.h     # 客户端源码
      ├── file.pro        # Qt 项目文件
      ├── qss/            # QSS 样式表
      └── res/            # 客户端资源文件
```

## 客户端功能

Qt C++ 桌面聊天客户端，基于 Qt 6.9 + MSVC 2022 构建。

### 已实现功能

- **用户注册/登录** — 邮箱验证码注册，密码登录，密码重置
- **好友搜索** — 搜索用户并发送好友申请
- **好友申请系统** — 查看待处理的好友申请，同意或拒绝
  - ApplyFriendPage — 好友申请页面列表
  - ApplyFriendItem — 单个申请条目（头像、昵称、认证按钮）
  - AuthenFriend — 好友认证处理对话框
- **联系人管理** — 联系人列表展示，好友信息管理
  - ContactUserList — 自定义联系人列表
  - ConUserItem — 联系人条目
  - GroupTipItem — 分组提示条目
- **聊天界面** — 侧边栏导航（聊天/联系人），搜索框，消息列表
- **TCP 连接管理** — 基于 QTcpSocket 的消息收发，协议解析
- **HTTP 通信** — 基于 QNetworkAccessManager 的 REST API 请求
- **界面美化** — 自定义 QSS 样式，标签系统，好友信息预览

### 数据结构

- `SearchInfo` / `AuthInfo` / `AuthRsp` — 用户搜索与认证信息
- `FriendInfo` / `UserInfo` — 好友与用户数据模型
- `TextChatData` / `TextChatMsg` — 聊天消息数据模型
- `ApplyInfo` — 好友申请信息

## 服务端

- **ChatServer** — 主聊天服务器，处理消息路由和持久化（C++/gRPC）
- **GateSaver** — 网关服务，请求转发与负载均衡
- **StatusServer** — 状态服务，用户在线状态管理
- **VerifyServer** — 验证码服务（Node.js）

### 技术栈

| 组件 | 技术 |
|------|------|
| 服务端 | C++, gRPC, MySQL, Redis |
| 验证服务 | Node.js |
| 客户端 | Qt 6.9, C++17, MSVC 2022 |
| 通信 | HTTP (REST), TCP (自定义协议) |

## 构建

### 服务端

使用 Visual Studio 2022 打开 `server/` 下对应 `.sln` 文件编译。

### 客户端

使用 Qt Creator 打开 `client/file.pro`，配置 Qt 6.9 + MSVC 2022 kit 编译运行。

客户端配置文件 `client/config.ini`：
```ini
[GateServer]
host=localhost
port=8080
```

## 变更日志

详见 [CHANGE_LOG.md](CHANGE_LOG.md)

## Git

```bash
git add .
git commit -m "描述你的改动"
git push
```
