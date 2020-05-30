#-------------------------------------------------
#
# Project created by QtCreator 2019-07-02T11:16:35
#
#-------------------------------------------------

QT       += core gui

QT       += multimedia
QT       += multimediawidgets

CONFIG += c++11 console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 7-2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/hidapi-master/windows/hid.c \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/calibration/calibration_camera.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/calibration/calibration_data.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/calibration/calibration_projector.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/camera/camera.cpp \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/camera/opencv_cam/opencv_cam.cpp \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/camera/pg_flycap2/pg_flycap2_c.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/capture/capture.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/capture/capture_sequence.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/debug.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/disparity_map.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/image/image.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/image/image_window.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/module.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/other.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/parameters.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/pattern/pattern.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/pattern/pattern_sequence.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/point_cloud/point_cloud.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/point_cloud/point_cloud_window.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/common/returncode.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/dlp_platforms/dlp_platform.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/dlp_platforms/lightcrafter_4500/common.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/dlp_platforms/lightcrafter_4500/dlpc350_api.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/dlp_platforms/lightcrafter_4500/dlpc350_firmware.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/dlp_platforms/lightcrafter_4500/dlpc350_usb.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/dlp_platforms/lightcrafter_4500/lcr4500.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/geometry/geometry.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/structured_light/gray_code/gray_code.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/structured_light/structured_light.cpp \
        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/src/structured_light/three_phase/three_phase.cpp \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/deps/getopt.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/deps/tinycthread.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/clipboard.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/context.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/gamma.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/init.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/input.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/joystick.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/monitor.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/time.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/wgl_context.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/win32_clipboard.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/win32_gamma.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/win32_init.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/win32_joystick.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/win32_monitor.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/win32_time.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/win32_window.c \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK/3rd_party/glfw-3.0.4/src/window.c \
        dlp4500.cpp \
        main.cpp \
        mw.cpp \
        dvp.cpp


HEADERS += \
#        E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0_DLP/3rd_party/glfw-3.0.4/src/glfw_config.h \
        dlp4500.h \
#        iostream \
        mw.h \
        dvp.h \
        DVPCamera.h

FORMS += \
        mw.ui




DVP_DIR = "E:/download/DVP2SDK/library/Visual C++"
LIBS += -L$$DVP_DIR/lib/x64 -lDVPCamera64
INCLUDEPATH += $$DVP_DIR/include

OPENCV_DIR = "D:/opencv/opencv/build/include"
INCLUDEPATH += $$OPENCV_DIR \


DLP_DIR = "E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0/DLP-ALC-LIGHTCRAFTER-SDK"
INCLUDEPATH += $$DLP_DIR/include \
               $$DLP_DIR/3rd_party/glfw-3.0.4/include \
               $$DLP_DIR/3rd_party/glfw-3.0.4/src \
               $$DLP_DIR/3rd_party/glfw-3.0.4/deps \
               $$DLP_DIR/3rd_party/glfw-3.0.4/ \
               $$DLP_DIR/3rd_party/hidapi-master/hidapi \
               $$DLP_DIR/3rd_party/hidapi-master

#               E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0_DLP/3rd_party/glfw-3.0.4/src/
PATH += $$DLP_DIR/src

FLY_DIR = "E:/download/Point Grey Research/FlyCapture2/include"
INCLUDEPATH += $$FLY_DIR

QT_DIR = "E:/download/Qt/Qt5.13.0/Tools/mingw730_64/x86_64-w64-mingw32/lib/"
DLP_MAKE_DIR = "E:/download/TexasInstruments-DLP/DLP-ALC-LIGHTCRAFTER-SDK-2.0_DLP"
LIBS +=\
-L$$DLP_MAKE_DIR/lib/ -lglfw3dll -lglfw3 -llibiomp5md\
-L$$DLP_MAKE_DIR/bin/ -llibopencv_imgproc2413 -llibopencv_highgui2413 -llibopencv_flann2413 -llibopencv_features2d2413 -llibopencv_calib3d2413 -llibopencv_core2413\
-L$$DLP_MAKE_DIR/bin/ -lFlyCapture2 -lFlyCapture2_C \
-L$$DLP_MAKE_DIR/lib/ -llibglfw3 \
-L$$QT_DIR -llibopengl32 -llibsetupapi
#-L$$DLP_MAKE_DIR/lib/ -llibDLP_SDK




#win32: CONFIG ( release, debug | release){ LIBS +=\}
#else: win32: CONFIG (debug, debug | release){ LIBS +=\}
#else: unix{ LIBS +=\}




