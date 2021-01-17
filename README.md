# 简介
创建 BetterRa创建于2017年11月28日..他是在Rathena的基础上经过二次修改的一款模拟器, 加入了大量的功能..

# 使用方法
#####使用方法只针对 `github` 仓库拉取代码的朋友, 下载完整包的朋友请忽略. 
仓库中没有启动所必须的 login-server.exe char-server.exe 和 map-server.exe, 请到网站 http://betterra.cn/customization.html?page=server 定制并放入根目录后, 可运行 runserver.bat 启动.
如果你不会架设环境, 也可以到 http://betterra.cn 下载完整包, 将包中 `BetterRA服务端一键启动（新手专用）.exe` 工具复制到目录下, 使用一键启动功能..

# 更新日志
* 20210117  [添加] 兼容旧版物品数据库相关文档移至目录 db\import-tmpl\csv_version
* 20210117  [修复] 文档变更任务数据 quest_db.yml 升级, 版本号 2, 旧版数据库无法使用
* 20210117  [添加] 文档变更物品数据 yml 化完成, 其他物品相关数据文档均已移入 yml 版数据库中
* 20210117  [添加] 战斗配置 itemdb_reload_thread 重读 item_db.yml 是否启用多线程加载
* 20210117  [添加] 重读 item_db.yml 数据库时默认开启多线程加载
* 20210116  [优化] GM指令 @allskill 直接学会任务技能 避免技能在使用装备和上下线后被重置
* 20210116  [优化] 脚本指令 unitskilltowide 移除限制条件
* 20210116  [优化] 脚本指令 unitskilltowide2 移除限制条件
* 20210116  [优化] 脚本指令 unitskilltoAOE 移除限制条件
* 20210116  [修复] 当使用2020客户端时 因装备穿戴 重置技能导致的技能在技能栏中停留的问题
* 20210116  [修复] 当 skill_db 中的技能数量超过 1250 个以后无法显示在技能列表中的问题
* 20210115  [修复] 机器人找不到敌对目标的问题
* 20210115  [修复] 机器人瞬移后依然以上一个敌对目标作为攻击目标的问题
* 20210114  [添加] 脚本指令 startsellitem 为 NPC 绑定的兑换商店增加一个可兑换道具
* 20210114  [添加] 脚本指令 sellitemcurrency 为兑换物品添加一项所需道具
* 20210114  [添加] 脚本指令 endsellitem 完成兑换道具添加
* 20210114  [添加] 脚本指令 bartershop 为指定角色开启一个兑换商店
* 20210114  [添加] 脚本结构 bartershop 简易的兑换商店, 用法类似于 itemshop
* 20210114  [添加] 脚本指令 getitemcombo 返回物品所绑定的套装属性
* 20210114  [添加] 脚本指令 runcomboscript 运行套装数组所保存的套装效果
* 20210114  [优化] 脚本事件 OnPCRefineUIFilter 加入参数 @refine_zeny 精炼时消耗的zeny
* 20210114  [优化] 脚本事件 OnPCRefineUIEvent 加入参数 @refine_blessing 精炼是否使用祝福矿石
* 20210112  [修复] bot寻路失败掉线问题
* 20210112  [添加] 脚本指令 refineui_result 指令用于自定义精炼UI, 播放精炼动画, 并修改物品精炼值
* 20210111  [修复] 物品附加 bSkillToAround bSkillForAOE 攻击单体魔物时攻击两次的问题
* 20210106  [修复] 物品附加 bSkillCritical bSkillAttribute 使用技能名称无效的问题
* 20210106  [添加] 当加载NPC时所使用的外观无效时, 增加详细报错
* 20210106  [添加] 物品附加 bReborn 当玩家死亡时有 r% 的概率复活并恢复 n% 的血量
* 20210101  [添加] 魔物技能 ID: 696 NPC_CHANGEUNDEAD2 转为不死属性
* 20210101  [添加] 魔物技能 ID: 712 NPC_PULSESTRIKE2 脉冲攻击
* 20210101  [添加] 魔物技能 ID: 713 NPC_DANCINGBLADE 刃舞
* 20210101  [添加] 魔物技能 ID: 723 NPC_STORMGUST2 暴风雪
* 20210101  [添加] 魔物技能 ID: 729 NPC_CHEAL 治愈之光
* 20210101  [添加] 魔物技能 ID: 730 NPC_SR_CURSEDCIRCLE 咒缚阵
* 20210101  [添加] 魔物技能 ID: 731 NPC_DRAGONBREATH 龙之气息
* 20210101  [添加] 魔物技能 ID: 732 NPC_FATALMENACE 致命威胁
* 20210101  [添加] 魔物技能 ID: 733 NPC_MAGMA_ERUPTION 熔岩喷发
* 20210101  [添加] 魔物技能 ID: 735 NPC_MANDRAGORA 曼陀罗魔花的尖叫
* 20210101  [添加] 魔物技能 ID: 737 NPC_RAYOFGENESIS 创世之光
* 20210101  [添加] 魔物技能 ID: 738 NPC_VENOMIMPRESS 毒抗性弱化
* 20210101  [添加] 魔物技能 ID: 739 NPC_CLOUD_KILL 毒云术
* 20210101  [添加] 魔物技能 ID: 740 NPC_IGNITIONBREAK 致命爆裂
* 20210101  [添加] 魔物技能 ID: 741 NPC_PHANTOMTHRUST 幻影刺击
* 20210101  [添加] 魔物技能 ID: 742 NPC_POISON_BUSTER 剧毒爆发
* 20210101  [添加] 魔物技能 ID: 743 NPC_HALLUCINATIONWALK 疾风步
* 20210101  [添加] 魔物技能 ID: 744 NPC_ELECTRICWALK 电流路径
* 20210101  [添加] 魔物技能 ID: 745 NPC_FIREWALK 火焰路径
* 20210101  [添加] 魔物技能 ID: 750 NPC_EARTHQUAKE_K 地崩术
* 20201227  [修复] 移除 conf/maps_athena.conf 中的 copy 关键字, 移动至 db/map_index.txt 中
* 20201224  [添加] 魔物技能 ID: 736 NPC_PSYCHIC_WAVE 精神冲击
* 20201222  [添加] 魔物技能 ID: 707 NPC_MILLENNIUMSHIELD 千年神盾
* 20201222  [添加] 魔物技能 ID: 749 NPC_WIDECRITICALWOUND 广范围致命伤害
* 20201222  [优化] 物品附加函数带来的技能更新算法
* 20201221  [添加] 魔物技能 ID:747 NPC_LEASH 死亡之手
* 20201221  [添加] 魔物技能 ID:748 NPC_WIDELEASH 广范围死亡之手
* 20201220  [添加] 魔物技能 ID: 746 NPC_WIDEDISPEL 广范围魔法效果解除
* 20201220  [添加] 脚本指令 unitblown 移动个体到指定的坐标
* 20201220  [添加] 脚本指令 unitmove 移动个体到指定的坐标
* 20201219  [添加] 魔物技能 ID:700 NPC_WIDEHEALTHFEAR 广范围恐惧
* 20201219  [添加] 魔物技能 ID:701 NPC_WIDEBODYBURNNING 广范围燃烧
* 20201219  [添加] 魔物技能 ID:702 NPC_WIDEFROSTMISTY 广范围冰洁
* 20201219  [添加] 魔物技能 ID:703 NPC_WIDECOLD 广范围冰冻
* 20201219  [添加] 魔物技能 ID:704 NPC_WIDE_DEEP_SLEEP 广范围沉睡
* 20201219  [添加] 魔物技能 ID:705 NPC_WIDESIREN 广范围魅惑
* 20201219  [添加] 魔物技能 ID:721 NPC_WIDEWEB 广范围易燃之网
* 20201219  [添加] 魔物技能 ID:722 NPC_WIDESUCK 广范围吸血
* 20201219  [添加] 魔物技能 ID:751 NPC_ALL_STAT_DOWN 全素质下降
* 20201219  [添加] 魔物技能 ID:752 NPC_GRADUAL_GRAVITY 重力增加
* 20201219  [添加] 魔物技能 ID:753 NPC_DAMAGE_HEAL 转祸为福
* 20201219  [添加] 魔物技能 ID:754 NPC_IMMUNE_PROPERTY 属性免疫
* 20201219  [添加] 魔物技能 ID:755 NPC_MOVE_COORDINATE 位置转换
* 20201215  [添加] 脚本指令 bot_delmode 移除机器人的AI模式
* 20201215  [添加] 脚本指令 bot_hasmode 检查机器人的AI模式
* 20201215  [添加] 脚本指令 bot_pause 暂停机器人
* 20201215  [优化] 脚本指令 is_bot 加入检查BOT是否处于闲置状态还是运行状态
* 20201215  [优化] 脚本指令 bot_stop 当玩家离线挂机时, BOT停止后会被踢掉线
* 20201215  [修复] 脚本事件 OnBOTRecvActEvent 返回值不正确的问题
* 20201215  [修复] 脚本指令 pctalk 私聊返回两次名字的问题
* 20201209  [优化] 脚本指令 bot_login 支持 角色ID 玩家名直接登陆BOT账号
* 20201209  [优化] 脚本指令 bot_login 登陆过程中, 脚本暂停等待登陆完成后, 脚本关联会转移至登陆的BOT账号
* 20201209  [移除] 脚本事件 OnBOTLackItemEvent 当BOT自动使用物品后少于指定数量触发该事件
* 20201209  [添加] 脚本事件 OnBOTRecvActEvent 当BOT接收到指定请求或聊天信息时触发该事件
* 20201209  [添加] 脚本指令 bot_trade 当BOT收到交易请求时, 可以通过这个指令操作BOT交易
* 20201209  [添加] 脚本指令 bot_party 当BOT收到加入队伍请求时, 可以通过这个指令操作BOT接受或拒绝申请
* 20201209  [添加] 脚本指令 bot_guild 当BOT收到加入公会请求时, 可以通过这个指令操作BOT接受或拒绝申请
* 20201209  [添加] 脚本指令 pctalk 发送一条聊天信息给指定目标或群组
* 20201209  [添加] 脚本指令 pcjoinchat 加入一个与指定GID的玩家或NPC相关的聊天室
* 20201208  [添加] GM指令 showconsole 可在游戏中查看 map-server.exe 的输出信息
* 20201208  [添加] 机器人实现加入组队、加入公会、交易、聊天触发和邀请触发事件的功能
* 20201207  [修复] 机器人账号在机器人运行时登录的延迟下线的问题
* 20201207  [修复] 因上次修改机器登陆AI, 导致的登陆机器人指令造成的宕机
* 20201207  [修复] 仇恨系统在释放时造成的宕机
* 20201205  [修复] 规避玩家走近目标使用技能时, 客户端与服务端行走时间计算规范的问题
* 20201201  [修复] 脚本指令 unitdelaura 删除无效指针的问题
* 20201201  [修复] 当地图标记 mobinfo等于 -1 时不能移除名称扩展显示的问题
* 20201126  [优化] 优化 mapcache 读取时内存读写, 优化加载速度
* 20201125  [添加] 脚本指令 getskillsc 获取技能使用后给玩家附加的状态ID, 可用于BOT相关的状态条件
* 20201123  [修复] 机器人不检查目标死亡状态的问题
* 20201113  [添加] 脚本指令 bot_follow 机器人自动跟随一个玩家
* 20201108  [修复] 物品附加 bSkillToAround 和 bSkillToGround 重登后不能将技能移入技能栏的问题
* 20201108  [修复] 使用脚本指令 bot_login 登录的玩家不能被移动的问题
* 20201101  [添加] 物品附加 bEquipIgnore 无效装备指定属性
* 20201101  [添加] 物品附加 bEquipIgnoreClass 移除装备职业限制
* 20201101  [添加] 脚本指令 dual_weapon 允许玩家双持武器
* 20201030  [添加] 物品附加 bEquipLoc 更改装备穿戴位置
* 20201030  [添加] 物品附加 bEquipBound 修改物品绑定类型
* 20201030  [添加] 物品附加 bEquipIgnore 无效装备指定属性
* 20201026  [修复] 脚本指令 instance_npcname 无法获取 copynpc 生成的NPC真实名称的问题
* 20201026  [优化] 脚本指令 duplicate copynpc dupnpc 脚本说明
* 20201026  [优化] 脚本指令 unloadduplicate unloadcopynpc 脚本说明
* 20201026  [修复] 脚本指令 logout 无法使离线挂机和离线挂店的人下线的问题
* 20201024  [修复] 配置选项 min_storechat_distance 可以重叠挂店开聊天室的问题
* 20201024  [修复] 技能数据文件中 IgnoreLandProtector配置 不能无视地领的问题
* 20201024  [修复] 脚本指令 logout 使用玩家名时无法正确退出游戏的问题
* 20201019  [修复] GM指令 whodrops 不能正常获取掉率的问题
* 20201019  [修复] 无法点击魔物墓碑的问题
* 20201018  [修复] 自动挂机机器人系统卡AI的问题
* 20201016  [修复] 音速投掷伤害公式
* 20201014  [优化] 修改文件 db/mob_item_ratefix.txt 可通过此文件修改魔物掉率和显示掉率
* 20201014  [修复] 手推车中装备数量超出448时造成的宕机问题
* 20201010  [添加] db/import/mapcache/ 文件夹, 将地图的 gat 和 rsw 文件放入此文件, 会优先读取此文件夹中的地图信息
* 20201010  [添加] 脚本指令 itempreview 刷新正在查看的物品信息
* 20201010  [修复] 旧版csv数据库 pet_db.txt 参数的兼容性问题
* 20201010  [优化] 脚本指令 unitspecialeffect 添加参数 发送目标 与 目标GID
* 20201010  [优化] 脚本指令 unitaura 可以有多个光环共存
* 20201010  [添加] 脚本指令 unitdelaura 为指定实体移除一个光环特效, 在客户端版本大于 20181002 会直接移除特效
* 20201010  [添加] 脚本指令 setnpcdistance 指令玩家距必须靠近NPC多少格之内才可以点击, 一般使用在 OnInit 事件下设置
* 20201010  [添加] 脚本指令 getdatatype 返回给定<参数>的原始类型. 返回的类型是掩码类型
* 20201010  [添加] 脚本指令 tostring 将给定的参数转换为字符串类型.
* 20201010  [添加] 脚本指令 chr 返回整数对应的 ASCII 字符
* 20201010  [添加] 脚本指令 ord 返回字符对应的 ASCII 数值
* 20201010  [添加] 脚本指令 log10 返回传入数值 log 10 的结果
* 20201009  [添加] 脚本指令 specialeffectnum 使目标显示一个带参数的特效效果
* 20201009  [添加] 脚本指令 removespecialeffect 移除目标身上的一个特效效果
* 20201009  [添加] 脚本指令 getarrayindex 获取数组变量中的 key 值, 一般用在函数调用中获取传参 getarg() 的源变量中括号中的数字
* 20201009  [添加] 脚本指令 countnameditem 获取身上有多少个署名物品, 具体参考 getnameditem
* 20201009  [添加] 脚本指令 getguildinfo 获取指定类型的公会相关信息
* 20201009  [添加] 脚本指令 gettimer 返回由 addtimer() 创建的玩家时钟的相关信息
* 20201008  [修复] 技能磁场不能使人不能移动的问题
* 20201008  [修复] 在连续升级时成就系统不能正常触发成就检查的问题
* 20201008  [修复] 技能闪光脚不能重新标记魔物的问题
* 20201008  [添加] 战斗配置 mob_dmgrate_db 是否开启 mob_dmgrate_db.txt 功能
* 20201008  [添加] 数据库文件 mob_dmgrate_db.txt 在一些官服中, 特定的魔物受到的伤害只有 1/10 可以通过此数据库配置减伤
* 20201007  [添加] 地图标记 convenio 地图上无视限制条件, 可以使用大主教集结技能
* 20201007  [修复] 战斗配置 extremityfist_castend_ball_check 在组合技能后不能正确检查气弹数量的问题
* 20200928  [优化] 脚本指令 getcharid 输入无效的玩家名会返回脚本出错的位置
* 20200928  [修复] 当使用4级武器时精炼加成不正确的问题
* 20200928  [修复] 地图标记 hideguildinfo 会显示公会图标和地图位置的问题
* 20200928  [修复] 地图标记 hidepartyinfo 会显示地图未知的问题
* 20200927  [添加] GM指令 mapname 通过地图ID查询地图名字
* 20200927  [修复] 战斗配置 bonus_addmax 取消最大值限制
* 20200924  [修复] import中的 instance.yml 地图不能覆盖 db中预先加载的地图导致NPC载入两次的问题
* 20200923  [修复] 上次因修复多脚本对战造成的队列不能触发的问题
* 20200923  [修复] doc中的几处描述错误
* 20200923  [优化] 脚本指令 script4each 支持官方阵营系统
* 20200916  [修复] 物品附加 bSkillAttribute 技能ID无效的问题
* 20200916  [修复] 脚本指令 setinventoryinfo 设置词条时不管用的问题
* 20200916  [添加] 脚本指令 array_keys 获取数组中所有值非0(非空字符串)的键, 并将键存入指定的数组中
* 20200916  [修改] 仇恨系统索敌范围修改为魔物的range3
* 20200916  [修复] reloadbattleconf 造成的宕机问题
* 20200915  [添加] 脚本事件 OnPCOpenBuyingFilter 当玩家开启采购商店之前触发
* 20200915  [修复] 可重生魔物不能正确的去除仇恨和临时技能问题
* 20200913  [修复] 魔导机甲与一些状态共存的问题
* 20200913  [修复] 由于卸除装备失败造成的复制装备的问题
* 20200909  [修复] 由于触发NPC交互过程中点击其他NPC导致的无法移动的问题
* 20200909  [添加] 战斗配置 ignore_insert_card_specific 对应位置(左侧或右侧)的饰品卡片可以插在左右通用饰品(发夹)上
* 20200909  [添加] 网页定制 最大聊天室人数 最小不小于 1 最大不大于 3000
* 20200909  [优化] 脚本指令 setdragon 取消对职业的检查
* 20200908  [修复] 魔物释放逐影一些技能造成的宕机
* 20200908  [优化] 脚本指令 setwall 可在墙的坐标点召唤不能被攻击的魔物作为标记
* 20200908  [添加] 脚本指令 setwallline 由坐标 (x0,y0) 到 (x1,y1) 画直线, 画线, 本指令采用 Bresenham 算法
* 20200908  [添加] 脚本指令 setwallcircle 以坐标 (x,y) 为圆心, 以<半径>画圆, 本指令采用 Bresenham 算法
* 20200908  [优化] 脚本指令 delwall 通过墙名字删除墙, 当删除副本中的墙时, 必须加入副本ID
* 20200908  [优化] 脚本指令 checkwall 查询墙是否存在, 当查询副本中的墙时, 必须加入副本ID
* 20200908  [添加] 脚本指令 delwallmap 删除地图上的所有墙
* 20200908  [添加] 脚本指令 getlinexy 获得坐标 (x0,y0) 到 (x1,y1) 直线上的坐标, 并存入传入坐标中
* 20200908  [添加] 脚本指令 getcirclexy 获得坐标 (x,y) 为圆心, 以<半径>圆的坐标, 并存入传入坐标中
* 20200906  [修复] 脚本指令 getvariableofpc 不能正常获取玩家变量的问题
* 20200906  [优化] 脚本事件 OnPCKillMvpEvent 添加返回参数 killedgid
* 20200906  [添加] 脚本事件 OnMobDeadEvent 魔物被杀死时触发该事件 (此事件不关联玩家)
* 20200906  [添加] 所有魔物死亡事件都可以通过 $killedgid $killedrid $killerrid 三个变量获取相应数据
* 20200906  [修复] 上次更新后 disablenpc 以后仍然可以触发事件的问题
* 20200906  [修复] A星寻路漏掉的1格距离检查的问题
* 20200906  [修复] 物品附加 bBlockStatus 设置 STATUS_ASPD 时攻速显示不正常的问题
* 20200905  [添加] GM指令 @cashshopsale 在 item_cash_db.txt 中的第八项 8: Sale 可以通过此指令上架到 cashshop 中
* 20200905  [添加] 脚本指令 getheaddir 获取角色的脑袋朝向
* 20200905  [添加] 脚本指令 getbodydir 获取角色身体的朝向
* 20200905  [修复] 战场上下线造成的宕机问题
* 20200905  [添加] 脚本指令 bounditembyidx 绑定背包中 背包序号 指定的道具
* 20200905  [添加] 脚本指令 bounditembypos 绑定装备栏中 装备编号 指定的道具
* 20200904  [优化] 脚本指令 disablenpc enablenpc hideonnpc hideoffnpc 加入角色ID参数可对角色进行操作
* 20200904  [添加] 脚本指令 disableontouch 关闭 NPC 的 OnTouch 触发
* 20200904  [添加] 脚本指令 enableontouch 开启 NPC 的 OnTouch 触发
* 20200904  [修复] 脚本指令 instance_addmap 在不复制NPC情况下走入源地图NPC触发范围内的报错问题
* 20200902  [添加] 脚本指令 unit_cross_map_walkto 跨图寻路指令 
* 20200830  [修复] 当减伤修正大于 100 时减伤后伤害大于减伤前的问题
* 20200826  [添加] 脚本指令 unitaura 为指定实体增加一个光环特效, 如果是短特效则每过间隔时间播放一次特效
* 20200823  [优化] 脚本指令 getunitstatus 可以忽略gid输入, 标示获取关联玩家的战斗属性
* 20200823  [添加] 战斗配置 string.status_atk 自定义素质 ATK 公式
* 20200823  [添加] 战斗配置 string.status_matk_l 自定义素质最小 MATK 公式
* 20200823  [添加] 战斗配置 string.status_matk_r 自定义素质最大 MATK 公式
* 20200823  [添加] 战斗配置 string.status_def_r 自定义减算物理防御公式
* 20200823  [添加] 战斗配置 string.status_mdef_r 自定义减算魔法防御公式
* 20200823  [添加] 战斗配置 string.status_hit 自定义命中率公式
* 20200823  [添加] 战斗配置 string.status_flee_l 自定义回避率公式
* 20200823  [添加] 战斗配置 string.status_flee_r 自定义完全回避率公式
* 20200823  [添加] 战斗配置 string.status_cri 自定义暴击率公式
* 20200823  [添加] 物品附加 bBlockStatus 固定玩家属性 x 为 n
* 20200823  [添加] 网站定制 初始创建角色数量, 可通过修改 login 表中的 character_slots 创建更多角色
* 20200822  [修复] 仇恨系统因为魔物mode导致的来回切换攻击目标的问题
* 20200822  [添加] 战斗配置 damage_return_mode 物理攻击反射最大伤害模式
* 20200822  [添加] 网站定制添加 魔物伤害记录数量 的定制选项
* 20200821  [修复] 螺旋刺击 武器重量对伤害的权重问题
* 20200821  [修复] 召唤师 职业技能等级调整到 60级
* 20200821  [移除] 战斗配置 overlay_back_stack
* 20200819  [修复] 复兴后 骑乘攻击 应为远距离攻击
* 20200819  [添加] 战斗配置 drop_connection_on_quit 解决玩家正常退出时, 玩家无法保存自己配置的问题
* 20200819  [添加] KRO官放 2019.07.16 对大法师技能优化
* 20200819  [添加] 在使用 20181121 及以后的客户端时支援21亿物品ID
* 20200819  [修复] 在使用 import中的pet_db.yml设置脚本时造成的内存泄露问题
* 20200818  [修复] 在开着邮箱过图或瞬移后不能使用邮箱的问题
* 20200818  [优化] 台服版攻速公式按照 https://rocalweb.github.io/ 计算公式更新
* 20200817  [优化] 脚本指令 OnPCAttackFilter 和 OnPCAttackedFilter 优先级排在伤害计算最后
* 20200813  [修复] 脚本指令 setequipedcard 在 20181121 版本之后客户端不能使用的问题
* 20200813  [修复] 当 mounting_cant_use 禁止可使用物品时不能使用坐骑缰绳的问题
* 20200812  [添加] 脚本指令 getmapmember 获取地图内指定组队、公会、战场成员的AID
* 20200812  [修复] 脚本指令 getcharbonus 获取不到值的问题
* 20200812  [添加] 脚本指令 checktrade 检查玩家是否正在交易, 并返回交易对方的 AID
* 20200812  [添加] 脚本指令 checkcashshop 检查玩家是否正在使用 cashshop 现金商城
* 20200812  [添加] 脚本指令 checkmailwriting 检查玩家是否正在写邮件
* 20200812  [添加] 脚本指令 checkrefineui 检查玩家是否正在使用精炼UI
* 20200812  [添加] 物品附加 bAddMastery 增加掌控伤害 n
* 20200812  [优化] itemflag 中的 256 可以无视conf选项 mounting_cant_use 限制
* 20200811  [优化] 脚本事件 OnPCTradeAddFilter 支持返回对方的 aid 和 cid
* 20200811  [优化] 脚本事件 OnPCPickLogEvent 支持返回 @pick_type$ log 类型, @pick_partner_aid 物品来自对方的AID
* 20200810  [修复] 取消技能对免疫负面属性魔物的强制僵直
* 20200809  [优化] 脚本指令 getareamember 增加地图名查找设置
* 20200809  [修复] 脚本指令 delmobskill 说明错误
* 20200807  [修复] 邮件增加物品宕机的BUG
* 20200807  [修复] 限购现金商城不能正确删除商城物品的问题
* 20200807  [修复] 现金商城免费点数支付的问题
* 20200806  [修复] 魔物在被攻击时不能正确的使用技能的问题
* 20200806  [修复] 灵媒技能艾斯敦只能击晕中型魔物的问题
* 20200805  [添加] 战斗配置 preloading_mvps 使用 boss_monster 召唤出来的魔物无视动态魔物刷新(monster.conf 中的 dynamic_mobs)直接生成在地图上
* 20200805  [修复] 读入 map_cache.dat 越界宕机
* 20200805  [修改] 公会经验上限修改至 9223372036854775807 显示上限为 2147483647
* 20200805  [添加] 支持邮箱检索邮件按钮, 支持全部删除和全部接收按钮
* 20200805  [添加] 物品附加 bMagicSubDefEle 减少魔法属性伤害, 以支援官方的一些物品脚本
* 20200805  [添加] 为玩具娃娃工厂添加 NPC_HELLBURNING 地狱燃烧 技能
* 20200805  [修复] 脚本指令 skilleffect 与 npcskilleffect 不能显示技能特效的问题
* 20200805  [修复] 武器的所获得的攻击力会覆盖掉原本武器带的ATK2攻击力的问题
* 20200805  [修复] 种族减伤卡片与全种族减伤卡片效果相互抵消的问题
* 20200805  [修复] 当玩家与NPC交互超时, 会弹出关闭窗口, 点击关闭造成的一系列报错问题
* 20200805  [添加] 配置选项 homunculus_starving_rate 生命体饱食度小于多少时, 改变饱食度判定间隔
* 20200805  [添加] 配置选项 homunculus_starving_delay 生命体饱食度小于homunculus_starving_rate时, 间隔多久判定一次饱食度状态
* 20200805  [修复] 当特定技能使用失败时, 取消玩家正正在使用该技能的状态, 以确保技能可以正常使用
* 20200805  [修复] 脚本指令 getbossinfo 无效的问题
* 20200805  [添加] 物品附加 bDefSize 修改体型大小为 s
* 20200805  [添加] 物品附加 bDefRace 修改种族为 r
* 20200803  [添加] 物品附加 bDefEleLevel 修改防御属性等级为 n
* 20200803  [修复] 战斗配置 npc_timeout 设置为 no 时卡脚本的问题
* 20200803  [添加] 脚本事件 OnPCAutoSpellFilter 当自动念咒时触发
* 20200803  [添加] 脚本事件 OnPCAutoSpellEvent 当自动念咒后触发
* 20200803  [优化] 脚本事件 OnAnyoneUseSkillFilter 当释放技能前触发, 可通过在skill_db中自创ID为20000以上的技能通过打断实现一些新技能
* 20200803  [优化] 脚本事件 OnAnyoneUseSkillEvent 返回变量更名,优化事件返回参数的读取速度
* 20200803  [优化] 脚本事件更名 OnPCUseSkillExFilter -> OnPCUseSkillFilter, 返回变量更名, 优化事件返回参数的读取速度
* 20200803  [优化] 脚本事件更名 OnPCUseSkillExEvent -> OnPCUseSkillEvent, 返回变量更名, 优化事件返回参数的读取速度
* 20200803  [优化] 脚本事件更名 OnPCUseItemExFilter -> OnPCUseItemFilter, 返回变量更名,优化事件返回参数的读取速度
* 20200803  [优化] 脚本事件更名 OnPCUseItemExEvent -> OnPCUseItemEvent, 返回变量更名,优化事件返回参数的读取速度
* 20200802  [修复] 当玩家聊天窗不存在时产生的宕机问题
* 20200802  [修复] 脚本指令 itemlist skilllist 在选择框只有一项时返回不正确的问题
* 20200731  [添加] 经验值提高到 uint64位类型, 最高等级提升至200
* 20200731  [修复] 当NPC超时时map服务器输出debug信息的问题
* 20200731  [优化] 脚本指令 getbonus 当 bf 参数为 -1 时 则忽略 bf参数, 获取的结果为所有满足条件之和
* 20200730  [优化] 脚本事件 OnHTTPReceivedEvent 增加参数 $@http_code 返回代码（200 404 301 302 502等）
* 20200730  [修复] 读入 MySQL 数据库无法正确获取物品和魔物变量的问题
* 20200730  [修复] 脚本事件 OnPCAttackFilter 与 OnPCAttackedFilter 不能正常获取 dmg_mark 的问题
* 20200730  [优化] 脚本指令 getbonus 增加对 bFinalAddRace 与 bFinalAddClass 的支持
* 20200729  [添加] 脚本指令 getbonus getcharbonus bonus_minfix bonus_maxfix 支援 bLongAtkRate 和 bCritAtkRate
* 20200729  [修复] 物品附加 bMagicDamageReturn 只对指向性技能有效
* 20200729  [添加] 脚本指令 openchat 开启一个聊天窗口
* 20200729  [添加] 脚本指令 openvending 开启一个贩卖商店, 手推车中必须有这些物品.
* 20200729  [添加] 脚本指令 openbuying 开启一个采购商店, 最多只能同时采购 3 个物品.
* 20200723  [修复] 读入csv版本副本数据时, 因为地图不存在导致的宕机问题
* 20200723  [修复] 通过csv创建的副本无法被副本销毁按钮销毁的问题
* 20200723  [修复] 配置选项 pet_ignore_intimate_decrease 选项在降低好感度时直接降低到1的问题
* 20200723  [添加] 脚本指令 rfifo_int 当 OnPCSocketRecvFilter 事件接收到封包时, 可以用本指令获取‘接收封包’中对应位置的数字数据.
* 20200723  [添加] 脚本指令 rfifo_str 当 OnPCSocketRecvFilter 事件接收到封包时, 可以用本指令获取‘接收封包’中的文本数据.
* 20200723  [添加] 脚本指令 wfifo_int 当 OnPCSocketRecvFilter 事件接收到封包时, 可以用本指令改写‘发送封包’的数字数据.
* 20200723  [添加] 脚本指令 wfifo_str 当 OnPCSocketRecvFilter 事件接收到封包时, 可以用本指令改写‘发送封包’的文本数据.
* 20200723  [添加] 脚本指令 fifo_debug 将 ‘发送封包’ 或 ‘接收封包’ 打印到控制台, 或是禁止事件结束后发送 ‘发送封包’ 给客户端.
* 20200723  [添加] 脚本事件 OnPCSocketRecvFilter 当服务端接收到封包时, 触发该事件.
* 20200720  [修复] 元素精灵导致的宕机问题
* 20200718  [添加] 脚本事件 OnPCReviveFilter 当玩复活时触发该事件
* 20200718  [修复] 脚本指令 autobonus autobonus2 autobonus3 在 OnPCStatCalcEvent 事件下无效的问题
* 20200718  [修复] 脚本指令 unitdefratefixed 造成的宕机问题
* 20200718  [修复] 物品附加 bAddEle bSubEle 使用 Ele_All 没有效果的问题
* 20200718  [修复] 物品附加 bSubRace 使用 RC_ALL 没有效果的问题
* 20200718  [修复] 客户端 20190724 之后的版本不能换公会图标的问题
* 20200718  [修复] 由于副本DB转yml造成的掉线回传失效的问题
* 20200717  [修复] 脚本指令 autobonus 在 OnPCStatCalcEvent 事件下使用不生效的问题
* 20200715  [修复] 复兴后出生点出生在空气墙上的问题
* 20200714  [修复] 当玩家死后技能持续输出伤害的问题
* 20200714  [修复] GM指令 itemreset 导致宠物不能收回的问题
* 20200714  [修复] GVG最低伤害小于 1 导致攻击华丽miss的问题
* 20200714  [修复] 脚本指令 sendmail 发送不存在的物品发生的宕机问题
* 20200713  [修复] 在201811月的封包版本中精炼UI显示苹果的问题
* 20200712  [添加] 文件 status_change_db.yml 用于定义每一种状态在特定的条件下的进行清除、保留。
* 20200709  [修复] 技能公式 db/skill_formula_db.yml bonus 计算远程增伤无效的问题
* 20200709  [修复] 脚本指令 skf_card_fix 当无视NK值时, 不采用默认NK值的问题
* 20200707  [添加] 脚本指令 setunitdata 设置前缀图标 UXXX_GROUP_ID
* 20200707  [添加] 脚本指令 getunitdata 获取前缀图标 UXXX_GROUP_ID
* 20200707  [添加] 脚本指令 setunittitle 设置前缀名称
* 20200707  [添加] 脚本指令 getunittitle 获取前缀名称
* 20200705  [优化] 添加 2019 客户端版本之后对公会图标的支持
* 20200705  [优化] 脚本指令 itemlist 脚本指令直接返回玩家选择的 物品ID
* 20200705  [优化] 脚本指令 skilllist 脚本指令直接返回玩家选择的 技能ID
* 20200705  [移除] 移除事件 OnPCSelectItemEvent
* 20200705  [移除] 移除事件 OnPCSelectSkillEvent
* 20200705  [修复] 在 20181121 后的版本的客户端中不能使用扩展挂店的问题
* 20200705  [修复] 脚本指令 itemlist 在 20181121 后的版本的客户端中不能使用的问题
* 20200705  [添加] 脚本指令 getcharbonus 获取指定玩家的 bonus 参数
* 20200705  [优化] db/skill_formula_db.yml 中的变量更改为系统变量, 避免关联玩家时造成变量不可用的问题
* 20200705  [修复] 脚本指令 searchstorage 相关问题, 优化仓库ID参数置于条件参数后
* 20200704  [修复] 拓展仓库相关脚本指令, 在第一次使用时获取不到数据的问题
* 20200703  [修复] 物品附加 bSkillDamage 当技能等级为-1时对技能伤害加成无效的问题
* 20200703  [修复] 物品附加 bSkillCritical bSkillDamage bSkillAddDiv bSkillChangeType bSkillAttribute 使用技能名无效的问题
* 20200701  [添加] 战斗配置 at_logout_event 当使用 @at 离线后触发 OnPCLogoutEvent 事件
* 20200629  [优化] 取消 callshop 类型 10 对于物品类型和物品ID的数量限制
* 20200629  [添加] 物品附加 bRegen 每隔 t 毫秒, 恢复 HP 和 SP (不覆盖, 独立运算)
* 20200629  [添加] 物品附加 bRegenPercent 每隔 t 毫秒, 按百分比恢复 HP 和 SP(不覆盖, 独立运算)
* 20200627  [优化] 物品附加 bSkillAttribute 当sk为 0 时变更 n 的表达意义
* 20200627  [优化] 物品附加 bSkillAttribute 当sk为 -1 时为全部技能
* 20200627  [添加] 脚本指令 getbossinfo 获取使用 boss_monster 标签召唤出来的魔物的信息
* 20200627  [优化] 召唤师为独立种族, 默认 RC_PLAYER_DORAM 可在配置中通过 summoner_race 修改召唤师种族
* 20200627  [添加] 脚本指令 getmapinfo 获取地图相关的信息
* 20200627  [添加] 脚本指令 getmobspawninfo 获取魔物在哪些地图生成
* 20200625  [添加] 网页登陆 token 验证
* 20200625  [添加] OnTouch 支持移动的NPC触发事件
* 20200625  [添加] 脚本指令 rentalcountitem rentalcountitem2 rentalcountitem3 获取租赁物品数量
* 20200625  [优化] 脚本指令 rentitem 不能再获取租赁道具的数量
* 20200624  [修复] 幸运大转盘硬币不对应的问题 
* 20200622  [优化] 项目 chat-server 名成修改为 websocket-server
* 20200622  [优化] 项目 websocket-server 架构
* 20200622  [添加] 项目 websocket-server 加入 websocket 代理用于支持 robrowser
* 20200622  [修复] 当客户端版本小于 20141016 读取性别时的宕机问题
* 20200618  [优化] 副本销毁按钮可在 instance_db.yml 中禁用
* 20200618  [修复] 精炼事件当玩家缺少精炼材料时的宕机问题
* 20200617  [修复] 地图标记 mapflag inventory_db 客户端报错和服装外观刷新问题
* 20200617  [添加] 地图标记 nocart 禁止玩家在此地图上手推车
* 20200617  [优化] 地图标记 inventory_db 当切换背包后默认不能使用手推车
* 20200615  [修复] 脚本事件 OnPCAttackedFilter 和 OnPCAttackFilter 伤害不能突破21亿的问题
* 20200612  [[修复] 商城使用免费点数超出拥有点数的报错问题
* 20200612  [[修复] 获取技能名和技能说明时的宕机问题
* 20200612  [[添加] 脚本事件 OnMOBSpawnFilter 当魔物生成前触发
* 20200609  [修复] 由于攻速达到193以后不能触发连锁动作的问题
* 20200605  [添加] 魔物大型技能在念咒时显示红色警戒范围, skill_db 中 ShowScale 选项
* 20200605  [添加] 魔神塔副本增加怪物卡片掉落
* 20200604  [优化] 取消性别'U'保留性别'F'、'M'
* 20200603  [修复] 超级初学者死亡次数不能重新计数问题
* 20200603  [修复] 使用 @reloadmobdb 导致的宕机问题
* 20200603  [添加] 客户端支持至20200401
* 20200603  [修复] 加速子弹状态在切换子弹时会消失的问题
* 20200601  [添加] 物品附加 bSkillChangeType 可更改技能的bf, 影响最终伤害计算结果
* 20200601  [修复] 机甲状态下不能使用速度激发和凶砍的问题
* 20200531  [添加] 地图标记 mapflag inventory_db 可以在不同地图不同的背包
* 20200531  [添加] conf/inter_server.yml 支持扩展背包配置, 可与 mapflag 配合实现不同地图不同的背包
* 20200528  [修复] 仇恨系统宕机BUG
* 20200528  [修复] 仇恨系统地面技能对周围魔物不起作用的问题
* 20200528  [修复] 当魔物的小弟离开一定距离后不能返回主人旁边的问题
* 20200527  [修复] 二刀不检查 nk 直接多段暴击的问题
* 20200527  [添加] 检查小鸟挂的开关, 暂不开放使用
* 20200525  [修复] 战斗配置 guild_storage_official 在没有技能时不能打开仓库的问题
* 20200524  [添加] 脚本事件 OnInstanceDestroyEvent 当副本销毁时触发
* 20200524  [添加] 战斗配置 min_damage 最小伤害值
* 20200524  [优化] 战斗配置 attribute_recover 默认为打开
* 20200522  [修复] 复制NPC时 OnPCInChatroomFilter 事件不能正确获得NPC信息的问题
* 20200520  [删除] 战斗配置 gm_perfect_hide 此选项已被ra实现
* 20200520  [添加] 脚本指令 broadcast_item 弹出一条特殊的物品广播
* 20200518  [添加] 脚本事件 OnInstanceCreateFilter 当创建副本时触发
* 20200518  [添加] 脚本事件 OnInstanceCreateEvent 当创建副本成功后触发
* 20200518  [修复] 当 instance_db.yml ID重复时，不检查副本名是否重名
* 20200517  [修复] 魔物免疫mode在 mapflag mobdmgrate 标签下的伤害问题
* 20200517  [添加] skill_db.yml 添加 IgnoreFalse 使用技能失败不计算技能延迟和咏唱时间
* 20200517  [修复] NPC商店在使用 duplicate 复制时的内存释放问题
* 20200507  [修复] battleground 的宕机BUG
* 20200503  [修复] GM指令 @offline 以后再次登录会长时间登录失败的问题
* 20200501  [添加] KRO 2019.10.15 对十字刺客的技能优化
* 20200501  [添加] KRO 2019.10.15 对骑士系技能的优化
* 20200501  [添加] KRO 2019.12.04 对炼金系技能的优化
* 20200429  [修复] 精炼UI在精炼到最大值以后卡在上一级精炼值的问题
* 20200429  [添加] refine_db.yml 添加 MaxLevel 限制精炼最大等级, 限制影装精炼最高 10级
* 20200426  [修复] 魔物mode免疫物理远程魔法多段伤害只扣一滴血的问题
* 20200425  [修复] 精炼UI中影装精炼数据被防具数据替代的问题
* 20200425  [修复] 当 item_flag.txt 中 2048 选项开启时, item_announce.txt 中 1 选项不生效的问题
* 20200425  [修复] 战斗配置 max_baby_third_parameter 不管用的问题
* 20200425  [转换] 将技能相关数据表更新为 yaml 格式
* 20200425  [修复] 脚本指令 getfreecell 参数flag = 2时不管用的问题
* 20200425  [修复] 技能 SA_ABRACADABRA和WM_RANDOMIZESPELL 无视 noskill 地图标记的问题
* 20200425  [优化] 为 shop 增加折扣
* 20200425  [新增] KRO 2018.08.27 十六周年技能优化
* 20200425  [修复] 自由施法对攻速的计算
* 20200425  [优化] 脚本指令 mail 增加附件可以使用精炼和绑定设定
* 20200425  [新增] 脚本指令 bg_reserve, bg_unbook 和 bg_desert
* 20200425  [新增] 为战场系统增加队列系统
* 20200425  [修复] mob_avail.txt 的读取问题
* 20200425  [添加] GM指令 reloadachievementdb 重读成就数据
* 20200425  [添加] GM指令 reloadattendancedb 重读签到数据
* 20200425  [添加] GM指令 reloadinstancedb 重读副本数据
* 20200425  [优化] skill_damage_db 使用BL变量规定使用者
* 20200425  [添加] KRO 2018.11.26 对于一转二转的技能优化
* 20200425  [添加] 脚本指令 cloakonnpc 隐藏 NPC 但是不禁止NPC附带的脚本
* 20200425  [添加] 脚本指令 cloakoffnpc 显示 cloakonnpc 隐藏的NPC
* 20200425  [添加] 脚本指令 isnpccloaked NPC是否被隐藏
* 20200425  [添加] 战斗配置 land_protector_behavior 地领对范围溅射范围生效
* 20200425  [修复] 圣母祈福 和 霸邪之阵 能叠加使用的问题
* 20200425  [优化] quest_db.txt 转换为 yaml格式
* 20200425  [添加] 战斗配置 npc_emotion_behavior 当魔物使用表情时变为被动状态
* 20200425  [修复] 脚本指令 itemheal 恢复血量不正确的问题
* 20200425  [修复] 弹指神通 消耗气球蛋数量不正确的问题
* 20200425  [修复] 当写邮件时可以使用物品和走动的问题
* 20200425  [优化] instance_db.txt 转换为 yaml格式
* 20200425  [修复] 星帝技能的一些BUG
* 20200425  [修复] 使用玩家模型创建NPC不会走路而是闪现的问题
* 20200425  [修复] 强酸火焰瓶 攻击 中立屏障 玩家没有伤害的问题
* 20200425  [修复] 脚本指令 instance_check_party 检查自动挂店玩家的问题
* 20200425  [修复] 在WoE：TE中不检查星帝和灵魂收割者等级限制的问题
* 20200425  [修复] 当星帝 灵魂收割者 超级初学者使用 @bodystyle 1 指令导致的客户端崩溃的问题
* 20200425  [修复] 移除 虎炮 对于目标造成的SP减少的问题
* 20200425  [修复] 灵魂收集 状态重复计时的问题
* 20200425  [修复] 灵魂收集 在灵魂满时状态不取消的问题
* 20200425  [修复] 商店 NPC 聊天窗 之间距离检查的问题
* 20200425  [修复] 崩裂术技能公式错误的问题
* 20200425  [添加] KRO 2019.02.25 对叛乱的技能优化
* 20200425  [添加] 战斗配置 idletime_hom_option 生命体经验分享和物品拾取在什么状态下生效
* 20200425  [添加] 脚本指令 checkidlehom 返回指定挂生命体的玩家空闲时间
* 20200424  [修复] 物品附加 bSkillToAround 消失后不能正常选择目标的问题
* 20200424  [修复] 物品附加 bSkillToAround 技能范围大于3格不能判定为远距离攻击的问题
* 20200424  [修复] 物品附加 bSkillAttribute 属性为-3时造成的属性修正不正确的问题
* 20200424  [修复] 仇恨系统的近战OT问题
* 20200424  [修复] 仇恨系统的一些宕机BUG
* 20200422  [修复] 扩展仓库自动储存导致的仓库关闭的问题
* 20200422  [修复] 扩展仓库导致的物品消失的问题
* 20200422  [修复] 脚本指令 itemlist 在使用 item_avail 物品时不能正常显示的问题
* 20200422  [修复] 伤害修正后不能对特定攻击免疫的问题
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/instance_db.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/mob_avail.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/pet_db.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/quest_db.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/skill_cast_db.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/skill_castnodex_db.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/skill_db.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/skill_require_db.txt
* 20200421  [添加] 兼容CSV数据库 db/import/csv_version/skill_unit_db.txt
* 20200418  [添加] 战斗配置 mob_matk_equal_atk 魔物的魔法攻击力根据物理攻击力计算
* 20200418  [添加] 战斗配置 zero_steal_rate 零偷成功的概率, 表示偷窃多少次可以偷窃成功
* 20200418  [修复] 魔物的地崩伤害不基于物理攻击力的问题
* 20200418  [修复] 繁体“听”字不能用作角色名的问题
* 20200418  [修复] GM指令 删除 @autotrade2 修复 @at可以无商店离线挂机的问题
* 20200409  [优化] GM指令 删除 @offline 的条件和自动挂店标记
* 20200407  [添加] 物品附加 bFinalAddRace 使用 bf 攻击时, 增加 r 种族 x% 的伤害
* 20200407  [添加] 物品附加 bFinalAddClass 使用 bf 攻击时, 增加 c 类别魔物 x% 的伤害
* 20200407  [修复] 脚本指令 skf_card_fix 将伤害清零的问题
* 20200405  [优化] 仓库容量优化 目前网页可定制 1200 上限仓库容量
* 20200328  [优化] 脚本指令 instance_id 可以获取指定玩家的副本ID
* 20200326  [添加] 物品附加 bSkillCritAtkRate 技能暴击伤害 + n
* 20200326  [添加] 物品附加 bSkillAddDiv 技能 sk 打击段数增加 div, 伤害 + n
* 20200324  [添加] 仇恨系统第一版
* 20200324  [添加] 仇恨系统技能数据库 db/skill_aggro_db.txt
* 20200324  [添加] 脚本指令 mobaggroswitch 当 mob_aggro_all_mob 没有开启时, 控制单个魔物开启仇恨系统
* 20200324  [添加] 战斗配置 mob_aggro_enable 是否开启仇恨系
* 20200324  [添加] 战斗配置 mob_aggro_all_mob 是否开启所有魔物的仇恨系
* 20200324  [添加] 战斗配置 mob_aggro_ot_tick 当其他玩家的仇恨值大于魔物正在攻击的玩家时, 间隔多少时间后会改变攻击目标 (
* 20200324  [添加] 战斗配置 mob_aggro_ot_emotion 当其他玩家的仇恨值大于魔物正在攻击的玩家时, 魔物会重复做那种表
* 20200324  [添加] 战斗配置 mob_aggro_damge_rate 当玩家对魔物造成伤害时, 伤害的万分比会换算成魔物的仇恨
* 20200324  [添加] 战斗配置 mob_aggro_heal_rate 当玩家使用治疗系技能时, 治疗量的百分比会换算成魔物的仇恨
* 20200324  [添加] 战斗配置 mob_aggro_heal_range 治疗系技能仇恨范围, 当使用治愈类技能加血时范围内的敌对魔物都会获得仇恨
* 20200324  [添加] 战斗配置 mob_aggro_disengage 当玩家脱战多久后, 仇恨值会清
* 20200324  [添加] 战斗配置 mob_aggro_logout_clear 当玩家下线后是否清除玩家仇
* 20200324  [添加] 战斗配置 item_drop_gmlv 当 GM 等级大于等于多少级, 可以丢弃任何物
* 20200324  [优化] 更改旧版数据库支持文件夹, 将旧版数据库移入 db\import\old_db_version 中
* 20200324  [修复] 脚本指令 instance_name 传入参数不正确的问题
* 20200324  [修复] 脚本指令 getpartyleade 传入类型4时不能正确获取到副本地图的问题
* 20200324  [修复] 魔法效果解除可以对隐匿或伪装玩家使用的问题
* 20200324  [添加] 战斗配置 item_trade_expire_gmlv 根据GM等级解除租赁物品交易限制
* 20200324  [修复] 脚本事件 OnPCDropItemFilter 触发时机不对的问题
* 20200323  [修复] 当物品ID大于 65535 或小于 0 时会返回其他物品的问题
* 20200323  [优化] 为组队事件添加参数
* 20200323  [修复] 脚本事件 OnPCPartyJoinFilter 表现与说明不符的问题
* 20200323  [修复] BOT系统使用普通攻击会导致攻击失败次数过多的问题
* 20200321  [修复] 当玩家随机传送时位置时, 因地图不存而宕机的问题
* 20200321  [修复] 由于服务端之间链接丢失导致的 char-server.exe 宕机的问题
* 20200318  [修复] 在特殊情况下使用 autotrade 造成的宕机问题
* 20200318  [添加] GM指令 offline 在不挂店的情况下可以离线挂机
* 20200318  [修复] 修复 BOT 使用sc_end可以取消 BOT状态的问题
* 20200315  [修复] 当使用 特殊变量 作为 pointshop 时, 特殊变量不能正常被扣除的问题
* 20200313  [修复] 脚本指令 copynpc 复制NPC时在创建副本的状态下复制不成功的问题
* 20200313  [添加] 战斗配置 boss_skill_rate BOSS魔物的技能咏唱概率
* 20200313  [添加] 战斗配置 mvp_skill_rate MVP魔物的技能咏唱概率
* 20200313  [添加] 战斗配置 boss_skill_delay BOSS魔物的技能延迟百分比
* 20200313  [添加] 战斗配置 mvp_skill_delay MVP魔物的技能延迟百分比
* 20200312  [添加] 战斗配置 bonus_addmax 所有增伤修正叠加后的最大比率 (千分率)
* 20200312  [添加] 战斗配置 bonus_submax 所有减伤修正叠加后的最大比率 (千分率)
* 20200312  [修复] 在使用放入仓库指令时, 仓库会自动打开的问题
* 20200312  [修复] 脚本指令 getvariableofpc 获取不到正确值的问题
* 20200311  [优化] 脚本事件 OnPCOpenVendFilter 增加货币ID返回
* 20200310  [添加] item_trade.txt 添加 4096 选项, 无视交易限制可以挂店出售
* 20200310  [添加] 脚本事件 OnPCOpenVendFilter 当玩家完成挂店的之前触发
* 20200309  [[修复] 当对物品附加进行限制时, 类似于 SZ_ALL + SZ_BIG > 100 对伤害免疫的问题
* 20200307  [修复] 物品附加 bSkillNoRequire 支持技能名
* 20200307  [修复] 脚本指令 setpolicyid 在设置回原本设置后 头饰没刷新的问题
* 20200307  [优化] map-server 调试版改为多线程编译
* 20200307  [添加] 脚本事件 OnBOTStartFilter 当BOT启动时触发
* 20200307  [添加] 脚本事件 OnBOTStopEvent 当BOT停止时触发
* 20200307  [优化] 当BOT多次攻击或使用技能失败时自动退出挂机
* 20200305  [修复] 在机器人挂机过程中超时会自动退出登录的问题
* 20200305  [修复] 物品附加 bSkillAttribute 某些技能无效的问题
* 20200307  [添加] 物品附加 bMagicReturnDefRate 受到魔法反射伤害时, 伤害减少原来的 n%
* 20200305  [修复] 物品附加 bWeaponReturnDefRat 受到物理反射伤害时, 伤害减少原来的 n
* 20200305  [修复] 脚本文件中数据库读入算法，解决魔物技能读入不管用的问题
* 20200304  [添加] skill_castnodex_db.txt 冷却 1024 技能的冷却在技能释放成功后才开始计时
* 20200304  [添加] skill_castnodex_db.txt 延迟 1024 技能的延迟在技能释放成功后才开始计时
* 20200304  [修复] 逐影假面技能在使用失败时也进入冷却和延迟的问题 文件skill_castnodex_db.txt
* 20200304  [修复] 脚本事件 OnPCStorageAddFilter 不能正常返回idx的问题
* 20200304  [修复] 部分BONUS参数为负数时导致的map_server假死的问题
* 20200302  [优化] 当没有伤害的技能使用成功后有独立CD时将skill_cast_db.txt中 cooldown改为负数
* 20200301  [添加] 战斗配置 enable_delitem_event OnPC(*)AddFilter 事件支持在库存减少时触发
* 20200301  [添加] 脚本事件 OnPCPickLogEvent 当玩家记录获取失去物品时触发该事件
* 20200301  [修改] 脚本指令 searchcartinventory -> searchcart 兼容旧指令
* 20200301  [修改] 脚本指令 searchstorageinventory -> searchstorage 兼容旧指令
* 20200301  [添加] 脚本指令 searchguildstorage 过滤并查找公会仓库中的物品
* 20200301  [添加] 脚本指令 putcartbyidx 将背包中的物品放入手推车
* 20200301  [添加] 脚本指令 putstoragebyidx 将背包中的物品放入仓库
* 20200301  [添加] 脚本指令 putguildstoragebyidx 将背包中的物品放入公会仓库
* 20200301  [添加] 脚本指令 getcartbyidx 将手推车的物品拿到背包中
* 20200301  [添加] 脚本指令 getstoragebyidx 将仓库的物品拿到背包中
* 20200301  [添加] 脚本指令 getguildstoragebyidx 将公会仓库的物品拿到背包中
* 20200301  [添加] 脚本指令 delinventorybyidx 将背包中的物品删除
* 20200301  [添加] 脚本指令 delcartbyidx 将手推车中的物品删除
* 20200301  [添加] 脚本指令 delstoragebyidx 将仓库中的物品删除
* 20200301  [添加] 脚本指令 delguildstoragebyidx 将公会仓库中的物品删除
* 20200301  [优化] 脚本指令 (*)storage (*)guildstorage 会在线读取仓库, 不需要打开仓库才能操作
* 20200301  [修复] 脚本指令 getstorageinfo 不能使用仓库ID的问题
* 20200226  [优化] 将BOT中的技能状态与物品状态分开配置
* 20200225  [修复] 在mapserver中 玩家的mac不自动读取的问题
* 20200225  [添加] 脚本指令 getequipinfo 在装备的 item_script 中使用可以获得装备在背包中的IDX 和 装备ID
* 20200225  [添加] 脚本指令 getcardinfo 在卡片的 item_script 中使用可以获得卡片在装备卡槽中的IDX 和 卡片ID
* 20200225  [修复] 脚本指令 eaclass 不能使用第三个参数的问题
* 20200221  [添加] 脚本事件 OnPCProduceFilter 当玩家制作物品时触发
* 20200220  [修复] 脚本指令 sortarray 格式为 <数组>{,<标签>}; 时的报错问题
* 20200219  [修复] 战斗配置 npc_timeout NPC是否超时说明与实际效果反转的问题
* 20200215  [修复] 脚本指令 unitdefratefixed 和 settitleicon 传入无效的GID时会宕机的问题
* 20200214  [优化] BOT系统的AI流程, 使其占用更少的系统资源
* 20200214  [修复] 物品附加 SkillToGround 和 SkillToAround 造成的技能无效化的问题
* 20200214  [优化] 脚本事件 OnPCStorageAddFilter 对公会仓库生效, 返回参数增加 @recv_stortype
* 20200214  [修复] 脚本指令 summon 在召唤失败时的宕机问题
* 20200213  [修复] 2月10日更新后造成的 地图标签 mobspawnbase 鬼畜的问题
* 20200213  [修复] 脚本指令 skilllist 输出的菜单带有 空指令的问题
* 20200213  [优化] 魔物的物理/魔法攻击力可以高于 65545
* 20200213  [修复] 脚本底层在运行时, 菜单和虚拟数据丢失的问题
* 20200212  [修复] 机器人吃东西清零的问题
* 20200212  [修复] refreshequip 不生效的问题
* 20200212  [添加] 脚本指令 getskillinfo 获取技能信息
* 20200212  [添加] 脚本指令 getinventoryinfo 支持 类型 13 装备的换装穿戴位置(equipSwitch)
* 20200212  [添加] 脚本指令 setinventoryinfo 改变背包中的物品相应的数据
* 20200212  [添加] 脚本指令 setequipedinfo 改变装备中的物品相应的数据
* 20200212  [添加] 脚本指令 refreshinventory 刷新背包中的物品
* 20200212  [添加] 脚本指令 refreshequip 刷新背包中的装备
* 20200212  [修复] 机器人参数 @ignore_touch 不能无视传送和范围触发的问题
* 20200211  [修复] 装备插卡无效化的宕机问题
* 20200211  [添加] GM指令 @bot 开启或关闭自动挂机
* 20200211  [修复] 机器人吃回复药水吃一次就不能继续再吃的问题
* 20200210  [添加] 地图标记 pcatkrate 玩家的攻击伤害会被此标记按倍率修正
* 20200210  [添加] 地图标记 pcdmgrate  玩家所受到的伤害均会被此标记按倍率修正
* 20200210  [添加] 地图标记 otheratkrate 宠物生命体元素等的攻击伤害会被此标记按倍率修正
* 20200210  [添加] 地图标记 otherdmgrate 宠物生命体元素等所受到的伤害均会被此标记按倍率修正
* 20200210  [优化] 地图标记 合并 mobatkrate 与 mobatkrate2 为 mobatkrate 魔物的攻击伤害会被此标记按倍率修正
* 20200210  [优化] 地图标记 合并 mobdmgrate 与 mobdmgrate2 为 mobdmgrate 魔物所受到的伤害均会被此标记按倍率修正
* 20200210  [优化] 地图标记 mobdmgrate 与 mobatkrate 修改为万分率
* 20200210  [添加] 脚本指令 settitleicon 使某个GID显示图标和标题, 对任意单位都有效
* 20200210  [添加] 战斗配置 mobinfo_refresh_tick 魔物名称和血条刷新频率, 减少服务器负担
* 20200206  [添加] 脚本指令 skilllist 生成一个物品选择框, 当玩家选择完成后触发 OnPCSelectSkillEvent 事件
* 20200206  [添加] 脚本事件 OnPCSelectSkillEvent 当玩家选择技能列表中的物品后执行该事件
* 20200203  [添加] 脚本指令 unitdefratefixed 使对应的对象获得的伤害变为倍率后的伤害
* 20200203  [添加] 战斗选项 gm_perfect_hide 当使用 @hide 指令后, 实现完美隐藏, 不会被外挂和内挂发现
* 20200203  [添加] 脚本指令 next_dropitem_special 当下一次掉落在地上的物品, 会按照本次设定的特殊要求掉落
* 20200202  [添加] 脚本指令 getconstantstr 通过数值获取常量的字符串, 加常量前缀可以增加获得常量字符串的准确度
* 20200202  [添加] 物品附加 bonus3 bStatusAddBonus 当玩家拥有 sc 的时候, 会增加 bonus 参数为 n
* 20200202  [添加] 物品附加 bonus4 bStatusAddBonus 当玩家拥有 sc 的时候, 会增加 bonus 参数为 r,n
* 20200202  [添加] 物品附加 bonus5 bAddStatusVal 赋予状态时, 参数分别累加 val1, val2, val3, val4
* 20200202  [添加] 物品标签 item_flag.txt 添加专属标记 &2 插有该物品的装备变为时装/影装装备属性会无效化
* 20200202  [添加] 物品附加 bonus2 bAddSkillArea 增加 sk 技能 n 格范围, 可用于溅射或范围技能
* 20200202  [添加] 物品附加 bonus2 bAddSkillRange 增加 sk 技能 n 格攻击距离
* 20200202  [添加] 物品附加 bonus2 bSkillToAround 技能 sk 变为群体技能, 自动选择自身 n 格范围内的敌对目标进行攻击
* 20200131  [优化] 自定义技能公式 db/skill_formula_db.yml
* 20200131  [添加] 脚本指令 skf_src_status 获取攻击者的基础属性, 用于自定义技能公式
* 20200131  [添加] 脚本指令 skf_target_status 获取目标(被攻击者)的基础属性, 用于自定义技能公式
* 20200131  [添加] 脚本指令 skf_attr_fix 计算本次伤害的属性修正, 用于自定义技能公式
* 20200131  [添加] 脚本指令 skf_card_fix 计算本次伤害的 BONUS 修正, 用于自定义技能公式
* 20200131  [移除] 战斗选项 jro_formulas 日服公式分离已移至其他GIT库
* 20200131  [添加] 战斗配置 skill_critical_enable 哪些单位可以使用全局技能暴击
* 20200131  [添加] 战斗配置 skill_critical_rate 全局技能暴击概率
* 20200131  [添加] 战斗配置 skill_critical_damage 全局技能暴击伤害倍率
* 20200131  [添加] 战斗配置 skill_critical_boss 魔法暴击对 Boss级魔物的倍率
* 20200131  [添加] 战斗配置 skill_critical_luk 魔法暴击概率是否受对方 LUK 的影响, LUK抵消暴击率同普通暴击
* 20200131  [优化] 物品附加 bSkillCritical 参数 sk 支持 -1 对所有的技能可暴击
* 20200129  [优化] 支持装备 equip_uid 以整数型输出 覆盖指令 getinventorylist getcartinventorylist getcartinventorylist getinventoryinfo
* 20200129  [修复] 技能 太阳和月亮和星星的感觉 菜单取消的问题
* 20200129  [修复] 在PVP时 GM隐藏玩家的问题
* 20200129  [添加] 战斗配置 skill.conf devotion_standup_fix 是否修复玩家被攻击后仍然坐着的问题
* 20200129  [修复] 宝宝职业最大属性点不正确的问题
* 20200129  [修复] 技能 伪装炸弹 的攻击间隔问题
* 20200129  [添加] 战斗配置 homunc.conf hom_idle_no_share 当玩家处于闲置状态时不分享经验掉落的配置
* 20200129  [优化] 脚本指令 announce 支持 charid
* 20200129  [修复] 玩家多余的状态计算导致的某些状态不能正常使用的问题
* 20200129  [修复] 脚本指令 unitwalk 事件重复调用的问题
* 20200129  [添加] 脚本引擎支持 int64 位
* 20200129  [添加] 物品标记 item_flag.txt 添加专属标记 2 物品无视 autoloot 等自动拾取指令, 会直接掉到地上
* 20200129  [添加] 脚本指令 itemlist 生成一个物品选择框, 当玩家选择完成后触发 OnPCSelectItemEvent 事件
* 20200129  [添加] 脚本事件 OnPCSelectItemEvent 当玩家在物品选择框中选择物品列表中的物品后执行该事件
* 20200128  [添加] 战斗配置 item_costume_card 是否开启插卡变时装/影装功能
* 20200128  [添加] 物品标记 item_flag.txt 添加专属标记 1 插有该物品的普通装备变为时装/影装
* 20200128  [添加] 战斗选项 attack_event 当实体攻击时哪种实体触发 OnPCAttackFilter 事件
* 20200128  [添加] 战斗选项 attacked_event 当被实体攻击时哪种实体触发 OnPCAttackedFilter 事件
* 20200128  [添加] 战斗选项 mob_drop_event 当魔物掉落哪些物品时, 那种物品会触发 OnMOBDropItemFilter 事件
* 20200128  [修复] 脚本指令 bSkillAttribute 不生效的问题
* 20200128  [添加] 新增 db/statcost.txt 数据库 定义每次升级素质所消耗的素质点数
* 20200127  [添加] 脚本指令 instance_addmap 在副本中添加任意地图, 可以复制 NPC 到副本中
* 20200127  [添加] 脚本指令 instance_delmap 删除副本中已存在的地图
* 20200127  [修复] 脚本指令 equipinventory 和 unequipinventory 传入不存在的 idx 造成的宕机
* 20200124  [添加] 物品附加 bSkillNoSpecial 移除技能 sk 的特殊条件
* 20200124  [添加] 物品附加 bSkillNoRequire 移除技能 sk 的需求条件
* 20200124  [添加] 物品附加 bSkillAttribute 技能 sk 变为 ele 属性, sk 为  0 时为普通攻击
* 20200124  [添加] 物品附加 bReturnDefRate 受到反射伤害时, 伤害变为原来的 n%
* 20200118  [修复]  脚本指令 NPC复制指令中不能获取gid的错误, 且优化DOC说明
* 20200112  [修复] 修复宠物蛋作为邮件附件时的问题
* 20200112  [添加] 战斗配置 monster.conf mob_nopc_move_rate 当魔物附近没有玩家时 魔物随机移动的概率
* 20200112  [添加] 战斗配置 monster.conf boss_nopc_move_rate 当boss附近没有玩家时 boss随机移动的概率
* 20200112  [添加] 战斗配置 monster.conf mob_nopc_idleskill_rate 当魔物附近没有玩家时 魔物使用技能的概率
* 20200112  [添加] 战斗配置 monster.conf boss_nopc_idleskill_rate 当boss附近没有玩家时 boss使用技能的概率
* 20200112  [修复] 格林机关枪狂热 受体型影响的问题
* 20200112  [优化] 行为策略相关内容
* 20200112  [添加] 脚本指令 battle_check 判断两个GID之间是否是所提供的敌对/友好关系
* 20200112  [优化] 技能公式文件 db/skill_formula_db.yml
* 20200111  [修复] 调整 conf/battle_athena.conf 中 BetterRa.conf的加载顺序 解决 import失效的问题
* 20200110  [添加] 文件 conf/msg_conf/chat_msg.conf
* 20200110  [添加] 脚本指令 getposrandomoption 获取装备编号指定道具的 Random Option 属性
* 20200110  [添加] 脚本指令 setposrandomoption 设置装备编号指定道具的 Random Option 属性
* 20200110  [添加] 战斗配置 guild_storage_official 是否使用官方的公会仓库容量计算方法
* 20200110  [添加] 物品附加 bonus3 bSkillCritical 当玩家使用 sk 攻击时, 有 r/10% 的概率使伤害变为原来的 n% 倍
* 20200110  [添加] 物品附加 bonus2 bSkillToGround 技能 sk 变为地面技能, 并攻击 n 格范围内的敌对目标
* 20200107  [添加] 脚本指令 mapcutin 地图上的所有玩家都显示同一张照片
* 20200107  [添加] 脚本指令 end2 只结束脚本不影响NPC对话和商店
* 20200107  [修复] 脚本指令 getmobdmglog 不能获取MVP魔物伤害记录的问题
* 20200105  [添加] 技能公式修正, 通过 db/skill_formula_db.yml 可调整伤害公式、吟唱公式、延迟公式和状态持续公式
* 20200101  [添加] 脚本指令 addmobskill 给指定GID的魔物增加一个技能
* 20200101  [添加] 脚本指令 delmobskill 给指定GID的魔物删除技能
* 20200101  [优化] 玩家HP上限修改为21亿
* 20200101  [优化] 玩家SP上限修改为21亿
* 20191231  [修复] 物品附加 bRegenPercentHP bRegenPercentSP 不管用的问题
* 20191231  [修复] 脚本事件 OnPCRefineUIFilter 返回变量 @refine_blessing 数量不对的问题
* 20191231  [修复] 脚本事件 OnPCRefineUIFilter 打断时消耗材料的问题
* 20191230  [修复] chat-server临时更新, 修复一些BUG
* 20191215  [修复] 元素技能 黑洞 不能正常吸取玩家的问题
* 20191215  [修改] 数据文件 mob_avail txt转yml setunitdata 和 getunitdata 支持外观数据 
* 20191215  [修复] MySQL数据库结构优化, 老数据库依然可以用 
* 20191215  [修复] NPC技能 硬化防御 和 绝对防御 的技能问题
* 20191215  [修改] 将精炼上限提取到conf配置中
* 20191215  [修改] 数据文件 atcommand_athena conf转yml
* 20191215  [修改] 数据文件 abra_db txt转yml
* 20191215  [修改] 数据文件 improvise_db txt转yml
* 20191215  [修改] 数据文件 spellbook_db txt转yml
* 20191215  [修改] 数据文件 magicmushroom_db txt转yml
* 20191215  [添加] 战斗选项 monster.conf 中 mob_warp 支持魔物站在传送点会被传送的选项
* 20191215  [修复] 宠物在交易时孵化和丢失宠物后蛋还保留在背包的问题。
* 20191215  [优化] 物品附加 bVariableCast bSkillVariableCast 可在复兴前生效
* 20191215  [修复] 修罗点血技能可以叠加的问题
* 20191205  [修复] 由于实体越界造成的宕机
* 20191204  [优化] 脚本指令 unitskillfixed 加入普通攻击和倍率增减伤害和念咒
* 20191203  [修复] 1处严重级别注入漏洞, 2处低级别注入风险
* 20191129  [优化] 脚本指令 sprintf 结构改为 %[参数排序符][flags][width]specifier 添加更多说明符支持
* 20191129  [添加] 脚本指令 mesf 在NPC对话框打印一句话, 同 mes,具体使用方法参见 sprintf
* 20191129  [添加] 脚本指令 consolemes 在控制台打印文本, 第2个参数往后使用的是类似于sprintf的格式
* 20191129  [添加] 脚本指令 abs 返回数字绝对值
* 20191128  [添加] 脚本指令 setequipedcard 在已装备的道具中设置相应卡槽中的物品
* 20191128  [添加] 脚本指令 bonus_minfix 限制bonus的最小值
* 20191128  [添加] 脚本指令 bonus_maxfix 限制bonus的最大值
* 20191126  [优化] 脚本指令 getmobdmglog 获取伤害数据时对数据进行排序(可选), 并同时返回 cid 和 dmg 数据
* 20191126  [优化] getbonus 和 bonus_fixrate 的算法
* 20191126  [添加] 脚本指令 getbonus 和 fixrate 支持 bResEff bCastrate bFixedCastrate bDelayrate bSkillCooldown
* 20191122  [添加] 脚本指令 progressbar_unit 在GID对应的目标头上显示一个读条进度
* 20191122  [添加] 脚本指令 getvariableofpc 脚本返回指定玩家的变量
* 20191122  [添加] 脚本指令 getcalendartime 指令返回下次出现指定时间的时间戳
* 20191120  [优化] 脚本文件中数据库读入算法, 使其可以在reloaditemdb和reloadmobdb时重读
* 20191120  [添加] item_noequip在地图上的限制规则, 对护符物品生效
* 20191120  [修复] 正常关闭服务器产生的报错问题
* 20191120  [添加] 脚本文件中数据库读入算法 更新item_noequip 和 item_trade 加载的算法;
* 20191118  [优化] 脚本指令 atcommand 和 useatcmd 添加标签参数, 控制是否显示指令输出信息 (绿色字)
* 20191118  [修复] 使用指令 callshop 后玩家可以移动的问题
* 20191118  [添加] 脚本指令 getunitstatus 获取相应GID对应类型的战斗属性
* 20191117  [修复] 清除玩家 bonus 时, 结构体未对齐的问题
* 20191114  [优化] 脚本指令 getinventorylist 根据类型获取变量数组
* 20191114  [添加] 脚本指令 getcartinventorylist 返回手推车物品列表, 用法与 getinventorylist 一样
* 20191114  [添加] 脚本指令 getstorageinventorylist 返回手推车物品列表, 用法与 getinventorylist 一样
* 20191114  [添加] 脚本指令 searchinventory 过滤并查找背包中的物品
* 20191114  [添加] 脚本指令 searchcartinventory 过滤并查找手推车中的物品
* 20191114  [添加] 脚本指令 searstoragechinventory 过滤并查找仓库中的物品
* 20191114  [添加] 脚本指令 mobattached 脚本是否与魔物相关联
* 20191114  [添加] 脚本指令 setgroupid 赋予玩家新的权限等级
* 20191114  [删除] 战斗配置 enable_unittype_type 开启时会有新旧脚本混用的问题
* 20191112  [添加] 战斗配置 getmapxy_compatible 兼容 getmapxy 旧脚本中的数字类型
* 20191112  [添加] 脚本指令 getmapxy 兼容模块 兼容第四个参数为数字的情况。通过战斗配置 enable_unittype_type 开启（暂时关闭中待测试）
* 20191111  [优化] 地图mapflag noitemfunc 在原有基础上自由控制禁止与允许
* 20191111  [修复] 由于上次更新导致的套装属性在 item_noequip.txt 中禁止后依然生效的问题
* 20191111  [添加] 战斗配置 item_noequip_deny_all 当使用 item_noequip.txt 禁止装备类道具时, 其上的攻击力、防御力、卡片等内容全部失效
* 20191111  [优化] 战斗配置 multiple_script_state 多脚本堆栈默认打开状态
* 20191101  [添加] 服务端定制添加最大等级定制
* 20191027  [添加] 地图标记 noitemfunc 在玩家重计算属性时禁止加载一些物品的属性
* 20191027  [添加] 物品标记 item_flag.txt 添加 128 - 物品无视 mapflag 中 noitemfunc 对应的项目
* 20191027  [优化] 脚本指令 monster 和 areamonster 返回最后一个召唤魔物的GID
* 20191027  [添加] 状态 SC_ATK_ATTR_FIX 攻击属性伤害修正
* 20191027  [添加] 状态 SC_DEF_ATTR_FIX 防御属性伤害修正
* 20191027  [添加] 服务端在运行时显示 git hash 可以快速查找到宕机的源码
* 20191027  [修复] 战斗配置 pet_ignore_intimate_decrease 无效的问题
* 20191022  [添加] 脚本文件中数据库读入算法，可以在NPC文件中加载db数据 详情见：npc/BetterRa/BetterRa_data_demo.txt
* 20191022  [修复] 商店 pointshop 中重读宕机的问题
* 20191022  [添加] 脚本加密按照机码加密的算法, 可在此算法上增加到期时间
* 20191017  [添加] 2019年3月KRO对忍者 技能的平衡
* 20191017  [添加] 2019年4月KRO对修罗 技能的平衡
* 20191017  [修复] 太阳和月亮和星星的恶魔\骑兵修练\训龙术 对攻速的修正不正确的问题
* 20191017  [修复] GM指令 mobinfo 对VIP系统增加的概率不支持的问题
* 20191017  [添加] 成就系统 添加获得的经验值支持, 再次更新成就系统结构
* 20191017  [删除] 战斗配置 castle_defense_rate 该配置从未生效过
* 20191017  [优化] 脚本指令 questinfo 增加了条件判断, 此更新导致旧脚本quest图标全部失效
* 20191017  [修复] 成就系统导致的内存泄露的问题
* 20191017  [修复] 脚本指令 achievementexists 返回错误的问题
* 20191017  [修复] 当购买物品大于背包容纳的数量时, 导致的购买失败的问题
* 20191017  [修复] GM指令 dropall 总是显示0个物品的问题
* 20191014  [修复] 脚本时间 OnPCAttackedFilter 重复触发的问题
* 20191014  [修复] 脚本指令 unitskillfixed 使用后不能失效和在某些情况不起作用的问题
* 20191013  [修复] HTTP协议 HTTP头 解析错误的问题
* 20191013  [修复] HTTP协议 Cookies 解析的一些问题
* 20191013  [添加] HTTP读NPC文件实例, 位于 npc/script_BetterRa.conf
* 20191013  [优化] HTTP协议 增加cookie存储空间
* 20191013  [优化] HTTP功能演示脚本
* 20191013  [添加] GM指令 autotrade2 用于非挂店玩家离线挂店
* 20191013  [优化] 脚本指令 bot_stop 加入参数 <是否登出>
* 20191011  [修复] 物品bonus MultiDamageRate 伤害和概率叠加的问题
* 20191011  [修复] 物品bonus cooldownrate 与其他 bonus 生效顺序的问题
* 20191011  [添加] 战斗配置 questinfo_updatewarning 是否提示更新 questinfo脚本指令
* 20191010  [添加] 地图mapflag maxaspd 带有此标签的地图限制玩家最大攻击速度
* 20191010  [添加] char_athena.conf 添加 login_timeout 与map服连接超时时间
* 20191010  [添加] char_athena.conf 添加 login_check_interval 检查与login服连接是否失效间隔
* 20191010  [添加] map_athena.conf 添加 char_timeout 与char服连接超时时间
* 20191010  [添加] map_athena.conf 添加 char_check_interval 检查与char服连接是否失效间隔
* 20191010  [添加] NPC网络文件加载实例 npc/BetterRa.conf （待更新排队算法）
* 20191008  [优化] 优化 itemdb.txt 加载算法, 加载速度更快
* 20191008  [优化] 优化 mapcache.data 加载算法, 加载速度更快
* 20190925  [修复] 使用物品脚本为空的物品时崩溃的问题
* 20190923  [添加] skill_nocast_db 添加第三个参数, 不能使用区域对应的实体类型
* 20190919  [添加] 脚本指令 unitsleep 延迟一定时间 当GID在延迟过程中不存在时返回 0
* 20190919  [添加] 脚本指令 unitsleepend 延迟一定时间 当GID在延迟过程中不存在时脚本停止
* 20190919  [修复] 因不明原因调用空地图信息造成的宕机问题
* 20190917  [修复] mail_additem系列指令必须关联玩家的问题
* 20190914  [修复] 脚本堆栈被未知原因释放造成的宕机问题
* 20190913  [添加] 物品标签(item_flag.txt)类型64 当玩家处于不能使用物品的状态时依然可以使用带有此标签的物品
* 20190911  [添加] 脚本指令 99max_sleep2 等同于 sleep3
* 20190905  [修复] 官方BUG, 私人频道删除后的宕机问题
* 20190903  [修复] 脚本指令 sortarray 对照排序失败和宕机的问题
* 20190902  [添加] 脚本指令 unitdamage 使目标受到固定伤害值可显示伤害数字
* 20190902  [添加] 脚本指令 unitdamagerate 使目标受到MAXHP比例伤害值可显示伤害数字
* 20190901  [修复] 脚本指令 bonus_script_remove 清空后的宕机问题
* 20190830  [添加] 战斗配置 disable_mobdead_queue 所有的魔物绑定的被击杀事件都不排队
* 20190830  [修复] 脚本指令 setquesttime 在设置已有任务的时间时不能正常设置的问题
* 20190830  [添加] 脚本事件 OnPCBonusFilter 当玩家被赋予Bonus时触发
* 20190828  [添加] 脚本指令 pcisdead 检查玩家是否死亡
* 20190824  [修复] 地图标记 notoken 不管用的问题
* 20190820  [添加] 配置选项 cannonball_attr_alone 手推车加农炮是否独立计算炮弹属性
* 20190820  [优化] itemdb_search 报错提示报错源自哪个函数, 纠错用
* 20190820  [修复] RA官方邮箱漏洞导致的几处宕机问题
* 20190819  [添加] 配置选项 cannonball_attr_alone 手推车加农炮是否独立计算炮弹属性
* 20190816  [修复] 官方攻击力计算公式错误的问题
* 20190815  [优化] 移除 ragsrvinfo 功能与数据库
* 20190815  [优化] 数据库升级, 更新服务端的用户请注意！！！
* 20190814  [优化] 脚本指令 isequipped 最多限制在4个装备内的问题
* 20190814  [优化] 数据库升级, 更新服务端的用户请注意！！！
* 20190809  [修复] 修复宠物加成不与亲密度绑定 移除战斗配置 pet_equip_min_friendly
* 20190809  [修复] 当公告指令发送空公告时会发送失败
* 20190809  [修复] 在function中使用end导致 65K脚本副本同时运行的问题
* 20190809  [优化] 地图标记 mobspawnbase 在写错时的报错
* 20190809  [添加] 脚本指令 runitemscript 运行物品绑定的脚本
* 20190809  [修复] 龙息不接受属性修正的问题
* 20190802  [添加] 地图标记 noslave 地图上的魔物不会召唤小弟
* 20190802  [修复] 地图标记 nouseitem 在物品使用失败时没有提示的问题
* 20190801  [修改] log_athena.conf 配置中的树枝日志开启
* 20190801  [优化] log_athena.conf 配置中的Zeny日志开启
* 20190801  [添加] 脚本指令 setquesttime 设置任务到期时间
* 20190730  [优化] 邮箱最多只能显示30封, 邮箱已满后添加提示
* 20190730  [添加] GM指令 @change banchannel 频道 +时间 封禁频道（重启服务端后失效）
* 20190730  [添加] GM指令 @change unbanchannel 频道 解封频道
* 20190729  [优化] 脚本指令 unitskillwide 优化技能召唤方法, 解决技能放不完的问题
* 20190728  [修复] 复兴前治疗公式错误的问题
* 20190728  [修复] 所有卸除持续时间不正确的问题
* 20190728  [添加] 修复登录密码长度的问题
* 20190728  [添加] GM指令 @resurrect 使用复活之证复活
* 20190728  [修复] 成就系统分等级步骤的成就不能正常运行的问题
* 20190728  [添加] 战斗配置 hidenpc_willbe_disable 当使用hideonnpc时, 是否禁止玩家触发该NPC的范围触发
* 20190728  [修复] 脚本指令 duplicate 和 copynpc 指令宕机问题
* 20190728  [修复] 第二次尝试修复离开或进入不存在地图时的宕机问题
* 20190728  [优化] 优化装备套装属性无限时序的报错, 使其共容易定位错误
* 20190721  [添加] 脚本事件 OnPCAttackedFilter 当玩家被攻击且造成伤害时触发该事件
* 20190721  [添加] 地图标记 maxdmg_normal 限制此地图上单位的最大平砍伤害
* 20190721  [添加] 地图标记 maxdmg_skill 限制此地图上单位的最大技能伤害
* 20190721  [添加] 地图标记 maxheal 限制此地图上单位的最大治疗量
* 20190721  [添加] 地图标记 attackevent 当标地图标有此标记时玩家（攻击时）会触发
* 20190721  [添加] 地图标记 attackedevent 当标地图标有此标记时玩家（被攻击时）会触发
* 20190721  [添加] 地图标记 healevent 当标地图标有此标记时玩家使用治疗技能时会触发
* 20190721  [修复] NPC事件 OnDisplay 不与NPC关联的问题
* 20190718  [添加] 地图标记 maxdmg_mormal 限制地图上普通攻击伤害上限
* 20190718  [添加] 地图标记 maxdmg_skill 限制地图上技能攻击伤害上限
* 20190718  [添加] 地图标记 maxheal 限制地图上治愈量上限
* 20190716  [修复] 巧打技能概率和持续时间
* 20190716  [修复] 复兴前STR对魔物攻击力有加成的问题
* 20190716  [修复] 数据库升级, 更新服务端的用户请注意！！！
* 20190716  [修复] 当成就数据库 achievement.cpp 奖励数量超过1个时的报错问题
* 20190716  [修复] 已孵化的宠物蛋消失的问题
* 20190716  [添加] 地图标记 mobdmgrate 按百分比修正伤害
* 20190716  [添加] 地图标记 mobdmgfix 固定伤害值
* 20190716  [添加] 地图标记 mobdmgmax 限制最大伤害值
* 20190715  [添加] 脚本事件 OnAnyoneUseSkillEvent 当释放技能时触发
* 20190714  [优化] 脚本指令 script4each 优化指令架构, 替换掉类型 3 和 4 两个几乎用不到的类型
* 20190714  [添加] 脚本指令 script4eachmob 对指定范围的魔物执行相同的一段脚本
* 20190714  [添加] 脚本指令 script4eachnpc 对指定范围的NPC执行相同的一段脚本
* 20190714  [修复] 在未知地图上传送出来造成的宕机问题（原因暂且不明）
* 20190714  [修复] 脚本事件 OnPCAttackFilter 在使用一些技能时不能触发的问题
* 20190713  [修复] setunitdata 设置魔物HP上限时错误的问题
* 20190708  [修复] 登入到一个非法地图中造成的宕机问题（产生原因暂不明）
* 20190707  [优化] 脚本指令 unitshow 当 NPCOnDisplay 被打断时也能显示NPC
* 20190707  [优化] 脚本指令 unithide 加入特效编号以控制NPC消失特效
* 20190707  [优化] 生成dmp文件类型改为 MiniDumpNormal 生成的文件更小
* 20190706  [添加] 脚本指令 convertpcinfo 角色ID 账号ID 角色名之间互相转换
* 20190706  [修复] 使用克隆指令时导致的崩溃问题
* 20190706  [修复] 战斗配置 autospell_stacking 自动念咒堆叠开关不管用的问题
* 20190705  [添加] 脚本事件 OnPCBuffFixFilter 当玩家获得状态(Buff)时触发 可通过打断修改参数
* 20190704  [修复] 魔法子弹导致伤害溢出的问题
* 20190704  [修复] 脚本指令 setunitdata 操作魔物物攻魔攻无效的问题
* 20190703  [修复] 战斗配置 wedding_modifydisplay 不显示结婚礼服和燕尾服的情况
* 20190701  [添加] 脚本指令 getstorageinfo 通过IDX获取普通仓库物品信息
* 20190701  [添加] 脚本指令 getguildstorageinfo 通过IDX获取公会仓库物品信息
* 20190701  [添加] 脚本事件 OnPCInventoryAddFilter 当背包中放入物品时触发
* 20190701  [添加] 脚本事件 OnPCStorageAddFilter 当仓库放入物品时触发
* 20190701  [添加] 脚本事件 OnPCCartAddFilter 当手推车放入物品时触发
* 20190701  [优化] HTTP协议GET接口实现多线程 不用担心卡服的问题了
* 20190630  [添加] 脚本指令 instance_live_info 返回玩家激活的副本信息
* 20190630  [添加] 物品bonus bLongSPGainValue 与 bLongHPGainValue 远程攻击吸收HPSP
* 20190630  [添加] 160个JRO物品 
* 20190630  [修复] 怪物情报技能显示数据为负数的情况
* 20190630  [修复] PK模式下 GVG地图带有PVP的地图标志的问题
* 20190628  [添加] 反内挂源码支持(暂时不开放, 有需求请联系作者)
* 20190626  [添加] 脚本事件 OnPCAttackFilter 当玩家攻击时触发, 脚本中不消耗资源
* 20190626  [添加] 脚本指令 unitatkeventmark 用于控制 OnPCAttackFilter 事件触发
* 20190626  [添加] 战斗配置 attack_filter_target 用于控制 OnPCAttackFilter 事件触发
* 20190623  [修复] 使用新版防外挂后登陆不上的问题
* 20190623  [优化] 魔物信息显示的读入算法, 使其更快
* 20190622  [修复] manner过滤机制遇到类型2以后如果未打断依然会被过滤并弹出提示的问题
* 20190622  [添加] 战斗配置 multiple_script_state 是否开启多层的脚本堆栈
* 20190619  [修复] 在非法地图下线造成的宕机问题（产生原因暂不明）
* 20190617  [修复] 龙息受体型种族属性等卡片修正的问题
* 20190617  [修复] 重读魔物数据会宕机的问题
* 20190617  [修复] 在吟唱技能时不能使用消耗品的问题
* 20190617  [修复] 在商城坐骑下不能使用消耗品的问题
* 20190617  [修复] 脚本指令 getiteminfo 在提供参数大于16时不能正确获取数据的问题
* 20190617  [修复] 魔物使用左手武器技能宕机的问题
* 20190613  [修复] reloadnpcfile 和 reloadscript 不能重读 function 的问题
* 20190613  [添加] 安全区支持 CELL_SAFEZONE 可以通过 setcell 将地图的某个区域设置为安全区
* 20190613  [添加] 安全区支持 CELL_CHKSAFEZONE 可以通过 checkcell 检查是否是安全区
* 20190613  [添加] 脚本指令 countrentitem countrentitem2 countrentitem3 获取租赁道具的数量
* 20190610  [修复] 写邮件时发送的物品数量错误的问题
* 20190608  [修复] 被隐藏的NPC (通过脚本指令hideonnpc隐藏) 取消范围触发
* 20190608  [优化] GM指令 jumpto follow 指令支持 AID 和 CID
* 20190607  [添加] 脚本指令 isloggedin2 通过角色ID检查玩家在线并获取账号ID 不在线返回 0
* 20190606  [修复] 技能暴击不能正常使用的问题
* 20190606  [修复] itemdb中武器类型写错会报错，并修复其导致的宕机问题
* 20190606  [修复] 脚本事件 OnKillMvpEvent 返回的killedrid killerrid killedgid不正确的问题
* 20190606  [修复] http GET协议等待时间过长的问题（之后http GET协议将会改为多线程）
* 20190606  [修复] 地图标记 restricted 参数大于8时 不能正确使用的问题
* 20190606  [修复] 武器锻造检查铁砧时不检查背包中第一个物品的问题
* 20190606  [修复] 当物品不存在时使用快速换装会宕机的问题
* 20190606  [优化] 脚本指令 viewpoint 支持给对应charid玩家标注记号
* 20190604  [修复] 脚本指令 getunits 和 getareagid 坐标写反的时候不能获取正确的位置空间的问题
* 20190604  [修复] 只要是玩家的groupid 大于 gm_allow_group 都可以在超出服务器人数上限的时候登录。
* 20190604  [修复] 第二次修复玩家超重时，拿出手推车中的物品会丢失的问题
* 20190604  [修复] 一些bonus被限制在-10000~10000之间的问题
* 20190603  [修复] 地图标记 mobspawnbase 对特定魔物不生效的问题
* 20190603  [修复] 副本群体传送失效的问题(必须开启战斗配置 instance_activation)
* 20190603  [添加] 脚本指令 getsameuidinfo 根据UID筛选玩家，类似于 getsameipinfo 具体参见doc
* 20190603  [添加] 脚本指令 getcomputeruid 获取玩家电脑的机器码
* 20190523  [优化] 脚本事件 OnPCTradeAddFilter 返回交易对方的cid、aid @trade_partner_aid @trade_partner_cid
* 20190523  [添加] 战斗配置 homunculus_share_exp 玩家击杀魔物时生命体可获得部分经验
* 20190523  [添加] 在新版的客户端中支持使用邮箱删除角色
* 20190521  [添加] 在使用多线系统时，玩家被分发了非默认IP时在相应窗口广播分发IP
* 20190521  [添加] 在使用配套BetterRA.dll的时候可以实现真正的多线连接而不用配置繁杂的配置
* 20190521  [修复] 修复BetterRA.dll可用版，修复了所有掉线及连不上的问题，客户端必须开启MD5封包
* 20190520  [修复] 宠物重复加载时造成的内存泄露问题
* 20190520  [添加] 脚本事件 OnPCTradeAddFilter 当把物品(zeny)添加到交易菜单时触发
* 20190520  [添加] 脚本事件 OnPCMailAddFilter 当把物品(zeny)添加到邮件附件时触发
* 20190520  [添加] 脚本事件 OnPCDropItemFilter 当玩家扔掉物品时触发
* 20190519  [添加] 多线路支持, 此版本仍不完美
* 20190519  [修复] 宠物蛋出现在铁匠武器修理菜单里面的问题
* 20190519  [修复] 死亡时不能赋予 SC_BLOCKING_ALIVE 状态的问题
* 20190516  [修复] 脚本事件 OnPCRefineUIEvent 的宕机问题 添加返回值 @refine_blessing @refine_refine
* 20190514  [优化] 事件 OnMOBDropItemFilter 添加掉落概率 @drop_rate、@drop_ratefix
* 20190514  [添加] 脚本事件 OnPCRefineEvent 返回精炼成功变量 @refine_success
* 20190512  [优化] 露天商店拓展当物品中有重复的货币时不能使用@at指令挂店, 并控制台报错
* 20190512  [优化] 露天商店拓展当物品中有重复的货币时, 离线商店重登报错 Error 返回 6 而不是 5
* 20190512  [修复] 脚本指令 killaroundmonster 对无敌的魔物(UMOB_DMGIMMUNE)、华丽水晶和守护石无效
* 20190512  [添加] 状态 SC_BLOCKING_ALIVE 状态期间不能使用不死之证或被技能复活
* 20190512  [添加] 状态 SC_BLOCKING_WEAPON 状态期间不能穿戴或脱下武器
* 20190512  [添加] 状态 SC_BLOCKING_ARMOR 状态期间不能穿戴或脱下防具
* 20190512  [添加] 状态 SC_BLOCKING_SHADOWGEAR 状态期间不能穿戴或脱下影子装备
* 20190512  [添加] 状态 SC_IGNORE_RETURN 状态期间无视攻击时造成的反射伤害
* 20190512  [修复] 当物品脚本为空时验证脚本内容造成的宕机问题
* 20190510  [修复] bonus_ratefix 逻辑问题, 在conf配置 bonus_ratefix 后, 可以通过脚本指令对特定的bonus进一步修正
* 20190508  [添加] 战斗配置 remove_noinfo_pet 当尝试孵化坏蛋时移除坏蛋
* 20190508  [修复] 脚本事件 OnMOBDropItemFilter 拦截掉落后依然会公告的问题
* 20190508  [修复] 在一些错位的情况下造成的释放技能失败的问题
* 20190507  [修复] 在无视防御（元灵武士卡）情况下扔计算防御和渗透效果的问题
* 20190507  [修复] 战斗选项 instance_activation 允许在没有副本时候使用变量返回空字符串或0
* 20190507  [修复] 在RA更新副本结构后，没有激活副本且在副本外使用副本变量造成的宕机问题
* 20190507  [修复] 脚本指令 script4each 在RA更新副本结构后，使用会造成的宕机或报错问题
* 20190505  [添加] 战斗配置 homunculus_killed_drop 生命体击杀魔物时, 是否掉落物品
* 20190505  [添加] item_flag.txt 添加掩码 1024 魔物单纯被生命体杀死不会掉落此物品
* 20190505  [添加] item_flag.txt 添加掩码 2048 魔物被玩家击杀时, 会触发 OnMOBDropItemFilter 事件
* 20190505  [添加] 脚本事件 OnMOBDropItemFilter 当魔物掉落在 item_flag.txt 中标记有2048的物品时触发
* 20190505  [修复] 系统删除过期的 email 时会触发 map-server.exe 报错的问题
* 20190505  [修复] 物品bonus3 bSkillDamage 参数溢出导致伤害修正错误的问题
* 20190505  [修复] 官方在修复手推车刷物品的BUG出现的不检查超重的问题
* 20190504  [添加] 战斗配置 support_old_mob_db 支持旧版魔物数据库的物理攻击力和魔法攻击力
* 20190503  [修复] RA官方修复手推车刷物品的BUG
* 20190503  [修复] 封包加密系统在网络延迟高的情况下造成的不能登陆的问题
* 20190503  [修复] 在使用BetterRA.dll获取玩家机器唯一码的同时也在login表中保存最后登陆IP信息
* 20190503  [修复] 在即时触发类的脚本(装备脚本、script4each等等)中使用end指令不会影响正在运行的脚本
* 20190503  [修复] 脚本事件 OnPCStatCalcEvent 没有在所有bonus计算完成后再触发的问题
* 20190503  [修复] 十字斩首者武器格挡技能下线重登后图标计时不准确问题
* 20190503  [修复] 铁匠机甲状态下过热造成的状态重复重置的问题, 修复过热不掉血的问题
* 20190427  [添加] 添加 38个 hat_effect常量并添加到对应的物品中
* 20190427  [添加] 战斗配置 instance_activation 使用旧版副本脚本指令结构和副本变量结构
* 20190426  [优化] 脚本指令 unitwarp 可以将NPC移动到其他地图, 而不只是限于在相同的地图移动
* 20190426  [添加] 脚本指令 getinventoryinfo 传入参数 16 获取该物品是否被玩家收藏
* 20190426  [添加] 脚本指令 getinventorylist 返回数组 @inventorylist_favorite 收藏数组
* 20190426  [修复] 修复了宠物在亲密度小于0时宠物逃跑而蛋会变坏依然留在背包中的问题
* 20190425  [修复] 修复了DEX对鞭子和乐器类武器不增加攻击力的问题
* 20190425  [修复] 修复当使用物品给武器附加属性时根据属性显示图标
* 20190425  [修复] 修复了成就系统在杀死魔物时崩溃的问题(之前我修过，现在恢复到官方版本)
* 20190425  [修复] 修复了 2014-10-08之前的客户端不能使用自动喂食的问题
* 20190425  [添加] 战斗配置 pet_walk_speed 配置宠物行走速度与什么有关, 修复宠物行走过慢的问题
* 20190425  [修复] 为物品 bonus 参数添加上限，使其参数限制在-10000~10000之内
* 20190425  [修复] SQL脚本 upgrade_20190309.sql 在升级PET数据库后宠物不能回复成蛋的问题
* 20190423  [优化] 将黑窗中提示管理员权限和root权限的提示从警告级别更改为信息级别
* 20190423  [修复] 当商店拓展的货币数量小于2时，不能正常开启商店的问题
* 20190423  [修复] 当物品的item_flag.txt中的道具被设置为256时, 在吟唱技能时不能使用的问题
* 20190423  [优化] 优化 conf\msg_conf\translation.conf 读入算法和文件格式(现在可自行在这个文件中翻译黑窗文字)
* 20190423  [修复] 路点系统 unitsteppath 在执行时没有释放掉私有路径造成的内存泄露问题
* 20190423  [优化] 路点系统 行走完成触发事件时 返回 $@steppath_gid 和 $@steppath_fail 两个变量
* 20190422  [修复] 由于魔物做表情时路点系统认为他处于空闲状态而让他走路导致的报错问题
* 20190422  [修复] 脚本指令 unit_Astart_walkto 在4个参数为空时不能自动释放内存的问题
* 20190422  [修复] 路点行走完成后还试图再次移动到下一个路点坐标的问题
* 20190422  [添加] GM指令 battleignore 无视战斗伤害
* 20190421  [添加] 战斗配置 guild_storage_per_lvup 每一级'公会仓库扩充' 增加的公会仓库容量
* 20190414  [修复] unitskilltowide 指令的宕机问题
* 20190414  [修复] mobstaypos 范围参数不管用的问题
* 20190413  [优化] 脚本指令 unit_Astart_walkto <uid>,<x坐标>,<y坐标>{,<自动释放>{,<寻路AI>,{<参数>,{<"事件名称">}}}};
* 20190413  [修复] 自动寻路AI逻辑 被打僵直后找不到路的BUG
* 20190413  [添加] 脚本指令 unitspecialeffect 指令给指定GID的单位一个effect效果
* 20190410  [添加] 脚本指令 killaroundmonster 根据mode类型杀死范围内的魔物
* 20190409  [修复] bot_login后通过登录事件不能通过is_bot获取是否为机器人的问题
* 20190409  [修复] 战斗配置 skill_chorus_check 和 skill_partner_check 的逻辑错误
* 20190409  [修复] 玩家不能向GM交易不可交易的物品的问题
* 20190409  [修复] NPC事件 OnDisplay 不能正常打断的问题
* 20190409  [修复] 读取pet_db.yml宕机的问题
* 20190409  [优化] 脚本指令 instance_users 传入0 不再报错
* 20190406  [修复] 脚本指令 battleignore
* 20190406  [添加] 战斗配置 skill_chorus_check 当玩家使用合奏类技能时，是否不检查队员满足条件
* 20190406  [添加] 战斗配置 skill_partner_check 当玩家使用合作类（除合奏类技能之外）技能时，是否不检查队员满足条件
* 20190406  [修复] processhalt在多个事件触发时不能正常打断的问题
* 20190405  [修复] Ontouch不能同时出发多个脚本的问题
* 20190405  [修复] 宠物饱食度下降过快的问题
* 20190405  [修复] 更正火墙术检查机制
* 20190405  [修复] 成就系统的一些问题(需更新achievement_db.yml)
* 20190403  [添加] 战斗配置 enable_unittype_type getmapxy和getunittype 使用旧版参数
* 20190403  [添加] step_path.txt 重读指令，@reloadpathdb
* 20190403  [添加] 脚本指令 unitshow 临时显示某个单位，与unithide对应
* 20190402  [修复] item_combo_db 在特殊情况下读入时的宕机BUG
* 20190402  [修复] 登陆数据库中没有的账号时，char-server不报错的问题(针对bot_login脚本指令)
* 20190402  [修复] 尝试修复封包加密算法导致的掉线问题(需更新客户端BetterRA.dll)
* 20190331  [优化] 路点算法
* 20190331  [添加] 采购商店扩展货币支持，目前只支持cash点数采购
* 20190331  [添加] 战斗配置 extended_buying 采购商店扩展开关
* 20190331  [修复] 物品扩展 bSPVanishRate 可以针对攻击类型恢复SP
* 20190331  [添加] 宠物进化支持，宠物数据库变更为yml格式
* 20190331  [添加] 战斗配置 atcommand_disable_npc 当玩家正在与NPC对话时是否可以使用GM指令
* 20190331  [添加] IRO乐园团任务(详见npc/re/quests/eden/eden_iro.txt)
* 20190331  [添加] 脚本指令 getpcblock 获取玩家被禁止的操作(详见script_commands.txt)
* 20190331  [添加] 脚本指令 setpcblock 禁止玩家使用一些操作(详见script_commands.txt)
* 20190331  [修复] 岩浆喷发的伤害问题(需更新skill_cast_db.txt)
* 20190331  [修复] 反叛技能的施法时间和持续时间(需更新skill_cast_db.txt)
* 20190331  [修复] 脚本指令 setunitdata 生命体、元素MaxSP调整问题
* 20190331  [修复] 脚本指令 setunitdata 魔物、生命体、佣兵、元素体型种族调整问题
* 20190331  [修复] 脚本指令 setunitdata 魔物、生命体、佣兵、元素、NPC的HP和MaxHP调整问题
* 20190331  [修复] 脚本指令 setunitdata 没有正确计算魔物MODE问题
* 20190331  [修复] bonus3 bSubEle 和 bonus3 bAddEle 互相覆盖的问题
* 20190331  [修复] 杜兰族攻速错误的问题 (需更新job_db1.txt)
* 20190331  [添加] csv转yml工具(csv2yaml.exe)
* 20190320  [优化] 优化yaml读取库，yml文件格式错误时添加报错
* 20190317  [修复] GVG攻速在特定的GVG地图上不能正常运行的问题
* 20190317  [添加] IRO攻速公式
* 20190316  [修复] 在某些脚本中刷怪卡住的问题
* 20190316  [优化] 事件触发检查
* 20190313  [添加] 脚本事件 OnPCMoveFilter 当玩家移动时触发
* 20190313  [添加] 脚本指令 when_pcmove_doevent 控制玩家移动时触发事件
* 20190313  [添加] 战斗配置 player_move_event_type 控制玩家移动事件的触发类型
* 20190313  [修复] 全图A星算法找不到重点的位置
* 20190311  [修复] bonus_ratefix 战斗配置不管用的问题
* 20190311  [修复] 路点系统的若干BUG
* 20190310  [添加] 战斗配置 official_cro 使用CRO技能公式
* 20190310  [添加] CRO技能公式支持诗人舞娘的夕阳之歌和远古呐喊
* 20190303  [修复] 脚本指令 instance_record 的报错问题和返回值的问题
* 20190303  [修复] 状态抗性大于100%时反而不能免疫状态的问题
* 20190303  [添加] 战斗配置 autobonus_in_order 是否防止重复触发相同的 autobonus
* 20190223  [修复] 增加UTF8编码检测的兼容性, 修复\n在windows环境读入出错问题
* 20190223  [添加] 网页定制添加容错模式(建议测试阶段不要开启此项)
* 20190223  [修复] 元素漩涡和武僧一些技能有可能造成的宕机问题
* 20190222  [优化] 地图标记 移除nopcsc和nomobsc添加nosc在地图上不能使用技能
* 20190222  [添加] 地图标记 noskill2 在地图上不能使用技能
* 20190222  [添加] 地图标记 nouseitem 在地图上不能使用物品
* 20190221  [修复] 脚本指令 progressbar_npc 未关联玩家时的报错问题
* 20190220  [添加] 战斗配置 disable_event_queue 禁用事件队列机制
* 20190220  [修复] 冰墙的表现形式为韩服官方的表现形式 (需更新 skill_cast_db.txt)
* 20190220  [修复] 跆拳道跑步固定施法时间的问题 (需更新 skill_cast_db.txt)
* 20190220  [修复] 修复NPC进度条报错 npc_scriptcont: unexpected state '2' for continue call 的问题
* 20190219  [添加] 战斗配置 sleep2_ignore_attach 当sleep2未关联玩家时, 是否执行和sleep同样的操作
* 20190219  [添加] 战斗配置 multiple_autobonus 同装备是否可以触发多个autobonus
* 20190219  [修复] item_trade.txt 交易限制中将不再检查获得物品玩家的group等级
* 20190218  [添加] 地图标记 nopcsc 玩家不允许在该地图上获得sc, 传入地图的玩家所有的sc将被清空
* 20190218  [添加] 地图标记 nomobsc 魔物不允许在该地图上获得sc
* 20190216  [添加] 战斗配置 enable_manner 是否开启敏感词过滤功能
* 20190216  [添加] 敏感词数据库 db/manner.txt 
* 20190216  [添加] 脚本事件 OnPCMannerFilter 检测到玩家说敏感词时触发该事件, 可用于其他功能扩展
* 20190216  [修复] 脚本指令 mail_additembound 相关指令不能正常使用的问题 (感谢 此方此方此方方)
* 20190216  [修复] 去除角色创建限制的相关源码 (感谢 尐駒頭寶寶)
* 20190214  [添加] 脚本指令 showvending 使 NPC 显示露天商店的招牌
* 20190214  [添加] 脚本指令 showbuying 使 NPC 显示采购商店的招牌
* 20190214  [添加] 脚本指令 sendmail 发送邮件和只带有物品ID和数量的邮件
* 20190214  [添加] 脚本指令 sendmail2 在sendmail基础上支持精炼、卡片等
* 20190214  [添加] 脚本指令 sendmail3 在sendmail2基础上支持(随机属性)
* 20190213  [修改] 战斗配置 get_identify_equip 增加 &64: 使用 getrandgroupitem 获得的装备
* 20190213  [添加] 战斗配置 disable_items_callfunc NPC对话时禁止使用callfunc道具
* 20190213  [添加] 战斗配置 disable_items_equip NPC对话时禁止穿戴脱下装备
* 20190213  [添加] 战斗配置 disable_skill_delay 是否禁用技能公共延迟
* 20190207  [修复] 脚本指令 getequipexpiretick 当传入参数不是装备位置时会宕机的问题 (感谢 此方此方此方方)
* 20190207  [修复] 新版成就系统宕机BUG
* 20190207  [修复] script4each在三层堆栈时的宕机BUG (感谢 此方此方此方方)
* 20190207  [添加] 支持UTF8 和 UTF8 BOM 编码文件，需要在网站定制
* 20190205  [添加] 技能数据库 db/re(pre)/skill_castnodex_db.txt 添加掩码选项2048 取消技能后摇
* 20190205  [修复] 魔物使用元素旋涡没有伤害的问题 (感谢 此方此方此方方)
* 20190205  [优化] 脚本指令 getmapxy 类型弃用UNITTYPE_改为BL_参数，不影响之前脚本使用
* 20190205  [优化] 脚本指令 getunittype 返回类型弃用UNITTYPE_改为BL_参数
* 20190205  [修复] 技能在计算素质部分伤害时 不附加武器属性修正，这意味着伤害又被削了
* 20190205  [修复] 切换地图服务器时，由于切换失败造成的不能正常登陆到char-sever服务器的问题
* 20190205  [修复] 脚本指令 发送邮件指令可能造成的宕机问题
* 20190205  [修复] 修复了一处非常严重的源码级别的BUG，由于此BUG的修复导致该BUG被完全暴漏，可被外挂利用。
* 20190205  [修复] 脚本指令 changelook 检查合理范围
* 20190205  [修复] 修复技能 物品投掷 检查sc移入源码 请更新itemdb中13275~13280
* 20190203  [修复] 设定怪物路点以后玩家不在怪物视野范围内不能移动的问题
* 20190203  [修复] chat-server 接口造成的封包加密掉线问题
* 20190203  [修复] 更新 re/trade.txt 数据库，此数据库完全遵照客户端itemmoveinfov5
* 20190203  [修复] 脚本指令 awake 的一处宕机BUG
* 20190203  [修复] 在服务端正常结束释放资源时候的宕机BUG
* 20190203  [修复] 技能 毒液制作 在制作条件不满足时也能被制作的问题
* 20190203  [修复] 流氓天堂和跆拳道休息状态不管用的问题
* 20190203  [修复] GM指令 @points 不能正确移除卡普拉点数的问题
* 20190203  [修复] 物品套装属性 item_combo_db 扩展到附魔装备，附魔也可以写入套装了
* 20190203  [修复] 重构成就系统
* 20190129  [修复] 地图mapflag nightmaredrop在执行 @reloadscript 时不能重置的问题
* 20190127  [添加] 脚本指令 OnPCCashShopBuyEvent 商城购买结束后触发的事件
* 20190127  [修复] 地图mapflag mobspawnbase在副本中归零的问题
* 20190127  [修复] 地图mapflag mobspawnbase血量不能突破21亿的问题
* 20190127  [添加] get接口简单支持 cookie 操作
* 20190127  [修复] 在脚本事件(Filter事件)中 不能使用close、close2、select、menu、next等指令的问题
* 20190127  [修复] mapcache.exe 不能正确读取 conf/grf-files.txt 的问题
* 20190127  [添加] 授权系统
* 20190122  [修复] 使用商店拓展后，部分露天商店离线挂店后重启服务端后不能重读的问题
* 20190121  [添加] 客户端 BetterRA.dll 使用进程锁 防止封包没有加密就发出的问题
* 20190121  [添加] 封包加密支持官方三组KEY秘钥生成封包头加密机制（可防WPE）建议一起打开
* 20190121  [修复] 在启用封包加密时login-server对封包裁剪不准确导致MD5发送失败的问题 (感谢 此方此方此方方)
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
* 20180506  [提交] 帐号类型跨区登录及交易补丁.patch 暂不启用
* 20180506  [增加] 事件 OnBOTLackItemEvent 当BOT的消耗品少于5个时触发的事件, 返回 @lack_itemid 物品ID
* 20180506  [增加] BOT技能支持, 详情请查看 npc\BetterRa\BetterRa_bot.txt
* 20180506  [修复] 登录时不能顶下BOT账号的BUG
* 20180506  [增加] 事件 OnBOTLackItemEvent 当BOT的消耗品少于5个时触发的事件, 返回 @lack_itemid 物品ID
* 20180506  [增加] BOT技能支持, 详情请查看 npc\BetterRa\BetterRa_bot.txt
* 20180506  [修复] 登录时不能顶下BOT账号的BUG
* 20180506  [修复] getsameipinfo指令 可以返回IP数量
* 20180307  [合并] 合并分支 rathena
* 20180306  [修复] 鉴定事件错误
* 20180306  [修复] 脚本指令 delinventory 不能使用第2个参数的问题
* 20180127  [合并] 合并分支 rathena
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
* 20180102  [添加] 战斗配置 enable_coply 当其他职业拥有流氓的抄袭技能时, 是否可以复制技能
* 20180102  [修复] 当背包大于128时, 商城不能购买的问题.
* 20180102  [添加] 脚本指令 getidxrandomoption 用来获取玩家背包中物品的OPT
* 20180102  [添加] 脚本指令 setidxrandomoption 用来设置玩家背包中物品的OPT
* 20180102  [添加] 属性拓展 bWeaponAddDamage 直接增加每次武器攻击的伤害
* 20180102  [添加] 属性拓展 bMagicAddDamage 直接增加每次魔法攻击的伤害
* 20180102  [添加] 属性拓展 bMiscAddDamage 直接增加每次特殊攻击的伤害
* 20180102  [添加] 脚本指令 is_bot 用来判断指定玩家是否为机器人
* 20180102  [添加] 脚本指令 bot_run 启动机器人, 配置方法请查看 doc/BetterRa_script_commands.txt
* 20180102  [添加] 脚本指令 bot_reset 用来重新配置机器人参数, 配置方法请查看 doc/BetterRa_script_commands.txt
* 20171228  [添加] 护身符道具 类型为20, 需要在 conf/battle/BetterRa.conf 启用护符
* 20171228  [添加] 机器人系统
* 20171222  [修改] 提取 packet_keys 配置服务端封包混淆KEY的设置提取到 conf/map_athena.conf
* 20171222  [添加] 地图标记 nopet 禁止玩家在此地图上使用宠物, 登陆到此地图宠物会自动变蛋回到玩家背包.
* 20171222  [添加] 地图标记 nohomunculus 禁止玩家在此地图上使用生命体, 登陆到此地图生命体会被自动安息.
* 20171222  [添加] 地图标记 nomercenary 禁止玩家在此地图上使用佣兵, 登陆到此地图佣兵会被自动解散.
* 20171222  [修改] 宠物脚本 pet_skill_attack 的伤害太小的问题
* 20171222  [修改] 指令名称 hasfollow -> getservantgid 用来获取玩家有没有 宠物、佣兵、生命体、元素体等
* 20171222  [添加] 脚本指令 tribe_mob_spawn 用于召唤指定阵营下的魔物
* 20171214  [添加] 战斗配置 buff_for_guild_member 在GVG模式下 群体辅助技能 是否对所有公会成员有效
* 20171214  [添加] 战斗配置 buff_for_tribe_member 在阵营模式下 群体辅助技能是否对同阵营成员（玩家、魔物）有效
* 20171214  [添加] 脚本指令 tribe_getteam 获取玩家的阵营ID
* 20171214  [添加] 脚本指令 tribe_setteam 将玩家添加至某个阵营
* 20171214  [添加] 脚本指令 tribe_mobteam 将上次召唤的魔物全部添加至某个阵营
* 20171212  [添加] 路点系统 使用 setsteppath 为魔物设置路点
* 20171212  [添加] 阵营系统 在 mapflag 被设为 tribe 时生效
* 20171209  [添加] 脚本指令 setsteppath 用于设置MOB的行走路径, 配置以后魔物会自动行走
* 20171209  [添加] policies.conf 文件
* 20171209  [添加] 脚本配置文档 npc/script_BetterRa.conf
* 20171209  [添加] demo文件 npc/BetterRa/BetterRa_event_demo.txt
* 20171209  [添加] GM指令 policy 用于设置指定玩家的行为策略编号
* 20171209  [添加] GM指令 title 用于更改称号
* 20171209  [添加] policies 行为策略，用于动态修改 经验 掉率 GM等级等
* 20171209  [添加] setpolicyid 设置指定玩家的行为策略编号, 配合 conf/policies.conf 中定义的规则使用
* 20171209  [添加] getpolicyid 获取指定玩家的行为策略编号, 配合 conf/policies.conf 中定义的规则使用
* 20171209  [添加] getpolicyinfo 获得指定玩家当前的行为策略详情, 配合 conf/policies.conf 中定义的规则使用
* 20171208  [添加] 脚本指令 mail_create mail_additem* mail_send 发送邮件支持
* 20171208  [添加] BD配置文档 item_announce.txt 根据物品掉率来判定是否需要进行公告
* 20171208  [修复] 登陆时, 显示"不知名区域"的BUG
* 20171207  [修复] getitem3 delitem3 等指令不能使用玩家关联的变量的问题
* 20171207  [添加] GM指令 recallmap 将地图上的玩家瞬移至身旁
* 20171207  [添加] 脚本指令 hasfollower 用来获取玩家有没有 宠物、佣兵、生命体、元素体等
* 20171206  [添加] 脚本指令 unitremove 可以通过GID移除实体, 并播放指定的消失动画
* 20171205  [修复] 事件队列问题带来的一系列问题
* 20171205  [修复] callshop 后玩家可以随意移动的问题
* 20171205  [添加] 战斗配置 combo_mode 用于配置 item_combo 效果生效策略
* 20171204  [汉化] 生命体DB中的名称
* 20171204  [添加] 地图标记 hideguildinfo 隐藏公会信息
* 20171204  [添加] 地图标记 hidepartyinfo 隐藏组队信息
* 20171204  [添加] 地图标记 notoken 不能使用复活之证
* 20171204  [添加] 地图标记 nocapture 不能捉宠物
* 20171204  [添加] 地图标记 noautoloot 不能使用自动拾取
* 20171204  [添加] BetterRa 战斗配置文件 conf/battle/BetterRa.conf
* 20171204  [添加] 战斗配置 max_pvp_aspd 玩家在PVP地图时的最大攻速限制
* 20171204  [添加] 战斗配置 max_gvg_aspd 玩家在GVG地图时的最大攻速限制
* 20171204  [添加] 战斗配置 extremityfist_castend_ball_check 阿修罗霸凰拳咏唱完毕时, 是否再次确认气弹数量
* 20171204  [添加] 战斗配置 extremityfist_castend_status_check 阿修罗霸凰拳咏唱完毕时, 是否再次确认“爆气”状态
* 20171204  [添加] 战斗配置 extremityfist_nocast_sp 使用“阿修罗霸凰拳”不会直接耗尽玩家的SP
* 20171204  [添加] 战斗配置 loadevent_for_eachmap 强制所有地图触发 OnPCLoadMapEvent 事件
* 20171204  [添加] 战斗配置 get_identify_equip 通过特定渠道获得的装备, 强制变成已鉴定
* 20171204  [添加] 战斗配置 mounting_cant_use 当玩家乘坐商城坐骑后, 是否禁止使用物品
* 20171204  [添加] 战斗配置 hide_cmd 当没有权限的玩家执行了GM指令是否隐藏并返回一条信息
* 20171204  [添加] 战斗配置 bAspd_fixed 使用 bonus bAspd,x; 进行攻速加成时, x 值是否会受到其他加成因素影响
* 20171204  [添加] 战斗配置 share_quests_area 在什么位置才能共享任务的杀怪进度
* 20171204  [修改] 战斗配置 show_mob_info 添加 级别/编号/体型/种族/属性和属性等级
* 20171204  [添加] 脚本指令 identifybyidx 根据指定的背包序号, 对该道具进行鉴定操作
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
* 20171130  [汉化] db数据库文件中文化
* 20171129  [汉化] conf配置文件中文化
* 20171129  [添加] 用于配置新加入的控制台输出文本 conf/msg_conf/BetterRa_map_msg.conf
* 20171128  [添加] 控制台翻译代码, 控制台翻译文件 conf\msg_conf\translation.conf
