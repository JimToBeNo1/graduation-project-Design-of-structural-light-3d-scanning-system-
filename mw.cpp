#include "mw.h"
#include "ui_mw.h"




MW::MW(QWidget *parent ) :
    QMainWindow(parent),
    ui(new Ui::MW)
{
    ui->setupUi(this);
    QTimer * timer = new QTimer(this);
    connect(this,SIGNAL(Camera_Refresh_Sign(void)),this,SLOT(Camera_refresh(void)));
    connect(timer,&QTimer::timeout,this,&MW::timerUpdate);
    timer -> start(1000);
}

MW::~MW()
{
    SDK.menu(10);
    delete ui;
}

void MW::on_photo_clicked()
{
    if(Camera_Count>0)
    {
        Image_set();
        camera.MW_Close();
    }
    else
        std::cout << "no find camera" << std::endl;
}
void MW::on_video_clicked()
{
    if(!SDK_Verify==1)
    {
        std::cout << "-------------------SDK.verify()----------------" << std::endl;
        if(!SDK.verify()==1)return;
        this->SDK_Verify=1;
    }
    Camera_Video();
}
void MW::on_calibration_clicked()
{
    if(!SDK_Verify==1)
    {
        std::cout << "-------------------SDK.verify()----------------" << std::endl;
        if(!SDK.verify()==1)return;
        this->SDK_Verify=1;
    }
    Camera_Calibration();
}
void MW::on_discern_clicked()
{
    if(!SDK_Verify==1)
    {
        std::cout << "-------------------SDK.verify()----------------" << std::endl;
        if(!SDK.verify()==1)return;
        this->SDK_Verify=1;
    }
    Camera_Discern();
}

