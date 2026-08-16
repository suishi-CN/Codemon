#define NOMINMAX
#include <Windows.h>
#include <cmath>
#include <algorithm>
#include <SFML/System/String.hpp>
#include <cstdint>
#include "../include/BagUI.h"
#include "../include/MoveDesc.h"
#include "../include/PathUtil.h"

//宽字符串转换函数
std::wstring BagUI::toWideString(const std::string &str)
{
    if (str.empty())
        return L"";

    //优先尝试将输入当作 UTF-8 解码（严格模式，遇到非法字节会失败）
    int req = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
    if (req > 0)
    {
        //返回长度包括终止符，所以构造时去掉末尾的 '\0'
        std::wstring wstr(req - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], req);
        return wstr;
    }

    //如果不是 UTF-8（例如源码/数据是 ANSI/GBK 编码），回退到系统 ANSI 代码页转换
    req = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (req > 0)
    {
        std::wstring wstr(req - 1, L'\0');
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], req);
        return wstr;
    }

    //最后兜底返回空宽字符串
    return L"";
}

std::size_t BagUI::computeInventoryHash() const
{
    std::size_t h = 1469598103934665603ULL;
    const auto &inv = player->bag.getInventory();
    for (const auto &p : inv)
    {
        std::uint64_t v = (static_cast<std::uint64_t>(static_cast<int>(p.first)) << 32) ^ static_cast<std::uint64_t>(p.second.second);
        h ^= static_cast<std::size_t>(v);
        h *= 1099511628211ULL;
    }
    return h;
}

//将宽字符串按字符数折行
std::wstring BagUI::wrapWideString(const std::wstring &src, std::size_t maxChars) const
{
    if (maxChars == 0)
        return src;
    std::wstring out;
    std::size_t count = 0;
    for (wchar_t ch : src)
    {
        out.push_back(ch);
        count++;
        if (count >= maxChars)
        {
            out.push_back(L'\n');
            count = 0;
        }
    }
    return out;
}

//将元素类型转换为中文字符串
std::wstring BagUI::elementToWideString(monbase::Element e) const
{
    switch (e)
    {
    case monbase::Element::None:
        return L"无";
    case monbase::Element::Normal:
        return L"普通";
    case monbase::Element::Fire:
        return L"火";
    case monbase::Element::Water:
        return L"水";
    case monbase::Element::Grass:
        return L"草";
    case monbase::Element::Electric:
        return L"电";
    case monbase::Element::Ice:
        return L"冰";
    case monbase::Element::Fighting:
        return L"格斗";
    case monbase::Element::Poison:
        return L"毒";
    case monbase::Element::Ground:
        return L"地";
    case monbase::Element::Wind:
        return L"风";
    case monbase::Element::Flying:
        return L"飞行";
    case monbase::Element::Ghost:
        return L"幽灵";
    case monbase::Element::Dark:
        return L"恶";
    case monbase::Element::Dragon:
        return L"龙";
    case monbase::Element::Cute:
        return L"可爱";
    default:
        return L"未知";
    }
}

//将技能类别转换为中文字符串
static std::wstring categoryToWideString(monbase::MoveCategory c)
{
    switch (c)
    {
    case monbase::MoveCategory::Attack:
        return L"攻击";
    case monbase::MoveCategory::Defense:
        return L"防御";
    case monbase::MoveCategory::Status:
        return L"状态";
    default:
        return L"未知";
    }
}

//将技能效果转换为中文字符串
static std::wstring effectToWideString(monbase::MoveEffect ef, int power)
{
    switch (ef)
    {
    case monbase::MoveEffect::Damage:
        return L"造成伤害";
    case monbase::MoveEffect::HealHP:
        return L"回复" + std::to_wstring(power) + L"点HP";
    case monbase::MoveEffect::HealHPPercent:
        return L"回复" + std::to_wstring(power) + L"%HP";
    case monbase::MoveEffect::RestoreEnergy:
        return L"回复" + std::to_wstring(power) + L"能量";
    case monbase::MoveEffect::BuffAttack:
        return L"物攻+" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::BuffDefense:
        return L"物防+" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::BuffMagicAttack:
        return L"魔攻+" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::BuffMagicDefense:
        return L"魔防+" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::BuffSpeed:
        return L"速度+" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::DebuffAttack:
        return L"敌物攻-" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::DebuffDefense:
        return L"敌物防-" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::DebuffMagicAttack:
        return L"敌魔攻-" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::DebuffMagicDefense:
        return L"敌魔防-" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::DebuffSpeed:
        return L"敌速度-" + std::to_wstring(power) + L"阶段";
    case monbase::MoveEffect::Shield:
        return L"护盾永久减伤" + std::to_wstring(power) + L"%";
    case monbase::MoveEffect::Dispel:
        return L"驱散敌方所有增益";
    case monbase::MoveEffect::DispelOne:
        return L"驱散敌方1种增益";
    case monbase::MoveEffect::ClearDebuffs:
        return L"清除自身减益";
    case monbase::MoveEffect::ReduceAllCost:
        return L"全技能费用-" + std::to_wstring(power);
    case monbase::MoveEffect::NextReduceCost:
        return L"下次技能费用-" + std::to_wstring(power);
    case monbase::MoveEffect::BoostAllPower:
        return L"全技能威力+" + std::to_wstring(power);
    case monbase::MoveEffect::ReduceSelfCost:
        return L"本技能费用-" + std::to_wstring(power);
    case monbase::MoveEffect::ApplyPoison:
        return L"施加" + std::to_wstring(power) + L"层中毒";
    case monbase::MoveEffect::ApplyBurn:
        return L"施加" + std::to_wstring(power) + L"层灼烧";
    case monbase::MoveEffect::ApplyFreeze:
        return L"施加" + std::to_wstring(power) + L"层冻结";
    case monbase::MoveEffect::IncreaseAllCost:
        return L"敌方全技能费用+" + std::to_wstring(power);
    case monbase::MoveEffect::EnemyLoseEnergy:
        return L"敌方损失" + std::to_wstring(power) + L"能量";
    default:
        return L"无";
    }
}

//将技能描述按指定宽度换行
static std::wstring wrapText(const std::wstring &text, size_t maxChars)
{
    if (maxChars == 0)
        return text;
    std::wstring result;
    size_t count = 0;
    for (wchar_t ch : text)
    {
        result.push_back(ch);
        ++count;
        if (count >= maxChars)
        {
            result.push_back(L'\n');
            count = 0;
        }
    }
    return result;
}

BagUI::BagUI(Player *player) : player(player)
{
    loadFont();
    setupUI();
    updateGridItems();
    updateDetailPanel();
    prevInventoryHash = computeInventoryHash();
}

void BagUI::loadFont()
{
    if (!font.loadFromFile("fonts/simhei.ttf"))
    {
        std::cerr << "警告: 无法加载字体文件 simhei.ttf！UI文本可能无法显示中文。" << std::endl;
        font = sf::Font(); //创建一个空字体，避免程序崩溃
    }
}

