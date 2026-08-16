# Codemon

大一游戏开发项目，精灵对战游戏。

## 项目结构

```text
Codemon/
├── main.cpp
├── include/          # 头文件
├── src/              # 实现文件
└── README.md
```

## 运行依赖

- Windows
- SFML（Graphics / Window / System / Audio）
- `fonts/simhei.ttf`，用于中文界面显示
- `Resources/` 图片资源，包括地图、玩家、敌人、道具和 UI

## 操作

- `W/A/S/D`：移动
- `E`：开启宝箱、使用钥匙
- `F`：与附近敌人进入战斗
- `Q`：调试用快速击败敌人
- `C`：检查地图通关状态，通关后进入下一关
- `Z`：保存并返回主菜单
- `B`：打开背包
- `M`：打开精灵图鉴
- `H`：打开商店

## 说明

- 入口文件是根目录的 `main.cpp`。
- 头文件位于 `include/`，实现文件位于 `src/`。
- 当前仓库未包含 `fonts/` 和 `Resources/` 资源目录。