void MW::Camera_refresh(void)
{
    //刷新相机
    dvpRefresh(&camera.maincount);
    int a = int(camera.maincount);
    ui->camera_count->display(a);

    if(a==0)
    {
        Camera_Count = 0;
        ui->camera_ID->display(0);
    }
    else
    {//枚举相机
        camera.get_info();
        QString b= camera.camerainfo.UserID;
        ui->camera_ID->display(b);
        Camera_Count = a;
    }
}
void MW::Camera_Photo(void)
{
//新建QLabel类对象
    QLabel *pick = new QLabel();
//打开相机
    if(!camera.MW_IsValid())camera.get_ID(OPEN_NORMAL);
//设置相机
    camera.camera_set(Image_setnumber.format,Image_setnumber.size_x,Image_setnumber.size_y,Image_setnumber.fExposure);
//视频流启动
    if(camera.MW_IsHold())camera.MW_Restart();
    else camera.MW_Start();
//获得一帧图像
    camera.get_oneframe();
    camera.MW_Stop();

    QPixmap pix(camera.file_name.c_str());
    pick->setPixmap(pix);
    pick->resize(pix.size());
    //pick->setAttribute(Qt::WA_DeleteOnClose);

//显示
    pick -> show();
    Image_set2();
}
void MW::Camera_Video(void)
{

/*按钮*/
    QDialog *Dialog_Video = new QDialog(this);
    Dialog_Video->setWindowTitle("视频控制界面");
    Dialog_Video->resize(200,50);
    QGridLayout *GridLayout = new QGridLayout();

    QPushButton *Button_OK = new QPushButton(Dialog_Video);
    Button_OK->setText("开始/暂停");
    connect(Button_OK,SIGNAL(clicked()),this,SLOT(Video_Start_SLOT()));

    QPushButton *Button_EXIT = new QPushButton(Dialog_Video);
    Button_EXIT->setText("退出");
    connect(Button_EXIT,SIGNAL(clicked()),Dialog_Video,SLOT(close()));
    connect(Button_EXIT,SIGNAL(clicked()),this,SLOT(Video_Close_SLOT()));

    GridLayout->addWidget(Button_OK,0,0,1,1);
    GridLayout->addWidget(Button_EXIT,1,0,1,1);
    Dialog_Video->setLayout(GridLayout);

    Dialog_Video->show();
}
void MW::Camera_Calibration(void)
{
    QDialog *Calibration = new QDialog(this);
    QGridLayout *GridLayout = new QGridLayout();
    Calibration->setWindowModality(Qt::WindowModal);
    Calibration->setWindowTitle("系统校准");
    Calibration->resize(170,250);

    QPushButton *Button_XiaoZhunBan = new QPushButton(Calibration);
    Button_XiaoZhunBan->setText("生成校准版");
    connect(Button_XiaoZhunBan,SIGNAL(clicked()),this,SLOT(Menu_Choose1(void)));

    QSpinBox *SpinBox_XiaoZhunBan = new QSpinBox;
    QSlider *Slider_XiaoZhunBan = new QSlider(Qt::Horizontal);
    SpinBox_XiaoZhunBan->setMaximum(10000);
    SpinBox_XiaoZhunBan->setMinimum(0);
    Slider_XiaoZhunBan->setMaximum(10000);
    Slider_XiaoZhunBan->setMinimum(0);
    SpinBox_XiaoZhunBan->setValue(10);
    Slider_XiaoZhunBan->setValue(10);

    connect(SpinBox_XiaoZhunBan,SIGNAL(valueChanged(int)),Slider_XiaoZhunBan,SLOT(setValue(int)));
    connect(Slider_XiaoZhunBan,SIGNAL(valueChanged(int)),SpinBox_XiaoZhunBan,SLOT(setValue(int)));
    connect(SpinBox_XiaoZhunBan,SIGNAL(valueChanged(int)),this,SLOT(Menu_XiaoZhunBan(int)));

    QPushButton *Button_PrepareDLP450 = new QPushButton(Calibration);
    Button_PrepareDLP450->setText("准备投影仪（仅需一次）");
    connect(Button_PrepareDLP450,SIGNAL(clicked()),this,SLOT(Menu_Choose2()));

    QPushButton *Button_PrepareSystem = new QPushButton(Calibration);
    Button_PrepareSystem->setText("准备校准和扫描系统");
    connect(Button_PrepareSystem,SIGNAL(clicked()),this,SLOT(Menu_Choose3()));

    QPushButton *Button_CalibrationCamera = new QPushButton(Calibration);
    Button_CalibrationCamera->setText("校准相机");
    connect(Button_CalibrationCamera,SIGNAL(clicked()),this,SLOT(Menu_Choose4()));

    QPushButton *Button_CalibrationSystem = new QPushButton(Calibration);
    Button_CalibrationSystem->setText("校准系统");
    connect(Button_CalibrationSystem,SIGNAL(clicked()),this,SLOT(Menu_Choose5()));

/*按钮*/
    QPushButton *Button_Reconnect = new QPushButton(Calibration);
    Button_Reconnect->setText("重连");
    connect(Button_Reconnect,SIGNAL(clicked()),this,SLOT(Menu_Choose9()));

    QPushButton *Button_Disconnect = new QPushButton(Calibration);
    Button_Disconnect->setText("断连");
    connect(Button_Disconnect,SIGNAL(clicked()),this,SLOT(Menu_Choose10()));

    QPushButton *Button_EXIT = new QPushButton(Calibration);
    Button_EXIT->setText("退出");
    connect(Button_EXIT,SIGNAL(clicked()),Calibration,SLOT(close()));

    GridLayout->addWidget(Button_XiaoZhunBan,0,0,1,10);
    GridLayout->addWidget(SpinBox_XiaoZhunBan,0,10,1,2);
    GridLayout->addWidget(Slider_XiaoZhunBan,0,12,1,8);
    GridLayout->addWidget(Button_PrepareDLP450,1,0,1,10);
    GridLayout->addWidget(Button_PrepareSystem,1,10,1,10);
    GridLayout->addWidget(Button_CalibrationCamera,2,0,1,10);
    GridLayout->addWidget(Button_CalibrationSystem,2,10,1,10);
    GridLayout->addWidget(Button_Reconnect,3,0,1,6);
    GridLayout->addWidget(Button_Disconnect,3,7,1,6);
    GridLayout->addWidget(Button_EXIT,3,14,1,6);
/*显示*/
    Calibration->setLayout(GridLayout);
    Calibration->exec();
}
void MW::Camera_Discern(void)
{
    QDialog *Discern = new QDialog(this);
    QGridLayout *GridLayout = new QGridLayout();
    Discern->setWindowModality(Qt::WindowModal);
    Discern->setWindowTitle("图像识别");
    Discern->resize(170,250);

    QPushButton *Button_Vertical = new QPushButton(Discern);
    Button_Vertical->setText("执行扫描(仅垂直模式)");
    connect(Button_Vertical,SIGNAL(clicked()),this,SLOT(Menu_Choose6()));

    QPushButton *Button_Standard = new QPushButton(Discern);
    Button_Standard->setText("执行扫描(仅水平模式)");
    connect(Button_Standard,SIGNAL(clicked()),this,SLOT(Menu_Choose7()));

    QPushButton *Button_VerticalStandard = new QPushButton(Discern);
    Button_VerticalStandard->setText("执行扫描(垂直和水平模式)");
    connect(Button_VerticalStandard,SIGNAL(clicked()),this,SLOT(Menu_Choose8()));

/*按钮*/
    QPushButton *Button_Reconnect = new QPushButton(Discern);
    Button_Reconnect->setText("重连");
    connect(Button_Reconnect,SIGNAL(clicked()),this,SLOT(Menu_Choose9()));

    QPushButton *Button_Disconnect = new QPushButton(Discern);
    Button_Disconnect->setText("断连");
    connect(Button_Disconnect,SIGNAL(clicked()),this,SLOT(Menu_Choose10()));

    QPushButton *Button_EXIT = new QPushButton(Discern);
    Button_EXIT->setText("退出");
    connect(Button_EXIT,SIGNAL(clicked()),Discern,SLOT(close()));

    GridLayout->addWidget(Button_Vertical,0,0,1,17);
    GridLayout->addWidget(Button_Standard,1,0,1,17);
    GridLayout->addWidget(Button_VerticalStandard,2,0,1,17);
    GridLayout->addWidget(Button_Reconnect,3,0,1,5);
    GridLayout->addWidget(Button_Disconnect,3,6,1,5);
    GridLayout->addWidget(Button_EXIT,3,12,1,5);
/*显示*/
    Discern->setLayout(GridLayout);
    Discern->exec();
}

