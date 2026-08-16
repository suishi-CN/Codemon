#include "spritePokedex.h"
#include "backpack.h"
#include "MoveDesc.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <filesystem>

// 元素名称
const wchar_t* SpritePokedex::kElemNames[16] = {
    L"无", L"普通", L"火", L"水", L"草", L"电", L"冰", L"武",
    L"毒", L"地", L"风", L"翼", L"幽", L"恶", L"龙", L"萌"
};

// 全部 45 个精灵数据
const SpritePokedex::SpeciesInfo SpritePokedex::kSpecies[45] = {
    // ID 1-3: 吉米进化链
    {L"吉米",      4, 0, L"光合作用：草系攻击后回复10%HP",           65,66,49,66,91,33},
    {L"耄耋",      4, 0, L"光合作用：草系攻击后回复10%HP",           86,87,65,87,121,44},
    {L"胖猫",      4, 0, L"光合作用：草系攻击后回复10%HP",           108,109,81,109,151,55},
    // ID 4-6: 火花进化链
    {L"火花",      2, 0, L"烈焰之魂：火系攻击后物攻+2、魔攻+2",     70,84,56,37,43,78},
    {L"焰火",      2, 0, L"烈焰之魂：火系攻击后物攻+2、魔攻+2",     93,111,75,49,58,109},
    {L"火神",      2, 0, L"烈焰之魂：火系攻击后物攻+2、魔攻+2",     117,139,94,61,72,130},
    // ID 7-9: 水蓝蓝进化链
    {L"水蓝蓝",    3, 0, L"滋润：水系攻击后全技能能耗-1",            75,35,56,76,79,51},
    {L"波波拉",    3, 0, L"滋润：水系攻击后全技能能耗-1",            100,46,75,102,106,68},
    {L"水灵",      3, 0, L"滋润：水系攻击后全技能能耗-1",            125,58,94,127,132,85},
    // ID 10-11: 海豹战士进化链
    {L"海豹战士",  7, 3, L"斗志：己方每成功应对一次，入场时双攻+20%", 72,91,89,87,62,80},
    {L"海豹船长",  7, 3, L"斗志：己方每成功应对一次，入场时双攻+20%", 90,113,111,109,77,100},
    // ID 12-14: 乖乖鹄进化链
    {L"乖乖鹄",    11,3, L"传承：离场时给队友加双攻",                 75,57,83,52,58,69},
    {L"蓝珠天鹅",  11,3, L"传承：离场时给队友加双攻",                 100,76,110,69,78,92},
    {L"翠顶夫人",  11,3, L"传承：离场时给队友加双攻",                 125,95,138,86,97,115},
    // ID 15-17: 大耳帽兜进化链
    {L"大耳帽兜",  6, 0, L"捉迷藏：使敌方冻结时全技能+1费",           78,62,39,59,78,54},
    {L"帽兜娃娃",  6, 0, L"捉迷藏：使敌方冻结时全技能+1费",           104,83,52,78,104,72},
    {L"雪影娃娃",  6, 0, L"捉迷藏：使敌方冻结时全技能+1费",           130,103,66,98,130,90},
    // ID 18-20: 画精灵进化链
    {L"画精灵",    1, 7, L"变形活画：敌方每层增益+10%威力",            61,78,56,32,72,72},
    {L"画像守护",  1, 7, L"变形活画：敌方每层增益+10%威力",            81,104,75,42,96,96},
    {L"画间沉铁兽",1, 7, L"变形活画：敌方每层增益+10%威力",            100,160,100,67,76,105},
    // ID 21-22: 小星光进化链
    {L"小星光",    5, 0, L"电流刺激：迸发·攻击技能威力+40",           64,76,72,85,96,100},
    {L"星光狮",    5, 0, L"电流刺激：迸发·攻击技能威力+40",           80,95,90,107,120,125},
    // ID 23-24: 梦游进化链
    {L"梦游",      12,0, L"做噩梦：应对成功时敌失去3能量",             90,52,60,125,74,92},
    {L"梦悠悠",    12,0, L"做噩梦：应对成功时敌失去3能量",             113,65,75,157,93,115},
    // ID 25-27: 阿米亚特进化链
    {L"阿米亚特",  9, 0, L"盛宴：能量不足时5%HP代替1能量",             64,95,67,47,42,45},
    {L"阿米樱",    9, 0, L"盛宴：能量不足时5%HP代替1能量",             86,127,89,62,56,60},
    {L"罗隐",      9, 13,L"盛宴：能量不足时5%HP代替1能量",             107,159,112,78,70,75},
    // ID 28-29: 大头骨龙进化链
    {L"大头骨龙",  14,12,L"不朽：首次低于50%HP时回满",                 96,109,84,40,65,48},
    {L"寂灭骨龙",  14,12,L"不朽：首次低于50%HP时回满",                 120,137,104,50,81,60},
    // ID 30-31: 胆小鳗鱼进化链
    {L"胆小鳗鱼",  5, 3, L"生物电：电系技能迸发时能耗-2",              68,92,83,92,90,92},
    {L"闪电鳗鱼",  5, 3, L"生物电：电系技能迸发时能耗-2",              85,115,104,115,112,115},
    // ID 32-33: 翡翠水母进化链
    {L"翡翠水母",  3, 8, L"扩散侵蚀：水系技能后中毒层数翻倍",          108,30,75,68,109,76},
    {L"琉璃水母",  3, 8, L"扩散侵蚀：水系技能后中毒层数翻倍",          134,37,93,85,136,95},
    // ID 34: 深蓝鲸
    {L"深蓝鲸",    3, 0, L"倾轧：所有能耗变化效果翻倍",                129,40,119,115,135,50},
    // ID 35-37: 水滴蛇进化链
    {L"水滴蛇",    3, 7, L"思维之盾：应对成功后下次技能能耗-5",        63,66,62,62,62,63},
    {L"水蛇锁",    3, 7, L"思维之盾：应对成功后下次技能能耗-5",        84,88,83,83,83,84},
    {L"游蛇魔使",  3, 7, L"思维之盾：应对成功后下次技能能耗-5",        105,110,103,104,103,105},
    // ID 38-39: 加油海葵进化链
    {L"加油海葵",  3, 15,L"拉拉队长：回合结束敌方双攻-20%，自身双攻+20%",93,87,102,87,102,76},
    {L"加油蟹",    3, 15,L"拉拉队长：回合结束敌方双攻-20%，自身双攻+20%",116,108,128,108,128,95},
    // ID 40-42: 小狮鹫进化链
    {L"小狮鹫",    11,0, L"乘风连击：翼系技能后全技能+20威力",         64,70,76,41,39,72},
    {L"神圣狮鹫",  11,0, L"乘风连击：翼系技能后全技能+20威力",         85,93,102,55,52,96},
    {L"皇家狮鹫",  11,0, L"乘风连击：翼系技能后全技能+20威力",         107,116,127,69,65,120},
    // ID 43-45: 可爱猿进化链
    {L"可爱猿",    2, 0, L"散热：初始能量为5",                          100,97,86,43,67,36},
    {L"炽热猿",    2, 0, L"散热：初始能量为5",                          133,130,115,57,89,48},
    {L"火焰猿",    2, 0, L"散热：初始能量为5",                          167,162,143,72,111,60},
};

