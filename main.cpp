#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <cmath>
#include <filesystem>
#include <cfloat>
#include <sstream>
#include <locale>
#include "include/MapManager.h"
#include "include/SetMap.h"
#include "include/MainMenu.h"
#include "include/Player.h"
#include "include/battleUI.h"
#include "include/BagUI.h"
#include "include/Shop.h"
#include "include/ShopUI.h"
#include "include/spritePokedex.h"
using namespace std;

/* ===================== 应用状态 ===================== */
enum class AppState
{
    MainMenu,
    InGame,
    InBattle
};

/* ===================== 游戏会话 ===================== */
struct GameSession
{
    AppState state = AppState::MainMenu;
    MapManager mapManager;
    Player player;

    unique_ptr<BagUI> bagUI;
    unique_ptr<Shop> shop;
    unique_ptr<ShopUI> shopUI;

    // 初始位置相关
    sf::Vector2f playerPos{400.f, 300.f};
    int currentMapIndex = 0;
    int currentRoomIndex = 0;

    // 战斗系统相关
    unique_ptr<BattleSystem> battleSystem;
    unique_ptr<battleUI> ui;

    // 传送冷却相关
    sf::Clock transmitCooldown;
    bool canTransmit = true;
    const float TRANSMIT_CD = 2.f;

    // 精灵图鉴
    unique_ptr<SpritePokedex> pokedex;

    GameSession() : player("Unknown") { }
    void StartNewGame()
    {
        // 重置地图进度
        mapManager.ResetAllProgress();
        mapManager.AddMap(CreateLevel0());
        mapManager.AddMap(CreateLevel1());
        mapManager.AddMap(CreateLevel2());
        mapManager.AddMap(CreateLevel3());
        mapManager.AddMap(CreateLevel4());
        mapManager.AddMap(CreateLevel5());
        mapManager.LoadMap(0);

        // 重新构造玩家对象以重置所有进度
        player = Player(mapManager.GetCurrentPlayer());
        player.SetMapManager(&mapManager);

        // 给予初始精灵
        player.acquireCodemon(make_unique<Huohua>(5));
        player.withdrawToTeam(0);

        currentMapIndex = 0;
        currentRoomIndex = 0;
        playerPos = {400.f, 300.f};
        player.currentMapIndex = 0;
        player.currentRoomIndex = 0;
        player.position = playerPos;
    }

    void LoadOrNewGame(bool forceNew = false)
    {
        if (forceNew)
        {
            StartNewGame();
            return;
        }

        // 文件名约定
        string playerSave = mapManager.GetCurrentPlayer() + "SaveGame.dat";
        string mapSave = mapManager.GetCurrentPlayer() + "MapProgress.dat";
        bool mapLoadedFromFile = false;

        if (filesystem::exists(mapSave))
        {
			cout << "发现地图进度文件，尝试加载地图进度..." << endl;
            if (mapManager.LoadProgress(mapSave))
                mapLoadedFromFile = true;
            else cerr << "地图进度加载失败，使用默认模板初始化地图" << endl;
        }
        else cout << "未发现地图进度文件，使用默认模板初始化地图" << endl;

        // 如果没有从文件加载地图，则按模板创建
        if (!mapLoadedFromFile && mapManager.GetMapCount() == 0)
        {
            mapManager.AddMap(CreateLevel0());
            mapManager.AddMap(CreateLevel1());
            mapManager.AddMap(CreateLevel2());
            mapManager.AddMap(CreateLevel3());
            mapManager.AddMap(CreateLevel4());
            mapManager.AddMap(CreateLevel5());
        }

		// 确保玩家的地图管理器指针正确设置
        player.SetMapManager(&mapManager);
        // 读取玩家数据
        if (player.LoadFromFile(playerSave))
        {
            // 已读入 currentMapIndex/currentRoomIndex/position
            // 现在根据 player 保存的索引切换地图
            if (!mapManager.LoadMap(player.currentMapIndex))
            {
                cerr << "无法加载玩家指定地图索引：" << player.currentMapIndex << endl;
                cout << "启动新游戏！" << endl;
                StartNewGame();
                return;
            }

            currentMapIndex = player.currentMapIndex;
            currentRoomIndex = player.currentRoomIndex;
            playerPos = player.position;

            if (auto *map = mapManager.GetCurrentMap())
                map->SetCurrentRoom(player.currentRoomIndex);
            player.SetMapManager(&mapManager);
        }
        else
        {
            cerr << "读档失败，启动新游戏！" << endl;
            StartNewGame();
        }
    }

