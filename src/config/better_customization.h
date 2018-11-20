
#define SERVER_DIFF 2

#if SERVER_DIFF == 1
//版本20150916、复兴前，金币上限改21E、自动拾取上限100条
#undef C_PACKETVER
#define C_PACKETVER 20150916

#undef C_MAX_ZENY
#define C_MAX_ZENY INT_MAX

#undef C_AUTOLOOTITEM_SIZE
#define C_AUTOLOOTITEM_SIZE 100

#elif SERVER_DIFF == 2
//泡沫奶瓶
#undef C_PACKETVER
#define C_PACKETVER 20180620

#elif SERVER_DIFF == 99
//官方测试服
#undef C_MAX_ZENY
#define C_MAX_ZENY INT_MAX

#define C_DEFAULT_WALK_SPEED
#define C_DEFAULT_WALK_SPEED 125     //玩家默认移动速度

#define BONUS_SKILL_AOE                 // 单体技能改为AOE
#define BONUS_MULTIDAMAGERATE           // 多倍伤害
#define SCRIPT_UNITSKILLTOWIDE          // 技能扩散
#define SCRIPT_ROULETTE                 // 转盘规则
#define SCRIPT_MAGICSHIELD              // 魔法护盾
#define JRO_FORMULAS                    // JRO技能公式
#else
#define BEATVER
#endif