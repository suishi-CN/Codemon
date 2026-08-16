#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

// 精灵背包：记录玩家拥有的所有精灵及其等级和技能
class SpriteBackpack {
public:
    struct SpriteEntry {
        int speciesId;                // 图鉴编号 (1-45)
        int level;                    // 当前等级
        int experience;               // 当前经验值
        std::vector<std::string> unlockedMoves;   // 已解锁的技能名列表
        std::vector<std::string> equippedMoves;   // 当前装备的技能名列表（最多4个）
    };

    void add(int speciesId, int level = 1) {
        sprites.push_back({speciesId, level, 0, {}, {}});
    }

    void remove(int index) {
        if (index >= 0 && index < static_cast<int>(sprites.size()))
            sprites.erase(sprites.begin() + index);
    }

    SpriteEntry& get(int index) {
        return sprites[index];
    }

    const SpriteEntry& get(int index) const {
        return sprites[index];
    }

    int size() const { return static_cast<int>(sprites.size()); }

    bool empty() const { return sprites.empty(); }

    void clear() { sprites.clear(); }

    void levelUp(int index) {
        if (index >= 0 && index < static_cast<int>(sprites.size()))
            sprites[index].level++;
    }

    bool hasSpecies(int speciesId) const {
        return std::any_of(sprites.begin(), sprites.end(),
            [speciesId](const SpriteEntry& e) { return e.speciesId == speciesId; });
    }

    int countOf(int speciesId) const {
        return static_cast<int>(std::count_if(sprites.begin(), sprites.end(),
            [speciesId](const SpriteEntry& e) { return e.speciesId == speciesId; }));
    }

    const std::vector<SpriteEntry>& getAll() const { return sprites; }

    // --- 存档 / 读档 ---

    void saveToFile(const std::string& path, int headerInt = 0) const {
        std::ofstream file(path, std::ios::binary);
        if (!file) return;
        file.write((const char*)&headerInt, sizeof(headerInt));
        int count = (int)sprites.size();
        file.write((const char*)&count, sizeof(count));
        for (auto& e : sprites) {
            file.write((const char*)&e.speciesId, sizeof(e.speciesId));
            file.write((const char*)&e.level, sizeof(e.level));
            file.write((const char*)&e.experience, sizeof(e.experience));
            auto writeStrVec = [&](const std::vector<std::string>& vec) {
                int n = (int)vec.size();
                file.write((const char*)&n, sizeof(n));
                for (auto& s : vec) {
                    int len = (int)s.size();
                    file.write((const char*)&len, sizeof(len));
                    file.write(s.data(), len);
                }
            };
            writeStrVec(e.unlockedMoves);
            writeStrVec(e.equippedMoves);
        }
    }

    void loadFromFile(const std::string& path, int& headerInt) {
        std::ifstream file(path, std::ios::binary);
        if (!file) return;
        sprites.clear();
        file.read((char*)&headerInt, sizeof(headerInt));
        int count;
        file.read((char*)&count, sizeof(count));
        sprites.resize(count);
        for (int i = 0; i < count; i++) {
            auto& e = sprites[i];
            file.read((char*)&e.speciesId, sizeof(e.speciesId));
            file.read((char*)&e.level, sizeof(e.level));
            file.read((char*)&e.experience, sizeof(e.experience));
            auto readStrVec = [&](std::vector<std::string>& vec) {
                int n;
                file.read((char*)&n, sizeof(n));
                vec.resize(n);
                for (int j = 0; j < n; j++) {
                    int len;
                    file.read((char*)&len, sizeof(len));
                    vec[j].resize(len);
                    file.read(vec[j].data(), len);
                }
            };
            readStrVec(e.unlockedMoves);
            readStrVec(e.equippedMoves);
        }
    }

    /// Move entry at index `from` to index `to` (in terms of original positions)
    void moveEntry(int from, int to) {
        if (from < 0 || from >= (int)sprites.size()) return;
        if (to < 0 || to >= (int)sprites.size()) return;
        if (from == to) return;
        auto entry = std::move(sprites[from]);
        sprites.erase(sprites.begin() + from);
        sprites.insert(sprites.begin() + to, std::move(entry));
    }

    // --- 技能管理 ---

    // 为指定精灵解锁一个新技能
    void unlockMove(int index, const std::string& moveName) {
        if (index < 0 || index >= static_cast<int>(sprites.size())) return;
        auto& entry = sprites[index];
        if (std::find(entry.unlockedMoves.begin(), entry.unlockedMoves.end(), moveName)
            == entry.unlockedMoves.end()) {
            entry.unlockedMoves.push_back(moveName);
        }
    }

    // 装备一个已解锁的技能（替换指定槽位，slot 0-3）
    void equipMove(int index, int slot, const std::string& moveName) {
        if (index < 0 || index >= static_cast<int>(sprites.size())) return;
        if (slot < 0 || slot > 3) return;
        auto& entry = sprites[index];
        // 只能装备已解锁的技能
        if (std::find(entry.unlockedMoves.begin(), entry.unlockedMoves.end(), moveName)
            == entry.unlockedMoves.end()) return;
        // 如果槽位不够，补到够
        while (static_cast<int>(entry.equippedMoves.size()) <= slot)
            entry.equippedMoves.push_back("");
        entry.equippedMoves[slot] = moveName;
    }

    // 卸下指定槽位的技能
    void unequipMove(int index, int slot) {
        if (index < 0 || index >= static_cast<int>(sprites.size())) return;
        auto& entry = sprites[index];
        if (slot >= 0 && slot < static_cast<int>(entry.equippedMoves.size()))
            entry.equippedMoves[slot] = "";
    }

    // 批量设置装备的四个技能（替换当前装备列表）
    void setEquippedMoves(int index, const std::vector<std::string>& moves) {
        if (index < 0 || index >= static_cast<int>(sprites.size())) return;
        auto& entry = sprites[index];
        entry.equippedMoves.clear();
        for (size_t i = 0; i < moves.size() && i < 4; i++) {
            if (std::find(entry.unlockedMoves.begin(), entry.unlockedMoves.end(), moves[i])
                != entry.unlockedMoves.end()) {
                entry.equippedMoves.push_back(moves[i]);
            }
        }
    }

    // 检查是否已解锁某技能
    bool hasUnlockedMove(int index, const std::string& moveName) const {
        if (index < 0 || index >= static_cast<int>(sprites.size())) return false;
        const auto& entry = sprites[index];
        return std::find(entry.unlockedMoves.begin(), entry.unlockedMoves.end(), moveName)
            != entry.unlockedMoves.end();
    }

private:
    std::vector<SpriteEntry> sprites;
};
