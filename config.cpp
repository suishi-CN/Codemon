//对UI我们开发时遇见了两个问题，第一个是字体显示问题，第二个是中文显示问题
//字体显示问题我们用用simhei.ttf这个字体文件，这个字体文件既支持中文又能在SFML中正常显示，所以我们就用这个字体文件了
//中文显示问题我们在网络上找到了一个人给出的解决方案，就是在设置文本字符串时
//选择用宽字符串（wstring）而不是普通字符串（string），这样就能正确显示中文了，所以我们在代码中使用了wstring来设置文本字符串
//我们的UI是根据赛尔号的界面设计的
//可能会看到莫名其妙要加f，这是因为防止下面warning那一直警告说数据类型的转换
#define NOMINMAX//这个是为了防止Windows.h中定义的min和max与std::min和std::max函数冲突
//我们后面有用min和max，结果发现有编译错误，所以我们在包含Windows.h之前定义了这个宏来禁用min和max宏的定义
#include <iostream>
#include<Windows.h>
#include <cmath>
#include <string>
#include "battleUI.h"
#include "../include/Player.h"
#include "../include/PathUtil.h"
#include <SFML/System/String.hpp>

using namespace monbase;
using namespace std;
using namespace sf;
//这里是处理宽字符串的函数，因为我们在UI中使用了宽字符串来显示中文，所以我们需要一个函数来将普通字符串转换为宽字符串，这样才能正确显示中文
//这个处理方法是在网上找到一个网友给出的解决方案,这是原本的网址https://blog.csdn.net/weixin_42600024/article/details/133763347
//因为原本我们用的不是宽字符，所以我这里加了一个转换函数来将普通字符串转换为宽字符串，这样就能正确显示中文了
wstring battleUI::toWideString(const string& str)
{
    if (str.empty()) return L"";

    // 优先尝试将输入当作 UTF-8 解码（严格模式，遇到非法字节会失败）
    int req = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
    if (req > 0)
    {
        // 返回长度包括终止符，所以构造时去掉末尾的 '\0'
        wstring wstr(req - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], req);
        return wstr;
    }

    // 如果不是 UTF-8（例如源码/数据是 ANSI/GBK 编码），回退到系统 ANSI 代码页转换
    req = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (req > 0)
    {
        wstring wstr(req - 1, L'\0');
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], req);
        return wstr;
    }

    // 最后兜底返回空宽字符串
    return L"";
}

//构造函数，初始化一些默认值
battleUI::battleUI() : battleUIComponents()
{
    // 初始化血条和背景图片
    playerHPBarBg.setFillColor(Color(60, 60, 60));    // 设置为深灰色
    enemyHPBarBg.setFillColor(Color(60, 60, 60));
    playerHPBar.setFillColor(Color::Red);             // 设置为红色
    enemyHPBar.setFillColor(Color::Red);
    currentTab = CurrentTab::BATTLE;                 // 默认战斗栏目
    showLogWindow = false;                           // 默认不显示日志窗口，点开才显示
}

