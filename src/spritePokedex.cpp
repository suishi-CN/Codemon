#define NOMINMAX
#include <Windows.h>
#include <SFML/System/String.hpp>
#include "../include/ShopUI.h"
#include "../include/ItemDB.h"
#include <cmath>
#include <algorithm>
#include <map>

static std::string getItemTextureName(ItemID id)
{
    static const std::map<int, std::string> texMap = {
        {11001, "BasicHealthPotion"}, {11002, "NormalHealthPotion"}, {11003, "AdvancedHealthPotion"}, {12001, "BasicAttackPotion"}, {12002, "NormalAttackPotion"}, {12003, "AdvancedAttackPotion"}, {13001, "BasicMagicAttackPotion"}, {13002, "NormalMagicAttackPotion"}, {13003, "AdvancedMagicAttackPotion"}, {14001, "BasicDefencePotion"}, {14002, "NormalDefencePotion"}, {14003, "AdvancedDefencePotion"}, {15001, "BasicMagicDefencePotion"}, {15002, "NormalMagicDefencePotion"}, {15003, "AdvancedMagicDefencePotion"}, {16001, "EnergyPotion"}, {17001, "PurificationPotion"}, {18001, "BloodragePotion"}, {18002, "StonehidePotion"}, {18003, "SacredElixir"}, {18004, "ThiefPotion"}, {18005, "ArcanePotion"}, {18006, "GiantPotion"}, {18007, "VoidPotion"}, {21001, "CopperSword"}, {21002, "IronSword"}, {21003, "SteelSword"}, {21004, "DiamondSword"}, {21005, "DragonBoneSword"}, {21006, "CopperWand"}, {21007, "IronWand"}, {21008, "CrystalWand"}, {21009, "DiamondWand"}, {21010, "DragonBoneWand"}, {22001, "LeatherArmour"}, {22002, "CopperArmour"}, {22003, "IronArmour"}, {22004, "SteelArmour"}, {22005, "CrystalArmour"}, {22006, "DiamondArmour"}, {22007, "DragonScaleArmour"}, {23001, "IronNecklace"}, {23002, "GoldenNecklace"}, {23003, "RubyNecklace"}, {23004, "SapphireNecklace"}, {23005, "EmeraldNecklace"}, {23006, "DiamondNecklace"}, {23007, "DragonBoneNecklace"}, {24001, "IronRing"}, {24002, "QuartzRing"}, {24003, "GoldenRing"}, {24004, "RubyRing"}, {24005, "SapphireRing"}, {24006, "EmeraldRing"}, {24007, "DiamondRing"}, {24008, "DragonBoneRing"}, {30001, "KEY01"}, {30002, "KEY02"}, {30003, "KEY11"}, {30004, "KEY12"}, {30005, "KEY21"}, {30006, "KEY22"}, {30007, "KEY31"}, {30008, "KEY32"}, {30009, "KEY41"}, {30010, "KEY42"}, {30011, "KEY51"}, {30012, "KEY52"}};
    auto it = texMap.find(static_cast<int>(id));
    if (it != texMap.end())
    {
        return it->second;
    }
    return std::to_string(static_cast<int>(id));
}

std::wstring ShopUI::toWideString(const std::string &str)
{
    if (str.empty())
    {
        return L"";
    }
    int req = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
    if (req > 0)
    {
        std::wstring wstr(req - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], req);
        return wstr;
    }
    req = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (req > 0)
    {
        std::wstring wstr(req - 1, L'\0');
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], req);
        return wstr;
    }
    return L"";
}

ShopUI::ShopUI(Player *player, Shop *shop)
    : player(player), shop(shop)
{
    loadFont();
    setupUI();
    updateGridItems();
}

void ShopUI::loadFont()
{
    if (!font.loadFromFile("fonts/simhei.ttf"))
    {
        std::cerr << "警告: 无法加载字体文件 simhei.ttf！" << std::endl;
        font = sf::Font();
    }
}

