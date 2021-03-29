// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef	_BETTERRA_CONFIG_H_
#define	_BETTERRA_CONFIG_H_

//----------------------------------
//自定义宏定义
//----------------------------------
#define C_PACKETVER 20200401
#define C_PRERE 0                      //是否编译为复兴前版本
#define C_VIP_ENABLE 0                 //是否开启VIP系统
#define C_PACKET_OBFUSCATION 0         //是否禁用封包加密

#define C_MAX_ZENY 1000000000          //最大随身携带的金币
#define C_MAX_INVENTORY 100            //最大背包容量
#define C_MIN_STORAGE 300              //玩家仓库容量
#define C_MAX_CART 100                 //玩家手推车容量
#define C_MAX_STORAGE 800              //最大玩家仓库容量上限
#define C_DEFAULT_WALK_SPEED 150       //玩家默认移动速度
#define C_AUTOLOOTITEM_SIZE 10         //自动拾取的物品清单

#define C_MAX_PARTY 12                 //最大队伍成员数量
#define C_MAX_GUILD_STORAGE 600        //最大公会仓库容量
#define C_MIN_GUILD 16                 //最大公会初始成员容量
#define C_GUILD_SKILL_NUMBER_ADD 6     //扩充组合体制每级增加人数

#define C_MAX_PETLOOT_SIZE 30          //最大宠物拾取容量
#define C_MAX_PET_DB 600               //最大宠物数据库的ID容量
#define C_MAX_MOB_DB 3999              //最大魔物数据库的ID容量
#define C_MAX_NPC_CLASS2_END 0         //最大NPC数据库的ID容量

#define C_MAX_MERCENARY_CLASS 61       //最大佣兵的种类
#define C_MAX_FAME_LIST 10             //最大声望列表长度
#define C_MAX_REFINE 0                 //最大精炼值
#define C_MAX_EVENTQUEUE 200           //最大事件队列的容量
#define C_MAX_HATEFFECT 0              //最大特殊头饰效果的ID上限
#define C_MAX_RC2 0                    //最大自定义种族数量
#define C_MAX_SKILLUNITTEMP 20         //最大地面上最大技能存在数量
#define C_MAX_SKILLUNITGROUPTICKSET 25 //最大地面技能计时器数量
#define C_MAX_LEVEL 200                //最大等级
#define C_MAX_DAMAGELOG 30             //魔物伤害记录数量
#define C_MIN_CHARS 0                  //初始创建角色数量
#define C_MAX_CHAT_USERS 20            //最大聊天室人数
#define C_MAX_SEARCH 5                 //最大物品掉落排序数量

#define C_SHOW_SERVER_STATS 0

#define C_SOCKET_ENCRYPT 0
#define C_SOCKET_HOOKDLL 0
#define C_BETTERRA_BETA 0
#define C_NULLPO_CHECK 0
#define C_MAX_MULTI_IP 10
//----------------------------------
//收费开关
//----------------------------------
#define C_MAX_ASPDTYPE 1

//----------------------------------
//自定义功能集
//----------------------------------

#define BRA_STEP_PATH
#define BRA_TRIBE_TEAM

#define BRA_MSG_TRANSLATE

//----------------------------------
//事件列表
//----------------------------------
#define EVENT_HOMLVUP
#define EVENT_HOMCALL
#define EVENT_HOMREST
#define EVENT_HOMDEAD
#define EVENT_HOMALIVE
#define EVENT_IDENTIFYFILTER
#define EVENT_IDENTIFY
#define EVENT_BFRELOADSCRIPT
#define EVENT_AFRELOADSCRIPT
#define EVENT_KILLMVP
#define EVENT_EQUIPFILTER
#define EVENT_EQUIPEVENT
#define EVENT_UNEQUIPFILTER
#define EVENT_UNEQUIPEVENT
#define EVENT_BUFFSTART
#define EVENT_BUFFEND
#define EVENT_PARTYCREATEFILTER
#define EVENT_PARTYJOINFILTER
#define EVENT_PARTYLEAVEFILTER
#define EVENT_GUILDCREATEFILTER
#define EVENT_GUILDJOINFILTER
#define EVENT_GUILDLEAVEFILTER
#define EVENT_PETLOOTEVENT
#define EVENT_PETFEEDEVENT
#define EVENT_PETSKILLFILTER
#define EVENT_VIEWEQUIPFILTER
#define EVENT_USESKILLFILTER
#define EVENT_USESKILLEVENT
#define EVENT_USEITEMFILTER
#define EVENT_USEITEMEVENT
#define EVENT_WILLLOADMAPEVENT
#define EVENT_INCHATROOMFILTER
#define EVENT_TITLECHANGEFILTER
#define EVENT_ROULETTEOPENFILTER
#define EVENT_ROULETTESTARTFILTER
#define EVENT_ROULETTEGETFILTER
#define EVENT_ROULETTECLOSEEVENT

//----------------------------------
//脚本与指令开关
//----------------------------------
#define SCRIPT_SENDMAIL

//----------------------------------
//默认开关
//----------------------------------
#define C_CUSTOMIZEDDATE "null"
#define C_USEREMAIL "admin@mail.betterra.cn"
#define C_VERIFYURL "http://vef.betterra.cn/BetterRA_Verification"
#define C_VERIFYURLMD5 "24610aeb5d33982bf2c3dc42585083e3"
#define C_READ_KEY_IN_CONFIG
#define C_MAX_GUILD C_MIN_GUILD + 10 * C_GUILD_SKILL_NUMBER_ADD
#define MAX_POLICIES 20
#include "better_customization.hpp"
#ifdef BEATVER
	#ifdef _DEBUG
		#define PLAYER_LIMIT 20
		#undef C_SOCKET_ENCRYPT
		#define C_SOCKET_ENCRYPT 1
		#undef C_SOCKET_HOOKDLL
		#define C_SOCKET_HOOKDLL 1
		#undef C_SOCKET_EVENT
		#define C_SOCKET_EVENT 1
		#undef C_SOCKET_RH
		#define C_SOCKET_CHECKRH 1
		#undef C_MAX_ASPDTYPE
		#define C_MAX_ASPDTYPE 3
	#else
		#define PLAYER_LIMIT 5
	#endif
	#define BRA_SKILL_FORMULA               // 自定义技能公式
	#define BONUS_SKILL_AOE                 // 单体技能改为AOE
	#define BONUS_MULTIDAMAGERATE           // 多倍伤害
	#define BONUS_ADD_SKILL_AREA            // 增加技能范围
	#define BONUS_ADD_SKILL_RANGE           // 增加技能释放距离
	#define BONUS_SKILL_TO_AROUND           // 按住自动对附近目标单体攻击
	#define BONUS_SKILL_TO_GROUND           // 单体技能变地面范围攻击
	#define BONUS_SKILL_NO_CAST             // 忽略技能消耗
	#define BONUS_STATUS_ADD_BONUS          // 当获得状态时增加新的bonus
	#define BONUS_ADD_STATUS_VAL            // 赋予状态时状态参数累加
	#define SCRIPT_ROULETTE                 // 转盘规则
	#define SCRIPT_MAGICSHIELD              // 魔法护盾
	#define C_UTF8_CODE                     // 支持UTF8编码文件
#endif
#endif //_BETTERRA_CONFIG_H_
