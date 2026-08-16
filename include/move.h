#pragma once
#include "Map.h"
#include <vector>
#include <memory>
using namespace std;

class MapManager
{
private:
    unique_ptr<Map> maps[6];                 // 所有关卡
    string currentPlayer;                    // 当前玩家名称
    int currentMapIndex;                     // 当前关卡索引
    int lastMapIndex;                        // 上一个关卡索引
    bool IsValidMapIndex(int index) const;   // 检查地图索引是否有效

public:
    MapManager();
    ~MapManager() = default;

    // 关卡管理
    int AddMap(std::unique_ptr<Map> map);    // 添加关卡
    bool LoadMap(int mapIndex);              // 加载指定关卡
    void UnloadCurrentMap();                 // 卸载当前关卡
    void ReloadCurrentMap();                 // 重新加载当前关卡

    // 关卡切换
    bool SwitchToMap(int mapIndex);          // 切换到指定关卡
    bool SwitchToNextMap();                  // 切换到下一关
    bool SwitchToPreviousMap();              // 切换到上一关

    // 查询
    Map* GetCurrentMap() const;                 // 获取当前关卡
    Map* GetMap(int index) const;               // 获取指定关卡
    int GetCurrentMapIndex() const;             // 获取当前关卡索引
    int GetMapCount() const;                    // 获取当前关卡数量
	string GetCurrentPlayer() const;            // 获取当前玩家名称
	void SetCurrentPlayer(const string& name);  // 设置当前玩家名称

    // 进度保存
    void SaveProgress(const string& filename);  // 保存游戏进度
    bool LoadProgress(const string& filename);  // 加载游戏进度

    // 工具函数
    bool IsMapCompleted(int index) const;    // 检查关卡是否完成
    void ResetAllProgress();                 // 重置所有进度

    bool CheckPlayerArchive(const string& playerName);                      // 检查是否为活跃玩家
    bool VerifyPassword(const string& playerName, const string& password);  // 验证玩家密码
    void RegisterPlayer(const string& playerName, const string& password);  // 注册新玩家
};