void BagUI::setupUI()
{
    //初始化占位符纹理
    sf::Image placeholder;
    placeholder.create(64, 64, sf::Color(200, 100, 200));
    placeholderTexture.loadFromImage(placeholder);

    //设置背景
    background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setFillColor(BACKGROUND_COLOR);
    background.setPosition(0, 0);

    //设置左侧面板
    float leftWidth = WINDOW_WIDTH * LEFT_PANEL_RATIO;
    float topBarHeight = WINDOW_HEIGHT * TOP_BAR_HEIGHT_RATIO;
    leftPanel.setSize(sf::Vector2f(leftWidth, WINDOW_HEIGHT - topBarHeight));
    leftPanel.setFillColor(PANEL_COLOR);
    leftPanel.setPosition(0, topBarHeight);

    //设置右侧面板
    float rightWidth = WINDOW_WIDTH * RIGHT_PANEL_RATIO;
    rightPanel.setSize(sf::Vector2f(rightWidth, WINDOW_HEIGHT - topBarHeight));
    rightPanel.setFillColor(PANEL_COLOR);
    rightPanel.setPosition(leftWidth, topBarHeight);

    //设置顶部横条
    topBar.setSize(sf::Vector2f(WINDOW_WIDTH, topBarHeight));
    topBar.setFillColor(sf::Color(50, 50, 50));
    topBar.setPosition(0, 0);

    //设置标签页按钮
    float buttonWidth = 120.0f;
    float buttonHeight = topBarHeight * 0.7f;
    float buttonSpacing = 10.0f;
    float startX = 20.0f;
    float startY = (topBarHeight - buttonHeight) / 2;

    std::string tabNames[] = {"道具", "精灵", "钥匙"};

    for (int i = 0; i < 3; i++)
    {
        tabButtons[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
        tabButtons[i].setFillColor(BUTTON_COLOR);
        tabButtons[i].setPosition(startX + i * (buttonWidth + buttonSpacing), startY);

        tabButtonsText[i].setFont(font);
        tabButtonsText[i].setString(toWideString(tabNames[i]));
        tabButtonsText[i].setCharacterSize(16);
        tabButtonsText[i].setFillColor(TEXT_COLOR);

        //居中文本
        sf::FloatRect textBounds = tabButtonsText[i].getLocalBounds();
        tabButtonsText[i].setOrigin(textBounds.width / 2, textBounds.height / 2);
        tabButtonsText[i].setPosition(
            tabButtons[i].getPosition().x + buttonWidth / 2,
            tabButtons[i].getPosition().y + buttonHeight / 2);
    }

    //设置关闭按钮
    float closeButtonSize = topBarHeight * 0.7f;
    closeButton.setSize(sf::Vector2f(closeButtonSize, closeButtonSize));
    closeButton.setFillColor(sf::Color(200, 50, 50));
    closeButton.setPosition(WINDOW_WIDTH - closeButtonSize - 20, startY);

    closeButtonText.setFont(font);
    closeButtonText.setString(L"X");
    closeButtonText.setCharacterSize(20);
    closeButtonText.setFillColor(TEXT_COLOR);

    sf::FloatRect closeTextBounds = closeButtonText.getLocalBounds();
    closeButtonText.setOrigin(closeTextBounds.width / 2, closeTextBounds.height / 2);
    closeButtonText.setPosition(
        closeButton.getPosition().x + closeButtonSize / 2,
        closeButton.getPosition().y + closeButtonSize / 2);

    //顶部金币与经验文本
    expText.setFont(font);
    expText.setCharacterSize(16);
    expText.setFillColor(TEXT_COLOR);
    expText.setString(L"EXP: 0");

    goldText.setFont(font);
    goldText.setCharacterSize(16);
    goldText.setFillColor(sf::Color(255, 215, 0));
    goldText.setString(L"G: 0");

    //初始放置关闭按钮文本
    closeButtonText.setPosition(
        closeButton.getPosition().x + closeButtonSize / 2,
        closeButton.getPosition().y + closeButtonSize / 2);

    //初始放置金币与经验文本
    expText.setPosition(closeButton.getPosition().x - 150, startY);
    goldText.setPosition(closeButton.getPosition().x - 70, startY);

    //设置网格视图
    gridView.setSize(sf::Vector2f(leftWidth, WINDOW_HEIGHT - topBarHeight));
    gridView.setCenter(sf::Vector2f(leftWidth / 2, (WINDOW_HEIGHT - topBarHeight) / 2));
    gridView.setViewport(sf::FloatRect(0, TOP_BAR_HEIGHT_RATIO, LEFT_PANEL_RATIO, 1 - TOP_BAR_HEIGHT_RATIO));

    //设置右侧详情面板
    float detailPadding = 20.0f;

    detailTitle.setFont(font);
    detailTitle.setCharacterSize(24);
    detailTitle.setFillColor(TEXT_COLOR);
    detailTitle.setPosition(leftWidth + detailPadding, topBarHeight + detailPadding);

    detailName.setFont(font);
    detailName.setCharacterSize(20);
    detailName.setFillColor(TEXT_COLOR);
    detailName.setPosition(leftWidth + detailPadding, topBarHeight + detailPadding + 40);

    detailCount.setFont(font);
    detailCount.setCharacterSize(16);
    detailCount.setFillColor(TEXT_COLOR);
    detailCount.setPosition(leftWidth + detailPadding, topBarHeight + detailPadding + 70);

    detailType.setFont(font);
    detailType.setCharacterSize(16);
    detailType.setFillColor(TEXT_COLOR);
    detailType.setPosition(leftWidth + detailPadding, topBarHeight + detailPadding + 100);

    detailDescription.setFont(font);
    detailDescription.setCharacterSize(14);
    detailDescription.setFillColor(TEXT_COLOR);
    detailDescription.setPosition(leftWidth + detailPadding, topBarHeight + detailPadding + 130);
    detailDescription.setLineSpacing(1.2f);

    detailPrice.setFont(font);
    detailPrice.setCharacterSize(16);
    detailPrice.setFillColor(sf::Color(255, 215, 0));
    detailPrice.setPosition(leftWidth + detailPadding, topBarHeight + detailPadding + 220);

    //设置按钮
    //上场按钮
    float buttonWidth2 = 120.0f;
    float buttonHeight2 = 40.0f;

    sellButton.setSize(sf::Vector2f(buttonWidth2, buttonHeight2));
    sellButton.setFillColor(sf::Color(200, 50, 50));
    sellButton.setPosition(leftWidth + detailPadding, WINDOW_HEIGHT - 100);

    sellButtonText.setFont(font);
    sellButtonText.setString(L"出售");
    sellButtonText.setCharacterSize(18);
    sellButtonText.setFillColor(TEXT_COLOR);

    sf::FloatRect sellTextBounds = sellButtonText.getLocalBounds();
    sellButtonText.setOrigin(sellTextBounds.width / 2, sellTextBounds.height / 2);
    sellButtonText.setPosition(
        sellButton.getPosition().x + buttonWidth2 / 2,
        sellButton.getPosition().y + buttonHeight2 / 2);

    //精灵上场按钮
    switchButton.setSize(sf::Vector2f(buttonWidth2, buttonHeight2));
    switchButton.setFillColor(sf::Color(50, 150, 50));
    switchButton.setPosition(leftWidth + detailPadding + buttonWidth2 + 20, WINDOW_HEIGHT - 100);

    switchButtonText.setFont(font);
    switchButtonText.setString(L"上场");
    switchButtonText.setCharacterSize(18);
    switchButtonText.setFillColor(TEXT_COLOR);

    sf::FloatRect switchTextBounds = switchButtonText.getLocalBounds();
    switchButtonText.setOrigin(switchTextBounds.width / 2, switchTextBounds.height / 2);
    switchButtonText.setPosition(
        switchButton.getPosition().x + buttonWidth2 / 2,
        switchButton.getPosition().y + buttonHeight2 / 2);

    //初始化精灵技能栏
    skillSlots.resize(4);
    skillText.resize(4);
    skillEquipIndicators.resize(4);

    float skillSlotSize = 60.0f;
    float skillSlotSpacing = 10.0f;
    float skillStartX = leftWidth + detailPadding;
    float skillStartY = topBarHeight + detailPadding + 240;

    for (int i = 0; i < 4; i++)
    {
        skillSlots[i].setSize(sf::Vector2f(skillSlotSize, skillSlotSize));
        skillSlots[i].setFillColor(sf::Color(80, 80, 80));
        skillSlots[i].setPosition(skillStartX + i * (skillSlotSize + skillSlotSpacing), skillStartY);

        skillText[i].setFont(font);
        skillText[i].setCharacterSize(12);
        skillText[i].setFillColor(TEXT_COLOR);
        skillText[i].setPosition(skillStartX + i * (skillSlotSize + skillSlotSpacing) + 5, skillStartY + 5);

        //装备指示器
        skillEquipIndicators[i].setSize(sf::Vector2f(15, 15));
        skillEquipIndicators[i].setFillColor(sf::Color::Transparent);
        skillEquipIndicators[i].setOutlineColor(sf::Color::White);
        skillEquipIndicators[i].setOutlineThickness(2);
        skillEquipIndicators[i].setPosition(
            skillStartX + i * (skillSlotSize + skillSlotSpacing) + skillSlotSize - 20,
            skillStartY + skillSlotSize - 20);
    }

    //初始化装备栏
    equipmentSlot.resize(4);
    equipmentSprites.resize(4);
    equipmentTexts.resize(4);

    float equipSlotSize = 80.0f;
    float equipStartX = leftWidth + detailPadding;
    //将装备栏起始位置设为技能栏底部+间隔
    float equipStartY = skillStartY + skillSlotSize + 20.0f;

    std::string equipNames[] = {"武器", "护具", "项链", "戒指"};

    for (int i = 0; i < 4; i++)
    {
        equipmentSlot[i].setSize(sf::Vector2f(equipSlotSize, equipSlotSize));
        equipmentSlot[i].setFillColor(sf::Color(60, 60, 60));
        equipmentSlot[i].setOutlineColor(sf::Color(100, 100, 100));
        equipmentSlot[i].setOutlineThickness(1);
        equipmentSlot[i].setPosition(
            equipStartX + (i % 2) * (equipSlotSize + 10),
            equipStartY + (i / 2) * (equipSlotSize + 10));

        equipmentTexts[i].setFont(font);
        equipmentTexts[i].setString(toWideString(equipNames[i])); // 使用宽字符串
        equipmentTexts[i].setCharacterSize(12);
        equipmentTexts[i].setFillColor(TEXT_COLOR);
        equipmentTexts[i].setPosition(
            equipmentSlot[i].getPosition().x + 5,
            equipmentSlot[i].getPosition().y + equipSlotSize - 20);
    }

    float upgradeBtnWidth = 80.0f;
    float upgradeBtnHeight = equipSlotSize * 2 + 10;
    upgradeButton.setSize(sf::Vector2f(upgradeBtnWidth, upgradeBtnHeight));
    upgradeButton.setFillColor(sf::Color(80, 80, 80));
    upgradeButton.setPosition(equipStartX + (2 * (equipSlotSize + 10)) + 10, equipStartY);

    upgradeButtonText.setFont(font);
    upgradeButtonText.setString(L"升级");
    upgradeButtonText.setCharacterSize(16);
    upgradeButtonText.setFillColor(TEXT_COLOR);
    sf::FloatRect upb = upgradeButtonText.getLocalBounds();
    upgradeButtonText.setOrigin(upb.width / 2, upb.height / 2);
    upgradeButtonText.setPosition(
        upgradeButton.getPosition().x + upgradeBtnWidth / 2,
        upgradeButton.getPosition().y + upgradeBtnHeight / 2);
}

void BagUI::updateGridItems()
{
    gridItems.clear();
    selectedItemIndex = -1;

    if (isSelectingEquipment)
    {
        std::cout << "更新装备选择网格，槽位: " << static_cast<int>(selectingSlot) << std::endl;

        const auto &inventory = player->bag.getInventory();
        for (const auto &pair : inventory)
        {
            auto itemPtr = ItemDB::getItem(pair.first);
            if (itemPtr && itemPtr->getType() == ItemType::EQUIPMENT)
            {
                //检查槽位是否匹配
                if (itemPtr->getSlot() == selectingSlot)
                {
                    GridItem item;
                    item.id = static_cast<int>(pair.first);
                    item.count = pair.second.second;
                    item.name = itemPtr->getName();
                    item.description = itemPtr->getDescription();
                    item.price = itemPtr->getPrice();
                    item.type = itemPtr->getType();
                    item.itemId = pair.first;

                    std::string texturePath = ItemDB::getTexturePath(pair.first);
                    if (!texturePath.empty())
                    {
                        auto it = textureCache.find(item.id);
                        if (it == textureCache.end())
                        {
                            sf::Texture tempTex;
                            if (tempTex.loadFromFile(texturePath))
                            {
                                textureCache[item.id] = std::move(tempTex);

                                item.sprite.setTexture(textureCache[item.id]);
                                item.hasTexture = true;

                                float scaleX = GRID_ITEM_SIZE / textureCache[item.id].getSize().x;
                                float scaleY = GRID_ITEM_SIZE / textureCache[item.id].getSize().y;
                                float scale = std::min(scaleX, scaleY);
                                item.sprite.setScale(scale, scale);
                            }
                        }
                        else
                        {
                            item.sprite.setTexture(it->second);
                            item.hasTexture = true;
                            float scaleX = GRID_ITEM_SIZE / it->second.getSize().x;
                            float scaleY = GRID_ITEM_SIZE / it->second.getSize().y;
                            float scale = std::min(scaleX, scaleY);
                            item.sprite.setScale(scale, scale);
                        }
                    }
                    gridItems.push_back(item);
                }
            }
        }

        if (!gridItems.empty())
        {
            gridItems[0].isSelected = true;
            selectedItemIndex = 0;
        }
        return;
    }

    switch (currentTab)
    {
    case BagTab::ITEMS:
    {
        const auto &inventory = player->bag.getInventory();

        for (const auto &pair : inventory)
        {
            GridItem item;
            item.id = static_cast<int>(pair.first);
            item.count = pair.second.second;
            item.itemId = pair.first;

            auto itemPtr = ItemDB::getItem(pair.first);
            if (itemPtr)
            {
                if (itemPtr->getType() == ItemType::KEY)
                    continue;

                item.name = itemPtr->getName();
                item.description = itemPtr->getDescription();
                item.price = itemPtr->getPrice();
                item.type = itemPtr->getType();

                std::string texturePath = ItemDB::getTexturePath(pair.first);
                if (!texturePath.empty())
                {
                    auto it = textureCache.find(item.id);
                    if (it == textureCache.end())
                    {
                        sf::Texture tempTex;
                        if (tempTex.loadFromFile(texturePath))
                        {
                            textureCache[item.id] = std::move(tempTex);
                            item.sprite.setTexture(textureCache[item.id]);
                            item.hasTexture = true;

                            float scaleX = GRID_ITEM_SIZE / textureCache[item.id].getSize().x;
                            float scaleY = GRID_ITEM_SIZE / textureCache[item.id].getSize().y;
                            float scale = std::min(scaleX, scaleY);
                            item.sprite.setScale(scale, scale);
                        }
                        else
                        {
                            item.hasTexture = false;
                        }
                    }
                    else
                    {
                        item.sprite.setTexture(it->second);
                        item.hasTexture = true;
                        float scaleX = GRID_ITEM_SIZE / it->second.getSize().x;
                        float scaleY = GRID_ITEM_SIZE / it->second.getSize().y;
                        float scale = std::min(scaleX, scaleY);
                        item.sprite.setScale(scale, scale);
                    }
                }
                else
                {
                    item.hasTexture = false;
                }
            }

            gridItems.push_back(item);
        }
        break;
    }

    case BagTab::CODEMONS:
    {
        //队伍精灵
        for (size_t i = 0; i < player->getTeamSize(); i++)
        {
            auto codemon = player->getTeamCodemon(i);
            if (codemon)
            {
                GridItem item;
                item.id = i;
                item.name = codemon->getName();
                item.codemon = codemon;
                item.isTeamSlot = true;

                int maxMoves = 4;
                for (int slot = 0; slot < maxMoves; ++slot)
                {
                    const monbase::Move *move = codemon->getEquippedMove(slot);
                    if (move)
                    {
                        item.equippedMoves.push_back(move);
                    }
                }

                const auto &unlocked = codemon->getUnlockedMoves();
                for (const auto &move : unlocked)
                {
                    item.unlockedMoves.push_back(&move);
                }

                //纹理
                std::string codemonTexturePath = "Resources/" + codemon->getName() + ".png";
                auto it = textureCache.find(codemon->getCodemonId());
                if (it == textureCache.end())
                {
                    sf::Texture tempTex;
                    if (loadTextureFromUtf8(tempTex, codemonTexturePath))
                    {
                        textureCache[codemon->getCodemonId()] = std::move(tempTex);
                        item.sprite.setTexture(textureCache[codemon->getCodemonId()]);
                        item.hasTexture = true;

                        float scaleX = GRID_ITEM_SIZE / textureCache[codemon->getCodemonId()].getSize().x;
                        float scaleY = GRID_ITEM_SIZE / textureCache[codemon->getCodemonId()].getSize().y;
                        float scale = std::min(scaleX, scaleY);
                        item.sprite.setScale(scale, scale);
                    }
                    else
                    {
                        item.sprite.setTexture(placeholderTexture);
                        item.hasTexture = true;
                        item.sprite.setScale(1.0f, 1.0f);
                    }
                }
                else
                {
                    item.sprite.setTexture(it->second);
                    item.hasTexture = true;
                    float scaleX = GRID_ITEM_SIZE / it->second.getSize().x;
                    float scaleY = GRID_ITEM_SIZE / it->second.getSize().y;
                    float scale = std::min(scaleX, scaleY);
                    item.sprite.setScale(scale, scale);
                }

                gridItems.push_back(item);
            }
        }

        //仓库精灵
        for (size_t i = 0; i < player->getStorageSize(); i++)
        {
            auto codemon = player->getStorageCodemon(i);
            if (codemon)
            {
                GridItem item;
                item.id = static_cast<int>(i + 1000);
                item.name = codemon->getName();
                item.codemon = codemon;
                item.isTeamSlot = false;

                int maxMoves = 4;
                for (int slot = 0; slot < maxMoves; ++slot)
                {
                    const monbase::Move *move = codemon->getEquippedMove(slot);
                    if (move)
                    {
                        item.equippedMoves.push_back(move);
                    }
                }

                const auto &unlocked = codemon->getUnlockedMoves();
                for (const auto &move : unlocked)
                {
                    item.unlockedMoves.push_back(&move);
                }

                //纹理
                std::string codemonTexturePath = "Resources/" + codemon->getName() + ".png";
                auto it = textureCache.find(codemon->getCodemonId());
                if (it == textureCache.end())
                {
                    sf::Texture tempTex;
                    if (loadTextureFromUtf8(tempTex, codemonTexturePath))
                    {
                        textureCache[codemon->getCodemonId()] = std::move(tempTex);
                        item.sprite.setTexture(textureCache[codemon->getCodemonId()]);
                        item.hasTexture = true;

                        float scaleX = GRID_ITEM_SIZE / textureCache[codemon->getCodemonId()].getSize().x;
                        float scaleY = GRID_ITEM_SIZE / textureCache[codemon->getCodemonId()].getSize().y;
                        float scale = std::min(scaleX, scaleY);
                        item.sprite.setScale(scale, scale);
                    }
                    else
                    {
                        item.sprite.setTexture(placeholderTexture);
                        item.hasTexture = true;
                        item.sprite.setScale(1.0f, 1.0f);
                    }
                }
                else
                {
                    item.sprite.setTexture(it->second);
                    item.hasTexture = true;
                    float scaleX = GRID_ITEM_SIZE / it->second.getSize().x;
                    float scaleY = GRID_ITEM_SIZE / it->second.getSize().y;
                    float scale = std::min(scaleX, scaleY);
                    item.sprite.setScale(scale, scale);
                }

                gridItems.push_back(item);
            }
        }
        break;
    }

    case BagTab::KEYS:
    {
        for (const auto &key : player->keyStorage)
        {
            if (key)
            {
                if (key->getType() != ItemType::KEY)
                    continue;

                GridItem item;
                item.id = static_cast<int>(key->getID());
                item.name = key->getName();
                item.description = key->getDescription();
                item.type = ItemType::KEY;
                item.count = 1;
                item.itemId = key->getID();

                std::string texturePath = ItemDB::getTexturePath(key->getID());
                if (!texturePath.empty())
                {
                    auto it = textureCache.find(item.id);
                    if (it == textureCache.end())
                    {
                        sf::Texture tempTex;
                        if (tempTex.loadFromFile(texturePath))
                        {
                            textureCache[item.id] = std::move(tempTex);
                            item.sprite.setTexture(textureCache[item.id]);
                            item.hasTexture = true;

                            float scaleX = GRID_ITEM_SIZE / textureCache[item.id].getSize().x;
                            float scaleY = GRID_ITEM_SIZE / textureCache[item.id].getSize().y;
                            float scale = std::min(scaleX, scaleY);
                            item.sprite.setScale(scale, scale);
                        }
                        else
                        {
                            item.hasTexture = false;
                        }
                    }
                    else
                    {
                        item.sprite.setTexture(it->second);
                        item.hasTexture = true;
                        float scaleX = GRID_ITEM_SIZE / it->second.getSize().x;
                        float scaleY = GRID_ITEM_SIZE / it->second.getSize().y;
                        float scale = std::min(scaleX, scaleY);
                        item.sprite.setScale(scale, scale);
                    }
                }
                else
                {
                    item.hasTexture = false;
                }

                gridItems.push_back(item);
            }
        }
        break;
    }

    case BagTab::SKILLS:
    {
        if (skillTargetCodemon)
        {
            const auto &unlockedMoves = skillTargetCodemon->getUnlockedMoves();
            const auto &equippedIndices = skillTargetCodemon->getEquippedIndices();

            std::vector<std::pair<const monbase::Move *, int>> availableMoves;

            //遍历所有已解锁的技能
            for (size_t idx = 0; idx < unlockedMoves.size(); ++idx)
            {
                const auto &move = unlockedMoves[idx];

                //检查技能是否已经被装备
                bool alreadyEquipped = false;
                for (int equippedIdx : equippedIndices)
                {
                    if (equippedIdx == static_cast<int>(idx))
                    {
                        alreadyEquipped = true;
                        break;
                    }
                }

                //添加未装备的技能
                if (!alreadyEquipped)
                {
                    availableMoves.push_back({&move, static_cast<int>(idx)});
                }
            }

            if (availableMoves.empty())
            {
                GridItem placeholder;
                placeholder.id = -1;
                placeholder.name = "(无可用技能)";
                placeholder.unlockedMoveIndex = -1;
                gridItems.push_back(placeholder);
            }
            else
            {
                for (const auto &p : availableMoves)
                {
                    GridItem item;
                    item.id = static_cast<int>(gridItems.size());
                    item.name = p.first->name;
                    if (!p.first->description.empty())
                        item.description = p.first->description;
                    else
                    {
                        auto it = moveDescriptions.find(p.first->name);
                        item.description = (it != moveDescriptions.end()) ? it->second : p.first->name;
                    }
                    item.unlockedMoveIndex = p.second;
                    gridItems.push_back(item);
                }
            }
        }
        break;
    }
    }

    if (!gridItems.empty() && selectedItemIndex == -1 && currentTab != BagTab::SKILLS)
    {
        gridItems[0].isSelected = true;
        selectedItemIndex = 0;
    }
}

void BagUI::updateDetailPanel()
{
    for (int i = 0; i < 4; i++)
    {
        equipmentSprites[i].setTexture(placeholderTexture);
        equipmentSprites[i].setPosition(-1000, -1000);
    }

    //技能标签页：显示技能详情
    if (currentTab == BagTab::SKILLS)
    {
        detailTitle.setString(L"选择技能");
        if (skillTargetCodemon)
        {
            detailName.setString(L"为 " + toWideString(skillTargetCodemon->getName()) +
                                 L" 选择第" + std::to_wstring(skillTargetSlot + 1) + L" 个技能");
        }
        else
        {
            detailName.setString(L"请先选择精灵");
        }
        //查找悬停或选中的技能
        int hoveredIdx = -1;
        for (size_t i = 0; i < gridItems.size(); ++i)
        {
            if (gridItems[i].isHovered)
            {
                hoveredIdx = static_cast<int>(i);
                break;
            }
        }
        int displayIdx = (hoveredIdx >= 0) ? hoveredIdx : selectedItemIndex;
        if (displayIdx >= 0 && displayIdx < static_cast<int>(gridItems.size()))
        {
            //从availableMoves获取对应的Move指针
            const monbase::Move *movePtr = nullptr;
            if (skillTargetCodemon)
            {
                const auto &unlockedMoves = skillTargetCodemon->getUnlockedMoves();
                int moveIdx = gridItems[displayIdx].unlockedMoveIndex;
                if (moveIdx >= 0 && moveIdx < static_cast<int>(unlockedMoves.size()))
                    movePtr = &unlockedMoves[moveIdx];
            }
            if (movePtr)
            {
                std::wstring info;
                info += L"元素：" + elementToWideString(movePtr->element) + L"\n";
                info += L"类别：" + categoryToWideString(movePtr->category) + L"\n";
                info += L"类型：" + std::wstring(movePtr->isPhysical ? L"物理" : L"魔法") + L"\n";
                info += L"威力：" + std::to_wstring(movePtr->power) + L"\n";
                info += L"费用：" + std::to_wstring(movePtr->energyCost) + L"\n";
                info += L"主效果：" + effectToWideString(movePtr->effect, movePtr->power) + L"\n";
                std::wstring descW = toWideString(gridItems[displayIdx].description);
                info += L"介绍：" + wrapText(descW, 15) + L"\n";

                //成长机制
                std::wstring growth;
                if (movePtr->selfScalingBonus > 0)
                    growth += L"自成长+" + std::to_wstring(movePtr->selfScalingBonus) + L" ";
                if (movePtr->scalingBonus > 0)
                    growth += L"同元素成长+" + std::to_wstring(movePtr->scalingBonus) + L" ";
                if (movePtr->exponentScaling > 0)
                    growth += L"指数型成长 ";
                if (movePtr->costSelfReduce > 0)
                    growth += L"自费用缩减-" + std::to_wstring(movePtr->costSelfReduce) + L" ";
                if (movePtr->costToPowerBonus > 0)
                    growth += L"费用转威力+" + std::to_wstring(movePtr->costToPowerBonus) + L" ";
                if (movePtr->turnEndPowerBonus > 0)
                    growth += L"回合结束成长+" + std::to_wstring(movePtr->turnEndPowerBonus) + L" ";
                if (growth.empty())
                    growth = L"无";
                info += L"成长机制：" + growth + L"\n";

                //应对
                if (movePtr->isCounter)
                {
                    std::wstring counterInfo = L"应对" + categoryToWideString(movePtr->counterTarget) + L" ";
                    counterInfo += effectToWideString(movePtr->counterEffect, movePtr->counterPower);
                    info += L"应对：" + counterInfo + L"\n";
                }
                else
                {
                    info += L"应对：无\n";
                }

                detailDescription.setString(info);
            }
            else
            {
                detailDescription.setString(L"技能: " + toWideString(gridItems[displayIdx].name));
            }
        }
        else
        {
            detailDescription.setString(L"");
        }
        detailSprite.setTexture(placeholderTexture);
        detailCount.setString(L"");
        detailType.setString(L"");
        detailPrice.setString(L"");
        return;
    }

    //装备选择模式：显示提示，隐藏无关UI
    if (isSelectingEquipment)
    {
        detailTitle.setString(L"选择装备");
        std::wstring slotName;
        switch (selectingSlot)
        {
        case EquipSlot::WEAPON:
            slotName = L"武器";
            break;
        case EquipSlot::ARMOR:
            slotName = L"护具";
            break;
        case EquipSlot::NECKLACE:
            slotName = L"项链";
            break;
        case EquipSlot::RING:
            slotName = L"戒指";
            break;
        default:
            slotName = L"未知";
            break;
        }
        detailName.setString(L"为 " + slotName + L" 槽位选择装备");
        detailDescription.setString(L"请从左侧列表中选择可装备的道具");
        detailSprite.setTexture(placeholderTexture);
        detailCount.setString(L"");
        detailType.setString(L"");
        detailPrice.setString(L"");
        return;
    }

    //没有任何选中项时的处理
    if (selectedItemIndex < 0 || selectedItemIndex >= static_cast<int>(gridItems.size()))
    {
        //当处于道具标签页且背包内无道具时，不显示占位贴图，改为显示提示文本
        if (currentTab == BagTab::ITEMS && gridItems.empty())
        {
            showDetailSprite = false;
            detailTitle.setString(L"");
            detailName.setString(L"");
            detailCount.setString(L"");
            detailType.setString(L"");
            detailPrice.setString(L"");

            //在贴图原位置显示提示文本
            sf::Vector2f msgPos = sf::Vector2f(rightPanel.getPosition().x + 20, topBar.getSize().y + 60);
            detailDescription.setPosition(msgPos);
            detailDescription.setString(L"当前背包内无道具");
        }
        else if (currentTab == BagTab::KEYS && gridItems.empty())
        {
            showDetailSprite = false;
            detailTitle.setString(L"");
            detailName.setString(L"");
            detailCount.setString(L"");
            detailType.setString(L"");
            detailPrice.setString(L"");
            sf::Vector2f msgPos = sf::Vector2f(rightPanel.getPosition().x + 20, topBar.getSize().y + 60);
            detailDescription.setPosition(msgPos);
            detailDescription.setString(L"当前背包内无钥匙");
        }
        else
        {
            //其他情况恢复默认占位贴图并清空文本
            showDetailSprite = true;
            detailSprite.setTexture(placeholderTexture);
            detailTitle.setString(L"");
            detailName.setString(L"");
            detailCount.setString(L"");
            detailType.setString(L"");
            detailDescription.setString(L"");
            detailPrice.setString(L"");
        }
        return;
    }

    const GridItem &item = gridItems[selectedItemIndex];

    //更新标题，并重置详情文本位置
    detailTitle.setString(toWideString(item.name));
    showDetailSprite = false;

    //根据不同标签页填充具体文本
    if (currentTab == BagTab::CODEMONS && item.codemon)
    {
        //精灵标签页
        detailName.setString(L"名称：" + toWideString(item.name) + L" Lv." + std::to_wstring(item.codemon->getLevel()));
        detailCount.setString(L"");
        detailType.setString(L"类型：" + elementToWideString(item.codemon->getType1()) +
                             (item.codemon->getType2() != monbase::Element::None ? L"/" + elementToWideString(item.codemon->getType2()) : L""));
        detailDescription.setString(L"描述：");
        detailPrice.setString(L"");

        //重置详情文本位置
        {
            float x = detailTitle.getPosition().x;
            float y = detailTitle.getPosition().y;
            float spacing = 6.0f;
            sf::FloatRect tb = detailTitle.getLocalBounds();
            float curY = y + tb.height + spacing;

            detailName.setPosition(x, curY);
            sf::FloatRect nb = detailName.getLocalBounds();
            curY += nb.height + spacing;

            detailCount.setPosition(x, curY);
            sf::FloatRect cb = detailCount.getLocalBounds();
            curY += cb.height + spacing;

            detailType.setPosition(x, curY);
            sf::FloatRect tyb = detailType.getLocalBounds();
            curY += tyb.height + spacing;

            detailDescription.setPosition(x, curY);
            sf::FloatRect db = detailDescription.getLocalBounds();
            curY += db.height + spacing;

            detailPrice.setPosition(x, curY);
        }

        //更新技能栏文本
        for (int i = 0; i < 4; i++)
        {
            const monbase::Move *move = item.codemon->getEquippedMove(i);
            if (move)
            {
                std::string shortName = move->name;
                //截断过长字符串
                if (shortName.length() > 12)
                {
                    shortName = shortName.substr(0, 12);
                }
                std::wstring wname = toWideString(shortName);
                std::wstring wrapped = wrapWideString(wname, 2);
                skillText[i].setString(wrapped);
                skillEquipIndicators[i].setFillColor(sf::Color::White);
            }
            else
            {
                skillText[i].setString(L"空");
                skillEquipIndicators[i].setFillColor(sf::Color::Transparent);
            }
        }

        //更新装备栏贴图
        const auto &equippedItems = item.codemon->getEquippedItems();

        //清空所有装备槽位的精灵纹理
        for (int i = 0; i < 4; i++)
        {
            equipmentSprites[i].setTexture(placeholderTexture);
            equipmentSprites[i].setPosition(-1000, -1000);
        }

        //遍历已装备的物品，设置对应槽位的贴图
        for (const auto &pair : equippedItems)
        {
            EquipSlot slot = pair.first;
            ItemID itemId = pair.second;

            int slotIndex = -1;
            switch (slot)
            {
            case EquipSlot::WEAPON:
                slotIndex = 0;
                break;
            case EquipSlot::ARMOR:
                slotIndex = 1;
                break;
            case EquipSlot::NECKLACE:
                slotIndex = 2;
                break;
            case EquipSlot::RING:
                slotIndex = 3;
                break;
            default:
                continue;
            }

            //获取纹理路径
            std::string texturePath = ItemDB::getTexturePath(itemId);
            if (texturePath.empty())
            {
                //如果没有路径，使用占位符
                equipmentSprites[slotIndex].setTexture(placeholderTexture);
            }
            else
            {
                //从缓存获取
                auto it = textureCache.find(static_cast<int>(itemId));
                if (it != textureCache.end())
                {
                    equipmentSprites[slotIndex].setTexture(it->second, true);
                }
                else
                {
                    sf::Texture tempTex;
                    if (tempTex.loadFromFile(texturePath))
                    {
                        //存入缓存
                        textureCache[static_cast<int>(itemId)] = std::move(tempTex);
                        sf::Texture &texRef = textureCache[static_cast<int>(itemId)];
                        equipmentSprites[slotIndex].setTexture(texRef, true);
                    }
                    else
                    {
                        //加载失败，使用占位符
                        std::cerr << "装备纹理加载失败: " << texturePath << std::endl;
                        equipmentSprites[slotIndex].setTexture(placeholderTexture, true);
                    }
                }
            }

            //处理缩放和居中
            const sf::Texture *tex = equipmentSprites[slotIndex].getTexture();
            if (tex && tex->getSize().x > 0)
            {
                float slotSizeValue = 80.0f;
                float scaleX = slotSizeValue / static_cast<float>(tex->getSize().x);
                float scaleY = slotSizeValue / static_cast<float>(tex->getSize().y);
                float scale = std::min(scaleX, scaleY);

                equipmentSprites[slotIndex].setScale(scale, scale);
                equipmentSprites[slotIndex].setOrigin(0.f, 0.f);

                sf::Vector2f slotPos = equipmentSlot[slotIndex].getPosition();
                sf::Vector2f slotSizeVec = equipmentSlot[slotIndex].getSize();

                //计算居中位置
                float centerX = slotPos.x + slotSizeVec.x / 2.0f;
                float centerY = slotPos.y + slotSizeVec.y / 2.0f;
                float spriteWidth = tex->getSize().x * scale;
                float spriteHeight = tex->getSize().y * scale;

                equipmentSprites[slotIndex].setPosition(
                    centerX - spriteWidth / 2.0f,
                    centerY - spriteHeight / 2.0f);
            }
        }
        int needExp = item.codemon->getXpToNextLevel() - item.codemon->getXpProgress();
        upgradeEnabled = (needExp > 0 && player->getExp() >= needExp);
    }
    else
    {
        //道具或钥匙标签页
        detailName.setString(L"名称：" + toWideString(item.name));
        detailCount.setString(L"数量：" + std::to_wstring(item.count));
        std::wstring typeStr;
        if (item.type == ItemType::CONSUMABLE)
        {
            typeStr = L"消耗品";
        }
        else if (item.type == ItemType::EQUIPMENT)
        {
            typeStr = L"装备";
        }
        else
        {
            typeStr = L"钥匙";
        }
        detailType.setString(L"类型：" + typeStr);
        //自动换行，18个字符一行
        std::wstring desc = toWideString(item.description);
        detailDescription.setString(L"描述：\n" + wrapWideString(desc, 18));
        //售价在道具标签页显示
        if (currentTab == BagTab::ITEMS)
        {
            detailPrice.setString(L"售价 " + std::to_wstring(item.price / 2) + L" 金币");
        }
        else
        {
            detailPrice.setString(L"");
        }

        float x = detailTitle.getPosition().x;
        float y = detailTitle.getPosition().y;
        float spacing = 6.0f;
        sf::FloatRect tb = detailTitle.getLocalBounds();
        float curY = y + tb.height + spacing;

        detailName.setPosition(x, curY);
        sf::FloatRect nb = detailName.getLocalBounds();
        curY += nb.height + spacing;

        detailCount.setPosition(x, curY);
        sf::FloatRect cb = detailCount.getLocalBounds();
        curY += cb.height + spacing;

        detailType.setPosition(x, curY);
        sf::FloatRect tyb = detailType.getLocalBounds();
        curY += tyb.height + spacing;

        detailDescription.setPosition(x, curY);
        sf::FloatRect db = detailDescription.getLocalBounds();
        curY += db.height + spacing;

        detailPrice.setPosition(x, curY);
    }
}

void BagUI::drawGrid(sf::RenderWindow &window)
{
    //获取左侧面板的绝对位置和大小
    sf::Vector2f panelPos = leftPanel.getPosition();
    sf::Vector2f panelSize = leftPanel.getSize();

    //计算网格的起始位置
    float startX = panelPos.x + 20.0f;
    float startY = panelPos.y + 40.0f;

    if (currentTab == BagTab::SKILLS)
    {
        //绘制技能列表背景
        sf::RectangleShape listBackground;
        listBackground.setSize(sf::Vector2f(panelSize.x - 40, panelSize.y - 40));
        listBackground.setFillColor(sf::Color(50, 50, 50));
        listBackground.setPosition(startX, panelPos.y + 40);
        window.draw(listBackground);

        //绘制技能列表项
        float listStartY = 20.0f;
        float listItemHeight = SKILL_LIST_ITEM_HEIGHT;
        float listItemWidth = panelSize.x - 40;

        for (size_t i = 0; i < gridItems.size(); i++)
        {
            float itemY = listStartY + i * listItemHeight - skillListScrollOffset;

            //只绘制可见的列表项
            if (itemY + listItemHeight >= 0 && itemY <= panelSize.y - 40)
            {
                //绘制列表项背景
                sf::RectangleShape itemBg(sf::Vector2f(listItemWidth, listItemHeight - 5));
                itemBg.setFillColor(gridItems[i].isSelected ? SELECTED_COLOR : (gridItems[i].isHovered ? sf::Color(100, 100, 120) : sf::Color(70, 70, 70)));
                itemBg.setPosition(startX, panelPos.y + 40 + itemY);
                window.draw(itemBg);

                //绘制技能名称
                sf::Text skillName;
                skillName.setFont(font);
                skillName.setString(toWideString(gridItems[i].name));
                skillName.setCharacterSize(18);
                skillName.setFillColor(TEXT_COLOR);
                skillName.setPosition(startX + 50, panelPos.y + 40 + itemY + 10);
                window.draw(skillName);

                //绘制技能描述
                sf::Text skillDesc;
                skillDesc.setFont(font);
                std::wstring descW = toWideString(gridItems[i].description);
                skillDesc.setString(descW);
                skillDesc.setCharacterSize(12);
                skillDesc.setFillColor(sf::Color(180, 180, 180));
                skillDesc.setPosition(startX + 50, panelPos.y + 40 + itemY + 35);
                window.draw(skillDesc);
            }
        }

        //绘制滚动条
        if (gridItems.size() > SKILL_LIST_MAX_VISIBLE_ITEMS)
        {
            float totalHeight = gridItems.size() * listItemHeight;
            float visibleHeight = panelSize.y - 40;
            float scrollBarHeight = visibleHeight * (visibleHeight / totalHeight);
            float scrollBarY = (-skillListScrollOffset / totalHeight) * visibleHeight;

            sf::RectangleShape scrollBar;
            scrollBar.setSize(sf::Vector2f(10, scrollBarHeight));
            scrollBar.setFillColor(sf::Color(100, 100, 100));
            scrollBar.setPosition(panelPos.x + panelSize.x - 30, panelPos.y + 40 + scrollBarY);
            window.draw(scrollBar);
        }
    }
    else
    {
        //正常网格绘制
        int columns = 6;
        float teamSlotSize = 80.0f;

        if (currentTab == BagTab::CODEMONS)
        {
            //绘制队伍槽位
            float teamStartX = startX;
            float teamStartY = startY;

            for (int i = 0; i < 6; i++)
            {
                sf::RectangleShape teamSlot(sf::Vector2f(teamSlotSize, teamSlotSize));
                teamSlot.setFillColor(i == selectedTeamSlot ? SELECTED_COLOR : PANEL_COLOR);
                teamSlot.setPosition(teamStartX + i * (teamSlotSize + 10), teamStartY);
                window.draw(teamSlot);

                if (i < static_cast<int>(player->getTeamSize()))
                {
                    auto codemon = player->getTeamCodemon(i);
                    if (codemon)
                    {
                        sf::Sprite sprite;
                        std::string codemonTexturePath = "Resources/" + codemon->getName() + ".png";

                        //从缓存获取纹理
                        auto it = textureCache.find(codemon->getCodemonId());
                        if (it == textureCache.end())
                        {
                            sf::Texture texture;
                            if (loadTextureFromUtf8(texture, codemonTexturePath))
                            {
                                textureCache[codemon->getCodemonId()] = std::move(texture);
                                sprite.setTexture(textureCache[codemon->getCodemonId()]);
                            }
                            else
                            {
                                sprite.setTexture(placeholderTexture);
                            }
                        }
                        else
                        {
                            sprite.setTexture(it->second);
                        }

                        //计算缩放
                        float scaleX = teamSlotSize / sprite.getTexture()->getSize().x;
                        float scaleY = teamSlotSize / sprite.getTexture()->getSize().y;
                        float scale = std::min(scaleX, scaleY);
                        sprite.setScale(scale, scale);

                        //居中显示
                        float spriteWidth = sprite.getTexture()->getSize().x * scale;
                        float spriteHeight = sprite.getTexture()->getSize().y * scale;
                        float offsetX = (teamSlotSize - spriteWidth) / 2;
                        float offsetY = (teamSlotSize - spriteHeight) / 2;

                        sprite.setPosition(teamStartX + i * (teamSlotSize + 10) + offsetX, teamStartY + offsetY);
                        window.draw(sprite);
                    }
                }
            }

            //仓库精灵网格
            startY += teamSlotSize + 20;
            columns = 6;
        }

        //绘制网格项
        for (size_t i = 0; i < gridItems.size(); i++)
        {
            int col = i % columns;
            int row = i / columns;

            float x = startX + col * (GRID_ITEM_SIZE + GRID_PADDING);
            float y = startY + row * (GRID_ITEM_SIZE + GRID_PADDING) + gridScrollOffset;

            //绘制可见的项
            if (y + GRID_ITEM_SIZE >= panelPos.y && y <= panelPos.y + panelSize.y)
            {
                sf::RectangleShape itemBg(sf::Vector2f(GRID_ITEM_SIZE, GRID_ITEM_SIZE));
                if (gridItems[i].isSelected)
                    itemBg.setFillColor(SELECTED_COLOR);
                else if (gridItems[i].isHovered)
                    itemBg.setFillColor(BUTTON_HOVER_COLOR);
                else
                    itemBg.setFillColor(PANEL_COLOR);
                itemBg.setPosition(x, y);
                window.draw(itemBg);

                //绘制精灵/物品
                if (gridItems[i].hasTexture)
                {
                    sf::Sprite &sprite = gridItems[i].sprite;

                    //确保精灵有正确的缩放
                    if (sprite.getScale().x == 0 || sprite.getScale().y == 0)
                    {
                        const sf::Texture *texture = sprite.getTexture();
                        if (texture && texture->getSize().x > 0)
                        {
                            float scaleX = GRID_ITEM_SIZE / texture->getSize().x;
                            float scaleY = GRID_ITEM_SIZE / texture->getSize().y;
                            float scale = std::min(scaleX, scaleY);
                            sprite.setScale(scale, scale);
                        }
                    }

                    //计算精灵在格子内的居中位置
                    sf::FloatRect spriteBounds = sprite.getLocalBounds();
                    float spriteWidth = spriteBounds.width * sprite.getScale().x;
                    float spriteHeight = spriteBounds.height * sprite.getScale().y;
                    float offsetX = (GRID_ITEM_SIZE - spriteWidth) / 2;
                    float offsetY = (GRID_ITEM_SIZE - spriteHeight) / 2;

                    sprite.setPosition(x + offsetX, y + offsetY);
                    window.draw(sprite);
                }
                else
                {
                    if (currentTab != BagTab::CODEMONS)
                    {
                        sf::RectangleShape placeholder(sf::Vector2f(GRID_ITEM_SIZE - 16, GRID_ITEM_SIZE - 16));
                        placeholder.setFillColor(sf::Color(100, 100, 100));
                        placeholder.setPosition(x + 8, y + 8);
                        window.draw(placeholder);
                    }
                }

                //绘制名称
                sf::Text nameText;
                nameText.setFont(font);
                nameText.setString(toWideString(gridItems[i].name));
                nameText.setCharacterSize(12);
                nameText.setFillColor(TEXT_COLOR);
                nameText.setPosition(x + 5, y + GRID_ITEM_SIZE - 25);
                window.draw(nameText);

                //绘制数量
                if (gridItems[i].count > 0)
                {
                    sf::Text countText;
                    countText.setFont(font);
                    countText.setString(std::to_wstring(gridItems[i].count));
                    countText.setCharacterSize(14);
                    countText.setFillColor(sf::Color::White);
                    countText.setPosition(x + 4, y + 4);
                    window.draw(countText);
                }
            }
        }
    }
}

void BagUI::drawDetailPanel(sf::RenderWindow &window)
{
    //绘制右侧面板背景
    window.draw(rightPanel);

    if (currentTab == BagTab::SKILLS)
    {
        window.draw(detailTitle);
        window.draw(detailDescription);
        return;
    }

    if (isSelectingEquipment)
    {
        sf::Text selectHint;
        selectHint.setFont(font);
        selectHint.setString(L"请从左侧选择装备");
        selectHint.setCharacterSize(20);
        selectHint.setFillColor(TEXT_COLOR);
        selectHint.setPosition(rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 40);
        window.draw(selectHint);
        return;
    }

    //绘制详情文本
    window.draw(detailTitle);
    if (showDetailSprite)
    {
        window.draw(detailSprite);
    }
    window.draw(detailName);
    window.draw(detailCount);
    window.draw(detailType);
    window.draw(detailDescription);
    if (currentTab == BagTab::ITEMS)
    {
        window.draw(detailPrice);
    }

    if (currentTab == BagTab::CODEMONS)
    {
        //绘制技能栏
        for (int i = 0; i < 4; i++)
        {
            window.draw(skillSlots[i]);
            window.draw(skillText[i]);
        }

        //绘制装备栏
        for (int i = 0; i < 4; i++)
        {
            window.draw(equipmentSlot[i]);

            equipmentSprites[i].setColor(sf::Color::White);
            sf::Vector2f scale = equipmentSprites[i].getScale();

            window.draw(equipmentSprites[i]);
            window.draw(equipmentTexts[i]);
        }
        //上场按钮只在仓库精灵被选中时显示
        if (selectedItemIndex >= static_cast<int>(player->getTeamSize()))
        {
            window.draw(switchButton);
            window.draw(switchButtonText);
        }
        //升级按钮显示
        if (upgradeEnabled)
        {
            upgradeButton.setFillColor(sf::Color(100, 200, 100));
            upgradeButtonText.setFillColor(TEXT_COLOR);
        }
        else
        {
            upgradeButton.setFillColor(sf::Color(80, 80, 80));
            upgradeButtonText.setFillColor(sf::Color(150, 150, 150));
        }
        window.draw(upgradeButton);
        window.draw(upgradeButtonText);
    }

    //仅道具标签页显示出售按钮
    if (currentTab == BagTab::ITEMS)
    {
        window.draw(sellButton);
        window.draw(sellButtonText);
    }
}

//计算网格索引
int BagUI::getGridIndexFromMouse(float mouseX, float mouseY, float startX, float startY, int cols, float padding, float iconSize)
{
    float adjustedY = mouseY - startY - gridScrollOffset;

    int col = static_cast<int>(std::floor((mouseX - startX) / (iconSize + padding)));
    int row = static_cast<int>(std::floor(adjustedY / (iconSize + padding)));

    if (col >= 0 && col < cols && row >= 0)
    {
        int index = row * cols + col;
        if (index < static_cast<int>(gridItems.size()))
        {
            return index;
        }
    }
    return -1;
}

void BagUI::handleMouseClick(sf::Vector2f mousePos, sf::Mouse::Button button)
{
    //获取左侧面板的绝对位置和大小
    sf::Vector2f panelPos = leftPanel.getPosition();
    sf::Vector2f panelSize = leftPanel.getSize();

    //计算网格的起始位置
    float startX = panelPos.x + 20.0f;
    float startY = panelPos.y + 40.0f;

    //保存当前选中状态
    int previousSelectedIndex = selectedItemIndex;
    int previousSelectedTeamSlot = selectedTeamSlot;

    //检查标签页按钮
    for (int i = 0; i < 3; i++)
    {
        if (tabButtons[i].getGlobalBounds().contains(mousePos))
        {
            switchToTab(static_cast<BagTab>(i));
            return;
        }
    }

    //检查关闭按钮
    if (closeButton.getGlobalBounds().contains(mousePos))
    {
        isVisible = false;
        return;
    }

    //检查网格项点击
    if (!isSelectingEquipment && currentTab != BagTab::SKILLS && leftPanel.getGlobalBounds().contains(mousePos))
    {
        int columns = 6;
        float teamSlotSize = 80.0f;

        if (currentTab == BagTab::CODEMONS)
        {
            //检查队伍槽位
            for (int i = 0; i < 6; i++)
            {
                sf::FloatRect teamSlot(
                    startX + i * (teamSlotSize + 10),
                    startY,
                    teamSlotSize,
                    teamSlotSize);

                if (teamSlot.contains(mousePos))
                {
                    selectedTeamSlot = i;
                    //右键卸下队伍精灵，队伍仅剩一只时禁用卸下
                    if (button == sf::Mouse::Right)
                    {
                        if (currentTab == BagTab::CODEMONS && i < static_cast<int>(player->getTeamSize()))
                        {
                            if (player->getTeamSize() > 1)
                            {
                                //将队伍索引i的精灵存入仓库
                                player->depositToStorage(i);
                                updateGridItems();
                                updateDetailPanel();

                                if (previousSelectedTeamSlot >= static_cast<int>(player->getTeamSize()))
                                    selectedTeamSlot = previousSelectedTeamSlot - 1;
                                else
                                    selectedTeamSlot = previousSelectedTeamSlot;
                            }
                            else
                            {
                                std::cout << "无法卸下：队伍中至少需保留一只精灵。" << std::endl;
                            }
                        }
                        return;
                    }

                    //选择队伍槽位
                    if (button == sf::Mouse::Left)
                    {
                        for (auto &item : gridItems)
                            item.isSelected = false;
                        if (i < static_cast<int>(gridItems.size()))
                        {
                            gridItems[i].isSelected = true;
                            selectedItemIndex = i;
                            updateDetailPanel();
                        }
                    }
                    return;
                }
            }

            //仓库精灵网格
            startY += teamSlotSize + 20;
            columns = 6;
        }

        //计算点击的网格项
        int index = getGridIndexFromMouse(mousePos.x, mousePos.y, startX, startY, columns, GRID_PADDING, GRID_ITEM_SIZE);
        if (index != -1)
        {
            //清除悬停标记
            for (auto &item : gridItems)
            {
                item.isHovered = false;
            }
            //设置选中项并刷新详情
            for (auto &item : gridItems)
            {
                item.isSelected = false;
            }
            gridItems[index].isSelected = true;
            selectedItemIndex = index;
            updateDetailPanel();
        }
    }

    //检查右侧面板点击
    if (rightPanel.getGlobalBounds().contains(mousePos))
    {
        //出售按钮
        if (sellButton.getGlobalBounds().contains(mousePos))
        {
            //检查selectedItemIndex是否有效
            if (selectedItemIndex >= 0 && selectedItemIndex < static_cast<int>(gridItems.size()))
            {
                GridItem &item = gridItems[selectedItemIndex];
                if (item.count > 0)
                {
                    ItemID idToRemove = item.itemId;
                    if (idToRemove == static_cast<ItemID>(0))
                    {
                        idToRemove = static_cast<ItemID>(item.id);
                    }

                    //从背包移除物品
                    if (player->bag.removeItem(idToRemove, 1))
                    {
                        std::cout << "出售成功: " << item.name << std::endl;

                        //获得金币
                        player->earnGold(item.price / 2);

                        item.count--;

                        if (item.count <= 0)
                        {
                            gridItems.erase(gridItems.begin() + selectedItemIndex);
                            if (gridItems.empty())
                                selectedItemIndex = -1;
                            else if (selectedItemIndex >= static_cast<int>(gridItems.size()))
                                selectedItemIndex = gridItems.size() - 1;
                            else
                                selectedItemIndex = previousSelectedIndex;
                        }
                        else
                        {
                            selectedItemIndex = previousSelectedIndex;
                        }

                        //强制刷新
                        updateGridItems();
                        updateDetailPanel();

                        if (selectedItemIndex >= 0 && selectedItemIndex < static_cast<int>(gridItems.size()))
                        {
                            gridItems[selectedItemIndex].isSelected = true;
                        }
                    }
                    else
                    {
                        std::cout << "出售失败: 背包中没有该物品" << std::endl;
                    }
                }
            }
            return;
        }

        //上场按钮
        if (switchButton.getGlobalBounds().contains(mousePos))
        {
            if (currentTab == BagTab::CODEMONS && selectedItemIndex >= player->getTeamSize())
            {
                int storageIndex = selectedItemIndex - player->getTeamSize();
                player->withdrawToTeam(storageIndex);
                updateGridItems();
                updateDetailPanel();
                selectedItemIndex = player->getTeamSize() - 1;
                selectedTeamSlot = selectedItemIndex;
            }
            return;
        }

        //升级按钮处理
        if (upgradeButton.getGlobalBounds().contains(mousePos) && button == sf::Mouse::Left)
        {
            if (currentTab == BagTab::CODEMONS && upgradeEnabled && selectedItemIndex >= 0 && selectedItemIndex < static_cast<int>(gridItems.size()))
            {
                GridItem &git = gridItems[selectedItemIndex];
                if (git.codemon)
                {
                    monbase::Codemon *cod = git.codemon;
                    int need = cod->getXpToNextLevel() - cod->getXpProgress();
                    if (need > 0 && player->getExp() >= need)
                    {
                        player->addExp(-need);
                        cod->addExperience(need);
                        updateGridItems();
                        updateDetailPanel();
                        selectedItemIndex = previousSelectedIndex;
                        selectedTeamSlot = previousSelectedTeamSlot;
                    }
                }
            }
            return;
        }

        //检查技能栏
        for (int i = 0; i < 4; i++)
        {
            if (skillSlots[i].getGlobalBounds().contains(mousePos))
            {
                if (currentTab == BagTab::CODEMONS)
                {
                    monbase::Codemon *codemon = nullptr;
                    if (selectedItemIndex >= 0 && selectedItemIndex < static_cast<int>(gridItems.size()))
                        codemon = gridItems[selectedItemIndex].codemon;
                    if (!codemon && player->getTeamSize() > 0)
                        codemon = player->getTeamCodemon(0);
                    if (!codemon)
                        return;

                    //确保装备槽位存在
                    while (static_cast<int>(codemon->getEquippedIndices().size()) <= i &&
                           codemon->getEquippedCount() < 4)
                    {
                        codemon->addEquippedIndex(0);
                    }

                    //使用左键进入技能选择模式
                    if (button == sf::Mouse::Left)
                    {
                        skillTargetCodemon = codemon;
                        skillTargetSlot = i;
                        switchToTab(BagTab::SKILLS);
                    }
                }
                return;
            }
        }

        //检查装备栏
        for (int i = 0; i < 4; i++)
        {
            if (equipmentSlot[i].getGlobalBounds().contains(mousePos))
            {
                if (currentTab == BagTab::CODEMONS && selectedItemIndex < static_cast<int>(gridItems.size()))
                {
                    GridItem &selectedItem = gridItems[selectedItemIndex];
                    monbase::Codemon *codemon = selectedItem.codemon;

                    if (codemon)
                    {
                        EquipSlot slot = static_cast<EquipSlot>(i);

                        //检查该槽位是否已装备物品
                        const auto &equippedItems = codemon->getEquippedItems();
                        auto it = equippedItems.find(slot);
                        if (it != equippedItems.end())
                        {
                            //右键卸下装备
                            if (button == sf::Mouse::Right)
                            {
                                ItemID itemId = it->second;
                                auto item = ItemDB::getItem(itemId);
                                if (item)
                                {
                                    item->unequip(codemon);
                                    player->bag.addItem(std::move(item->clone()));
                                    std::cout << "卸下装备:" << item->getName() << std::endl;
                                    updateGridItems();
                                    updateDetailPanel();
                                    selectedItemIndex = previousSelectedIndex;
                                    selectedTeamSlot = previousSelectedTeamSlot;
                                }
                            }
                        }
                        else
                        {
                            //左键选择装备
                            if (button == sf::Mouse::Left)
                            {
                                //选择装备槽位
                                int codemonIndex = selectedItem.isTeamSlot ? selectedItemIndex : (selectedItemIndex - static_cast<int>(player->getTeamSize()));
                                startEquipmentSelection(codemonIndex, slot, selectedItem.isTeamSlot);
                            }
                        }
                    }
                }
                return;
            }
        }
    }

    //检查技能列表
    if (currentTab == BagTab::SKILLS && leftPanel.getGlobalBounds().contains(mousePos))
    {
        float listStartY = leftPanel.getPosition().y + 40.0f;
        float adjustedY = mousePos.y - listStartY - skillListScrollOffset;
        int clickedIndex = static_cast<int>(adjustedY / SKILL_LIST_ITEM_HEIGHT);

        if (clickedIndex >= 0 && clickedIndex < static_cast<int>(gridItems.size()))
        {
            const GridItem &clickedItem = gridItems[clickedIndex];
            if (clickedItem.unlockedMoveIndex == -1)
            {
                return;
            }

            if (skillTargetCodemon && skillTargetSlot >= 0)
            {
                //验证技能索引有效性
                const auto &unlockedMoves = skillTargetCodemon->getUnlockedMoves();
                if (clickedItem.unlockedMoveIndex < 0 ||
                    clickedItem.unlockedMoveIndex >= static_cast<int>(unlockedMoves.size()))
                {
                    std::cout << "技能索引无效！索引: " << clickedItem.unlockedMoveIndex
                              << ", 技能总数: " << unlockedMoves.size() << std::endl;
                    return;
                }

                //验证技能指针有效性
                const monbase::Move *selectedMove = &unlockedMoves[clickedItem.unlockedMoveIndex];
                if (!selectedMove || selectedMove->name.empty())
                {
                    std::cout << "技能指针无效！" << std::endl;
                    return;
                }

                //使用存储的技能索引装备
                if (skillTargetCodemon->swapEquippedMove(skillTargetSlot, clickedItem.unlockedMoveIndex))
                {
                    std::cout << "装备技能成功: " << clickedItem.name << std::endl;
                    switchToTab(BagTab::CODEMONS);
                    skillTargetCodemon = nullptr;
                    skillTargetSlot = -1;
                    selectedItemIndex = previousSelectedIndex;
                    selectedTeamSlot = previousSelectedTeamSlot;
                }
                else
                {
                    std::cout << "装备技能失败" << std::endl;
                }
            }
        }
        return;
    }

    //检查装备选择
    if (isSelectingEquipment && leftPanel.getGlobalBounds().contains(mousePos))
    {
        handleEquipmentSelection(mousePos);
        selectedItemIndex = previousSelectedIndex;
        selectedTeamSlot = previousSelectedTeamSlot;
        return;
    }
}

void BagUI::handleMouseWheel(float delta)
{
    if (currentTab == BagTab::SKILLS)
    {
        skillListScrollOffset -= delta * SKILL_LIST_ITEM_HEIGHT;

        float totalHeight = gridItems.size() * SKILL_LIST_ITEM_HEIGHT;
        float visibleHegiht = leftPanel.getSize().y - 40.0f;

        if (totalHeight > visibleHegiht)
        {
            skillListScrollOffset = std::max(skillListScrollOffset, -(totalHeight - visibleHegiht));
            skillListScrollOffset = std::min(skillListScrollOffset, 0.0f);
        }
        else
        {
            skillListScrollOffset = 0.0f;
        }
    }
    else
    {
        gridScrollOffset += delta * 20;

        int totalRows = (gridItems.size() + 5) / 6;
        float totalHeight = totalRows * (GRID_ITEM_SIZE + GRID_PADDING);
        float visibleHeight = leftPanel.getSize().y - 20;

        if (totalHeight > visibleHeight)
        {
            gridScrollOffset = std::max(gridScrollOffset, -(totalHeight - visibleHeight));
            gridScrollOffset = std::min(gridScrollOffset, 0.0f);
        }
        else
        {
            gridScrollOffset = 0.0f;
        }
    }
}

void BagUI::switchToTab(BagTab tab)
{
    if (currentTab == BagTab::SKILLS)
    {
        skillTargetCodemon = nullptr;
        skillTargetSlot = -1;
    }
    if (tab != BagTab::ITEMS)
    {
        isSelectingEquipment = false;
    }
    currentTab = tab;
    selectedItemIndex = (tab == BagTab::SKILLS) ? -1 : 0;
    selectedTeamSlot = -1;
    gridScrollOffset = 0.0f;
    skillListScrollOffset = 0.0f;

    for (int i = 0; i < 3; i++)
    {
        tabButtons[i].setFillColor(i == static_cast<int>(tab) ? SELECTED_COLOR : BUTTON_COLOR);
    }

    updateGridItems();
    updateDetailPanel();
}

void BagUI::handleEvent(sf::Event &event, sf::RenderWindow &window)
{
    if (!isVisible)
    {
        return;
    }

    ownerWindow = &window;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            //将窗口像素坐标转换为与UI绘制一致的坐标
            sf::Vector2i pix(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f world = window.mapPixelToCoords(pix, window.getDefaultView());
            handleMouseClick(world, sf::Mouse::Left);
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            sf::Vector2i pix(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f world = window.mapPixelToCoords(pix, window.getDefaultView());
            handleMouseClick(world, sf::Mouse::Right);
        }
        break;

    case sf::Event::MouseWheelScrolled:
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            handleMouseWheel(event.mouseWheelScroll.delta);
        }
        break;

    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape)
        {
            if (currentTab == BagTab::SKILLS)
            {
                //从技能标签页返回精灵标签页
                switchToTab(BagTab::CODEMONS);
                skillTargetCodemon = nullptr;
                skillTargetSlot = -1;
            }
            else if (isSelectingEquipment)
            {
                isSelectingEquipment = false;
                switchToTab(BagTab::CODEMONS);
            }
            else
            {
                isVisible = false;
            }
        }
        break;

    default:
        break;
    }
}

