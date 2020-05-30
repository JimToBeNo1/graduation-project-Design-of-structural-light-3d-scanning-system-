#ifndef DLP4500_H
#define DLP4500_H



#include <dlp_sdk.hpp>
#include <common/debug.hpp>
#include <common/returncode.hpp>
#include <common/other.hpp>
#include <common/image/image.hpp>
#include <vector>
#include <dvp.h>
#include <iostream>

#define EXAMPLE_ERROR_1 "EXAMPLE_ERROR_1"
#define EXAMPLE_ERROR_2 "EXAMPLE_ERROR_2"
#define EXAMPLE_ERROR_3 "EXAMPLE_ERROR_3"
#define EXAMPLE_WARNING_1 "EXAMPLE_WARNING_1"
#define EXAMPLE_WARNING_2 "EXAMPLE_WARNING_2"

//相机类型:0 -通用OpenCV相机，1 - PointGrey，…
DLP_NEW_PARAMETERS_ENTRY(CameraType,         "CAMERA_TYPE",  int, 2);
//算法类型:0 -灰度，1 -混合三相，…
DLP_NEW_PARAMETERS_ENTRY(AlgorithmType,      "ALGORITHM_TYPE",  int, 0);

DLP_NEW_PARAMETERS_ENTRY(ConnectIdProjector,           "CONNECT_ID_PROJECTOR", std::string, "0");
DLP_NEW_PARAMETERS_ENTRY(ConnectIdCamera,              "CONNECT_ID_CAMERA",    std::string, "0");


//#define DLP_Dir  "D:/photo"

DLP_NEW_PARAMETERS_ENTRY(ConfigFileProjector,           "CONFIG_FILE_PROJECTOR",                std::string, "D:/photo/src/config/config_projector.txt");
DLP_NEW_PARAMETERS_ENTRY(ConfigFileCamera,              "CONFIG_FILE_CAMERA",                   std::string, "D:/photo/src/config/config_camera.txt");
DLP_NEW_PARAMETERS_ENTRY(ConfigFileCalibProjector,      "CONFIG_FILE_CALIBRATION_PROJECTOR",    std::string, "D:/photo/src/config/calibration_projector.txt");
DLP_NEW_PARAMETERS_ENTRY(ConfigFileCalibCamera,         "CONFIG_FILE_CALIBRATION_CAMERA",       std::string, "D:/photo/src/config/calibration_camera.txt");
DLP_NEW_PARAMETERS_ENTRY(ConfigFileGeometry,            "CONFIG_FILE_GEOMETRY",                 std::string, "D:/photo/src/config/geometry.txt");
DLP_NEW_PARAMETERS_ENTRY(ConfigFileStructuredLight1,    "CONFIG_FILE_STRUCTURED_LIGHT_1",       std::string, "D:/photo/src/config/algorithm_vertical.txt");
DLP_NEW_PARAMETERS_ENTRY(ConfigFileStructuredLight2,    "CONFIG_FILE_STRUCTURED_LIGHT_2",       std::string, "D:/photo/src/config/algorithm_horizontal.txt");

DLP_NEW_PARAMETERS_ENTRY(ContinuousScanning,            "CONTINUOUS_SCANNING", bool, false);

DLP_NEW_PARAMETERS_ENTRY(CalibDataFileProjector,        "CALIBRATION_DATA_FILE_PROJECTOR",      std::string, "D:/photo/src/calibration/data/projector.xml");
DLP_NEW_PARAMETERS_ENTRY(CalibDataFileCamera,           "CALIBRATION_DATA_FILE_CAMERA",         std::string, "D:/photo/src/calibration/data/camera.xml");
DLP_NEW_PARAMETERS_ENTRY(DirCalibData,                  "DIRECTORY_CALIBRATION_DATA",                   std::string, "D:/photo/src/calibration/data/");
DLP_NEW_PARAMETERS_ENTRY(DirCameraCalibImageOutput,     "DIRECTORY_CAMERA_CALIBRATION_IMAGE_OUTPUT",    std::string, "D:/photo/src/calibration/camera_images/");
DLP_NEW_PARAMETERS_ENTRY(DirSystemCalibImageOutput,     "DIRECTORY_SYSTEM_CALIBRATION_IMAGE_OUTPUT",    std::string, "D:/photo/src/calibration/system_images/");
DLP_NEW_PARAMETERS_ENTRY(DirScanDataOutput,             "DIRECTORY_SCAN_DATA_OUTPUT",                   std::string, "D:/photo/src/output/scan_data/");
DLP_NEW_PARAMETERS_ENTRY(DirScanImagesOutput,           "DIRECTORY_SCAN_IMAGES_OUTPUT",                 std::string, "D:/photo/src/output/scan_images/");
DLP_NEW_PARAMETERS_ENTRY(OutputNameImageCameraCalibBoard, "OUTPUT_NAME_IMAGE_CAMERA_CALIBRATION_BOARD", std::string, "camera_calibration_board");
DLP_NEW_PARAMETERS_ENTRY(OutputNameImageCameraCalib,    "OUTPUT_NAME_IMAGE_CAMERA_CALIBRATION", std::string, "camera_calibration_capture_");
DLP_NEW_PARAMETERS_ENTRY(OutputNameImageSystemCalib,    "OUTPUT_NAME_IMAGE_SYSTEM_CALIBRATION", std::string, "system_calibration_capture_");
DLP_NEW_PARAMETERS_ENTRY(OutputNameImageDepthMap,       "OUTPUT_NAME_IMAGE_DEPTHMAP",           std::string, "_depthmap");
DLP_NEW_PARAMETERS_ENTRY(OutputNameXYZPointCloud,       "OUTPUT_NAME_XYZ_POINTCLOUD",           std::string, "_pointcloud");