void ShopUI::setupUI()
{
    float leftWidth = WINDOW_WIDTH * 0.7f;
    float topBarHeight = WINDOW_HEIGHT * 0.08f;

    //背景
    background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setFillColor(BACKGROUND_COLOR);

    //顶栏
    topBar.setSize(sf::Vector2f(WINDOW_WIDTH, topBarHeight));
    topBar.setFillColor(sf::Color(50, 50, 50));

    //左侧面板
    leftPanel.setSize(sf::Vector2f(leftWidth, WINDOW_HEIGHT - topBarHeight));
    leftPanel.setFillColor(PANEL_COLOR);
    leftPanel.setPosition(0, topBarHeight);

    //右侧面板
    rightPanel.setSize(sf::Vector2f(WINDOW_WIDTH - leftWidth, WINDOW_HEIGHT - topBarHeight));
    rightPanel.setFillColor(PANEL_COLOR);
    rightPanel.setPosition(leftWidth, topBarHeight);

    //标题
    titleText.setFont(font);
    titleText.setString(L"商店");
    titleText.setCharacterSize(22);
    titleText.setFillColor(TEXT_COLOR);
    titleText.setPosition(20, 12);

    //金币
    goldText.setFont(font);
    goldText.setCharacterSize(18);
    goldText.setFillColor(GOLD_COLOR);
    goldText.setString(L"金币: 0");

    //关闭按钮
    float closeSize = topBarHeight * 0.7f;
    closeButton.setSize(sf::Vector2f(closeSize, closeSize));
    closeButton.setFillColor(sf::Color(200, 50, 50));
    closeButton.setPosition(WINDOW_WIDTH - closeSize - 20, (topBarHeight - closeSize) / 2);

    closeButtonText.setFont(font);
    closeButtonText.setString(L"X");
    closeButtonText.setCharacterSize(20);
    closeButtonText.setFillColor(TEXT_COLOR);
    sf::FloatRect ctb = closeButtonText.getLocalBounds();
    closeButtonText.setOrigin(ctb.width / 2, ctb.height / 2);
    closeButtonText.setPosition(
        closeButton.getPosition().x + closeSize / 2,
        closeButton.getPosition().y + closeSize / 2);

    //详情面板控件
    float dp = 20.0f;
    float leftWidth_dp = leftWidth;

    detailName.setFont(font);
    detailName.setCharacterSize(20);
    detailName.setFillColor(TEXT_COLOR);
    detailName.setPosition(leftWidth_dp + dp, topBarHeight + dp + 40);

    detailType.setFont(font);
    detailType.setCharacterSize(16);
    detailType.setFillColor(TEXT_COLOR);
    detailType.setPosition(leftWidth_dp + dp, topBarHeight + dp + 70);

    detailPrice.setFont(font);
    detailPrice.setCharacterSize(18);
    detailPrice.setFillColor(GOLD_COLOR);
    detailPrice.setPosition(leftWidth_dp + dp, topBarHeight + dp + 100);

    detailDescription.setFont(font);
    detailDescription.setCharacterSize(14);
    detailDescription.setFillColor(TEXT_COLOR);
    detailDescription.setPosition(leftWidth_dp + dp, topBarHeight + dp + 140);
    detailDescription.setLineSpacing(1.2f);

    //购买按钮
    float bw = 140.0f, bh = 45.0f;
    buyButton.setSize(sf::Vector2f(bw, bh));
    buyButton.setFillColor(sf::Color(50, 150, 50));
    buyButton.setPosition(
        leftWidth_dp + (WINDOW_WIDTH - leftWidth_dp - bw) / 2,
        WINDOW_HEIGHT - 100);

    buyButtonText.setFont(font);
    buyButtonText.setString(L"购买");
    buyButtonText.setCharacterSize(18);
    buyButtonText.setFillColor(TEXT_COLOR);
    sf::FloatRect btb = buyButtonText.getLocalBounds();
    buyButtonText.setOrigin(btb.width / 2, btb.height / 2);
    buyButtonText.setPosition(
        buyButton.getPosition().x + bw / 2,
        buyButton.getPosition().y + bh / 2);

    //反馈文本
    feedbackText.setFont(font);
    feedbackText.setCharacterSize(16);
    feedbackText.setFillColor(sf::Color(255, 100, 100));
    feedbackText.setString(L"");
}

void ShopUI::open()
{
    shop->refreshGoods(*player);
    updateGridItems();
    selectedItemIndex = 0;
    gridScrollOffset = 0.0f;
    updateDetailPanel();
    isVisible = true;
}

void ShopUI::close()
{
    isVisible = false;
}