void MW::Image_set(void)
{
    QDialog *SetImage = new QDialog(this);
    QGridLayout *GridLayout = new QGridLayout();
    SetImage->setWindowModality(Qt::WindowModal);
    SetImage->setWindowTitle("设置图片属性");
    SetImage->resize(170,250);

/*格式*/
    QLabel *Label_format = new QLabel(SetImage);
    Label_format->setText("format");
    QComboBox *comBox_format = new QComboBox(SetImage);
    comBox_format->addItem("S_RAW8 8位图像");
    comBox_format->addItem("S_RAW10 10位图像");
    comBox_format->addItem("S_RAW12 12位图像");
    comBox_format->addItem("S_RAW14 14位图像");
    comBox_format->addItem("S_RAW16 16位图像");
    comBox_format->addItem("S_BGR24 BGR三通道24比特图像");
    comBox_format->addItem("S_BGR32 BGRA四通道32比特图像");
    comBox_format->addItem("S_BGR48 BGRA四通道48比特图像");
    comBox_format->addItem("S_BGR64 BGRA四通道64比特图像");
    comBox_format->addItem("S_RGB24 RGB三通道24比特图像");
    comBox_format->addItem("S_RGB32 RGBA四通道32比特图像");
    comBox_format->addItem("S_RGB48 RGBA四通道48比特图像");
    comBox_format->addItem("S_RGB64 RGBA四通道64比特图像");
    comBox_format->addItem("S_YCBCR_411 YUV411");
    comBox_format->addItem("S_YCBCR_422 YUV422");
    comBox_format->addItem("S_YCBCR_444 YUV444");
    comBox_format->addItem("S_MONO8 8位灰度图像");
    comBox_format->addItem("S_MONO10 10位灰度图像");
    comBox_format->addItem("S_MONO12 12位灰度图像");
    comBox_format->addItem("S_MONO14 14位灰度图像");
    comBox_format->addItem("S_MONO16 16位灰度图像");
    comBox_format->setCurrentIndex(16);
    connect(comBox_format,SIGNAL(currentIndexChanged(int)),this,SLOT(Image_setnumber_format(int)));

    GridLayout->addWidget(Label_format,0,0,1,10);
    GridLayout->addWidget(comBox_format,0,10,1,10);

/*大小*/
    QLabel *Label_size = new QLabel(SetImage);
    Label_size->setText("size");
    QLabel *Label_size_x = new QLabel(SetImage);
    Label_size_x->setText("iWidth");
    QLabel *Label_size_y = new QLabel(SetImage);
    Label_size_y->setText("iHeight");

    QSpinBox *SpinBox_size_x = new QSpinBox;
    QSpinBox *SpinBox_size_y = new QSpinBox;
    QSlider *Slider_size_x = new QSlider(Qt::Horizontal);
    QSlider *Slider_size_y = new QSlider(Qt::Horizontal);

    QComboBox *comBox_size = new QComboBox(SetImage);
    comBox_size->addItem("1280*1024(MAX)");
    comBox_size->addItem("1024*768");
    comBox_size->addItem("640*512");
    comBox_size->addItem("自定义roi");
    SpinBox_size_x->setMaximum(1280);
    SpinBox_size_x->setMinimum(1);
    SpinBox_size_y->setMaximum(1024);
    SpinBox_size_y->setMinimum(1);
    Slider_size_x->setMaximum(1280);
    Slider_size_x->setMinimum(1);
    Slider_size_y->setMaximum(1024);
    Slider_size_y->setMinimum(1);
    SpinBox_size_x->setValue(1280);
    SpinBox_size_y->setValue(1024);
    Slider_size_x->setValue(1280);
    Slider_size_y->setValue(1024);

    connect(comBox_size,SIGNAL(currentIndexChanged(int)),this,SLOT(Image_setnumber_size(int)));
    connect(this,SIGNAL(Camera_SetSize_Sign(int)),comBox_size,SLOT(setCurrentIndex(int)));
    connect(SpinBox_size_x,SIGNAL(valueChanged(int)),this,SLOT(Image_setnumber_size_x(int)));
    connect(SpinBox_size_y,SIGNAL(valueChanged(int)),this,SLOT(Image_setnumber_size_y(int)));
    connect(SpinBox_size_x,SIGNAL(valueChanged(int)),Slider_size_x,SLOT(setValue(int)));
    connect(SpinBox_size_y,SIGNAL(valueChanged(int)),Slider_size_y,SLOT(setValue(int)));
    connect(Slider_size_x,SIGNAL(valueChanged(int)),SpinBox_size_x,SLOT(setValue(int)));
    connect(Slider_size_y,SIGNAL(valueChanged(int)),SpinBox_size_y,SLOT(setValue(int)));
    connect(this,SIGNAL(Camera_SetSizeX_Sign(int)),SpinBox_size_x,SLOT(setValue(int)));
    connect(this,SIGNAL(Camera_SetSizeY_Sign(int)),SpinBox_size_y,SLOT(setValue(int)));
    connect(this,SIGNAL(Camera_SetSizeX_Sign(int)),Slider_size_x,SLOT(setValue(int)));
    connect(this,SIGNAL(Camera_SetSizeY_Sign(int)),Slider_size_y,SLOT(setValue(int)));
    connect(Slider_size_x,SIGNAL(valueChanged(int)),this,SLOT(Image_setnumber_sizechange(int)));
    connect(Slider_size_y,SIGNAL(valueChanged(int)),this,SLOT(Image_setnumber_sizechange(int)));

    GridLayout->addWidget(Label_size,1,0,1,10);
    GridLayout->addWidget(comBox_size,1,10,1,10);
    GridLayout->addWidget(Label_size_x,2,0,1,5);
    GridLayout->addWidget(SpinBox_size_x,2,5,1,5);
    GridLayout->addWidget(Slider_size_x,2,10,1,10);
    GridLayout->addWidget(Label_size_y,3,0,1,5);
    GridLayout->addWidget(SpinBox_size_y,3,5,1,5);
    GridLayout->addWidget(Slider_size_y,3,10,1,10);

/*曝光*/
    QLabel *Label_fExposure = new QLabel(SetImage);
    Label_fExposure->setText("曝光时间");
    QSpinBox *SpinBox_fExposure = new QSpinBox;
    QSlider *Slider_fExposure = new QSlider(Qt::Horizontal);
    SpinBox_fExposure->setMaximum(1000);
    SpinBox_fExposure->setMinimum(0);
    Slider_fExposure->setMaximum(1000);
    Slider_fExposure->setMinimum(0);
    SpinBox_fExposure->setValue(0);
    Slider_fExposure->setValue(0);

    connect(SpinBox_fExposure,SIGNAL(valueChanged(int)),Slider_fExposure,SLOT(setValue(int)));
    connect(Slider_fExposure,SIGNAL(valueChanged(int)),SpinBox_fExposure,SLOT(setValue(int)));
    connect(SpinBox_fExposure,SIGNAL(valueChanged(int)),this,SLOT(Image_setnumber_fExposure(int)));

    GridLayout->addWidget(Label_fExposure,4,0,1,5);
    GridLayout->addWidget(SpinBox_fExposure,4,5,1,5);
    GridLayout->addWidget(Slider_fExposure,4,10,1,10);

/*超时时间（毫秒）*/
    QLabel *Label_timeout = new QLabel(SetImage);
    Label_timeout->setText("超时时间");
    QSpinBox *SpinBox_timeout = new QSpinBox;
    QSlider *Slider_timeout = new QSlider(Qt::Horizontal);
    SpinBox_timeout->setMaximum(1000);
    SpinBox_timeout->setMinimum(1);
    Slider_timeout->setMaximum(1000);
    Slider_timeout->setMinimum(1);
    SpinBox_timeout->setValue(100);
    Slider_timeout->setValue(100);

    connect(SpinBox_timeout,SIGNAL(valueChanged(int)),Slider_timeout,SLOT(setValue(int)));
    connect(Slider_timeout,SIGNAL(valueChanged(int)),SpinBox_timeout,SLOT(setValue(int)));
    connect(SpinBox_timeout,SIGNAL(valueChanged(int)),this,SLOT(Image_setnumber_timeout(int)));

    GridLayout->addWidget(Label_timeout,5,0,1,5);
    GridLayout->addWidget(SpinBox_timeout,5,5,1,5);
    GridLayout->addWidget(Slider_timeout,5,10,1,10);

/*图像品质，仅对jpeg格式有效，并且取值范围是[1,100]*/
    QLabel *Label_quality = new QLabel(SetImage);
    Label_quality->setText("图像品质");
    QSpinBox *SpinBox_quality = new QSpinBox;
    QSlider *Slider_quality = new QSlider(Qt::Horizontal);
    SpinBox_quality->setMaximum(100);
    SpinBox_quality->setMinimum(1);
    Slider_quality->setMaximum(100);
    Slider_quality->setMinimum(1);
    SpinBox_quality->setValue(100);
    Slider_quality->setValue(100);

    connect(SpinBox_quality,SIGNAL(valueChanged(int)),Slider_quality,SLOT(setValue(int)));
    connect(Slider_quality,SIGNAL(valueChanged(int)),SpinBox_quality,SLOT(setValue(int)));
    connect(SpinBox_quality,SIGNAL(valueChanged(int)),this,SLOT(Image_setnumber_quality(int)));

    GridLayout->addWidget(Label_quality,6,0,1,5);
    GridLayout->addWidget(SpinBox_quality,6,5,1,5);
    GridLayout->addWidget(Slider_quality,6,10,1,10);

/*按钮*/
    QPushButton *Button_OK = new QPushButton(SetImage);
    Button_OK->setText("拍照并保存");
    connect(Button_OK,SIGNAL(clicked()),this,SLOT(Camera_Photo(void)));

    QPushButton *Button_EXIT = new QPushButton(SetImage);
    Button_EXIT->setText("退出");
    connect(Button_EXIT,SIGNAL(clicked()),SetImage,SLOT(close()));

    GridLayout->addWidget(Button_OK,7,0,2,10);
    GridLayout->addWidget(Button_EXIT,7,10,2,10);
/*显示*/
    SetImage->setLayout(GridLayout);
    SetImage->exec();
}
void MW::Image_set2(void)
{
    QDialog *SetImage = new QDialog(this);
    QGridLayout *GridLayout = new QGridLayout();
    SetImage->setWindowModality(Qt::WindowModal);
    SetImage->setWindowTitle("图片属性");
    SetImage->resize(170,250);

    QLabel *Label_format = new QLabel(SetImage);
    Label_format->setText("格式format");
    QLabel *Label_bits = new QLabel(SetImage);
    Label_bits->setText("位宽bits");
    QLabel *Label_uBytes = new QLabel(SetImage);
    Label_uBytes->setText("字节数uBytes");
    QLabel *Label_iWidth = new QLabel(SetImage);
    Label_iWidth->setText("宽度iWidth");
    QLabel *Label_iHeight = new QLabel(SetImage);
    Label_iHeight->setText("高度iHeight");
    QLabel *Label_fExposure = new QLabel(SetImage);
    Label_fExposure->setText("曝光时间fExposure");
    QLabel *Label_timeout = new QLabel(SetImage);
    Label_timeout->setText("超时时间timeout");
    QLabel *Label_quality = new QLabel(SetImage);
    Label_quality->setText("图像品质quality");

    QLabel *Label_format2 = new QLabel(SetImage);

   switch (camera.pFrame.format) {
       case 0: Label_format2->setText("FORMAT_MONO 黑白图像");break;
       case 1: Label_format2->setText("FORMAT_BAYER_BG BGGR图像");break;
       case 2: Label_format2->setText("FORMAT_BAYER_GB GBRG图像");break;
       case 3: Label_format2->setText("FORMAT_BAYER_GR GRBG图像");break;
       case 4: Label_format2->setText("FORMAT_BAYER_RG RGGB图像");break;
       case 10: Label_format2->setText("FORMAT_BGR24 BGR三通道24比特图像");break;
       case 11: Label_format2->setText("FORMAT_BGR32 BGRA四通道32比特图像");break;
       case 15: Label_format2->setText("FORMAT_RGB32 RGBA四通道32比特图像");break;
       case 20: Label_format2->setText("FORMAT_YUV_411 YUV411");break;
       case 21: Label_format2->setText("FORMAT_YUV_422 YUV422");break;}
    QLabel *Label_bits2 = new QLabel(SetImage);
   switch (camera.pFrame.bits) {
       case 0: Label_bits2->setText("BITS_8 8比特数据");break;
       case 1: Label_bits2->setText("BITS_10 10比特数据");break;
       case 2: Label_bits2->setText("BITS_12 12比特数据");break;
       case 3: Label_bits2->setText("BITS_14 14比特数据");break;
       case 4: Label_bits2->setText("BITS_16 16比特数据");break;}
    QLabel *Label_uBytes2 = new QLabel(SetImage);
    Label_uBytes2->setText(QString::number(camera.pFrame.uBytes));
    QLabel *Label_iWidth2 = new QLabel(SetImage);
    Label_iWidth2->setText(QString::number(camera.pFrame.iWidth));
    QLabel *Label_iHeight2 = new QLabel(SetImage);
    Label_iHeight2->setText(QString::number(camera.pFrame.iHeight));
    QLabel *Label_fExposure2 = new QLabel(SetImage);
    Label_fExposure2->setText(QString::number(Image_setnumber.fExposure));
    QLabel *Label_timeout2 = new QLabel(SetImage);
    Label_timeout2->setText(QString::number(Image_setnumber.timeout));
    QLabel *Label_quality2 = new QLabel(SetImage);
    Label_quality2->setText(QString::number(Image_setnumber.quality));

    GridLayout->addWidget(Label_format,0,0,1,5);
    GridLayout->addWidget(Label_format2,0,5,1,5);
    GridLayout->addWidget(Label_bits,1,0,1,5);
    GridLayout->addWidget(Label_bits2,1,5,1,5);
    GridLayout->addWidget(Label_uBytes,2,0,1,5);
    GridLayout->addWidget(Label_uBytes2,2,5,1,5);
    GridLayout->addWidget(Label_iWidth,3,0,1,5);
    GridLayout->addWidget(Label_iWidth2,3,5,1,5);
    GridLayout->addWidget(Label_iHeight,4,0,1,5);
    GridLayout->addWidget(Label_iHeight2,4,5,1,5);
    GridLayout->addWidget(Label_fExposure,5,0,1,5);
    GridLayout->addWidget(Label_fExposure2,5,5,1,5);
    GridLayout->addWidget(Label_timeout,6,0,1,5);
    GridLayout->addWidget(Label_timeout2,6,5,1,5);
    GridLayout->addWidget(Label_quality,7,0,1,5);
    GridLayout->addWidget(Label_quality2,7,5,1,5);

/*显示*/
    SetImage->setLayout(GridLayout);
    SetImage->show();
}
void MW::Image_setnumber_format(int a) {camera.MW_Close();Image_setnumber.format=a;}
void MW::Image_setnumber_size(int a)
{
    Image_setnumber.size=a;
    if(a==3)return;
    else if (a==0)
    {
        Image_setnumber.size_x=1280;
        Image_setnumber.size_y=1024;
    }
    else if (a==1)
    {
        Image_setnumber.size_x=1024;
        Image_setnumber.size_y=768;
    }
    else if (a==2)
    {
        Image_setnumber.size_x=640;
        Image_setnumber.size_y=512;
    }
    emit Camera_SetSizeX_Sign(Image_setnumber.size_x);
    emit Camera_SetSizeY_Sign(Image_setnumber.size_y);
}
void MW::Image_setnumber_size_x(int a) {Image_setnumber.size_x=a;}
void MW::Image_setnumber_size_y(int a) {Image_setnumber.size_y=a;}
void MW::Image_setnumber_fExposure(int a) {Image_setnumber.fExposure=a;}
void MW::Image_setnumber_sizechange(int a) {if(a!=1280 && a!=1024 && a!=768 && a!=640 && a!=512)emit Camera_SetSize_Sign(3);}
void MW::Image_setnumber_timeout(int a){camera.timeout=uint(a);Image_setnumber.timeout=a;}
void MW::Image_setnumber_quality(int a){Image_setnumber.quality=camera.quality=a;}
void MW::Video_Start_SLOT(){
    if(Video_Start==1)return;
        SDK.VideoBool = 1;
        SDK.menu(11);
        Video_Start=1;
}
void MW::Video_Close_SLOT(){SDK.VideoBool=2;Video_Start=0;std::cout << "camera close" << std::endl;}
void MW::Menu_Choose1(){SDK.menu(1);std::cout << "---------Menu 1 success-------" << std::endl;}
void MW::Menu_Choose2(){SDK.menu(2);std::cout << "---------Menu 2 success-------" << std::endl;}
void MW::Menu_Choose3(){SDK.menu(3);std::cout << "---------Menu 3 success-------" << std::endl;}
void MW::Menu_Choose4(){SDK.menu(4);std::cout << "---------Menu 4 success-------" << std::endl;}
void MW::Menu_Choose5(){SDK.menu(5);std::cout << "---------Menu 5 success-------" << std::endl;}
void MW::Menu_Choose6(){SDK.menu(6);std::cout << "---------Menu 6 success-------" << std::endl;}
void MW::Menu_Choose7(){SDK.menu(7);std::cout << "---------Menu 7 success-------" << std::endl;}
void MW::Menu_Choose8(){SDK.menu(8);std::cout << "---------Menu 8 success-------" << std::endl;}
void MW::Menu_Choose9(){SDK.menu(9);std::cout << "---------Menu 9 success-------" << std::endl;}
void MW::Menu_Choose10(){SDK.menu(10);this->SDK_Verify=0;std::cout << "---------Menu 10 success-------" << std::endl;}
void MW::Menu_XiaoZhunBan(int a){SDK.side_length = double(a);}
void MW::timerUpdate()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    ui->Time_Lcd->display(text);
    emit Camera_Refresh_Sign();
}