// 加载字体

bool SpritePokedex::loadFont(const std::string& fontPath) {
    return font.loadFromFile(fontPath);
}

// 纹理加载

void SpritePokedex::loadTextures() {
    for (int i = 0; i < 45; i++) {
        // Use std::filesystem::path which handles Windows encoding correctly
        std::filesystem::path p = L"Resources/";
        p += kSpecies[i].name;
        p += L".png";

        if (!std::filesystem::exists(p))
            continue;

        // .string() on Windows returns system ANSI (CP_ACP / GBK on Chinese Windows)
        if (textures[i].loadFromFile(p.string()))
            textureLoaded[i] = true;
    }
}

// 技能池加载

// Simple UTF-8 to wstring converter (no windows.h dependency)
static std::wstring Utf8ToWide(const std::string& s) {
    std::wstring out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = (unsigned char)s[i];
        wchar_t wc;
        if (c <= 0x7F) { wc = (wchar_t)c; i += 1; }
        else if (c >= 0xC2 && c <= 0xDF && i + 1 < s.size()) { wc = (wchar_t)(((c & 0x1F) << 6) | ((unsigned char)s[i+1] & 0x3F)); i += 2; }
        else if (c >= 0xE0 && c <= 0xEF && i + 2 < s.size()) { wc = (wchar_t)(((c & 0x0F) << 12) | (((unsigned char)s[i+1] & 0x3F) << 6) | ((unsigned char)s[i+2] & 0x3F)); i += 3; }
        else { wc = L'?'; i += 1; }
        out += wc;
    }
    return out;
}

void SpritePokedex::loadSkillPools() {
    for (int i = 0; i < 45; i++) {
        speciesMovePools[i].clear();
        auto cm = Backpack::create(i + 1, 100);
        if (!cm) continue;
        for (auto& entry : cm->getSpeciesMovePool()) {
            std::wstring wname = Utf8ToWide(entry.move.name);
            std::wstring wdesc;
            auto it = moveDescriptions.find(entry.move.name);
            if (it != moveDescriptions.end())
                wdesc = Utf8ToWide(it->second);
            speciesMovePools[i].push_back({entry.learnLevel, wname, wdesc});
        }
    }
}

// 事件处理

