/** @mainpage 概述
*
*@section 公司网址
* http://www.do3think.com
*
*@section 当前API
* Version 2.16.32.605(build: May 21 2018 12:46:08)
*
*
*@section 文档发布记录
*-# 2016-4-1(2.5.12) 发布第一个版本
*-# 2016-10-31(2.7.8)
*-# a.增加@link #dvpGetCurveLut 曲线LUT@endlink功能
*-# b.增加@link #dvpColorCorrection 颜色校正参数@endlink的读写功能
*-# c.增加@link #dvpCaptureFile 在指定分辨率模式和ROI下抓图并保存图像@endlink的功能
*-# d.增加@link #dvpGetRotateOpposite 图像的正/负90度旋转@endlink的功能
*-# e.扩展了@link #dvpFrame 帧信息结构体@endlink，增加了翻转、旋转等标识
*-# 2016-11-29(2.8.9)
*-# a.增加@link #dvpSetAeConfig 自动曝光配置@endlink功能
*-# b.增加@link #dvpEventCallback 事件通知回调函数@endlink功能
*-# c.增加@link #dvpGetFrameBuffer 同时捕获原始采集帧和目标输出帧@endlink的功能
*-# 2017-02-08(2.9.13)
*-# a.增加@link #dvpResetDevice 复位设备@endlink的功能
*-# 2017-03-22(2.10.16)
*-# a.增加@link #dvpSetAutoDefectFixState 使能自动消除彩点@endlink的功能
*-# 2017-05-12(2.11.18)
*-# a.增加@link #dvpHold 中断传感器的图像采集@endlink的功能
*-# b.增加@link #dvpSetHardwareIspState 相机硬件ISP使能@endlink的功能
*-# 2017-06-16(2.12.19)
*-# a.增加@link #dvpGetFunctionInfo 获取相机功能描述信息@endlink的接口
*-# b.增加@link #dvpImageFormat::FORMAT_RGB32 RGB32图像格式@endlink输出的支持
*-# 2017-07-17(2.14.21)
*-# a.扩充了@link #dvpStreamFormat 数据流格式@endlink的定义
*-# 2017-09-15(2.15.24)
*-# a.增加了根据@link #dvpCameraInfo::UserID 用户命名@endlink来@link #dvpOpenByUserId 打开相机@endlink的功能
*-# b.增加了加载@link #dvpLoadUserSet 用户设置@endlink的功能
*-# 2018-04-11(2.16.32)
*-# a.增加了在用户线程中@link #dvpProcessImage 处理图像@endlink的功能
*/
#ifndef __DVPCAMERA_H__
#define __DVPCAMERA_H__

#define dvpPtr(ptr) ptr*

#ifndef __cplusplus
#define bool unsigned char
#define false (0)
#define true  (1)
#endif

#ifndef _WIN32
#ifndef __cplusplus
#define dvp2api extern
#else
#define dvp2api extern "C"
#endif
#else
#include <windows.h>
#ifndef __cplusplus
#define dvp2api extern     __declspec(dllimport)
#else
#define dvp2api extern "C" __declspec(dllimport)
#endif
#endif

/** @defgroup __DVP2_ENUM__ 枚举型
*@{
*/
/** @brief 图像格式*/
typedef enum dvpImageFormat
{
    /** @brief 黑白图像 */
    FORMAT_MONO = 0,

    /** @brief BGGR图像 */
    FORMAT_BAYER_BG = 1,

    /** @brief GBRG图像 */
    FORMAT_BAYER_GB = 2,

    /** @brief GRBG图像 */
    FORMAT_BAYER_GR = 3,

    /** @brief RGGB图像 */
    FORMAT_BAYER_RG = 4,

    /** @brief BGR三通道24比特图像 */
    FORMAT_BGR24 = 10,

    /** @brief BGRA四通道32比特图像 */
    FORMAT_BGR32 = 11,

    /** @brief RGBA四通道32比特图像 */
    FORMAT_RGB32 = 15,

    /** @brief YUV411 */
    FORMAT_YUV_411 = 20,

    /** @brief YUV422 */
    FORMAT_YUV_422 = 21,

} dvpImageFormat;


/** @brief 数据流格式*/
typedef enum dvpStreamFormat
{
    /** @brief 8位图像 */
    S_RAW8 = 0,

    /** @brief 10位图像 */
    S_RAW10 = 1,

    /** @brief 12位图像 */
    S_RAW12 = 2,

    /** @brief 14位图像 */
    S_RAW14 = 3,

    /** @brief 16位图像 */
    S_RAW16 = 4,

    /** @brief BGR三通道24比特图像 */
    S_BGR24 = 10,

    /** @brief BGRA四通道32比特图像 */
    S_BGR32 = 11,

    /** @brief BGRA四通道48比特图像 */
    S_BGR48 = 12,

    /** @brief BGRA四通道64比特图像 */
    S_BGR64 = 13,

    /** @brief RGB三通道24比特图像 */
    S_RGB24 = 14,

    /** @brief RGBA四通道32比特图像 */
    S_RGB32 = 15,

    /** @brief RGBA四通道48比特图像 */
    S_RGB48 = 16,

    /** @brief RGBA四通道64比特图像 */
    S_RGB64 = 17,

    /** @brief YUV411 */
    S_YCBCR_411 = 20,

    /** @brief YUV422 */
    S_YCBCR_422 = 21,

    /** @brief YUV444 */
    S_YCBCR_444 = 22,

    /** @brief 8位灰度图像 */
    S_MONO8 = 30,

    /** @brief 10位灰度图像 */
    S_MONO10 = 31,

    /** @brief 12位灰度图像 */
    S_MONO12 = 32,

    /** @brief 14位灰度图像 */
    S_MONO14 = 33,

    /** @brief 16位灰度图像 */
    S_MONO16 = 34,

} dvpStreamFormat;


/** @brief 图像位宽*/
typedef enum dvpBits
{
    /** @brief 8比特数据 */
    BITS_8 = 0,

    /** @brief 10比特数据 */
    BITS_10 = 1,

    /** @brief 12比特数据 */
    BITS_12 = 2,

    /** @brief 14比特数据 */
    BITS_14 = 3,

    /** @brief 16比特数据 */
    BITS_16 = 4,

} dvpBits;


/** @brief 自动曝光模式*/
typedef enum dvpAeMode
{
    /** @brief 自动曝光，自动增益同时开启，曝光优先 */
    AE_MODE_AE_AG = 0,

    /** @brief 自动曝光，自动增益同时开启，增益优先 */
    AE_MODE_AG_AE = 1,

    /** @brief 自动曝光 */
    AE_MODE_AE_ONLY = 2,

    /** @brief 自动增益 */
    AE_MODE_AG_ONLY = 3,

} dvpAeMode;


/** @brief 自动曝光操作*/
typedef enum dvpAeOperation
{
    /** @brief 功能关闭 */
    AE_OP_OFF = 0,

    /** @brief 进行一次 */
    AE_OP_ONCE = 1,

    /** @brief 连续操作 */
    AE_OP_CONTINUOUS = 2,

} dvpAeOperation;


/** @brief 自动白平衡操作*/
typedef enum dvpAwbOperation
{
    /** @brief 功能关闭 */
    AWB_OP_OFF = 0,

    /** @brief 进行一次 */
    AWB_OP_ONCE = 1,

    /** @brief 连续操作 */
    AWB_OP_CONTINUOUS = 2,

} dvpAwbOperation;


/** @brief 灯光频闪*/
typedef enum dvpAntiFlick
{
    /** @brief 禁止消除频闪 */
    ANTIFLICK_DISABLE = 0,

    /** @brief 消除50hz频闪 */
    ANTIFLICK_50HZ = 1,

    /** @brief 消除60hz频闪 */
    ANTIFLICK_60HZ = 2,

} dvpAntiFlick;


/** @brief 传感器类型*/
typedef enum dvpSensorType
{
    /** @brief CMOS图像传感器 */
    SENSOR_TYPE_CMOS = 0,

    /** @brief CCD图像传感器 */
    SENSOR_TYPE_CCD = 1,

} dvpSensorType;


/** @brief 传感器像素类型*/
typedef enum dvpSensorPixel
{
    /** @brief 黑白像素 */
    SENSOR_PIXEL_MONO = 0,

    /** @brief RGGB彩色像素 */
    SENSOR_PIXEL_BAYER_RG = 1,

    /** @brief GBRG彩色像素 */
    SENSOR_PIXEL_BAYER_GB = 2,

    /** @brief GRBG彩色像素 */
    SENSOR_PIXEL_BAYER_GR = 3,

    /** @brief BGGR彩色像素 */
    SENSOR_PIXEL_BAYER_BG = 4,

} dvpSensorPixel;


/** @brief 视频流回调事件*/
typedef enum dvpStreamEvent
{
    /** @brief 图像达到后 */
    STREAM_EVENT_ARRIVED = 0,

    /** @brief 图像校正后 */
    STREAM_EVENT_CORRECTED = 1,

    /** @brief 图像处理后 */
    STREAM_EVENT_PROCESSED = 2,

    /** @brief 启动一个专门的线程以@link #dvpGetFrame dvpGetFrame@endlink 的方式获取图像，并调用其他用户处理过程 
*@warning 须在@link #dvpStart 启动视频流@endlink之前设置好
*@see dvpProcessImage */
    STREAM_EVENT_FRAME_THREAD = 3,

} dvpStreamEvent;


/** @brief 普通事件*/
typedef enum dvpEvent
{
    /** @brief 未知的 */
    EVENT_UNKNOW = 0,

    /** @brief 连接成功 */
    EVENT_CONNECTED = 1,

    /** @brief 断开连接 */
    EVENT_DISCONNECTED = 2,

    /** @brief 数据流已经启动 */
    EVENT_STREAM_STARTRD = 3,

    /** @brief 数据流已经停止 */
    EVENT_STREAM_STOPPED = 4,

    /** @brief 丢帧(错误帧) */
    EVENT_FRAME_LOST = 5,

    /** @brief 等待帧超时 */
    EVENT_FRAME_TIMEOUT = 6,

    /** @brief 失去连接 */
    EVENT_LOST_CONNECTION = 7,

    /** @brief 重新连接 */
    EVENT_RECONNECTED = 8,

    /** @brief 帧开始传输 */
    EVENT_FRAME_START = 9,

    /** @brief 帧结束传输 */
    EVENT_FRAME_END = 10,

} dvpEvent;


/** @brief 触发输入信号类型*/
typedef enum dvpTriggerInputType
{
    /** @brief 触发输入关闭 */
    TRIGGER_IN_OFF = 0,

    /** @brief 下降沿触发 */
    TRIGGER_NEG_EDGE = 1,

    /** @brief 低电平触发 */
    TRIGGER_LOW_LEVEL = 2,

    /** @brief 上升沿触发 */
    TRIGGER_POS_EDGE = 3,

    /** @brief 高电平触发 */
    TRIGGER_HIGH_LEVEL = 4,

} dvpTriggerInputType;


/** @brief Strobe输出信号类型*/
typedef enum dvpStrobeOutputType
{
    /** @brief Strobe输出关闭 */
    STROBE_OUT_OFF = 0,

    /** @brief 输出低电平 */
    STROBE_OUT_LOW = 1,

    /** @brief 输出高电平 */
    STROBE_OUT_HIGH = 2,

} dvpStrobeOutputType;


/** @brief Strobe信号驱动方式*/
typedef enum dvpStrobeDriver
{
    /** @brief 整个帧周期都有效 */
    FRAME_DURATION = 0,

    /** @brief 由定时器驱动 */
    TIMER_LOGIC = 1,

    /** @brief 跟随SENSOR的Strobe信号 */
    SENSOR_STROBE = 2,

} dvpStrobeDriver;


/** @brief 输出模式*/
typedef enum dvpOutputIoFunction
{
    /** @brief 普通输出 */
    OUTPUT_FUNCTION_NORMAL = 0x0000,

    /** @brief 闪光灯输出 */
    OUTPUT_FUNCTION_STROBE = 0x0001,

} dvpOutputIoFunction;


/** @brief 输入模式*/
typedef enum dvpInputIoFunction
{
    /** @brief 普通输入 */
    INPUT_FUNCTION_NORMAL = 0x8001,

    /** @brief 触发输入 */
    INPUT_FUNCTION_TRIGGER = 0x8002,

} dvpInputIoFunction;


/** @brief 输出IO*/
typedef enum dvpOutputIo
{
    /** @brief 输出1 */
    OUTPUT_IO_1 = 0x0001,

    /** @brief 输出2 */
    OUTPUT_IO_2 = 0x0002,

    /** @brief 输出3 */
    OUTPUT_IO_3 = 0x0003,

    /** @brief 输出4 */
    OUTPUT_IO_4 = 0x0004,

    /** @brief 输出5 */
    OUTPUT_IO_5 = 0x0005,

    /** @brief 输出6 */
    OUTPUT_IO_6 = 0x0006,

    /** @brief 输出7 */
    OUTPUT_IO_7 = 0x0007,

    /** @brief 输出8 */
    OUTPUT_IO_8 = 0x0008,

} dvpOutputIo;


/** @brief 输入IO*/
typedef enum dvpInputIo
{
    /** @brief 输入1 */
    INPUT_IO_1 = 0x8001,

    /** @brief 输入2 */
    INPUT_IO_2 = 0x8002,

    /** @brief 输入3 */
    INPUT_IO_3 = 0x8003,

    /** @brief 输入4 */
    INPUT_IO_4 = 0x8004,

    /** @brief 输入5 */
    INPUT_IO_5 = 0x8005,

    /** @brief 输入6 */
    INPUT_IO_6 = 0x8006,

    /** @brief 输入7 */
    INPUT_IO_7 = 0x8007,

    /** @brief 输入8 */
    INPUT_IO_8 = 0x8008,

} dvpInputIo;


/** @brief 第一个像素对应的位置*/
typedef enum dvpFirstPosition
{
    /** @brief 左上角 */
    UP_LEFT = 0,

    /** @brief 右上角 */
    UP_RIGHT = 1,

    /** @brief 左下角 */
    BOTTOM_LEFT = 2,

    /** @brief 右下角 */
    BOTTOM_RIGHT = 3,

} dvpFirstPosition;


/** @brief 设备打开模式
*@note 支持"或"运算，比如：@code dvpOpen(0, dvpOpenMode(OPEN_NORMAL | OPEN_DEBUG), &handle) @endcode*/
typedef enum dvpOpenMode
{
    /** @brief 离线打开 */
    OPEN_OFFLINE = 0,

    /** @brief 打开实际设备 */
    OPEN_NORMAL = 1,

    /** @brief 以调试方式打开设备；对于千兆网相机，可以避免单步调试或断点引起的心跳包超时 */
    OPEN_DEBUG = 1<<3,

    /** @brief 图像采集和处理线程使用较高的优先级 */
    HIGH_PRIORITY = 1<<4,

} dvpOpenMode;


/** @brief 视频流状态@see dvpGetStreamState*/
typedef enum dvpStreamState
{
    /** @brief 已停止@see dvpStop */
    STATE_STOPED = 0,

    /** @brief 已启动@see dvpStart */
    STATE_STARTED = 2,

} dvpStreamState;


/** @brief 曲线拟合风格*/
typedef enum dvpCurveStyle
{
    /** @brief 直线拟合 */
    CURVE_STYLE_LINE = 0,

    /** @brief 平滑拟合 */
    CURVE_STYLE_WAVE = 1,

} dvpCurveStyle;


/** @brief 用户设置
*@note 通常存储在相机存储器上*/
typedef enum dvpUserSet
{
    /** @brief 默认只读设置 */
    USER_SET_DEFAULT = 0,

    /** @brief 用户设置1 */
    USER_SET_1 = 1,

    /** @brief 用户设置2 */
    USER_SET_2 = 2,

} dvpUserSet;


