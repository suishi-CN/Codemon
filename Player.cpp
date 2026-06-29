#include "../include/MapManager.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <filesystem>
using namespace std;
static const char* ADMIN_FILE = "admin.dat";

// 登录与验证
bool MapManager::CheckPlayerArchive(const string& playerName)
{
    string filename = playerName + "SaveGame.dat";
    return filesystem::exists(filename);
}

bool MapManager::VerifyPassword(const string& playerName, const string& password)
{
    ifstream in(ADMIN_FILE);
    string line, name, pwd;

    while (getline(in, line))
    {
        istringstream iss(line);
        if (iss >> name >> pwd)
        {
            if (name == playerName)
                return pwd == password;
        }
    }
    return false;
}

void MapManager::RegisterPlayer(const string& playerName, const string& password)
{
    ifstream in(ADMIN_FILE);
    ofstream out("admin_temp.dat");
    string line, name, pwd;

    while (getline(in, line))
    {
        istringstream iss(line);
        if (iss >> name >> pwd)
        {
            // 如果玩家已存在，跳过旧记录
            if (name == playerName)
				continue;
        }
        out << line << '\n';
    }
    in.close();
    out.close();

	// 删除旧文件并重命名新文件
    remove(ADMIN_FILE);
    auto ans = rename("admin_temp.dat", ADMIN_FILE);
    // 追加新玩家
    ofstream append(ADMIN_FILE, ios::app);
    append << playerName << " " << password << '\n';
    currentPlayer = playerName;
}

MapManager::MapManager()
{
    currentMapIndex = -1;
    lastMapIndex = -1;
    cout << "地图管理器已创建" << endl;
}

// 地图管理
int MapManager::AddMap(unique_ptr<Map> map)
{
    if (!map)
    {
        cout << "添加失败，尝试添加空地图指针" << endl;
        return -1;
    }
    for (int i = 0; i < 6; i++)
    {
        if (!maps[i])
        {
            maps[i] = move(map);
            cout << "已成功添加地图，索引：" << i << endl;
            return i;
        }
    }
    cout << "地图已满，无法添加" << endl;
    return -1;
}

bool MapManager::LoadMap(int mapIndex)
{
    if (!IsValidMapIndex(mapIndex))
    {
        cout << "地图索引 " << mapIndex << " 无效！" << endl;
        return false;
    }
    lastMapIndex = currentMapIndex;
    currentMapIndex = mapIndex;
    return true;
}

void MapManager::UnloadCurrentMap()
{
    if (currentMapIndex == -1)
    {
        cout << "当前没有加载的地图" << endl;
        return;
    }
    cout << "地图已卸载，索引：" << currentMapIndex << endl;
    lastMapIndex = currentMapIndex;
    currentMapIndex = -1;
}

void MapManager::ReloadCurrentMap()
{
    if (currentMapIndex == -1)
    {
        cout << "当前没有加载的地图" << endl;
        return;
    }
    int temp = currentMapIndex;
    UnloadCurrentMap();
    LoadMap(temp);
    cout << "地图已重新加载" << endl;
}

// 地图切换
bool MapManager::SwitchToMap(int mapIndex)
{
    if (mapIndex == currentMapIndex)
        return true;
    return LoadMap(mapIndex);
}

bool MapManager::SwitchToNextMap()
{
    return SwitchToMap(currentMapIndex + 1);
}

bool MapManager::SwitchToPreviousMap()
{
    return SwitchToMap(currentMapIndex - 1);
}

// 查询 
Map* MapManager::GetCurrentMap() const
{
    if (!IsValidMapIndex(currentMapIndex))
        return nullptr;
    return maps[currentMapIndex].get();
}

Map* MapManager::GetMap(int index) const
{
    if (!IsValidMapIndex(index))
        return nullptr;
    return maps[index].get();
}

int MapManager::GetCurrentMapIndex() const
{
    return currentMapIndex;
}

int MapManager::GetMapCount() const
{
    int count = 0;
    for (int i = 0; i < 6; i++)
        if (maps[i]) count++;
    return count;
}

string MapManager::GetCurrentPlayer() const
{
    return currentPlayer;
}

void MapManager::SetCurrentPlayer(const string& name)
{
    currentPlayer = name;
}

bool MapManager::IsValidMapIndex(int index) const
{ 
    return index >= 0 && index < 6 && maps[index] != nullptr; 
}

// 存档 / 读档
void MapManager::SaveProgress(const string& filename)
{
    // 使用二进制模式
    ofstream f(filename, ios::binary);
    if (!f) return;

    // 将文本数据也转换为二进制写入
    int count = GetMapCount();
    f.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // 保持原有的二进制序列化
    for (int i = 0; i < 6; i++) 
        if (maps[i]) maps[i]->Serialize(f);
}

bool MapManager::LoadProgress(const string& filename)
{
    // 确保二进制读取
    ifstream f(filename, ios::binary);
    if (!f) return false;
    int count = 0;
	int* countPtr = &count;
    f.read(reinterpret_cast<char*>(countPtr), sizeof(count));

    for (int i = 0; i < 6; i++)
        maps[i].reset();
    for (int i = 0; i < count && i < 6; i++)
    {
        maps[i] = make_unique<Map>();
        maps[i]->Deserialize(f);
    }
    currentMapIndex = 0;
    return true;
}

// 重置
void MapManager::ResetAllProgress()
{
    cout << "正在重置所有游戏进度..." << endl;
    for (int i = 0; i < 6; i++)
        maps[i].reset();
    currentMapIndex = -1;
    lastMapIndex = -1;
    string playerSave = GetCurrentPlayer() + "SaveGame.dat";
    string mapSave = GetCurrentPlayer() + "MapProgress.dat";
    remove(playerSave.c_str());
    remove(mapSave.c_str());
    cout << "所有游戏进度已重置" << endl;
}

// 完成判断
bool MapManager::IsMapCompleted(int index) const
{
    if (!IsValidMapIndex(index))
        return false;
    Map* map = maps[index].get();
    if (index == 0)
        return map->isRoomCompleted(3);
    else
        return map->isRoomCompleted(8);
}