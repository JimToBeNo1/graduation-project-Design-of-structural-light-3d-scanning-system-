#ifndef MW_H
#define MW_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLayout>
#include <QSlider>
#include <QSpinBox>


#include "dvp.h"
#include "dlp4500.h"

#include <QImage>
#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QTime>



namespace Ui {
class MW;
}

class MW : public QMainWindow
{
    Q_OBJECT

public:
    explicit MW(QWidget *parent=nullptr);
    ~MW();

    struct Image_setnumber{
        int format=16;     //格式信号
        int size_x=1280;   //iWidth信号
        int size_y=1024;   //iHeight信号
        int size;          //大小信号
        int fExposure=0;   //曝光信号
        int timeout=100;   //超时时间(dvpGetFrame)
        int quality=100;   //图像品质(dvpSavePicture)
        int x;             //强设iWidth信号
        int y;             //强设iHeight信号
    }Image_setnumber;

private slots:
    void timerUpdate();
    void on_photo_clicked();
    void on_video_clicked();
    void on_calibration_clicked();
    void on_discern_clicked();

    void Camera_Photo();
    void Camera_Video();
    void Camera_Calibration();
    void Camera_Discern();

    void Camera_refresh();
    void Image_setnumber_format(int);
    void Image_setnumber_size(int);
    void Image_setnumber_size_x(int);
    void Image_setnumber_size_y(int);
    void Image_setnumber_fExposure(int);
    void Image_setnumber_sizechange(int);
    void Image_setnumber_timeout(int);
    void Image_setnumber_quality(int);
    void Video_Start_SLOT();
    void Video_Close_SLOT();
    void Menu_Choose1();
    void Menu_Choose2();
    void Menu_Choose3();
    void Menu_Choose4();
    void Menu_Choose5();
    void Menu_Choose6();
    void Menu_Choose7();
    void Menu_Choose8();
    void Menu_Choose9();
    void Menu_Choose10();
    void Menu_XiaoZhunBan(int);


private:
    Ui::MW *ui;
    dlp::dvpclass camera;
    dlpclass SDK;
    int Camera_Count=0;
    bool Video_Start=0;
    bool SDK_Verify=0;

    void Image_set();
    void Image_set2();

signals:
    void Camera_Refresh_Sign(void);
    void Camera_SetSize_Sign(int x);
    void Camera_SetSizeX_Sign(int x);
    void Camera_SetSizeY_Sign(int y);
    void Camera_Photo_Sign(void);
    void Camera_Video_Sign(void);

};

#endif // MW_H
