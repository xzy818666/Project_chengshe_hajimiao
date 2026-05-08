# 赛博功德银行 (Cyber Merit Bank)

一款赛博朋克风格的桌面金融模拟器游戏。

## 🎮 游戏概述

**赛博功德银行** 是一款桌面金融模拟器，玩家通过点击法器积累基础货币"功德"，并在多资产金融市场中进行配置、交易与风控管理。

每一轮游戏算一个 **"轮回"**，时长大约10分钟。游戏特色包括：
- **功德系统**：基础货币，无波动，用于结算
- **资产市场**：四种风险等级的金融产品
- **业障机制**：负向征信状态，透支或逾期时触发
- **法器商城**：多种法器可购买和装备

## 🛠️ 技术栈

- **框架**: Qt 6.x (Qt Widgets + Qt Charts)
- **语言**: C++17
- **构建工具**: qmake + MinGW/MSVC

## 📦 安装依赖

### 方法一：使用 Qt Installer (推荐)

1. 下载 Qt 6.x 安装器：https://www.qt.io/download-open-source
2. 安装时选择以下组件：
   - Qt 6.x (MinGW 64-bit 或 MSVC 2022)
   - Qt Charts
   - Qt Tools (qmake, Qt Designer)

### 方法二：使用 vcpkg (高级用户)

```bash
vcpkg install qt6-base qt6-charts
```

## 🔧 编译运行

### 使用 Qt Creator (推荐)

1. 打开 Qt Creator
2. 打开项目文件 `merit_bank.pro`
3. 选择合适的构建套件 (MinGW 64-bit)
4. 点击"构建"按钮
5. 点击"运行"按钮

### 使用命令行

```bash
# 进入项目目录
cd d:\chengxu\trae\bank

# 使用 qmake 生成 Makefile
qmake merit_bank.pro

# 编译 (MinGW)
mingw32-make

# 或编译 (MSVC)
nmake

# 运行
release\merit_bank.exe
```

## 🎯 功能模块

### W1 功德堂 (主界面)
- 法器点击积累功德
- 实时HUD显示：功德余额、资产市值、净资产、业障值
- 底部导航：善财司、幻缘所、法器阁、忏悔
- 侧边统计：点击次数、自动收益、效率系数

### W2 善财司 (银行)
- **活期存款**：5%年利率，按秒计息
- **定期存款**：7天6%、30天8%、90天12%
- **功德借贷**：日利率1%，信用分0-1000，可借额度=信用分×2
- **业障状态**：效率惩罚机制

### W3 幻缘所 (交易市场)
- 四种资产：众生法缘、善行福缘、因果私缘、轮回孽缘
- K线图实时显示价格走势
- 交易面板：买入/卖出操作
- 资产配置饼图和风险评级

### W4 法器阁 (商城)
- 基础木鱼：+1/击
- 电子烧香：+1/秒 (持续60秒)
- 涡轮增压木鱼：+5/击
- 量子佛珠：+2/击，10%概率×10暴击
- AI诵经机：+10/秒，每秒扣1功德维护费

## 🎲 市场事件系统

- 期末祈福潮，指数+15%
- 体测取消，掀起退市潮
- 官方辟谣反涌
- 期中考试临近，功德需求上升
- 政策利好，全面上涨
- 恐慌抛售

## 📁 项目结构

```
merit_bank/
├── merit_bank.pro          # Qt项目配置
├── main.cpp                # 程序入口
├── meritbank.cpp           # 主程序逻辑
├── resources.qrc           # 资源文件
├── wallet.h/cpp            # 钱包系统
├── asset.h/cpp             # 资产基类
├── meritindex.h/cpp        # 众生法缘指数
├── karmabond.h/cpp         # 善行福缘债券
├── dharmafund.h/cpp        # 因果私缘基金
├── samsarafutures.h/cpp    # 轮回孽缘期货
├── instrument.h/cpp        # 法器系统
├── marketevent.h/cpp       # 市场事件系统
├── merithall.ui/h/cpp      # 功德堂主界面
├── bankdialog.ui/h/cpp     # 善财司对话框
├── exchangedialog.ui/h/cpp # 幻缘所对话框
└── shopdialog.ui/h/cpp     # 法器阁对话框
```

## 📜 许可证

MIT License

## 🤝 贡献

欢迎提交Issue和Pull Request！