void SpritePokedex::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!isVisible) return;

    // ESC key: back up one level
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (showSkills) { showSkills = false; return; }
        if (detailIndex >= 0) { detailIndex = -1; return; }
        isVisible = false;
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos(
            static_cast<float>(event.mouseButton.x),
            static_cast<float>(event.mouseButton.y)
        );

        if (showSkills) {
            // Skills overlay: "← 返回" button + click outside closes
            float px = (kW - 900) / 2.0f;
            float py = (kH - 640) / 2.0f;
            sf::FloatRect backBtn(px + 20, py + 15, 90, 32);
            if (backBtn.contains(mousePos)) { showSkills = false; return; }
            // Click outside the skills panel → close
            sf::FloatRect panelRect(px, py, 900, 640);
            if (!panelRect.contains(mousePos)) { showSkills = false; return; }

            // Skills pagination
            int idx = detailIndex;
            if (idx >= 0 && idx < 45) {
                int total = (int)speciesMovePools[idx].size();
                int totalPages = (total + 9) / 10;  // 10 per page
                float btnY = py + 640 - 48;
                if (skillPage > 0 && sf::FloatRect(px + 60, btnY, 100, 30).contains(mousePos))
                    { skillPage--; return; }
                if (skillPage < totalPages - 1 && sf::FloatRect(px + 740, btnY, 100, 30).contains(mousePos))
                    { skillPage++; return; }
            }
            return;
        }

        if (detailIndex >= 0) {
            // Detail mode
            float px = (kW - 900) / 2.0f;
            float py = (kH - 640) / 2.0f;
            sf::FloatRect backBtn(px + 20, py + 15, 90, 32);
            if (backBtn.contains(mousePos)) { detailIndex = -1; return; }

            // "技能" button (below type badges, generous hit area)
            float nameY = py + 60 + 280 + 20;
            sf::FloatRect skillBtn(px + 40, nameY + 80, 80, 50);
            if (skillBtn.contains(mousePos)) { showSkills = true; skillPage = 0; return; }
        } else {
            // Gallery mode

            // "Back to Game" button (top-left)
            sf::FloatRect backBtn(30, 22, 110, 36);
            if (backBtn.contains(mousePos)) {
                isVisible = false;
                return;
            }

            // Calculate grid start X (centered)
            float gridW = COLS * CARD_W + (COLS - 1) * CARD_GAP_X;
            float startX = (kW - gridW) / 2.0f;

            int startIdx = currentPage * PER_PAGE;
            int endIdx = std::min(startIdx + PER_PAGE, 45);

            // Hit-test each card
            for (int i = startIdx; i < endIdx; i++) {
                int local = i - startIdx;
                int col = local % COLS;
                int row = local / COLS;
                float cx = startX + col * (CARD_W + CARD_GAP_X);
                float cy = GRID_START_Y + row * (CARD_H + CARD_GAP_Y);

                sf::FloatRect cardRect(cx, cy, CARD_W, CARD_H);
                if (cardRect.contains(mousePos)) {
                    detailIndex = i;
                    return;
                }
            }

            // Pagination buttons - bottom corners with wrap-around
            float btnY = kH - 70;
            int totalPages = (45 + PER_PAGE - 1) / PER_PAGE;

            // Previous page (bottom-left)
            sf::FloatRect prevBtn(40, btnY, 100, 36);
            if (prevBtn.contains(mousePos)) {
                currentPage = (currentPage == 0) ? totalPages - 1 : currentPage - 1;
                return;
            }
            // Next page (bottom-right)
            sf::FloatRect nextBtn(kW - 140, btnY, 100, 36);
            if (nextBtn.contains(mousePos)) {
                currentPage = (currentPage == totalPages - 1) ? 0 : currentPage + 1;
                return;
            }
        }
    }
}

// 主绘制入口

void SpritePokedex::draw(sf::RenderWindow& window) {
    if (!isVisible) return;
    window.setView(window.getDefaultView());

    if (showSkills)
        drawSkills(window);
    else if (detailIndex >= 0)
        drawDetail(window);
    else
        drawGallery(window);
}

// 画廊模式