void BagUI::update(sf::RenderWindow *window)
{
    //如果从不可见到可见，强制刷新数据
    if (isVisible && !prevVisible)
    {
        updateGridItems();
        updateDetailPanel();
        prevVisible = true;
    }

    //如果当前不可见，重置prevVisible并直接返回
    if (!isVisible)
    {
        prevVisible = false;
        return;
    }

    //更新顶部金币与经验显示
    std::wstring gw = std::wstring(L"金币: ") + std::to_wstring(player->getGold());
    std::wstring ew = std::wstring(L"经验: ") + std::to_wstring(player->getExp());
    goldText.setString(sf::String(gw));
    expText.setString(sf::String(ew));

    //根据字符串宽度调整位置，确保关闭按钮位于最右侧
    float closeX = closeButton.getPosition().x;
    float closeW = closeButton.getSize().x;

    sf::FloatRect goldBounds = goldText.getLocalBounds();
    sf::FloatRect expBounds = expText.getLocalBounds();

    //右对齐到关闭按钮左侧
    float padding = 10.0f;
    goldText.setPosition(closeX - padding - goldBounds.width, closeButton.getPosition().y);
    expText.setPosition(goldText.getPosition().x - padding - expBounds.width, closeButton.getPosition().y);

    //检测背包内容是否发生变化
    std::size_t currentHash = computeInventoryHash();
    if (currentHash != prevInventoryHash)
    {
        prevInventoryHash = currentHash;
        updateGridItems();
        updateDetailPanel();
    }

    //获取当前鼠标位置
    sf::Vector2f mousePos;
    sf::RenderWindow *useWindow = window ? window : ownerWindow;
    if (useWindow)
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*useWindow);
        mousePos = useWindow->mapPixelToCoords(pixelPos, useWindow->getDefaultView());
    }
    else
    {
        //退回到屏幕坐标
        sf::Vector2i p = sf::Mouse::getPosition();
        mousePos = sf::Vector2f(static_cast<float>(p.x), static_cast<float>(p.y));
    }

    //获取面板位置
    sf::Vector2f panelPos = leftPanel.getPosition();
    sf::Vector2f panelSize = leftPanel.getSize();

    //计算网格的起始位置
    float startX = panelPos.x + 20.0f;
    float startY = panelPos.y + 40.0f;

    //更新标签页按钮的悬停效果
    for (int i = 0; i < 3; i++)
    {
        if (tabButtons[i].getGlobalBounds().contains(mousePos))
        {
            if (i != static_cast<int>(currentTab))
            {
                tabButtons[i].setFillColor(BUTTON_HOVER_COLOR);
            }
        }
        else
        {
            if (i == static_cast<int>(currentTab))
            {
                tabButtons[i].setFillColor(SELECTED_COLOR);
            }
            else
            {
                tabButtons[i].setFillColor(BUTTON_COLOR);
            }
        }
    }

    //更新关闭按钮的悬停效果
    if (closeButton.getGlobalBounds().contains(mousePos))
    {
        closeButton.setFillColor(sf::Color(255, 100, 100));
    }
    else
    {
        closeButton.setFillColor(sf::Color(200, 50, 50));
    }

    //更新出售按钮的悬停效果
    if (currentTab == BagTab::ITEMS || currentTab == BagTab::KEYS)
    {
        if (sellButton.getGlobalBounds().contains(mousePos))
        {
            sellButton.setFillColor(sf::Color(255, 100, 100));
        }
        else
        {
            sellButton.setFillColor(sf::Color(200, 50, 50));
        }
    }

    //更新上场按钮的悬停效果
    if (currentTab == BagTab::CODEMONS)
    {
        if (switchButton.getGlobalBounds().contains(mousePos))
        {
            switchButton.setFillColor(sf::Color(100, 200, 100));
        }
        else
        {
            switchButton.setFillColor(sf::Color(50, 150, 50));
        }
    }

    //更新技能栏的悬停效果
    if (currentTab == BagTab::CODEMONS && selectedItemIndex >= 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (skillSlots[i].getGlobalBounds().contains(mousePos))
            {
                skillSlots[i].setOutlineColor(sf::Color::Yellow);
                skillSlots[i].setOutlineThickness(3);
            }
            else
            {
                skillSlots[i].setOutlineColor(sf::Color(100, 100, 100));
                skillSlots[i].setOutlineThickness(1);
            }
        }
    }

    //更新装备栏的悬停效果
    if (currentTab == BagTab::CODEMONS && selectedItemIndex >= 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (equipmentSlot[i].getGlobalBounds().contains(mousePos))
            {
                equipmentSlot[i].setOutlineColor(sf::Color::Yellow);
                equipmentSlot[i].setOutlineThickness(3);
            }
            else
            {
                equipmentSlot[i].setOutlineColor(sf::Color(100, 100, 100));
                equipmentSlot[i].setOutlineThickness(1);
            }
        }
    }

    //技能列表悬停效果
    if (currentTab == BagTab::SKILLS && leftPanel.getGlobalBounds().contains(mousePos))
    {
        sf::Vector2f panelPos = leftPanel.getPosition();
        sf::Vector2f panelSize = leftPanel.getSize();
        float startX = panelPos.x + 20.0f;
        float listStartY = 20.0f;
        float listItemHeight = SKILL_LIST_ITEM_HEIGHT;
        float listItemWidth = panelSize.x - 40;

        for (auto &item : gridItems)
            item.isHovered = false;

        for (size_t i = 0; i < gridItems.size(); i++)
        {
            float itemY = listStartY + i * listItemHeight - skillListScrollOffset;
            sf::FloatRect itemRect(startX, panelPos.y + 40 + itemY, listItemWidth, listItemHeight - 5);
            if (itemRect.contains(mousePos))
            {
                gridItems[i].isHovered = true;
                updateDetailPanel();
                break;
            }
        }
    }

    //更新左侧网格的悬停效果
    if (leftPanel.getGlobalBounds().contains(mousePos) && currentTab != BagTab::SKILLS)
    {
        int columns = 6;
        float teamSlotSize = 80.0f;

        if (currentTab == BagTab::CODEMONS)
        {
            //检查队伍槽位悬停
            for (int i = 0; i < 6; i++)
            {
                sf::FloatRect teamSlot(
                    startX + i * (teamSlotSize + 10),
                    startY,
                    teamSlotSize,
                    teamSlotSize);

                if (teamSlot.contains(mousePos))
                {
                    selectedTeamSlot = i;
                    //仅更新悬停状态，不改变选中项或详情面板
                    for (auto &item : gridItems)
                        item.isHovered = false;
                    if (i < static_cast<int>(gridItems.size()))
                    {
                        gridItems[i].isHovered = true;
                    }
                }
            }

            //仓库精灵网格
            startY += teamSlotSize + 20;
            columns = 6;
        }

        //计算悬停的网格项
        int index = getGridIndexFromMouse(mousePos.x, mousePos.y, startX, startY, columns, GRID_PADDING, GRID_ITEM_SIZE);
        if (index != -1)
        {
            for (auto &item : gridItems)
                item.isHovered = false;
            gridItems[index].isHovered = true;
        }
    }
}