//初始化界面的函数，加载资源
bool battleUI::init(const string& fontPath) 
{
    //加载字体
	font.loadFromFile(fontPath);

    //加载背景图
	bgTexture.loadFromFile("Resources/ui_bg.png");
    bgSprite.setTexture(bgTexture);
    bgSprite.setPosition(0, 0);


    //加载克制关系的图片
    loadTextureFromUtf8(advantageRightTex, "Resources/左向右克制.png");
    loadTextureFromUtf8(advantageLeftTex, "Resources/右向左克制.png");
    loadTextureFromUtf8(advantageNoneTex, "Resources/左向右普通.png");
    setTypeAdvantage();
    advantageSprite.setPosition(450, 50);

	//加载精灵头像的纹理，这里先创建一个空白的纹理，后面会设置具体的图片
    playerTex.create(100, 100);
    enemyTex.create(100, 100);

    //创建站场精灵纹理
    playerFieldTex.create(128, 128);
    enemyFieldTex.create(128, 128);

    //精灵设置
    playerSprite.setTexture(playerTex);
    enemySprite.setTexture(enemyTex);
    playerSprite.setPosition(50, 30);
    enemySprite.setPosition(848, 30);
    playerFieldSprite.setTexture(playerFieldTex);
    enemyFieldSprite.setTexture(enemyFieldTex);
    playerFieldSprite.setPosition(10, 200);      // 左边距150像素
    enemyFieldSprite.setPosition(722, 200);      // 右边距150像素（1000-128-150=722）


    //血条设置
	playerHPBarBg.setSize(Vector2f(250, 18));//这里SFML默认的用Vector2f来设置大小，参数是宽和高，这里我们设置宽250，高18，和赛尔号的血条差不多
    playerHPBarBg.setPosition(162, 65);
    playerHPBar.setPosition(162, 65);
    enemyHPBarBg.setSize(Vector2f(250, 18));
    enemyHPBarBg.setPosition(588, 65);
    enemyHPBar.setPosition(588, 65);

    //下面文本的设置
    //精灵名字
    playerNameText.setFont(font);
    playerNameText.setCharacterSize(22);
    playerNameText.setFillColor(Color::White);
    playerNameText.setPosition(75, 10);

    //敌人名字
    enemyNameText.setFont(font);
    enemyNameText.setCharacterSize(22);
    enemyNameText.setFillColor(Color::White);
    enemyNameText.setPosition(848, 10);

    //玩家血条文本的显示
    playerHPText.setFont(font);
    playerHPText.setCharacterSize(16);
    playerHPText.setFillColor(Color::Yellow);
    playerHPText.setPosition(170, 90);

    //敌人血条文本的显示
    enemyHPText.setFont(font);
    enemyHPText.setCharacterSize(16);
    enemyHPText.setFillColor(Color::Yellow);
    enemyHPText.setPosition(595, 90);

    //玩家精灵等级文本显示
    playerLevelText.setFont(font);
    playerLevelText.setCharacterSize(18);
    playerLevelText.setFillColor(Color::Yellow);
    playerLevelText.setPosition(60, 5);

    enemyLevelText.setFont(font);
    enemyLevelText.setCharacterSize(18);
    enemyLevelText.setFillColor(Color::Yellow);
    enemyLevelText.setPosition(860, 5);

    //玩家状态文本
    playerStatusText.setFont(font);
    playerStatusText.setCharacterSize(16);
    playerStatusText.setFillColor(Color::Red);
    playerStatusText.setStyle(Text::Bold);//加粗
    playerStatusText.setString(L"");//默认是没有的

    //敌人状态文本
    enemyStatusText.setFont(font);
    enemyStatusText.setCharacterSize(16);
    enemyStatusText.setFillColor(Color::Red);
    enemyStatusText.setStyle(Text::Bold);
    enemyStatusText.setString(L"");

    //倒计时文本
    countdownText.setFont(font);
    countdownText.setCharacterSize(48);  // 大字体
    countdownText.setFillColor(Color::White);//默认白色后面会改成红的
    countdownText.setStyle(Text::Bold);
    countdownText.setPosition(470, 300);//屏幕中央位置
    countdownText.setString("30");

    //下面是设置按钮的
    //右下角的四宫格
    float fw = 125.0f;
    float fh = 49.0f;
    float gap = 12.0f;
	float windowWidth = 1000.0f;//这个是我们窗口的宽度，后面计算按钮位置的时候会用到
    float windowHeight = 731.0f;//这个是窗口的高度
    float startX = windowWidth - (2 * fw + gap) - 20.0f;
    float startY = windowHeight - (2 * fh + gap) - 20.0f;

    //战斗按钮
    fightBtn.bg.setSize(Vector2f(fw, fh));
    fightBtn.bg.setFillColor(Color(100, 100, 200));
    fightBtn.bg.setPosition(startX, startY);
    fightBtn.text.setFont(font);
    fightBtn.text.setString(L"战斗");
    fightBtn.text.setCharacterSize(20);
    fightBtn.text.setFillColor(Color::White);
    fightBtn.text.setPosition(fightBtn.bg.getPosition().x + 20.0f, fightBtn.bg.getPosition().y + 10.0f);
    fightBtn.enabled = true;//这里是默认一直显示的

	//道具按钮
    itemBtn.bg.setSize(Vector2f(fw, fh));
    itemBtn.bg.setFillColor(Color(60, 60, 100));
    itemBtn.bg.setPosition(startX + fw + gap, startY);
    itemBtn.text.setFont(font);
    itemBtn.text.setString(L"道具");
    itemBtn.text.setCharacterSize(20);
    itemBtn.text.setFillColor(Color::White);
    itemBtn.text.setPosition(itemBtn.bg.getPosition().x + 20.0f, itemBtn.bg.getPosition().y + 10.0f);
    itemBtn.enabled = true;

	//精灵按钮和撤退按钮在第二行，所以Y坐标要加上一个按钮的高度和间隔
    float secondRowY = startY + fh + gap;

	//精灵按钮
    codeBtn.bg.setSize(Vector2f(fw, fh));
    codeBtn.bg.setFillColor(Color(60, 60, 100));
    codeBtn.bg.setPosition(startX, secondRowY);
    codeBtn.text.setFont(font);
    codeBtn.text.setString(L"精灵");
    codeBtn.text.setCharacterSize(20);
    codeBtn.text.setFillColor(Color::White);
    codeBtn.text.setPosition(codeBtn.bg.getPosition().x + 20.0f, codeBtn.bg.getPosition().y + 10.0f);
    codeBtn.enabled = true;

	//撤退按钮
    retreatBtn.bg.setSize(Vector2f(fw, fh));
    retreatBtn.bg.setFillColor(Color(60, 60, 100));
    retreatBtn.bg.setPosition(startX + fw + gap, secondRowY);
    retreatBtn.text.setFont(font);
    retreatBtn.text.setString(L"撤退");
    retreatBtn.text.setCharacterSize(20);
    retreatBtn.text.setFillColor(Color::White);
    retreatBtn.text.setPosition(retreatBtn.bg.getPosition().x + 20.0f, retreatBtn.bg.getPosition().y + 10.0f);
    retreatBtn.enabled = true;

	//这里是战斗栏目下的按钮的初始化，战斗栏目下有一个聚能按钮和四个技能按钮，这些按钮是只有当玩家选择战斗栏目时才会启用
    float skillBtnWidth = 120.0f;
    float skillBtnHeight = 45.0f;
    float skillGap = 10.0f;

    //计算战斗栏目按钮的起始位置
    float totalSkillWidth = skillBtnWidth * 5 + skillGap * 4;
    float skillStartX = startX - totalSkillWidth - 50.0f;//整体左移50像素，避免重叠
    float skillStartY = startY;//与四宫格第一行按钮同一水平线

    //聚能的按钮
    energyBtn.bg.setSize(Vector2f(skillBtnWidth, skillBtnHeight));
    energyBtn.bg.setFillColor(Color(100, 150, 100));//绿色表示和一般的技能不一样
    energyBtn.bg.setPosition(skillStartX, skillStartY);
    energyBtn.text.setFont(font);
    energyBtn.text.setString(L"聚能\n0/10");
    energyBtn.text.setCharacterSize(16);
    energyBtn.text.setFillColor(Color::White);
	//得保证文本在按钮中间显示，这里我们先获取文本的边界框，然后设置文本的原点为边界框的中心，这样就能保证文本在按钮中间显示了
    FloatRect textRect = energyBtn.text.getLocalBounds();
    energyBtn.text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    energyBtn.text.setPosition(energyBtn.bg.getPosition().x + skillBtnWidth / 2.0f,energyBtn.bg.getPosition().y + skillBtnHeight / 2.0f);
    energyBtn.enabled = true;  // 默认启用

    //中间下面四个技能按钮
    for (int i = 0; i < 4; i++) 
    {
        skillBtns[i].bg.setSize(Vector2f(skillBtnWidth, skillBtnHeight));
        skillBtns[i].bg.setFillColor(Color(80, 80, 150));//蓝色
        skillBtns[i].bg.setPosition(skillStartX + skillBtnWidth * (i + 1) + skillGap * (i + 1), skillStartY);
        skillBtns[i].name.setFont(font);
        skillBtns[i].name.setCharacterSize(14);
        skillBtns[i].name.setFillColor(Color::White);//技能名字我设置的是白色
        skillBtns[i].pp.setFont(font);
        skillBtns[i].pp.setCharacterSize(12);
		skillBtns[i].pp.setFillColor(Color::Yellow);//技能的耗能我设置成黄色，这样和技能名字区分开来
        //设置一个默认的文本
        skillBtns[i].name.setString(L"技能" + to_wstring(i + 1));
        //技能按钮显示两行文本
        skillBtns[i].pp.setString(L"耗能：0\n威力：0");
        //和聚能的文本设置位置情况一样
        FloatRect nameRect = skillBtns[i].name.getLocalBounds();
        skillBtns[i].name.setOrigin(nameRect.left + nameRect.width / 2.0f, nameRect.top + nameRect.height / 2.0f);
        skillBtns[i].name.setPosition(skillBtns[i].bg.getPosition().x + skillBtnWidth / 2.0f, skillBtns[i].bg.getPosition().y + skillBtnHeight / 2.0f - 10.0f );
        FloatRect ppRect = skillBtns[i].pp.getLocalBounds();
        skillBtns[i].pp.setOrigin(ppRect.left + ppRect.width / 2.0f, ppRect.top + ppRect.height / 2.0f);
        skillBtns[i].pp.setPosition(skillBtns[i].bg.getPosition().x + skillBtnWidth / 2.0f,skillBtns[i].bg.getPosition().y + skillBtnHeight / 2.0f + 10.0f );
		skillBtns[i].enabled = false;//这里默认是禁用的，只有当玩家选择战斗栏目才会启用
    }

    //日志按钮
    logBtn.bg.setSize(Vector2f(60.0f, 40.0f));
    logBtn.bg.setFillColor(Color(80, 80, 80, 200));//灰色并且半透明
    logBtn.bg.setPosition(windowWidth - 80.0f, 180.0f);//我这里放在屏幕的右端上面一点点
    logBtn.text.setFont(font);
    logBtn.text.setString(L"日志");
    logBtn.text.setCharacterSize(16);
    logBtn.text.setFillColor(Color::White);
    FloatRect logTextRect = logBtn.text.getLocalBounds();
    logBtn.text.setOrigin(logTextRect.left + logTextRect.width / 2.0f, logTextRect.top + logTextRect.height / 2.0f);
    logBtn.text.setPosition(logBtn.bg.getPosition().x + logBtn.bg.getSize().x / 2.0f,logBtn.bg.getPosition().y + logBtn.bg.getSize().y / 2.0f);
    logBtn.enabled = true;//默认一直开启的，赛尔号就是这样的

    //日志窗口
    logWindowBg.setSize(Vector2f(400.0f, 500.0f));
    logWindowBg.setFillColor(Color(30, 30, 30, 230));//深灰色半透明
    logWindowBg.setOutlineColor(Color::White);
    logWindowBg.setOutlineThickness(2.0f);
    logWindowBg.setPosition((windowWidth - 400.0f) / 2.0f, (windowHeight - 500.0f) / 2.0f);

	//四个道具按钮的设置，道具按钮在战斗按钮的右侧，所以X坐标要加上战斗按钮的宽度和间隔，Y坐标和战斗按钮一样
    float itemBtnWidth = 120.0f;
    float itemBtnHeight = 45.0f;
    float itemGap = 10.0f;

    //计算道具按钮的起始位置(我这里直接跟技能的按钮对齐了）
    float totalItemWidth = itemBtnWidth * 4 + itemGap * 3;
    float itemStartX = skillStartX + 150;//使用技能按钮的起始X坐标
    float itemStartY = skillStartY;//使用技能按钮的起始Y坐标

    //4个道具按钮具体的设置
	itemBtns.resize(4);//resize一下，保证有4个元素,不会超出范围
    for (int i = 0; i < 4; i++) 
    {
        itemBtns[i].bg.setSize(Vector2f(itemBtnWidth, itemBtnHeight));
        itemBtns[i].bg.setFillColor(Color(150, 100, 50));//棕色系
        itemBtns[i].bg.setPosition(itemStartX + i * (itemBtnWidth + itemGap), itemStartY);
        itemBtns[i].text.setFont(font);
        itemBtns[i].text.setCharacterSize(12);
        itemBtns[i].text.setFillColor(Color::White);
        itemBtns[i].text.setString(L"空");
        itemBtns[i].enabled = false;
    }

    //换页按钮（我这里直接用技能的规格了）
    float pageBtnWidth = skillBtnWidth;
    float pageBtnHeight = skillBtnHeight / 2.0f;

    //上一页的按钮
    prevPageBtn.bg.setSize(Vector2f(pageBtnWidth, pageBtnHeight));
    prevPageBtn.bg.setFillColor(Color(100, 100, 150));//他们说紫色有韵味
    prevPageBtn.bg.setPosition(energyBtn.bg.getPosition().x, energyBtn.bg.getPosition().y);
    prevPageBtn.text.setFont(font);
    prevPageBtn.text.setString(L"上一页");
    prevPageBtn.text.setCharacterSize(14);
    prevPageBtn.text.setFillColor(Color::White);
    FloatRect prevTextRect = prevPageBtn.text.getLocalBounds();
    prevPageBtn.text.setOrigin(prevTextRect.left + prevTextRect.width / 2.0f, prevTextRect.top + prevTextRect.height / 2.0f);
    prevPageBtn.text.setPosition(prevPageBtn.bg.getPosition().x + pageBtnWidth / 2.0f,prevPageBtn.bg.getPosition().y + pageBtnHeight / 2.0f);
    prevPageBtn.enabled = false;

    //下一页按钮
    nextPageBtn.bg.setSize(Vector2f(pageBtnWidth, pageBtnHeight));
    nextPageBtn.bg.setFillColor(Color(100, 100, 150));
    nextPageBtn.bg.setPosition(energyBtn.bg.getPosition().x, energyBtn.bg.getPosition().y + pageBtnHeight + 5.0f);
    nextPageBtn.text.setFont(font);
    nextPageBtn.text.setString(L"下一页");
    nextPageBtn.text.setCharacterSize(14);
    nextPageBtn.text.setFillColor(Color::White);
    FloatRect nextTextRect = nextPageBtn.text.getLocalBounds();
    nextPageBtn.text.setOrigin(nextTextRect.left + nextTextRect.width / 2.0f, nextTextRect.top + nextTextRect.height / 2.0f);
    nextPageBtn.text.setPosition(nextPageBtn.bg.getPosition().x + pageBtnWidth / 2.0f,nextPageBtn.bg.getPosition().y + pageBtnHeight / 2.0f);
    nextPageBtn.enabled = false;  // 默认禁用

    //三个精灵的按钮,我这里直接和技能的一样了
    float codemonBtnWidth = 150.0f;
    float codemonBtnHeight = 80.0f;
    float codemonGap = 10.0f;
    float codemonStartX = 200.0f;
    float codemonStartY = skillStartY;
    codemonBtns.resize(3);
    for (int i = 0; i < 3; i++) 
    {
        codemonBtns[i].bg.setSize(Vector2f(codemonBtnWidth, codemonBtnHeight));
        codemonBtns[i].bg.setFillColor(Color(100, 150, 100));//绿色
        codemonBtns[i].bg.setPosition(codemonStartX + i * (codemonBtnWidth + codemonGap), codemonStartY);
        codemonBtns[i].text.setFont(font);
        codemonBtns[i].text.setCharacterSize(12);
        codemonBtns[i].text.setFillColor(Color::White);
        codemonBtns[i].text.setString(L"空\n属性：无\n克制：普通");
        FloatRect textRect = codemonBtns[i].text.getLocalBounds();
        codemonBtns[i].text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        codemonBtns[i].text.setPosition(
        codemonBtns[i].bg.getPosition().x + codemonBtnWidth / 2.0f,codemonBtns[i].bg.getPosition().y + codemonBtnHeight / 2.0f);
        codemonBtns[i].enabled = false;  // 默认禁用
    }

	//精灵的分页按钮，我直接抄道具的分页按钮规格了
    float codemonPageBtnWidth = skillBtnWidth;
    float codemonPageBtnHeight = skillBtnHeight / 2.0f;

    //上一页按钮
    prevCodemonPageBtn.bg.setSize(Vector2f(codemonPageBtnWidth, codemonPageBtnHeight));
    prevCodemonPageBtn.bg.setFillColor(Color(100, 100, 150)); 
    prevCodemonPageBtn.bg.setPosition(energyBtn.bg.getPosition().x, energyBtn.bg.getPosition().y);
    prevCodemonPageBtn.text.setFont(font);
    prevCodemonPageBtn.text.setString(L"上一页");
    prevCodemonPageBtn.text.setCharacterSize(14);
    prevCodemonPageBtn.text.setFillColor(Color::White);
    FloatRect prevCodemonTextRect = prevCodemonPageBtn.text.getLocalBounds();
    prevCodemonPageBtn.text.setOrigin(prevCodemonTextRect.left + prevCodemonTextRect.width / 2.0f, prevCodemonTextRect.top + prevCodemonTextRect.height / 2.0f);
    prevCodemonPageBtn.text.setPosition(prevCodemonPageBtn.bg.getPosition().x + codemonPageBtnWidth / 2.0f,prevCodemonPageBtn.bg.getPosition().y + codemonPageBtnHeight / 2.0f );
    prevCodemonPageBtn.enabled = false;

    //下一页按钮
    nextCodemonPageBtn.bg.setSize(Vector2f(codemonPageBtnWidth, codemonPageBtnHeight));
    nextCodemonPageBtn.bg.setFillColor(Color(100, 100, 150));
    nextCodemonPageBtn.bg.setPosition(energyBtn.bg.getPosition().x, energyBtn.bg.getPosition().y + codemonPageBtnHeight + 5.0f);
    nextCodemonPageBtn.text.setFont(font);
    nextCodemonPageBtn.text.setString(L"下一页");
    nextCodemonPageBtn.text.setCharacterSize(14);
    nextCodemonPageBtn.text.setFillColor(Color::White);
    FloatRect nextCodemonTextRect = nextCodemonPageBtn.text.getLocalBounds();
    nextCodemonPageBtn.text.setOrigin(nextCodemonTextRect.left + nextCodemonTextRect.width / 2.0f, nextCodemonTextRect.top + nextCodemonTextRect.height / 2.0f);
    nextCodemonPageBtn.text.setPosition(nextCodemonPageBtn.bg.getPosition().x + codemonPageBtnWidth / 2.0f,nextCodemonPageBtn.bg.getPosition().y + codemonPageBtnHeight / 2.0f);
    nextCodemonPageBtn.enabled = false;

    //日志窗口里也有一个换页的按钮，我放窗口的右下角
    float logPageBtnWidth = 60.0f;
    float logPageBtnHeight = 30.0f;
    //上一页按钮
    logPrevPageBtn.bg.setSize(Vector2f(logPageBtnWidth, logPageBtnHeight));
    logPrevPageBtn.bg.setFillColor(Color(100, 100, 150));
    logPrevPageBtn.bg.setPosition(logWindowBg.getPosition().x + 20.0f,logWindowBg.getPosition().y + logWindowBg.getSize().y - logPageBtnHeight - 20.0f);
    logPrevPageBtn.text.setFont(font);
    logPrevPageBtn.text.setString(L"上一页");
    logPrevPageBtn.text.setCharacterSize(14);
    logPrevPageBtn.text.setFillColor(Color::White);
    FloatRect logPrevTextRect = logPrevPageBtn.text.getLocalBounds();
    logPrevPageBtn.text.setOrigin(logPrevTextRect.left + logPrevTextRect.width / 2.0f,logPrevTextRect.top + logPrevTextRect.height / 2.0f);
    logPrevPageBtn.text.setPosition(logPrevPageBtn.bg.getPosition().x + logPageBtnWidth / 2.0f,logPrevPageBtn.bg.getPosition().y + logPageBtnHeight / 2.0f);
    logPrevPageBtn.enabled = false;
    // 下一页按钮
    logNextPageBtn.bg.setSize(Vector2f(logPageBtnWidth, logPageBtnHeight));
    logNextPageBtn.bg.setFillColor(Color(100, 100, 150));  // 紫色系
    logNextPageBtn.bg.setPosition(logWindowBg.getPosition().x + logWindowBg.getSize().x - logPageBtnWidth - 20.0f,logWindowBg.getPosition().y + logWindowBg.getSize().y - logPageBtnHeight - 20.0f);
    logNextPageBtn.text.setFont(font);
    logNextPageBtn.text.setString(L"下一页");
    logNextPageBtn.text.setCharacterSize(14);
    logNextPageBtn.text.setFillColor(Color::White);
    FloatRect logNextTextRect = logNextPageBtn.text.getLocalBounds();
    logNextPageBtn.text.setOrigin(logNextTextRect.left + logNextTextRect.width / 2.0f,logNextTextRect.top + logNextTextRect.height / 2.0f);
    logNextPageBtn.text.setPosition(logNextPageBtn.bg.getPosition().x + logPageBtnWidth / 2.0f,logNextPageBtn.bg.getPosition().y + logPageBtnHeight / 2.0f);logNextPageBtn.enabled = false;  // 默认禁用

    return true;
}

