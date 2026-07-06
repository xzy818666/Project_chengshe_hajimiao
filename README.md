# 赛博功德银行 (Cyber Merit Bank)

一款赛博朋克 × 古风仙侠风格的桌面金融模拟器游戏。

## 快速开始

```bash
qmake merit_bank.pro
make -j4
./merit_bank.app/Contents/MacOS/merit_bank   # macOS
# 或 release\merit_bank.exe                  # Windows
```

依赖：Qt 6（Charts 模块必需）

## 提交内容说明

| 文件/文件夹 | 说明 |
|---|---|
| `src/` | **源代码**（.cpp / .h / .ui / .pro / .qrc / images/） |
| `docs/` | **文档**（项目规范、修改记录、详细 README） |
| `哈基喵-作业报告.pdf` | **作业报告**（10页以内，论文形式） |
| `哈基喵小组程设ppt.pptx` | **路演 PPT**（课堂展示用） |
| `demo_hajimiao.mp4` | **演示视频**（程序运行录屏） |

## 仓库结构

```
merit_bank/
├── src/                    # 源代码
│   ├── *.cpp, *.h, *.ui    # C++ 源码与 UI 文件
│   ├── merit_bank.pro      # Qt 项目配置
│   ├── resources.qrc       # 资源清单
│   └── images/             # 图片素材
├── docs/                   # 文档
│   ├── README.md           # 详细项目说明
│   ├── AGENTS.md           # 项目规范与现状
│   ├── CHANGELOG_*.md      # 修改记录
│   └── 哈基喵小组程设ppt.pptx  # 路演 PPT
├── 哈基喵-作业报告.pdf      # 作业报告
├── demo_hajimiao.mp4        # 演示视频
├── README.md                # 本文件（快速导航）
└── .gitignore
```

## 核心功能一览

- **修炼等级**：10级称号体系，点击积累修为
- **法器共鸣**：7种组合，位掩码实时检测
- **信用等级**：S-F 动态评级，行为影响利率与额度
- **业障因果**：天劫机制（业障≥1000触发资产腰斩）
- **38步教程**：内嵌遮罩式实时引导
- **智能配置**：CAL 风险-收益图 + 夏普比率

详见 `docs/README.md` 与 `docs/AGENTS.md`。