/** @brief 所有标准接口的状态码*/
typedef enum dvpStatus
{
    /** @brief 操作忽略掉了，不须要任何动作 */
    DVP_STATUS_IGNORED = 7,

    /** @brief 需要其他数据和操作 */
    DVP_STATUS_NEED_OTHER = 6,

    /** @brief 还须进行下一阶段，只完成了部分动作 */
    DVP_STATUS_NEXT_STAGE = 5,

    /** @brief 正忙，此次操作不能进行 */
    DVP_STATUS_BUSY = 4,

    /** @brief 需要等待，可以再次尝试 */
    DVP_STATUS_WAIT = 3,

    /** @brief 正在进行，已经被操作过 */
    DVP_STATUS_IN_PROCESS = 2,

    /** @brief 操作成功 */
    DVP_STATUS_OK = 1,

    /** @brief 操作失败 */
    DVP_STATUS_FAILED = 0,

    /** @brief 未知错误 */
    DVP_STATUS_UNKNOW = -1,

    /** @brief 不支持该功能 */
    DVP_STATUS_NOT_SUPPORTED = -2,

    /** @brief 初始化未完成 */
    DVP_STATUS_NOT_INITIALIZED = -3,

    /** @brief 参数无效 */
    DVP_STATUS_PARAMETER_INVALID = -4,

    /** @brief 参数越界 */
    DVP_STATUS_PARAMETER_OUT_OF_BOUND = -5,

    /** @brief 特性未打开 */
    DVP_STATUS_UNENABLED = -6,

    /** @brief 未连接到设备 */
    DVP_STATUS_UNCONNECTED = -7,

    /** @brief 功能无效 */
    DVP_STATUS_NOT_VALID = -8,

    /** @brief 设备没打开 */
    DVP_STATUS_UNPLAY = -9,

    /** @brief 未启动 */
    DVP_STATUS_NOT_STARTED = -10,

    /** @brief 未停止 */
    DVP_STATUS_NOT_STOPPED = -11,

    /** @brief 未准备好 */
    DVP_STATUS_NOT_READY = -12,

    /** @brief 无效句柄（空句柄或野句柄），通常是相机未打开所致 */
    DVP_STATUS_INVALID_HANDLE = -13,

    /** @brief 错误的描述 */
    DVP_STATUS_DESCR_FAULT = -20,

    /** @brief 错误的名称 */
    DVP_STATUS_NAME_FAULT = -21,

    /** @brief 错误的赋值 */
    DVP_STATUS_VALUE_FAULT = -22,

    /** @brief 被限制 */
    DVP_STATUS_LIMITED = -28,

    /** @brief 功能无效 */
    DVP_STATUS_FUNCTION_INVALID = -29,

    /** @brief 在自动进行中，手动方式无效 */
    DVP_STATUS_IN_AUTO = -30,

    /** @brief 操作被拒绝 */
    DVP_STATUS_DENIED = -31,

    /** @brief 偏移或地址未对齐 */
    DVP_STATUS_BAD_ALIGNMENT = -40,

    /** @brief 地址无效 */
    DVP_STATUS_ADDRESS_INVALID = -41,

    /** @brief 数据块大小无效 */
    DVP_STATUS_SIZE_INVALID = -42,

    /** @brief 数据量过载 */
    DVP_STATUS_OVER_LOAD = -43,

    /** @brief 数据量不够 */
    DVP_STATUS_UNDER_LOAD = -44,

    /** @brief 检查，校验失败 */
    DVP_STATUS_CHECKED_FAILED = -50,

    /** @brief 不可用 */
    DVP_STATUS_UNUSABLE = -51,

    /** @brief 业务ID无效或不匹配 */
    DVP_STATUS_BID_INVALID = -52,

    /** @brief 超时错误 */
    DVP_STATUS_TIME_OUT = -1000,

    /** @brief 硬件IO错误 */
    DVP_STATUS_IO_ERROR = -1001,

    /** @brief 通讯错误 */
    DVP_STATUS_COMM_ERROR = -1002,

    /** @brief 总线错误 */
    DVP_STATUS_BUS_ERROR = -1003,

    /** @brief 格式错误 */
    DVP_STATUS_FORMAT_INVALID = -1004,

    /** @brief 内容无效 */
    DVP_STATUS_CONTENT_INVALID = -1005,

    /** @brief I2C总线错误 */
    DVP_STATUS_I2C_FAULT = -1010,

    /** @brief I2C等待应答超时 */
    DVP_STATUS_I2C_ACK_TIMEOUT = -1011,

    /** @brief I2C等待总线动作超时，例如SCL被外部器件拉为低电平 */
    DVP_STATUS_I2C_BUS_TIMEOUT = -1012,

    /** @brief SPI总线错误 */
    DVP_STATUS_SPI_FAULT = -1020,

    /** @brief UART总线错误 */
    DVP_STATUS_UART_FAULT = -1030,

    /** @brief GPIO总线错误 */
    DVP_STATUS_GPIO_FAULT = -1040,

    /** @brief USB总线错误 */
    DVP_STATUS_USB_FAULT = -1050,

    /** @brief PCI总线错误 */
    DVP_STATUS_PCI_FAULT = -1060,

    /** @brief 物理层错误 */
    DVP_STATUS_PHY_FAULT = -1070,

    /** @brief 链路层错误 */
    DVP_STATUS_LINK_FAULT = -1080,

    /** @brief 传输层错误 */
    DVP_STATUS_TRANS_FAULT = -1090,

    /** @brief 没有发现设备 */
    DVP_STATUS_NO_DEVICE_FOUND = -1100,

    /** @brief 未找到逻辑设备 */
    DVP_STATUS_NO_LOGIC_DEVICE_FOUND = -1101,

    /** @brief 设备已经打开 */
    DVP_STATUS_DEVICE_IS_OPENED = -1102,

    /** @brief 设备已经关闭 */
    DVP_STATUS_DEVICE_IS_CLOSED = -1103,

    /** @brief 设备已经断开连接 */
    DVP_STATUS_DEVICE_IS_DISCONNECTED = -1104,

    /** @brief 设备已经被其他主机打开 */
    DVP_STATUS_DEVICE_IS_OPENED_BY_ANOTHER = -1105,

    /** @brief 设备已经被启动 */
    DVP_STATUS_DEVICE_IS_STARTED = -1106,

    /** @brief 设备已经被停止设备已经被其他主机打开 */
    DVP_STATUS_DEVICE_IS_STOPPED = -1107,

    /** @brief 没有足够系统内存 */
    DVP_STATUS_INSUFFICIENT_MEMORY = -1200,

    /** @brief 存储器读写出现误码或无法正常读写 */
    DVP_STATUS_MEMORY_FAULT = -1201,

    /** @brief 写保护，不可写 */
    DVP_STATUS_WRITE_PROTECTED = -1202,

    /** @brief 创建文件失败 */
    DVP_STATUS_FILE_CREATE_FAILED = -1300,

    /** @brief 文件格式无效 */
    DVP_STATUS_FILE_INVALID = -1301,

    /** @brief 读取文件失败 */
    DVP_STATUS_FILE_READ_FAILED = -1302,

    /** @brief 写入文件失败 */
    DVP_STATUS_FILE_WRITE_FAILED = -1303,

    /** @brief 打开文件失败 */
    DVP_STATUS_FILE_OPEN_FAILED = -1304,

    /** @brief 读取数据较检失败 */
    DVP_STATUS_FILE_CHECKSUM_FAILED = -1305,

    /** @brief 数据采集失败，指定的时间内未获得数据 */
    DVP_STATUS_GRAB_FAILED = -1600,

    /** @brief 数据丢失，不完整 */
    DVP_STATUS_LOST_DATA = -1601,

    /** @brief 未接收到帧结束符 */
    DVP_STATUS_EOF_ERROR = -1602,

    /** @brief 数据采集功能已经打开 */
    DVP_STATUS_GRAB_IS_OPENED = -1603,

    /** @brief 数据采集功能已经关闭 */
    DVP_STATUS_GRAB_IS_CLOSED = -1604,

    /** @brief 数据采集已经启动 */
    DVP_STATUS_GRAB_IS_STARTED = -1605,

    /** @brief 数据采集已经停止 */
    DVP_STATUS_GRAB_IS_STOPPED = -1606,

    /** @brief 数据采集正在重启 */
    DVP_STATUS_GRAB_IS_RESTARTING = -1607,

    /** @brief 设置的ROI参数无效 */
    DVP_STATUS_ROI_PARAM_INVALID = -1610,

    /** @brief ROI功能不支持 */
    DVP_STATUS_ROI_NOT_SUPPORTED = -1611,

} dvpStatus;


/** @} end of __DVP2_ENUM__ */
#ifndef _WIN32

#include <stdint.h>

/** @brief 8位无符号整数 */
typedef uint8_t dvpByte;

/** @brief 32位有符号整数 */
typedef int32_t dvpInt32;

/** @brief 32位无符号整数 */
typedef uint32_t dvpUint32;

/** @brief 64位有符号整数 */
typedef int64_t dvpInt64;

/** @brief 64位无符号整数 */
typedef uint64_t dvpUint64;

#else
/** @brief 8位无符号整数 */
typedef BYTE dvpByte;

/** @brief 32位有符号整数 */
typedef INT32 dvpInt32;

/** @brief 32位无符号整数 */
typedef UINT32 dvpUint32;

/** @brief 64位有符号整数 */
typedef INT64 dvpInt64;

/** @brief 64位无符号整数 */
typedef UINT64 dvpUint64;

#endif // _WIN32
/** @brief 128字节保留空间 */
typedef dvpUint32 dvpReserved[32];

/** @brief 64字节长的字符串 */
typedef char dvpString64[64];

/** @brief 128字节长的字符串 */
typedef char dvpString128[128];

/** @brief 32字节长的布尔数组 */
typedef bool dvpBool32[32];

/** @brief 4行3列简单矩阵 */
typedef float dvpMatrix[4][3];

/** @brief 3元简单浮点数组 */
typedef float dvpFloat3c[3];

/** @brief 3元简单整型数组 */
typedef dvpUint32 dvpSize3c[3];

/** @brief 相机句柄
*@see dvpOpen
*@see dvpOpenByName */
typedef dvpUint32 dvpHandle;

/** @brief 视频编码器句柄
*@see dvpStartVideoRecord */
typedef dvpUint32 dvpRecordHandle;

/** @brief 字符串指针 */
typedef const char* dvpStr;

