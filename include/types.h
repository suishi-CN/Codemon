#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "spriteBackpack.h"

class SpriteWarehouse {
public:
    enum class RightPage {
        TeamDetail,
        Warehouse,
        Pokedex
    };

    SpriteWarehouse(SpriteBackpack& backpack, const sf::Font& font, const std::string& savePath = "backpack.dat");

    void loadTextures();
    void draw(sf::RenderWindow& window);
    bool handleClick(sf::Vector2f mousePos);

    int getSelectedSlot() const { return selectedSlot; }
    void setSelectedSlot(int slot);
    void setTeamCount(int tc) { teamCount = tc; }
    RightPage getCurrentPage() const { return currentPage; }

private:
    struct SpeciesDisp {
        const wchar_t* name;
        int type1, type2;
        const wchar_t* ability;
        int hp, atk, def, mgAtk, mgDef, spd;
    };
    static const SpeciesDisp kSpecies[45];

    SpriteBackpack& backpack;
    const sf::Font& font;
    std::string savePath;
    int selectedSlot = 0;
    RightPage currentPage = RightPage::TeamDetail;

    // Edit mode
    bool editMode = false;
    int teamCount = 0;

    // Pokedex sub-state
    int pokedexPage = 0;
    int pokedexSelected = 0;
    bool pokedexDetailView = false;
    std::vector<sf::Texture> textures;

    // Layout constants
    static constexpr int kMaxTeam = 6;
    static constexpr float kLeftRatio = 0.2f;
    static constexpr float kPad = 15.0f;
    static constexpr float kCapGap = 6.0f;
    static constexpr float kCapMarginX = 8.0f;
    static constexpr float kAvatarRatio = 0.30f;
    static constexpr int kW = 960;
    static constexpr int kH = 720;
    static constexpr float kBtnW = 72.0f;
    static constexpr float kBtnH = 28.0f;

    float getCapsuleHeight() const;
    sf::FloatRect getCapsuleRect(int index) const;
    sf::FloatRect getRightRect() const;
    sf::FloatRect getNavBtnRect() const;
    int hitTestCapsule(sf::Vector2f pos) const;

    void drawTeamPanel(sf::RenderWindow& window);
    void drawCapsule(sf::RenderWindow& window, int index, sf::FloatRect rect);
    void drawRightPanel(sf::RenderWindow& window);

    void drawTeamDetail(sf::RenderWindow& window, sf::FloatRect rr);
    void drawEditMode(sf::RenderWindow& window, sf::FloatRect rr);
    void drawWarehouse(sf::RenderWindow& window, sf::FloatRect rr);
    void drawPokedex(sf::RenderWindow& window, sf::FloatRect rr);
    void drawPokedexGallery(sf::RenderWindow& window, sf::FloatRect rr);
    void drawPokedexDetail(sf::RenderWindow& window, sf::FloatRect rr);

    void drawCapsuleShape(sf::RenderWindow& window, sf::FloatRect r, sf::Color fill, float outlineW, sf::Color outline);
    void drawRadarChart(sf::RenderWindow& window, sf::Vector2f center, float radius,
                         int hp, int atk, int def, int mgAtk, int mgDef, int spd);

    static sf::Color elemColor(int e);
};
