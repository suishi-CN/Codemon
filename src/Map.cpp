#include "../include/MainMenu.h"
#include <iostream>
using namespace std;

MainMenu::MainMenu(const sf::RenderWindow& window)
{
    if (!menuBgTex.loadFromFile("Resources/Menu.png"))
        cerr << "Failed to load menu background!" << endl;
    menuBg.setTexture(menuBgTex);
    font.loadFromFile("fonts/simhei.ttf");
    vector<wstring> labels =
    { L"继续游戏", L"新游戏", L"退出游戏" };

    float startY = 260.f;
    float step = 80.f;

    for (size_t i = 0; i < labels.size(); ++i)
    {
        Button b{};
        b.shape.setSize({ 300, 60 });
        b.shape.setPosition({
            window.getSize().x / 2.f - 150,
            startY + i * step });
        b.shape.setFillColor(sf::Color(80, 80, 80));
        b.text.setFont(font);
        b.text.setString(labels[i]);
        b.text.setCharacterSize(26);
        b.text.setFillColor(sf::Color::White);

        auto bounds = b.text.getLocalBounds();
        b.text.setOrigin(bounds.width / 2, bounds.height / 2);
        b.text.setPosition(
            b.shape.getPosition().x + 150,
            b.shape.getPosition().y + 30);
        buttons.push_back(std::move(b));
    }
}

void MainMenu::ResetFlags()
{
    continueClicked = false;
    newGameClicked = false;
    resetClicked = false;
    exitClicked = false;
}

void MainMenu::Update(const sf::RenderWindow& window, const sf::Vector2f& mp)
{
    menuBg.setScale(
        (float)window.getSize().x / menuBgTex.getSize().x,
        (float)window.getSize().y / menuBgTex.getSize().y);
    for (auto& b : buttons)
    {
        bool enabled = true;
        if (&b == &buttons[0])
            enabled = m_hasSave;

        b.shape.setFillColor(
            enabled && b.shape.getGlobalBounds().contains(mp)
            ? sf::Color(140, 140, 140)
            : (enabled ? sf::Color(80, 80, 80) : sf::Color(40, 40, 40)));
    }
}

void MainMenu::HandleClick(const sf::Vector2f& mp)
{
    continueClicked = false;
    newGameClicked = false;
    exitClicked = false;
    resetClicked = false;

    if (buttons[0].shape.getGlobalBounds().contains(mp))
    {
        if (m_hasSave)
            continueClicked = true;
    }
    else if (buttons[1].shape.getGlobalBounds().contains(mp))
        newGameClicked = true;
    else if (buttons[2].shape.getGlobalBounds().contains(mp))
        exitClicked = true;
}

void MainMenu::Draw(sf::RenderWindow& window)
{
    sf::View prevView = window.getView();
    window.setView(window.getDefaultView());
    if (menuBgTex.getSize().x > 0 && menuBgTex.getSize().y > 0)
    {
        menuBg.setPosition(0.f, 0.f);
        menuBg.setScale(
            static_cast<float>(window.getSize().x) / static_cast<float>(menuBgTex.getSize().x),
            static_cast<float>(window.getSize().y) / static_cast<float>(menuBgTex.getSize().y));
    }
    window.draw(menuBg);
    for (auto& b : buttons)
    {
        window.draw(b.shape);
        window.draw(b.text);
    }
    window.setView(prevView);
}