//这个是绘制的函数
void battleUI::draw(RenderWindow& window) 
{
    //画背景
    window.draw(bgSprite);
	//画精灵
    window.draw(playerSprite);
    window.draw(enemySprite);
    window.draw(playerFieldSprite);
    window.draw(enemyFieldSprite);
	//画血条和血条背景
    window.draw(playerHPBarBg);
    window.draw(playerHPBar);
    window.draw(enemyHPBarBg);
    window.draw(enemyHPBar);
	//画文本
    window.draw(playerNameText);
    window.draw(playerHPText);
    window.draw(enemyNameText);
    window.draw(enemyHPText);
    window.draw(playerLevelText);
    window.draw(enemyLevelText);
    window.draw(playerStatusText);
    window.draw(enemyStatusText);
    //画克制关系
    window.draw(advantageSprite);

    //依照现在选择的栏目画按钮
    switch (currentTab) 
    {
	    case CurrentTab::BATTLE: //战斗栏目是显示聚能和技能按钮
        {
            if (energyBtn.enabled) 
            {
                window.draw(energyBtn.bg);
                window.draw(energyBtn.text);
            }
            for (auto& btn : skillBtns) 
            {
                if (btn.enabled) 
                {
                    window.draw(btn.bg);
                    window.draw(btn.name);
                    window.draw(btn.pp);
                }
            }
            break;
        }
		case CurrentTab::ITEM: //道具栏目是显示道具按钮和分页按钮
        {
            for (auto& btn : itemBtns) 
            {
                if (btn.enabled) 
                {
                    window.draw(btn.bg);
                    window.draw(btn.text);
                }
            }
            if (prevPageBtn.enabled) 
            {
                window.draw(prevPageBtn.bg);
                window.draw(prevPageBtn.text);
            }
            if (nextPageBtn.enabled) 
            {
                window.draw(nextPageBtn.bg);
                window.draw(nextPageBtn.text);
            }
            break;
        }
		case CurrentTab::CODEMON: //精灵栏目是显示精灵按钮和分页按钮
        {
            for (auto& btn : codemonBtns) 
            {
                if (btn.enabled)
                {
                    window.draw(btn.bg);
                    window.draw(btn.text);
                }
            }
            if (prevCodemonPageBtn.enabled) 
            {
                window.draw(prevCodemonPageBtn.bg);
                window.draw(prevCodemonPageBtn.text);
            }
            if (nextCodemonPageBtn.enabled) 
            {
                window.draw(nextCodemonPageBtn.bg);
                window.draw(nextCodemonPageBtn.text);
            }
            break;
        }
		case CurrentTab::RETREAT: //这个是画撤退的
        {
            // 撤退栏目：无额外按钮
            break;
        }
    }

    if (battleSystem && battleSystem->getState() == BattleState::WAITING_FOR_SWITCH) 
    {
        //当精灵死亡时，只能显示精灵按钮因为只能换精灵
        window.draw(codeBtn.bg);
        window.draw(codeBtn.text);
        currentTab = CurrentTab::CODEMON;
        //绘制精灵按钮和上面一样的
        for (auto& btn : codemonBtns) 
        {
            if (btn.enabled) 
            {
                window.draw(btn.bg);
                window.draw(btn.text);
            }
        }
        if (prevCodemonPageBtn.enabled) 
        {
            window.draw(prevCodemonPageBtn.bg);
            window.draw(prevCodemonPageBtn.text);
        }
        if (nextCodemonPageBtn.enabled)
        {
            window.draw(nextCodemonPageBtn.bg);
            window.draw(nextCodemonPageBtn.text);
        }
    }
	else //正常情况下是显示四宫格按钮的
    {
        window.draw(fightBtn.bg);
        window.draw(fightBtn.text);
        window.draw(itemBtn.bg);
        window.draw(itemBtn.text);
        window.draw(codeBtn.bg);
        window.draw(codeBtn.text);
        window.draw(retreatBtn.bg);
        window.draw(retreatBtn.text);
    }
    //绘制日志按钮
    window.draw(logBtn.bg);
    window.draw(logBtn.text);
	//在打开日志窗口时会画日志窗口和里面的文本
    if (showLogWindow) 
    {
        window.draw(logWindowBg);
        //绘制日志文本
        for (auto& logText : logWindowTexts)
        {
            window.draw(logText);
        }
        if (logPrevPageBtn.enabled) 
        {
            window.draw(logPrevPageBtn.bg);
            window.draw(logPrevPageBtn.text);
        }
        if (logNextPageBtn.enabled) 
        {
            window.draw(logNextPageBtn.bg);
            window.draw(logNextPageBtn.text);
        }
    }

   //画倒计时
	if (battleSystem && !showLogWindow) //只在日志窗口没有打开的时候显示倒计时，打开日志窗口就不显示了，要不然日志和倒计时重了
    {
        BattleState state = battleSystem->getState();
        if (state == BattleState::PLAYER_TURN ||state == BattleState::WAITING_FOR_SWITCH ||state == BattleState::BATTLE_END) 
        {
            float remaining = battleSystem->getRemainingTime();
            if (remaining > 0.0f) 
            {
				//文本设置为剩余时间的整数部分加1因为有一个int向下取整了，比如1.5我这里会显示2的
                countdownText.setString(to_string(int(remaining) + 1));
				//我这里小于10秒的时候会变红闪烁
                if (remaining <= 10.0f) 
                {
                    countdownText.setFillColor(Color::Red);
					//闪烁效果我用的取模实现0.5秒闪烁一次，剩余时间乘以2取模，如果结果小于1就显示，否则不显示，这样就能实现0.5秒闪烁一次了
                    if (int(remaining * 2) % 2 == 0) 
                    {
                        window.draw(countdownText);
                    }
                }
                else 
                {
                    countdownText.setFillColor(Color::White);
                    window.draw(countdownText);
                }
            }
        }
    }
}

