#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "Player.h"
#include "Item.h"
#include "ItemDB.h"
#include "codemon.h"

enum class BagTab
{
    ITEMS,
    CODEMONS,
    KEYS,
    SKILLS
};

enum class EquipmentAction
{
    NONE,
    SELECTING_EQUIPMENT
};

struct GridItem
{
    int id = 0;
    std::string name;
    std::string description;
    int count = 0;
    int price = 0;
    ItemType type = ItemType::CONSUMABLE;
    ItemID itemId = static_cast<ItemID>(0);
    sf::Texture texture;
    bool hasTexture = false;
    sf::Sprite sprite;
    bool isSelected = false;
    bool isHovered = false;

    //精灵特有属性
    monbase::Codemon* codemon = nullptr;
    bool isTeamSlot = false;
    std::vector<const monbase::Move*> equippedMoves;
    std::vector<const monbase::Move*> unlockedMoves;
    int unlockedMoveIndex = -1;

    //装备特有属性
    bool isEquipped = false;
    EquipSlot equipSlot = EquipSlot::NONE;
};

class BagUI
{
private:
    //窗口尺寸
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 731;

    //布局参数
    const float LEFT_PANEL_RATIO = 0.7f;
    const float RIGHT_PANEL_RATIO = 0.3f;
    const float TOP_BAR_HEIGHT_RATIO = 0.08f;

    //颜色定义
    const sf::Color BACKGROUND_COLOR = sf::Color(40, 40, 40, 230);
    const sf::Color PANEL_COLOR = sf::Color(60, 60, 60);
    const sf::Color SELECTED_COLOR = sf::Color(100, 150, 255);
    const sf::Color TEXT_COLOR = sf::Color::White;
    const sf::Color BUTTON_COLOR = sf::Color(80, 80, 80);
    const sf::Color BUTTON_HOVER_COLOR = sf::Color(120, 120, 120);

    //纹理缓存
    std::map<int, sf::Texture> textureCache;
    sf::Texture placeholderTexture;

    //字体
    sf::Font font;

    //玩家指针
    Player* player;

    //当前标签页
    BagTab currentTab = BagTab::ITEMS;

    //网格项
    std::vector<GridItem> gridItems;
    int selectedItemIndex = 0;
    int previousSelectedIndex = -1;
    int previousSelectedTeamSlot = -1;

    //精灵背包特殊变量
    int selectedTeamSlot = -1;
    int selectedStorageIndex = 0;
    bool isSelectingEquipment = false;
    EquipSlot selectingSlot = EquipSlot::NONE;

    //装备系统变量
    EquipmentAction equipmentAction = EquipmentAction::NONE;
    int selectingCodemonIndex = -1;
    bool isTeamCodemon = false;

    //技能系统变量
    monbase::Codemon* skillTargetCodemon = nullptr;  //当前要装备技能的精灵
    int skillTargetSlot = -1;                        //目标技能槽位

    //UI元素
    sf::RectangleShape background;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;
    sf::RectangleShape topBar;

    //窗口指针
    sf::RenderWindow* ownerWindow = nullptr;

    //标签页按钮
    sf::RectangleShape tabButtons[3];
    sf::Text tabButtonsText[3];

    //关闭按钮
    sf::RectangleShape closeButton;
    sf::Text closeButtonText;

    //网格视图
    sf::View gridView;
    float gridScrollOffset = 0.0f;
    const float GRID_ITEM_SIZE = 80.0f;
    const float GRID_PADDING = 10.0f;

    //右侧详情面板
    sf::Text detailTitle;
    sf::Sprite detailSprite;
    sf::Text detailName;
    sf::Text detailCount;
    sf::Text detailType;
    sf::Text detailDescription;
    sf::Text detailPrice;
    //顶部显示金币与经验
    sf::Text goldText;
    sf::Text expText;

    //按钮
    sf::RectangleShape sellButton;
    sf::Text sellButtonText;
    sf::RectangleShape switchButton;
    sf::Text switchButtonText;
    //升级按钮
    sf::RectangleShape upgradeButton;
    sf::Text upgradeButtonText;

    //精灵技能栏
    std::vector<sf::RectangleShape> skillSlots;
    std::vector<sf::Text> skillText;
    std::vector<sf::RectangleShape> skillEquipIndicators;

    //装备栏
    std::vector<sf::RectangleShape> equipmentSlot;
    std::vector<sf::Sprite> equipmentSprites;
    std::vector<sf::Text> equipmentTexts;

    //技能列表相关
    float skillListScrollOffset = 0.0f;
    const float SKILL_LIST_ITEM_HEIGHT = 60.0f;
    const float SKILL_LIST_MAX_VISIBLE_ITEMS = 8.0f;

    bool prevVisible = false;
    std::size_t prevInventoryHash = 0;

    //宽字符串转换函数
    std::wstring toWideString(const std::string& str);
    std::size_t computeInventoryHash() const;
    bool showDetailSprite = false;
    bool upgradeEnabled = false;
    std::wstring wrapWideString(const std::wstring& src, std::size_t maxChars) const;
    std::wstring elementToWideString(monbase::Element e) const;

    void loadFont();
    void setupUI();
    void drawGrid(sf::RenderWindow& window);
    void drawDetailPanel(sf::RenderWindow& window);
    void handleMouseClick(sf::Vector2f mousePos, sf::Mouse::Button button);
    void handleMouseWheel(float delta);
    void switchToTab(BagTab tab);

    int getGridIndexFromMouse(float mouseX, float mouseY, float startX, float startY, int cols, float padding, float iconSize);

    void handleEquipmentSlotClick(sf::Vector2f mousePos, int slotIndex);
    void handleEquipmentSelection(sf::Vector2f mousePos);
    void startEquipmentSelection(int codemonIndex, EquipSlot slot, bool isTeam);
    void cancelEquipmentSelection();
    void equipItemToCodemon(monbase::Codemon* codemon, Item* item, ItemID itemId);
    void unequipItemFromCodemon(monbase::Codemon* codemon, EquipSlot slot, ItemID itemId);
    bool isValidSkill(const monbase::Codemon* codemon, int skillIndex) const;

public:
    BagUI(Player* player);
    ~BagUI() = default;

    void updateGridItems();
    void updateDetailPanel();
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void update(sf::RenderWindow* window = nullptr);
    void setOwnerWindow(sf::RenderWindow* window) { ownerWindow = window; }
    void render(sf::RenderWindow& window);
    bool isVisible = false;
};