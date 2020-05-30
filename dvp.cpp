#include "dvp.h"

#include <iostream>



namespace dlp {

dvpclass::dvpclass()
{
    this->maincount    = 0;
    this->timeout      = 100;
    this->quality      = 100;

    this->is_connected_= FALSE;
    this->is_started_  = FALSE;
    this->is_setup_    = FALSE;
}
dvpclass::~dvpclass()
{
    this->debug_.Msg("Disconnecting...");
    this->Disconnect();

    this->debug_.Msg("Deconstructed");
}
void dvpclass::file_name_add()
{
    std::string name = "D:\\photo\\Camera\\";
    if(file_name_num<10)name+="0";
    else name+=char(file_name_num/10+48);
    name+=char(file_name_num%10+48);
    name+=".jpg";
    file_name = name;
    file_name_num++;
    if(file_name_num==100)file_name_num=0;
//    std::cout << "file_name_num:" << file_name_num << "  file_name:" << file_name << std::endl ;
}
void dvpclass::get_info()
{
/**    dvpString64 Vendor;          设计厂商     /
//     dvpString64 Manufacturer;    生产厂商     /
//     dvpString64 Model;           型号        M2S130M
//     dvpString64 Family;          系列        USB2.0 Camera
//     dvpString64 LinkName;        连接名       m2s130m-0
//     dvpString64 SensorInfo;      传感器描述   CMOS
//     dvpString64 HardwareVersion; 硬件版本     /
//     dvpString64 FirmwareVersion; 固件版本     /
//     dvpString64 KernelVersion;   内核驱动版本  /
//     dvpString64 DscamVersion;    设备驱动版本  0.7.44.19960
//     dvpString64 FriendlyName;    友好设备名称  USB2_1.3M_MONO@130W
//     dvpString64 PortInfo;        接口描述      USB
//     dvpString64 SerialNumber;    序列号       130W
//     dvpString128 CameraInfo;     相机描述     /
//     dvpString128 UserID;         用户命名     UE131001219
//     dvpString128 reserved;       保留字节     /
**/
    for (dvpUint32 i = 0; i < maincount; i++)
    {
        Camera_ID_set.clear();
        if(dvpEnum(i, &camerainfo) == DVP_STATUS_OK)
        {
            Camera_ID_set.insert(camerainfo.UserID );
//            cout << "Camera FriendlyName(i=" << i << ") : " << camerainfo.FriendlyName << endl;
//            cout << "Camera UserID(i=" << i << ") : " << camerainfo.UserID << endl;
        }
    }
    if(int(Camera_ID_set.size())!=Camera_ID_count)
    {
        std::set<std::string>::iterator myit;
        int i=1;
        Camera_ID_count = int(Camera_ID_set.size());
        for (myit = Camera_ID_set.begin();myit != Camera_ID_set.end();++myit)
        {
            std::cout << "Camera UserID(i=" << i << ") : " << *myit << std::endl;
            i++;
        }
    }
}

void dvpclass::get_ID(dvpHandle &Handle,dvpOpenMode mode)
{
    dvpStr UserId = "UE131001219";
    /** mode
        @brief OPEN_OFFLINE 离线打开
        @brief OPEN_NORMAL 打开实际设备
        @brief OPEN_DEBUG 以调试方式打开设备；对于千兆网相机，可以避免单步调试或断点引起的心跳包超时
        @brief HIGH_PRIORITY 图像采集和处理线程使用较高的优先级
    **/
    status = dvpOpenByUserId(UserId,mode,&Handle);
    this->is_connected_ = TRUE;
}
void dvpclass::camera_sive_set(dvpHandle &Handle,int Weight,int Hight)
{
    dvpRegion roi;
    roi.X = 0;
    roi.Y = 0;
    roi.W = Weight; //max(1280,1024)
    roi.H = Hight;
    dvpSetRoi(Handle,roi);
//dvpShowPropertyModalDialog
}
void dvpclass::camera_format_set(dvpHandle &Handle)
{
    //设置源图像格式
    dvpSetSourceFormat(Handle,S_MONO8);
    //设置目标图像格式
    dvpSetTargetFormat(Handle,S_MONO8);
}
void dvpclass::get_oneframe(dvpHandle &Handle)
{
    dvpFrame pFrame;                       //帧信息
    void *pBuffer;                         //首地址
    get_ID(Handle,OPEN_NORMAL);

    camera_sive_set(Handle,1280,1024);
    camera_format_set(Handle);

    dvpStart(Handle);                             //开始视频流
    dvpGetFrame(Handle,&pFrame,&pBuffer,timeout); //获得一张照片

    /**
 format
    FORMAT_MONO = 0       @brief 黑白图像
    FORMAT_BAYER_BG = 1   @brief BGGR图像
    FORMAT_BAYER_GB = 2   @brief GBRG图像
    FORMAT_BAYER_GR = 3   @brief GRBG图像
    FORMAT_BAYER_RG = 4   @brief RGGB图像
    FORMAT_BGR24 = 10     @brief BGR三通道24比特图像
    FORMAT_BGR32 = 11     @brief BGRA四通道32比特图像
    FORMAT_RGB32 = 15     @brief RGBA四通道32比特图像
    FORMAT_YUV_411 = 20   @brief YUV411
    FORMAT_YUV_422 = 21   @brief YUV422

 bits
    BITS_8 = 0            @brief 8比特数据
    BITS_10 = 1           @brief 10比特数据
    BITS_12 = 2           @brief 12比特数据
    BITS_14 = 3           @brief 14比特数据
    BITS_16 = 4           @brief 16比特数据
    **/
//    std::cout << "Handle: " << Handle << std::endl;
//    std::cout << "pFrame(format):" << pFrame.format << std::endl;
//    std::cout << "pFrame(bits):" << pFrame.bits << std::endl;
//    std::cout << "pFrame(iWidth,iHeight):" << pFrame.iWidth << "," << pFrame.iHeight << std::endl;
    file_name_add();
    dvpSavePicture(&pFrame,pBuffer,file_name.c_str(),quality);

//    dvpStop(H);
    dvpClose(Handle);
}
void dvpclass::get_video(dvpHandle &Handle)
{
    dvpFrame pFrame;                       //帧信息
    void *pBuffer;                         //首地址

    if(isConnected())
    {
        dvpRestart(Handle);                             //开始视频流
        dvpGetFrame(Handle,&pFrame,&pBuffer,timeout); //获得一张照片

        std::cout << "Handle: " << Handle << std::endl;
        std::cout << "pFrame(format):" << pFrame.format << std::endl;
        std::cout << "pFrame(bits):" << pFrame.bits << std::endl;
        std::cout << "pFrame(iWidth,iHeight):" << pFrame.iWidth << "," << pFrame.iHeight << std::endl;
        file_name_add();
        dvpSavePicture(&pFrame,pBuffer,file_name.c_str(),quality);

        dvpHold(Handle);
    }
    else
    {
        get_ID(Handle,OPEN_NORMAL);
        camera_sive_set(Handle,1280,1024);
        camera_format_set(Handle);

        dvpStart(Handle);                             //开始视频流
        dvpGetFrame(Handle,&pFrame,&pBuffer,timeout); //获得一张照片

        std::cout << "Handle: " << Handle << std::endl;
        std::cout << "pFrame(format):" << pFrame.format << std::endl;
        std::cout << "pFrame(bits):" << pFrame.bits << std::endl;
        std::cout << "pFrame(iWidth,iHeight):" << pFrame.iWidth << "," << pFrame.iHeight << std::endl;
        file_name_add();
        dvpSavePicture(&pFrame,pBuffer,file_name.c_str(),quality);

        dvpStop(Handle);

        this->is_connected_ = TRUE;
    }

}

void dvpclass::get_ID(dvpOpenMode mode)
{
//    dvpStr UserId = "UE131001219";
    /** mode
        @brief OPEN_OFFLINE 离线打开
        @brief OPEN_NORMAL 打开实际设备
        @brief OPEN_DEBUG 以调试方式打开设备；对于千兆网相机，可以避免单步调试或断点引起的心跳包超时
        @brief HIGH_PRIORITY 图像采集和处理线程使用较高的优先级
    **/
    status = dvpOpenByUserId("UE131001219",mode,&camera_Handle);
    this->is_connected_ = TRUE;
}
void dvpclass::camera_set(int format,int size_x,int size_y,int fExposure)
{
    dvpInt32 pAeTarget;    //曝光目标亮度
    dvpInt32 pSaturation;  //饱和度
    dvpInt32 pContrast;    //对比度

    switch (format) {
        case 0:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RAW8);break;
        case 1:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RAW10);break;
        case 2:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RAW12);break;
        case 3:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RAW14);break;
        case 4:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RAW16);break;
        case 5:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_BGR24);break;
        case 6:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_BGR32);break;
        case 7:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_BGR48);break;
        case 8:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_BGR64);break;
        case 9:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RGB24);break;
        case 10:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RGB32);break;
        case 11:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RGB48);break;
        case 12:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_RGB64);break;
        case 13:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_YCBCR_411);break;
        case 14:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_YCBCR_422);break;
        case 15:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_YCBCR_444);break;
        case 16:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_MONO8);break;
        case 17:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_MONO10);break;
        case 18:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_MONO12);break;
        case 19:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_MONO14);break;
        case 20:dvpSetTargetFormat(camera_Handle,dvpStreamFormat::S_MONO16);break;
    }

    dvpRegion roi;
    roi.X=(1280-size_x)/2;
    roi.Y=(1024-size_y)/2;
    roi.W=size_x;
    roi.H=size_y;
    status = dvpSetRoi(camera_Handle,roi);
    status = dvpSetExposure(camera_Handle,fExposure);

    //曝光目标亮度80