/** @brief 颜色矩阵 */
typedef struct dvpColorMatrix
{
    /** @brief 矩阵 */
    dvpMatrix matrix;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpColorMatrix;


/** @brief 颜色校正信息 */
typedef struct dvpColorCorrection
{
    /** @brief BGR三色校正系数 */
    dvpFloat3c bgr;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpColorCorrection;


/** @brief 二维浮点数坐标 */
typedef struct dvpPoint2f
{
    /** @brief 横坐标 */
    float X;

    /** @brief 纵坐标 */
    float Y;

} dvpPoint2f;


/** @brief BGR三色256个锚点 */
typedef struct dvpPoint2f dvpPoint3c[3][256];

/** @brief BGR曲线数组 */
typedef struct dvpCurveArray
{
    /** @brief BGR曲线数组空间 */
    dvpPoint3c point;

    /** @brief BGR有效通道数，最多3个 */
    dvpUint32 rows;

    /** @brief BGR三通道的有效点数，每通道最多256个 */
    dvpSize3c cols;

} dvpCurveArray;


/** @brief 由曲线定义的LUT */
typedef struct dvpCurveLut
{
    /** @brief 使能状态 */
    bool enable;

    /** @brief 曲线风格 */
    dvpCurveStyle style;

    /** @brief 锚点数组，由少数几个锚点可以确定一个完整的LUT */
    dvpCurveArray array;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpCurveLut;


/** @brief 相机信息 
*@see dvpGetCameraInfo
*@see dvpEnum */
typedef struct dvpCameraInfo
{
    /** @brief 设计厂商 */
    dvpString64 Vendor;

    /** @brief 生产厂商 */
    dvpString64 Manufacturer;

    /** @brief 型号 */
    dvpString64 Model;

    /** @brief 系列 */
    dvpString64 Family;

    /** @brief 连接名 */
    dvpString64 LinkName;

    /** @brief 传感器描述 */
    dvpString64 SensorInfo;

    /** @brief 硬件版本 */
    dvpString64 HardwareVersion;

    /** @brief 固件版本 */
    dvpString64 FirmwareVersion;

    /** @brief 内核驱动版本 */
    dvpString64 KernelVersion;

    /** @brief 设备驱动版本 */
    dvpString64 DscamVersion;

    /** @brief 友好设备名称 */
    dvpString64 FriendlyName;

    /** @brief 接口描述 */
    dvpString64 PortInfo;

    /** @brief 序列号 */
    dvpString64 SerialNumber;

    /** @brief 相机描述 */
    dvpString128 CameraInfo;

    /** @brief 用户命名 */
    dvpString128 UserID;

    /** @brief 保留字节 */
    dvpString128 reserved;
} dvpCameraInfo;


/** @brief 区域描述 */
typedef struct dvpRegionDescr
{
    /** @brief 最小宽度 */
    dvpInt32 iMinW;

    /** @brief 最小高度 */
    dvpInt32 iMinH;

    /** @brief 最大宽度 */
    dvpInt32 iMaxW;

    /** @brief 最大高度 */
    dvpInt32 iMaxH;

    /** @brief 宽度步长 */
    dvpInt32 iStepW;

    /** @brief 高度步长 */
    dvpInt32 iStepH;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpRegionDescr;


/** @brief 起始于左上角的感兴趣的区域
*@see dvpGetRoi
*@see dvpGetAwbRoi
*@see dvpGetAeRoi
*@warning 数据成员通常为偶数，详情须参考设备手册 */
typedef struct dvpRegion
{
    /** @brief 横向起始位置 */
    dvpInt32 X;

    /** @brief 纵向起始位置 */
    dvpInt32 Y;

    /** @brief 宽度（大于0） */
    dvpInt32 W;

    /** @brief 高度（大于0） */
    dvpInt32 H;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpRegion;


/** @brief 帧信息
*@see dvpGetFrame */
typedef struct dvpFrame
{
    /** @brief 格式 */
    enum dvpImageFormat format;

    /** @brief 位宽 */
    enum dvpBits bits;

    /** @brief 字节数 */
    dvpUint32 uBytes;

    /** @brief 宽度 */
    dvpInt32 iWidth;

    /** @brief 高度 */
    dvpInt32 iHeight;

    /** @brief 帧编号 */
    dvpUint64 uFrameID;

    /** @brief 时间戳 */
    dvpUint64 uTimestamp;

    /** @brief 曝光时间（单位为微秒）
*@see dvpGetExposure */
    double fExposure;

    /** @brief 模拟增益
*@see dvpGetAnalogGain */
    float fAGain;

    /** @brief 第一个像素点的位置
*@see dvpGetFirstPosition */
    enum dvpFirstPosition position;

    /** @brief 是否水平翻转
*@see dvpGetFlipHorizontalState */
    bool bFlipHorizontalState;

    /** @brief 是否垂直翻转
*@see dvpGetFlipVerticalState */
    bool bFlipVerticalState;

    /** @brief 是否旋转90度
*@see dvpGetRotateState */
    bool bRotateState;

    /** @brief 是否逆时针旋转
*@see dvpGetRotateOpposite */
    bool bRotateOpposite;

    /** @brief 内部标志位 */
    dvpUint32 internalFlags;

    /** @brief 内部信息 */
    dvpUint32 internalValue;

    /** @brief 保留字节 */
    dvpUint32 reserved[28];
} dvpFrame;


/** @brief 帧信息与图像数据
*@see dvpGetFrameBuffer */
typedef struct dvpFrameBuffer
{
    /** @brief 帧信息 */
    struct dvpFrame frame;

    /** @brief 图像数据首地址
*@warning 切勿手动释放 */
    dvpPtr(dvpByte) pBuffer;

} dvpFrameBuffer;


/** @brief 通用数据
*@see dvpEventCallback */
typedef struct dvpVariant
{
    /** @brief 数据首地址 */
    dvpPtr(dvpByte) pData;

    /** @brief 数据字节数 */
    dvpUint32 uSize;

    /** @brief 数据类型名称 */
    dvpString64 name;

} dvpVariant;


/** @brief 整型数据描述 */
typedef struct dvpIntDescr
{
    /** @brief 步长 */
    dvpInt32 iStep;

    /** @brief 最小值 */
    dvpInt32 iMin;

    /** @brief 最大值 */
    dvpInt32 iMax;

    /** @brief 默认值 */
    dvpInt32 iDefault;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpIntDescr;


/** @brief 浮点型数据描述 */
typedef struct dvpFloatDescr
{
    /** @brief 步长 */
    float fStep;

    /** @brief 最小值 */
    float fMin;

    /** @brief 最大值 */
    float fMax;

    /** @brief 默认值 */
    float fDefault;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpFloatDescr;


/** @brief 双精度型数据描述 */
typedef struct dvpDoubleDescr
{
    /** @brief 步长 */
    double fStep;

    /** @brief 最小值 */
    double fMin;

    /** @brief 最大值 */
    double fMax;

    /** @brief 默认值 */
    double fDefault;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpDoubleDescr;


/** @brief 64位无符号整型数据描述 */
typedef struct dvpUint64Descr
{
    /** @brief 步长 */
    dvpUint64 uStep;

    /** @brief 最小值 */
    dvpUint64 uMin;

    /** @brief 最大值 */
    dvpUint64 uMax;

    /** @brief 默认值 */
    dvpUint64 uDefault;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpUint64Descr;


/** @brief 选择项描述 */
typedef struct dvpSelectionDescr
{
    /** @brief 默认索引 */
    dvpUint32 uDefault;

    /** @brief 索引个数 */
    dvpUint32 uCount;

    /** @brief 是否应停止视频流 */
    bool bNeedStop;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpSelectionDescr;


/** @brief 选择项 */
typedef struct dvpSelection
{
    /** @brief 字符串描述 */
    dvpString64 string;

    /** @brief 索引 */
    dvpInt32 iIndex;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpSelection;


/** @brief 分辨率模式
*@see dvpGetTriggerModeSelDetail */
typedef struct dvpResolutionMode
{
    /** @brief 分辨率模式选择项 */
    struct dvpSelection selection;

    /** @brief 区域 */
    struct dvpRegionDescr region;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpResolutionMode;


/** @brief 快速Roi
*@see dvpGetQuickRoiSelDetail */
typedef struct dvpQuickRoi
{
    /** @brief 选择项 */
    struct dvpSelection selection;

    /** @brief 感兴趣的区域 */
    struct dvpRegion roi;

    /** @brief 分辨率模式 */
    struct dvpResolutionMode mode;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpQuickRoi;


/** @brief 图像格式 */
typedef struct dvpFormatSelection
{
    /** @brief 分辨率模式选择项 */
    struct dvpSelection selection;

    /** @brief 对应的枚举类型 */
    enum dvpStreamFormat format;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpFormatSelection;


/** @brief 帧计数
*@see dvpGetFrameCount */
typedef struct dvpFrameCount
{
    /** @brief 接收帧数，一般为传输到电脑的帧 */
    dvpUint32 uFrameCount;

    /** @brief 丢失帧数，来不及读取的 */
    dvpUint32 uFrameDrop;

    /** @brief 放弃的，采集后被丢掉的 */
    dvpUint32 uFrameIgnore;

    /** @brief 错误帧计数 */
    dvpUint32 uFrameError;

    /** @brief 被采集到的正确帧 */
    dvpUint32 uFrameOK;

    /** @brief 被提交 / 输出的有效帧 */
    dvpUint32 uFrameOut;

    /** @brief 重传帧 */
    dvpUint32 uFrameResend;

    /** @brief 图像处理帧总数 */
    dvpUint32 uFrameProc;

    /** @brief 采集帧率 */
    float fFrameRate;

    /** @brief 图像处理帧率 */
    float fProcRate;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpFrameCount;


/** @brief 用户IO信息
*@see dvpGetUserIoInfo */
typedef struct dvpUserIoInfo
{
    /** @brief 可用的输入IO */
    dvpBool32 inputValid;

    /** @brief 可用的输出IO */
    dvpBool32 outputValid;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpUserIoInfo;


/** @brief 自动曝光配置
*@see dvpGetAeConfig */
typedef struct dvpAeConfig
{
    /** @brief 最小自动曝光时间（单位为微秒） */
    double fExposureMin;

    /** @brief 最大自动曝光时间（单位为微秒） */
    double fExposureMax;

    /** @brief 保留值1 */
    double reserved1;

    /** @brief 最小自动增益值 */
    float fGainMin;

    /** @brief 最大自动增益值 */
    float fGainMax;

    /** @brief 保留值2 */
    float reserved2;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpAeConfig;


/** @brief Sensor(图像传感器)信息
*@see dvpGetSensorInfo */
typedef struct dvpSensorInfo
{
    /** @brief 字符串描述 */
    dvpString64 descr;

    /** @brief 传感器类型 */
    enum dvpSensorType sensor;

    /** @brief 原始像素类型 */
    enum dvpSensorPixel pixel;

    /** @brief 区域 */
    struct dvpRegionDescr region;

    /** @brief 保留字节 */
    dvpReserved reserved;
} dvpSensorInfo;


/** @brief 相机功能描述信息
*@see dvpGetFunctionInfo */
typedef struct dvpFunctionInfo
{
    /** @brief 是否支持触发功能 */
    bool bTrigger;

    /** @brief 是否支持硬件ISP功能 */
    bool bHardwareIsp;

    /** @brief 是否支持UserSet功能 */
    bool bUserSet;

    /** @brief 是否支持水平翻转功能 */
    bool bHflip;

    /** @brief 是否支持垂直翻转功能 */
    bool bVflip;

    /** @brief 是否支持负片功能 */
    bool bInverse;

    /** @brief 保留字节 */
    dvpUint32 reserved[31];
} dvpFunctionInfo;



/** @defgroup __DVP2_API__ API
*@{
*/
/**
*@brief 获取相机信息
*@param[in] handle 相机句柄
*@param[out] pCameraInfo 相机信息
*@return 状态码
*@ingroup __DVP_INFORMATION__
*/
dvp2api dvpStatus dvpGetCameraInfo(dvpHandle handle, dvpCameraInfo *pCameraInfo);


/**
*@brief 获取传感器信息
*@param[in] handle 相机句柄
*@param[out] pSensorInfo 传感器信息
*@return 状态码
*@ingroup __DVP_INFORMATION__
*/
dvp2api dvpStatus dvpGetSensorInfo(dvpHandle handle, dvpSensorInfo *pSensorInfo);


/**
*@brief 获取帧计数
*@param[in] handle 相机句柄
*@param[out] pFrameCount 帧计数
*@return 状态码
*@ingroup __DVP_INFORMATION__
*/
dvp2api dvpStatus dvpGetFrameCount(dvpHandle handle, dvpFrameCount *pFrameCount);


/**
*@brief 获取用户IO有效标志
*@param[in] handle 相机句柄
*@param[out] pUserIoInfo 用户IO有效标志
*@return 状态码
*@ingroup __DVP_INFORMATION__
*/
dvp2api dvpStatus dvpGetUserIoInfo(dvpHandle handle, dvpUserIoInfo *pUserIoInfo);


/**
*@brief 获取第一个像素对应的位置
*@param[in] handle 相机句柄
*@param[out] pFirstPosition 第一个像素对应的位置
*@return 状态码
*@ingroup __DVP_INFORMATION__
*/
dvp2api dvpStatus dvpGetFirstPosition(dvpHandle handle, dvpFirstPosition *pFirstPosition);


/**
*@brief 获取功能信息
*@param[in] handle 相机句柄
*@param[out] pFunctionInfo 功能信息
*@return 状态码
*@ingroup __DVP_INFORMATION__
*/
dvp2api dvpStatus dvpGetFunctionInfo(dvpHandle handle, dvpFunctionInfo *pFunctionInfo);


/**
*@brief 发送软件触发信号
*@param[in] handle 相机句柄
*@return 状态码
*@ingroup __DVP_TRIGGER__
*/
dvp2api dvpStatus dvpTriggerFire(dvpHandle handle);


/**
*@brief 创建彩点信息
*@param[in] handle 相机句柄
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetDefectFixState
*@see dvpSetAutoDefectFixState
*/
dvp2api dvpStatus dvpCreateDefectFixInfo(dvpHandle handle);


/**
*@brief 创建平场信息
*@param[in] handle 相机句柄
*@return 状态码
*@ingroup __DVP_ADVANCED__
*/
dvp2api dvpStatus dvpCreateFlatFieldInfo(dvpHandle handle);


/**
*@brief 复位设备
*@param[in] handle 相机句柄
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@note 操作成功后自动恢复到复位前的设备状态
*/
dvp2api dvpStatus dvpResetDevice(dvpHandle handle);


/**
*@brief 快速重启视频流
*@param[in] handle 相机句柄
*@return 状态码
*@ingroup __DVP2_API_BASE__
*@see dvpHold
*/
dvp2api dvpStatus dvpRestart(dvpHandle handle);


/**
*@brief 快速暂停视频流
*@param[in] handle 相机句柄
*@return 状态码
*@ingroup __DVP2_API_BASE__
*@see dvpRestart
*/
dvp2api dvpStatus dvpHold(dvpHandle handle);


/**
*@brief 获取伽玛的值
*@param[in] handle 相机句柄
*@param[out] pGamma 伽玛的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetGammaState
*@see dvpGetGammaState
*@see dvpGetGammaDescr
*@see dvpSetGamma
*/
dvp2api dvpStatus dvpGetGamma(dvpHandle handle, dvpInt32 *pGamma);


/**
*@brief 设置伽玛的值
*@param[in] handle 相机句柄
*@param[in] Gamma 伽玛的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetGammaState
*@see dvpGetGammaState
*@see dvpGetGammaDescr
*@see dvpGetGamma
*/
dvp2api dvpStatus dvpSetGamma(dvpHandle handle, dvpInt32 Gamma);


/**
*@brief 获取伽玛的描述信息
*@param[in] handle 相机句柄
*@param[out] pGammaDescr 伽玛的描述信息
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetGammaState
*@see dvpGetGammaState
*@see dvpSetGamma
*@see dvpGetGamma
*/
dvp2api dvpStatus dvpGetGammaDescr(dvpHandle handle, dvpIntDescr *pGammaDescr);


/**
*@brief 获取伽玛功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pGammaState 伽玛功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetGamma
*@see dvpGetGamma
*@see dvpGetGammaDescr
*@see dvpSetGammaState
*/
dvp2api dvpStatus dvpGetGammaState(dvpHandle handle, bool *pGammaState);


/**
*@brief 设置伽玛功能的使能状态
*@param[in] handle 相机句柄
*@param[in] GammaState 伽玛功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetGamma
*@see dvpGetGamma
*@see dvpGetGammaDescr
*@see dvpGetGammaState
*/
dvp2api dvpStatus dvpSetGammaState(dvpHandle handle, bool GammaState);


/**
*@brief 获取对比度的值
*@param[in] handle 相机句柄
*@param[out] pContrast 对比度的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetContrastState
*@see dvpGetContrastState
*@see dvpGetContrastDescr
*@see dvpSetContrast
*/
dvp2api dvpStatus dvpGetContrast(dvpHandle handle, dvpInt32 *pContrast);


/**
*@brief 设置对比度的值
*@param[in] handle 相机句柄
*@param[in] Contrast 对比度的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetContrastState
*@see dvpGetContrastState
*@see dvpGetContrastDescr
*@see dvpGetContrast
*/
dvp2api dvpStatus dvpSetContrast(dvpHandle handle, dvpInt32 Contrast);


/**
*@brief 获取对比度的描述信息
*@param[in] handle 相机句柄
*@param[out] pContrastDescr 对比度的描述信息
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetContrastState
*@see dvpGetContrastState
*@see dvpSetContrast
*@see dvpGetContrast
*/
dvp2api dvpStatus dvpGetContrastDescr(dvpHandle handle, dvpIntDescr *pContrastDescr);


/**
*@brief 获取对比度功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pContrastState 对比度功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetContrast
*@see dvpGetContrast
*@see dvpGetContrastDescr
*@see dvpSetContrastState
*/
dvp2api dvpStatus dvpGetContrastState(dvpHandle handle, bool *pContrastState);


/**
*@brief 设置对比度功能的使能状态
*@param[in] handle 相机句柄
*@param[in] ContrastState 对比度功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetContrast
*@see dvpGetContrast
*@see dvpGetContrastDescr
*@see dvpGetContrastState
*/
dvp2api dvpStatus dvpSetContrastState(dvpHandle handle, bool ContrastState);


/**
*@brief 获取图像Roi(相对于整个传感器区域)
*@param[in] handle 相机句柄
*@param[out] pRoi 图像Roi(相对于整个传感器区域)
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpSetRoiState
*@see dvpGetRoiState
*@see dvpGetRoiDescr
*@see dvpSetRoi
*/
dvp2api dvpStatus dvpGetRoi(dvpHandle handle, dvpRegion *pRoi);


/**
*@brief 设置图像Roi(相对于整个传感器区域)
*@param[in] handle 相机句柄
*@param[in] Roi 图像Roi(相对于整个传感器区域)
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpSetRoiState
*@see dvpGetRoiState
*@see dvpGetRoiDescr
*@see dvpGetRoi
*/
dvp2api dvpStatus dvpSetRoi(dvpHandle handle, dvpRegion Roi);


/**
*@brief 获取图像Roi(相对于整个传感器区域)的描述信息
*@param[in] handle 相机句柄
*@param[out] pRoiDescr 图像Roi(相对于整个传感器区域)的描述信息
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpSetRoiState
*@see dvpGetRoiState
*@see dvpSetRoi
*@see dvpGetRoi
*/
dvp2api dvpStatus dvpGetRoiDescr(dvpHandle handle, dvpRegionDescr *pRoiDescr);


/**
*@brief 获取图像Roi(相对于整个传感器区域)功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pRoiState 图像Roi(相对于整个传感器区域)功能的使能状态
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpSetRoi
*@see dvpGetRoi
*@see dvpGetRoiDescr
*@see dvpSetRoiState
*/
dvp2api dvpStatus dvpGetRoiState(dvpHandle handle, bool *pRoiState);


/**
*@brief 设置图像Roi(相对于整个传感器区域)功能的使能状态
*@param[in] handle 相机句柄
*@param[in] RoiState 图像Roi(相对于整个传感器区域)功能的使能状态
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpSetRoi
*@see dvpGetRoi
*@see dvpGetRoiDescr
*@see dvpGetRoiState
*/
dvp2api dvpStatus dvpSetRoiState(dvpHandle handle, bool RoiState);


/**
*@brief 获取色温的值（单位为K）
*@param[in] handle 相机句柄
*@param[out] pColorTemperature 色温的值（单位为K）
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetColorTemperatureState
*@see dvpGetColorTemperatureState
*@see dvpGetColorTemperatureDescr
*@see dvpSetColorTemperature
*/
dvp2api dvpStatus dvpGetColorTemperature(dvpHandle handle, dvpInt32 *pColorTemperature);


/**
*@brief 设置色温的值（单位为K）
*@param[in] handle 相机句柄
*@param[in] ColorTemperature 色温的值（单位为K）
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetColorTemperatureState
*@see dvpGetColorTemperatureState
*@see dvpGetColorTemperatureDescr
*@see dvpGetColorTemperature
*/
dvp2api dvpStatus dvpSetColorTemperature(dvpHandle handle, dvpInt32 ColorTemperature);


/**
*@brief 获取色温的描述信息
*@param[in] handle 相机句柄
*@param[out] pColorTemperatureDescr 色温的描述信息
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetColorTemperatureState
*@see dvpGetColorTemperatureState
*@see dvpSetColorTemperature
*@see dvpGetColorTemperature
*/
dvp2api dvpStatus dvpGetColorTemperatureDescr(dvpHandle handle, dvpIntDescr *pColorTemperatureDescr);


/**
*@brief 获取色温功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pColorTemperatureState 色温功能的使能状态
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetColorTemperature
*@see dvpGetColorTemperature
*@see dvpGetColorTemperatureDescr
*@see dvpSetColorTemperatureState
*/
dvp2api dvpStatus dvpGetColorTemperatureState(dvpHandle handle, bool *pColorTemperatureState);


/**
*@brief 设置色温功能的使能状态
*@param[in] handle 相机句柄
*@param[in] ColorTemperatureState 色温功能的使能状态
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetColorTemperature
*@see dvpGetColorTemperature
*@see dvpGetColorTemperatureDescr
*@see dvpGetColorTemperatureState
*/
dvp2api dvpStatus dvpSetColorTemperatureState(dvpHandle handle, bool ColorTemperatureState);


/**
*@brief 获取锐度的值
*@param[in] handle 相机句柄
*@param[out] pSharpness 锐度的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetSharpnessState
*@see dvpGetSharpnessState
*@see dvpGetSharpnessDescr
*@see dvpSetSharpness
*/
dvp2api dvpStatus dvpGetSharpness(dvpHandle handle, dvpInt32 *pSharpness);


/**
*@brief 设置锐度的值
*@param[in] handle 相机句柄
*@param[in] Sharpness 锐度的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetSharpnessState
*@see dvpGetSharpnessState
*@see dvpGetSharpnessDescr
*@see dvpGetSharpness
*/
dvp2api dvpStatus dvpSetSharpness(dvpHandle handle, dvpInt32 Sharpness);


/**
*@brief 获取锐度的描述信息
*@param[in] handle 相机句柄
*@param[out] pSharpnessDescr 锐度的描述信息
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetSharpnessState
*@see dvpGetSharpnessState
*@see dvpSetSharpness
*@see dvpGetSharpness
*/
dvp2api dvpStatus dvpGetSharpnessDescr(dvpHandle handle, dvpIntDescr *pSharpnessDescr);


/**
*@brief 获取锐度功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pSharpnessState 锐度功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetSharpness
*@see dvpGetSharpness
*@see dvpGetSharpnessDescr
*@see dvpSetSharpnessState
*/
dvp2api dvpStatus dvpGetSharpnessState(dvpHandle handle, bool *pSharpnessState);


/**
*@brief 设置锐度功能的使能状态
*@param[in] handle 相机句柄
*@param[in] SharpnessState 锐度功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetSharpness
*@see dvpGetSharpness
*@see dvpGetSharpnessDescr
*@see dvpGetSharpnessState
*/
dvp2api dvpStatus dvpSetSharpnessState(dvpHandle handle, bool SharpnessState);


/**
*@brief 获取饱和度的值
*@param[in] handle 相机句柄
*@param[out] pSaturation 饱和度的值
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetSaturationState
*@see dvpGetSaturationState
*@see dvpGetSaturationDescr
*@see dvpSetSaturation
*/
dvp2api dvpStatus dvpGetSaturation(dvpHandle handle, dvpInt32 *pSaturation);


/**
*@brief 设置饱和度的值
*@param[in] handle 相机句柄
*@param[in] Saturation 饱和度的值
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetSaturationState
*@see dvpGetSaturationState
*@see dvpGetSaturationDescr
*@see dvpGetSaturation
*/
dvp2api dvpStatus dvpSetSaturation(dvpHandle handle, dvpInt32 Saturation);


/**
*@brief 获取饱和度的描述信息
*@param[in] handle 相机句柄
*@param[out] pSaturationDescr 饱和度的描述信息
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetSaturationState
*@see dvpGetSaturationState
*@see dvpSetSaturation
*@see dvpGetSaturation
*/
dvp2api dvpStatus dvpGetSaturationDescr(dvpHandle handle, dvpIntDescr *pSaturationDescr);


/**
*@brief 获取饱和度功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pSaturationState 饱和度功能的使能状态
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetSaturation
*@see dvpGetSaturation
*@see dvpGetSaturationDescr
*@see dvpSetSaturationState
*/
dvp2api dvpStatus dvpGetSaturationState(dvpHandle handle, bool *pSaturationState);


/**
*@brief 设置饱和度功能的使能状态
*@param[in] handle 相机句柄
*@param[in] SaturationState 饱和度功能的使能状态
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetSaturation
*@see dvpGetSaturation
*@see dvpGetSaturationDescr
*@see dvpGetSaturationState
*/
dvp2api dvpStatus dvpSetSaturationState(dvpHandle handle, bool SaturationState);


/**
*@brief 获取2D降噪的值
*@param[in] handle 相机句柄
*@param[out] pNoiseReduct2d 2D降噪的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct2dState
*@see dvpGetNoiseReduct2dState
*@see dvpGetNoiseReduct2dDescr
*@see dvpSetNoiseReduct2d
*/
dvp2api dvpStatus dvpGetNoiseReduct2d(dvpHandle handle, dvpInt32 *pNoiseReduct2d);


/**
*@brief 设置2D降噪的值
*@param[in] handle 相机句柄
*@param[in] NoiseReduct2d 2D降噪的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct2dState
*@see dvpGetNoiseReduct2dState
*@see dvpGetNoiseReduct2dDescr
*@see dvpGetNoiseReduct2d
*/
dvp2api dvpStatus dvpSetNoiseReduct2d(dvpHandle handle, dvpInt32 NoiseReduct2d);


/**
*@brief 获取2D降噪的描述信息
*@param[in] handle 相机句柄
*@param[out] pNoiseReduct2dDescr 2D降噪的描述信息
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct2dState
*@see dvpGetNoiseReduct2dState
*@see dvpSetNoiseReduct2d
*@see dvpGetNoiseReduct2d
*/
dvp2api dvpStatus dvpGetNoiseReduct2dDescr(dvpHandle handle, dvpIntDescr *pNoiseReduct2dDescr);


/**
*@brief 获取2D降噪功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pNoiseReduct2dState 2D降噪功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct2d
*@see dvpGetNoiseReduct2d
*@see dvpGetNoiseReduct2dDescr
*@see dvpSetNoiseReduct2dState
*/
dvp2api dvpStatus dvpGetNoiseReduct2dState(dvpHandle handle, bool *pNoiseReduct2dState);


/**
*@brief 设置2D降噪功能的使能状态
*@param[in] handle 相机句柄
*@param[in] NoiseReduct2dState 2D降噪功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct2d
*@see dvpGetNoiseReduct2d
*@see dvpGetNoiseReduct2dDescr
*@see dvpGetNoiseReduct2dState
*/
dvp2api dvpStatus dvpSetNoiseReduct2dState(dvpHandle handle, bool NoiseReduct2dState);


/**
*@brief 获取3D降噪的值
*@param[in] handle 相机句柄
*@param[out] pNoiseReduct3d 3D降噪的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct3dState
*@see dvpGetNoiseReduct3dState
*@see dvpGetNoiseReduct3dDescr
*@see dvpSetNoiseReduct3d
*/
dvp2api dvpStatus dvpGetNoiseReduct3d(dvpHandle handle, dvpInt32 *pNoiseReduct3d);


/**
*@brief 设置3D降噪的值
*@param[in] handle 相机句柄
*@param[in] NoiseReduct3d 3D降噪的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct3dState
*@see dvpGetNoiseReduct3dState
*@see dvpGetNoiseReduct3dDescr
*@see dvpGetNoiseReduct3d
*/
dvp2api dvpStatus dvpSetNoiseReduct3d(dvpHandle handle, dvpInt32 NoiseReduct3d);


/**
*@brief 获取3D降噪的描述信息
*@param[in] handle 相机句柄
*@param[out] pNoiseReduct3dDescr 3D降噪的描述信息
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct3dState
*@see dvpGetNoiseReduct3dState
*@see dvpSetNoiseReduct3d
*@see dvpGetNoiseReduct3d
*/
dvp2api dvpStatus dvpGetNoiseReduct3dDescr(dvpHandle handle, dvpIntDescr *pNoiseReduct3dDescr);


/**
*@brief 获取3D降噪功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pNoiseReduct3dState 3D降噪功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct3d
*@see dvpGetNoiseReduct3d
*@see dvpGetNoiseReduct3dDescr
*@see dvpSetNoiseReduct3dState
*/
dvp2api dvpStatus dvpGetNoiseReduct3dState(dvpHandle handle, bool *pNoiseReduct3dState);


/**
*@brief 设置3D降噪功能的使能状态
*@param[in] handle 相机句柄
*@param[in] NoiseReduct3dState 3D降噪功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetNoiseReduct3d
*@see dvpGetNoiseReduct3d
*@see dvpGetNoiseReduct3dDescr
*@see dvpGetNoiseReduct3dState
*/
dvp2api dvpStatus dvpSetNoiseReduct3dState(dvpHandle handle, bool NoiseReduct3dState);


/**
*@brief 获取black level的值
*@param[in] handle 相机句柄
*@param[out] pBlackLevel black level的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetBlackLevelState
*@see dvpGetBlackLevelState
*@see dvpGetBlackLevelDescr
*@see dvpSetBlackLevel
*/
dvp2api dvpStatus dvpGetBlackLevel(dvpHandle handle, float *pBlackLevel);


/**
*@brief 设置black level的值
*@param[in] handle 相机句柄
*@param[in] BlackLevel black level的值
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetBlackLevelState
*@see dvpGetBlackLevelState
*@see dvpGetBlackLevelDescr
*@see dvpGetBlackLevel
*/
dvp2api dvpStatus dvpSetBlackLevel(dvpHandle handle, float BlackLevel);


/**
*@brief 获取black level的描述信息
*@param[in] handle 相机句柄
*@param[out] pBlackLevelDescr black level的描述信息
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetBlackLevelState
*@see dvpGetBlackLevelState
*@see dvpSetBlackLevel
*@see dvpGetBlackLevel
*/
dvp2api dvpStatus dvpGetBlackLevelDescr(dvpHandle handle, dvpFloatDescr *pBlackLevelDescr);


/**
*@brief 获取black level功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pBlackLevelState black level功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetBlackLevel
*@see dvpGetBlackLevel
*@see dvpGetBlackLevelDescr
*@see dvpSetBlackLevelState
*/
dvp2api dvpStatus dvpGetBlackLevelState(dvpHandle handle, bool *pBlackLevelState);


/**
*@brief 设置black level功能的使能状态
*@param[in] handle 相机句柄
*@param[in] BlackLevelState black level功能的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetBlackLevel
*@see dvpGetBlackLevel
*@see dvpGetBlackLevelDescr
*@see dvpGetBlackLevelState
*/
dvp2api dvpStatus dvpSetBlackLevelState(dvpHandle handle, bool BlackLevelState);


/**
*@brief 获取循环触发的周期值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pSoftTriggerLoop 循环触发的周期值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopDescr
*@see dvpSetSoftTriggerLoop
*/
dvp2api dvpStatus dvpGetSoftTriggerLoop(dvpHandle handle, double *pSoftTriggerLoop);


/**
*@brief 设置循环触发的周期值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] SoftTriggerLoop 循环触发的周期值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopDescr
*@see dvpGetSoftTriggerLoop
*/
dvp2api dvpStatus dvpSetSoftTriggerLoop(dvpHandle handle, double SoftTriggerLoop);


/**
*@brief 获取循环触发的描述信息
*@param[in] handle 相机句柄
*@param[out] pSoftTriggerLoopDescr 循环触发的描述信息
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopState
*@see dvpSetSoftTriggerLoop
*@see dvpGetSoftTriggerLoop
*/
dvp2api dvpStatus dvpGetSoftTriggerLoopDescr(dvpHandle handle, dvpDoubleDescr *pSoftTriggerLoopDescr);


/**
*@brief 获取循环触发功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pSoftTriggerLoopState 循环触发功能的使能状态
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetSoftTriggerLoop
*@see dvpGetSoftTriggerLoop
*@see dvpGetSoftTriggerLoopDescr
*@see dvpSetSoftTriggerLoopState
*/
dvp2api dvpStatus dvpGetSoftTriggerLoopState(dvpHandle handle, bool *pSoftTriggerLoopState);


/**
*@brief 设置循环触发功能的使能状态
*@param[in] handle 相机句柄
*@param[in] SoftTriggerLoopState 循环触发功能的使能状态
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetSoftTriggerLoop
*@see dvpGetSoftTriggerLoop
*@see dvpGetSoftTriggerLoopDescr
*@see dvpGetSoftTriggerLoopState
*/
dvp2api dvpStatus dvpSetSoftTriggerLoopState(dvpHandle handle, bool SoftTriggerLoopState);


/**
*@brief 获取数据传输包间隔的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pStreamPackInterval 数据传输包间隔的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetStreamPackIntervalDescr
*@see dvpSetStreamPackInterval
*/
dvp2api dvpStatus dvpGetStreamPackInterval(dvpHandle handle, dvpInt32 *pStreamPackInterval);


/**
*@brief 设置数据传输包间隔的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] StreamPackInterval 数据传输包间隔的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetStreamPackIntervalDescr
*@see dvpGetStreamPackInterval
*/
dvp2api dvpStatus dvpSetStreamPackInterval(dvpHandle handle, dvpInt32 StreamPackInterval);


/**
*@brief 获取数据传输包间隔的描述信息
*@param[in] handle 相机句柄
*@param[out] pStreamPackIntervalDescr 数据传输包间隔的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetStreamPackInterval
*@see dvpGetStreamPackInterval
*/
dvp2api dvpStatus dvpGetStreamPackIntervalDescr(dvpHandle handle, dvpIntDescr *pStreamPackIntervalDescr);


/**
*@brief 获取数据传输包长的值（单位为字节）
*@param[in] handle 相机句柄
*@param[out] pStreamPackSize 数据传输包长的值（单位为字节）
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetStreamPackSizeDescr
*@see dvpSetStreamPackSize
*/
dvp2api dvpStatus dvpGetStreamPackSize(dvpHandle handle, dvpInt32 *pStreamPackSize);


/**
*@brief 设置数据传输包长的值（单位为字节）
*@param[in] handle 相机句柄
*@param[in] StreamPackSize 数据传输包长的值（单位为字节）
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetStreamPackSizeDescr
*@see dvpGetStreamPackSize
*/
dvp2api dvpStatus dvpSetStreamPackSize(dvpHandle handle, dvpInt32 StreamPackSize);


/**
*@brief 获取数据传输包长的描述信息
*@param[in] handle 相机句柄
*@param[out] pStreamPackSizeDescr 数据传输包长的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetStreamPackSize
*@see dvpGetStreamPackSize
*/
dvp2api dvpStatus dvpGetStreamPackSizeDescr(dvpHandle handle, dvpIntDescr *pStreamPackSizeDescr);


/**
*@brief 获取自动曝光目标亮度的值
*@param[in] handle 相机句柄
*@param[out] pAeTarget 自动曝光目标亮度的值
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAeTargetDescr
*@see dvpSetAeTarget
*/
dvp2api dvpStatus dvpGetAeTarget(dvpHandle handle, dvpInt32 *pAeTarget);


/**
*@brief 设置自动曝光目标亮度的值
*@param[in] handle 相机句柄
*@param[in] AeTarget 自动曝光目标亮度的值
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAeTargetDescr
*@see dvpGetAeTarget
*/
dvp2api dvpStatus dvpSetAeTarget(dvpHandle handle, dvpInt32 AeTarget);


/**
*@brief 获取自动曝光目标亮度的描述信息
*@param[in] handle 相机句柄
*@param[out] pAeTargetDescr 自动曝光目标亮度的描述信息
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpSetAeTarget
*@see dvpGetAeTarget
*/
dvp2api dvpStatus dvpGetAeTargetDescr(dvpHandle handle, dvpIntDescr *pAeTargetDescr);


/**
*@brief 获取模拟增益的值
*@param[in] handle 相机句柄
*@param[out] pAnalogGain 模拟增益的值
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetExposure
*@see dvpGetAnalogGainDescr
*@see dvpSetAnalogGain
*/
dvp2api dvpStatus dvpGetAnalogGain(dvpHandle handle, float *pAnalogGain);


/**
*@brief 设置模拟增益的值
*@param[in] handle 相机句柄
*@param[in] AnalogGain 模拟增益的值
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetExposure
*@see dvpGetAnalogGainDescr
*@see dvpGetAnalogGain
*/
dvp2api dvpStatus dvpSetAnalogGain(dvpHandle handle, float AnalogGain);


/**
*@brief 获取模拟增益的描述信息
*@param[in] handle 相机句柄
*@param[out] pAnalogGainDescr 模拟增益的描述信息
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetExposure
*@see dvpSetAnalogGain
*@see dvpGetAnalogGain
*/
dvp2api dvpStatus dvpGetAnalogGainDescr(dvpHandle handle, dvpFloatDescr *pAnalogGainDescr);


/**
*@brief 获取曝光时间的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pExposure 曝光时间的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAnalogGain
*@see dvpGetExposureDescr
*@see dvpSetExposure
*/
dvp2api dvpStatus dvpGetExposure(dvpHandle handle, double *pExposure);


/**
*@brief 设置曝光时间的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] Exposure 曝光时间的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAnalogGain
*@see dvpGetExposureDescr
*@see dvpGetExposure
*/
dvp2api dvpStatus dvpSetExposure(dvpHandle handle, double Exposure);


/**
*@brief 获取曝光时间的描述信息
*@param[in] handle 相机句柄
*@param[out] pExposureDescr 曝光时间的描述信息
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAnalogGain
*@see dvpSetExposure
*@see dvpGetExposure
*/
dvp2api dvpStatus dvpGetExposureDescr(dvpHandle handle, dvpDoubleDescr *pExposureDescr);


/**
*@brief 获取外部触发信号消抖时间的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pTriggerJitterFilter 外部触发信号消抖时间的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetTriggerJitterFilterDescr
*@see dvpSetTriggerJitterFilter
*/
dvp2api dvpStatus dvpGetTriggerJitterFilter(dvpHandle handle, double *pTriggerJitterFilter);


/**
*@brief 设置外部触发信号消抖时间的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] TriggerJitterFilter 外部触发信号消抖时间的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetTriggerJitterFilterDescr
*@see dvpGetTriggerJitterFilter
*/
dvp2api dvpStatus dvpSetTriggerJitterFilter(dvpHandle handle, double TriggerJitterFilter);


/**
*@brief 获取外部触发信号消抖时间的描述信息
*@param[in] handle 相机句柄
*@param[out] pTriggerJitterFilterDescr 外部触发信号消抖时间的描述信息
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetTriggerJitterFilter
*@see dvpGetTriggerJitterFilter
*/
dvp2api dvpStatus dvpGetTriggerJitterFilterDescr(dvpHandle handle, dvpDoubleDescr *pTriggerJitterFilterDescr);


/**
*@brief 获取触发延时的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pTriggerDelay 触发延时的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetTriggerDelayDescr
*@see dvpSetTriggerDelay
*/
dvp2api dvpStatus dvpGetTriggerDelay(dvpHandle handle, double *pTriggerDelay);


/**
*@brief 设置触发延时的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] TriggerDelay 触发延时的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetTriggerDelayDescr
*@see dvpGetTriggerDelay
*/
dvp2api dvpStatus dvpSetTriggerDelay(dvpHandle handle, double TriggerDelay);


/**
*@brief 获取触发延时的描述信息
*@param[in] handle 相机句柄
*@param[out] pTriggerDelayDescr 触发延时的描述信息
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetTriggerDelay
*@see dvpGetTriggerDelay
*/
dvp2api dvpStatus dvpGetTriggerDelayDescr(dvpHandle handle, dvpDoubleDescr *pTriggerDelayDescr);


/**
*@brief 获取Strobe信号的延时的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pStrobeDelay Strobe信号的延时的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetStrobeDelayDescr
*@see dvpSetStrobeDelay
*/
dvp2api dvpStatus dvpGetStrobeDelay(dvpHandle handle, double *pStrobeDelay);


/**
*@brief 设置Strobe信号的延时的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] StrobeDelay Strobe信号的延时的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetStrobeDelayDescr
*@see dvpGetStrobeDelay
*/
dvp2api dvpStatus dvpSetStrobeDelay(dvpHandle handle, double StrobeDelay);


/**
*@brief 获取Strobe信号的延时的描述信息
*@param[in] handle 相机句柄
*@param[out] pStrobeDelayDescr Strobe信号的延时的描述信息
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetStrobeDelay
*@see dvpGetStrobeDelay
*/
dvp2api dvpStatus dvpGetStrobeDelayDescr(dvpHandle handle, dvpDoubleDescr *pStrobeDelayDescr);


/**
*@brief 获取Strobe信号的持续时间的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pStrobeDuration Strobe信号的持续时间的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetStrobeDurationDescr
*@see dvpSetStrobeDuration
*/
dvp2api dvpStatus dvpGetStrobeDuration(dvpHandle handle, double *pStrobeDuration);


/**
*@brief 设置Strobe信号的持续时间的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] StrobeDuration Strobe信号的持续时间的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetStrobeDurationDescr
*@see dvpGetStrobeDuration
*/
dvp2api dvpStatus dvpSetStrobeDuration(dvpHandle handle, double StrobeDuration);


/**
*@brief 获取Strobe信号的持续时间的描述信息
*@param[in] handle 相机句柄
*@param[out] pStrobeDurationDescr Strobe信号的持续时间的描述信息
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetStrobeDuration
*@see dvpGetStrobeDuration
*/
dvp2api dvpStatus dvpGetStrobeDurationDescr(dvpHandle handle, dvpDoubleDescr *pStrobeDurationDescr);


/**
*@brief 获取单次触发的输出帧数
*@param[in] handle 相机句柄
*@param[out] pFramesPerTrigger 单次触发的输出帧数
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetFramesPerTriggerDescr
*@see dvpSetFramesPerTrigger
*/
dvp2api dvpStatus dvpGetFramesPerTrigger(dvpHandle handle, dvpInt32 *pFramesPerTrigger);


/**
*@brief 设置单次触发的输出帧数
*@param[in] handle 相机句柄
*@param[in] FramesPerTrigger 单次触发的输出帧数
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetFramesPerTriggerDescr
*@see dvpGetFramesPerTrigger
*/
dvp2api dvpStatus dvpSetFramesPerTrigger(dvpHandle handle, dvpInt32 FramesPerTrigger);


/**
*@brief 获取单次触发的输出帧数的描述信息
*@param[in] handle 相机句柄
*@param[out] pFramesPerTriggerDescr 单次触发的输出帧数的描述信息
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetFramesPerTrigger
*@see dvpGetFramesPerTrigger
*/
dvp2api dvpStatus dvpGetFramesPerTriggerDescr(dvpHandle handle, dvpIntDescr *pFramesPerTriggerDescr);


/**
*@brief 获取设备中计时器的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[out] pTimerValue 设备中计时器的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetTimerValueDescr
*@see dvpSetTimerValue
*/
dvp2api dvpStatus dvpGetTimerValue(dvpHandle handle, double *pTimerValue);


/**
*@brief 设置设备中计时器的值（单位为微秒）
*@param[in] handle 相机句柄
*@param[in] TimerValue 设备中计时器的值（单位为微秒）
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetTimerValueDescr
*@see dvpGetTimerValue
*/
dvp2api dvpStatus dvpSetTimerValue(dvpHandle handle, double TimerValue);


/**
*@brief 获取设备中计时器的描述信息
*@param[in] handle 相机句柄
*@param[out] pTimerValueDescr 设备中计时器的描述信息
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpSetTimerValue
*@see dvpGetTimerValue
*/
dvp2api dvpStatus dvpGetTimerValueDescr(dvpHandle handle, dvpDoubleDescr *pTimerValueDescr);


/**
*@brief 获取当前用户设置
*@param[in] handle 相机句柄
*@param[out] pUserSet 当前用户设置
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetFunctionInfo
*@note 即上电初始设置
*@see dvpSetUserSet
*/
dvp2api dvpStatus dvpGetUserSet(dvpHandle handle, dvpUserSet *pUserSet);


/**
*@brief 设置当前用户设置
*@param[in] handle 相机句柄
*@param[in] UserSet 当前用户设置
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetFunctionInfo
*@note 即上电初始设置
*@see dvpGetUserSet
*/
dvp2api dvpStatus dvpSetUserSet(dvpHandle handle, dvpUserSet UserSet);


/**
*@brief 获取LUT曲线
*@param[in] handle 相机句柄
*@param[out] pCurveLut LUT曲线
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetCurveLut
*/
dvp2api dvpStatus dvpGetCurveLut(dvpHandle handle, dvpCurveLut *pCurveLut);


/**
*@brief 设置LUT曲线
*@param[in] handle 相机句柄
*@param[in] CurveLut LUT曲线
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetCurveLut
*/
dvp2api dvpStatus dvpSetCurveLut(dvpHandle handle, dvpCurveLut CurveLut);


/**
*@brief 获取颜色校正数据
*@param[in] handle 相机句柄
*@param[out] pColorCorrection 颜色校正数据
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetColorCorrection
*/
dvp2api dvpStatus dvpGetColorCorrection(dvpHandle handle, dvpColorCorrection *pColorCorrection);


/**
*@brief 设置颜色校正数据
*@param[in] handle 相机句柄
*@param[in] ColorCorrection 颜色校正数据
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetColorCorrection
*/
dvp2api dvpStatus dvpSetColorCorrection(dvpHandle handle, dvpColorCorrection ColorCorrection);


/**
*@brief 获取视频流状态
*@param[in] handle 相机句柄
*@param[out] pStreamState 视频流状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetStreamState
*/
dvp2api dvpStatus dvpGetStreamState(dvpHandle handle, dvpStreamState *pStreamState);


/**
*@brief 设置视频流状态
*@param[in] handle 相机句柄
*@param[in] StreamState 视频流状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetStreamState
*/
dvp2api dvpStatus dvpSetStreamState(dvpHandle handle, dvpStreamState StreamState);


/**
*@brief 获取相机内置硬件ISP的使能状态
*@param[in] handle 相机句柄
*@param[out] pHardwareIspState 相机内置硬件ISP的使能状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetFunctionInfo
*@see dvpSetHardwareIspState
*/
dvp2api dvpStatus dvpGetHardwareIspState(dvpHandle handle, bool *pHardwareIspState);


/**
*@brief 设置相机内置硬件ISP的使能状态
*@param[in] handle 相机句柄
*@param[in] HardwareIspState 相机内置硬件ISP的使能状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetFunctionInfo
*@see dvpGetHardwareIspState
*/
dvp2api dvpStatus dvpSetHardwareIspState(dvpHandle handle, bool HardwareIspState);


/**
*@brief 获取触发模式的使能状态
*@param[in] handle 相机句柄
*@param[out] pTriggerState 触发模式的使能状态
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetFunctionInfo
*@see dvpSetTriggerState
*/
dvp2api dvpStatus dvpGetTriggerState(dvpHandle handle, bool *pTriggerState);


/**
*@brief 设置触发模式的使能状态
*@param[in] handle 相机句柄
*@param[in] TriggerState 触发模式的使能状态
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetFunctionInfo
*@see dvpGetTriggerState
*/
dvp2api dvpStatus dvpSetTriggerState(dvpHandle handle, bool TriggerState);


/**
*@brief 获取去色的使能状态
*@param[in] handle 相机句柄
*@param[out] pMonoState 去色的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetMonoState
*/
dvp2api dvpStatus dvpGetMonoState(dvpHandle handle, bool *pMonoState);


/**
*@brief 设置去色的使能状态
*@param[in] handle 相机句柄
*@param[in] MonoState 去色的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetMonoState
*/
dvp2api dvpStatus dvpSetMonoState(dvpHandle handle, bool MonoState);


/**
*@brief 获取负片的使能状态
*@param[in] handle 相机句柄
*@param[out] pInverseState 负片的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetInverseState
*/
dvp2api dvpStatus dvpGetInverseState(dvpHandle handle, bool *pInverseState);


/**
*@brief 设置负片的使能状态
*@param[in] handle 相机句柄
*@param[in] InverseState 负片的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetInverseState
*/
dvp2api dvpStatus dvpSetInverseState(dvpHandle handle, bool InverseState);


/**
*@brief 获取横向翻转的使能状态
*@param[in] handle 相机句柄
*@param[out] pFlipHorizontalState 横向翻转的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetFlipHorizontalState
*/
dvp2api dvpStatus dvpGetFlipHorizontalState(dvpHandle handle, bool *pFlipHorizontalState);


/**
*@brief 设置横向翻转的使能状态
*@param[in] handle 相机句柄
*@param[in] FlipHorizontalState 横向翻转的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetFlipHorizontalState
*/
dvp2api dvpStatus dvpSetFlipHorizontalState(dvpHandle handle, bool FlipHorizontalState);


/**
*@brief 获取纵向翻转的使能状态
*@param[in] handle 相机句柄
*@param[out] pFlipVerticalState 纵向翻转的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetFlipVerticalState
*/
dvp2api dvpStatus dvpGetFlipVerticalState(dvpHandle handle, bool *pFlipVerticalState);


/**
*@brief 设置纵向翻转的使能状态
*@param[in] handle 相机句柄
*@param[in] FlipVerticalState 纵向翻转的使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetFlipVerticalState
*/
dvp2api dvpStatus dvpSetFlipVerticalState(dvpHandle handle, bool FlipVerticalState);


/**
*@brief 获取平场的使能状态
*@param[in] handle 相机句柄
*@param[out] pFlatFieldState 平场的使能状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetFlatFieldState
*/
dvp2api dvpStatus dvpGetFlatFieldState(dvpHandle handle, bool *pFlatFieldState);


/**
*@brief 设置平场的使能状态
*@param[in] handle 相机句柄
*@param[in] FlatFieldState 平场的使能状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetFlatFieldState
*/
dvp2api dvpStatus dvpSetFlatFieldState(dvpHandle handle, bool FlatFieldState);


/**
*@brief 获取消除彩点的使能状态
*@param[in] handle 相机句柄
*@param[out] pDefectFixState 消除彩点的使能状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpCreateDefectFixInfo
*@see dvpSetAutoDefectFixState
*@see dvpSetDefectFixState
*/
dvp2api dvpStatus dvpGetDefectFixState(dvpHandle handle, bool *pDefectFixState);


/**
*@brief 设置消除彩点的使能状态
*@param[in] handle 相机句柄
*@param[in] DefectFixState 消除彩点的使能状态
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpCreateDefectFixInfo
*@see dvpSetAutoDefectFixState
*@see dvpGetDefectFixState
*/
dvp2api dvpStatus dvpSetDefectFixState(dvpHandle handle, bool DefectFixState);


/**
*@brief 获取自动消除彩点的使能状态，true为自动，false为手动
*@param[in] handle 相机句柄
*@param[out] pAutoDefectFixState 自动消除彩点的使能状态，true为自动，false为手动
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpCreateDefectFixInfo
*@see dvpSetDefectFixState
*@see dvpSetAutoDefectFixState
*/
dvp2api dvpStatus dvpGetAutoDefectFixState(dvpHandle handle, bool *pAutoDefectFixState);


/**
*@brief 设置自动消除彩点的使能状态，true为自动，false为手动
*@param[in] handle 相机句柄
*@param[in] AutoDefectFixState 自动消除彩点的使能状态，true为自动，false为手动
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpCreateDefectFixInfo
*@see dvpSetDefectFixState
*@see dvpGetAutoDefectFixState
*/
dvp2api dvpStatus dvpSetAutoDefectFixState(dvpHandle handle, bool AutoDefectFixState);


/**
*@brief 获取图像旋转使能状态
*@param[in] handle 相机句柄
*@param[out] pRotateState 图像旋转使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetRotateOpposite
*@see dvpSetRotateState
*/
dvp2api dvpStatus dvpGetRotateState(dvpHandle handle, bool *pRotateState);


/**
*@brief 设置图像旋转使能状态
*@param[in] handle 相机句柄
*@param[in] RotateState 图像旋转使能状态
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetRotateOpposite
*@see dvpGetRotateState
*/
dvp2api dvpStatus dvpSetRotateState(dvpHandle handle, bool RotateState);


/**
*@brief 获取图像旋转方向的标志（顺时针 true 或逆时针 false 90度）
*@see dvpGetRotateState
*@param[in] handle 相机句柄
*@param[out] pRotateOpposite 图像旋转方向的标志（顺时针 true 或逆时针 false 90度）
*@see dvpGetRotateState
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetRotateOpposite
*/
dvp2api dvpStatus dvpGetRotateOpposite(dvpHandle handle, bool *pRotateOpposite);


/**
*@brief 设置图像旋转方向的标志（顺时针 true 或逆时针 false 90度）
*@see dvpGetRotateState
*@param[in] handle 相机句柄
*@param[in] RotateOpposite 图像旋转方向的标志（顺时针 true 或逆时针 false 90度）
*@see dvpGetRotateState
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetRotateOpposite
*/
dvp2api dvpStatus dvpSetRotateOpposite(dvpHandle handle, bool RotateOpposite);


/**
*@brief 获取自动曝光统计区域（相对于整个可见区域）
*@param[in] handle 相机句柄
*@param[out] pAeRoi 自动曝光统计区域（相对于整个可见区域）
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpSetAeRoi
*/
dvp2api dvpStatus dvpGetAeRoi(dvpHandle handle, dvpRegion *pAeRoi);


/**
*@brief 设置自动曝光统计区域（相对于整个可见区域）
*@param[in] handle 相机句柄
*@param[in] AeRoi 自动曝光统计区域（相对于整个可见区域）
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAeRoi
*/
dvp2api dvpStatus dvpSetAeRoi(dvpHandle handle, dvpRegion AeRoi);


/**
*@brief 获取自动白平衡统计区域（相对于整个可见区域）
*@param[in] handle 相机句柄
*@param[out] pAwbRoi 自动白平衡统计区域（相对于整个可见区域）
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetAwbRoi
*/
dvp2api dvpStatus dvpGetAwbRoi(dvpHandle handle, dvpRegion *pAwbRoi);


/**
*@brief 设置自动白平衡统计区域（相对于整个可见区域）
*@param[in] handle 相机句柄
*@param[in] AwbRoi 自动白平衡统计区域（相对于整个可见区域）
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpGetAwbRoi
*/
dvp2api dvpStatus dvpSetAwbRoi(dvpHandle handle, dvpRegion AwbRoi);


/**
*@brief 获取自动曝光模式
*@param[in] handle 相机句柄
*@param[out] pAeMode 自动曝光模式
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpSetAeMode
*/
dvp2api dvpStatus dvpGetAeMode(dvpHandle handle, dvpAeMode *pAeMode);


/**
*@brief 设置自动曝光模式
*@param[in] handle 相机句柄
*@param[in] AeMode 自动曝光模式
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAeMode
*/
dvp2api dvpStatus dvpSetAeMode(dvpHandle handle, dvpAeMode AeMode);


/**
*@brief 获取抗频闪状态
*@param[in] handle 相机句柄
*@param[out] pAntiFlick 抗频闪状态
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpSetAntiFlick
*/
dvp2api dvpStatus dvpGetAntiFlick(dvpHandle handle, dvpAntiFlick *pAntiFlick);


/**
*@brief 设置抗频闪状态
*@param[in] handle 相机句柄
*@param[in] AntiFlick 抗频闪状态
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAntiFlick
*/
dvp2api dvpStatus dvpSetAntiFlick(dvpHandle handle, dvpAntiFlick AntiFlick);


/**
*@brief 获取自动曝光的操作方式
*@param[in] handle 相机句柄
*@param[out] pAeOperation 自动曝光的操作方式
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAeConfig
*@see dvpGetAeTarget
*@see dvpSetAeOperation
*/
dvp2api dvpStatus dvpGetAeOperation(dvpHandle handle, dvpAeOperation *pAeOperation);


/**
*@brief 设置自动曝光的操作方式
*@param[in] handle 相机句柄
*@param[in] AeOperation 自动曝光的操作方式
*@return 状态码
*@ingroup __DVP_EXPOSURE__
*@see dvpGetAeConfig
*@see dvpGetAeTarget
*@see dvpGetAeOperation
*/
dvp2api dvpStatus dvpSetAeOperation(dvpHandle handle, dvpAeOperation AeOperation);


/**
*@brief 获取自动白平衡的操作方式
*@param[in] handle 相机句柄
*@param[out] pAwbOperation 自动白平衡的操作方式
*@return 状态码
*@ingroup __DVP_COLOR__
*@note 在执行白平衡操作后，须自行@link #dvpSetRgbGain 设置RGB增益@endlink为1.0，并且@link dvpSetColorTemperatureState 禁用色温效果@endlink才能得到纯正的白平衡
*@see dvpSetAwbOperation
*/
dvp2api dvpStatus dvpGetAwbOperation(dvpHandle handle, dvpAwbOperation *pAwbOperation);


/**
*@brief 设置自动白平衡的操作方式
*@param[in] handle 相机句柄
*@param[in] AwbOperation 自动白平衡的操作方式
*@return 状态码
*@ingroup __DVP_COLOR__
*@note 在执行白平衡操作后，须自行@link #dvpSetRgbGain 设置RGB增益@endlink为1.0，并且@link dvpSetColorTemperatureState 禁用色温效果@endlink才能得到纯正的白平衡
*@see dvpGetAwbOperation
*/
dvp2api dvpStatus dvpSetAwbOperation(dvpHandle handle, dvpAwbOperation AwbOperation);


/**
*@brief 获取Strobe信号驱动方式
*@param[in] handle 相机句柄
*@param[out] pStrobeDriver Strobe信号驱动方式
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetStrobeDriver
*/
dvp2api dvpStatus dvpGetStrobeDriver(dvpHandle handle, dvpStrobeDriver *pStrobeDriver);


/**
*@brief 设置Strobe信号驱动方式
*@param[in] handle 相机句柄
*@param[in] StrobeDriver Strobe信号驱动方式
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetStrobeDriver
*/
dvp2api dvpStatus dvpSetStrobeDriver(dvpHandle handle, dvpStrobeDriver StrobeDriver);


/**
*@brief 获取Strobe信号类型
*@param[in] handle 相机句柄
*@param[out] pStrobeOutputType Strobe信号类型
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetStrobeOutputType
*/
dvp2api dvpStatus dvpGetStrobeOutputType(dvpHandle handle, dvpStrobeOutputType *pStrobeOutputType);


/**
*@brief 设置Strobe信号类型
*@param[in] handle 相机句柄
*@param[in] StrobeOutputType Strobe信号类型
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetStrobeOutputType
*/
dvp2api dvpStatus dvpSetStrobeOutputType(dvpHandle handle, dvpStrobeOutputType StrobeOutputType);


/**
*@brief 获取触发输入信号类型
*@param[in] handle 相机句柄
*@param[out] pTriggerInputType 触发输入信号类型
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetTriggerInputType
*/
dvp2api dvpStatus dvpGetTriggerInputType(dvpHandle handle, dvpTriggerInputType *pTriggerInputType);


/**
*@brief 设置触发输入信号类型
*@param[in] handle 相机句柄
*@param[in] TriggerInputType 触发输入信号类型
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetTriggerInputType
*/
dvp2api dvpStatus dvpSetTriggerInputType(dvpHandle handle, dvpTriggerInputType TriggerInputType);


/**
*@brief 获取源图像格式
*@param[in] handle 相机句柄
*@param[out] pSourceFormat 源图像格式
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetSourceFormat
*/
dvp2api dvpStatus dvpGetSourceFormat(dvpHandle handle, dvpStreamFormat *pSourceFormat);


/**
*@brief 设置源图像格式
*@param[in] handle 相机句柄
*@param[in] SourceFormat 源图像格式
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetSourceFormat
*/
dvp2api dvpStatus dvpSetSourceFormat(dvpHandle handle, dvpStreamFormat SourceFormat);


/**
*@brief 获取目标图像格式
*@param[in] handle 相机句柄
*@param[out] pTargetFormat 目标图像格式
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpSetTargetFormat
*/
dvp2api dvpStatus dvpGetTargetFormat(dvpHandle handle, dvpStreamFormat *pTargetFormat);


/**
*@brief 设置目标图像格式
*@param[in] handle 相机句柄
*@param[in] TargetFormat 目标图像格式
*@return 状态码
*@ingroup __DVP_ENHANCE__
*@see dvpGetTargetFormat
*/
dvp2api dvpStatus dvpSetTargetFormat(dvpHandle handle, dvpStreamFormat TargetFormat);


/**
*@brief 获取用户颜色矩阵
*@param[in] handle 相机句柄
*@param[out] pUserColorMatrix 用户颜色矩阵
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpSetUserColorMatrix
*@note MAT[0][0] + MAT[1][0] + MAT[2][0] 恒等于 1.0
*@note MAT[0][1] + MAT[1][1] + MAT[2][1] 恒等于 1.0
*@note MAT[0][2] + MAT[1][2] + MAT[2][2] 恒等于 1.0
*@note 输入颜色 [B0.G0.R0]
*@note 输出颜色 [B1.G1.R1]
*@note B1 = B0*MAT[0][0] + G0*MAT[1][0] + R0*MAT[2][0] + MAT[3][0]
*@note G1 = B0*MAT[0][1] + G0*MAT[1][1] + R0*MAT[2][1] + MAT[3][1]
*@note R1 = B0*MAT[0][2] + G0*MAT[1][2] + R0*MAT[2][2] + MAT[3][2]
*/
dvp2api dvpStatus dvpGetUserColorMatrix(dvpHandle handle, dvpColorMatrix *pUserColorMatrix);


/**
*@brief 设置用户颜色矩阵
*@param[in] handle 相机句柄
*@param[in] UserColorMatrix 用户颜色矩阵
*@return 状态码
*@ingroup __DVP_COLOR__
*@see dvpGetUserColorMatrix
*@note MAT[0][0] + MAT[1][0] + MAT[2][0] 恒等于 1.0
*@note MAT[0][1] + MAT[1][1] + MAT[2][1] 恒等于 1.0
*@note MAT[0][2] + MAT[1][2] + MAT[2][2] 恒等于 1.0
*@note 输入颜色 [B0.G0.R0]
*@note 输出颜色 [B1.G1.R1]
*@note B1 = B0*MAT[0][0] + G0*MAT[1][0] + R0*MAT[2][0] + MAT[3][0]
*@note G1 = B0*MAT[0][1] + G0*MAT[1][1] + R0*MAT[2][1] + MAT[3][1]
*@note R1 = B0*MAT[0][2] + G0*MAT[1][2] + R0*MAT[2][2] + MAT[3][2]
*/
dvp2api dvpStatus dvpSetUserColorMatrix(dvpHandle handle, dvpColorMatrix UserColorMatrix);


/**
*@brief 获取连接超时时间的值 （单位为毫秒）
*@param[in] handle 相机句柄
*@param[out] pLinkTimeout 连接超时时间的值 （单位为毫秒）
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpSetLinkTimeout
*/
dvp2api dvpStatus dvpGetLinkTimeout(dvpHandle handle, dvpUint32 *pLinkTimeout);


/**
*@brief 设置连接超时时间的值 （单位为毫秒）
*@param[in] handle 相机句柄
*@param[in] LinkTimeout 连接超时时间的值 （单位为毫秒）
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetLinkTimeout
*/
dvp2api dvpStatus dvpSetLinkTimeout(dvpHandle handle, dvpUint32 LinkTimeout);


/**
*@brief 获取自动曝光配置信息
*@param[in] handle 相机句柄
*@param[out] pAeConfig 自动曝光配置信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAeOperation
*@see dvpGetAeTarget
*@see dvpSetAeConfig
*/
dvp2api dvpStatus dvpGetAeConfig(dvpHandle handle, dvpAeConfig *pAeConfig);


/**
*@brief 设置自动曝光配置信息
*@param[in] handle 相机句柄
*@param[in] AeConfig 自动曝光配置信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAeOperation
*@see dvpGetAeTarget
*@see dvpGetAeConfig
*/
dvp2api dvpStatus dvpSetAeConfig(dvpHandle handle, dvpAeConfig AeConfig);


/**
*@brief 获取输入IO的电平状态
*@param[in] handle 相机句柄
*@param[in] inputIo 输入IO
*@param[out] pInputIoLevel 输入IO的电平状态
*@return 状态码
*@ingroup __DVP_TRIGGER__
*/
dvp2api dvpStatus dvpGetInputIoLevel(dvpHandle handle, dvpInputIo inputIo, bool *pInputIoLevel);


/**
*@brief 获取输出IO的电平状态
*@param[in] handle 相机句柄
*@param[in] outputIo 输出IO
*@param[out] pOutputIoLevel 输出IO的电平状态
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetOutputIoLevel
*/
dvp2api dvpStatus dvpGetOutputIoLevel(dvpHandle handle, dvpOutputIo outputIo, bool *pOutputIoLevel);


/**
*@brief 设置输出IO的电平状态
*@param[in] handle 相机句柄
*@param[in] outputIo 输出IO
*@param[in] OutputIoLevel 输出IO的电平状态
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetOutputIoLevel
*/
dvp2api dvpStatus dvpSetOutputIoLevel(dvpHandle handle, dvpOutputIo outputIo, bool OutputIoLevel);


/**
*@brief 获取输出IO的功能配置
*@param[in] handle 相机句柄
*@param[in] outputIo 输出IO
*@param[out] pOutputIoFunction 输出IO的功能配置
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetOutputIoFunction
*/
dvp2api dvpStatus dvpGetOutputIoFunction(dvpHandle handle, dvpOutputIo outputIo, dvpOutputIoFunction *pOutputIoFunction);


/**
*@brief 设置输出IO的功能配置
*@param[in] handle 相机句柄
*@param[in] outputIo 输出IO
*@param[in] OutputIoFunction 输出IO的功能配置
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetOutputIoFunction
*/
dvp2api dvpStatus dvpSetOutputIoFunction(dvpHandle handle, dvpOutputIo outputIo, dvpOutputIoFunction OutputIoFunction);


/**
*@brief 获取输入IO的功能配置
*@param[in] handle 相机句柄
*@param[in] inputIo 输入IO
*@param[out] pInputIoFunction 输入IO的功能配置
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpSetInputIoFunction
*/
dvp2api dvpStatus dvpGetInputIoFunction(dvpHandle handle, dvpInputIo inputIo, dvpInputIoFunction *pInputIoFunction);


/**
*@brief 设置输入IO的功能配置
*@param[in] handle 相机句柄
*@param[in] inputIo 输入IO
*@param[in] InputIoFunction 输入IO的功能配置
*@return 状态码
*@ingroup __DVP_TRIGGER__
*@see dvpGetInputIoFunction
*/
dvp2api dvpStatus dvpSetInputIoFunction(dvpHandle handle, dvpInputIo inputIo, dvpInputIoFunction InputIoFunction);


/**
*@brief 获取当前色彩方案选项的索引
*@param[in] handle 相机句柄
*@param[out] pColorSolutionSel 当前色彩方案选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetColorSolutionSelDescr
*@see dvpGetColorSolutionSelDetail
*@see dvpSetColorSolutionSel
*/
dvp2api dvpStatus dvpGetColorSolutionSel(dvpHandle handle, dvpUint32 *pColorSolutionSel);


/**
*@brief 设置当前色彩方案选项的索引
*@param[in] handle 相机句柄
*@param[in] ColorSolutionSel 当前色彩方案选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetColorSolutionSelDescr
*@see dvpGetColorSolutionSelDetail
*@see dvpGetColorSolutionSel
*/
dvp2api dvpStatus dvpSetColorSolutionSel(dvpHandle handle, dvpUint32 ColorSolutionSel);


/**
*@brief 获取色彩方案选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pColorSolutionSelDescr 色彩方案选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetColorSolutionSel
*@see dvpGetColorSolutionSelDetail
*/
dvp2api dvpStatus dvpGetColorSolutionSelDescr(dvpHandle handle, dvpSelectionDescr *pColorSolutionSelDescr);


/**
*@brief 获取当前色彩方案选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] ColorSolutionSel 色彩方案的索引
*@param[out] pColorSolutionSelDetail 当前色彩方案选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetColorSolutionSel
*@see dvpGetColorSolutionSelDescr
*/
dvp2api dvpStatus dvpGetColorSolutionSelDetail(dvpHandle handle, dvpUint32 ColorSolutionSel, dvpSelection *pColorSolutionSelDetail);


/**
*@brief 获取当前Bayer转RGB解码算法选项的索引
*@param[in] handle 相机句柄
*@param[out] pBayerDecodeSel 当前Bayer转RGB解码算法选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetBayerDecodeSelDescr
*@see dvpGetBayerDecodeSelDetail
*@see dvpSetBayerDecodeSel
*/
dvp2api dvpStatus dvpGetBayerDecodeSel(dvpHandle handle, dvpUint32 *pBayerDecodeSel);


/**
*@brief 设置当前Bayer转RGB解码算法选项的索引
*@param[in] handle 相机句柄
*@param[in] BayerDecodeSel 当前Bayer转RGB解码算法选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetBayerDecodeSelDescr
*@see dvpGetBayerDecodeSelDetail
*@see dvpGetBayerDecodeSel
*/
dvp2api dvpStatus dvpSetBayerDecodeSel(dvpHandle handle, dvpUint32 BayerDecodeSel);


/**
*@brief 获取Bayer转RGB解码算法选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pBayerDecodeSelDescr Bayer转RGB解码算法选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetBayerDecodeSel
*@see dvpGetBayerDecodeSelDetail
*/
dvp2api dvpStatus dvpGetBayerDecodeSelDescr(dvpHandle handle, dvpSelectionDescr *pBayerDecodeSelDescr);


/**
*@brief 获取当前Bayer转RGB解码算法选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] BayerDecodeSel Bayer转RGB解码算法的索引
*@param[out] pBayerDecodeSelDetail 当前Bayer转RGB解码算法选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetBayerDecodeSel
*@see dvpGetBayerDecodeSelDescr
*/
dvp2api dvpStatus dvpGetBayerDecodeSelDetail(dvpHandle handle, dvpUint32 BayerDecodeSel, dvpSelection *pBayerDecodeSelDetail);


/**
*@brief 获取当前原始图像格式选项的索引
*@param[in] handle 相机句柄
*@param[out] pSourceFormatSel 当前原始图像格式选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetSourceFormatSelDescr
*@see dvpGetSourceFormatSelDetail
*@see dvpSetSourceFormatSel
*/
dvp2api dvpStatus dvpGetSourceFormatSel(dvpHandle handle, dvpUint32 *pSourceFormatSel);


/**
*@brief 设置当前原始图像格式选项的索引
*@param[in] handle 相机句柄
*@param[in] SourceFormatSel 当前原始图像格式选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetSourceFormatSelDescr
*@see dvpGetSourceFormatSelDetail
*@see dvpGetSourceFormatSel
*/
dvp2api dvpStatus dvpSetSourceFormatSel(dvpHandle handle, dvpUint32 SourceFormatSel);


/**
*@brief 获取原始图像格式选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pSourceFormatSelDescr 原始图像格式选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetSourceFormatSel
*@see dvpGetSourceFormatSelDetail
*/
dvp2api dvpStatus dvpGetSourceFormatSelDescr(dvpHandle handle, dvpSelectionDescr *pSourceFormatSelDescr);


/**
*@brief 获取当前原始图像格式选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] SourceFormatSel 原始图像格式的索引
*@param[out] pSourceFormatSelDetail 当前原始图像格式选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetSourceFormatSel
*@see dvpGetSourceFormatSelDescr
*/
dvp2api dvpStatus dvpGetSourceFormatSelDetail(dvpHandle handle, dvpUint32 SourceFormatSel, dvpFormatSelection *pSourceFormatSelDetail);


/**
*@brief 获取当前输出图像格式选项的索引
*@param[in] handle 相机句柄
*@param[out] pTargetFormatSel 当前输出图像格式选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetTargetFormatSelDescr
*@see dvpGetTargetFormatSelDetail
*@see dvpSetTargetFormatSel
*/
dvp2api dvpStatus dvpGetTargetFormatSel(dvpHandle handle, dvpUint32 *pTargetFormatSel);


/**
*@brief 设置当前输出图像格式选项的索引
*@param[in] handle 相机句柄
*@param[in] TargetFormatSel 当前输出图像格式选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetTargetFormatSelDescr
*@see dvpGetTargetFormatSelDetail
*@see dvpGetTargetFormatSel
*/
dvp2api dvpStatus dvpSetTargetFormatSel(dvpHandle handle, dvpUint32 TargetFormatSel);


/**
*@brief 获取输出图像格式选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pTargetFormatSelDescr 输出图像格式选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetTargetFormatSel
*@see dvpGetTargetFormatSelDetail
*/
dvp2api dvpStatus dvpGetTargetFormatSelDescr(dvpHandle handle, dvpSelectionDescr *pTargetFormatSelDescr);


/**
*@brief 获取当前输出图像格式选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] TargetFormatSel 输出图像格式的索引
*@param[out] pTargetFormatSelDetail 当前输出图像格式选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetTargetFormatSel
*@see dvpGetTargetFormatSelDescr
*/
dvp2api dvpStatus dvpGetTargetFormatSelDetail(dvpHandle handle, dvpUint32 TargetFormatSel, dvpFormatSelection *pTargetFormatSelDetail);


/**
*@brief 获取当前分辨率模式选项的索引
*@param[in] handle 相机句柄
*@param[out] pResolutionModeSel 当前分辨率模式选项的索引
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetResolutionModeSelDescr
*@see dvpGetResolutionModeSelDetail
*@see dvpSetResolutionModeSel
*/
dvp2api dvpStatus dvpGetResolutionModeSel(dvpHandle handle, dvpUint32 *pResolutionModeSel);


/**
*@brief 设置当前分辨率模式选项的索引
*@param[in] handle 相机句柄
*@param[in] ResolutionModeSel 当前分辨率模式选项的索引
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetResolutionModeSelDescr
*@see dvpGetResolutionModeSelDetail
*@see dvpGetResolutionModeSel
*/
dvp2api dvpStatus dvpSetResolutionModeSel(dvpHandle handle, dvpUint32 ResolutionModeSel);


/**
*@brief 获取分辨率模式选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pResolutionModeSelDescr 分辨率模式选项索引的描述信息
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetResolutionModeSel
*@see dvpGetResolutionModeSelDetail
*/
dvp2api dvpStatus dvpGetResolutionModeSelDescr(dvpHandle handle, dvpSelectionDescr *pResolutionModeSelDescr);


/**
*@brief 获取当前分辨率模式选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] ResolutionModeSel 分辨率模式的索引
*@param[out] pResolutionModeSelDetail 当前分辨率模式选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetResolutionModeSel
*@see dvpGetResolutionModeSelDescr
*/
dvp2api dvpStatus dvpGetResolutionModeSelDetail(dvpHandle handle, dvpUint32 ResolutionModeSel, dvpResolutionMode *pResolutionModeSelDetail);


/**
*@brief 获取当前自动曝光方案选项的索引
*@param[in] handle 相机句柄
*@param[out] pAeSchemeSel 当前自动曝光方案选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAeSchemeSelDescr
*@see dvpGetAeSchemeSelDetail
*@see dvpSetAeSchemeSel
*/
dvp2api dvpStatus dvpGetAeSchemeSel(dvpHandle handle, dvpUint32 *pAeSchemeSel);


/**
*@brief 设置当前自动曝光方案选项的索引
*@param[in] handle 相机句柄
*@param[in] AeSchemeSel 当前自动曝光方案选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAeSchemeSelDescr
*@see dvpGetAeSchemeSelDetail
*@see dvpGetAeSchemeSel
*/
dvp2api dvpStatus dvpSetAeSchemeSel(dvpHandle handle, dvpUint32 AeSchemeSel);


/**
*@brief 获取自动曝光方案选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pAeSchemeSelDescr 自动曝光方案选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAeSchemeSel
*@see dvpGetAeSchemeSelDetail
*/
dvp2api dvpStatus dvpGetAeSchemeSelDescr(dvpHandle handle, dvpSelectionDescr *pAeSchemeSelDescr);


/**
*@brief 获取当前自动曝光方案选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] AeSchemeSel 自动曝光方案的索引
*@param[out] pAeSchemeSelDetail 当前自动曝光方案选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAeSchemeSel
*@see dvpGetAeSchemeSelDescr
*/
dvp2api dvpStatus dvpGetAeSchemeSelDetail(dvpHandle handle, dvpUint32 AeSchemeSel, dvpSelection *pAeSchemeSelDetail);


/**
*@brief 获取当前快速Roi选项的索引
*@param[in] handle 相机句柄
*@param[out] pQuickRoiSel 当前快速Roi选项的索引
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetQuickRoiSelDescr
*@see dvpGetQuickRoiSelDetail
*@see dvpSetQuickRoiSel
*/
dvp2api dvpStatus dvpGetQuickRoiSel(dvpHandle handle, dvpUint32 *pQuickRoiSel);


/**
*@brief 设置当前快速Roi选项的索引
*@param[in] handle 相机句柄
*@param[in] QuickRoiSel 当前快速Roi选项的索引
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetQuickRoiSelDescr
*@see dvpGetQuickRoiSelDetail
*@see dvpGetQuickRoiSel
*/
dvp2api dvpStatus dvpSetQuickRoiSel(dvpHandle handle, dvpUint32 QuickRoiSel);


/**
*@brief 获取快速Roi选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pQuickRoiSelDescr 快速Roi选项索引的描述信息
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetQuickRoiSel
*@see dvpGetQuickRoiSelDetail
*/
dvp2api dvpStatus dvpGetQuickRoiSelDescr(dvpHandle handle, dvpSelectionDescr *pQuickRoiSelDescr);


/**
*@brief 获取当前快速Roi选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] QuickRoiSel 快速Roi的索引
*@param[out] pQuickRoiSelDetail 当前快速Roi选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_SIZE__
*@see dvpGetQuickRoiSel
*@see dvpGetQuickRoiSelDescr
*/
dvp2api dvpStatus dvpGetQuickRoiSelDetail(dvpHandle handle, dvpUint32 QuickRoiSel, dvpQuickRoi *pQuickRoiSelDetail);


/**
*@brief 获取当前像素采集速率选项的索引
*@param[in] handle 相机句柄
*@param[out] pPixelRateSel 当前像素采集速率选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPixelRateSelDescr
*@see dvpGetPixelRateSelDetail
*@see dvpSetPixelRateSel
*/
dvp2api dvpStatus dvpGetPixelRateSel(dvpHandle handle, dvpUint32 *pPixelRateSel);


/**
*@brief 设置当前像素采集速率选项的索引
*@param[in] handle 相机句柄
*@param[in] PixelRateSel 当前像素采集速率选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPixelRateSelDescr
*@see dvpGetPixelRateSelDetail
*@see dvpGetPixelRateSel
*/
dvp2api dvpStatus dvpSetPixelRateSel(dvpHandle handle, dvpUint32 PixelRateSel);


/**
*@brief 获取像素采集速率选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pPixelRateSelDescr 像素采集速率选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPixelRateSel
*@see dvpGetPixelRateSelDetail
*/
dvp2api dvpStatus dvpGetPixelRateSelDescr(dvpHandle handle, dvpSelectionDescr *pPixelRateSelDescr);


/**
*@brief 获取当前像素采集速率选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] PixelRateSel 像素采集速率的索引
*@param[out] pPixelRateSelDetail 当前像素采集速率选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPixelRateSel
*@see dvpGetPixelRateSelDescr
*/
dvp2api dvpStatus dvpGetPixelRateSelDetail(dvpHandle handle, dvpUint32 PixelRateSel, dvpSelection *pPixelRateSelDetail);


/**
*@brief 获取当前典型的数据传输包长选项的索引
*@param[in] handle 相机句柄
*@param[out] pPacketSizeSel 当前典型的数据传输包长选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPacketSizeSelDescr
*@see dvpGetPacketSizeSelDetail
*@see dvpSetPacketSizeSel
*/
dvp2api dvpStatus dvpGetPacketSizeSel(dvpHandle handle, dvpUint32 *pPacketSizeSel);


/**
*@brief 设置当前典型的数据传输包长选项的索引
*@param[in] handle 相机句柄
*@param[in] PacketSizeSel 当前典型的数据传输包长选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPacketSizeSelDescr
*@see dvpGetPacketSizeSelDetail
*@see dvpGetPacketSizeSel
*/
dvp2api dvpStatus dvpSetPacketSizeSel(dvpHandle handle, dvpUint32 PacketSizeSel);


/**
*@brief 获取典型的数据传输包长选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pPacketSizeSelDescr 典型的数据传输包长选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPacketSizeSel
*@see dvpGetPacketSizeSelDetail
*/
dvp2api dvpStatus dvpGetPacketSizeSelDescr(dvpHandle handle, dvpSelectionDescr *pPacketSizeSelDescr);


/**
*@brief 获取当前典型的数据传输包长选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] PacketSizeSel 典型的数据传输包长的索引
*@param[out] pPacketSizeSelDetail 当前典型的数据传输包长选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetPacketSizeSel
*@see dvpGetPacketSizeSelDescr
*/
dvp2api dvpStatus dvpGetPacketSizeSelDetail(dvpHandle handle, dvpUint32 PacketSizeSel, dvpSelection *pPacketSizeSelDetail);


/**
*@brief 获取当前硬件加速器选项的索引
*@param[in] handle 相机句柄
*@param[out] pAccelerateSel 当前硬件加速器选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAccelerateSelDescr
*@see dvpGetAccelerateSelDetail
*@see dvpSetAccelerateSel
*/
dvp2api dvpStatus dvpGetAccelerateSel(dvpHandle handle, dvpUint32 *pAccelerateSel);


/**
*@brief 设置当前硬件加速器选项的索引
*@param[in] handle 相机句柄
*@param[in] AccelerateSel 当前硬件加速器选项的索引
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAccelerateSelDescr
*@see dvpGetAccelerateSelDetail
*@see dvpGetAccelerateSel
*/
dvp2api dvpStatus dvpSetAccelerateSel(dvpHandle handle, dvpUint32 AccelerateSel);


/**
*@brief 获取硬件加速器选项索引的描述信息
*@param[in] handle 相机句柄
*@param[out] pAccelerateSelDescr 硬件加速器选项索引的描述信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAccelerateSel
*@see dvpGetAccelerateSelDetail
*/
dvp2api dvpStatus dvpGetAccelerateSelDescr(dvpHandle handle, dvpSelectionDescr *pAccelerateSelDescr);


/**
*@brief 获取当前硬件加速器选项索引对应的具体信息
*@param[in] handle 相机句柄
*@param[in] AccelerateSel 硬件加速器的索引
*@param[out] pAccelerateSelDetail 当前硬件加速器选项索引对应的具体信息
*@return 状态码
*@ingroup __DVP_ADVANCED__
*@see dvpGetAccelerateSel
*@see dvpGetAccelerateSelDescr
*/
dvp2api dvpStatus dvpGetAccelerateSelDetail(dvpHandle handle, dvpUint32 AccelerateSel, dvpSelection *pAccelerateSelDetail);


/**
*@brief 设置红,绿,蓝三色的数字增益
*@param[in] handle 相机句柄
*@param[in] rGain 红色数字增益
*@param[in] gGain 绿色数字增益
*@param[in] bGain 蓝色数字增益
*@ingroup __DVP_COLOR__
*@see dvpGetRgbGain
*@see dvpSetRgbGainState
*@see dvpGetRgbGainState
*@note 此增益附加在@link #dvpSetAwbOperation 白平衡@endlink的效果之上，但两者彼此独立
*/
dvp2api dvpStatus dvpSetRgbGain(dvpHandle handle, float rGain, float gGain, float bGain);


/**
*@brief 获取红,绿,蓝三色的数字增益
*@param[in] handle 相机句柄
*@param[out] prGain 红色数字增益
*@param[out] pgGain 绿色数字增益
*@param[out] pbGain 蓝色数字增益
*@ingroup __DVP_COLOR__
*@see dvpSetRgbGain
*@see dvpSetRgbGainState
*@see dvpGetRgbGainState
*/
dvp2api dvpStatus dvpGetRgbGain(dvpHandle handle, float *prGain, float *pgGain, float *pbGain);


/**
*@brief 设置红,绿,蓝三色的数字增益功能的使能状态
*@param[in] handle 相机句柄
*@param[in] state 红,绿,蓝三色的数字增益功能的使能状态
*@ingroup __DVP_COLOR__
*@see dvpSetRgbGain
*@see dvpGetRgbGain
*@see dvpGetRgbGainState
*/
dvp2api dvpStatus dvpSetRgbGainState(dvpHandle handle, bool state);


/**
*@brief 获取红,绿,蓝三色的数字增益功能的使能状态
*@param[in] handle 相机句柄
*@param[out] pState 红,绿,蓝三色的数字增益功能的使能状态
*@ingroup __DVP_COLOR__
*@see dvpSetRgbGain
*@see dvpGetRgbGain
*@see dvpSetRgbGainState
*/
dvp2api dvpStatus dvpGetRgbGainState(dvpHandle handle, bool *pState);



/** @defgroup __DVP2_API_BASE__ 基本功能
*@{
*/
/**
*@brief 刷新已连接的相机列表，并获得相机的数量
*@param[out] pCount 相机总数
*@see dvpEnum
*@see dvpOpen
*@see dvpOpenByName
*@note 应该在@link #dvpEnum 枚举@endlink或@link #dvpOpen 打开@endlink相机前执行（并且同在一个线程）
*/
dvp2api dvpStatus dvpRefresh(dvpUint32 *pCount);


/**
*@brief 根据索引枚举相机信息
*@param[in] index 相机索引
*@param[out] pCameraInfo 相机信息
*@see dvpGetCameraInfo
*@code
dvpUint32 count;
dvpRefresh(&count);
for (dvpUint32 i = 0; i < count; i++)
{
    dvpCameraInfo info;
    if(dvpEnum(i, &info) == DVP_STATUS_OK)
    {
        printf("Camera FriendlyName : %s", info.FriendlyName);
    }
}
*@endcode
*@note 应该在@link #dvpRefresh 刷新@endlink相机后执行（并且同在一个线程）
*/
dvp2api dvpStatus dvpEnum(dvpUint32 index, dvpCameraInfo *pCameraInfo);


/**
*@brief 根据@link #dvpCameraInfo::FriendlyName 友好名称@endlink来打开相机
*@param[in] friendlyName 友好名称
*@param[in] mode 打开方式
*@param[out] pHandle 相机句柄
*@see dvpEnum
*@see dvpOpen
*@see dvpOpenByUserId
*@see dvpGetCameraInfo
*@note 此操作会自动@link #dvpRefresh 刷新@endlink相机总数
*@warning 切勿在@link #dvpStreamCallback 回调函数@endlink中使用此功能
*/
dvp2api dvpStatus dvpOpenByName(dvpStr friendlyName, dvpOpenMode mode, dvpHandle *pHandle);


/**
*@brief 根据@link #dvpCameraInfo::UserID (User Defined Name) 用户命名@endlink来打开相机
*@param[in] UserId 用户命名
*@param[in] mode 打开方式
*@param[out] pHandle 相机句柄
*@see dvpSetUserId
*@see dvpEnum
*@see dvpOpen
*@see dvpOpenByName
*@see dvpGetCameraInfo
*@note 此操作会自动@link #dvpRefresh 刷新@endlink相机总数
*@warning 切勿在@link #dvpStreamCallback 回调函数@endlink中使用此功能
*/
dvp2api dvpStatus dvpOpenByUserId(dvpStr UserId, dvpOpenMode mode, dvpHandle *pHandle);


/**
*@brief 根据索引来打开相机
*@param[in] index 相机索引
*@param[in] mode 打开方式
*@param[out] pHandle 相机句柄
*@see dvpRefresh
*@see dvpEnum
*@see dvpOpenByName
*@see dvpGetCameraInfo
*@note 应该在@link #dvpRefresh 刷新@endlink相机后执行（并且同在一个线程）
*@warning 切勿在@link #dvpStreamCallback 回调函数@endlink中使用此功能
*/
dvp2api dvpStatus dvpOpen(dvpUint32 index, dvpOpenMode mode, dvpHandle *pHandle);


/**
*@brief 检查相机句柄是否有效
*@param[in] handle 相机句柄
*@param[out] pIsValid 有效状态
*@note @link #dvpOpen 打开@endlink成功则句柄有效，@link #dvpClose 关闭@endlink成功则句柄失效
*/
dvp2api dvpStatus dvpIsValid(dvpHandle handle, bool *pIsValid);


/**
*@brief 检查相机是否联机
*@param[in] handle 相机句柄
*@param[out] pIsOnline 联机状态
*/
dvp2api dvpStatus dvpIsOnline(dvpHandle handle, bool *pIsOnline);


/**
*@brief 启动视频流
*@param[in] handle 相机句柄
*@see dvpOpen
*@see dvpStop
*/
dvp2api dvpStatus dvpStart(dvpHandle handle);


/**
*@brief 停止视频流
*@param[in] handle 相机句柄
*@see dvpStart
*@see dvpClose
*/
dvp2api dvpStatus dvpStop(dvpHandle handle);


/**
*@brief 查询视频流是否处于暂停状态
*@param[in] handle 相机句柄
*@param[out] pIsHold 暂停状态
*@see dvpHold
*@see dvpRestart
*/
dvp2api dvpStatus dvpIsHold(dvpHandle handle, bool *pIsHold);


/**
*@brief 获取一帧图像
*@param[in] handle 相机句柄
*@param[out] pFrame 帧信息
*@param[out] pBuffer 图像数据的内存首地址 @warning 切勿手动释放
*@param[in] timeout 超时时间（毫秒）
*@see dvpGetFrameBuffer
*@see dvpStreamCallback
*@note 须先@link #dvpStart 播放@endlink视频流，才能使用此功能
*@warning 由本函数获得的图像数据缓存，在下一次调用本函数时，将变得无效，因此，在多线程同时获得图像的应用中，需要做好线程同步或临界段保护
*/
dvp2api dvpStatus dvpGetFrame(dvpHandle handle, dvpFrame *pFrame, void **pBuffer, dvpUint32 timeout);


/**
*@brief 同时获取时间戳和帧ID相同的原始采集帧和目标输出帧
*@param[in] handle 相机句柄
*@param[out] pRaw 原始采集帧
*@param[out] pOut 目标输出帧
*@param[in] timeout 超时时间（毫秒）
*@see dvpGetFrame
*@note 须先@link #dvpStart 播放@endlink视频流，才能使用此功能
*@warning 由本函数获得的图像数据缓存，在下一次调用本函数时，将变得无效，因此，在多线程同时获得图像的应用中，需要做好线程同步或临界段保护
*/
dvp2api dvpStatus dvpGetFrameBuffer(dvpHandle handle, dvpFrameBuffer *pRaw, dvpFrameBuffer *pOut, dvpUint32 timeout);


/**
*@brief 在用户线程中对源图像进行（ISP）处理，得到目标图像
*@param[in] handle 相机句柄
*@param[in] pSourceFrame 源帧信息
*@note 通常由@link #dvpGetFrame dvpGetFrame@endlink 所得 
*@param[in] pSourceBuffer 源帧图像数据的内存首地址
*@note 通常由@link #dvpGetFrame dvpGetFrame@endlink 所得
*@param[out] pTargetFrame 目标帧信息
*@param[out] pTargetBuffer 内存首地址。须由用户自行申请和释放
*@param[in] targetBufferSize 内存字节数。须大于或等于目标图像的数据量
*@param[in] targetFormat 目标图像格式
*@see dvpGetFrame
*@note 须先@link #dvpStart 播放@endlink视频流，才能使用此功能
*@warning 如果在@link #dvpStreamCallback 视频流回调函数@endlink中使用此功能，则只能注册为@link #dvpStreamEvent::STREAM_EVENT_FRAME_THREAD STREAM_EVENT_FRAME_THREAD@endlink事件类型
*/
dvp2api dvpStatus dvpProcessImage(dvpHandle handle, const dvpFrame *pSourceFrame, const void *pSourceBuffer, dvpFrame *pTargetFrame, void *pTargetBuffer, dvpUint32 targetBufferSize, dvpStreamFormat targetFormat);


/**
*@brief 关闭相机
*@param[in] handle 相机句柄
*@see dvpStop
*@warning 切勿在@link #dvpStreamCallback 回调函数@endlink中使用此功能
*@warning 关闭相机后，相机句柄将不再可用
*/
dvp2api dvpStatus dvpClose(dvpHandle handle);



/** @defgroup __DVP2_API_CALLBACK__ 回调函数
*@{
*/
/** @brief 视频流回调函数
*@param[in] handle 相机句柄
*@param[in] event 事件类型
*@param[in] pContext 用户指针
*@param[in] pFrame 帧信息
*@param[in] pBuffer 图像数据
*@see dvpRegisterStreamCallback
*@see dvpUnregisterStreamCallback
*@note 可以在回调函数中，处理，@link #dvpDrawPicture 显示@endlink与@link #dvpSavePicture 存储@endlink图像
*@warning 切勿在回调函数中，@link #dvpOpen 打开@endlink，@link #dvpClose 关闭@endlink，@link #dvpStart 播放@endlink，@link #dvpStop 停止@endlink相机，以及@link #dvpGetFrame 抓取图像@endlink  */
typedef dvpInt32(dvpStreamCallback)(dvpHandle handle, dvpStreamEvent event, void *pContext, dvpFrame *pFrame, void *pBuffer);

/**
*@brief 注册视频流回调函数
*@param[in] handle 相机句柄
*@param[in] proc 回调函数的函数指针
*@param[in] event 回调函数的事件类型
*@param[in] pContext 回调函数的用户指针
*@see dvpUnregisterStreamCallback
*/
dvp2api dvpStatus dvpRegisterStreamCallback(dvpHandle handle, dvpStreamCallback proc, dvpStreamEvent event, void *pContext);


/**
*@brief 注销视频流回调函数
*@param[in] handle 相机句柄
*@param[in] proc 回调函数的函数指针
*@param[in] event 回调函数的事件类型
*@param[in] pContext 回调函数的用户指针
*@note 须与@link #dvpRegisterStreamCallback 注册回调函数@endlink所使用的参数一致才能注销对应的回调函数
*/
dvp2api dvpStatus dvpUnregisterStreamCallback(dvpHandle handle, dvpStreamCallback proc, dvpStreamEvent event, void *pContext);


/** @brief 事件通知回调函数
*@param[in] handle 相机句柄
*@param[in] event 事件类型
*@param[in] pContext 用户指针
*@param[in] param 轻量级事件附加数据
*@param[in] pVariant 事件附加数据
*@see dvpRegisterEventCallback */
typedef dvpInt32(dvpEventCallback)(dvpHandle handle, dvpEvent event, void *pContext, dvpInt32 param, struct dvpVariant *pVariant);

/**
*@brief 注册事件通知的回调函数
*@param[in] handle 相机句柄
*@param[in] proc 回调函数的函数指针
*@param[in] event 回调函数的事件类型
*@param[in] pContext 回调函数的用户指针
*@see dvpUnregisterEventCallback
*/
dvp2api dvpStatus dvpRegisterEventCallback(dvpHandle handle, dvpEventCallback proc, dvpEvent event, void *pContext);


/**
*@brief 注销事件通知的回调函数
*@param[in] handle 相机句柄
*@param[in] proc 回调函数的函数指针
*@param[in] event 回调函数的事件类型
*@param[in] pContext 回调函数的用户指针
*@note 须与@link #dvpRegisterEventCallback 注册回调函数@endlink所使用的参数一致才能注销对应的回调函数
*/
dvp2api dvpStatus dvpUnregisterEventCallback(dvpHandle handle, dvpEventCallback proc, dvpEvent event, void *pContext);


/** @} end of __DVP2_API_CALLBACK__ */

/** @defgroup __DVP2_API_CONFIG__ 存档读档
*@{
*/
/**
*@brief 加载默认设置
*@param[in] handle 相机句柄
*@see dvpLoadConfig
*@see dvpSaveConfig
*@warning 会覆盖当前设置，如有必要，请事先@link #dvpSaveConfig 备份当前设置@endlink到配置文件
*/
dvp2api dvpStatus dvpLoadDefault(dvpHandle handle);


/**
*@brief 从配置文件中加载设置
*@param[in] handle 相机句柄
*@param[in] path 配置文件的路径／文件名（自动选择路径）／空指针（自动选择路径和文件名）
*@see dvpLoadDefault
*@see dvpSaveConfig
*@warning 会覆盖当前设置，如有必要，请事先@link #dvpSaveConfig 备份当前设置@endlink到配置文件
*/
dvp2api dvpStatus dvpLoadConfig(dvpHandle handle, dvpStr path);


/**
*@brief 当前设置保存到配置文件
*@param[in] handle 相机句柄
*@param[in] path 配置文件的路径／文件名（自动选择路径）／空指针（自动选择路径和文件名）
*@see dvpLoadDefault
*@see dvpLoadConfig
*@warning 须确保当前进程拥有对此文件的写入权限
*/
dvp2api dvpStatus dvpSaveConfig(dvpHandle handle, dvpStr path);


/**
*@brief 保存用户设置到相机存储器
*@param[in] handle 相机句柄
*@param[in] UserSet 目标用户设置
*@see dvpLoadUserSet
*@see dvpGetUserSet
*@warning 不可保存为@link #dvpUserSet::USER_SET_DEFAULT 默认用户设置@endlink
*/
dvp2api dvpStatus dvpSaveUserSet(dvpHandle handle, dvpUserSet UserSet);


/**
*@brief 从相机存储器加载用户设置
*@param[in] handle 相机句柄
*@param[in] UserSet 目标用户设置
*@see dvpSaveUserSet
*@see dvpGetUserSet
*@note 可以从@link #dvpUserSet::USER_SET_DEFAULT 默认用户设置@endlink加载
*/
dvp2api dvpStatus dvpLoadUserSet(dvpHandle handle, dvpUserSet UserSet);


/** @} end of __DVP2_API_CONFIG__ */
/** @} end of __DVP2_API_BASE__ */

/** @defgroup __DVP2_API_EXTEND__ 实用功能
*@{
*/
/**
*@brief 将图像保存到文件
*@param[in] pFrame 帧信息
*@param[in] pBuffer 图像数据的内存首地址（目前支持的数据格式包括：RAW, RGB24）
*@param[in] file 文件名或完整路径（图像文件的格式由文件的扩展名来决定，目前支持的图像格式包括：bmp, jpeg, jpg, png, tiff, tif, gif, dat(纯图像数据)）
*@param[in] quality 图像品质，仅对jpeg格式有效，并且取值范围是[1,100]
*@see dvpGetFrame
*@see dvpStreamCallback
*/
dvp2api dvpStatus dvpSavePicture(const dvpFrame *pFrame, const void *pBuffer, dvpStr file, dvpInt32 quality);


/**
*@brief 根据指定分辨率抓取并保存图像
*@param[in] handle 相机句柄
*@param[in] ResoulutionModeSel 分辨率模式@link #dvpGetResolutionModeSel 索引@endlink
*@param[in] roi @link #dvpGetRoi ROI@endlink指针，如果为NULL指针，则是此分辨率模式下的最大宽高
*@param[in] timeout @link #dvpGetFrame 采集@endlink超时时间
*@param[in] filePath 文件名或完整路径（图像文件的格式由文件的扩展名来决定，目前支持的图像格式包括：bmp, jpeg, jpg, png, tiff, tif, gif, dat(纯图像数据)）
*@param[in] quality 图像品质，仅对jpeg格式有效，并且取值范围是[1,100]
*@see dvpSavePicture
*/
dvp2api dvpStatus dvpCaptureFile(dvpHandle handle, dvpUint32 ResoulutionModeSel, dvpRegion *roi, dvpUint32 timeout, dvpStr filePath, dvpInt32 quality);


#ifdef _WIN32
/**
*@brief 在窗口上显示图像
*@param[in] pFrame 帧信息
*@param[in] pBuffer 图像数据的内存首地址（目前支持的数据格式包括：@link #dvpImageFormat::FORMAT_MONO MONO@endlink，@link #dvpImageFormat::FORMAT_BAYER_BG BAYER@endlink， @link #dvpImageFormat::FORMAT_BGR24 BGR24@endlink） 
*@param[in] hWnd 用于显示图像的窗口句柄
*@param[in] sRect 用于显示的图像区域，空指针代表整个图像区域
*@param[in] dRect 用于显示的窗口区域，空指针代表整个窗口区域
*@note 建议在@link #dvpStreamCallback 视频流回调函数@endlink中使用此功能
*/
dvp2api dvpStatus dvpDrawPicture(const dvpFrame *pFrame, const void *pBuffer, HWND hWnd, LPRECT sRect, LPRECT dRect);


/**
*@brief 启动录像，创建视频文件，并获得视频编码器的句柄
*@param[in] filePath 视频文件名或完整路径（视频文件的格式由文件的扩展名来决定，目前支持的视频格式包括：wmv, avi）
*@param[in] width 此参数已经废弃，可为任意值
*@param[in] height 此参数已经废弃，可为任意值
*@param[in] quality 视频画面品质，仅对avi格式有效，并且取值范围是[1,100]
*@param[out] pHandle 视频编码器句柄
*@see dvpSetTargetFormat
*@see dvpVideoRecordWrite
*@see dvpStopVideoRecord
*@warning 欧洲和韩国等地区所发行的WINDOWS操作系统没有内置WMV格式支持，可能需要自行到 <a href="https://www.codecguide.com/windows_media_codecs.htm">微软公司网站</a>下载并安装相应版本的更新
*/
dvp2api dvpStatus dvpStartVideoRecord(dvpStr filePath, dvpUint32 width, dvpUint32 height, dvpInt32 quality, dvpRecordHandle *pHandle);


/**
*@brief 检查视频编码器的句柄是否有效
*@param[in] handle 视频编码器句柄
*@param[out] pValid 有效状态
*@note @link #dvpStartVideoRecord 启动录像@endlink成功则句柄有效，@link #dvpStopVideoRecord 停止录像@endlink成功则句柄失效
*/
dvp2api dvpStatus dvpIsVideoRecorderValid(dvpRecordHandle handle, bool *pValid);


/**
*@brief 把视频流数据写入视频文件
*@param[in] handle 视频编码器句柄
*@param[in] pFrame 帧信息
*@param[in] pBuffer 图像数据的内存首地址（目前支持的数据格式包括：@link #dvpImageFormat::FORMAT_BGR24 BGR24@endlink）
*@see dvpGetFrame
*@see dvpStartVideoRecord
*@see dvpStopVideoRecord
*@note @link #dvpStartVideoRecord 启动录像@endlink之前，请正确@link #dvpSetTargetFormat 设定目标图像格式@endlink
*/
dvp2api dvpStatus dvpVideoRecordWrite(dvpRecordHandle handle, const dvpFrame *pFrame, const void *pBuffer);


/**
*@brief 停止录像，关闭视频文件，并释放视频编码器的句柄
*@param[in] handle 视频编码器句柄
*@see dvpVideoRecordWrite
*@see dvpStartVideoRecord
*/
dvp2api dvpStatus dvpStopVideoRecord(dvpRecordHandle handle);


/**
*@brief 显示相机属性模态对话框
*@param[in] handle 相机句柄
*@param[in] hParent 父窗口的窗口句柄
*/
dvp2api dvpStatus dvpShowPropertyModalDialog(dvpHandle handle, HWND hParent);


/**
*@brief 显示相机选择对话框
*@param[in] hParent 父窗口的窗口句柄
*@param[out] pCameraInfo 相机信息
*/
dvp2api dvpStatus dvpShowSelectionDialog(HWND hParent, dvpCameraInfo *pCameraInfo);


#endif
/**
*@brief 从相机存储器中读取用户自定义数据
*@param[in] handle 相机句柄
*@param[in] addr 读取地址
*@param[in, out] pBuffer 数据的内存首地址
*@param[in] size 数据长度（通常不大于4k字节）
*@see dvpWriteUserData
*@note 读取量，不能大于存储器的最大存储空间
*@note 须先@link #dvpOpen 打开@endlink相机，才能使用此功能
*@warning 可能会消耗较长的等待时间
*/
dvp2api dvpStatus dvpReadUserData(dvpHandle handle, dvpUint32 addr, void *pBuffer, dvpUint32 size);


/**
*@brief 将用户自定义数据写入至相机存储器
*@param[in] handle 相机句柄
*@param[in] addr 写入地址
*@param[in] pBuffer 数据的内存首地址
*@param[in] size 数据长度（通常不大于4k字节）
*@see dvpReadUserData
*@note 写入量，不能大于存储器的最大存储空间
*@note 须先@link #dvpOpen 打开@endlink相机，才能使用此功能
*@warning 可能会消耗较长的等待时间
*/
dvp2api dvpStatus dvpWriteUserData(dvpHandle handle, dvpUint32 addr, const void *pBuffer, dvpUint32 size);


/**
*@brief 为相机设定一个@link #dvpCameraInfo::UserID (User Defined Name) @endlink
*@param[in] handle 相机句柄
*@param[in] UserId 用户命名
*@param[in, out] pLength 输入提供的字符串长度，输出实际使用的字符串长度
*@see dvpGetCameraInfo
*/
dvp2api dvpStatus dvpSetUserId(dvpHandle handle, dvpStr UserId, dvpUint32 *pLength);


/** @} end of __DVP2_API_EXTEND__ */

/** @defgroup __DVP_EXPOSURE__ 曝光功能
*@{
*/
/** @} end of __DVP_EXPOSURE__ */

/** @defgroup __DVP_COLOR__ 色彩调节
*@{
*/
/** @} end of __DVP_COLOR__ */

/** @defgroup __DVP_ENHANCE__ 增强效果
*@{
*/
/** @} end of __DVP_ENHANCE__ */

/** @defgroup __DVP_ADVANCED__ 高级设置
*@{
*/
/** @} end of __DVP_ADVANCED__ */

/** @defgroup __DVP_SIZE__ 图像尺寸
*@{
*/
/** @} end of __DVP_SIZE__ */

/** @defgroup __DVP_TRIGGER__ 触发功能
*@{
*/
/** @} end of __DVP_TRIGGER__ */

/** @defgroup __DVP_INFORMATION__ 基本信息
*@{
*/
/** @} end of __DVP_INFORMATION__ */
/** @} end of __DVP2_API__ */

#endif /* __DVPCAMERA_H__ */