//这个是处理点击的函数，参数是鼠标点击的位置，返回值是一个int，对应点击了哪个按钮
int battleUI::handleClick(const Vector2f& mousePos) 
{
    //检查冷却时间
    if (isClickOnCooldown) 
    {
        float elapsed = clickCooldownClock.getElapsedTime().asSeconds();
        if (elapsed < clickCooldownTime) 
        {
            return -1;  //还在冷却中,忽略点击
        }
        else
        {
            isClickOnCooldown = false; //冷却结束
        }
    }
	//我们先处理日志的因为这个特殊一点，日志按钮是一直显示的，不管在哪个栏目，所以我们先检测这个按钮的点击
    //如果点击了这个按钮，我们就切换日志窗口的显示状态
    if (logBtn.bg.getGlobalBounds().contains(mousePos)) 
    {
		showLogWindow = !showLogWindow;//切换日志窗口的显示状态
        if (showLogWindow && battleSystem) 
        {
            const vector<string>& battleLogs = battleSystem->getBattleLog();
			logWindowTexts.clear();//先清空之前的日志文本
            // 计算总页数
            int totalLogPages = (battleLogs.size() + logsPerPage - 1) / logsPerPage;
            if (totalLogPages == 0)
            {
                totalLogPages = 1;
            }
            // 确保当前页有效
            if (currentLogPage >= totalLogPages)
            {
                currentLogPage = totalLogPages - 1;
            }
            if (currentLogPage < 0)
            {
                currentLogPage = 0;
            }
            //计算当前页的起始和结束索引
            int startIndex = currentLogPage * logsPerPage;
            int endIndex = min(startIndex + logsPerPage, int(battleLogs.size()));
            // 设置日志文本位置
            float startX = logWindowBg.getPosition().x + 20.0f;
            float startY = logWindowBg.getPosition().y + 20.0f;
            for (int i = startIndex; i < endIndex; i++) 
            {
                Text logText;
                logText.setFont(font);
                logText.setCharacterSize(18);
                logText.setFillColor(Color::White);
                wstring wideMsg = toWideString(battleLogs[i]);
                logText.setString(wideMsg);
                logText.setPosition(startX, startY + (i - startIndex) * 25.0f);//每行间隔25个单位长度
                logWindowTexts.push_back(logText);
            }
            //更新换页按钮的状态
            logPrevPageBtn.enabled = (currentLogPage > 0);
            logNextPageBtn.enabled = (currentLogPage < totalLogPages - 1);
        }
        isClickOnCooldown = true;
        clickCooldownClock.restart();
        return -1;
    }
    //如果日志窗口是打开的，我们就从战斗系统里获取日志文本，然后根据当前页数来显示对应的日志文本
    //同时更新换页按钮的状态，最后返回-1表示处理了这个点击事件，就不需要继续往下检测其他按钮了
    if (showLogWindow) 
    {
        //处理上一页
        if (logPrevPageBtn.enabled && logPrevPageBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            currentLogPage--;
            //重新生成当前页日志
			//下面的操作和上面打开日志窗口时生成日志文本的操作是一样
            const vector<string>& battleLogs = battleSystem->getBattleLog();
            logWindowTexts.clear();
            int totalLogPages = (battleLogs.size() + logsPerPage - 1) / logsPerPage;
            int startIndex = currentLogPage * logsPerPage;
            int endIndex = min(startIndex + logsPerPage, int(battleLogs.size()));
            float startX = logWindowBg.getPosition().x + 20.0f;
            float startY = logWindowBg.getPosition().y + 20.0f;
            for (int i = startIndex; i < endIndex; i++) 
            {
                Text logText;
                logText.setFont(font);
                logText.setCharacterSize(18);
                logText.setFillColor(Color::White);
                wstring wideMsg = toWideString(battleLogs[i]);
                logText.setString(wideMsg);
                logText.setPosition(startX, startY + (i - startIndex) * 25.0f);
                logWindowTexts.push_back(logText);
            }
            //更新按钮状态
            logPrevPageBtn.enabled = (currentLogPage > 0);
            logNextPageBtn.enabled = (currentLogPage < totalLogPages - 1);
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
		//处理下一页
        if (logNextPageBtn.enabled && logNextPageBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            currentLogPage++;
            //重新生成当前页日志
            //依旧一样
            const vector<string>& battleLogs = battleSystem->getBattleLog();
            logWindowTexts.clear();
            int totalLogPages = (battleLogs.size() + logsPerPage - 1) / logsPerPage;
            int startIndex = currentLogPage * logsPerPage;
            int endIndex = min(startIndex + logsPerPage, int(battleLogs.size()));
            float startX = logWindowBg.getPosition().x + 20.0f;
            float startY = logWindowBg.getPosition().y + 20.0f;
            for (int i = startIndex; i < endIndex; i++) 
            {
                Text logText;
                logText.setFont(font);
                logText.setCharacterSize(18);
                logText.setFillColor(Color::White);
                wstring wideMsg = toWideString(battleLogs[i]);
                logText.setString(wideMsg);
                logText.setPosition(startX, startY + (i - startIndex) * 25.0f);
                logWindowTexts.push_back(logText);
            }
            //更新按钮状态
            logPrevPageBtn.enabled = (currentLogPage > 0);
            logNextPageBtn.enabled = (currentLogPage < totalLogPages - 1);
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
    }

	//现在处理完日志后我们特殊处理一下精灵死亡时的情况，在这个状态下我们只允许点击精灵按钮
    if (battleSystem && battleSystem->getState() == BattleState::WAITING_FOR_SWITCH)
    {
        //这里更新按钮的高亮状态，只有精灵栏目按钮高亮，其他按钮都暗淡
        fightBtn.bg.setFillColor(Color(60, 60, 100));
        itemBtn.bg.setFillColor(Color(60, 60, 100));
        codeBtn.bg.setFillColor(Color(100, 100, 200));
        retreatBtn.bg.setFillColor(Color(60, 60, 100));
        updateCodemonBtns();
        //现在是在精灵栏目内的点击就是点击那三个精灵按钮或者分页按钮
        //检测三个精灵按钮
        for (int i = 0; i < codemonBtns.size(); i++)
        {
            if (codemonBtns[i].enabled && codemonBtns[i].bg.getGlobalBounds().contains(mousePos))
            {
                if (battleSystem && battleSystem->getPlayer())
                {
                    int startIndex = currentCodemonPage * codemonsPerPage;
                    int codemonIndex = startIndex + i;
                    //获取精灵
                    Codemon* codemon = battleSystem->getPlayerCodemon(codemonIndex);
                    if (codemon && codemon->getHP() > 0) 
                    {
                        //切换精灵
                        battleSystem->executePlayerAction(10, codemonIndex);
                        //更新玩家精灵图片
                        string texturePath = "Resources/" + codemon->getName() + ".png";
                        setPlayerSprite(texturePath);
                        //更新精灵的信息
                        updatePlayer(codemon);
                        //更新敌人信息
                        if (battleSystem->getActiveEnemy()) 
                        {
                            updateEnemy(battleSystem->getActiveEnemy());
                        }
                        // 更新UI
                        updateCodemonBtns();
                        isClickOnCooldown = true;
                        clickCooldownClock.restart();
                        return -1;
                    }
                }
                return -1;
            }
        }
        //检测上一页按钮
        if (prevCodemonPageBtn.enabled && prevCodemonPageBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            currentCodemonPage--;
            updateCodemonBtns();
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
        //检测下一页按钮
        if (nextCodemonPageBtn.enabled && nextCodemonPageBtn.bg.getGlobalBounds().contains(mousePos))
        {
            currentCodemonPage++;
            updateCodemonBtns();
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
        //其他按钮点击无效
        return -1;
    }

    //四宫格功能按钮
    //战斗栏目
    if (fightBtn.bg.getGlobalBounds().contains(mousePos)) 
    {
        currentTab = CurrentTab::BATTLE;
        //更新高亮
        fightBtn.bg.setFillColor(Color(100, 100, 200));
        itemBtn.bg.setFillColor(Color(60, 60, 100));
        codeBtn.bg.setFillColor(Color(60, 60, 100));
        retreatBtn.bg.setFillColor(Color(60, 60, 100));
        isClickOnCooldown = true;
        clickCooldownClock.restart();
        return 5; // 战斗栏目
    }
	//道具栏目
    if (itemBtn.bg.getGlobalBounds().contains(mousePos)) 
    {
        currentTab = CurrentTab::ITEM;
        // 更新高亮
        fightBtn.bg.setFillColor(Color(60, 60, 100));
        itemBtn.bg.setFillColor(Color(100, 100, 200));
        codeBtn.bg.setFillColor(Color(60, 60, 100));
        retreatBtn.bg.setFillColor(Color(60, 60, 100));
        isClickOnCooldown = true;
        clickCooldownClock.restart();
        return 6; // 道具栏目
    }
	//精灵栏目
    if (codeBtn.bg.getGlobalBounds().contains(mousePos)) 
    {
        currentTab = CurrentTab::CODEMON;
        // 更新高亮
        fightBtn.bg.setFillColor(Color(60, 60, 100));
        itemBtn.bg.setFillColor(Color(60, 60, 100));
        codeBtn.bg.setFillColor(Color(100, 100, 200));
        retreatBtn.bg.setFillColor(Color(60, 60, 100));
        isClickOnCooldown = true;
        clickCooldownClock.restart();
        updateCodemonBtns();
        return 7; // 精灵栏目
    }
	//撤退栏目
    if (retreatBtn.bg.getGlobalBounds().contains(mousePos)) 
    {
        currentTab = CurrentTab::RETREAT;
        isClickOnCooldown = true;
        clickCooldownClock.restart();
        return 8; // 撤退
    }

    //根据当前栏目检测点击
    switch (currentTab) 
    {
	case CurrentTab::BATTLE: //战斗栏目是检测聚能和技能按钮
    {
        //检测聚能按钮
        if (energyBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            battleSystem->executePlayerAction(4, 0);
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }

        //检测技能按钮
        for (int i = 0; i < 4; i++) 
        {
            if (skillBtns[i].enabled && skillBtns[i].bg.getGlobalBounds().contains(mousePos))
            {
                    battleSystem->executePlayerAction(i, 0); 
                    isClickOnCooldown = true;
                    clickCooldownClock.restart();
                return -1;
            }
        }
        break;
    }
	case CurrentTab::ITEM: //道具栏目是检测道具按钮和分页按钮
    {
        //检测道具按钮
        for (int i = 0; i < itemBtns.size(); i++) 
        {
            if (itemBtns[i].enabled && itemBtns[i].bg.getGlobalBounds().contains(mousePos)) 
            {
                if (battleSystem && battleSystem->getPlayer()) 
                {
                    Player* player = battleSystem->getPlayer();
                    Bag& bag = player->bag;
                    //获取所有道具
                    const auto& allItems = ItemDB::getAllItems();
                    //收集玩家拥有的道具
                    vector<pair<ItemID, int>> ownedItems;
                    for (const auto& pair : allItems)
                    {
                        ItemID id = pair.first;
                        int count = bag.getItemCount(id);
                        if (count > 0)
                        {
                            ownedItems.push_back({ id, count });
                        }
                    }
                    //现在要过滤出消耗品的道具
                    vector<pair<ItemID, int>> consumableItems;
                    for (const auto& item : ownedItems)
                    {
                        auto it = allItems.find(item.first);
                        if (it != allItems.end())
                        {
                            const Item* itemPtr = it->second.get();
                            if (itemPtr && itemPtr->getType() == ItemType::CONSUMABLE)
                            {
                                consumableItems.push_back(item);
                            }
                        }
                    }


                    //计算道具索引
                    int startIndex = currentItemPage * itemsPerPage;
                    int itemIndex = startIndex + i;
                    if (itemIndex < int(consumableItems.size()))
                    {
                        ItemID itemId = consumableItems[itemIndex].first;
						battleSystem->executePlayerAction(9, int(itemId));//这里得强制转int，因为executePlayerAction的参数是int类型的，而itemId是一个枚举类型，底层也是一个整数，所以可以直接强制转换成int类型
                        updateItemBtns();
                        isClickOnCooldown = true;
                        clickCooldownClock.restart();
                        return -1;  //使用道具
                    }
                }
                isClickOnCooldown = true;
                clickCooldownClock.restart();
                return -1;
            }
        }

        //检测上一页按钮
        if (prevPageBtn.enabled && prevPageBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            currentItemPage--;
            updateItemBtns();
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
        //检测下一页按钮
        if (nextPageBtn.enabled && nextPageBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            currentItemPage++;
            updateItemBtns();
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
        break;
    }
	case CurrentTab::CODEMON: //精灵栏目是检测精灵按钮和分页按钮，其实这里在上面已经实现过一次了
    {
        //检测精灵按钮
        for (int i = 0; i < codemonBtns.size(); i++) 
        {
            if (codemonBtns[i].enabled && codemonBtns[i].bg.getGlobalBounds().contains(mousePos)) 
            {
                if (battleSystem && battleSystem->getPlayer()) 
                {
                    int startIndex = currentCodemonPage * codemonsPerPage;
                    int codemonIndex = startIndex + i;
                    //获取精灵
                    Codemon* codemon = battleSystem->getPlayerCodemon(codemonIndex);
                    if (codemon && codemon->getHP() > 0) 
                    {
                        //切换精灵
                        battleSystem->executePlayerAction(10, codemonIndex);
                        //更新信息
                        string texturePath = "Resources/" + codemon->getName() + ".png";
                        setPlayerSprite(texturePath);
                        updatePlayer(codemon);
                        if (battleSystem->getActiveEnemy()) {
                            updateEnemy(battleSystem->getActiveEnemy());
                        }
                        updateCodemonBtns();
                        isClickOnCooldown = true;
                        clickCooldownClock.restart();
                        return -1;
                    }
                }
                isClickOnCooldown = true;
                clickCooldownClock.restart();
                return -1;
            }
        }
        // 检测上一页按钮
        if (prevCodemonPageBtn.enabled && prevCodemonPageBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            currentCodemonPage--;
            updateCodemonBtns();
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
        //检测下一页按钮
        if (nextCodemonPageBtn.enabled && nextCodemonPageBtn.bg.getGlobalBounds().contains(mousePos)) 
        {
            currentCodemonPage++;
            updateCodemonBtns();
            isClickOnCooldown = true;
            clickCooldownClock.restart();
            return -1;
        }
        break;
    }
    //撤退栏目
    case CurrentTab::RETREAT: 
    {
		battleSystem->executePlayerAction(8, 0);
        isClickOnCooldown = true;
        clickCooldownClock.restart();
        break;
    }
    default: break;
    }

    return -1;
}

//这个是更新玩家精灵信息的函数，c是一个指向玩家当前精灵的指针，大部分在init其实就设置好了
void battleUI::updatePlayer(const Codemon* c) 
{
    setTypeAdvantage();
    wstring wideName = toWideString(c->getName());
    playerNameText.setString(wideName);

    string texturePath = "Resources/" + c->getName() + ".png";
    setPlayerSprite(texturePath);
    if (loadTextureFromUtf8(playerFieldTex, texturePath))
    {
        playerFieldSprite.setTexture(playerFieldTex);
        //保持比例缩放
        float scaleX = 300.f / playerFieldTex.getSize().x;
        float scaleY = 300.f / playerFieldTex.getSize().y;
        playerFieldSprite.setScale(scaleX, scaleY);
    }
    FloatRect namespriteBounds = playerSprite.getGlobalBounds();
    FloatRect nametextBounds = playerNameText.getLocalBounds();
    playerNameText.setOrigin(nametextBounds.left + nametextBounds.width / 2, nametextBounds.top + nametextBounds.height / 2);
    playerNameText.setPosition(namespriteBounds.left + namespriteBounds.width / 2, namespriteBounds.top - 15);

    playerLevelText.setString(L"LV : " + to_wstring(c->getLevel()));
    FloatRect levelspriteBounds = playerSprite.getGlobalBounds();
    FloatRect leveltextBounds = playerLevelText.getLocalBounds();
    playerLevelText.setOrigin(leveltextBounds.left + leveltextBounds.width / 2, leveltextBounds.top + leveltextBounds.height / 2);
    playerLevelText.setPosition(levelspriteBounds.left + levelspriteBounds.width / 2,levelspriteBounds.top + levelspriteBounds.height + 15  );

    //血条比例
    float hpRatio = (c->getMaxHP() > 0) ? (float)c->getHP() / c->getMaxHP() : 0;
    playerHPBar.setSize(Vector2f(250.0f * hpRatio, 18.0f));
    wstring hpStr = L"HP:" + to_wstring(c->getHP()) + L"/" +to_wstring(c->getMaxHP());
    playerHPText.setString(hpStr);
    FloatRect hpBarBounds = playerHPBarBg.getGlobalBounds();
    FloatRect hpTextBounds = playerHPText.getLocalBounds();
    playerHPText.setOrigin(hpTextBounds.left + hpTextBounds.width / 2, hpTextBounds.top + hpTextBounds.height / 2);
    playerHPText.setPosition(hpBarBounds.left + hpBarBounds.width / 2, hpBarBounds.top + hpBarBounds.height + 15  );

    int energy = c->getEnergy();
    energyBtn.text.setString(L"聚能\n" +to_wstring(energy) + L"/10");
    countdownText.setFillColor(Color::White);

    wstring statusStr = L"";
    if (c->hasStatus(StatusType::Poison))
    {
        statusStr += L"中毒";
    }
    if (c->hasStatus(StatusType::Burn)) 
    {
        if (!statusStr.empty())
        {
            statusStr += L"、";
        }
        statusStr += L"灼烧";
    }
    if (c->hasStatus(StatusType::Freeze))
    {
        if (!statusStr.empty())
        {
            statusStr += L"、";
        }
        statusStr += L"冻伤";
    }
    playerStatusText.setString(statusStr);
    FloatRect levelTextBounds = playerLevelText.getGlobalBounds();
    playerStatusText.setPosition(levelTextBounds.left + levelTextBounds.width / 2, levelTextBounds.top + levelTextBounds.height + 5 );
    FloatRect statusTextBounds = playerStatusText.getLocalBounds();playerStatusText.setOrigin(statusTextBounds.left + statusTextBounds.width / 2,statusTextBounds.top + statusTextBounds.height / 2 );
}

//这个是更新敌人精灵信息的函数，c是一个指向敌人当前精灵的指针，大致的设置直接跟updatePlayer一样的，主要是位置不太一样，还有克制关系的显示
void battleUI::updateEnemy(const Codemon* c) 
{
    setTypeAdvantage();
    wstring wideName = toWideString(c->getName());
    if (battleSystem)
    {
        int enemyTeamSize = battleSystem->getEnemyTeamSize();
        if (enemyTeamSize > 1)
        {
            int activeIndex = battleSystem->getActiveEnemyIndex();
            if (activeIndex >= 0)
            {
                wideName += L" (" + to_wstring(enemyTeamSize - activeIndex) + L"/" + to_wstring(enemyTeamSize) + L")";
            }
        }
    }
    enemyNameText.setString(wideName);
    string texturePath = "Resources/" + c->getName() + ".png";
    setEnemySprite(texturePath);
    if (loadTextureFromUtf8(enemyFieldTex, texturePath))
    {
        enemyFieldSprite.setTexture(enemyFieldTex);
        //保持比例缩放
        float scaleX = 300.f / enemyFieldTex.getSize().x;
        float scaleY = 300.f / enemyFieldTex.getSize().y;
        enemyFieldSprite.setScale(scaleX, scaleY);
    }
    enemyLevelText.setString(L"LV : " + to_wstring(c->getLevel()));
    FloatRect spriteBounds = enemySprite.getGlobalBounds();
    FloatRect textBounds = enemyLevelText.getLocalBounds();
    enemyLevelText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    enemyLevelText.setPosition(spriteBounds.left + spriteBounds.width / 2,spriteBounds.top + spriteBounds.height + 15 );

    //血条比例
    float hpRatio = (c->getMaxHP() > 0) ? (float)c->getHP() / c->getMaxHP() : 0;
    enemyHPBar.setSize(Vector2f(250.0f * hpRatio, 18.0f));
    wstring hpStr = L"HP:" + to_wstring(c->getHP()) + L"/" + to_wstring(c->getMaxHP());
    enemyHPText.setString(hpStr);
    FloatRect hpBarBounds = enemyHPBarBg.getGlobalBounds();
    FloatRect hpTextBounds = enemyHPText.getLocalBounds();
    enemyHPText.setOrigin(hpTextBounds.left + hpTextBounds.width / 2, hpTextBounds.top + hpTextBounds.height / 2);
    enemyHPText.setPosition(hpBarBounds.left + hpBarBounds.width / 2,hpBarBounds.top + hpBarBounds.height + 15 );

    wstring statusStr = L"";
    if (c->hasStatus(StatusType::Poison)) 
    {
        statusStr += L"中毒";
    }
    if (c->hasStatus(StatusType::Burn)) 
    {
        if (!statusStr.empty())
        {
            statusStr += L"、";
        }
        statusStr += L"灼烧";
    }
    if (c->hasStatus(StatusType::Freeze)) 
    {
        if (!statusStr.empty())
        {
            statusStr += L"、";
        }
        statusStr += L"冻伤";
    }
    enemyStatusText.setString(statusStr);
    FloatRect levelTextBounds = enemyLevelText.getGlobalBounds();
    enemyStatusText.setPosition(levelTextBounds.left + levelTextBounds.width / 2,levelTextBounds.top + levelTextBounds.height + 5);
    FloatRect statusTextBounds = enemyStatusText.getLocalBounds();
    enemyStatusText.setOrigin(statusTextBounds.left + statusTextBounds.width / 2,statusTextBounds.top + statusTextBounds.height / 2 );
}

//更新技能按钮的显示
void battleUI::updateSkillBtns(const vector<const Move*>& skills) 
{
    for (int i = 0; i < 4 && i < (int)skills.size(); i++) 
    {
        if (skills[i])
        {
            wstring wideMoveName = toWideString(skills[i]->name);
            skillBtns[i].name.setString(wideMoveName);
            //耗能和威力
            int effectiveCost = 0;
            if (battleSystem && battleSystem->getActiveCodemon()) 
            {
                effectiveCost = battleSystem->getActiveCodemon()->getEffectiveCost(*skills[i]);
            }
            skillBtns[i].pp.setString(L"耗能：" + to_wstring(effectiveCost) + L"\n威力：" + to_wstring(skills[i]->power));
            skillBtns[i].enabled = true;
        }
    }
}

//添加日志的函数
void battleUI::addLog(const string& msg) 
{
    Text logText;
    logText.setFont(font);
    logText.setCharacterSize(20);
    logText.setFillColor(Color::White);
    wstring wideMsg = toWideString(msg);
    logText.setString(wideMsg);
    float yOffset = logWindowTexts.size() * 25.0f;
    logText.setPosition(672.0f, 260.0f + yOffset);
    logWindowTexts.push_back(logText);
    if (logWindowTexts.size() > 5)
    {
        logWindowTexts.erase(logWindowTexts.begin());
    }
}

//设置克制关系的函数
void battleUI::setTypeAdvantage() 
{
	int type = 0; //0表示无克制关系，1表示克制，2表示被克制;
    if (battleSystem && battleSystem->getActiveEnemy())
    {
        Codemon* enemy = battleSystem->getActiveEnemy();
        Codemon* codemon = battleSystem->getActiveCodemon();
        float multiplier = battleSystem->calculateTypeMultiplier(codemon->getType1(), { enemy->getType1(), enemy->getType2() });
        if (multiplier > 1.0f)
        {
			type = 1; //克制
        }
        else if (multiplier < 1.0f)
        {
			type = 2; //被克制
        }
        else
        {
			type = 0; //无克制关系
        }
    }
    switch (type)
    {
        case 0:
        {
            advantageSprite.setTexture(advantageNoneTex);
            break;
        }
        case 1:
        {
            advantageSprite.setTexture(advantageRightTex);
            break;
        }
        case 2:
        {
            advantageSprite.setTexture(advantageLeftTex);
            break;
        }
    }
}

//这个函数会在BattleSystem初始化UI的时候调用，把当前UI实例传递给BattleSystem
void battleUI::setBattleSystem(BattleSystem* bs) 
{
    battleSystem = bs;
    if (bs) 
    {
        bs->setUI(this);//将当前UI实例设置给BattleSystem
        updateItemBtns();//更新道具按钮显示
        updateCodemonBtns(); //更新精灵按钮
        if (bs->getActiveCodemon()) 
        {
            updatePlayer(bs->getActiveCodemon());
        }
        if (bs->getActiveEnemy())
        {
            updateEnemy(bs->getActiveEnemy());
        }
    }
}

//更新道具按钮，在click那其实有一些已经重复处理过的
void battleUI::updateItemBtns() 
{
    Player* player = battleSystem->getPlayer();
    Bag& bag = player->bag;
    const auto& allItems = ItemDB::getAllItems();
	//收集玩家拥有的道具，这里先遍历所有道具，然后通过背包查询玩家拥有的数量，如果数量大于0就加入到ownedItems列表中，这样就能得到玩家拥有的所有道具和对应的数量了
    vector<pair<ItemID, int>> ownedItems;
    for (const auto& pair : allItems) 
    {
        ItemID id = pair.first;
        int count = bag.getItemCount(id);
        if (count > 0) 
        {
            ownedItems.push_back({ id, count });
        }
    }
    //现在要过滤出消耗品的道具
	vector<pair<ItemID, int>> consumableItems;
    for(const auto& item : ownedItems) 
    {
        auto it = allItems.find(item.first);
        if (it != allItems.end()) 
        {
            const Item* itemPtr = it->second.get();
            if (itemPtr && itemPtr->getType() == ItemType::CONSUMABLE) 
            {
                consumableItems.push_back(item);
            }
        }
	}
    //计算总页数
    int totalPages = (consumableItems.size() + itemsPerPage - 1) / itemsPerPage;
    if (totalPages == 0)
    {
        totalPages = 1;
    }
    //确保当前页有效
    if (currentItemPage >= totalPages) 
    {
        currentItemPage = totalPages - 1;
    }
    if (currentItemPage < 0) 
    {
        currentItemPage = 0;
    }
    //更新分页按钮
    prevPageBtn.enabled = (currentItemPage > 0);
    nextPageBtn.enabled = (currentItemPage < totalPages - 1);

    //计算当前页的起始索引
    int startIndex = currentItemPage * itemsPerPage;

    // 更新道具按钮
    for (int i = 0; i < itemsPerPage; i++) 
    {
        int itemIndex = startIndex + i;
        if (itemIndex < int(consumableItems.size()))
        {
            //有道具
            ItemID itemId = consumableItems[itemIndex].first;
            int count = consumableItems[itemIndex].second;
            auto it = allItems.find(itemId);
            if (it != allItems.end()) 
            {
                const Item* item = it->second.get();
                //设置道具信息
                if (item) 
                {
                    wstring buttonText = toWideString(item->getName()) + L"\n数量：" + to_wstring(count)+L"\n"+toWideString(item->getDescription());
                    itemBtns[i].text.setString(buttonText);
                    itemBtns[i].enabled = true;
                    itemBtns[i].bg.setFillColor(Color(150, 100, 50));
                }
            }
        }
        else 
        {
            // 没有道具，显示空按钮
            itemBtns[i].text.setString(L"空");
            itemBtns[i].enabled = false;
            itemBtns[i].bg.setFillColor(Color(80, 80, 80));  // 灰色
        }

        // 文本居中
        FloatRect textRect = itemBtns[i].text.getLocalBounds();
        itemBtns[i].text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        itemBtns[i].text.setPosition(itemBtns[i].bg.getPosition().x + itemBtns[i].bg.getSize().x / 2.0f, itemBtns[i].bg.getPosition().y + itemBtns[i].bg.getSize().y / 2.0f );
    }
}

//更新道具按钮
void battleUI::updateCodemonBtns() 
{
    int teamSize = battleSystem->getPlayerTeamSize();
    //计算总页数
    int totalPages = (teamSize + codemonsPerPage - 1) / codemonsPerPage;
    if (totalPages == 0)
    {
        totalPages = 1;
    }
    if (currentCodemonPage >= totalPages)
    {
        currentCodemonPage = totalPages - 1;
    }
    if (currentCodemonPage < 0)
    {
        currentCodemonPage = 0;
    }
    prevCodemonPageBtn.enabled = (currentCodemonPage > 0);
    nextCodemonPageBtn.enabled = (currentCodemonPage < totalPages - 1);
    int startIndex = currentCodemonPage * codemonsPerPage;
    for (int i = 0; i < codemonsPerPage; i++)
    {
        int idx = startIndex + i;
        if (idx < teamSize)
        {
            Codemon* codemon = battleSystem->getPlayerCodemon(idx);
            if (codemon)
            {
                wstring name = toWideString(codemon->getName());
                wstring typeStr = L"属性：";
                if (codemon->getType1() != Element::None)
                {
                    typeStr += getElementName(codemon->getType1());
                }
                if (codemon->getType2() != Element::None)
                {
                    typeStr += L"/" + getElementName(codemon->getType2());
                }
                wstring advantageStr = L"关系：";
                if (battleSystem && battleSystem->getActiveEnemy()) 
                {
                    Codemon* enemy = battleSystem->getActiveEnemy();
                    float multiplier = battleSystem->calculateTypeMultiplier(codemon->getType1(), { enemy->getType1(), enemy->getType2() });
                    if (multiplier > 1.0f)
                    {
                        advantageStr += L"克制";
                    }
                    else if (multiplier < 1.0f)
                    {
                        advantageStr += L"被克制";
                    }
                    else
                    {
                        advantageStr += L"普通";
                    }
                }
                wstring buttonText = name + L"\n" + typeStr + L"\n" + advantageStr;
                codemonBtns[i].text.setString(buttonText);
                codemonBtns[i].enabled = true;
                if (codemon->getHP() <= 0)
                {
                    codemonBtns[i].bg.setFillColor(Color(100, 100, 100));//死了的灰
                }
                else if (codemon == battleSystem->getActiveCodemon())
                {
                    codemonBtns[i].bg.setFillColor(Color(100, 200, 100));
                }
                else
                {
                    codemonBtns[i].bg.setFillColor(Color(100, 150, 100));
                }
            }
        }
        else
        {
            codemonBtns[i].text.setString(L"空\n属性：无\n关系：无");
            codemonBtns[i].enabled = true;
            codemonBtns[i].bg.setFillColor(Color(80,80,80));
        }
        FloatRect textRect = codemonBtns[i].text.getLocalBounds();
        codemonBtns[i].text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        codemonBtns[i].text.setPosition(codemonBtns[i].bg.getPosition().x + codemonBtns[i].bg.getSize().x / 2.0f, codemonBtns[i].bg.getPosition().y + codemonBtns[i].bg.getSize().y / 2.0f);
    }
}

//这个函数是根据元素类型返回对应的中文名称的
wstring battleUI::getElementName(Element element)
{
    switch (element) 
    {
        case Element::Normal:
        {
            return L"普通";
        }
        case Element::Fire:
        {
            return L"火";
        }
        case Element::Water:
        {
            return L"水";
        }
        case Element::Grass:
        {
            return L"草";
        }
        case Element::Electric:
        {
            return L"电";
        }
        case Element::Ice:
        {
            return L"冰";
        }
        case Element::Fighting:
        {
            return L"格斗";
        }
        case Element::Poison:
        {
            return L"毒";
        }
        case Element::Ground:
        {
            return L"地面";
        }
        case Element::Wind:
        {
            return L"风";
        }
        case Element::Flying:
        {
            return L"飞行";
        }
        default://这个不可能实现只是我不想让下面输出有警告而已要不然看着难受
        {
            return L"无";
		}
    }
}

//这个函数是设置玩家精灵图片的
void battleUI::setPlayerSprite(const string& texturePath) 
{
    if (loadTextureFromUtf8(playerTex, texturePath)) 
    {
        playerSprite.setTexture(playerTex);
        float scaleX = 128.f / playerTex.getSize().x;
        float scaleY = 128.f / playerTex.getSize().y;
        playerSprite.setScale(scaleX, scaleY);
    }
    else 
    {
        cout << "玩家精灵图片加载失败：" << texturePath << endl;
    }
}

//设置敌人精灵图片
void battleUI::setEnemySprite(const string & texturePath)
{
    if (loadTextureFromUtf8(enemyTex, texturePath)) 
    {
        enemySprite.setTexture(enemyTex);
        float scaleX = 128.f / enemyTex.getSize().x;
        float scaleY = 128.f / enemyTex.getSize().y;
        enemySprite.setScale(scaleX, scaleY);
    }
    else 
    {
        cout << "警告：敌人头像加载失败：" << texturePath << endl;
    }
}