void BagUI::render(sf::RenderWindow &window)
{
    if (!isVisible)
    {
        return;
    }

    //使用默认视图
    window.setView(window.getDefaultView());

    window.draw(background);
    window.draw(topBar);
    window.draw(leftPanel);

    //绘制标签页按钮
    for (int i = 0; i < 3; i++)
    {
        window.draw(tabButtons[i]);
        window.draw(tabButtonsText[i]);
    }

    //绘制关闭按钮
    window.draw(closeButton);
    window.draw(closeButtonText);
    //绘制顶部金币与经验
    window.draw(expText);
    window.draw(goldText);

    //绘制网格
    drawGrid(window);

    //绘制详情面板
    drawDetailPanel(window);
}

void BagUI::handleEquipmentSlotClick(sf::Vector2f mousePos, int slotIndex)
{
    if (selectedItemIndex < 0 || selectedItemIndex >= static_cast<int>(gridItems.size()))
    {
        return;
    }

    const GridItem &selectedGridItem = gridItems[selectedItemIndex];
    if (!selectedGridItem.codemon)
    {
        return;
    }

    monbase::Codemon *targetCodemon = selectedGridItem.codemon;
    EquipSlot slot = static_cast<EquipSlot>(slotIndex);

    //检查该槽位是否已装备物品
    bool isEquipped = false;
    ItemID equippedItemId = static_cast<ItemID>(0);

    const auto &equippedItems = targetCodemon->getEquippedItems();
    auto it = equippedItems.find(slot);
    if (it != equippedItems.end())
    {
        isEquipped = true;
        equippedItemId = it->second;
    }

    if (isEquipped)
    {
        unequipItemFromCodemon(targetCodemon, slot, equippedItemId);
    }
    else
    {
        startEquipmentSelection(selectedItemIndex, slot, selectedGridItem.isTeamSlot);
    }
}