void ShopUI::updateGridItems()
{
    gridItems.clear();
    const auto &pool = shop->getUnlockPools();
    const auto &stages = shop->getStageOrder();
    int idx = 0;

    for (size_t si = 0; si < stages.size(); ++si)
    {
        const std::string &stageName = stages[si];
        auto pit = pool.find(stageName);
        if (pit == pool.end())
        {
            continue;
        }

        bool stageHasVisible = false;
        //检查该阶段是否有商品因玩家进度而解锁
        bool stageUnlocked = (stageName == "stage1") ||
                             (player->reachedStage.find(stageName) != player->reachedStage.end());

        if (!stageUnlocked)
        {
            continue;
        }

        for (const auto &itemPair : pit->second)
        {
            ShopGridItem gi;
            gi.itemId = itemPair.first;
            auto itemPtr = ItemDB::getItem(itemPair.first);
            if (!itemPtr)
            {
                continue;
            }
            gi.name = itemPtr->getName();
            gi.description = itemPtr->getDescription();
            gi.price = itemPair.second;
            gi.type = itemPtr->getType();
            gi.stageLabel = stageName;
            gi.hasTexture = false;

            //使用ItemDB中的纹理路径
            std::string texPath = ItemDB::getTexturePath(itemPair.first);
            if (!texPath.empty())
            {
                auto cacheIt = textureCache.find(static_cast<int>(itemPair.first));
                if (cacheIt == textureCache.end())
                {
                    sf::Texture tempTex;
                    if (tempTex.loadFromFile(texPath))
                    {
                        textureCache[static_cast<int>(itemPair.first)] = std::move(tempTex);
                        gi.sprite.setTexture(textureCache[static_cast<int>(itemPair.first)]);
                        gi.hasTexture = true;
                    }
                }
                else
                {
                    gi.sprite.setTexture(cacheIt->second);
                    gi.hasTexture = true;
                }
            }
            gridItems.push_back(std::move(gi));
            stageHasVisible = true;
        }
    }
}

void ShopUI::updateDetailPanel()
{
    if (gridItems.empty() || selectedItemIndex < 0 || selectedItemIndex >= static_cast<int>(gridItems.size()))
    {
        detailName.setString(L"");
        detailType.setString(L"");
        detailPrice.setString(L"");
        detailDescription.setString(L"");
        return;
    }
    const auto &gi = gridItems[selectedItemIndex];
    detailName.setString(toWideString(gi.name));
    std::wstring typeStr;
    switch (gi.type)
    {
    case ItemType::CONSUMABLE:
        typeStr = L"类型: 消耗品";
        break;
    case ItemType::EQUIPMENT:
        typeStr = L"类型: 装备";
        break;
    default:
        typeStr = L"类型: 其他";
        break;
    }
    detailType.setString(typeStr);
    detailPrice.setString(L"价格: " + std::to_wstring(gi.price) + L" 金币");

    //描述文字自动换行（每 18 个字符换行）
    std::wstring rawDesc = toWideString(gi.description);
    std::wstring wrappedDesc;
    std::size_t chCount = 0;
    for (wchar_t ch : rawDesc)
    {
        wrappedDesc.push_back(ch);
        chCount++;
        if (chCount >= 18)
        {
            wrappedDesc.push_back(L'\n');
            chCount = 0;
        }
    }
    detailDescription.setString(wrappedDesc);
}

