#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class SpritePokedex {
public:
    SpritePokedex() = default;
    ~SpritePokedex() = default;

    bool loadFont(const std::string& fontPath);
    void loadTextures();
    void loadSkillPools();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    bool isVisible = false;
    void toggle() { isVisible = !isVisible; }

private:
    // 精灵数据
    struct SpeciesInfo {
        const wchar_t* name;
        int type1, type2;
        const wchar_t* ability;
        int hp, atk, def, mgAtk, mgDef, spd;
    };
    struct MovePoolEntry {
        int level;
        std::wstring name;
        std::wstring desc;
    };

    static const SpeciesInfo kSpecies[45];
    static const wchar_t* kElemNames[16];

    // 资源
    sf::Font font;
    sf::Texture textures[45];
    bool textureLoaded[45] = {false};
    std::vector<MovePoolEntry> speciesMovePools[45];

    // 导航
    int currentPage = 0;
    int detailIndex = -1;       // -1: 画廊, 0-44: 详情
    bool showSkills = false;
    int skillPage = 0;

    // 布局
    static constexpr int kW = 1000;
    static constexpr int kH = 731;
    static constexpr int COLS = 3, ROWS = 3, PER_PAGE = 9;
    static constexpr float CARD_W = 280.0f;
    static constexpr float CARD_H = 170.0f;
    static constexpr float CARD_GAP_X = 20.0f;
    static constexpr float CARD_GAP_Y = 16.0f;
    static constexpr float GRID_START_Y = 65.0f;

    void drawGallery(sf::RenderWindow& window);
    void drawDetail(sf::RenderWindow& window);
    void drawSkills(sf::RenderWindow& window);
    void drawRadarChart(sf::RenderWindow& window, sf::Vector2f center, float radius,
                        int hp, int atk, int def, int mgAtk, int mgDef, int spd);
    static sf::Color elemColor(int e);
};
