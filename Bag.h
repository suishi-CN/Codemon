#pragma once
#include "species.h"
#include <vector>
#include <memory>

class Backpack {
public:
    struct Entry {
        int codemonId;
        int level;
        std::unique_ptr<monbase::Codemon> instance;
    };

    void add(int speciesId, int level = 1) {
        auto codemon = create(speciesId, level);
        if (!codemon) return;
        int actualId = codemon->getCodemonId();
        entries.push_back({actualId, level, std::move(codemon)});
    }

    void remove(int index) {
        if (index >= 0 && index < static_cast<int>(entries.size()))
            entries.erase(entries.begin() + index);
    }

    monbase::Codemon* get(int index) const {
        if (index >= 0 && index < static_cast<int>(entries.size()))
            return entries[index].instance.get();
        return nullptr;
    }

    const Entry& getEntry(int index) const { return entries[index]; }

    int size() const { return static_cast<int>(entries.size()); }

    void clear() { entries.clear(); }

    static std::unique_ptr<monbase::Codemon> create(int id, int lvl) {
        switch (id) {
        case 1:  case 2:  case 3:  return std::make_unique<Jimi>(lvl);
        case 4:  case 5:  case 6:  return std::make_unique<Huohua>(lvl);
        case 7:  case 8:  case 9:  return std::make_unique<Shuilanlan>(lvl);
        case 10: case 11:          return std::make_unique<Haibao>(lvl);
        case 12: case 13: case 14: return std::make_unique<Guaiguaihu>(lvl);
        case 15: case 16: case 17: return std::make_unique<DaerMaodou>(lvl);
        case 18: case 19: case 20: return std::make_unique<Hualing>(lvl);
        case 21: case 22:          return std::make_unique<Xiaoxingguang>(lvl);
        case 23: case 24:          return std::make_unique<Mengyou>(lvl);
        case 25: case 26: case 27: return std::make_unique<Amiyate>(lvl);
        case 28: case 29:          return std::make_unique<DatouGulong>(lvl);
        case 30: case 31:          return std::make_unique<DanxiaoManyu>(lvl);
        case 32: case 33:          return std::make_unique<FeicuiShuimu>(lvl);
        case 34:                   return std::make_unique<ShenlanJing>(lvl);
        case 35: case 36: case 37: return std::make_unique<ShuidiShe>(lvl);
        case 38: case 39:          return std::make_unique<JiayouHaikui>(lvl);
        case 40: case 41: case 42: return std::make_unique<XiaoShijiu>(lvl);
        case 43: case 44: case 45: return std::make_unique<KeaiYuan>(lvl);
        default:                   return nullptr;
        }
    }

private:
    std::vector<Entry> entries;
};
