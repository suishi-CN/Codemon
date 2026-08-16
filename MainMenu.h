#pragma once

//道具类型枚举
enum class ItemType
{
	CONSUMABLE, //消耗品
	EQUIPMENT,   //装备
	KEY
};

//装备位置枚举
enum class EquipSlot
{
	WEAPON,    //武器
	ARMOR,	   //防具
	NECKLACE,  //项链
	RING,	   //戒指
	NONE
};

//定义道具ID枚举
enum class ItemID : int
{
	None = 0,
	//消耗类
	//治疗
	BasicHealthPotion = 11001,    //初级治疗药水 100 恢复10%血量
	NormalHealthPotion = 11002,   //中级治疗药水 200 恢复20%血量
	AdvancedHealthPotion = 11003, //高级治疗药水 300 恢复30%血量

	//加攻击
	BasicAttackPotion = 12001,    //初级物攻药水 100 增加20点物理攻击
	NormalAttackPotion = 12002,   //中级物攻药水 200 增加40点物理攻击
	AdvancedAttackPotion = 12003, //高级物攻药水 300 增加60点物理攻击

	//加魔法攻击
	BasicMagicAttackPotion = 13001,    //初级魔攻药水 100 增加20点魔法攻击
	NormalMagicAttackPotion = 13002,   //中级魔攻药水 200 增加40点魔法攻击
	AdvancedMagicAttackPotion = 13003, //高级魔攻药水 300 增加60点魔法攻击

	//加物理防御
	BasicDefencePotion = 14001,    //初级物防药水 100 增加20点物理防御
	NormalDefencePotion = 14002,   //中级物防药水 200 增加40点物理防御
	AdvancedDefencePotion = 14003, //高级物防药水 300 增加60点物理防御

	//加魔法防御
	BasicMagicDefencePotion = 15001,    //初级魔防药水 100 增加20点魔法防御
	NormalMagicDefencePotion = 15002,   //中级魔防药水 200 增加40点魔法防御
	AdvancedMagicDefencePotion = 15003, //高级魔防药水 300 增加60点魔法防御

	//加能量
	EnergyPotion = 16001, //能量药水 200 恢复全部能量

	//清除负面效果
	PurificationPotion = 17001, //净化药水 250 清除所有负面效果

	//复合效果
	BloodragePotion = 18001, //血怒药水 450 增加120点物理攻击，增加120点魔法攻击，减少30%生命值
	StonehidePotion = 18002, //石肤药水 450 增加80点物理防御，增加80点魔法防御
	SacredElixir = 18003,    //神圣药水 450 清除所有负面效果，增加10点能量，恢复25%生命值
	ThiefPotion = 18004,     //盗贼药水 450 增加100点物理攻击，增加20点速度
	ArcanePotion = 18005,    //奥术药水 450 增加100点魔法攻击，恢复满所有能量
	GiantPotion = 18006,     //巨人药水 450 增加80点物理攻击，增加80点物理防御
	VoidPotion = 18007,      //虚空药水 450 增加100点魔法攻击，增加20点速度

	//装备
	//武器
	CopperSword = 21001,     //铜剑 200 增加20点物理攻击
	IronSword = 21002,       //铁剑 400 增加40点物理攻击
	SteelSword = 21003,      //钢剑 600 增加60点物理攻击
	DiamondSword = 21004,    //钻石剑 1000 增加80点物理攻击
	DragonBoneSword = 21005, //龙骨剑 1500 增加100点物理攻击
	CopperWand = 21006,      //铜法杖 200 增加20点魔法攻击
	IronWand = 21007,        //铁法杖 400 增加40点魔法攻击
	CrystalWand = 21008,     //水晶法杖 600 增加60点魔法攻击
	DiamondWand = 21009,     //钻石法杖 1000 增加80点魔法攻击
	DragonBoneWand = 21010,  //龙骨法杖 1500 增加100点魔法攻击

	//防具
	LeatherArmour = 22001,     //皮质护甲 100 增加10点物理防御，增加10点魔法防御
	CopperArmour = 22002,      //铜护甲 200 增加20点物理防御，增加20点魔法防御
	IronArmour = 22003,        //铁护甲 400 增加40点物理防御，增加40点魔法防御
	SteelArmour = 22004,       //钢护甲 600 增加60点物理防御，增加40点魔法防御
	CrystalArmour = 22005,     //水晶护甲 600 增加40点物理防御，增加60点魔法防御
	DiamondArmour = 22006,     //钻石护甲 1000 增加80点物理防御，增加80点魔法防御
	DragonScaleArmour = 22007, //龙鳞护甲 1500 增加100点物理防御，增加100点魔法防御，增加10点速度

	//项链
	IronNecklace = 23001,       //铁项链 200 增加10点物理攻击，增加10点物理防御
	GoldenNecklace = 23002,     //金项链 200 增加10点魔法攻击，增加10点魔法防御
	RubyNecklace = 23003,       //红宝石项链 500 增加20点物理攻击，增加20点物理防御
	SapphireNecklace = 23004,   //蓝宝石项链 500 增加20点魔法攻击，增加20点魔法防御
	EmeraldNecklace = 23005,    //绿宝石项链 500 增加20点物理防御，增加20点魔法防御
	DiamondNecklace = 23006,    //钻石项链 800 增加20点物攻，增加30点物理防御，增加20点魔法攻击，增加30点魔法防御
	DragonBoneNecklace = 23007, //龙骨项链 1200 增加30点物攻，增加40点物理防御，增加30点魔法攻击，增加40点魔法防御

	//戒指
	IronRing = 24001,         //铁戒指 200 增加10点物理攻击，增加10点物理防御
	QuartzRing = 24002,       //石英戒指 200 增加10点速度
	GoldenRing = 24003,       //金戒指 200 增加10点魔法攻击，增加10点魔法防御
	RubyRing = 24004,         //红宝石戒指 500 增加20点物理攻击，增加20点物理防御
	SapphireRing = 24005,     //蓝宝石戒指 500 增加20点魔法攻击，增加20点魔法防御
	EmeraldRing = 24006,      //绿宝石戒指 500 增加20点物理防御，增加20点魔法防御
	DiamondRing = 24007,      //钻石戒指 800 增加30点物攻，增加20点物理防御，增加30点魔法攻击，增加20点魔法防御
	DragonBoneRing = 24008,   //龙骨戒指 1200 增加40点物攻，增加30点物理防御，增加40点魔法攻击，增加30点魔法防御

	//钥匙
	KEY01 = 30001,
	KEY02 = 30002,
	KEY11 = 30003,
	KEY12 = 30004,
	KEY21 = 30005,
	KEY22 = 30006,
	KEY31 = 30007,
	KEY32 = 30008,
	KEY41 = 30009,
	KEY42 = 30010,
	KEY51 = 30011,
	KEY52 = 30012
};