//    status = dvpSetAeTarget(camera_Handle,150);
    status = dvpGetAeTarget(camera_Handle,&pAeTarget);
    std::cout << "pAeTarget:" << pAeTarget << std::endl;

    //饱和度
//    status = dvpSetSaturation(camera_Handle,200);
    status = dvpGetSaturation(camera_Handle,&pSaturation);
    std::cout << "pSaturation:" << pSaturation << std::endl;

    //对比度100
//    status = dvpSetContrast(camera_Handle,200);
    status = dvpGetContrast(camera_Handle,&pContrast);
    std::cout << "pContrast:" << pContrast << std::endl;


}
void dvpclass::get_oneframe()
{
    void *pBuffer;                                //首地址
    pBuffer = nullptr;
    status = dvpGetFrame(camera_Handle,&pFrame,&pBuffer,timeout); //获得一张照片

    /** format
    FORMAT_MONO = 0       @brief 黑白图像
    FORMAT_BAYER_BG = 1   @brief BGGR图像
    FORMAT_BAYER_GB = 2   @brief GBRG图像
    FORMAT_BAYER_GR = 3   @brief GRBG图像
    FORMAT_BAYER_RG = 4   @brief RGGB图像
    FORMAT_BGR24 = 10     @brief BGR三通道24比特图像
    FORMAT_BGR32 = 11     @brief BGRA四通道32比特图像
    FORMAT_RGB32 = 15     @brief RGBA四通道32比特图像
    FORMAT_YUV_411 = 20   @brief YUV411
    FORMAT_YUV_422 = 21   @brief YUV422
    **/
    /**  bits
    BITS_8 = 0            @brief 8比特数据
    BITS_10 = 1           @brief 10比特数据
    BITS_12 = 2           @brief 12比特数据
    BITS_14 = 3           @brief 14比特数据
    BITS_16 = 4           @brief 16比特数据
    **/

    std::cout << "pFrame(format):" << pFrame.format << std::endl;
    std::cout << "pFrame(bits):" << pFrame.bits << std::endl;
    std::cout << "pFrame(iWidth,iHeight):" << pFrame.iWidth << "," << pFrame.iHeight << std::endl;
    file_name_add();
    status = dvpSavePicture(&pFrame,pBuffer,file_name.c_str(),quality);
}
void dvpclass::get_video()
{
//打开相机
    if(!MW_IsValid())
    {
        get_ID(OPEN_NORMAL);
        camera_set(16,1280,1024,0);
    }
//视频流启动
    if(MW_IsHold())MW_Restart();
    else MW_Start();

    void *pBuffer;                                //首地址
    pBuffer = nullptr;
    dvpGetFrame(camera_Handle,&pFrame,&pBuffer,timeout); //获得一张照片

    if(CAMERA_VIDEO_SAVESET){file_name_add();dvpSavePicture(&pFrame,pBuffer,file_name.c_str(),quality);}

}