void SpritePokedex::drawGallery(sf::RenderWindow& window) {
    // 半透明背景 overlay
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(kW), static_cast<float>(kH)));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    overlay.setPosition(0, 0);
    window.draw(overlay);

    // Main panel
    sf::RectangleShape panel(sf::Vector2f(static_cast<float>(kW) - 40, static_cast<float>(kH) - 40));
    panel.setFillColor(sf::Color(240, 242, 248));
    panel.setOutlineColor(sf::Color(180, 190, 210));
    panel.setOutlineThickness(2);
    panel.setPosition(20, 20);
    window.draw(panel);

    // Title (centered top area)
    sf::Text title(L"精灵图鉴", font, 28);
    title.setFillColor(sf::Color(30, 40, 60));
    sf::FloatRect titleB = title.getLocalBounds();
    title.setOrigin(titleB.left + titleB.width / 2, titleB.top + titleB.height / 2);
    title.setPosition(kW / 2, 40);
    window.draw(title);

    // 返回按钮
    sf::FloatRect backBtn(30, 22, 110, 36);
    sf::RectangleShape bBtn(sf::Vector2f(backBtn.width, backBtn.height));
    bBtn.setFillColor(sf::Color(100, 120, 150));
    bBtn.setPosition(backBtn.left, backBtn.top);
    window.draw(bBtn);
    sf::Text bText(L"← 返回游戏", font, 16);
    bText.setFillColor(sf::Color::White);
    sf::FloatRect btb = bText.getLocalBounds();
    bText.setOrigin(btb.left + btb.width / 2, btb.top + btb.height / 2);
    bText.setPosition(backBtn.left + backBtn.width / 2, backBtn.top + backBtn.height / 2);
    window.draw(bText);

    // Sprite card grid
    float gridW = COLS * CARD_W + (COLS - 1) * CARD_GAP_X;
    float startX = (kW - gridW) / 2.0f;
    float startY = GRID_START_Y;

    int startIdx = currentPage * PER_PAGE;
    int endIdx = std::min(startIdx + PER_PAGE, 45);
    int totalPages = (45 + PER_PAGE - 1) / PER_PAGE;

    for (int i = startIdx; i < endIdx; i++) {
        int local = i - startIdx;
        int col = local % COLS;
        int row = local / COLS;
        float cx = startX + col * (CARD_W + CARD_GAP_X);
        float cy = startY + row * (CARD_H + CARD_GAP_Y);

        const auto& info = kSpecies[i];

        // Card background
        sf::RectangleShape card(sf::Vector2f(CARD_W, CARD_H));
        card.setFillColor(sf::Color::White);
        card.setOutlineColor(sf::Color(200, 210, 225));
        card.setOutlineThickness(1.5f);
        card.setPosition(cx, cy);
        window.draw(card);

        // ID number
        sf::Text idText(L"#" + std::to_wstring(i + 1), font, 13);
        idText.setFillColor(sf::Color(130, 140, 165));
        idText.setPosition(cx + 8, cy + 5);
        window.draw(idText);

        // 图片
        float imgSize = 110.0f;
        float imgX = cx + (CARD_W - imgSize) / 2.0f;
        float imgY = cy + 20.0f;

        if (textureLoaded[i] && textures[i].getSize().x > 0) {
            sf::Vector2u texSize = textures[i].getSize();
            float scale = std::min(imgSize / texSize.x, imgSize / texSize.y);
            sf::Sprite sprite(textures[i]);
            sprite.setScale(scale, scale);
            sprite.setPosition(
                imgX + (imgSize - texSize.x * scale) / 2.0f,
                imgY + (imgSize - texSize.y * scale) / 2.0f
            );
            window.draw(sprite);
        } else {
            // Image not available - leave blank area
        }

        // Name
        sf::Text nameText(info.name, font, 17);
        nameText.setFillColor(sf::Color(30, 40, 60));
        sf::FloatRect nb = nameText.getLocalBounds();
        nameText.setOrigin(nb.left + nb.width / 2, nb.top + nb.height / 2);
        nameText.setPosition(cx + CARD_W / 2, cy + CARD_H - 18);
        window.draw(nameText);
    }

    // 底部导航
    float btnY = kH - 70;
    float centerX = kW / 2.0f;

    // Previous page button (bottom-left)
    sf::RectangleShape prevBtn(sf::Vector2f(100, 36));
    prevBtn.setFillColor(sf::Color(70, 110, 160));
    prevBtn.setPosition(40, btnY);
    window.draw(prevBtn);
    sf::Text prevText(L"← 上一页", font, 16);
    prevText.setFillColor(sf::Color::White);
    sf::FloatRect pb = prevText.getLocalBounds();
    prevText.setOrigin(pb.left + pb.width / 2, pb.top + pb.height / 2);
    prevText.setPosition(90, btnY + 18);
    window.draw(prevText);

    // Next page button (bottom-right)
    sf::RectangleShape nextBtn(sf::Vector2f(100, 36));
    nextBtn.setFillColor(sf::Color(70, 110, 160));
    nextBtn.setPosition(kW - 140, btnY);
    window.draw(nextBtn);
    sf::Text nextText(L"下一页 →", font, 16);
    nextText.setFillColor(sf::Color::White);
    sf::FloatRect nb2 = nextText.getLocalBounds();
    nextText.setOrigin(nb2.left + nb2.width / 2, nb2.top + nb2.height / 2);
    nextText.setPosition(kW - 90, btnY + 18);
    window.draw(nextText);

    // Page number (centered)
    sf::Text pageText(
        L"第 " + std::to_wstring(currentPage + 1) + L" / " + std::to_wstring(totalPages) + L" 页",
        font, 16
    );
    pageText.setFillColor(sf::Color(80, 90, 110));
    sf::FloatRect ptb = pageText.getLocalBounds();
    pageText.setOrigin(ptb.left + ptb.width / 2, ptb.top + ptb.height / 2);
    pageText.setPosition(centerX, btnY + 18);
    window.draw(pageText);
}