    void ReturnToMainMenuAndSave()
    {
        // 保存前确保所有精灵处于休息状态，防止存档时保存濒死状态
        for (size_t i = 0; i < player.getTeamSize(); ++i)
        {
            if (auto *c = player.getTeamCodemon(static_cast<int>(i)))
                c->rest();
        }
        for (size_t i = 0; i < player.getStorageSize(); ++i)
        {
            if (auto *c = player.getStorageCodemon(static_cast<int>(i)))
                c->rest();
        }

        player.currentMapIndex = currentMapIndex;
        player.currentRoomIndex = currentRoomIndex;
        player.position = playerPos;

        // 文件名约定
        string playerSave = mapManager.GetCurrentPlayer() + "SaveGame.dat";
        string mapSave = mapManager.GetCurrentPlayer() + "MapProgress.dat";

        // 保存玩家数据
        if (!player.SaveToFile(playerSave))
            cerr << "存档保存失败！" << playerSave << endl;
        else
            cout << "存档保存成功！" << playerSave << endl;
        // 保存地图 / 房间进度
        mapManager.SaveProgress(mapSave);
        cout << "地图进度已保存！" << mapSave << endl;
    }
};

/* ===================== Main ===================== */
int main()
{
    // 控制台设为 UTF-8 编码（配合 /utf-8 编译选项）
    system("chcp 65001 > nul");

    GameSession game;
    AppState state = AppState::MainMenu;
    string playerName, password;
    bool loggedIn = false;
    bool ifLeftPressed = false;

    /* ===================== 登录流程 ===================== */
    while (!loggedIn)
    {
        cout << "请输入玩家名称：";
        getline(cin, playerName);

        if (game.mapManager.CheckPlayerArchive(playerName))
        {
            cout << "请输入密码：";
            getline(cin, password);

            if (game.mapManager.VerifyPassword(playerName, password))
            {
                cout << "登录成功！" << endl;
                game.mapManager.SetCurrentPlayer(playerName);
                loggedIn = true;
            }
            else
                cout << "密码错误，请重新输入玩家名称！" << endl;
        }
        else
        {
            cout << "你好！新玩家：" << playerName << endl;
            cout << "请设置密码：";
            getline(cin, password);

            game.mapManager.RegisterPlayer(playerName, password);
            game.mapManager.SetCurrentPlayer(playerName);
            loggedIn = true;
        }
    }

    sf::RenderWindow window(sf::VideoMode(1000, 731), "Codemon Adventure");
    sf::View battleView(sf::FloatRect(0, 0, 1000.f, 731.f));
    window.setFramerateLimit(60);

    game.bagUI = make_unique<BagUI>(&game.player);
    game.shop = make_unique<Shop>("商店");
    game.shopUI = make_unique<ShopUI>(&game.player, game.shop.get());

    // 初始化精灵图鉴
    game.pokedex = make_unique<SpritePokedex>();
    if (game.pokedex->loadFont("fonts/simhei.ttf")) {
        game.pokedex->loadTextures();
        game.pokedex->loadSkillPools();
        cout << "精灵图鉴加载完成" << endl;
    } else {
        cerr << "精灵图鉴字体加载失败" << endl;
    }

    // HUD 字体与文本
    sf::Font uiFont;
    sf::Text controlText;
    if (uiFont.loadFromFile("fonts/simhei.ttf")) {
        controlText.setFont(uiFont);
        controlText.setCharacterSize(18);
        controlText.setFillColor(sf::Color::White);
        controlText.setOutlineColor(sf::Color::Black);
        controlText.setOutlineThickness(1.f);
        controlText.setPosition(10.f, 10.f);
    }

    /* ===================== 游戏初始化 ===================== */
    MainMenu menu(window);
    if (game.mapManager.CheckPlayerArchive(game.mapManager.GetCurrentPlayer()))
        menu.SetHasSave(true);
    else
        menu.SetHasSave(false);

    ItemDB::initialize();
    game.player = Player(game.mapManager.GetCurrentPlayer());
    game.player.acquireCodemon(make_unique<Huohua>(5));
    game.player.withdrawToTeam(0);

    /* ---------- 玩家视觉表示 ---------- */
    sf::Sprite playerSprite;
    sf::Sprite flippedPlayerSprite;
    sf::Texture playerTexture;
    sf::Texture flippedPlayerTexture;
    sf::CircleShape playerShape(15.f); // 回退形状（始终可用）
    playerShape.setFillColor(sf::Color::Green);
    playerShape.setOrigin(15.f, 15.f);

    bool playerTextureLoaded = false;  // 纹理是否成功加载
    bool flippedTextureLoaded = false; // 镜像纹理是否成功加载

    if (playerTexture.loadFromFile("Resources/Player.png"))
    {
        playerSprite.setTexture(playerTexture);
        playerSprite.setOrigin(playerTexture.getSize().x / 2.f, playerTexture.getSize().y / 2.f);
        float scaleX = 75.f / playerTexture.getSize().x;
        float scaleY = 75.f / playerTexture.getSize().y;
        float scale = min(scaleX, scaleY);
        playerSprite.setScale(scale, scale);
        playerTextureLoaded = true;

        // 镜像
        if (flippedPlayerTexture.loadFromFile("Resources/FlippedPlayer.png"))
        {
            flippedPlayerSprite.setTexture(flippedPlayerTexture);
            flippedPlayerSprite.setOrigin(flippedPlayerTexture.getSize().x / 2.f, flippedPlayerTexture.getSize().y / 2.f);
            flippedPlayerSprite.setScale(scale, scale);
            flippedTextureLoaded = true;
        }
    }
    else
        cerr << "无法加载玩家纹理，使用默认形状代替！" << endl;

    /* ---------- 相机与世界边界 ---------- */
    const int TILE_SIZE = 64;
    const int VIEW_W = 7;
    const int VIEW_H = 7;
    sf::View camera(sf::FloatRect(0, 0, VIEW_W * TILE_SIZE, VIEW_H * TILE_SIZE));
    float moveSpeed = 3.f;

    /* ===================== 主菜单逻辑 ===================== */
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // 精灵图鉴事件优先处理（仅鼠标事件）
            if (game.pokedex && game.pokedex->isVisible)
            {
                game.pokedex->handleEvent(event, window);
                continue;
            }

            if (game.shopUI && game.shopUI->isVisible)
            {
                game.shopUI->handleEvent(event, window);
                continue;
            }
            if (game.bagUI->isVisible)
            {
                game.bagUI->handleEvent(event, window);
                continue;
            }

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left &&
                state == AppState::MainMenu)
            {
                menu.HandleClick({(float)event.mouseButton.x, (float)event.mouseButton.y});
            }
        }

        if (state == AppState::MainMenu)
        {
            menu.Update(window, (sf::Vector2f)sf::Mouse::getPosition(window));
            window.clear(sf::Color(30, 30, 30));
            menu.Draw(window);
            window.display();

            if (menu.ShouldContinueGame())
            {
                game.LoadOrNewGame(false);
                state = AppState::InGame;
            }
            else if (menu.ShouldNewGame())
            {
                game.LoadOrNewGame(true);
                state = AppState::InGame;
            }
            else if (menu.ShouldResetGame())
            {
                remove((game.mapManager.GetCurrentPlayer() + "SaveGame.dat").c_str());
                menu.SetHasSave(false);
                menu.ResetFlags();
            }
            else if (menu.ShouldExitGame())
            {
                window.close();
            }
            else continue;
        }

        /* ===================== 战斗循环 ===================== */
        if (state == AppState::InBattle)
        {
            const string fontPath = "fonts/simhei.ttf";
            if (!game.battleSystem)
            {
                state = AppState::InGame;
                continue;
            }

            if (!game.ui) {
                game.ui = make_unique<battleUI>();
                if (!game.ui->init(fontPath))
                {
                    game.ui.reset();
                    state = AppState::InGame;
                    continue;
                }
            }

            //切回战斗视图
            window.setView(battleView);

            //绑定并同步初始 UI 状态
            game.ui->setBattleSystem(game.battleSystem.get());
            if (game.battleSystem->getActiveCodemon())
                game.ui->updatePlayer(game.battleSystem->getActiveCodemon());
            else if (game.player.getActiveCodemon())
                game.ui->updatePlayer(game.player.getActiveCodemon());

            if (game.battleSystem->getActiveEnemy())
                game.ui->updateEnemy(game.battleSystem->getActiveEnemy());
            if (game.player.getActiveCodemon())
                game.ui->updateSkillBtns(game.player.getActiveCodemon()->getEquippedMoves());
            game.ui->updateItemBtns();

            //战斗主循环
            while (window.isOpen() && game.battleSystem)
            {
                sf::Event event2;
                while (window.pollEvent(event2))
                {
                    if (event2.type == sf::Event::Closed)
                        window.close();

                    if (event2.type == sf::Event::MouseButtonPressed &&event2.mouseButton.button == sf::Mouse::Left)
                    {
                        if (game.ui && game.battleSystem)
                        {
                            sf::Vector2f mousePos(static_cast<float>(event2.mouseButton.x),static_cast<float>(event2.mouseButton.y));
                            int action = game.ui->handleClick(mousePos);
                            if (action != -1)
                            {
                                game.battleSystem->executePlayerAction(action, 0);
                                game.ui->updateItemBtns();
                            }
                        }
                    }
                }

                //更新战斗逻辑
                if (game.battleSystem)
                    game.battleSystem->update();

                //绘制
                window.clear();
                if (game.ui)
                    game.ui->draw(window);
                window.setView(battleView);
                window.display();

                // 轮询结果
                if (game.battleSystem->getBattleResult() != BattleResult::NONE)
                {
                    BattleResult res = game.battleSystem->getBattleResult();
                    if (res == BattleResult::PLAYER_WIN)
                    {
                        cout << "战斗胜利！" << endl;
                        Room *room = game.mapManager.GetCurrentMap()->GetCurrentRoom();
                        int currentMapIndex = game.mapManager.GetCurrentMapIndex();
                        int currentRoomIndex = game.mapManager.GetCurrentMap()->GetCurrentRoomIndex();

                        if (room)
                        {
                            Enemy *enemy = room->GetEnemyAtPosition({4, 4});
                            if (enemy)
                            {
                                // 目标箱子格
                                sf::Vector2i chestGrid(5, 4);
                                // 最佳位置和最佳距离
                                sf::Vector2i best(-1, -1);
                                float bestDist = FLT_MAX;

                                // 计算格中心的世界坐标
                                float tileSize = 64;
                                sf::Vector2f chestTopLeft = room->SqureToWorld(chestGrid.x, chestGrid.y);
                                sf::Vector2f chestCenter = chestTopLeft + sf::Vector2f(tileSize * 0.5f, tileSize * 0.5f);
                                sf::Vector2f dist = game.playerPos - chestCenter;

                                // 计算玩家与箱心是否重叠
                                const float playerRadius = 15.f;
                                float minDistance = (tileSize * 0.7f) + playerRadius + 1.0f;
                                if (dist.x * dist.x + dist.y * dist.y < minDistance * minDistance)
                                {
                                    // 四向候选格（以箱子格为中心）
                                    vector<sf::Vector2i> candidates =
                                        {
                                            {chestGrid.x, chestGrid.y - 1},
                                            {chestGrid.x, chestGrid.y + 1},
                                            {chestGrid.x - 1, chestGrid.y},
                                            {chestGrid.x + 1, chestGrid.y}};

                                    for (auto &g : candidates)
                                    {
                                        // 检查格子在范围内且格子本身可通行
                                        if (!room->IsSqureWalkable(g.x, g.y))
                                            continue;
                                        // 以该格中心为测试位置，检测玩家矩形是否与地图碰撞
                                        sf::Vector2f world = room->SqureToWorld(g.x, g.y) + sf::Vector2f(tileSize * 0.5f, tileSize * 0.5f);
                                        sf::FloatRect testRect(world.x - playerRadius, world.y - playerRadius, playerRadius * 2.f, playerRadius * 2.f);
                                        if (room->CheckCollision(testRect))
                                            continue;

                                        float dx = world.x - game.playerPos.x;
                                        float dy = world.y - game.playerPos.y;
                                        float dist = sqrt(dx * dx + dy * dy);
                                        if (dist < bestDist)
                                        {
                                            bestDist = dist;
                                            best = g;
                                        }
                                    }

                                    if (best.x != -1)
                                        game.playerPos = room->SqureToWorld(best.x, best.y) + sf::Vector2f(tileSize * 0.5f, tileSize * 0.5f);
                                    else
                                        game.playerPos = room->SqureToWorld(5, 3) + sf::Vector2f(tileSize * 0.5f, tileSize * 0.5f);
                                }

                                ChestRewardData reward;
                                if (currentRoomIndex == 8)
                                    reward = CreateChestY(currentMapIndex);
                                else if (currentRoomIndex != 8 && currentRoomIndex < 5)
                                    reward = CreateChestX1(currentMapIndex);
                                else if (currentRoomIndex != 8 && currentRoomIndex >= 5)
                                    reward = CreateChestX2(currentMapIndex);
                                auto rewardCodemon = room->RemoveEnemy(enemy, reward);
                                game.player.acquireCodemon(move(rewardCodemon));
                            }
                        }

                        state = AppState::InGame;
                        game.player.canMove = true;
                        game.player.state = Player::State::Idle;
                    }
                    else if (res == BattleResult::PLAYER_LOSE)
                    {
                        cout << "战斗失败" << endl;
                        game.player.canMove = true;
                        game.player.state = Player::State::Idle;
                        state = AppState::InGame;
                    }
                    game.battleSystem.reset();
                    game.ui.reset();
                    break;
                }
            }

            // 确保离开战斗后恢复移动状态（以防回合中未设置）
            if (state == AppState::InGame)
            {
                game.player.canMove = true;
                game.player.state = Player::State::Idle;
            }
        }

        /* ===================== 游戏内逻辑 ===================== */
        auto *currentMap = game.mapManager.GetCurrentMap();
        if (!currentMap)
        {
            cout << "地图未加载！" << endl;
            state = AppState::MainMenu;
            continue;
        }

        auto *currentRoom = currentMap->GetCurrentRoom();
        if (!currentRoom)
        {
            cout << "房间未加载！" << endl;
            state = AppState::MainMenu;
            continue;
        }

        /* ---- 玩家移动 ---- */
        sf::Vector2f prevPos = game.playerPos;
        sf::Vector2f moveDir(0.f, 0.f);

        if (game.player.canMove)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                game.playerPos.y -= moveSpeed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                game.playerPos.y += moveSpeed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                game.playerPos.x -= moveSpeed;
                ifLeftPressed = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                game.playerPos.x += moveSpeed;
                ifLeftPressed = false;
            }
            if (currentMap->GetCurrentRoom()->CheckCollision(
                    sf::FloatRect(game.playerPos.x - 20, game.playerPos.y - 20, 40, 40)))
                game.playerPos = prevPos;
        }

        /* ---- 归一化对角线移动 ---- */
        if (moveDir.x != 0 && moveDir.y != 0)
            moveDir /= sqrt(2.f);
        game.playerPos += moveDir * moveSpeed;

        /* ---- 碰撞检测 ---- */
        sf::FloatRect playerBounds(
            game.playerPos.x - 5, game.playerPos.y - 5, 10, 10);
        if (currentRoom->CheckCollision(playerBounds))
            game.playerPos = prevPos;

        /* ---- 传送逻辑 ---- */
        if (game.canTransmit)
        {
            sf::Vector2f newRoomPos = currentMap->TransmitToRoom(game.playerPos);
            if (newRoomPos != game.playerPos)
            {
                game.playerPos = newRoomPos;
                game.currentRoomIndex = currentMap->GetCurrentRoomIndex();
                game.canTransmit = false;
                game.transmitCooldown.restart();
                camera.setCenter(game.playerPos);
            }
        }
        else if (game.transmitCooldown.getElapsedTime().asSeconds() >= game.TRANSMIT_CD)
            game.canTransmit = true;

        /* ---- E：开宝箱 / 钥匙 ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            if (currentRoom)
            {
                int idx = game.mapManager.GetCurrentMapIndex();

                auto tryUseKey = [&](const char *keyName, const char *blockKey)
                {
                    if (game.player.HasKey(keyName))
                    {
                        if (currentRoom->RemoveBlocksByKey(blockKey))
                        {
                            cout << "使用" << keyName << "移除了障碍物！" << endl;
                        }
                    }
                };

                if (idx == 0)
                {
                    tryUseKey("钥匙01", "KEY01");
                }
                else if (idx == 1)
                {
                    tryUseKey("钥匙12", "KEY12");
                }
                else if (idx == 2)
                {
                    tryUseKey("钥匙21", "KEY21");
                    tryUseKey("钥匙22", "KEY22");
                }
                else if (idx == 3)
                {
                    tryUseKey("钥匙32", "KEY32");
                }
                else if (idx == 4)
                {
                    tryUseKey("钥匙42", "KEY42");
                }
                else if (idx == 5)
                {
                    tryUseKey("钥匙51", "KEY51");
                    tryUseKey("钥匙52", "KEY52");
                }

                ChestRewardData reward = currentRoom->SearchChest(game.playerPos);
                game.player.addChestReward(move(reward));

                if (game.bagUI->isVisible)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        game.bagUI->updateGridItems();
                        game.bagUI->updateDetailPanel();
                    }
                }
            }
            sf::sleep(sf::milliseconds(200));
        }

        /* ---- Q键：调试击杀敌人 ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            if (!currentRoom)
                continue;
            Enemy *enemy = currentRoom->GetEnemyAtPosition({4, 4});
            if (enemy && !enemy->IsDefeated())
            {
                ChestRewardData reward = CreateChestX1(game.currentMapIndex);
                auto stolenCodemon = currentRoom->RemoveEnemy(enemy, reward);
                if (stolenCodemon)
                {
                    game.player.acquireCodemon(move(stolenCodemon));
                    cout << "敌人被击败，精灵已进入仓库" << endl;
                }
            }
            sf::sleep(sf::milliseconds(200));
        }

        /* ---- F键：与敌人交互（进入战斗） ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        {
            if (!currentRoom)
                continue;
            Enemy *enemy = currentRoom->SearchEnemy(game.playerPos);
            if (!enemy || enemy->IsDefeated())
            {
                cout << "附近没有可战斗的敌人" << endl;
                sf::sleep(sf::milliseconds(200));
                continue;
            }

            game.battleSystem = currentRoom->StartBattleWithEnemy(enemy, &game.player);
            if (game.battleSystem)
            {

                game.ui = make_unique<battleUI>();
                if (game.ui->init("fonts/simhei.ttf"))
                {
                    game.ui->setBattleSystem(game.battleSystem.get());
                    state = AppState::InBattle;
                    game.player.canMove = false;
                    game.player.state = Player::State::InBattle;
                }
            }
            sf::sleep(sf::milliseconds(200));
        }

        /* ---- C键：检查通关并前往下一张地图 ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            if (game.mapManager.IsMapCompleted(game.currentMapIndex))
            {
                int nextIdx = game.currentMapIndex + 1;
                if (nextIdx < game.mapManager.GetMapCount())
                {
                    if (game.mapManager.LoadMap(nextIdx))
                    {
                        game.currentMapIndex = nextIdx;
                        game.currentRoomIndex = 0;
                        game.playerPos = {400.f, 300.f};
                        cout << "切换到下一关: " << nextIdx << endl;

                        // 解锁新商店阶段
                        int newStage = nextIdx + 1;
                        if (newStage >= 2 && newStage <= 6)
                        {
                            string stageName = "stage" + to_string(newStage);
                            game.player.enterStage(stageName);
                        }
                    }
                }
                else
                {
                    cout << "恭喜通关！返回主菜单并清空存档..." << endl;
                    // 删除存档
                    remove("SaveGame.dat");
                    menu.SetHasSave(false); // 没存档
                    // 重置菜单
                    menu.ResetFlags();
                    state = AppState::MainMenu;
                }
            }
            else
                cout << "当前地图尚未完成！" << endl;
            sf::sleep(sf::milliseconds(200));
        }

        /* ---- Z键：保存并返回主菜单 ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            game.ReturnToMainMenuAndSave();
            menu.ResetFlags();
            state = AppState::MainMenu;
            menu.SetHasSave(true);
            sf::sleep(sf::milliseconds(200));
            continue;
        }

        /* ---- B键：打开背包 ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            game.bagUI->isVisible = !game.bagUI->isVisible;
            sf::sleep(sf::milliseconds(200));
        }

        /* ---- M键：打开精灵图鉴 ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            if (game.pokedex)
                game.pokedex->toggle();
            sf::sleep(sf::milliseconds(200));
        }

        /* ---- H键：打开商店 ---- */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        {
            if (game.shopUI)
                game.shopUI->open();
            sf::sleep(sf::milliseconds(200));
        }

        /* ---- 相机更新 ---- */
        camera.setCenter(game.playerPos);
        window.setView(camera);

        if (game.shopUI && game.shopUI->isVisible)
        {
            game.shopUI->update(1.f / 60.f);
            window.clear(sf::Color::Black);
            game.shopUI->render(window);
            // 图鉴叠加层
            if (game.pokedex && game.pokedex->isVisible)
                game.pokedex->draw(window);
            window.display();
        }
        else if (game.bagUI->isVisible)
        {
            game.bagUI->update();
            window.clear(sf::Color::Black);
            game.bagUI->render(window);
            // 图鉴叠加层
            if (game.pokedex && game.pokedex->isVisible)
                game.pokedex->draw(window);
            window.display();
        }
        else
        {
            window.clear(sf::Color::Black);
            game.mapManager.GetCurrentMap()->Update(1.f / 60.f);
            game.mapManager.GetCurrentMap()->Draw(window);

            // 使用预加载的纹理绘制玩家精灵（避免每帧 loadFromFile 导致性能下降）
            if (playerTextureLoaded)
            {
                if (ifLeftPressed && flippedTextureLoaded)
                {
                    flippedPlayerSprite.setPosition(game.playerPos);
                    window.draw(flippedPlayerSprite);
                }
                else
                {
                    playerSprite.setPosition(game.playerPos);
                    window.draw(playerSprite);
                }
            }
            else
            {
                // 纹理加载失败时使用回退形状
                playerShape.setPosition(game.playerPos);
                window.draw(playerShape);
            }
            // 图鉴叠加层
            if (game.pokedex && game.pokedex->isVisible)
                game.pokedex->draw(window);

            // HUD 信息（切换到默认视图绘制屏幕坐标文本）
            if (controlText.getFont()) {
                window.setView(window.getDefaultView());
                std::wostringstream woss;
                woss << L"移动：WSAD    移除路障/打开宝箱：E    打开背包：B    打开商店：H    保存并退出：Z  \n";
                woss << L"当前关卡：" << std::to_wstring(game.currentMapIndex)
                     << L"    当前房间：" << std::to_wstring(game.currentRoomIndex)
                     << L"    金币：" << std::to_wstring(game.player.getGold())
                     << L"    经验：" << std::to_wstring(game.player.getExp());
                controlText.setString(woss.str());
                window.draw(controlText);
            }

            window.display();
        }
    }
    return 0;
}