void dvpclass::MW_Restart(){dvpRestart(camera_Handle);this->is_started_=TRUE;}
void dvpclass::MW_Hold(){dvpHold(camera_Handle);this->is_started_=FALSE;}
void dvpclass::MW_Start(){dvpStart(camera_Handle);this->is_started_=TRUE;}
void dvpclass::MW_Stop(){dvpStop(camera_Handle);this->is_started_=FALSE;}
void dvpclass::MW_Close(){dvpClose(camera_Handle);this->is_connected_=FALSE;}
bool dvpclass::MW_IsValid(){bool pIsValid;dvpIsValid(camera_Handle,&pIsValid);this->is_connected_=pIsValid;return pIsValid;}
bool dvpclass::MW_IsOnline(){bool pIsOnline;dvpIsOnline(camera_Handle,&pIsOnline);return pIsOnline;}
bool dvpclass::MW_IsHold(){bool pIsHold;dvpIsHold(camera_Handle,&pIsHold);this->is_started_=!pIsHold;return pIsHold;}


dlp::ReturnCode dvpclass::Connect(const std::string &id)
{
    dlp::ReturnCode ret;
    if(this->isConnected()){
            this->debug_.Msg("Camera already connected!");
            return ret.AddError(CAMERA_ALREADY_CONNECTED);
        }
    dvpRefresh(&maincount);

    if(maincount==0)
    {
        this->debug_.Msg("No Camera!");
        return ret.AddError(CAMERA_NO_CAMERA);
    }
    else
        //枚举相机
        this->get_info();

    //打开相机
//    this->get_ID(camera_Handle,OPEN_NORMAL);
    if(!IsValidHandle()) this->get_ID(OPEN_NORMAL);

    // 设置相机连接标志并保存相机id

    if(this->is_connected_)
        this->camera_id_ = id;
    else
    {
        this->debug_.Msg("Camera not connected!");
        return ret.AddError(CAMERA_NOT_CONNECTED);
    }
    return ret;
}
dlp::ReturnCode dvpclass::Disconnect()
{
    dlp::ReturnCode ret;
    if(isStarted())ret=this->Stop();
    if(isConnected())ret=this->Close();
    return ret;
}
dlp::ReturnCode dvpclass::Setup(const dlp::Parameters &settings)
{
    dlp::ReturnCode ret;
    
    if (!(this->is_connected_))
    {
        this->debug_.Msg("Camera is NOT Connected");
        return ret.AddError(CAMERA_NOT_CONNECTED);
    }
    

    //还原最大图像缓冲区大小(设置最大缓冲区大小)
    settings.Get(&this->image_queue_max_frames_);

    if(settings.Contains(this->mode_)){
        settings.Get(&this->mode_);
        }
    if(settings.Contains(this->height_)){
        settings.Get(&this->height_);
        }
    if(settings.Contains(this->width_)){
        settings.Get(&this->width_);
     }
    if(settings.Contains(this->image_format_)){
        settings.Get(&this->image_format_);
        }
    if(settings.Contains(this->pixel_format_)){
        settings.Get(&this->pixel_format_);
        }
    //设置相机
//    this->camera_set(10,1280,1024,1);//RGBA32
    this->is_setup_ = true;
    
    return ret;
}
dlp::ReturnCode dvpclass::GetSetup(dlp::Parameters *settings)const
{
    dlp::ReturnCode ret;
    
    // 检查指针是否不为空
    if(!settings)
        return ret.AddError(DVP_CAM_NULL_POINTER);

    if(!this->isSetup())
        return ret.AddError(CAMERA_NOT_SETUP);

    // 清除参数列表
    settings->Clear();
    settings->Set(this->mode_);
    settings->Set(this->height_);
    settings->Set(this->width_);
    settings->Set(this->image_format_);
    settings->Set(this->pixel_format_);

    return ret;
}
dlp::ReturnCode dvpclass::Start()
{
    dlp::ReturnCode ret;

    if (!(this->is_connected_  && this->is_setup_))
    {
        this->debug_.Msg("Connect and Set up the camera before starting it");
        return ret.AddError(CAMERA_NOT_CONNECTED);
    }

    if(this->is_started_==TRUE) return ret;

    if(MW_IsHold())
    {
        status = dvpRestart(camera_Handle);
//        if(status==DVP_STATUS_OK)
        this->debug_.Msg("Camera Restart");
//        else{this->debug_.Msg("Camera Restart failed!");return ret.AddError(CAMERA_NOT_STARTED);}
    }
    else
    {
        status = dvpStart(camera_Handle);
//        if(status==DVP_STATUS_OK)
        this->debug_.Msg("Camera Start");
//        else{this->debug_.Msg("Camera Start failed!");return ret.AddError(CAMERA_NOT_STARTED);}
    }
    this->is_started_=TRUE;
    this->debug_.Msg("Camera started.");

    return ret;
}
dlp::ReturnCode dvpclass::Stop()
{
    dlp::ReturnCode ret;
    status = dvpStop(camera_Handle);
//    if(status==DVP_STATUS_OK)
    this->debug_.Msg("Camera stopped");
//    else{this->debug_.Msg("Camera stop failed!");return ret.AddError(CAMERA_NOT_STOPPED);}

    this->is_started_=FALSE;
    return ret;
}
dlp::ReturnCode dvpclass::Hold()
{
    dlp::ReturnCode ret;
    status = dvpHold(camera_Handle);
//    if(status==DVP_STATUS_OK)
    this->debug_.Msg("Camera holded");
//    else{this->debug_.Msg("Camera hold failed!");return ret.AddError(CAMERA_NOT_STOPPED);}

    this->is_started_=FALSE;
    return ret;
}
dlp::ReturnCode dvpclass::Close(){
    dlp::ReturnCode ret;
    status = dvpClose(camera_Handle);
//    if(status==DVP_STATUS_OK)
    this->debug_.Msg("Camera connected");
//    else{this->debug_.Msg("Camera connected failed!");return ret.AddError(CAMERA_NOT_DISCONNECTED);}

    this->is_connected_=FALSE;
    return ret;
}
dlp::ReturnCode dvpclass::GetFrame(dlp::Image* ret_frame)
{
    ReturnCode ret;
    void *ppBuffer;                         //首地址
    ppBuffer = nullptr;


//获得一张照片
    status = dvpGetFrame(camera_Handle,&pFrame,&ppBuffer,timeout);
    if(CAMERA_VIDEO_SAVESET){file_name_add();dvpSavePicture(&pFrame,ppBuffer,file_name.c_str(),quality);}
//    ret = ret_frame->Create(1024,1280,Image::Format::RGB_UCHAR,ppBuffer,1280*3);
    ret = ret_frame->Load(file_name.c_str());
    return ret;
}
dlp::ReturnCode dvpclass::GetFrameBuffered(dlp::Image* ret_frame)
{
    ReturnCode ret;
    ret_frame = nullptr;

    this->debug_.Msg("Camera GetFrameBuffered is no set");
    ret = ret_frame->Load(file_name);

    return ret.AddError(CAMERA_NOSET_GETFRAMEBUFFERED);
}
dlp::ReturnCode dvpclass::GetCaptureSequence(const unsigned int &arg_number_captures,
                                      dlp::Capture::Sequence* ret_capture_sequence)
{
    ReturnCode ret;
    //DvpFrameBuffer* image_buffer = (DvpFrameBuffer*)this->image_buffer_;

    Capture dv_capture;
    Image dv_image;


    // 停止，然后开始相机拍摄
    ret = this->Stop();
    if(ret.hasErrors()) return ret;

    ret = this->Start();
    if(ret.hasErrors()) return ret;


    // 让相机抓取所需的图像
    unsigned int count = 0;
    while(count < arg_number_captures)
    {
        // 保存捕获的数量
//        count = this->image_buffer_->queue.size();

        if(count == arg_number_captures)
        {
            ret = this->Stop();
            if(ret.hasErrors()) return ret;
        }
    }

    dv_capture.data_type = dlp::Capture::DataType::IMAGE_DATA;

    // 从相机中获取帧数
    for (unsigned int i = 0; i < arg_number_captures; i++)
    {
        // 从相机里拿一个相框
        ret = this->GetFrame(&dv_image);

        if (ret.hasErrors())
        {
            this->debug_.Msg("Camera Capture Error");
            return ret;
        }

        dv_capture.image_data = dv_image;

        if (ret_capture_sequence->Add(dv_capture).hasErrors())
        {
            this->debug_.Msg("Camera Captures cannot be added to the Capture Sequence");
            ret.AddError(CAMERA_FRAME_GRAB_FAILED);
            return ret;
        }

        dv_capture.image_data.Clear();
    }

    return ret;
}

