#ifndef _IDBCMANAGER_H_
#define _IDBCMANAGER_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

typedef unsigned int        uint;
typedef unsigned int		DBCHandle;
typedef unsigned char       uint8;
typedef unsigned short       uint16;

#define INVALID_DBC_HANDLE -1 // 无效的DBC句柄

#define _MAX_FILE_PATH_                      1000 //最长文件路径
#define _DBC_NAME_LENGTH_					 65  //名称最长长度
#define _DBC_COMMENT_MAX_LENGTH_             200 //注释最长长度
#define _DBC_UNIT_MAX_LENGTH_                10  //单位最长长度
#define _DBC_SIGNAL_MAX_COUNT_                512 //一个消息含有的信号的最大数目

struct DBCSignal
{
	char strName[_DBC_NAME_LENGTH_+1];  //名称
	char strComment[_DBC_COMMENT_MAX_LENGTH_+1];  //注释
	uint nStartBit; // 起始位
	uint nLen;	// 位长度
	double nFactor; // 转换因子
	double nOffset;	// 转换偏移 实际值=原始值*nFactor+nOffset
	double nMin;    // 最小值
	double nMax;	// 最大值
	char unit[_DBC_UNIT_MAX_LENGTH_+1];//单位
	double nValue;  //实际值
};

struct  DBCMessage 
{
	char strName[_DBC_NAME_LENGTH_+1];  //名称
	char strComment[_DBC_COMMENT_MAX_LENGTH_+1];    //注释
	uint nID;
	uint8 nExtend; //1:扩展帧, 0:标准帧
	uint nSize;	//消息占的字节数目
	DBCSignal vSignals[_DBC_SIGNAL_MAX_COUNT_+1]; //信号集合
	uint nSignalCount; //信号数量
};

struct FileInfo
{
	char strFilePath[_MAX_FILE_PATH_+1]; //dbc文件路径
	enum ProtocolType
	{
		DBC_J1939,
		DBC_CAN,
	} nType; //dbc的协议类型, j1939选择DBC_J1939, 其他协议选择DBC_CAN
};

/*
功能说明：库没实际发送数据功能, 只提供发送的数据,
		  调用者需要实现该回调函数处理实际数据发送,
		  简单调用VCI_Transmit发送即可。
参    数[in]：ctx-回调函数的上下文
参    数[in]：pObj-要发送的数据
返 回 值：void
*/
typedef void (*OnSend)(void* ctx, void* pObj);

/*
功能说明：多帧传输完成后回调, 如果不涉及多帧传输也可以不实现该回调
参    数[in]：ctx-回调函数的上下文
参    数[in]：pMsg-返回的DBC消息
参    数[in]：data-多帧的实际数据
参    数[in]：nLen-多帧数据的字节数目
参    数[in]：nDirection-多帧的传输方向 1:接收, 0:发送
返 回 值：void
*/
typedef void (*OnMultiTransDone)(void* ctx, DBCMessage* pMsg, uint8* data, uint16 nLen, uint8 nDirection);

/*
功能说明：初始化解析模块。
返 回 值：INVALID_DBC_HANDLE:初始化失败
		  其他值:初始化成功
		  保存该值作后续使用调用
*/
EXTERN_C DBCHandle __stdcall DBC_Init();

/*
功能说明：释放资源, 与DBC_Init配对使用
参    数[in]：hDBC-句柄, DBC_Load的返回值
*/
EXTERN_C void __stdcall DBC_Release( DBCHandle hDBC );

/*
功能说明：加载DBC格式文件。
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[in]：pFileInfo-文件信息, 参考struct FileInfo
返 回 值：true加载成功
*/
EXTERN_C bool __stdcall DBC_LoadFile( DBCHandle hDBC, const FileInfo* pFileInfo );

/*
功能说明：获取第一条消息。
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[out]：pMsg 存储消息的信息
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall DBC_GetFirstMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
功能说明：获取下一条消息。
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[out]：pMsg 存储消息的信息
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall DBC_GetNextMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
功能说明：根据ID获取消息。
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[in]：nID-消息的ID
参    数[out]：pMsg 存储消息的信息
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall DBC_GetMessageById( DBCHandle hDBC, uint nID, DBCMessage* pMsg );
/*
功能说明：获取消息个数。
参    数[in]：hDBC-句柄, DBC_Load的返回值
返 回 值：消息个数
*/
EXTERN_C uint __stdcall DBC_GetMessageCount( DBCHandle hDBC );

/*
功能说明：根据DBC协议解析帧数据
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[in]：pObj 原始帧数据
参    数[out]：pMsg 解析结果
返 回 值：true表示成功
*/
EXTERN_C bool DBC_Analyse( DBCHandle hDBC, const void* pObj, DBCMessage* pMsg );

/*
功能说明：调用者需要调用该函数把接收到的帧数据传进来,
		  涉及多帧传输必须要这样调用, 否则无法实现设备交互。
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[in]：pObj 原始帧数据
返 回 值：void
*/
EXTERN_C void DBC_OnReceive( DBCHandle hDBC, const void* pObj );

/*
功能说明：设置实际发送数据的处理者, 涉及数据发送时必须设置，只需要设置一次。
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[in]：sender回调函数指针
参    数[in]：ctx-回调函数的上下文
返 回 值：void
*/
EXTERN_C void DBC_SetSender( DBCHandle hDBC, OnSend sender, void* ctx );

/*
功能说明：设置处理多帧传输数据的回调函数，只需要设置一次
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[in]：OnMultiTransDone回调函数指针
参    数[in]：ctx-回调函数的上下文
返 回 值：void
*/
EXTERN_C void DBC_SetOnMultiTransDoneFunc( DBCHandle hDBC, OnMultiTransDone func, void* ctx);
/*
功能说明：发送DBC消息。
参    数[in]：hDBC-句柄, DBC_Load的返回值
参    数[in]：pMsg 要发送的消息
返 回 值：true表示成功
*/
EXTERN_C bool DBC_Send( DBCHandle hDBC, const DBCMessage* pMsg );

#endif // _IDBCMANAGER_H_