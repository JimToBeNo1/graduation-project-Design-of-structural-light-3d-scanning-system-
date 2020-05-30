#ifndef DVP_H
#define DVP_H


#include "DVPCamera.h"


#include <iostream>
#include <set>


#include <common/debug.hpp>
#include <common/other.hpp>
#include <common/returncode.hpp>
#include <common/image/image.hpp>
#include <common/parameters.hpp>
#include <camera/camera.hpp>
#include <common/capture/capture.hpp>

#include <queue>
#include <atomic>

#include <memory>

#define CAMERA_NO_CAMERA                     "CAMERA_NO_CAMERA"
#define CAMERA_NO_GETFRAME                   "CAMERA_NO_GETFRAME"
#define CAMERA_NOSET_GETFRAMEBUFFERED        "CAMERA_NOSET_GETFRAMEBUFFERED"
#define DVP_CAM_NULL_POINTER                 "DVP_CAM_NULL_POINTER"
#define DVP_CAM_IMAGE_BUFFER_EMPTY           "DVP_CAM_IMAGE_BUFFER_EMPTY"
#define CAMERA_VIDEO_SAVESET 1 //1保存录像图片  0不保存录像图片

namespace  dlp{


class dvpclass : public dlp::Camera
{
public:
    dvpclass();
    virtual ~dvpclass();

    dvpCameraInfo camerainfo;              //相机信息
    dvpFrame pFrame;                       //帧信息

    dvpUint32 maincount;                   //相机数目
    dvpUint32 timeout;                 //超时时间（毫秒）(dvpGetFrame)
    dvpInt32 quality;                  //图像品质，仅对jpeg格式有效，并且取值范围是[1,100](dvpSavePicture)
    std::string file_name;
    const std::string file_new_name = "D:\\photo\\Camera\\100.jpg";

    void get_info();
    void get_ID(dvpHandle &Handle,dvpOpenMode mode);
    void camera_sive_set(dvpHandle &Handle,int Weight,int Hight);//max(1280,1024)
    void camera_format_set(dvpHandle &Handle);
    void get_oneframe(dvpHandle &Handle);
    void get_video(dvpHandle &Handle);

    void get_ID(dvpOpenMode mode);
    void camera_set(int,int,int,int);
    void get_oneframe();
    void get_video();

    void MW_Restart();//快速重启
    void MW_Hold();   //快速暂停
    void MW_Start();  //启动
    void MW_Stop();   //停止
    void MW_Close();  //关闭
    bool MW_IsValid();//检查相机句柄是否有效
    bool MW_IsOnline();//检查相机是否联机
    bool MW_IsHold();//查询视频流是否处于暂停状态

    class Parameters{
    public:
        DLP_NEW_PARAMETERS_ENTRY(Mode,        "DVP_PARAMETERS_MODE",          unsigned int, OPEN_NORMAL);
        DLP_NEW_PARAMETERS_ENTRY(Height,      "DVP_PARAMETERS_HEIGHT_PXL",    unsigned int, 1024);
        DLP_NEW_PARAMETERS_ENTRY(Width,       "DVP_PARAMETERS_WIDTH_PXL",     unsigned int, 1280);
        DLP_NEW_PARAMETERS_ENTRY(ImageFormat, "DVP_PARAMETERS_Image_FORMAT",  unsigned int, FORMAT_MONO );
        DLP_NEW_PARAMETERS_ENTRY(PixelFormat, "DVP_PARAMETERS_PIXEL_FORMAT",  unsigned int, BITS_8);
        DLP_NEW_PARAMETERS_ENTRY(FrameRate,   "DVP_PARAMETERS_FRAME_RATE_HZ",       float, 60.000);

    };

    // 纯虚函数
    dlp::ReturnCode Connect(const std::string &id = "0");
    dlp::ReturnCode Disconnect();
    dlp::ReturnCode Setup(const dlp::Parameters &settings);
    dlp::ReturnCode GetSetup(dlp::Parameters *settings)const;
    dlp::ReturnCode Start();
    dlp::ReturnCode Stop();
    dlp::ReturnCode Hold();
    dlp::ReturnCode Close();
    dlp::ReturnCode GetFrame(dlp::Image* ret_frame);
    dlp::ReturnCode GetFrameBuffered(dlp::Image* ret_frame);
    dlp::ReturnCode GetCaptureSequence(const unsigned int &arg_number_captures,
                                  dlp::Capture::Sequence* ret_capture_sequence);

    bool isConnected() const;
    bool isStarted() const;
    bool IsValidHandle() const;

    dlp::ReturnCode GetID(std::string* ret_id) const;
    dlp::ReturnCode GetRows(unsigned int* ret_rows) const;
    dlp::ReturnCode GetColumns(unsigned int* ret_columns) const;

    dlp::ReturnCode GetFrameRate(float* ret_framerate) const;
    dlp::ReturnCode GetExposure(float* ret_exposure) const;

    
protected:
    void file_name_add();

private:

    dvpStatus status;                      //状态枚举
    dvpHandle camera_Handle;                      //相机句柄

    int Camera_ID_count = 0;
    int file_name_num = 1;
    std::set<std::string>Camera_ID_set;

    bool bSoftTrigger = false;
    bool bSoftTriggerLoop = false;

    // 成员记录摄像机是否已连接或已启动
    bool is_connected_;
    bool is_started_;

    
    unsigned int packet_size;
    float        packet_percent;

    std::string   camera_id_;
    Camera::Parameters::FrameBufferSize image_queue_max_frames_;

    Parameters::Mode                mode_;
    Parameters::Height              height_;
    Parameters::Width               width_;
    Parameters::ImageFormat         image_format_;
    Parameters::PixelFormat         pixel_format_;
    Parameters::FrameRate           frame_rate_;

};

}







#endif // DVP_H
