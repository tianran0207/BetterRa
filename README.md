# 简介
创建 BetterRa创建于2017年11月28日..他是在Rathena的基础上经过二次修改的一款模拟器, 加入了大量的功能..

# 更新日志
* 20190122  [修复] 使用商店拓展后，部分露天商店离线挂店后重启服务端后不能重读的问题
* 20190119  [添加] 服务端网页定制 添加开启封包加密算法选项
* 20190119  [支持] 封包加密算法可防外挂，需要客户端额外加载 BetterRA.dll，加载方法请加Q群419290524
* 20190119  [支持] 开启封包加密算法时，可获取MAC地址，MAC存入 login表中的last_ip 字段
* 20190119  [添加] 脚本指令 getlastip 获取login表中的 last_ip 字段，防止重复执行mysql指令提高效率
* 20190113  [修复] 重载物品信息后玩家不能正常加载装备套装属性的问题
* 20190113  [修复] 物品拓展 skillfixcastrate 无效的问题
* 20190113  [添加] 战斗配置 backstap_overlap 在与目标重叠状态下是否可以使用流氓的背刺技能
* 20190113  [添加] 脚本事件 unithide 临时隐藏某个GID, 隐藏只针对指定的角色，当玩家离开再回来时还可以看到这个目标
* 20190113  [添加] NPC事件 OnDisplay, 当NPC进入玩家视野范围内触发, 打断后此玩家看不到该NPC
* 20190113  [添加] 物品拓展 bIgnoreWhiteImprison 无视白色监狱
* 20190113  [添加] OnPCKillMvpEvent 返回变量 @mob_dead_m 魔物死亡时所在的地图id
* 20190111  [修复] 蓄势待发对反射伤害无效的问题
* 20190110  [添加] 战斗配置 vending_cash_tax 当露天商店使用cash作为货币时的交易税率
* 20190110  [添加] 战斗配置 vending_cash_tax_min 当露天商店使用cash作为货币时，交易开始征税最小额度
* 20190110  [添加] 配置文件 db\item_vending.txt 支持对每物品设定税率和交易开始征税最小额度
* 20190110  [添加] 战斗配置 same_mapwarp_castcancel 同图传送是否会中断施法
* 20190110  [重写] 由于之前在RA论坛上的的露天商店扩展存在大量BUG, 所以重写露天商店拓展的全部逻辑
* 20190106  [添加] 服务端网页定制 添加跟踪技能单位技能ID的临时存储空间
* 20190106  [修复] 修复RA官方BUG 研究所5层怪物使用技能的宕机问题
* 20190104  [修复] 关闭RA默认的宕机处理程序, 将宕机处理移入生成dmp文件程序, 防止宕机信号被截断
* 20190103  [修复] 在开启露天商店货币系统的情况下 重读离线露天商店也不会造成货币不管用的情况
* 20190103  [修复] 魔物对玩家使用白色监狱失败的情况下不再报错
* 20190101  [修复] 战斗配置 walk_close_to_npc 在范围触发脚本中的报错问题
* 20181231  [优化] 脚本指令 sortarray 增加参考数组和排序数组, 实现多种排序方式
* 20181231  [添加] 脚本指令 charid2rid 通过角色ID(charid)对应的账号ID(rid, gid, aid)
* 20181231  [添加] 脚本指令 getmobdmglog 用来获取对魔物造成伤害的排名数据
* 20181231  [添加] 战斗配置 walk_close_to_npc 当玩家点击NPC时, 是否自动行走靠近NPC再说话
* 20181231  [添加] 战斗配置 pvp_short_attack_damage_rate 在PVP地图中的近距离攻击伤害倍率
* 20181231  [添加] 战斗配置 pvp_long_attack_damage_rate 在PVP地图中的远距离攻击伤害倍率
* 20181231  [添加] 战斗配置 pvp_weapon_attack_damage_rate 在PVP地图中的物理技能伤害倍率
* 20181231  [添加] 战斗配置 pvp_magic_attack_damage_rate 在PVP地图中的魔法技能伤害倍率
* 20181231  [添加] 战斗配置 pvp_misc_attack_damage_rate 在PVP地图中的特殊技能伤害倍率
* 20181230  [支持] MySQL内码表自动判定机制, inter_athena.conf 中 codepage可以设置为 auto (感谢 尐駒頭寶寶)
* 20181229  [支持] 简单支持了造型设计窗口
* 20181229  [添加] 造型设计窗口支持脚本, npc\BetterRa\BetterRa_stylist_demo.txt
* 20181229  [添加] 脚本事件 OnPCStylistChangeEvent 换装成功后返回该事件, 用来实现造型设计功能
* 20181229  [添加] 脚本指令 stylistui 打开造型设计窗口
* 20181229  [优化] 获得opt物品相关的脚本可以传入空数组
* 20181229  [修复] 修改窗口标题中显示流量信息默认为关闭 (感谢 尐駒頭寶寶)
* 20181229  [修复] 在计算 bonus 时由于数字过大造成数据溢出问题 (感谢 Edbin)
* 20181229  [修复] 当刺客使用双刀时由于武器攻击过高造成的副手伤害为1的问题 (感谢 Edbin)
* 20181228  [修复] 当 chat_server_interface 为 no 时接收到 chat-server.exe 连接不会显示未知封包, 会提示接口被禁用
* 20181228  [修复] 当 chat-server.exe 连接被拒绝时, 程序不会重复连接, 会自动退出
* 20181228  [添加] 添加 chat-server.exe 支持 websocket测试网页 chat-webclient.html 通过网页和游戏聊天的功能
* 20181226  [支持] 新版客户端中一键切换装备的按钮
* 20181226  [添加] 战斗配置 instance_leave_record 是否在instance_record$中记录玩家离开副本时的地图坐标信息
* 20181226  [添加] 脚本指令 instance_enter_left 返回上次离开副本的位置
* 20181226  [添加] 脚本指令 instance_record 可用来检查 instance_record$ 变量和获取是否合理
* 20181226  [修改] 现在最大仓库容量为 800 请不要超出此值
* 20181226  [移除] 移除战斗配置 instance_lastpoint
* 20181226  [移除] 移除战斗配置 instance_leavepoint
* 20181224  [修复] 使用 reloadmobdb 指令时可以重新计算魔物的素质属性
* 20181224  [修复] 跳高技能不能落点不能是魔物、NPC和玩家
* 20181224  [添加] 支持客户端指令 /viewpointvalue 和 /setcamera
* 20181224  [添加] GM指令 camerainfo 用来更新玩家视角限制
* 20181224  [添加] 脚本指令 camerainfo 用来更新玩家视角限制
* 20181224  [修复] 当选择角色界面中角色数量等于3时会显示所在地图为未知地图的问题
* 20181224  [修复] 当使用技能时 autobonus 被清除的问题
* 20181224  [修复] 物品拓展 autobonus 在特定情况下没有效果的问题
* 20181224  [修复] 物品拓展 bAutoSpellOnSkill 在特定情况下没有效果的问题
* 20181224  [优化] 引入常量 INFINITE_TICK 为 -1 来用作无限计时器的常量
* 20181224  [修复] 由于11月9日脚本更新造成的城堡投资不正常的问题
* 20181224  [添加] 物品扩展 bIgnoreMdefRaceVal 无视 r 种族 n 点魔法防御力 
* 20181224  [添加] 物品扩展 bIgnoreMdefClassVal 无视 c 类别魔物 n 点魔法防御力
* 20181224  [添加] 物品扩展 bIgnoreDefRaceVal 无视 r 种族 n 点魔法防御力
* 20181224  [添加] 物品扩展 bIgnoreDefClassVal 无视 c 类别魔物 n 点魔法防御力
* 20181224  [添加] 脚本指令 is_attach 获取玩家是否跟脚本有关联, 一般用来判断玩家是否正在跟NPC对话
* 20181224  [添加] 脚本指令 unitmasterid 获得GID对应的主人的GID, 支持宠物、魔物、生命体、佣兵、元素精灵、技能
* 20181223  [添加] 战斗配置 always_npc_kill_event 打开此选项在魔物拥有自己的事件标签时也可以触发OnNPCKillEvent
* 20181223  [添加] 战斗配置 always_mvp_kill_event 打开此选项在魔物拥有自己的事件标签时也可以触发OnMVPKillEvent
* 20181223  [添加] 战斗配置 overlay_back_stack 触发多次对话时, 是否直接覆盖上层堆栈（不报错）。
* 20181223  [添加] 脚本事件 OnPCCashShopOpenFilter 打开商城前触发(可被打断). 
* 20181223  [添加] 脚本事件 OnPCCashShopBuyFilter 购买物品前触发(可被打断). 返回购买物品ID、数量、单价
* 20181223  [添加] 脚本事件 OnPCCashShopCloseEvent 关闭商城后触发.
* 20181223  [添加] 脚本指令 getdaytick 返回指定日期的时间戳
* 20181223  [修复] 所有 Filter 事件中的使用 end 指令将不再影响玩家与NPC之间的正常对话
* 20181223  [修复] 使用 unitremove 删除佣兵时不再降低佣兵点数
* 20181223  [修复] 使用 getservantgid 获得佣兵UID时不再返回错误信息
* 20181220  [添加] maps_athena.conf 支持copy头 复制地图获得新地图 例copy: 12345 prontera 复制普隆德拉的地图到名为12345的地图上
* 20181218  [修复] 召唤师舔毛技能在异常状态时的表现形式 现在类似于点血愈的表现形式
* 20181218  [修复] 召唤师技能‘疯兔胡萝卜重击’和‘猫薄荷陨石’技能伤害问题
* 20181218  [修复] 召唤师技能‘美味活虾派对’不能正确地提高SP恢复的问题
* 20181218  [修复] 召唤师技能‘美味活虾派对’不支持‘活虾’的问题
* 20181218  [修复] 召唤师技能‘跳跃’使用完成后的朝向问题
* 20181218  [修复] 战场系统中聊天会出现乱码的问题
* 20181218  [修复] 缺少公会成员的登录日期的问题
* 20181218  [修复] skill_damage_db.txt 类型为 常量 时不能正常读取的问题
* 20181218  [修复] 每10点 STR 不会给额外攻击力的问题
* 20181218  [变更] 移除BetterRa.conf中的homunculus_autofeed_rate选项， 增加 feature.conf 中的 feature.homunculus_autofeed_rate选项
* 20181217  [添加] 战斗配置 bonus_ratefix 对9个bonus百分比最大值进行限制
* 20181217  [添加] 脚本指令 bonus_ratefix 可用在 OnPCStatCalcEvent 事件下面用来修正相应的bonus数值
* 20181217  [添加] 脚本事件 OnPCRefineUIFilter 准备使用精炼UI时触发事件
* 20181217  [添加] 脚本事件 OnPCRefineUIEvent 使用精炼UI精炼完成后触发事件
* 20181217  [修复] 在某次更新中导致阵营系统失效的问题
* 20181209  [添加] 帕拉市场脚本
* 20181209  [添加] 地图标记 partylock2 当组队中的任何玩家在标有该标记的地图上则阻止相应变更
* 20181209  [添加] 战斗配置 dropefect_weapon 魔物掉落武器时显示掉落特效
* 20181209  [添加] 战斗配置 dropefect_armor 魔物掉落防具时显示掉落特效
* 20181209  [添加] 战斗配置 dropefect_card 魔物掉落卡片时显示掉落特效
* 20181209  [添加] 战斗配置 dropefect_shadowgear 魔物掉落影装时显示掉落特效
* 20181209  [添加] 物品DB db/pre-re(re)/item_dropeffect.txt 对单个物品设置物品掉落特效
* 20181209  [拓展] 脚本指令 npcshopattach 拓展购买事件添加返回 @sold_idx 数组，返回物品在背包中的序列。
* 20181209  [优化] 优化路点系统
* 20181209  [优化] 优化BOT系统
* 20181209  [添加] 支持客户端销毁副本按钮
* 20181209  [修复] 官方RA 在发布版中才存在的一处随机传送魔物的BUG
* 20181206  [优化] 宕机就会输出DMP文件
* 20181206  [优化] 脚本指令 unitsteppath 增加参数 <其他参数><"结束调用事件">
* 20181206  [添加] 脚本指令 sortarray 用以对数组元素排序
* 20181201  [添加] 脚本指令 mobdrop 用来模拟魔物掉落
* 20181201  [添加] GM指令 killmonsterall 用来杀死地图上的魔物并触发事件, 多用来测试脚本
* 20181201  [添加] 地图标记 mobspawnbase 修改相应地图新生成魔物的血量、物理攻击、魔法攻击和命中率倍率
* 20181127  [添加] 战斗配置 homunculus_max_aspd 用来限制生命体攻速, 默认为180
* 20181127  [修复] 生命体自动攻击AI
* 20181125  [修复] 修复 callshop 类型为10时 需要点两次才可以打开商店的BUG
* 20181125  [添加] 生命体自动攻击AI（测试版，下次更新时可稳定使用）
* 20181125  [添加] 脚本指令 homattack 开启生命体自动攻击, 弥补2017年的禁用了生命体自动攻击
* 20181125  [添加] GM指令 homattack 开启生命体自动攻击, 弥补2017年的禁用了生命体自动攻击
* 20181125  [添加] 战斗配置 homunculus_autofeed_rate 生命体饥饿到指定值时喂食（修复不喂食BUG）
* 20181125  [添加] 战斗配置 shadowgear_refine_ui 影装是否可以在精炼UI中精炼
* 20181123  [添加] 精炼UI对铁匠的祝福的支持
* 20181122  [修复] 删除账号时不能取消预约的BUG
* 20181121  [添加] 精炼UI
* 20181121  [添加] 脚本指令 refineui 用来打开精炼UI
* 20181121  [添加] GM指令 @refineui 用来打开精炼UI
* 20181121  [修改] 精炼脚本支持精炼UI版
* 20181121  [添加] 服务端支持开关 feature.conf -> feature.refineui
* 20181121  [添加] 商人开启露天商店可以使用cash点数和物品作为交易货币。
* 20181121  [添加] 战斗配置 露天商店拓展开关 extended_vending
* 20181121  [添加] 战斗配置 露天商店显示交易货币 show_item_vending
* 20181121  [添加] 战斗配置 露天商店显示更多交易信息 ex_vending_info
* 20181121  [添加] 战斗配置 露天商店Zeny展示物品 item_zeny
* 20181121  [添加] 战斗配置 露天商店Cash展示物品 item_cash
* 20181119  [修改] 99级GM可以使用签到系统
* 20181119  [修改] BetterRa默认封包版本为 20180620
* 20181119  [修复] 常量BL_NPC重复赋值的问题
* 20180930  [添加] 战斗配置 mvpdrop_last_hit_loot mvp掉落的物品由最后一击，决定首先捡取
* 20180909  [修复] 玩家不能进入非法副本
* 20180904  [更新] 更新服务器中显示的 BetterRa 网址
* 20180903  [优化] 脚本指令 getatkevent 使用方法
* 20180903  [优化] 每个玩家的攻击事件增加至10个
* 20180903  [添加] 战斗选项 atcommand_info 可以在使用管理员指令的时候输出更多信息
* 20180903  [添加] 脚本指令 unitgettarget 可以获得任何GID对应的攻击目标
* 20180903  [关闭] 战斗选项 instance_lastpoint 此功能为实验功能, 已取消实际效果
* 20180903  [修复] 仓库容量上限显示错误的问题
* 20180903  [修复] 脚本指令 getunitdata 输入不存在的GID的时候, 取消报错, 返回数字0
* 20180903  [添加] 脚本指令 sleep3 合并sleep和sleep2功能
* 20180903  [修复] JRO技能公式的一处错误
* 20180827  [修复] map_delbot 宕机问题
* 20180827  [添加] 脚本指令 isequippedcnt2 可以检测插在装备上的任何道具
* 20180827  [添加] 脚本指令 isequipped2 可以检测插在装备上的任何道具
* 20180827  [优化] 脚本指令 bot_stop 添加第二个参数为1时 bot被踢掉
* 20180820  [更新] 转盘规则 demo 文件
* 20180820  [添加] 当服务器宕机时, 输出dmp文件以便定位错误.
* 20180820  [修改] 自定义宏定义规则
* 20180820  [修复] http->get接口, 不能获取网络内容的问题
* 20180814  [修复] 脚本指令 getrateidx 报错问题
* 20180814  [添加] 脚本指令 roulette_bonus 设置幸运大转盘的 bonus 物品
* 20180814  [添加] 脚本指令 roulette_open 打开幸运大转盘, 并且定位到相应位置
* 20180814  [添加] 脚本指令 roulette_generate 转盘开始转动, 停在指定的位置, 并显示当前摇奖的bonus物品
* 20180814  [添加] 脚本指令 roulette_iteminfo 获得幸运大转盘中物品信息
* 20180814  [修改] 脚本事件名称 OnPCGetRouletteEvent -> OnPCGetRouletteFilter 使其支持打断
* 20180814  [添加] JRO 猫族技能公式 (未测试)
* 20180814  [添加] 测试版支持, 只能同时登陆5个账号
* 20180802  [添加] 在GM指令mapinfo中加入扩展mapflag显示
* 20180729  [添加] 战斗配置 npc_timeout 和NPC对话永远不超时。
* 20180729  [添加] 脚本指令 unit_Astart_walkto 全图寻路指令。
* 20180729  [添加] 脚本指令 mobstaypos 将魔物固定于一点，被固定的魔物只能在坐标点周围活动。
* 20180729  [添加] 脚本指令 instance_name 获取玩家所拥有的副本的名字。
* 20180729  [添加] 定制选项 C_MAX_HATEFFECT 最大特殊头饰效果的ID上限
* 20180729  [修复] 战斗配置 pet_catch_rate_mode 设置为2时 报错问题。
* 20180729  [修复] 当NPC对话框出现时触发事件，不会影响对话内容。
* 20180729  [修复] 鉴定事件被触发时不能再次和NPC对话的问题
* 20180718  [修复] translation.conf 文件中描述错误的问题。
* 20180718  [添加] 脚本指令名称变更 setsteppath —> unitsteppath 增加寻路AI系统。
* 20180718  [添加] 脚本指令 magicshield 魔法护盾，用来抵消伤害（非免费）。
* 20180718  [添加] 脚本指令 getbonus 一般用在 OnPCStatCalcEvent 指令下面，用来获取当前的 bonus 信息，以此调整 bonus。
* 20180718  [添加] 脚本指令 getrateidx 用于获取概率系数。具体查看脚本说明。
* 20180718  [添加] 脚本指令 bot_login 用于登陆假人。
* 20180718  [支持] GM指令 @recall 支持传送，挂店玩家。
* 20180718  [支持] 影子装备支持插卡，需要在影子装备填写卡槽数，需要卡片填写装备位置。
* 20180718  [修复] 修复路点反应迟钝的问题，优化路点回归路径的算法（更节省CPU），添加AI支持。
* 20180718  [支持] 变量商店 pointshop 支持变量名为 “变量中文名|变量” 这种写法，当打开商店时，商店显示“变量中文名”。
* 20180718  [添加] 事件 OnPCInsertCardFilter 当玩家准备插入卡片的时候触发事件。
* 20180718  [添加] 事件 OnPCInsertCardEvent 已经成功将卡片插入到装备时触发事件。
* 20180718  [修改] 满级后最大经验值 MAX_LEVEL_BASE_EXP 为 INT_MAX。
* 20180718  [修改] 满级后最大职业经验值 MAX_LEVEL_JOB_EXP 为 INT_MAX。
* 20180718  [修复] 脚本指令 getinventorylist 返回变量 inventorylist_idx 错误问题。
* 20180718  [修复] 脚本指令 addatkevent 攻击事件触发时会排队的问题。
* 20180718  [修复] 事件 OnPCIdentifyEvent 有时变量返回错误的问题
* 20180701  [添加] 脚本指令 readstatus 获取属性的总量
* 20180701  [添加] 技能暴击支持
* 20180701  [添加] 物品扩展 bMultiDamageRate 概率翻倍伤害
* 20180701  [添加] 物品扩展 bFixedCastAddRate 固定咏唱技能时间累加
* 20180701  [添加] 脚本指令 getareamember 获得范围内的团队成员
* 20180701  [修复] EVENT_PARTYLEAVEFILTER 事件代码的一处错误
* 20180701  [修复] SP_MAXASPD 攻速不正确问题
* 20180701  [修复] SP_SKILL_ITEM_FIX 错误提示中数值不正确问题
* 20180701  [优化] 脚本指令 gettarget 可以传入任何GID
* 20180629  [添加] 战斗配置项 pc_max_def pc_max_def2 pc_max_mdef pc_max_mdef2 pc_max_flee pc_max_flee2 用来限制最大值
* 20180629  [开启] 流量信息在窗口标题中显示
* 20180629  [开启] 技能伤害修正 db/skill_damage.txt
* 20180629  [优化] 当使用变量名做为商店消耗时 支持 变量名|变量释义 只显示变量释义
* 20180629  [添加] 脚本指令 instance_name 用于获取玩家所开启的副本的副本名
* 20180629  [添加] 脚本指令 readstatus 用于获取玩家的素质总数 例：readstatus(bStr);
* 20180629  [修复] ATK最大不能超过 65535 的限制
* 20180629  [修复] 路点系统的一处错误
* 20180603  [优化] 路点和寻路
* 20180526  [添加] 战斗配置 amulet_type 用于设定护符类物品存在于背包哪个栏目
* 20180526  [添加] 物品扩展 bonus bCooldownrate,n; 技能独立延迟时间 + n%
* 20180526  [添加] 物品扩展 bonus3 bSkillItemFix,sk,iid,n; 技能 sk 消耗的物品ID iid 增加 n 个(n可以为负数)
* 20180526  [上传] chat-webclient.html 聊天网页客户端 用浏览器打开使用
* 20180526  [修改] mob_ele_attack 为 NO
* 20180526  [添加] 战斗配置 pc_cast_rate 玩家变动咏唱时间按百分比调整
* 20180526  [添加] 战斗配置 pc_fixed_cast_rate 玩家固定咏唱时间按百分比调整
* 20180526  [添加] 战斗配置 pc_cooldown_rate 玩家技能独立延迟按百分比调整
* 20180526  [添加] 战斗配置 pc_delay_rate 玩家技能公共延迟按百分比调整
* 20180526  [翻译] 战斗配置文件 conf/battle/skill.conf
* 20180526  [添加] 战斗配置 mob_ele_attack 魔物的普通攻击是否带有自身属性
* 20180526  [添加] 物品拓展 bSkillTreeDelay 职业技能独立技能延迟调整
* 20180526  [添加] 物品拓展 bMaxASPD 修改ASPD上限
* 20180526  [添加] 添加 chat-server 服务端和接口
* 20180521  [添加] 战斗配置 pet_ignore_intimate_decrease 无视宠物的亲密减少, 无论发生什么, 宠物的亲密度都不会减少
* 20180521  [添加] 战斗配置 short_attack_damage_rate 近距离攻击伤害倍率(非技能) (百分比选项)
* 20180521  [添加] 战斗配置 long_attack_damage_rate 远距离攻击伤害倍率(非技能) (百分比选项)
* 20180521  [添加] 战斗配置 weapon_attack_damage_rate 物理技能伤害倍率 (百分比选项)
* 20180521  [添加] 战斗配置 magic_attack_damage_rate 魔法技能伤害倍率 (百分比选项)
* 20180521  [添加] 战斗配置 misc_attack_damage_rate 特殊技能伤害倍率 (百分比选项)
* 20180521  [添加] 战斗配置 enable_damage_rate 伤害倍率修正对谁有效 (掩码选项)
* 20180521  [添加] 战斗配置 enable_re_exp_fix 是否开启复兴版等级惩罚的经验修正(基础经验和职业经验)
* 20180521  [添加] 战斗配置 enable_re_drop_fix 是否开启复兴版等级惩罚的掉落修正
* 20180521  [添加] 物品属性 bSkillTreeCooldownRate 职业技能公共技能延迟按百分比调整
* 20180521  [添加] 物品属性 bSkillTreeCooldown 职业技能公共技能延迟调整
* 20180521  [添加] 物品属性 bSkillTreeDelayRate 职业技能独立技能延迟按百分比调整
* 20180521  [添加] 物品属性 bSkillTreeDelay 职业技能独立技能延迟调整
* 20180520  [添加] 脚本指令http_get用于发送GET指令. 返回时触发事件 OnHTTPReceivedEvent
* 20180520  [支持] 支持加载网络脚本(加密/不加密).
* 20180520  [支持] 支持脚本加密, 加密工具在QQ群共享, QQ群 419290524
* 20180519  [更新] BetterRA 脚本加密解密工具
* 20180506  [添加] 配置选项 min_storechat_distance 商店/聊天窗与玩家的采购商店、贩卖商店、聊天窗口之间的最小距离
* 20180506  [添加] 配置选项 min_npc_distance 商店/聊天窗口与NPC之间的最小距离
* 20180506  [添加] 配置选项 instance_lastpoint 玩家在副本中下线后, 重新登陆是否会在下线的位置登陆(副本中)
* 20180506  [添加] 配置选项 instance_leavepoint 玩家离开副本以后是否记录玩家离开副本的位置
* 20180506  [添加] 脚本指令 instance_enter_last 进入副本中记录的点
* 20180506  [提交] 帐号类型跨区登录及交易补丁.patch 暂不启用
* 20180506  [增加] 事件 OnBOTLackItemEvent 当BOT的消耗品少于5个时触发的事件, 返回 @lack_itemid 物品ID
* 20180506  [增加] BOT技能支持, 详情请查看 npc\BetterRa\BetterRa_bot.txt
* 20180506  [修复] 登录时不能顶下BOT账号的BUG
* 20180506  [增加] 事件 OnBOTLackItemEvent 当BOT的消耗品少于5个时触发的事件, 返回 @lack_itemid 物品ID
* 20180506  [增加] BOT技能支持, 详情请查看 npc\BetterRa\BetterRa_bot.txt
* 20180506  [修复] 登录时不能顶下BOT账号的BUG
* 20180506  [修复] getsameipinfo指令 可以返回IP数量
* 20180307  [合并] 合并分支 rathena
* 20180306  [修复] 鉴定事件错误
* 20180306  [修复] 脚本指令 delinventory 不能使用第2个参数的问题
* 20180127  [合并] 合并分支 rathena
* 20180126  [添加] 战斗配置 display_skill_name 不是玩家释放的技能时, 是否要在对象头顶显示技能名字
* 20180126  [添加] 脚本指令 unitskilluse 对某个目标使用技能
* 20180126  [添加] 脚本指令 unitskilltowide 范围技能扩散
* 20180126  [添加] 脚本指令 addatkevent 添加攻击时触发事件
* 20180126  [添加] 脚本指令 delatkevent 删除攻击时触发事件
* 20180126  [添加] 脚本指令 getatkevent 获得攻击时触发事件参数
* 20180126  [添加] 脚本指令 gettarget 获取正在攻击的目标的GID
* 20180126  [添加] 脚本指令 unitskillfixed 对某个技能的, 技能伤害\念咒进行修正, 只生效一次
* 20180126  [添加] 脚本指令 unitskillfixedreset 重置技能修正
* 20180126  [添加] 脚本指令 unitheadmsg 在某个GID对象头上显示文字
* 20180116  [添加] 脚本事件 OnPCProgressAbortEvent 当使用 when_progressbar_abort_doevent 后读条被打断触发
* 20180116  [添加] 脚本指令 when_progressbar_abort_doevent 用于 OnPCProgressAbortEvent 支持
* 20180116  [添加] 脚本指令 mobskill 让指定GID的怪物使用技能 酷头提供
* 20180116  [修改] 指令 getsameipinfo 支持获取脚本关联玩家的打开的号
* 20180102  [添加] 战斗配置 enable_coply 当其他职业拥有流氓的抄袭技能时, 是否可以复制技能
* 20180102  [修复] 当背包大于128时, 商城不能购买的问题.
* 20180102  [添加] 脚本指令 getidxrandomoption 用来获取玩家背包中物品的OPT
* 20180102  [添加] 脚本指令 setidxrandomoption 用来设置玩家背包中物品的OPT
* 20180102  [添加] 属性拓展 bWeaponAddDamage 直接增加每次武器攻击的伤害
* 20180102  [添加] 属性拓展 bMagicAddDamage 直接增加每次魔法攻击的伤害
* 20180102  [添加] 属性拓展 bMiscAddDamage 直接增加每次特殊攻击的伤害
* 20180102  [添加] 脚本指令 is_bot 用来判断指定玩家是否为机器人
* 20180102  [添加] 脚本指令 bot_run 启动机器人, 配置方法请查看 doc/BetterRa_script_commands.txt
* 20180102  [添加] 脚本指令 bot_reset 用来重新配置机器人参数, 配置方法请查看 doc/BetterRa_script_commands.txt
* 20171228  [添加] 护身符道具 类型为20, 需要在 conf/battle/BetterRa.conf 启用护符
* 20171228  [添加] 机器人系统
* 20171222  [修改] 提取 packet_keys 配置服务端封包混淆KEY的设置提取到 conf/map_athena.conf
* 20171222  [添加] 地图标记 nopet 禁止玩家在此地图上使用宠物, 登陆到此地图宠物会自动变蛋回到玩家背包.
* 20171222  [添加] 地图标记 nohomunculus 禁止玩家在此地图上使用生命体, 登陆到此地图生命体会被自动安息.
* 20171222  [添加] 地图标记 nomercenary 禁止玩家在此地图上使用佣兵, 登陆到此地图佣兵会被自动解散.
* 20171222  [修改] 宠物脚本 pet_skill_attack 的伤害太小的问题
* 20171222  [修改] 指令名称 hasfollow -> getservantgid 用来获取玩家有没有 宠物、佣兵、生命体、元素体等
* 20171222  [添加] 脚本指令 tribe_mob_spawn 用于召唤指定阵营下的魔物
* 20171214  [添加] 战斗配置 buff_for_guild_member 在GVG模式下 群体辅助技能 是否对所有公会成员有效
* 20171214  [添加] 战斗配置 buff_for_tribe_member 在阵营模式下 群体辅助技能是否对同阵营成员（玩家、魔物）有效
* 20171214  [添加] 脚本指令 tribe_getteam 获取玩家的阵营ID
* 20171214  [添加] 脚本指令 tribe_setteam 将玩家添加至某个阵营
* 20171214  [添加] 脚本指令 tribe_mobteam 将上次召唤的魔物全部添加至某个阵营
* 20171212  [添加] 路点系统 使用 setsteppath 为魔物设置路点
* 20171212  [添加] 阵营系统 在 mapflag 被设为 tribe 时生效
* 20171209  [添加] 脚本指令 setsteppath 用于设置MOB的行走路径, 配置以后魔物会自动行走
* 20171209  [添加] policies.conf 文件
* 20171209  [添加] 脚本配置文档 npc/script_BetterRa.conf
* 20171209  [添加] demo文件 npc/BetterRa/BetterRa_event_demo.txt
* 20171209  [添加] GM指令 policy 用于设置指定玩家的行为策略编号
* 20171209  [添加] GM指令 title 用于更改称号
* 20171209  [添加] policies 行为策略，用于动态修改 经验 掉率 GM等级等
* 20171209  [添加] setpolicyid 设置指定玩家的行为策略编号, 配合 conf/policies.conf 中定义的规则使用
* 20171209  [添加] getpolicyid 获取指定玩家的行为策略编号, 配合 conf/policies.conf 中定义的规则使用
* 20171209  [添加] getpolicyinfo 获得指定玩家当前的行为策略详情, 配合 conf/policies.conf 中定义的规则使用
* 20171208  [添加] 脚本指令 mail_create mail_additem* mail_send 发送邮件支持
* 20171208  [添加] BD配置文档 item_announce.txt 根据物品掉率来判定是否需要进行公告
* 20171208  [修复] 登陆时, 显示"不知名区域"的BUG
* 20171207  [修复] getitem3 delitem3 等指令不能使用玩家关联的变量的问题
* 20171207  [添加] GM指令 recallmap 将地图上的玩家瞬移至身旁
* 20171207  [添加] 脚本指令 hasfollower 用来获取玩家有没有 宠物、佣兵、生命体、元素体等
* 20171206  [添加] 脚本指令 unitremove 可以通过GID移除实体, 并播放指定的消失动画
* 20171205  [修复] 事件队列问题带来的一系列问题
* 20171205  [修复] callshop 后玩家可以随意移动的问题
* 20171205  [添加] 战斗配置 combo_mode 用于配置 item_combo 效果生效策略
* 20171204  [汉化] 生命体DB中的名称
* 20171204  [添加] 地图标记 hideguildinfo 隐藏公会信息
* 20171204  [添加] 地图标记 hidepartyinfo 隐藏组队信息
* 20171204  [添加] 地图标记 notoken 不能使用复活之证
* 20171204  [添加] 地图标记 nocapture 不能捉宠物
* 20171204  [添加] 地图标记 noautoloot 不能使用自动拾取
* 20171204  [添加] BetterRa 战斗配置文件 conf/battle/BetterRa.conf
* 20171204  [添加] 战斗配置 max_pvp_aspd 玩家在PVP地图时的最大攻速限制
* 20171204  [添加] 战斗配置 max_gvg_aspd 玩家在GVG地图时的最大攻速限制
* 20171204  [添加] 战斗配置 extremityfist_castend_ball_check 阿修罗霸凰拳咏唱完毕时, 是否再次确认气弹数量
* 20171204  [添加] 战斗配置 extremityfist_castend_status_check 阿修罗霸凰拳咏唱完毕时, 是否再次确认“爆气”状态
* 20171204  [添加] 战斗配置 extremityfist_nocast_sp 使用“阿修罗霸凰拳”不会直接耗尽玩家的SP
* 20171204  [添加] 战斗配置 loadevent_for_eachmap 强制所有地图触发 OnPCLoadMapEvent 事件
* 20171204  [添加] 战斗配置 get_identify_equip 通过特定渠道获得的装备, 强制变成已鉴定
* 20171204  [添加] 战斗配置 mounting_cant_use 当玩家乘坐商城坐骑后, 是否禁止使用物品
* 20171204  [添加] 战斗配置 hide_cmd 当没有权限的玩家执行了GM指令是否隐藏并返回一条信息
* 20171204  [添加] 战斗配置 bAspd_fixed 使用 bonus bAspd,x; 进行攻速加成时, x 值是否会受到其他加成因素影响
* 20171204  [添加] 战斗配置 share_quests_area 在什么位置才能共享任务的杀怪进度
* 20171204  [修改] 战斗配置 show_mob_info 添加 级别/编号/体型/种族/属性和属性等级
* 20171204  [添加] 脚本指令 identifybyidx 根据指定的背包序号, 对该道具进行鉴定操作
* 20171204  [添加] 脚本指令 setrenttime(resume) 增加/减少指定位置装备的租赁时间
* 20171204  [添加] 脚本指令 script4each 对指定范围的玩家执行相同的一段脚本
* 20171204  [添加] 脚本指令 itemexists(existitem) 确认服务器的 item_db 中是否存在指定物品
* 20171204  [添加] 脚本指令 getequipexpiretick(isrental) 获得指定位置装备的租赁到期剩余秒数.
* 20171204  [添加] 脚本指令 gethotkey(get_hotkey) 获取指定快捷键位置的信息
* 20171204  [添加] 脚本指令 sethotkey(set_hotkey) 设置玩家快捷键上的内容
* 20171204  [添加] 脚本指令 statuscalc(status_calc) 为了兼容一些脚本
* 20171202  [添加] 脚本指令 warpparty2 用于传送整个组队, 可以传送死亡玩家
* 20171202  [添加] 脚本指令 setheaddir 调整角色头的朝向
* 20171202  [添加] 脚本指令 setbodydir 调整角色身体的朝向
* 20171202  [添加] 脚本指令 party_leave 让指定的角色立刻退出队伍
* 20171202  [添加] 脚本指令 openbank 让指定的角色立刻打开银行界面(只对拥有账号随身银行的客户端版本有效)
* 20171202  [添加] 脚本指令 instance_users 获取副本实例中的参与人数
* 20171202  [添加] 脚本指令 mesclear 清空当前NPC的对话框内容
* 20171202  [添加] 脚本指令 cap_value 用来确保数值变量不会低于最小值, 超过最大值
* 20171202  [添加] 脚本指令 mobremove 根据GID移除一个魔物(只是移除, 没有死亡动画)
* 20171202  [添加] 脚本指令 getsameipinfo 获得某个IP在线的玩家数以及玩家的角色编号等信息
* 20171202  [添加] 脚本指令 logout 使指定的角色立刻登出游戏(踢下线)
* 20171202  [添加] 脚本指令 searcharray 查询数组中第一次出现待查询内容的索引序号
* 20171202  [修改] 脚本指令 getinventorylist 添加返回 @inventorylist_idx[] 和 @inventorylist_uid$[]
* 20171202  [添加] 脚本指令 countinventory 根据指定背包序号, 获取该道具在背包中的数量
* 20171202  [添加] 脚本指令 equipinventory 穿戴指定背包位置的装备
* 20171202  [添加] 脚本指令 unequipinventory 脱下指定背包位置的装备
* 20171202  [添加] 脚本指令 viewequip 查看指定账号在线角色的装备信息
* 20171202  [添加] 脚本指令 setrenttime 增加/减少指定位置装备的租赁时间
* 20171202  [添加] 脚本指令 battleignore 将指定的角色设置为魔物无视状态
* 20171202  [添加] 脚本指令 sc_check 用于判断指定的状态(Buff)是否存在, 并取得状态参数
* 20171202  [添加] 脚本指令 bonus_script_remove 移除指定的 bonus_script 效果
* 20171202  [添加] 脚本指令 getchartitle 获得指定玩家的称号ID
* 20171202  [添加] 脚本指令 getequipidx 用来获取穿戴在指定位置的装备的“背包位置序号”.
* 20171202  [添加] 脚本指令 resetoptionbyidx 给定背包位置序号, 清空物品的随机属性
* 20171202  [添加] 脚本指令 resetoptionbypos 给定装备位置编号, 清空物品的随机属性
* 20171202  [添加] 脚本指令 getoptionbyidx 给定背包位置序号, 获得物品的随机属性
* 20171202  [添加] 脚本指令 getoptionbypos 给定装备位置编号, 获得物品的随机属性
* 20171202  [添加] 脚本指令 setoptionbyidx 给定背包位置序号, 配置物品的随机属性
* 20171202  [添加] 脚本指令 setoptionbypos 给定装备位置编号, 配置物品的随机属性
* 20171202  [添加] 脚本指令 duplicate 将一个指定的NPC复制到一个新的位置
* 20171202  [添加] 脚本指令 copynpc 将一个指定的NPC复制到一个新的位置
* 20171202  [添加] 脚本指令 unloadduplicate 卸载用脚本复制的NPC
* 20171202  [添加] 脚本指令 unloadcopynpc 卸载用脚本复制的NPC
* 20171202  [添加] 事件 OnPCGuildCreateFilter 玩家创建公会前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCPetSkillFilter 玩家宠物使用技能前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCGuildJoinFilter 玩家加入公会后触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCGuildLeaveFilter 玩家离开公会后触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCPetLootEvent 玩家宠物拾物品后触发
* 20171202  [添加] 事件 OnPCPetFeedEvent OnPCFoodPet 玩家给宠物喂食后触发
* 20171202  [添加] 事件 OnPCViewEquipFilter 玩家查看其他玩家装备后触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCUseSkillExFilter 玩家施放技能前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCUseSkillExEvent 玩家技能施放后触发
* 20171202  [添加] 事件 OnPCUseItemExFilter 玩家使用道具前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCUseItemExEvent 玩家使用道具后触发
* 20171202  [添加] 事件 OnPCWillLoadMapEvent 玩家进入地图或切换地图前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCInChatroomFilter 玩家进入聊天室前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCChangeTitleFilter 玩家改变称号前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCOpenRouletteFilter 玩家打开幸运转盘前触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCCloseRouletteEvent 玩家关闭幸运转盘后触发
* 20171202  [添加] 事件 OnPCStartRouletteFilter 玩家在转盘点击开始后触发, 可使用 processhalt 中断
* 20171202  [添加] 事件 OnPCGetRouletteEvent 玩家在转盘点击获取物品后触发
* 20171201  [添加] 事件 OnPCHomLvUpEvent 玩家的人工生命体升级后触发
* 20171201  [添加] 事件 OnPCHomCallEvent 玩家召唤出人工生命体后触发
* 20171201  [添加] 事件 OnPCHomRestEvent 玩家将人工生命体安息后触发
* 20171201  [添加] 事件 OnPCHomDeadEvent 玩家的人工生命体死亡后触发
* 20171201  [添加] 事件 OnPCHomAliveEvent 玩家复活人工生命体后触发, 可使用 processhalt 中断
* 20171201  [添加] 事件 OnPCIdentifyFilter 玩家成功鉴定装备前触发
* 20171201  [添加] 事件 OnPCIdentifyEvent 玩家成功鉴定装备后触发
* 20171201  [添加] 事件 OnBfReloadScriptEvent 刷新脚本前触发
* 20171201  [添加] 事件 OnAfReloadScriptEvent 刷新脚本结束时触发
* 20171201  [添加] 事件 OnPCKillMvpEvent 杀死MVP魔物时触发
* 20171201  [添加] 事件 OnPCEquipFilter 玩家准备穿戴物品后触发
* 20171201  [添加] 事件 OnPCEquipEvent 玩家穿戴物品后触发
* 20171201  [添加] 事件 OnPCUnequipFilter 玩家准备脱掉物品后触发
* 20171201  [添加] 事件 OnPCUnequipEvent 玩家成功脱掉物品后触发
* 20171201  [添加] 事件 OnPCBuffStartEvent 玩家获得状态(Buff)后触发
* 20171201  [添加] 事件 OnPCBuffEndEvent 玩家解除状态(Buff)后触发
* 20171201  [添加] 事件 OnPCPartyCreateFilter 当玩家创建队伍前触发, 可使用 processhalt 中断
* 20171201  [添加] 事件 OnPCPartyJoinFilter 当玩家加入队伍前触发, 可使用 processhalt 中断
* 20171201  [添加] 事件 OnPCPartyLeaveFilter 当玩家离开队伍前触发, 可使用 processhalt 中断
* 20171130  [添加] item_flag.txt支持物品不会由于被玩家主动使用而消耗(256), 物品不会由于被发动技能所需要而消耗(512)
* 20171130  [汉化] db数据库文件中文化
* 20171129  [汉化] conf配置文件中文化
* 20171129  [添加] 用于配置新加入的控制台输出文本 conf/msg_conf/BetterRa_map_msg.conf
* 20171128  [添加] 控制台翻译代码, 控制台翻译文件 conf\msg_conf\translation.conf