void ShopUI::drawGrid(sf::RenderWindow &window)
{
    float leftWidth = WINDOW_WIDTH * 0.7f;
    float topBarHeight = WINDOW_HEIGHT * 0.08f;
    float gridStartX = GRID_PADDING;
    float gridStartY = topBarHeight + GRID_PADDING + gridScrollOffset;
    int cols = static_cast<int>((leftWidth - GRID_PADDING * 2) / (GRID_ITEM_SIZE + GRID_PADDING));

    //用裁剪视图将网格限制在左侧面板区域
    sf::View clipView(sf::FloatRect(0, topBarHeight, leftWidth, WINDOW_HEIGHT - topBarHeight));
    clipView.setViewport(sf::FloatRect(0, topBarHeight / WINDOW_HEIGHT, leftWidth / WINDOW_WIDTH, (WINDOW_HEIGHT - topBarHeight) / WINDOW_HEIGHT));
    window.setView(clipView);

    std::string currentStage = "";
    float extraStageOffset = 0.0f;
    int stageStartIdx = 0;

    for (int i = 0; i < static_cast<int>(gridItems.size()); ++i)
    {
        const auto &gi = gridItems[i];

        if (gi.stageLabel != currentStage)
        {
            if (!currentStage.empty())
            {
                int itemsInPrevStage = i - stageStartIdx;
                int rowsUsedByPrevStage = (itemsInPrevStage + cols - 1) / cols;
                extraStageOffset += rowsUsedByPrevStage * (GRID_ITEM_SIZE + GRID_PADDING);
                extraStageOffset += (GRID_ITEM_SIZE + GRID_PADDING);
            }
            currentStage = gi.stageLabel;
            stageStartIdx = i;
        }

        int offsetInStage = i - stageStartIdx;
        int row = offsetInStage / cols;
        int col = offsetInStage % cols;
        float x = gridStartX + col * (GRID_ITEM_SIZE + GRID_PADDING);
        float y = gridStartY + row * (GRID_ITEM_SIZE + GRID_PADDING) + extraStageOffset;

        sf::RectangleShape cell(sf::Vector2f(GRID_ITEM_SIZE, GRID_ITEM_SIZE));
        if (i == selectedItemIndex)
        {
            cell.setFillColor(SELECTED_COLOR);
        }
        else if (gi.isHovered)
        {
            cell.setFillColor(BUTTON_HOVER_COLOR);
        }
        else
        {
            cell.setFillColor(BUTTON_COLOR);
        }
        cell.setPosition(x, y);
        window.draw(cell);

        if (gi.hasTexture)
        {
            sf::Sprite s = gi.sprite;
            float scale = std::min(GRID_ITEM_SIZE / s.getLocalBounds().width, GRID_ITEM_SIZE / s.getLocalBounds().height) * 0.8f;
            s.setScale(scale, scale);
            s.setPosition(x + (GRID_ITEM_SIZE - s.getLocalBounds().width * scale) / 2,
                          y + (GRID_ITEM_SIZE - s.getLocalBounds().height * scale) / 2);
            window.draw(s);
        }
    }

    window.setView(window.getDefaultView());
}

void ShopUI::drawDetailPanel(sf::RenderWindow &window)
{
    if (gridItems.empty() || selectedItemIndex < 0 || selectedItemIndex >= static_cast<int>(gridItems.size()))
    {
        return;
    }

    window.draw(detailName);
    window.draw(detailType);
    window.draw(detailPrice);
    window.draw(detailDescription);
}

void ShopUI::render(sf::RenderWindow &window)
{
    if (!isVisible)
    {
        return;
    }

    //重置视图到默认状态
    window.setView(window.getDefaultView());

    //背景&面板
    window.draw(background);
    window.draw(leftPanel);
    window.draw(rightPanel);

    //网格和详情
    drawGrid(window);
    drawDetailPanel(window);

    //顶栏
    window.draw(topBar);
    window.draw(titleText);

    //金币文本
    goldText.setString(L"金币: " + std::to_wstring(player->getGold()));
    sf::FloatRect gtb = goldText.getLocalBounds();
    goldText.setPosition(closeButton.getPosition().x - gtb.width - 25,
                         (WINDOW_HEIGHT * 0.08f - gtb.height) / 2);
    window.draw(goldText);

    window.draw(closeButton);
    window.draw(closeButtonText);

    drawGrid(window);
    drawDetailPanel(window);

    window.draw(buyButton);
    window.draw(buyButtonText);

    if (showFeedback)
    {
        feedbackText.setString(toWideString(""));
        window.draw(feedbackText);
    }
}

void ShopUI::update(float dt)
{
    updateDetailPanel();
}

int ShopUI::getGridIndexFromMouse(float mouseX, float mouseY) const
{
    float leftWidth = WINDOW_WIDTH * 0.7f;
    float topBarHeight = WINDOW_HEIGHT * 0.08f;
    float gridStartX = GRID_PADDING;
    float gridStartY = topBarHeight + GRID_PADDING + gridScrollOffset;
    int cols = static_cast<int>((leftWidth - GRID_PADDING * 2) / (GRID_ITEM_SIZE + GRID_PADDING));

    if (mouseX < gridStartX || mouseX >= leftWidth || mouseY < topBarHeight)
    {
        return -1;
    }

    //遍历所有商品
    std::string currentStage = "";
    float extraOffset = 0.0f;
    int stageStart = 0;

    for (int i = 0; i < static_cast<int>(gridItems.size()); ++i)
    {
        if (gridItems[i].stageLabel != currentStage)
        {
            if (!currentStage.empty())
            {
                int itemsInPrevStage = i - stageStart;
                int rowsUsed = (itemsInPrevStage + cols - 1) / cols;
                extraOffset += rowsUsed * (GRID_ITEM_SIZE + GRID_PADDING);
                extraOffset += (GRID_ITEM_SIZE + GRID_PADDING);
            }
            currentStage = gridItems[i].stageLabel;
            stageStart = i;
        }

        int offsetInStage = i - stageStart;
        int col = offsetInStage % cols;
        int row = offsetInStage / cols;
        float ix = gridStartX + col * (GRID_ITEM_SIZE + GRID_PADDING);
        float iy = gridStartY + row * (GRID_ITEM_SIZE + GRID_PADDING) + extraOffset;

        sf::FloatRect cellRect(ix, iy, GRID_ITEM_SIZE, GRID_ITEM_SIZE);
        if (cellRect.contains(mouseX, mouseY))
        {
            return i;
        }
    }
    return -1;
}

