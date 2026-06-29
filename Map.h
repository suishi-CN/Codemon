#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
using namespace std;

class MainMenu
{
public:
    // 构造函数
    MainMenu(const sf::RenderWindow& window);
    // 重置状态相关
    void ResetFlags();
    void SetHasSave(bool hasSave) { m_hasSave = hasSave; }

    void Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos);
    void HandleClick(const sf::Vector2f& mousePos);
    void Draw(sf::RenderWindow& window);

    bool ShouldResetGame() const { return resetClicked; }
    bool ShouldContinueGame() const { return continueClicked; }
    bool ShouldNewGame() const { return newGameClicked; }
    bool ShouldExitGame() const { return exitClicked; }

private:
	// 纹理和精灵和字体
    sf::Texture menuBgTex;
    sf::Sprite menuBg;
    sf::Font font;
    // 按钮结构体
    struct Button
    {
        sf::RectangleShape shape{ sf::Vector2f(300.f, 60.f) };
        sf::Text text;
    };
    // 按钮数组
    vector<Button> buttons;
	bool resetClicked = false;
    bool continueClicked = false;
    bool newGameClicked = false;
    bool exitClicked = false;
    bool m_hasSave = false;   // 新添加：是否有存档
};