void BagUI::handleEquipmentSelection(sf::Vector2f mousePos)
{
    if (!isSelectingEquipment)
        return;

    //获取左侧面板的绝对位置和大小
    sf::Vector2f panelPos = leftPanel.getPosition();
    sf::Vector2f panelSize = leftPanel.getSize();

    //计算网格的起始位置
    float startX = panelPos.x + 20.0f;
    float startY = panelPos.y + 40.0f;

    float adjustedY = mousePos.y - startY - gridScrollOffset;
    int columns = (currentTab == BagTab::CODEMONS) ? 5 : 6;

    int col = static_cast<int>((mousePos.x - startX) / (GRID_ITEM_SIZE + GRID_PADDING));
    int row = static_cast<int>(adjustedY / (GRID_ITEM_SIZE + GRID_PADDING));

    if (col >= 0 && col < columns && row >= 0)
    {
        int index = row * columns + col;
        if (index < static_cast<int>(gridItems.size()))
        {
            GridItem &selectedItem = gridItems[index];
            if (selectedItem.type == ItemType::EQUIPMENT)
            {
                auto item = ItemDB::getItem(selectedItem.itemId);
                if (item && item->getSlot() == selectingSlot)
                {
                    monbase::Codemon *targetCodemon = nullptr;
                    if (isTeamCodemon)
                    {
                        targetCodemon = player->getTeamCodemon(selectingCodemonIndex);
                    }
                    else
                    {
                        targetCodemon = player->getStorageCodemon(selectingCodemonIndex);
                    }

                    if (targetCodemon)
                    {
                        equipItemToCodemon(targetCodemon, item.get(), selectedItem.itemId);
                    }
                }
                else
                {
                    std::cout << "装备选择失败: 物品槽位不匹配，期望: "
                              << static_cast<int>(selectingSlot)
                              << " 实际: " << static_cast<int>(item ? item->getSlot() : EquipSlot::NONE)
                              << std::endl;
                }
            }
        }
    }
}

