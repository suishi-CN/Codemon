#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <set>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "codemon.h"
#include "Bag.h"
#include "Obj.h"

class MapManager;

class Player {
public:
    enum class State
    {
        Idle,
        Moving,
        InBattle,
        Dead
    };

    std::string name;
    int gold;
    int exp;
    Bag bag;

    static constexpr int MAX_TEAM_SIZE = 6; // 出战上限

    //地图进度系统
    std::string currentStage;
    std::set<std::string> reachedStage;

    //仓库&队伍系统
    std::vector<std::unique_ptr<monbase::Codemon>> storage; // 仓库
    std::vector<std::unique_ptr<monbase::Codemon>> team;    // 出战队伍（最多6只）

    //钥匙仓库
    std::vector<std::unique_ptr<Item>> keyStorage;

    MapManager* mapManager;
    void SetMapManager(MapManager* mm);
    MapManager* GetMapManager() const;

    bool HasKey(const std::string& keyName) const;
    void RemoveKey(const std::string& keyName);

    //空间属性
    sf::Vector2f position;

    int currentMapIndex = -1;
    int currentRoomIndex = -1;

    bool canMove = true;

    //战斗状态
    State state = State::Idle;

    bool IsInBattle() const;
    bool IsDead() const;

    //构造函数
    Player(const std::string& playerName);

    //仓库管理
    void enterStage(const std::string& stageName);
    void acquireCodemon(std::unique_ptr<monbase::Codemon>&& codemon);
    monbase::Codemon* getStorageCodemon(int index);
    size_t getStorageSize() const;

    //队伍管理
    bool withdrawToTeam(int storageIndex);
    bool depositToStorage(int teamIndex);
    monbase::Codemon* getTeamCodemon(int index);
    monbase::Codemon* getActiveCodemon();
    size_t getTeamSize() const;
    bool isTeamFull() const;

    //获取仓库中指定精灵的已解锁技能列表
    const std::vector<monbase::Move>* getStorageCodemonUnlockedMoves(int storageIndex) const;

    //获取仓库中指定精灵的装备技能索引
    const std::vector<int>* getStorageCodemonEquippedIndices(int storageIndex) const;

    //获取仓库中指定精灵的已装备技能
    std::vector<const monbase::Move*> getStorageCodemonEquippedMoves(int storageIndex) const;

    //切换仓库中指定精灵的技能
    bool swapStorageCodemonMove(int storageIndex, int eIdx, int uIdx);

    //获取仓库中指定精灵的已装备技能数量
    int getStorageCodemonEquippedCount(int storageIndex) const;

    //检查仓库中指定精灵的技能槽是否已满
    bool isStorageCodemonMovesFull(int storageIndex) const;

    //经济系统
    bool spendGold(int amount);
    void earnGold(int amount);
    int getGold() const;

    //经验
    void addExp(int e);
    int getExp() const;

    //钥匙系统
    void addKey(std::unique_ptr<Item> key);
    void useKey();

    //存档系统
    bool SaveToFile(const std::string& filename);
    bool LoadFromFile(const std::string& filename);

    //宝箱奖励
    void addChestReward(ChestRewardData&& reward);

    //清空
    void clearPlayer();

private:
    static const monbase::Move* findMoveByName(const std::string& name);
    void SaveCodemon(std::ofstream& ofs, const monbase::Codemon* codemon);
    std::unique_ptr<monbase::Codemon> LoadCodemon(std::ifstream& ifs);

    void SaveBag(std::ofstream& ofs);
    void LoadBag(std::ifstream& ifs);

    void SaveKeys(std::ofstream& ofs);
    void LoadKeys(std::ifstream& ifs);
};