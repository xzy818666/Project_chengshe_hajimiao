# 赛博功德银行 (Cyber Merit Bank)

一款赛博朋克 × 古风仙侠风格的桌面金融模拟器游戏。

## 🎮 游戏概述

**赛博功德银行** 是一款 Qt 桌面金融模拟经营游戏。玩家通过点击法器积累基础货币"功德"，在多资产金融市场中进行配置、交易与风控管理，同时修炼升级、收集法器、触发共鸣。

每一轮游戏算一个 **"轮回"**，核心循环：
> 点击获得功德 → 存款/借贷/投资 → 市场事件波动 → 风控与调仓 → **修炼升级/法器共鸣** → 净资产增长 → 成就与轮回结算

## ✨ 核心特色系统

| 系统 | 说明 |
|---|---|
| **修炼等级** | 10级称号体系（凡人→无上佛），点击积累修为，每级解锁效率/暴击/杠杆/自动收益加成 |
| **法器共鸣** | 7种共鸣组合（烈火焚心、珠联璧合、万法归宗等），位掩码实时检测与效果叠加 |
| **信用等级** | S-F 六级动态评级，行为（还款/逾期/爆仓）实时影响利率、额度与手续费 |
| **业障因果** | 业障影响市场波动率，≥1000触发"天劫"事件，资产价格腰斩 |
| **内嵌教程** | 38步遮罩式实时引导，角色"玄机子"边玩边教 |
| **智能配置** | 基于夏普比率的 CAL 风险-收益图，资产配置可视化 |

## 🛠️ 技术栈

- **框架**: Qt 6.x (Qt Widgets + Qt Charts)
- **语言**: C++17
- **构建工具**: qmake

## 🔧 编译运行

### macOS (ARM64)
```bash
/Users/qtx/Qt/6.11.0/macos/bin/qmake merit_bank.pro
make -j4
./merit_bank.app/Contents/MacOS/merit_bank
```

### 通用
```bash
qmake merit_bank.pro
make -j4
# macOS: ./merit_bank.app/Contents/MacOS/merit_bank
# Windows: release\merit_bank.exe
```

依赖：Qt 6（Charts 模块必需）

## 🎯 功能模块

### W0 轮回入口 (StartMenu)
- 三种难度选择（简单/普通/困难），影响初始功德、通胀率、信用分与最大杠杆

### W1 功德堂 (MeritHall) — 主界面
- 法器点击积累功德与修炼经验
- 实时 HUD：功德余额、资产市值、净资产、业障值、**修炼等级与增益明细**
- **法器共鸣显示**：当前激活的共鸣效果（紫色文字）
- 四角建筑热区（**门位置精确热区**），hover 时门口光芒效果
- 底部莲花台：辅助法器静态展示

### W2 善财司 (BankDialog)
- **活期存款**：5%年利率，按秒计息
- **定期存款**：7天6%、30天8%、90天12%
- **功德借贷**：日利率1%，**信用等级动态影响利率与额度**
- **业障状态**：效率惩罚机制 + 忏悔滑块
- 杠杆仓位：追加保证金、一键平仓

### W3 幻缘所 (ExchangeDialog)
- 四种资产：众生法缘、善行福缘、因果私缘、轮回孽缘
- K线图实时显示价格走势
- 交易面板：买入/卖出/杠杆买入（1×–3×）
- **CAL 风险-收益图**：散点+射线可视化
- **智能配置建议**（Sharpe比率）、**套利提示**、**自动定投**

### W4 法器阁 (ShopDialog)
- **11种法器**：4种主动（云上）+ 7种辅助（莲台）
- 主动：基础木鱼、涡轮木鱼、量子佛珠、全视之眼
- 辅助：基础烧香、电子烧香、AI诵经机、袈裟、罗盘、拂尘、如意
- **7种共鸣组合**实时检测
- 烧香倒计时：60秒后自动熄灭

### W5 成就榜 (AchievementDialog)
- 8项成就：初入佛门、腱鞘炎警告、华尔街之狼、负债累累、功德无量、天台见、资产配置大师、杠杆赌徒

### W6 新手教程 (TutorialManager + TutorialOverlay)
- **38步内嵌遮罩式教程**，角色"玄机子"引导
- 三种模式：点击继续 / 等待点击 / 云海点击
- 支持跟随进入子对话框（BankDialog、ExchangeDialog等）

## 🎲 市场事件系统

29个校园主题事件，临时修改资产漂移率与波动率：
- 期末祈福潮，指数+15%
- 体测取消，掀起退市潮
- 期中考试临近，功德需求上升
- 政策利好，全面上涨 / 恐慌抛售
- **天劫事件**：业障≥1000时概率触发，所有资产价格腰斩

## 📁 项目结构

```
merit_bank/
├── merit_bank.pro              # Qt项目配置
├── main.cpp                    # 程序入口
├── meritbank.cpp               # 主程序逻辑
├── resources.qrc               # 资源文件（图片、字体）
├── AGENTS.md                   # 项目规范与现状（重要！）
├── CHANGELOG_*.md              # 修改记录
│
├── 经济内核
│   ├── wallet.h/cpp            # 钱包系统（余额、债务、信用分、等级）
│   ├── asset.h/cpp             # 资产基类
│   ├── meritindex.h/cpp        # 众生法缘指数
│   ├── karmabond.h/cpp         # 善行福缘债券
│   ├── dharmafund.h/cpp        # 因果私缘基金
│   ├── samsarafutures.h/cpp    # 轮回孽缘期货
│   └── instrument.h/cpp        # 法器系统
│
├── 特色机制
│   ├── cultivationlevel.h/cpp  # 修炼等级系统
│   ├── creditrating.h/cpp      # 信用等级动态评级
│   ├── instrumentresonance.h/cpp # 法器组合共鸣
│   └── marketevent.h/cpp       # 市场事件系统
│
├── 策略模块
│   ├── portfolioadvisor.h/cpp  # 智能配置建议
│   ├── arbitragescanner.h/cpp  # 套利扫描
│   └── autoinvestor.h/cpp      # 自动定投
│
├── 界面层
│   ├── merithall.ui/h/cpp      # 功德堂主界面
│   ├── bankdialog.ui/h/cpp     # 善财司对话框
│   ├── exchangedialog.ui/h/cpp # 幻缘所对话框
│   ├── shopdialog.ui/h/cpp     # 法器阁对话框
│   ├── achievementdialog.ui/h/cpp # 成就榜对话框
│   ├── startmenu.h/cpp         # 轮回入口
│   ├── tutorialmanager.h/cpp   # 教程管理器
│   └── tutorialoverlay.h/cpp   # 教程遮罩层
│
└── 工具
    ├── achievementmanager.h/cpp # 成就管理
    └── savemanager.h/cpp        # 存档管理
```

## 📜 文档

- **[AGENTS.md](AGENTS.md)** — 项目规范与现状：架构、功能、代码约定、资源清单
- **[CHANGELOG_2026-06-10.md](CHANGELOG_2026-06-10.md)** — 修改报告：教程系统重构、热区精确化、HUD修复等

## 📜 许可证

MIT License
