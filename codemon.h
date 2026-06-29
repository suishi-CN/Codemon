// battleUI.h
//这里我们用的是SFML库实现战斗界面
//对目前的战斗界面我们借鉴了赛尔号这个知名超级厉害的游戏的界面
//对变量的命名我们是参考了SFML的命名习惯，使用驼峰命名法，并且在变量名前加上类型前缀（如playerNameText）以提高代码可读性
//这里我们在调试测试SFML时发现了两个问题，一个是字体显示问题，一个是中文显示问题
//后来我们选择了用simhei.ttf这个字体文件，这个字体文件既支持中文又能在SFML中正常显示，所以我们就用这个字体文件了
//然后中文显示方面我们在网络上找到了一个人给出的解决方案，就是在设置文本字符串时
//使用宽字符串（wstring）而不是普通字符串（string），这样就能正确显示中文了，所以我们在代码中使用了wstring来设置文本字符串
#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>        
#include <SFML/Audio.hpp>
#include <string>
#include "battlesystem.h"
#include "../include/codemon.h"
using namespace std;
using namespace sf;
using namespace monbase;
//这里就是右下角那4个主要的栏目按钮，对应的是战斗、道具、精灵、撤退
enum class CurrentTab 
{
    BATTLE,   // 战斗栏目
    ITEM,     // 道具栏目
    CODEMON,  // 精灵栏目   
    RETREAT   // 撤退栏目
};


//基础定义类
class battleUIBase 
{
protected:
    //字体
    Font font;

    //背景
    Texture bgTexture;
    Sprite bgSprite;

    //获取元素名称的函数
    wstring getElementName(Element element);

public:
    battleUIBase() = default;
    virtual ~battleUIBase() = default;

    //基础接口
    //初始化界面的函数
    virtual bool init(const string& fontPath) = 0;
    //绘制的函数
    virtual void draw(RenderWindow& window) = 0;
    //更新信息的函数
    virtual void updatePlayer(const Codemon* codemon) = 0;
    virtual void updateEnemy(const Codemon* codemon) = 0;
    virtual void updateSkillBtns(const vector<const Move*>& skills) = 0;
    virtual void addLog(const string& msg) = 0;
    virtual void updateItemBtns() = 0;
    virtual void updateCodemonBtns() = 0;
    //设置属性关系的函数
    virtual void setTypeAdvantage() = 0;
    //设置战斗系统指针的函数
    virtual void setBattleSystem(BattleSystem* bs) = 0;
    //鼠标交互的函数
    virtual int handleClick(const Vector2f& mousePos) = 0;
    //设置精灵头像图片的函数
    virtual void setPlayerSprite(const string& texturePath) = 0;
    virtual void setEnemySprite(const string& texturePath) = 0;
	//转为宽字符串的函数
    virtual wstring toWideString(const string& str) = 0;
};

//UI组件类
class battleUIComponents : public virtual battleUIBase 
{
protected:
    //精灵头像显示
    Texture playerTex, enemyTex;
    Sprite playerSprite, enemySprite;

    //站场精灵显示
    Texture playerFieldTex, enemyFieldTex;
    Sprite playerFieldSprite, enemyFieldSprite;
    //血条
    RectangleShape playerHPBarBg, playerHPBar;
    RectangleShape enemyHPBarBg, enemyHPBar;

    //精灵的名字、HP、等级、当前状态的文本
    Text playerNameText, playerHPText;
    Text enemyNameText, enemyHPText;
    Text playerLevelText, enemyLevelText;
    Text playerStatusText;
    Text enemyStatusText;
    //属性关系的图片显示
    Texture advantageRightTex;//向右克制
    Texture advantageLeftTex;//向左克制
    Texture advantageNoneTex;//无克制
    Sprite advantageSprite;

    //当前选中的栏目，这里默认是战斗栏目
    CurrentTab currentTab = CurrentTab::BATTLE;

	//冷却时间的变量
    Clock clickCooldownClock;
    float clickCooldownTime = 0.7f;
    bool isClickOnCooldown = false;

    //下方技能的按钮，我这用的结构体
    struct SkillBtn
    {
        RectangleShape bg;
        Text name;
        Text pp;
        bool enabled = true;
    };
    array<SkillBtn, 4> skillBtns;//这是默认4个技能的

    //右下角的栏目四宫格按钮和左下角的聚能按钮
    struct FuncBtn
    {
        RectangleShape bg;
        Text text;
        bool enabled = true;
    };
    FuncBtn codeBtn, itemBtn, fightBtn, retreatBtn, energyBtn;//对应的就是精灵、道具、战斗、撤退、聚能这几个按钮

    //道具按钮
    vector<FuncBtn> itemBtns;//我这用的vector，因为道具数量不只1个
    FuncBtn prevPageBtn;//上一页按钮
    FuncBtn nextPageBtn;//下一页按钮
    int currentItemPage = 0;//当前道具页码
    int itemsPerPage = 4;//每页显示的道具数量

    //日志
    FuncBtn logBtn;//日志按钮
    bool showLogWindow = false;//是否显示日志窗口
    RectangleShape logWindowBg;//日志窗口背景
    vector<Text> logWindowTexts;//日志窗口中的文本
    int currentLogPage = 0;//当前日志页码
    int logsPerPage = 18;//每页最多显示18行,这个18行是根据窗口的高度和文本的高度计算出来的
    FuncBtn logPrevPageBtn;//日志上一页按钮
    FuncBtn logNextPageBtn;//日志下一页按钮

    //倒计时
    Text countdownText;//倒计时文本
    bool showCountdownWarning = false;//是否显示10秒,就是剩10秒时会有一个红色的倒计时提醒玩家

    //战斗系统
    BattleSystem* battleSystem;//战斗系统指针

    //精灵栏目下的显示
    vector<FuncBtn> codemonBtns;//具体的精灵按钮
    FuncBtn prevCodemonPageBtn;//上一页按钮
    FuncBtn nextCodemonPageBtn;//下一页按钮
    int currentCodemonPage = 0;//当前精灵页码
    int codemonsPerPage = 3;//每页显示的精灵数量，我这设置的是3个，其实一般是6个，但是这里窗口不够大所以就3个

public:
    battleUIComponents() : battleSystem(nullptr) {}
    virtual ~battleUIComponents() = default;
};

//具体实现类
class battleUI : public virtual battleUIBase, public virtual battleUIComponents 
{
public:
    //构造函数
    battleUI();

    //初始化界面的函数
    bool init(const string& fontPath) override;

    //绘制的函数
    void draw(RenderWindow& window) override;

    //更新信息的函数
    void updatePlayer(const Codemon* codemon) override;
    void updateEnemy(const Codemon* codemon) override;
    void updateSkillBtns(const vector<const Move*>& skills) override;
    void addLog(const string& msg) override;
    void updateItemBtns() override;
    void updateCodemonBtns() override;

    //设置属性关系的函数
    void setTypeAdvantage() override;

    //设置战斗系统指针的函数
    void setBattleSystem(BattleSystem* bs) override;

    //鼠标交互的函数
    int handleClick(const Vector2f& mousePos) override;

    //设置精灵头像图片的函数
    void setPlayerSprite(const string& texturePath) override;
    void setEnemySprite(const string& texturePath) override;

    //转为宽字符串的函数
    wstring toWideString(const string& str) override;


private:
    //获取元素名称的函数实现
    wstring getElementName(Element element);
};