void ShopUI::handleEvent(sf::Event &event, sf::RenderWindow &window)
{
    if (!isVisible)
    {
        return;
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (isVisible && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        close();
        return;
    }

    if (event.type == sf::Event::MouseMoved)
    {
        //悬停检测
        int hoverIdx = getGridIndexFromMouse(mousePos.x, mousePos.y);
        for (int i = 0; i < static_cast<int>(gridItems.size()); ++i)
            gridItems[i].isHovered = (i == hoverIdx);
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        //关闭按钮
        sf::FloatRect cb = closeButton.getGlobalBounds();
        if (cb.contains(mousePos))
        {
            close();
            return;
        }

        //购买按钮
        sf::FloatRect bb = buyButton.getGlobalBounds();
        if (bb.contains(mousePos))
        {
            if (selectedItemIndex >= 0 && selectedItemIndex < static_cast<int>(gridItems.size()))
            {
                const auto &gi = gridItems[selectedItemIndex];
                bool ok = shop->buy(*player, gi.itemId);
                if (ok)
                {
                    showFeedback = true;
                    feedbackText.setString(L"购买成功！");
                    feedbackText.setPosition(buyButton.getPosition().x, buyButton.getPosition().y + 50);
                    feedbackClock.restart();
                    //刷新商品网格
                    updateGridItems();
                }
                else
                {
                    showFeedback = true;
                    feedbackText.setString(L"金币不足，购买失败");
                    feedbackText.setPosition(buyButton.getPosition().x, buyButton.getPosition().y + 50);
                    feedbackClock.restart();
                }
            }
            return;
        }

        //网格点击
        int clickedIdx = getGridIndexFromMouse(mousePos.x, mousePos.y);
        if (clickedIdx >= 0)
        {
            selectedItemIndex = clickedIdx;
            updateDetailPanel();
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled)
    {
        gridScrollOffset += event.mouseWheelScroll.delta * 30.0f;
        float maxScroll = 0.0f;
        if (gridScrollOffset > 0)
        {
            gridScrollOffset = 0.0f;
        }

        float leftWidth = WINDOW_WIDTH * 0.7f;
        int cols = static_cast<int>((leftWidth - GRID_PADDING * 2) / (GRID_ITEM_SIZE + GRID_PADDING));
        float contentHeight = 0.0f;
        std::string curSt = "";
        int stageStartLocal = 0;
        for (int gi = 0; gi < static_cast<int>(gridItems.size()); ++gi)
        {
            if (gridItems[gi].stageLabel != curSt)
            {
                if (!curSt.empty())
                {
                    int cnt = gi - stageStartLocal;
                    int rowsUsed = (cnt + cols - 1) / cols;
                    contentHeight += rowsUsed * (GRID_ITEM_SIZE + GRID_PADDING);
                    contentHeight += (GRID_ITEM_SIZE + GRID_PADDING);
                }
                curSt = gridItems[gi].stageLabel;
                stageStartLocal = gi;
            }
        }

        if (!gridItems.empty())
        {
            int cnt = static_cast<int>(gridItems.size()) - stageStartLocal;
            int rowsUsed = (cnt + cols - 1) / cols;
            contentHeight += rowsUsed * (GRID_ITEM_SIZE + GRID_PADDING);
        }
        float panelHeight = WINDOW_HEIGHT - WINDOW_HEIGHT * 0.08f;
        if (contentHeight > panelHeight)
        {
            maxScroll = panelHeight - contentHeight;
        }
        if (gridScrollOffset < maxScroll)
        {
            gridScrollOffset = maxScroll;
        }
    }
}