// 详情模式

void SpritePokedex::drawDetail(sf::RenderWindow& window) {
    int idx = detailIndex;
    if (idx < 0 || idx >= 45) return;
    const auto& info = kSpecies[idx];

    // 半透明背景
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(kW), static_cast<float>(kH)));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    overlay.setPosition(0, 0);
    window.draw(overlay);

    // Main panel
    float panelW = 900, panelH = 640;
    float panelX = (kW - panelW) / 2;
    float panelY = (kH - panelH) / 2;
    sf::RectangleShape panel(sf::Vector2f(panelW, panelH));
    panel.setFillColor(sf::Color(245, 247, 252));
    panel.setOutlineColor(sf::Color(180, 190, 210));
    panel.setOutlineThickness(2);
    panel.setPosition(panelX, panelY);
    window.draw(panel);

    // Back button
    sf::FloatRect backBtn(panelX + 20, panelY + 15, 90, 32);
    sf::RectangleShape back(sf::Vector2f(backBtn.width, backBtn.height));
    back.setFillColor(sf::Color(100, 120, 150));
    back.setPosition(backBtn.left, backBtn.top);
    window.draw(back);
    sf::Text backText(L"← 返回", font, 16);
    backText.setFillColor(sf::Color::White);
    sf::FloatRect btb = backText.getLocalBounds();
    backText.setOrigin(btb.left + btb.width / 2, btb.top + btb.height / 2);
    backText.setPosition(backBtn.left + backBtn.width / 2, backBtn.top + backBtn.height / 2);
    window.draw(backText);

    float leftX = panelX + 40;
    float rightX = panelX + 420;
    float topY = panelY + 60;

    // Left: large sprite image
    float bigImgSize = 280;
    if (textureLoaded[idx] && textures[idx].getSize().x > 0) {
        sf::Vector2u ts = textures[idx].getSize();
        float sc = std::min(bigImgSize / ts.x, bigImgSize / ts.y);
        sf::Sprite sprite(textures[idx]);
        sprite.setScale(sc, sc);
        sprite.setPosition(leftX + (bigImgSize - ts.x * sc) / 2, topY);
        window.draw(sprite);
    } else {
        sf::RectangleShape ph(sf::Vector2f(bigImgSize, bigImgSize));
        ph.setFillColor(sf::Color(225, 230, 242));
        ph.setPosition(leftX, topY);
        window.draw(ph);
        sf::Text q(L"?", font, 80);
        q.setFillColor(sf::Color(170, 180, 200));
        sf::FloatRect qb = q.getLocalBounds();
        q.setOrigin(qb.left + qb.width / 2, qb.top + qb.height / 2);
        q.setPosition(leftX + bigImgSize / 2, topY + bigImgSize / 2);
        window.draw(q);
    }

    // Left: Name and ID
    float nameY = topY + bigImgSize + 20;
    sf::Text nameText(info.name, font, 36);
    nameText.setFillColor(sf::Color::Black);
    nameText.setPosition(leftX, nameY);
    window.draw(nameText);

    sf::Text idBig(L"#" + std::to_wstring(idx + 1), font, 20);
    idBig.setFillColor(sf::Color(130, 140, 165));
    idBig.setPosition(leftX + 140, nameY + 8);
    window.draw(idBig);

    // Left: Type badges
    float typeY = nameY + 50;
    auto drawTypeBadge = [&](int e, float yy) -> float {
        if (e == 0) return yy;
        sf::RectangleShape badge(sf::Vector2f(80, 28));
        badge.setFillColor(elemColor(e));
        badge.setPosition(leftX, yy);
        window.draw(badge);
        sf::Text elemText(kElemNames[e], font, 16);
        elemText.setFillColor(sf::Color::White);
        sf::FloatRect tb = elemText.getLocalBounds();
        elemText.setOrigin(tb.left + tb.width / 2, tb.top + tb.height / 2);
        elemText.setPosition(leftX + 40, yy + 14);
        window.draw(elemText);
        return yy + 36;
    };
    typeY = drawTypeBadge(info.type1, typeY);
    drawTypeBadge(info.type2, typeY);

    // "Skills" button below types
    sf::RectangleShape skillBtn(sf::Vector2f(80, 28));
    skillBtn.setFillColor(sf::Color(120, 160, 80));
    skillBtn.setPosition(leftX, typeY + 4);
    window.draw(skillBtn);
    sf::Text skillText(L"技能", font, 16);
    skillText.setFillColor(sf::Color::White);
    sf::FloatRect skb = skillText.getLocalBounds();
    skillText.setOrigin(skb.left + skb.width / 2, skb.top + skb.height / 2);
    skillText.setPosition(leftX + 40, typeY + 18);
    window.draw(skillText);

    // Right top: Ability
    sf::Text abilityLabel(L"特性", font, 22);
    abilityLabel.setFillColor(sf::Color(40, 50, 70));
    abilityLabel.setPosition(rightX, topY);
    window.draw(abilityLabel);

    sf::RectangleShape abilityBox(sf::Vector2f(panelW - (rightX - panelX) - 40, 70));
    abilityBox.setFillColor(sf::Color(232, 237, 250));
    abilityBox.setPosition(rightX, topY + 35);
    window.draw(abilityBox);

    sf::Text abilityText(info.ability, font, 15);
    abilityText.setFillColor(sf::Color(50, 55, 75));
    abilityText.setPosition(rightX + 10, topY + 42);
    window.draw(abilityText);

    // Right: Base stats
    float statY = topY + 130;
    sf::Text statLabel(L"基础种族值", font, 22);
    statLabel.setFillColor(sf::Color(40, 50, 70));
    statLabel.setPosition(rightX, statY);
    window.draw(statLabel);

    // Stat bars
    struct StatItem { const wchar_t* label; int value; sf::Color color; };
    StatItem stats[6] = {
        {L"生命",   info.hp,   sf::Color(220, 60, 60)},
        {L"物攻",   info.atk,  sf::Color(240, 150, 30)},
        {L"物防",   info.def,  sf::Color(60, 160, 200)},
        {L"魔攻",   info.mgAtk,sf::Color(160, 80, 220)},
        {L"魔防",   info.mgDef,sf::Color(60, 190, 120)},
        {L"速度",   info.spd,  sf::Color(80, 180, 200)},
    };
    int total = info.hp + info.atk + info.def + info.mgAtk + info.mgDef + info.spd;

    float statX = rightX;
    float statStartY = statY + 35;
    float barMaxW = 250;
    float rowH = 24;

    for (int i = 0; i < 6; i++) {
        float sy = statStartY + i * rowH;
        int val = stats[i].value;

        // Label
        sf::Text stLabel(stats[i].label, font, 15);
        stLabel.setFillColor(sf::Color(60, 60, 80));
        stLabel.setPosition(statX, sy);
        window.draw(stLabel);

        // Value
        sf::Text stVal(std::to_wstring(val), font, 15);
        stVal.setFillColor(sf::Color(40, 40, 60));
        stVal.setPosition(statX + 40, sy);
        window.draw(stVal);

        // Bar background
        sf::RectangleShape barBg(sf::Vector2f(barMaxW, 14));
        barBg.setFillColor(sf::Color(210, 215, 230));
        barBg.setPosition(statX + 70, sy + 1);
        window.draw(barBg);

        // Bar fill (max value 200)
        float ratio = std::min(1.0f, val / 200.0f);
        sf::RectangleShape barFill(sf::Vector2f(barMaxW * ratio, 14));
        barFill.setFillColor(stats[i].color);
        barFill.setPosition(statX + 70, sy + 1);
        window.draw(barFill);
    }

    // Total (moved to left side, below stats)
    float totalY = statStartY + 6 * rowH + 8;
    sf::Text totalText(L"种族值合计: " + std::to_wstring(total), font, 18);
    totalText.setFillColor(sf::Color::Black);
    totalText.setPosition(rightX, totalY);
    window.draw(totalText);

    // Radar chart (below the stat bars, lower down)
    float radarX = rightX + 200;
    float radarY = totalY + 80;
    float radarR = 85;
    drawRadarChart(window, sf::Vector2f(radarX, radarY), radarR,
                   info.hp, info.atk, info.def,
                   info.mgAtk, info.mgDef, info.spd);
}

