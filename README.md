# SimpleChat

基于 Qt 6 + C++ 的聊天室项目，包含分布式服务端和桌面客户端。

## 项目结构

```
SimpleChat/
 ├── server/              # 服务端（微服务架构）
 │    ├── ChatServer/     # 聊天服务器 1（C++, gRPC, MySQL, Redis）
 │    ├── ChatServer2/    # 聊天服务器 2（多实例部署）
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
  - FindSuccessDlg — 查找成功对话框（用户信息展示 + 添加好友）
  - FindFailDlg — 查找失败对话框
- **好友申请系统** — 查看待处理的好友申请，同意或拒绝
  - ApplyFriendPage — 好友申请页面列表
  - ApplyFriendItem — 单个申请条目（头像、昵称、认证按钮）
  - AuthenFriend — 好友认证处理对话框
- **联系人管理** — 联系人列表展示，好友信息管理
  - ContactUserList — 自定义联系人列表
  - ConUserItem — 联系人条目
  - GroupTipItem — 分组提示条目
  - FriendInfoPage — 好友信息详情页
- **聊天界面** — 侧边栏导航（聊天/联系人），搜索框，消息列表
  - ChatPage — 聊天主页面
  - ChatDialog — 聊天对话窗口
  - BubbleFrame — 聊天气泡容器
  - TextBubble / PictureBubble — 文字/图片气泡
  - ChatUserWid — 聊天用户列表项
- **TCP 连接管理** — 基于 QTcpSocket 的消息收发，自定义协议解析（TcpMgr）
- **HTTP 通信** — 基于 QNetworkAccessManager 的 REST API 请求（HttpMgr）
- **界面美化** — 自定义 QSS 样式，状态标签系统，头像展示

### 自定义控件

- `ClickedLabel` / `ClickedOnceLabel` — 可点击标签
- `ClickedBtn` — 自定义按钮
- `CustomizeEdit` — 自定义输入框
- `MessageTextEdit` — 消息输入框
- `TimerBtn` — 倒计时按钮（验证码场景）
- `StateWidget` — 状态指示组件
- `LoadingDlg` — 加载动画对话框

## 服务端

- **ChatServer** — 聊天服务器 1，处理消息路由和持久化（C++/gRPC）
- **ChatServer2** — 聊天服务器 2，多实例部署，同级对等通信
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

## License

MIT License
