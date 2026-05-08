# Qt Creator 安装指南

## 方法一：从官网下载安装（推荐）

### 步骤1：访问Qt官网下载页面
打开浏览器访问：https://www.qt.io/download-open-source

### 步骤2：选择下载方式
- 选择 "Download the Qt Online Installer"
- 或直接访问：https://www.qt.io/offline-installers

### 步骤3：下载安装器
下载文件：`qt-unified-windows-x86-online.exe`（约30MB）

### 步骤4：安装Qt Creator
1. 运行安装程序
2. 使用Qt账号登录（或跳过）
3. 选择安装路径（建议：`D:\Qt`）
4. **重要**：勾选以下组件：
   - ✅ Qt 6.5.x → MinGW 11.2.0 64-bit
   - ✅ Qt Charts
   - ✅ Qt Designer
   - ✅ Qt Tools → Qt Creator
   - ✅ Qt 6.5.x → Qt 6.5.3 Sources（可选）
5. 接受许可协议
6. 点击"安装"

### 步骤5：打开并运行项目
1. 安装完成后，打开 **Qt Creator**
2. 点击 "文件" → "打开文件或项目"
3. 浏览到 `D:\chengxu\trae\bank\merit_bank.pro`
4. 选择 **Desktop Qt 6.5.x MinGW 64-bit** 套件
5. 点击 "配置项目"
6. 点击左下角 **▶运行** 按钮

---

## 方法二：使用预编译版本

### 下载预编译Qt Creator
1. 访问：https://github.com/nicdemDemivee/qt-creator/releases
2. 下载最新版本的Windows安装包

---

## 方法三：使用Visual Studio + Qt插件

如果你已有Visual Studio 2022：
1. 安装 **Qt Visual Studio Tools** 扩展
2. 在VS中安装Qt 6.x
3. 打开Qt项目

---

## 首次运行检查清单

- [ ] Qt Creator已安装
- [ ] Qt 6.x + MinGW已安装
- [ ] 项目文件 `merit_bank.pro` 已打开
- [ ] 构建套件已选择
- [ ] 点击"运行"按钮

---

## 常见问题

### Q: 安装后找不到Qt Creator
**A**: 默认路径是 `C:\Qt\6.5.x\mingw_64\bin\qtcreator.exe`

### Q: 编译报错"qmake not found"
**A**: 在Qt Creator中，工具→选项→Kits，选择正确的qmake路径

### Q: 提示缺少DLL
**A**: 将Qt的bin目录添加到系统PATH环境变量

---

## 安装后操作

安装完成后，你可以直接双击项目中的 `merit_bank.pro` 文件，Qt Creator会自动打开项目。

祝你玩得愉快！🎮