// 技能池视图

void SpritePokedex::drawSkills(sf::RenderWindow& window) {
    int idx = detailIndex;
    if (idx < 0 || idx >= 45) return;
    const auto& info = kSpecies[idx];

    // 遮罩
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(kW), static_cast<float>(kH)));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    overlay.setPosition(0, 0);
    window.draw(overlay);

    // Main panel
    float panelW = 900, panelH = 640;
    float px = (kW - panelW) / 2;
    float py = (kH - panelH) / 2;
    sf::RectangleShape panel(sf::Vector2f(panelW, panelH));
    panel.setFillColor(sf::Color(245, 247, 252));
    panel.setOutlineColor(sf::Color(180, 190, 210));
    panel.setOutlineThickness(2);
    panel.setPosition(px, py);
    window.draw(panel);

    // ← Back button
    sf::RectangleShape back(sf::Vector2f(90, 32));
    back.setFillColor(sf::Color(100, 120, 150));
    back.setPosition(px + 20, py + 15);
    window.draw(back);
    sf::Text backT(L"← 返回", font, 16);
    backT.setFillColor(sf::Color::White);
    sf::FloatRect btb2 = backT.getLocalBounds();
    backT.setOrigin(btb2.left + btb2.width / 2, btb2.top + btb2.height / 2);
    backT.setPosition(px + 65, py + 31);
    window.draw(backT);

    // Title
    sf::Text title(L"技能池", font, 30);
    title.setFillColor(sf::Color(30, 40, 60));
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2, 0);
    title.setPosition(px + panelW / 2, py + 20);
    window.draw(title);

    // Species name
    sf::Text nameT(info.name, font, 22);
    nameT.setFillColor(sf::Color(60, 70, 90));
    sf::FloatRect nb = nameT.getLocalBounds();
    nameT.setOrigin(nb.left + nb.width / 2, 0);
    nameT.setPosition(px + panelW / 2, py + 56);
    window.draw(nameT);

    // 技能列表
    float listX = px + 60;
    float listY = py + 95;
    float listW = panelW - 120;

    // Header bar
    sf::RectangleShape headerBg(sf::Vector2f(listW, 34));
    headerBg.setFillColor(sf::Color(80, 120, 180));
    headerBg.setPosition(listX, listY);
    window.draw(headerBg);

    sf::Text hLv(L"等级", font, 16);
    hLv.setFillColor(sf::Color::White);
    hLv.setPosition(listX + 16, listY + 7);
    window.draw(hLv);

    sf::Text hName(L"技能", font, 16);
    hName.setFillColor(sf::Color::White);
    hName.setPosition(listX + 80, listY + 7);
    window.draw(hName);

    sf::Text hDesc(L"效果描述", font, 16);
    hDesc.setFillColor(sf::Color::White);
    hDesc.setPosition(listX + 170, listY + 7);
    window.draw(hDesc);

    // Rows
    auto& pool = speciesMovePools[idx];
    int perPage = 10, totalPages = ((int)pool.size() + perPage - 1) / perPage;
    if (skillPage >= totalPages) skillPage = totalPages - 1;
    int start = skillPage * perPage, end = std::min(start + perPage, (int)pool.size());

    float rowH = 42;
    for (int vi = 0, i = start; i < end; i++, vi++) {
        float ry = listY + 34 + vi * rowH;

        bool even = (vi % 2 == 0);
        sf::RectangleShape rowBg(sf::Vector2f(listW, rowH));
        rowBg.setFillColor(even ? sf::Color(235, 238, 248) : sf::Color(250, 251, 255));
        rowBg.setPosition(listX, ry);
        window.draw(rowBg);

        // Level badge
        sf::RectangleShape lvBadge(sf::Vector2f(50, 22));
        lvBadge.setFillColor(sf::Color(90, 130, 190));
        lvBadge.setPosition(listX + 8, ry + 10);
        window.draw(lvBadge);
        sf::Text lvT(L"Lv." + std::to_wstring(pool[i].level), font, 13);
        lvT.setFillColor(sf::Color::White);
        sf::FloatRect lvb = lvT.getLocalBounds();
        lvT.setOrigin(lvb.left + lvb.width / 2, lvb.top + lvb.height / 2);
        lvT.setPosition(listX + 33, ry + 21);
        window.draw(lvT);

        // Name
        sf::Text mnT(pool[i].name, font, 17);
        mnT.setFillColor(sf::Color(30, 40, 60));
        mnT.setPosition(listX + 80, ry + 10);
        window.draw(mnT);

        // Description
        if (!pool[i].desc.empty()) {
            sf::Text descT(pool[i].desc, font, 14);
            descT.setFillColor(sf::Color(80, 90, 115));
            descT.setPosition(listX + 170, ry + 12);
            window.draw(descT);
        }
    }

    // Bottom bar: page count + navigation
    float btnY = py + 640 - 48;
    sf::Text countT(L"共 " + std::to_wstring(pool.size()) + L" 个技能", font, 15);
    countT.setFillColor(sf::Color(100, 110, 130));
    countT.setPosition(listX, btnY + 6);
    window.draw(countT);

    // Page number (centered)
    sf::Text pageT(L"第 " + std::to_wstring(skillPage + 1) + L" / " + std::to_wstring(totalPages) + L" 页", font, 16);
    pageT.setFillColor(sf::Color(80, 90, 110));
    sf::FloatRect ptb2 = pageT.getLocalBounds();
    pageT.setOrigin(ptb2.left + ptb2.width / 2, ptb2.top + ptb2.height / 2);
    pageT.setPosition(px + 900 / 2, btnY + 15);
    window.draw(pageT);

    // Prev button (left)
    if (skillPage > 0) {
        sf::RectangleShape pb(sf::Vector2f(100, 30));
        pb.setFillColor(sf::Color(70, 110, 160));
        pb.setPosition(px + 60, btnY);
        window.draw(pb);
        sf::Text pt(L"← 上一页", font, 14);
        pt.setFillColor(sf::Color::White);
        sf::FloatRect pbb = pt.getLocalBounds();
        pt.setOrigin(pbb.left + pbb.width / 2, pbb.top + pbb.height / 2);
        pt.setPosition(px + 110, btnY + 15);
        window.draw(pt);
    }

    // Next button (right)
    if (skillPage < totalPages - 1) {
        sf::RectangleShape nb(sf::Vector2f(100, 30));
        nb.setFillColor(sf::Color(70, 110, 160));
        nb.setPosition(px + 740, btnY);
        window.draw(nb);
        sf::Text nt(L"下一页 →", font, 14);
        nt.setFillColor(sf::Color::White);
        sf::FloatRect nbb = nt.getLocalBounds();
        nt.setOrigin(nbb.left + nbb.width / 2, nbb.top + nbb.height / 2);
        nt.setPosition(px + 790, btnY + 15);
        window.draw(nt);
    }
}

