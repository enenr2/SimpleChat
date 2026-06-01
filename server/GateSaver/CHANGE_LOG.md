# GateSaver 修改记录

**日期**：2026-05-15  
**状态**：已测试通过 ✅

## 问题

注册接口 `POST /user_register` 无法向数据库添加数据，始终返回 `error=1005 (UserExist)`。

## 根因

两个问题叠加导致：

### 1. `MysqlMgr::RegUser` 调用存储过程 `reg_user`
`MysqlDao::RegUser()` 依赖 MySQL 存储过程 `CALL reg_user(?,?,?,@result)`，但项目中已有不依赖存储过程的 `RegUserTransaction` 方法，未被使用。

### 2. 数据库 `user` 表缺少 `nick` 和 `icon` 列
`RegUserTransaction` 的 INSERT 语句需要 6 列（`uid, name, email, pwd, nick, icon`），但实际 `user` 表只有 5 列（`id, uid, name, email, pwd`），导致 INSERT 失败。

## 修改内容

### 文件 1：`MysqlMgr.cpp`（第 5 行）

**修改前**：
```cpp
return _dao.RegUser(name, email, pwd);
```

**修改后**：
```cpp
return _dao.RegUserTransaction(name, email, pwd, name);
```

### 文件 2：数据库 `enenr.user` 表

执行 SQL：
```sql
ALTER TABLE user ADD COLUMN nick VARCHAR(255) DEFAULT '';
ALTER TABLE user ADD COLUMN icon VARCHAR(255) DEFAULT '';
```

## 测试结果

| 项目 | 结果 |
|------|------|
| 编译 | 通过（零错误零警告） |
| 服务器启动 | 成功监听 8080 |
| POST /user_register | error=0, uid=3 |
| 数据库记录 | uid=3, name=newuser003, email=new003@test.com, pwd=testpwd123, nick=newuser003, icon=newuser003 |

## 回滚方法

1. 将 `MysqlMgr.cpp` 第 5 行恢复为 `return _dao.RegUser(name, email, pwd);`
2. 数据库执行 `ALTER TABLE user DROP COLUMN nick, DROP COLUMN icon;`
