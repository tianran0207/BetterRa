
#define SERVER_DIFF 0

//版本20150916、复兴前，金币上限改21E、自动拾取上限100条
#if SERVER_DIFF == 1

#undef C_PACKETVER
#define C_PACKETVER 20150916

#undef C_MAX_ZENY
#define C_MAX_ZENY INT_MAX

#undef C_AUTOLOOTITEM_SIZE
#define C_AUTOLOOTITEM_SIZE 100

#elif SERVER_DIFF == 2
#undef C_PACKETVER
#define C_PACKETVER 20180620

#else
#define BEATVER
#endif