// 雷达图

void SpritePokedex::drawRadarChart(sf::RenderWindow& window, sf::Vector2f center, float radius,
                                    int hp, int atk, int def, int mgAtk, int mgDef, int spd)
{
    // Stat caps
    static constexpr int kStatCaps[6] = {180, 180, 160, 180, 160, 150};
    // Angles for 6 vertices (starting from top, clockwise)
    static constexpr float angles[6] = {-90, -30, 30, 90, 150, 210};
    int vals[6] = {hp, atk, def, mgAtk, mgDef, spd};

    // Background hexagon grid (4 layers)
    for (int layer = 1; layer <= 4; layer++) {
        float r = radius * layer / 4.0f;
        sf::VertexArray hex(sf::PrimitiveType::LineStrip, 7);
        for (int i = 0; i < 7; i++) {
            float a = angles[i % 6] * 3.14159265f / 180.0f;
            hex[i].position = sf::Vector2f(
                center.x + r * std::cos(a),
                center.y + r * std::sin(a)
            );
            hex[i].color = sf::Color(180, 185, 200);
        }
        window.draw(hex);
    }

    // Axes
    for (int i = 0; i < 6; i++) {
        float a = angles[i] * 3.14159265f / 180.0f;
        sf::Vertex line[2] = {
            sf::Vertex(center, sf::Color(160, 165, 185)),
            sf::Vertex(sf::Vector2f(
                center.x + radius * std::cos(a),
                center.y + radius * std::sin(a)
            ), sf::Color(160, 165, 185))
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // Data polygon (fill)
    sf::VertexArray data(sf::PrimitiveType::TriangleFan, 8);
    data[0].position = center;
    data[0].color = sf::Color(60, 130, 220, 70);

    for (int i = 0; i < 6; i++) {
        float ratio = std::min(1.0f, vals[i] / (float)kStatCaps[i]);
        float a = angles[i] * 3.14159265f / 180.0f;
        float r = radius * ratio;
        data[i + 1].position = sf::Vector2f(
            center.x + r * std::cos(a),
            center.y + r * std::sin(a)
        );
        data[i + 1].color = sf::Color(50, 110, 210, 150);
    }
    data[7].position = data[1].position;
    data[7].color = data[1].color;
    window.draw(data);

    // Data polygon outline
    sf::VertexArray outline(sf::PrimitiveType::LineStrip, 7);
    for (int i = 0; i < 7; i++) {
        float ratio = std::min(1.0f, vals[i % 6] / (float)kStatCaps[i % 6]);
        float a = angles[i % 6] * 3.14159265f / 180.0f;
        float r = radius * ratio;
        outline[i].position = sf::Vector2f(
            center.x + r * std::cos(a),
            center.y + r * std::sin(a)
        );
        outline[i].color = sf::Color(30, 70, 170);
    }
    window.draw(outline);
}

// 颜色映射

sf::Color SpritePokedex::elemColor(int e) {
    switch (e) {
    case 0:  return sf::Color(160, 160, 160);
    case 1:  return sf::Color(168, 168, 120);
    case 2:  return sf::Color(240, 128, 48);
    case 3:  return sf::Color(104, 144, 240);
    case 4:  return sf::Color(120, 200, 80);
    case 5:  return sf::Color(248, 208, 48);
    case 6:  return sf::Color(152, 216, 216);
    case 7:  return sf::Color(192, 48, 40);
    case 8:  return sf::Color(160, 64, 160);
    case 9:  return sf::Color(224, 192, 104);
    case 10: return sf::Color(144, 200, 200);
    case 11: return sf::Color(168, 144, 240);
    case 12: return sf::Color(112, 88, 152);
    case 13: return sf::Color(112, 88, 72);
    case 14: return sf::Color(112, 56, 248);
    case 15: return sf::Color(240, 160, 184);
    default: return sf::Color(160, 160, 160);
    }
}
