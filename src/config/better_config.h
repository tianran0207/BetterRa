// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder


//#define PRERE
#define PACKETVER 20170614

#define BETTER_PACKET_OBFUSCATION

#define MAX_CHARS 9 
#define MAX_ZENY 1000000000        //随身携带的金币上限

//#define VIP_ENABLE               //是否开启VIP系统
#define MAX_STORAGE 600            //玩家仓库容量上限
#define MAX_GUILD_STORAGE 600      //公会仓库容量上限
#define MIN_GUILD 16               //公会初始成员容量上限
#define GUILD_SKILL_NUMBER_ADD 6   //扩充组合体制每级增加人数
#define MAX_INVENTORY 100          //背包中的物品种类上限
#define MAX_PET_DB 600             //宠物数据库的ID容量上限
#define MAX_PETLOOT_SIZE 30
#define MAX_PARTY 12               //队伍的最大成员数量上限
#define DEFAULT_WALK_SPEED 150     //玩家默认移动速度
#define AUTOLOOTITEM_SIZE 10       //自动拾取的物品清单上限
#define MAX_MOB_DB 3999            //魔物数据库的ID容量上限
#define MAX_MERCENARY_CLASS 61     //佣兵的种类上限
#define MAX_FAME_LIST 10           //声望列表长度上限
//#define MAX_REFINE 10            //最大精炼值
#define MAX_EVENTQUEUE 200         //事件队列的容量上限
//#define MAX_NPC_CLASS 0          //NPC种类上限
//#define PACKET_OBFUSCATION_KEY1 0x3839c33f        //key1
//#define PACKET_OBFUSCATION_KEY2 0x2a9de0a7        //key2
//#define PACKET_OBFUSCATION_KEY3 0x1849a1bb        //key3


#define CONF_PATH "conf"

#define MSG_TRANSLATE
#define FOLLOWPACKET

#define MAX_POLICIES 20

//evet
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

#define EVENT_ROULETTESTARTFILTER
#define EVENT_ROULETTEGETFILTER




#define SCRIPT_SENDMAIL






#define MAX_GUILD MIN_GUILD+10*GUILD_SKILL_NUMBER_ADD