bool dvpclass::isConnected() const{return this->is_connected_;}
bool dvpclass::isStarted() const{return this->is_started_;}
bool dvpclass::IsValidHandle() const{
    bool bValidHandle;
    dvpIsValid(camera_Handle, &bValidHandle);
    return bValidHandle;
}

dlp::ReturnCode dvpclass::GetID(std::string* ret_id) const
{    ReturnCode ret;
     if (!this->isConnected())
     {
         this->debug_.Msg("No Camera is connected");
         ret.AddError(CAMERA_NOT_CONNECTED);
     }
     else{
         *ret_id = this->camera_id_;
     }

     return ret;
}
dlp::ReturnCode dvpclass::GetRows(unsigned int* ret_rows) const
{
    ReturnCode ret;

    // 检查照相机是否已连接好
    if (!this->isConnected())
    {
        this->debug_.Msg("No Camera is connected");
        return ret.AddError(CAMERA_NOT_CONNECTED);
    }

    // 返回的行
    (*ret_rows) = this->height_.Get();

    return ret;
}
dlp::ReturnCode dvpclass::GetColumns(unsigned int* ret_columns) const
{
    ReturnCode ret;

    // 检查照相机是否已连接好
    if (!this->isConnected())
    {
        this->debug_.Msg("No Camera is connected");
        return ret.AddError(CAMERA_NOT_CONNECTED);
    }

    (*ret_columns) = this->width_.Get();

    return ret;
}
/** @brief  返回相机传感器上设置的帧速率*/
dlp::ReturnCode dvpclass::GetFrameRate(float* ret_framerate) const
{
    ReturnCode ret;

    // 检查照相机是否已连接好
    if (!this->isConnected())
    {
        this->debug_.Msg("No Camera is connected");
        return ret.AddError(CAMERA_NOT_CONNECTED);
    }

    (*ret_framerate) = this->frame_rate_.Get();

    return ret;
}
/** @brief  返回相机传感器上设置的曝光值*/
dlp::ReturnCode dvpclass::GetExposure(float* ret_exposure) const
{
    ReturnCode ret;
    dvpInt32 pAeTarget;


    // 检查照相机是否已连接好
    if (!this->isConnected())
    {
        this->debug_.Msg("No Camera is connected");
        return ret.AddError(CAMERA_NOT_CONNECTED);
    }
    dvpGetAeTarget(camera_Handle,&pAeTarget);

    *ret_exposure = pAeTarget;

    return ret;
}

}
