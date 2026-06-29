#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Shop.h"
#include "Player.h"

struct ShopGridItem
{
    ItemID itemId = static_cast<ItemID>(0);
    std::string name;
    std::string description;
    int price = 0;
    ItemType type = ItemType::CONSUMABLE;
    sf::Texture texture;
    bool hasTexture = false;
    sf::Sprite sprite;
    bool isHovered = false;
    std::string stageLabel;
};

class ShopUI
{
private:
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 731;

    const sf::Color BACKGROUND_COLOR = sf::Color(40, 40, 40, 230);
    const sf::Color PANEL_COLOR = sf::Color(60, 60, 60);
    const sf::Color SELECTED_COLOR = sf::Color(100, 150, 255);
    const sf::Color TEXT_COLOR = sf::Color::White;
    const sf::Color BUTTON_COLOR = sf::Color(80, 80, 80);
    const sf::Color BUTTON_HOVER_COLOR = sf::Color(120, 120, 120);
    const sf::Color GOLD_COLOR = sf::Color(255, 215, 0);

    sf::Font font;
    Shop *shop;
    Player *player;

    std::vector<ShopGridItem> gridItems;
    int selectedItemIndex = 0;

    //滚动
    float gridScrollOffset = 0.0f;
    const float GRID_ITEM_SIZE = 80.0f;
    const float GRID_PADDING = 10.0f;

    //纹理缓存
    std::map<int, sf::Texture> textureCache;

    //UI元素
    sf::RectangleShape background;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;
    sf::RectangleShape topBar;

    //关闭按钮
    sf::RectangleShape closeButton;
    sf::Text closeButtonText;

    //金币文本
    sf::Text goldText;

    //标题
    sf::Text titleText;

    //详情面板
    sf::Text detailName;
    sf::Text detailPrice;
    sf::Text detailType;
    sf::Text detailDescription;

    //购买按钮
    sf::RectangleShape buyButton;
    sf::Text buyButtonText;

    //提示文本
    sf::Text feedbackText;
    sf::Clock feedbackClock;
    bool showFeedback = false;

    //宽字符串转换
    std::wstring toWideString(const std::string &str);

    void loadFont();
    void setupUI();

    void updateGridItems();
    void updateDetailPanel();
    void drawGrid(sf::RenderWindow &window);
    void drawDetailPanel(sf::RenderWindow &window);

    int getGridIndexFromMouse(float mouseX, float mouseY) const;

public:
    ShopUI(Player *player, Shop *shop);
    ~ShopUI() = default;

    bool isVisible = false;

    void open();
    void close();
    void handleEvent(sf::Event &event, sf::RenderWindow &window);
    void update(float dt);
    void render(sf::RenderWindow &window);
};