void BagUI::startEquipmentSelection(int codemonIndex, EquipSlot slot, bool isTeam)
{
    selectingCodemonIndex = codemonIndex;
    selectingSlot = slot;
    isTeamCodemon = isTeam;
    equipmentAction = EquipmentAction::SELECTING_EQUIPMENT;
    isSelectingEquipment = true;

    //切换到道具背包标签页
    switchToTab(BagTab::ITEMS);
}

void BagUI::cancelEquipmentSelection()
{
    isSelectingEquipment = false;
    selectingSlot = EquipSlot::NONE;
    selectingCodemonIndex = -1;
    isTeamCodemon = false;
    switchToTab(BagTab::CODEMONS);
}

void BagUI::equipItemToCodemon(monbase::Codemon *codemon, Item *item, ItemID itemId)
{
    if (!codemon || !item)
    {
        std::cout << "装备失败: 精灵或物品为空" << std::endl;
        return;
    }

    std::cout << "尝试装备: " << item->getName() << " 到精灵: " << codemon->getName() << std::endl;

    //卸下该槽位的旧装备
    const auto &equippedItems = codemon->getEquippedItems();
    auto it = equippedItems.find(selectingSlot);
    if (it != equippedItems.end())
    {
        ItemID oldItemId = it->second;
        auto oldItem = ItemDB::getItem(oldItemId);
        if (oldItem)
        {
            oldItem->unequip(codemon);
            player->bag.addItem(std::move(oldItem->clone()));
            std::cout << "卸下旧装备: " << oldItem->getName() << std::endl;
        }
    }

    //装备新物品
    if (item->equip(codemon))
    {
        //从背包移除物品
        if (player->bag.removeItem(itemId, 1))
        {
            std::cout << "成功装备: " << item->getName() << std::endl;
        }
        else
        {
            std::cout << "装备成功但从背包移除失败" << std::endl;
        }
    }
    else
    {
        std::cout << "装备失败" << std::endl;
    }

    //取消装备选择状态，返回精灵背包
    cancelEquipmentSelection();
    //更新背包快照
    prevInventoryHash = computeInventoryHash();
}

void BagUI::unequipItemFromCodemon(monbase::Codemon *codemon, EquipSlot slot, ItemID itemId)
{
    if (!codemon)
    {
        return;
    }

    auto item = ItemDB::getItem(itemId);
    if (!item)
    {
        return;
    }

    //卸下装备
    if (item->unequip(codemon))
    {
        //将装备放回背包
        player->bag.addItem(std::move(item->clone()));
        std::cout << "成功卸下:" << item->getName() << std::endl;
    }

    updateDetailPanel();
    updateGridItems();
    prevInventoryHash = computeInventoryHash();
}

bool BagUI::isValidSkill(const monbase::Codemon *codemon, int skillIndex) const
{
    if (!codemon)
    {
        return false;
    }

    const auto &unlockedMoves = codemon->getUnlockedMoves();
    if (skillIndex < 0 || skillIndex >= static_cast<int>(unlockedMoves.size()))
    {
        return false;
    }

    const monbase::Move *move = &unlockedMoves[skillIndex];
    if (!move)
    {
        return false;
    }

    //检查技能名称是否有效
    if (move->name.empty())
    {
        return false;
    }

    return true;
}