class dlpclass
{
public:
    dlpclass();
    ~dlpclass();
    bool verify();
    void menu(int);
    void image_file_io();
    void image_pixels();
    void image_window();
    void point_cloud_viewer();
    void returncodes_example();
    void GenerateCameraCalibrationBoard(dlp::Camera *camera,const std::string &camera_calib_settings_file,const std::string &camera_calib_board_name);
    void PrepareProjectorPatterns(dlp::DLP_Platform *projector,const std::string &projector_calib_settings_file,dlp::StructuredLight *structured_light_vertical,const std::string &structured_light_vertical_settings_file,dlp::StructuredLight *structured_light_horizontal,const std::string &structured_light_horizontal_settings_file,const bool previously_prepared,unsigned int *total_pattern_count);
    void CalibrateCamera(dlp::Camera *camera,const std::string &camera_calib_settings_file,const std::string &camera_calib_data_file,const std::string &camera_calib_image_file_names,dlp::DLP_Platform *projector);
    void CalibrateSystem(dlp::Camera *camera,const std::string &camera_calib_settings_file,const std::string &camera_calib_data_file,dlp::DLP_Platform *projector,const std::string &projector_calib_settings_file,const std::string &projector_calib_data_file,const std::string &calib_image_file_names);
    void ScanObject(dlp::Camera *camera,const bool &cam_proj_hw_synchronized,const std::string &camera_calib_data_file,dlp::DLP_Platform *projector,const std::string &projector_calib_data_file,dlp::StructuredLight *structured_light_vertical,dlp::StructuredLight *structured_light_horizontal,const bool &use_vertical,const bool &use_horizontal,const std::string &geometry_settings_file,const bool &continuous_scanning);

    void GenerateCameraCalibrationBoard(double side_length);
    void Video(dlp::Camera *camera);

    dlp::Parameters settings;// 加载设置
    //dlp::OpenCV_Cam     camera_cv;
    //dlp::PG_FlyCap2_C   camera_pg;
    dlp::dvpclass       camera_ds;
    dlp::LCr4500        projector;

    dlp::GrayCode       algo_gray_code_vert;
    dlp::GrayCode       algo_gray_code_horz;
    dlp::ThreePhase     algo_three_phase_vert;
    dlp::ThreePhase     algo_three_phase_horz;

    unsigned int total_pattern_count = 0;
    double side_length = 10;
    unsigned int VideoBool=0;

protected:
    void file_name_add();

private:    
    std::string file_name;
    int file_name_num = 1;
    dlp::ReturnCode ret;                   // 创建错误代码对象
    dlp::Image example_image;              // 创建图像对象
    std::string image_file = file_name;         // 用图像文件名创建字符串
    std::string filename = "D:\\photo\\1.stl";  // 从用户处获取点云的文件名


    // 配置的目的
    AlgorithmType               algorithm_type;
    CameraType                  camera_type;

    ConnectIdProjector          connect_id_projector;
    ConnectIdCamera             connect_id_camera;

    ConfigFileProjector         config_file_projector;
    ConfigFileCamera            config_file_camera;
    ConfigFileCalibProjector    config_file_calib_projector;
    ConfigFileCalibCamera       config_file_calib_camera;
    ConfigFileGeometry          config_file_geometry;
    ConfigFileStructuredLight1  config_file_structured_light_1;
    ConfigFileStructuredLight2  config_file_structured_light_2;

    ContinuousScanning          continuous_scanning;

    CalibDataFileProjector      calib_data_file_projector;
    CalibDataFileCamera         calib_data_file_camera;
    DirCalibData                dir_calib_data;
    DirCameraCalibImageOutput   dir_camera_calib_image_output;
    DirSystemCalibImageOutput   dir_system_calib_image_output;
    DirScanDataOutput           dir_scan_data_output;
    DirScanImagesOutput         dir_scan_images_output;

    OutputNameImageCameraCalibBoard output_name_image_camera_calib_board;
    OutputNameImageCameraCalib  output_name_image_camera_calib;
    OutputNameImageSystemCalib  output_name_image_system_calib;
    OutputNameImageDepthMap     output_name_image_depthmap;
    OutputNameXYZPointCloud     output_name_xyz_pointcloud;


};


#endif // DLP4500_H
