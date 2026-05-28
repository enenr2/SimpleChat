# GateSaver 客户端修改记录

**日期**：2026-05-15
**状态**：已实施

## 背景

客户端与服务端 JSON 字段定义不一致，导致注册、登录、重置密码三个功能无法正常工作。

---

## 修改 1：注册 — 添加 `name` 字段

**文件**：`registerdialong.cpp`
**函数**：`on_pushButton_clicked()`

```diff
 QJsonObject json_obj;
 json_obj["user"]=ui->lineEdit->text();
+json_obj["name"]=ui->lineEdit->text();
 json_obj["email"]=ui->lineEdit_2->text();
```

**原因**：服务端 `RegUser` 取 `src_root["name"]` 作为数据库 name 列的值。客户端缺失此字段导致 name 为空，而 name 列有 UNIQUE 约束，第二次注册起即失败。

**回滚**：删除 `json_obj["name"]=ui->lineEdit->text();` 这一行。

---

## 修改 2：登录 — 添加 `user` 字段

**文件**：`longindialog.cpp`
**函数**：`on_pushButton_2_clicked()`

```diff
 QJsonObject json_obj;
+json_obj["user"]=email;
 json_obj["email"]=email;
```

**原因**：服务端 `/user_login` 取 `src_root["user"]` 作为 `CheckPwd` 的查询条件（内部 SQL 为 `WHERE email = ?`）。缺失此字段导致永远查不到用户。

**回滚**：删除 `json_obj["user"]=email;` 这一行。

---

## 修改 3：重置密码 — `passwd` → `pwd`

**文件**：`resetdialong.cpp`
**函数**：`on_pushButton_clicked()`

```diff
-json_obj["passwd"] = xorString(ui->pwd_edit->text());
+json_obj["pwd"] = xorString(ui->pwd_edit->text());
```

**原因**：服务端 `/reset_pwd` 取 `src_root["pwd"]`（不是 `passwd`）作为新密码值。字段名不匹配会导致密码被重置为空字符串。

**回滚**：将 `"pwd"` 改回 `"passwd"`。

---

## 修改汇总

| 文件 | 改动 | 回滚 |
|------|------|------|
| `registerdialong.cpp` | 加 1 行 `json_obj["name"]=...` | 删该行 |
| `longindialog.cpp` | 加 1 行 `json_obj["user"]=...` | 删该行 |
| `resetdialong.cpp` | `"passwd"` → `"pwd"` （1 处） | 改回 |
