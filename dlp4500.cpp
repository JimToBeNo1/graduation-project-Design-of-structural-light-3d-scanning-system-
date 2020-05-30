#include "dlp4500.h"


dlpclass::dlpclass()
{

    settings.Load("DLP_LightCrafter_4500_3D_Scan_Application_Config.txt");
//D://photo//src//
    // 检索的设置
    settings.Get(&algorithm_type);
    settings.Get(&camera_type);
    settings.Get(&connect_id_projector);
    settings.Get(&connect_id_camera);
    settings.Get(&config_file_projector);
    settings.Get(&config_file_camera);
    settings.Get(&config_file_calib_projector);
    settings.Get(&config_file_calib_camera);
    settings.Get(&config_file_geometry);
    settings.Get(&config_file_structured_light_1);
    settings.Get(&config_file_structured_light_2);
    settings.Get(&continuous_scanning);
    settings.Get(&calib_data_file_projector);
    settings.Get(&calib_data_file_camera);
    settings.Get(&dir_calib_data);
    settings.Get(&dir_camera_calib_image_output);
    settings.Get(&dir_system_calib_image_output);
    settings.Get(&dir_scan_data_output);
    settings.Get(&dir_scan_images_output);
    settings.Get(&output_name_image_camera_calib_board);
    settings.Get(&output_name_image_camera_calib);
    settings.Get(&output_name_image_system_calib);
    settings.Get(&output_name_image_depthmap);
    settings.Get(&output_name_xyz_pointcloud);
}

dlpclass::~dlpclass()
{

}

bool dlpclass::verify()
{
    // 验证支持列表中的摄像机和算法类型
    if(camera_type.Get() != 2)
    {   std::cout << "Unsupported CAMERA_TYPE set in the configuration file. Modify DLP_LightCrafter_3D_Scan_Application_Config.txt" << std::endl
             << "Press any key to exit..." << std::endl;
        std::cin.get();
        return 0;
    }

    if(algorithm_type.Get() > 1)
    {   std::cout << "Unsupported ALGORITHM_TYPE set in the configuration file. Modify DLP_LightCrafter_3D_Scan_Application_Config.txt" << std::endl
             << "Press any key to exit..." << std::endl;
        std::cin.get();
        return 0;
    }

    //连接投影仪
    ret = dlp::DLP_Platform::ConnectSetup(projector,connect_id_projector.Get(),config_file_projector.Get(),true);
    if(ret.hasErrors())
    {   std::cout << "\n\nPlease resolve the LightCrafter connection issue before proceeding to next step..." << std::endl;
        return 0;}


    //根据用户选择连接相机
    if(camera_type.Get() == 2)
    {
        ret = dlp::Camera::ConnectSetup(camera_ds,connect_id_camera.Get(),config_file_camera.Get(),true);
        std::cout << "ret = " << ret << std::endl ;
        if(ret.hasErrors())
        {   std::cout << "\n\nPlease resolve the camera connection issue before proceeding to next step..." << std::endl << ret.ToString () << std::endl;
            return 0;}
        }
    else
    {   std::cout << "error :dlpclass::verifystd::" << std::endl;//  执行不到的代码
        return 0;}

    return 1;
}

/*
0:退出
1:生成相机校准板，并输入特征测量值
2:准备DLP LightCrafter 4500(每个投影仪一次)
3:准备校准和扫描系统
4:校准相机
5:校准系统
6:执行扫描(仅垂直模式)
7:执行扫描(仅水平模式)
8:执行扫描(垂直和水平模式)
9.重新连接照相机和投影仪
*/
void dlpclass::menu(int menu_select)
{
    if(camera_type.Get() == 2)
    {
        switch(menu_select){
        case 0:
            break;

//1:生成相机校准板，并输入特征测量值
        case 1:
        {
//            GenerateCameraCalibrationBoard(10);
            GenerateCameraCalibrationBoard(&camera_ds,
                                           config_file_calib_camera.Get(),
                                           dir_camera_calib_image_output.Get() + output_name_image_camera_calib_board.Get() + ".bmp");
            //"D://photo//camera_calibration_board//camera_calibration_board.bmp"
            break;
        }

//2:准备DLP LightCrafter 4500(每个投影仪一次)
        case 2:
        {
            PrepareProjectorPatterns(&projector,
                                     config_file_calib_projector.Get(),
                                     &algo_three_phase_vert,
                                     config_file_structured_light_1.Get(),
                                     &algo_three_phase_horz,
                                     config_file_structured_light_2.Get(),
                                     false,    // 固件将上传
                                     &total_pattern_count);
            break;
        }

//3:准备校准和扫描系统
        case 3:
        {
            PrepareProjectorPatterns(&projector,
                                     config_file_calib_projector.Get(),
                                     &algo_gray_code_vert,
                                     config_file_structured_light_1.Get(),
                                     &algo_gray_code_horz,
                                     config_file_structured_light_2.Get(),
                                     true, // 固件将不会被上传
                                     &total_pattern_count);
            break;
        }

//4:校准相机
        case 4:
        {
            CalibrateCamera(&camera_ds,
                            config_file_calib_camera.Get(),
                            calib_data_file_camera.Get(),
                            dir_camera_calib_image_output.Get() +
                            output_name_image_camera_calib.Get(),
                            &projector);
            break;
        }

//5:校准系统
        case 5:
        {
            CalibrateSystem(&camera_ds,
                            config_file_calib_camera.Get(),
                            calib_data_file_camera.Get(),
                            &projector,
                            config_file_calib_projector.Get(),
                            calib_data_file_projector.Get(),
                            dir_system_calib_image_output.Get() +
                            output_name_image_system_calib.Get());
            break;
        }

//6:执行扫描(仅垂直模式)
        case 6:
        {
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_ds,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           true,
                           false,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_ds,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           true,
                           false,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            break;
            }

//7:执行扫描(仅水平模式)
        case 7:
        {
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_ds,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           false,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_ds,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           false,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            break;
            }

//8:执行扫描(垂直和水平模式)
        case 8:
        {
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_ds,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           true,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_ds,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           true,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            break;
        }

//9.重新连接照相机和投影仪
        case 9:
        {
            camera_ds.Disconnect();
            projector.Disconnect();
            dlp::DLP_Platform::ConnectSetup(projector,connect_id_projector.Get(),config_file_projector.Get(),true);
            dlp::Camera::ConnectSetup(camera_ds,connect_id_camera.Get(),config_file_camera.Get(),true);
            break;
            }
//断连
        case 10:
        {
            camera_ds.Disconnect();
            projector.Disconnect();
            break;
            } 
//Video
        case 11:
        {
            Video(&camera_ds);
            break;
            }
        default:
        {
            std::cout << "Invalid menu selection! \n" << std::endl;
            break;
            }
        }
    }
}

//1:生成相机校准板，并输入特征测量值
void dlpclass::GenerateCameraCalibrationBoard(double side_length)
{
    std::cout << std::endl << "<<<<<<<<<<<<<<<<<<<<<< Generate Camera Calibration Board >>>>>>>>>>>>>>>>>>>>>>" << std::endl;
    dlp::Camera *camera = &camera_ds;
    const std::string &camera_calib_settings_file = config_file_calib_camera.Get();
    const std::string &camera_calib_board_name = "D://photo//camera_calibration_board//camera_calibration_board.bmp";
     // 摄像机标定的变量
    dlp::Calibration::Camera    camera_calib;
    dlp::Parameters             camera_calib_settings;
    dlp::Image                  camera_calib_board_image;

    // 检查相机是否为空
    if(!camera) return;

    // 负载校准设置
    std::cout << "Loading camera calibration settings..." << std::endl;
    if(camera_calib_settings.Load(camera_calib_settings_file).hasErrors())
    {
        //相机校准设置文件未加载
        std::cout << "Could NOT load camera calibration settings!" << std::endl << ret.ToString() << std::endl;
        return;}

    //设置当前相机的相机校准
    camera_calib.SetCamera((*camera));

    std::cout << "Setting up camera calibration..." << std::endl;
    ret = camera_calib.Setup(camera_calib_settings);
    if(ret.hasErrors())
    {
        std::cout << "Camera calibration setup FAILED!" << std::endl << ret.ToString() << std::endl;
        return;}

    // 生成并保存校准图像
    std::cout << "Generating camera calibration board..." << std::endl;
    camera_calib.GenerateCalibrationBoard(&camera_calib_board_image);

    // 保存校准板
    std::cout << "Saving calibration board..." << std::endl;
    camera_calib_board_image.Save(camera_calib_board_name);


    std::cout << "\nPlease print the camera calibration board and attach it to a flat surface The calibration board image can be found in: "
              << camera_calib_board_name <<
              "\n\nOnce the calibration image has been printed and attached to a flat surface, measure the size of the square on the board \n \
               \nNOTE: Enter the measurement in the units desired for the point cloud (i.e. mm, in, cm, etc.)\n  \
               \nNOTE: Both camera and system calibrations must be redone! " << std::endl;

      // 检索测量
    std::cout << "Enter the length of the square (do NOT include units): " << std::endl;
    std::cout << "Saving feature distances to calibration settings file..."<<std::endl;
    camera_calib_settings.Set(dlp::Calibration::Parameters::BoardFeatureRowDistance(side_length));
    camera_calib_settings.Set(dlp::Calibration::Parameters::BoardFeatureColumnDistance(side_length));
    camera_calib_settings.Save(camera_calib_settings_file);

}

void dlpclass::GenerateCameraCalibrationBoard(dlp::Camera       *camera,           //相机
                                              const std::string &camera_calib_settings_file, //相机校准设置
                                              const std::string &camera_calib_board_name)  //校准板保存文件位置
{
    std::cout << std::endl << "<<<<<<<<<<<<<<<<<<<<<< Generate Camera Calibration Board >>>>>>>>>>>>>>>>>>>>>>" << std::endl;

     // 摄像机标定的变量
    dlp::Calibration::Camera    camera_calib;
    dlp::Parameters             camera_calib_settings;
    dlp::Image                  camera_calib_board_image;

    // 检查相机是否为空
    if(!camera) return;

    // 负载校准设置
    std::cout << "Loading camera calibration settings..." << std::endl;
    if(camera_calib_settings.Load(camera_calib_settings_file).hasErrors())
    {
        //相机校准设置文件未加载
        std::cout << "Could NOT load camera calibration settings!" << std::endl << ret.ToString() << std::endl;
        return;}

    //设置当前相机的相机校准
    camera_calib.SetCamera((*camera));

    std::cout << "Setting up camera calibration..." << std::endl;
    ret = camera_calib.Setup(camera_calib_settings);
    if(ret.hasErrors())
    {
        std::cout << "Camera calibration setup FAILED!" << std::endl << ret.ToString() << std::endl;
        return;}

    // 生成并保存校准图像
    std::cout << "Generating camera calibration board..." << std::endl;
    camera_calib.GenerateCalibrationBoard(&camera_calib_board_image);

    // 保存校准板
    std::cout << "Saving calibration board..." << std::endl;
    camera_calib_board_image.Save(camera_calib_board_name);

    // 等待用户打印校准板


    std::cout << "\nPlease print the camera calibration board and attach it to a flat surface The calibration board image can be found in: "
              << camera_calib_board_name
              << "\n\nOnce the calibration image has been printed and attached to a flat surface, measure the size of the square on the board \n \
                  \nNOTE: Enter the measurement in the units desired for the point cloud (i.e. mm, in, cm, etc.)\n  \
                  \nNOTE: Both camera and system calibrations must be redone! " << std::endl;

      // 检索测量
    std::cout << "Enter the length of the square (do NOT include units): " ;
    //std::cin >> side_length;
    std::cout << std::endl;
    std::cout << "Saving feature distances to calibration settings file..."<<std::endl;
    camera_calib_settings.Set(dlp::Calibration::Parameters::BoardFeatureRowDistance(side_length));
    camera_calib_settings.Set(dlp::Calibration::Parameters::BoardFeatureColumnDistance(side_length));
    camera_calib_settings.Save(camera_calib_settings_file);

}

//2:准备DLP LightCrafter 4500(每个投影仪一次) previously_prepared=false
//3:准备校准和扫描系统                        previously_prepared=true
void dlpclass::PrepareProjectorPatterns(dlp::DLP_Platform    *projector,
                                        const std::string    &projector_calib_settings_file,
                                        dlp::StructuredLight *structured_light_vertical,
                                        const std::string    &structured_light_vertical_settings_file,
                                        dlp::StructuredLight *structured_light_horizontal,
                                        const std::string    &structured_light_horizontal_settings_file,
                                        const bool           previously_prepared,
                                        unsigned int         *total_pattern_count)
{
    std::cout << std::endl << std::endl << "<<<<<<<<<<<<<<<<<<<<<< Load DLP LightCrafter 4500 Firmware >>>>>>>>>>>>>>>>>>>>>>" << std::endl;

    // 投影仪标定变量
    dlp::Calibration::Projector projector_calib;
    dlp::Parameters             projector_calib_settings;

    // 结构光序列
    dlp::Pattern::Sequence calibration_patterns;
    dlp::Pattern::Sequence vertical_patterns;
    dlp::Pattern::Sequence horizontal_patterns;
    dlp::Pattern::Sequence all_patterns;

    // 结构光的设置
    dlp::Parameters         structured_light_vertical_settings;
    dlp::Parameters         structured_light_horizontal_settings;

    projector_calib.SetDebugEnable(false);


    // 检查投影仪是否无效
    if(!projector) return;

    // 检查投影仪是否连接好
    if(!projector->isConnected())
    {
        std::cout << "Projector NOT connected!" << std::endl;
        return;
    }

    // 投影仪校准设置
    std::cout << "Loading projector calibration settings..." << std::endl;
    if(projector_calib_settings.Load(projector_calib_settings_file).hasErrors())
    {
        //投影仪校准设置文件未加载
        std::cout << "Could NOT load projector calibration settings!" << std::endl << ret.ToString() << std::endl;
        return;
    }


    //设置投影仪校准为当前投影仪
    projector_calib.SetDlpPlatform((*projector));

    // 添加临时相机分辨率设置
    projector_calib_settings.Set(dlp::Calibration::Parameters::ImageColumns(1000));
    projector_calib_settings.Set(dlp::Calibration::Parameters::ImageRows(1000));


    std::cout << "Setting up projector calibration..." << std::endl;
    ret = projector_calib.Setup(projector_calib_settings);
    if(ret.hasErrors())
    {
        std::cout << "Projector calibration setup FAILED: " << std::endl << ret.ToString() << std::endl;
        return;
    }

    //创建校准图像并转换为单色
    std::cout << "Generating projector calibration board..." << std::endl;
    dlp::Image   projector_calibration_board_image;
    projector_calib.GenerateCalibrationBoard(&projector_calibration_board_image);
    projector_calibration_board_image.ConvertToMonochrome();

    // 创建校准模式
    std::cout << "Generating projector calibration pattern..." << std::endl;
    dlp::Pattern calib_pattern;
    calib_pattern.bitdepth  = dlp::Pattern::Bitdepth::MONO_1BPP;
    calib_pattern.color     = dlp::Pattern::Color::WHITE;
    calib_pattern.data_type = dlp::Pattern::DataType::IMAGE_DATA;
    calib_pattern.image_data.Create(projector_calibration_board_image);

    // 将校准模式添加到校准模式序列中
    calibration_patterns.Add(calib_pattern);

    // 设置结构光模式
    if(!structured_light_vertical)   return;
    if(!structured_light_horizontal) return;

    // 加载垂直结构光设置
    std::cout << "Loading vertical structured light settings..." << std::endl;
    if(structured_light_vertical_settings.Load(structured_light_vertical_settings_file).hasErrors())
    {
        // 结构化光设置文件未加载
        std::cout << "Structured light settings did NOT load successfully" << std::endl;
        return;
    }

    // 加载水平结构光设置
    std::cout << "Loading horizontal structured light settings..." << std::endl;
    if(structured_light_horizontal_settings.Load(structured_light_horizontal_settings_file).hasErrors()){
        //结构化光设置文件未加载
        std::cout << "Structured light settings did NOT load successfully" << std::endl;
        return;
    }

    structured_light_horizontal->SetDlpPlatform((*projector));
    structured_light_vertical->SetDlpPlatform((*projector));

    std::cout << "Setting up vertical structured light module..." << std::endl <<
            structured_light_vertical->Setup(structured_light_vertical_settings).ToString() << std::endl <<
            "Setting up horizontal structured light module..." << std::endl <<
            structured_light_horizontal->Setup(structured_light_horizontal_settings).ToString() << std::endl;

    // 生成模式序列
    std::cout << "Generating vertical structured light module patterns..." << std::endl;
    structured_light_vertical->GeneratePatternSequence(&vertical_patterns);

    std::cout << "Generating horizontal structured light module patterns..." << std::endl;
    structured_light_horizontal->GeneratePatternSequence(&horizontal_patterns);

    // 组合所有模式序列
    std::cout << "Combining all patterns for projector..." << std::endl;
    all_patterns.Add(calibration_patterns);
    all_patterns.Add(vertical_patterns);
    all_patterns.Add(horizontal_patterns);

    //如果previously_prepared是假的，模块应该假设投影仪之前没有准备好
    //这对于LightCrafter 4500很重要，因为映像存储在固件中
    //因此固件必须上传准备投影仪，但这只需要发生一次
    dlp::Parameters force_preparation;
    force_preparation.Set(dlp::DLP_Platform::Parameters::SequencePrepared(previously_prepared));
    projector->Setup(force_preparation);

    //如果需要上传固件，则显示调试消息以便观察进程
    projector->SetDebugEnable(!previously_prepared);


    // 准备投影仪
    std::cout << "Preparing projector for calibration and structured light..." << std::endl;
    ret = projector->PreparePatternSequence(all_patterns);
    projector->SetDebugEnable(false);

    if( ret.hasErrors()){
        std::cout << "Projector prepare sequence FAILED: " << std::endl << ret.ToString() << std::endl;
        (*total_pattern_count) = 0;
    }

    std::cout << "Projector prepared" << std::endl;
    (*total_pattern_count) = all_patterns.GetCount();
}

//4:校准相机
void dlpclass::CalibrateCamera(dlp::Camera       *camera,
                               const std::string      &camera_calib_settings_file,
                               const std::string      &camera_calib_data_file,
                               const std::string      &camera_calib_image_file_names,
                               dlp::DLP_Platform *projector)
{
    std::cout << std::endl << std::endl << "<<<<<<<<<<<<<<<<<<<<<< Calibrating the Camera >>>>>>>>>>>>>>>>>>>>>>" << std::endl;

    // 摄像机标定的变量
    dlp::Calibration::Camera    camera_calib;
    dlp::Calibration::Data      camera_calib_data;
    dlp::Parameters             camera_calib_settings;

    // 检查相机是否为空
    if(!camera) return;

    // 检查摄像头是否连接好
    if(!camera->isConnected())
    {
        std::cout << "Camera NOT connected!" << std::endl;
        return;
    }

    // 检查投影仪是否无效
    if(!projector) return;

    // 检查投影仪是否连接好
    if(!projector->isConnected())
    {
        std::cout << "Projector NOT connected!" << std::endl;
        return;
    }

    // 负载校准设置
    std::cout << "Loading camera calibration settings..." << std::endl;
    if(camera_calib_settings.Load(camera_calib_settings_file).hasErrors())
    {
        // 相机校准设置文件未加载
        std::cout << "Could NOT load camera calibration settings!" << std::endl << ret.ToString() << std::endl;
        return;
    }

    // 设置当前相机的相机校准
    camera_calib.SetCamera((*camera));

    std::cout << "Setting up camera calibration" << std::endl;
    ret = camera_calib.Setup(camera_calib_settings);
    if(ret.hasErrors())
    {
        std::cout << "Camera calibration setup FAILED: " << std::endl << ret.ToString() << std::endl;
        return;
    }

    // 投射一个白色图案来照亮校准板
    std::cout << "Projecting white pattern to illuminate calibration board" << std::endl;
    projector->ProjectSolidWhitePattern();


    // 获取所需校准图像的数量
    unsigned int boards_success;
    unsigned int boards_required;
    camera_calib.GetCalibrationProgress(&boards_success,&boards_required);

    std::cout << "Calibrating camera..." << std::endl <<
            "Calibration boards required = " << boards_required << std::endl <<
            "Calibration boards captured = " << boards_success << std::endl;


    // 显示校准说明
    std::cout << "\nRead the following instructions in their entirety!" << std::endl <<
                 "Camera calibration instructions:" << std::endl <<
                 "1. After the live view has started, place the calibration board so it is completely visible within the camera frame and is stationary" << std::endl <<
                 "   - The board should be placed at the desired scanning distance illuminated and sharp" << std::endl <<
                 "   - Lock the aperture and focus in place if possible" << std::endl <<
                 "   - If the camera focus or aperture is adjusted, the calibration must be redone" << std::endl <<
                 "3. Select the live view window and press the SPACE_BAR key to capture the calibration board's current position" << std::endl <<
                 "4. Move the calibration board and repeat step 3 until calibration is complete" << std::endl <<
                 "   - Capture images with the board at different distances from the camera" << std::endl <<
                 "   - Capture images with the board at different positions in the camera frame" << std::endl <<
                 "   - The position of the camera can be changed as well rather than moving the board" << std::endl <<
                 "\nNOTE: Press ESC key to quit the calibration routine" << std::endl;
    dlp::CmdLine::PressEnterToContinue("Press ENTER after reading the above instructions...");

    // 如果还没有启动，请启动相机
    if(!camera->isStarted()){
        if(camera->Start().hasErrors()){
            dlp::CmdLine::Print("Could NOT start camera! \n");
            return;
        }
        else{
            dlp::CmdLine::Print("Started camera. \n");
        }
    }

    // 给相机一些时间来填充图像缓冲区
    dlp::Time::Sleep::Milliseconds(50);

    //开始捕捉图像进行相机校准
    dlp::Image          camera_printed_board;
    dlp::Image::Window  camera_view;

    // 打开相机视图
    camera_view.Open("Camera Calibration - press SPACE to capture or ESC to exit");

    // 捕获图像，直到足够多的板已成功添加
    while(boards_success < boards_required){

        std::cout << "boards_success : " << boards_success << "  boards_required : " << boards_required << std::endl;

        // 等待空格键添加校准图像
        unsigned int return_key = 0;
        while(return_key != ' '){
            camera_printed_board.Clear();               // 清除图像对象

            camera->GetFrame(&camera_printed_board);    // 捕获最新的相框
            camera_view.Update(camera_printed_board);   // 显示图像
            camera_view.WaitForKey(16,&return_key);     // 等待按键被按下或50ms超时
            if(return_key == 27) break;                 // ESC键被按下
        }

        // ESC键被按下
        if(return_key == 27){

            // 关闭图像窗口
            camera_view.Close();

            // 停止相机捕捉
            if(camera->Stop().hasErrors()){
                std::cout << "Camera failed to stop! Exiting calibration routine..." << std::endl;
            }

            // 退出校准功能
            std::cout << "Exiting camera calibration..." << std::endl;
            return;
        }

        // 添加校准板图像
        bool success = false;
        camera_printed_board.ConvertToMonochrome(); // 转换图像为单色
        camera_calib.AddCalibrationBoard(camera_printed_board,&success);

        // 更新状态
        camera_calib.GetCalibrationProgress(&boards_success,&boards_required);

        // 显示消息
        if(success){
            std::cout << "Calibration board successfully added! Captured " << dlp::Number::ToString(boards_success) << " of "  << dlp::Number::ToString(boards_required) << std::endl;
            camera_printed_board.Save(camera_calib_image_file_names + dlp::Number::ToString(boards_success) + ".bmp");
        }
        else{
            std::cout << "\nCalibration board NOT found! Please check the following:" << std::endl <<
                         "- All internal corners of the calibration board are visible within the camera frame" << std::endl <<
                         "- The calibration board does not have any glare" << std::endl <<
                         "- The calibration board is well illuminated" << std::endl <<
                         "- The calibration board is in focus" << std::endl;
        }
    }

    // 释放图像内存
    camera_printed_board.Clear();

    // 校准相机
    double reprojection_error;
    ret = camera_calib.Calibrate(&reprojection_error);
    if(!ret.hasErrors()){
        std::cout << "Camera was successfully calibrated!" << std::endl <<
                     "Reprojection error (closer to zero is better) = " << reprojection_error << std::endl;

        // 获取摄像机校准
        camera_calib.GetCalibrationData(&camera_calib_data);

        // 保存相机校准数据
        camera_calib_data.Save(camera_calib_data_file);
        std::cout << "Camera calibration data saved to :" << camera_calib_data_file << std::endl;
    }
    else{
        std::cout << "Camera calibration FAILED!" << std::endl;
    }

    return;
}



//5:校准系统
void dlpclass::CalibrateSystem(dlp::Camera       *camera,
                               const std::string      &camera_calib_settings_file,
                               const std::string      &camera_calib_data_file,
                               dlp::DLP_Platform *projector,
                               const std::string      &projector_calib_settings_file,
                               const std::string      &projector_calib_data_file,
                               const std::string      &calib_image_file_names)
{
    // 摄像机标定的变量
    dlp::Calibration::Camera    camera_calib;
    dlp::Calibration::Data      camera_calib_data;
    dlp::Parameters             camera_calib_settings;


    // 投影仪标定变量
    dlp::Calibration::Projector projector_calib;
    dlp::Calibration::Data      projector_calib_data;
    dlp::Parameters             projector_calib_settings;

    camera_calib.SetDebugEnable(false);
    projector_calib.SetDebugEnable(false);

    // 检查相机是否为空
    if(!camera) return;

    // 检查摄像头是否连接好
    if(!camera->isConnected())
    {
        std::cout << "Camera NOT connected! \n" << std::endl;
        return;
    }

    // 检查投影仪是否无效
    if(!projector) return;

    // 检查投影仪是否连接好
    if(!projector->isConnected())
    {
        std::cout << "Projector NOT connected! \n" << std::endl;
        return;
    }

    // 检查相机校准数据
    bool camera_calib_exists = true;
    camera_calib_data.Load(camera_calib_data_file);
    if(camera_calib.SetCalibrationData(camera_calib_data).hasErrors()) camera_calib_exists = false;

    // 相机校准设置
    std::cout << "Loading camera calibration settings..." << std::endl;
    if(camera_calib_settings.Load(camera_calib_settings_file).hasErrors())
    {
        // 相机校准设置文件未加载
        std::cout << "Could NOT load camera calibration settings!" << std::endl << ret.ToString() << std::endl;
        return;
    }

    // 投影仪校准设置
    std::cout << "Loading projector calibration settings..." << std::endl;
    if(projector_calib_settings.Load(projector_calib_settings_file).hasErrors())
    {
        // 投影仪校准设置文件未加载
        std::cout << "Could NOT load projector calibration settings!" << std::endl <<ret.ToString() << std::endl;
        return;
    }

    //用投影仪所需的数字调整相机校准板
    dlp::Calibration::Parameters::BoardCount projector_boards_required;
    projector_calib_settings.Get(&projector_boards_required);
    camera_calib_settings.Set(projector_boards_required);

    // 设置当前相机的相机校准
    camera_calib.SetCamera((*camera));
    projector_calib.SetCamera((*camera));
    projector_calib.SetDlpPlatform((*projector));

    std::cout << "Setting up camera calibration..." << std::endl;
    ret = camera_calib.Setup(camera_calib_settings);
    if(ret.hasErrors())
    {
        std::cout << "Camera calibration setup FAILED: " << std::endl << ret.ToString() << std::endl;
        return;
    }

    std::cout << "Setting up projector calibration..." << std::endl;
    ret = projector_calib.Setup(projector_calib_settings);
    if(ret.hasErrors())
    {
        std::cout << "Projector calibration setup FAILED: " << std::endl << ret.ToString() << std::endl;
        return;
    }

    // 如果还没有启动，请启动相机
    if(!camera->isStarted())
    {
        if(camera->Start().hasErrors())
        {
            std::cout << "Could NOT start camera! " << std::endl;
            return;
        }
    }

    // 项目校准模式聚焦投影仪
    ret = projector->DisplayPatternInSequence(0,true);
    if(ret.hasErrors())
    {
        std::cout << ret.ToString() << std::endl;
        return;
    }

    dlp::Time::Sleep::Milliseconds(100);
    std::cout << std::endl <<"Place the calibration board at the desired scanning distance and focus the projector..." << std::endl << std::endl;

    // 获取所需校准图像的数量
    unsigned int cam_boards_success;
    unsigned int cam_boards_required;
    unsigned int proj_boards_success;
    unsigned int proj_boards_required;

    camera_calib.GetCalibrationProgress(&cam_boards_success,&cam_boards_required);
    projector_calib.GetCalibrationProgress(&proj_boards_success,&proj_boards_required);

    // 显示校准说明
    std::cout << "Read the following instructions in their entirety!" << std::endl <<
                 "System calibration instructions:" << std::endl <<
                 "1. After the live view has started, place the calibration board at the furthest desired scanning distance" << std::endl <<
                 "   - The entire projection must fit on the calibration board!" << std::endl <<
                 "2. Place the camera in a position so that the projection is completely visible within the camera frame" << std::endl <<
                 "   - The board should be placed at the desired scanning distance" << std::endl <<
                 "3. Lock the projector and camera in place relative to each other if possible" << std::endl <<
                 "   - If the camera or projector move relative to each other, the system calibration must be redone" << std::endl <<
                 "4. Select the live view window and press the SPACE_BAR key to capture the calibration board's current position" << std::endl <<
                 "5. Move the calibration board and repeat step 4 until calibration is complete" << std::endl <<
                 "   - Capture images with the board at different distances from the projector" << std::endl <<
                 "   - The projected pattern must always fit on the calibration board" << std::endl <<
                 "   - Both the projected pattern and printed pattern must be viewable by the camera" << std::endl <<
                 "   - The position of the camera and projector relative to each other should NOT be changed!" << std::endl;
    dlp::CmdLine::PressEnterToContinue("\nNOTE: Press ESC key to quit the calibration routine\n");

    // 开始捕捉图像进行相机校准
    dlp::Image          camera_printed_board;
    dlp::Image          projector_camera_combo;
    dlp::Image::Window  camera_view;
    unsigned int        return_key = 0;

    // 打开相机实时视图
    camera_view.Open("System Calibration - press SPACE to capture or ESC to exit");

    while(proj_boards_success < proj_boards_required)
    {
        // 先拿相机板
        while(cam_boards_success < cam_boards_required)
        {
            // 投影白色图案，照亮相机校准板
            projector->ProjectSolidWhitePattern();
            dlp::Time::Sleep::Milliseconds(250);

            // 等待空格键添加校准图像
            while(return_key != ' ')
            {
                camera_printed_board.Clear();
                camera->GetFrame(&camera_printed_board);
                camera_view.Update(camera_printed_board);
                camera_view.WaitForKey(16,&return_key);
                if(return_key == 27) break;
            }

            if(return_key == 27){
                camera_view.Close();
                if(camera->Stop().hasErrors())
                    std::cout << "Camera failed to stop!" << std::endl;
               std:: cout <<"Exiting calibration..." << std::endl;
                return;
            }

            // 重置返回键
            return_key = 0;

            // 添加校准板图像
            bool success;
            camera_printed_board.ConvertToMonochrome();
            camera_calib.AddCalibrationBoard(camera_printed_board,&success);

            // 更新状态
            camera_calib.GetCalibrationProgress(&cam_boards_success,&cam_boards_required);

            // 显示消息
            if(success)
            {
                std::cout <<"Camera calibration board successfully added! Captured " + dlp::Number::ToString(cam_boards_success) + " of " + dlp::Number::ToString(cam_boards_required) << std::endl;
                camera_printed_board.Save(calib_image_file_names + "camera_" + dlp::Number::ToString(cam_boards_success) + ".bmp");

                // 现在就把投影仪校准好
                projector->DisplayPatternInSequence(0,true);
                dlp::Time::Sleep::Milliseconds(250);

                camera->GetFrame(&projector_camera_combo);
                camera_view.Update(projector_camera_combo);
                camera_view.WaitForKey(1,&return_key);

                dlp::Image projector_black;
                projector->ProjectSolidBlackPattern();
                dlp::Time::Sleep::Milliseconds(250);

                camera->GetFrame(&projector_black);
                camera_view.Update(projector_black);
                camera_view.WaitForKey(1,&return_key);


                // 添加打印板和组合板
                dlp::Image projector_pattern;
                projector_camera_combo.ConvertToMonochrome();
                projector_black.ConvertToMonochrome();

                projector_calib.RemovePrinted_AddProjectedBoard(camera_printed_board,
                                                                projector_black,
                                                                projector_camera_combo,
                                                                &projector_pattern,
                                                                &success);
                projector_black.Clear();

                // 更新状态
                projector_calib.GetCalibrationProgress(&proj_boards_success,&proj_boards_required);

                if(success)
                {
                    std::cout << "Projector calibration board successfully added! Captured "  + dlp::Number::ToString(proj_boards_success) + " of " + dlp::Number::ToString(proj_boards_required) << std::endl;
                    projector_pattern.Save(calib_image_file_names + "projector_pattern_" + dlp::Number::ToString(proj_boards_success) +".bmp");
                    projector_camera_combo.Save(calib_image_file_names + "projector_camera_combo_" + dlp::Number::ToString(proj_boards_success) +".bmp");
                }
                else
                {
                    std::cout << "Projector calibration board NOT found! Please check the following:" << std::endl <<
                                 "- All internal corners of the projected calibration board are visible within the camera frame" << std::endl <<
                                 "- The projector calibration board does NOT fall off of the printed calibration board" << std::endl <<
                                 "- The calibration board does not have any glare" << std::endl <<
                                 "- The calibration board is well illuminated" << std::endl <<
                                 "- The calibration board is in focus" << std::endl;

                    // 删除最近添加的相机校准板
                    camera_calib.RemoveLastCalibrationBoard();
                    camera_calib.GetCalibrationProgress(&cam_boards_success,&cam_boards_required);
                }
            }
            else
                std::cout << "Calibration board NOT found! Please check the following:" << std::endl <<
                             "- All internal corners of the calibration board are visible within the camera frame" << std::endl <<
                             "- The calibration board does not have any glare" << std::endl <<
                             "- The calibration board is well illuminated" << std::endl <<
                             "- The calibration board is in focus" << std::endl;
        }
    }

    // 释放图像内存
    camera_printed_board.Clear();

    // 关闭窗口
    camera_view.Close();

    // 校准相机
    double error;
    int camera_calib_option = 0;

    if(camera_calib_exists)
    {
        std::cout << std::endl << "Calibration Options:" << std::endl <<
                                  "0 - Update camera extrinsics (RECOMMENDED) + Uses a previously completed camera only calibration" << std::endl <<
                                  "1 - Update camera extrinsics, intrinsics, and distortion + Uses the camera calibration done with the system calibration" << std::endl;
//        cout << "Select option: ";
        if(!dlp::CmdLine::Get(camera_calib_option,"Select option: ")) camera_calib_option = 0;
//        if(!cin >> camera_calib_option)camera_calib_option = 0;cout << endl;
        if(camera_calib_option > 1) camera_calib_option = 0;
    }
    else{
        camera_calib_option = 1;
    }

    std::cout << "Calibrating camera... \n" << std::endl;

    bool update_intrinsic   = false;
    bool update_distortion  = false;
    bool update_extrinsic   = false;
    switch (camera_calib_option) {
    case 0:
        update_intrinsic   = false;
        update_distortion  = false;
        update_extrinsic   = true;
        break;
    case 1:
        update_intrinsic   = true;
        update_distortion  = true;
        update_extrinsic   = true;
        break;
    }

    // 执行校准计算
    camera_calib.Calibrate(&error,update_intrinsic,update_distortion,update_extrinsic);

    if( ret.hasErrors())
    {
        if(camera->Stop().hasErrors())
            std::cout << "Camera failed to stop! Exiting calibration routine..." << std::endl;
        std::cout << "Camera calibration FAILED!" << std::endl;
        return;
    }

    std::cout << "Camera calibration succeeded with reprojection error (closer to zero is better) = " << error << std::endl;

    // 获取摄像机校准
    camera_calib.GetCalibrationData(&camera_calib_data);

    // 将摄像机校准添加到投影仪校准中
    std::cout << "Loading camera calibration into projector calibration..." << std::endl;
    projector_calib.SetCameraCalibration(camera_calib_data);

    std::cout << "Calibrating projector... \n" << std::endl;
    ret = projector_calib.Calibrate(&error);

    if( ret.hasErrors())
    {
        if(camera->Stop().hasErrors())
            std::cout << "Camera failed to stop! Exiting calibration routine..." << std::endl;
        std::cout << "Projector calibration FAILED \n" << std::endl;
        return;
    }

    std::cout << "Projector calibration succeeded with reprojection error (closer to zero is better) = " << error << std::endl;

    // 获得投影仪和摄像机校准
    projector_calib.GetCalibrationData(&projector_calib_data);

    // 保存校准数据
    std::cout << "Saving camera and projector calibration data..." << std::endl;
    camera_calib_data.Save(camera_calib_data_file);
    projector_calib_data.Save(projector_calib_data_file);

    if(camera->Stop().hasErrors())
        std::cout << "Camera failed to stop! Exiting calibration routine..."<< std::endl;
    std::cout << "Calibration data saved" << std::endl;

    return;
}


//6:执行扫描(仅垂直模式)
//7:执行扫描(仅水平模式)
//8:执行扫描(垂直和水平模式)
void dlpclass::ScanObject(dlp::Camera          *camera,
                          const bool           &cam_proj_hw_synchronized,
                          const std::string    &camera_calib_data_file,
                          dlp::DLP_Platform    *projector,
                          const std::string    &projector_calib_data_file,
                          dlp::StructuredLight *structured_light_vertical,
                          dlp::StructuredLight *structured_light_horizontal,
                          const bool           &use_vertical,
                          const bool           &use_horizontal,
                          const std::string    &geometry_settings_file,
                          const bool           &continuous_scanning)
{

    // 检查相机是否为空
    if(!camera) return;

    // 检查摄像头是否连接好
    if(!camera->isConnected()){
        dlp::CmdLine::Print("Camera NOT connected! \n");
        return;
    }

    // 检查投影仪是否无效
    if(!projector) return;

    // 检查投影仪是否连接好
    if(!projector->isConnected()){
        dlp::CmdLine::Print("Projector NOT connected! \n");
        return;
    }

    // 检查结构化光模块指针是否有效
    if(!structured_light_vertical)   return;
    if(!structured_light_horizontal) return;

    if(use_vertical){
        if(!structured_light_vertical->isSetup()){
            dlp::CmdLine::Print("Vertical structured light module NOT setup! \n");
            return;
        }
    }

    if(use_horizontal){
        if(!structured_light_horizontal->isSetup()){
            dlp::CmdLine::Print("Horizontal structured light module NOT setup! \n");
            return;
        }
    }

    // 检查校准是否完成
    dlp::Calibration::Data calibration_data_camera;
    dlp::Calibration::Data calibration_data_projector;


    std::cout << "Loading camera and projector calibration data..." << std::endl;
    calibration_data_camera.Load(camera_calib_data_file);
    calibration_data_projector.Load(projector_calib_data_file);

    if(!calibration_data_camera.isComplete())
    {
        std::cout << "Camera calibration is NOT complete! \n" << std::endl;
        return;
    }

    if(!calibration_data_camera.isCamera())
    {
        std::cout << "Camera calibration is NOT from a camera calibration! \n" << std::endl;
        return;
    }

    if(!calibration_data_projector.isComplete())
    {
        std::cout << "Projector calibration is NOT complete! \n" << std::endl;
        return;
    }

    if(calibration_data_projector.isCamera())
    {
        std::cout << "Projector calibration is NOT from a projector calibration! \n" << std::endl;
        return;
    }

    // 检查相机分辨率是否与相机校准数据分辨率匹配
    unsigned int calibration_camera_rows;
    unsigned int calibration_camera_columns;
    unsigned int camera_rows;
    unsigned int camera_columns;
    calibration_data_camera.GetModelResolution(&calibration_camera_columns,&calibration_camera_rows);
    camera->GetColumns(&camera_columns);
    camera->GetRows(&camera_rows);

    if((calibration_camera_columns != camera_columns) ||
       (calibration_camera_rows    != camera_rows)){
        std::cout << "\nThe camera calibration data model resolution and camera resolution do NOT match!" << std::endl <<
                "Please use the calibration files for this specific camera!" << std::endl <<
                "Camera resolution from Calibration File  - (" << calibration_camera_columns << "," << calibration_camera_rows << ") " << "Actual Camera resolution - (" << camera_columns << "," << camera_rows << ") " << std::endl;
        return;
    }


    // 检查投影仪分辨率是否与投影仪校准数据分辨率相匹配
    unsigned int calibration_projector_rows;
    unsigned int calibration_projector_columns;
    unsigned int projector_rows;
    unsigned int projector_columns;
    calibration_data_projector.GetModelResolution(&calibration_projector_columns,&calibration_projector_rows);
    projector->GetColumns(&projector_columns);
    projector->GetRows(&projector_rows);

    if((calibration_projector_columns != projector_columns) ||
       (calibration_projector_rows    != projector_rows)){
        std::cout << "\nThe projector calibration data model resolution and projector resolution do NOT match!" << std::endl <<
         "Please use the calibration files for this specific projector!" << std::endl <<
         "Projector resolution from Calibration File  - (" << calibration_projector_columns << "," << calibration_projector_rows << ") " << "Actual Camera resolution - (" << projector_columns << "," << projector_rows << ") " << std::endl;
        return;
    }

    dlp::Parameters geometry_settings;
    dlp::Geometry scanner_geometry;
    geometry_settings.Load(geometry_settings_file);
    scanner_geometry.Setup(geometry_settings);

    // 设置几何
    std::cout << "Constructing the camera and projector geometry..." << std::endl;
    unsigned int camera_viewport;
    scanner_geometry.SetDebugEnable(false);
    scanner_geometry.SetOriginView(calibration_data_projector);
    scanner_geometry.AddView(calibration_data_camera,&camera_viewport);

    // 扫描期间查看器的变量
    dlp::Point::Cloud::Window view_point_cloud;

    // 扫描循环中使用的变量
    int scan_count = 0;
    std::atomic_bool continue_scanning(true);
    dlp::Time::Chronograph  timer;
    dlp::Capture::Sequence  capture_scan;
    dlp::Point::Cloud       point_cloud;
    dlp::Image depth_map;
    dlp::Image color_map;


    // 获取相机帧速率(假定相机触发了投影仪!)
    float frame_rate;
    camera->GetFrameRate(&frame_rate);

    // 确定总的扫描时间
    unsigned int period_us =uchar( 1000000 / frame_rate);
    unsigned int vertical_pattern_count   = structured_light_vertical->GetTotalPatternCount();
    unsigned int horizontal_pattern_count = structured_light_horizontal->GetTotalPatternCount();
    unsigned int capture_time = 0;
    unsigned int pattern_start = 1; // 有一个校准图像，所以从偏移量开始
    unsigned int pattern_count = 0;

    if(use_vertical)
        pattern_count += vertical_pattern_count;
    else
        pattern_start += vertical_pattern_count;


    if(use_horizontal)
        pattern_count += horizontal_pattern_count;


    capture_time += period_us * pattern_count;

    //打开摄像机视图，使目标对象可以放置在摄像机的视图内，投影仪开始捕捉图像进行摄像机校准
    dlp::Image          camera_frame;
    dlp::Image::Window  camera_view;

    // 打开相机视图
    std::cout << "\nPlace the scanning target within the view of the camera and projector. \nPress SPACE or ESC from window when ready to scan..." << std::endl;
    camera_view.Open("Place Target in View - press SPACE or ESC to scan");

    // 开始从相机捕捉图像
    if(camera->Start().hasErrors())
    {
        std::cout << "Could NOT start camera! \n" << std::endl;
        return;
    }

    // 项目白色，以便对象可以正确放置
    projector->ProjectSolidWhitePattern();

    // 等待空格键或ESC键被按下后再扫描
    unsigned int return_key = 0;
    while(return_key != ' ')
    {
        camera_frame.Clear();               // 清除图像对象
        camera->GetFrame(&camera_frame);    // 获得最新的帧
        camera_view.Update(camera_frame);   // 显示图像
        camera_view.WaitForKey(16,&return_key);     // 等待按键被按下或50ms超时
        if(return_key == 27) break;                 // ESC键被按下
    }

    // 关闭图像窗口
    camera_view.Close();


    // 显示使用点云查看器的说明
    std::cout << "Point Cloud Viewer Operation:"<< std::endl <<
            "i/I = Zoom in"<< std::endl <<
            "o/O = Zoom out"<< std::endl <<
            "s/S = Save point cloud xyz file"<< std::endl <<
            "a/A = Auto-rotate the point cloud"<< std::endl <<
            "c/C = Turn point cloud color on/off" << std::endl <<
            "\nNOTE: Press ESC key to quit the scan routine" << std::endl;


    // 打开点云查看器
    view_point_cloud.Open("Point Cloud Viewer (color based on z value) - press ESC to stop scanning...",600,400);

    // 进入扫描循环
    while(continue_scanning)
    {

        std::cout << "\nStarting scan " << scan_count <<  "..." << std::endl;

        dlp::Capture::Sequence vertical_scan;
        dlp::Capture::Sequence horizontal_scan;

        //当摄像机和投影仪通过HW触发信号连接时，Peform图像捕获进行同步
        if(cam_proj_hw_synchronized == true)
        {

            // 开始从相机捕捉图像
            if(camera->Start().hasErrors())
            {
                std::cout << "Could NOT start camera! \n" << std::endl;
                return;
            }

            // 给相机时间开始拍摄图像
            dlp::Time::Sleep::Milliseconds(100);

            // 扫描的对象
            dlp::ReturnCode sequence_return;
            sequence_return = projector->StartPatternSequence(pattern_start,pattern_count,false);
            if(sequence_return.hasErrors())
            {
                std::cout << "Sequence failed! Exiting scan routine..." << std::endl;
                if(camera->Stop().hasErrors())
                    std::cout << "Camera failed to stop! Exiting scan routine..." << std::endl;
                std::cout << "Sequence failed..." + sequence_return.ToString() << std::endl;
                return;
            }

            timer.Reset();

            // 等待序列完成并增加一点额外的时间来考虑所需的序列验证时间
            dlp::Time::Sleep::Microseconds(capture_time + 10*period_us);

            // 停止从相机抓取图像
            if(camera->Stop().hasErrors())
            {
                std::cout << "Camera failed to stop! Exiting scan routine..." << std::endl;
                return;
            }
            std::cout << "Pattern sequence capture completed in...\t" << timer.Lap() << "ms" << std::endl;
            projector->StopPatternSequence();

            // 从缓冲区获取所有图像，以找到模式序列
            bool            min_images = false;

            dlp::Capture    capture;
            dlp::Image      capture_image;

            unsigned int iPattern = 0;

            std::vector<double> capture_sums;

            while(!min_images)
            {
                capture_image.Clear();

                ret = camera->GetFrameBuffered(&capture_image);
                iPattern++;
                capture_image.ConvertToMonochrome();
                if(ret.hasErrors())
                    min_images = true;
                else
                {

                    double sum = 0;
                    capture_image.GetSum(&sum);
                    capture_sums.push_back(sum);

                    // 将框架添加到序列中
                    capture.image_data = capture_image;
                    capture.data_type  = dlp::Capture::DataType::IMAGE_DATA;
                    capture_scan.Add(capture);
                    capture_image.Clear();
                    capture.image_data.Clear();

                }

            }

            std::cout << "Images retreived from buffer in...\t\t" << timer.Lap() <<  "ms" << std::endl;

            // 找出第一种模式，并将垂直模式和水平模式分开
            vertical_scan.Clear();
            horizontal_scan.Clear();

            bool first_pattern_found = false;
            unsigned int capture_offset = 0;
            unsigned int vertical_patterns_added = 0;
            unsigned int horizontal_patterns_added = 0;
            double previous_sum = 0;

            timer.Lap();

            for(unsigned int iScan = 0; iScan < capture_scan.GetCount(); iScan++)
            {

                // 计算图像的平均值以确定模式是否开始
                if(!first_pattern_found )
                {

                    // 检索图像和值
                    double sum = capture_sums.at(iScan);

                    if(iScan == 0)
                        previous_sum = sum;
                    else if(previous_sum != 0.0)
                    {
                        // 如果百分比误差大于10%，则找到第一种模式
                        if( sum > (previous_sum * 1.1) )
                        {
                            first_pattern_found = true;
                            capture_offset = iScan;
                        }
                        previous_sum = sum;
                    }
                }

                if(first_pattern_found)
                {
                    if((capture_offset+pattern_count) < capture_scan.GetCount())
                    {

                        dlp::Capture temp;
                        capture_scan.Get(iScan,&temp);


                        if(use_vertical && (vertical_patterns_added < vertical_pattern_count))
                        {
                            // 添加垂直模式
                            vertical_scan.Add(temp);
                            temp.image_data.Save("output/scan_images/scan_capture_"+dlp::Number::ToString(iScan - capture_offset)+".bmp");
                            vertical_patterns_added++;
                        }
                        else if(use_horizontal && (horizontal_patterns_added < horizontal_pattern_count))
                        {
                            // 添加水平模式
                            horizontal_scan.Add(temp);
                            temp.image_data.Save("output/scan_images/scan_capture_"+dlp::Number::ToString(iScan - capture_offset)+".bmp");
                            horizontal_patterns_added++;
                        }

                        temp.image_data.Clear();
                    }
                }
            }
        }
        else
        {
            //执行图像捕捉相机是在自由运行模式，即如果他们不同步通过HW触发信号项目黑屏
            projector->ProjectSolidBlackPattern();
            dlp::Time::Sleep::Milliseconds(100);

            // 开始从相机捕捉图像
            if(camera->Start().hasErrors())
            {
                std::cout << "Could NOT start camera! \n" << std::endl;
                return;
            }

            timer.Reset();

            // 从缓冲区获取所有图像，以找到模式序列
            bool            min_images = false;

            dlp::Capture    capture;
            dlp::Image      capture_image;

            unsigned int iPattern = 0;

            while(!min_images){

                capture_image.Clear();

                // 显示每个模式
                projector->DisplayPatternInSequence(pattern_start+iPattern,true);
                iPattern++;

                dlp::Time::Sleep::Milliseconds(100);

                // 获取最新的相机图像
                ret = camera->GetFrame(&capture_image);
                capture_image.ConvertToMonochrome();

                if(ret.hasErrors())
                {
                    // 检查缓冲区是否为空
                    if(ret.ContainsError(OPENCV_CAM_IMAGE_BUFFER_EMPTY))
                        min_images = true;
                }
                else
                {
                    // 将框架添加到序列中
                    capture.image_data = capture_image;
                    capture.data_type  = dlp::Capture::DataType::IMAGE_DATA;
                    capture_scan.Add(capture);
                    capture_image.Clear();
                    capture.image_data.Clear();
                }

                if(iPattern == pattern_count) min_images = true;
            }

            std::cout << "Pattern sequence capture completed in...\t" << timer.Lap() << "ms" << std::endl;

            // 重新启动相机，以便在处理期间显示白色图案
            projector->ProjectSolidWhitePattern();
            camera->Start();

            // 找出第一种模式，并将垂直模式和水平模式分开
            vertical_scan.Clear();
            horizontal_scan.Clear();

            unsigned int capture_offset = 0;
            unsigned int vertical_patterns_added = 0;
            unsigned int horizontal_patterns_added = 0;

            timer.Lap();

            for(unsigned int iScan = 0; iScan < capture_scan.GetCount(); iScan++)
            {
                dlp::Capture temp;
                capture_scan.Get(iScan,&temp);

                if(use_vertical && (vertical_patterns_added < vertical_pattern_count))
                {
                    // 添加垂直模式
                    vertical_scan.Add(temp);
                    temp.image_data.Save("output/scan_images/scan_capture_"+dlp::Number::ToString(iScan - capture_offset)+".bmp");
                    vertical_patterns_added++;
                }
                else if(use_horizontal && (horizontal_patterns_added < horizontal_pattern_count))
                {
                    // 添加水平模式
                    horizontal_scan.Add(temp);
                    temp.image_data.Save("output/scan_images/scan_capture_"+dlp::Number::ToString(iScan - capture_offset)+".bmp");
                    horizontal_patterns_added++;
                }

                temp.image_data.Clear();
            }

        }

        capture_scan.Clear();
        std::cout << "Patterns sorted in...\t\t\t\t" << timer.Lap() << "ms" << std::endl;

        dlp::DisparityMap column_disparity;
        dlp::DisparityMap row_disparity;
        dlp::Point::Cloud point_cloud_new;

        column_disparity.Clear();
        row_disparity.Clear();

        if(use_vertical && (vertical_pattern_count == vertical_scan.GetCount()))
        {
            timer.Lap();
            structured_light_vertical->DecodeCaptureSequence(&vertical_scan,&column_disparity);
            std::cout << "Vertical patterns decoded in...\t\t\t" << timer.Lap() << "ms" << std::endl;
        }


        if(use_horizontal && (horizontal_pattern_count == horizontal_scan.GetCount()))
        {
            timer.Lap();
            structured_light_horizontal->DecodeCaptureSequence(&horizontal_scan,&row_disparity);
            std::cout << "Horizontal patterns decoded in...\t\t" << timer.Lap() << "ms" << std::endl;
        }

        if(use_vertical && (!use_horizontal))
        {
            // 只使用垂直模式

            // 检查是否有足够的模式需要解码
            if(vertical_pattern_count != vertical_scan.GetCount())
                std::cout << "NOT enough images. Scans may have been too dark. Please rescan. \n" << std::endl;
            else
            {
                scan_count++;
                point_cloud_new.Clear();
                scanner_geometry.GeneratePointCloud(camera_viewport,column_disparity,&point_cloud,&depth_map);
                std::cout << "Point cloud reconstructed in...\t\t\t" << timer.Lap() << "ms" << std::endl;
            }
        }
        else if(use_horizontal && (!use_vertical))
        {
            // 只使用水平模式

            //检查是否有足够的模式需要解码
            if(horizontal_pattern_count != horizontal_scan.GetCount())
                std::cout << "NOT enough images. Scans may have been too dark. Please rescan. \n" << std::endl;
            else
            {
                scan_count++;
                point_cloud_new.Clear();
                scanner_geometry.GeneratePointCloud(camera_viewport,row_disparity,&point_cloud,&depth_map);
                std::cout << "Point cloud reconstructed in...\t\t\t" << timer.Lap() << "ms" << std::endl;
            }
        }
        else if(use_vertical && use_horizontal)
        {
            // 使用垂直和水平

            // 检查是否有足够的模式需要解码
            if((structured_light_vertical->GetTotalPatternCount()   != vertical_scan.GetCount()) ||
               (structured_light_horizontal->GetTotalPatternCount() != horizontal_scan.GetCount()))
                std::cout << "NOT enough images. Scans may have been too dark. Please rescan. \n" << std::endl;
            else
            {
                scan_count++;
                point_cloud_new.Clear();
                scanner_geometry.GeneratePointCloud(camera_viewport,column_disparity,row_disparity,&point_cloud,&depth_map);
                std::cout << "Point cloud reconstructed in...\t\t\t" << timer.Lap() << "ms" << std::endl;
            }
        }

        // 更新阅读器
        view_point_cloud.Update(point_cloud);

        // 检查点云查看器是否打开，或者扫描应该只执行一次
        continue_scanning = view_point_cloud.isOpen() & continuous_scanning;

        // 清除变量
        vertical_scan.Clear();
        horizontal_scan.Clear();
        column_disparity.Clear();
        row_disparity.Clear();
    }

    // 等待视图关闭
    std::cout << "\nWaiting for point cloud viewer to close..." << std::endl;
    while(view_point_cloud.isOpen());

    // 检查最近的扫描是否应该保存
//    std::cout << std::endl << "Save most recent scan?" << std::endl <<
//            "0 - Do NOT save files" << std::endl <<
//            "1 - Save point cloud data and color depth map" <<std:: endl;


    unsigned int save_data = 1;
//    if(!dlp::CmdLine::Get(save_data,"Select option: ")) save_data = 0;
//    cout << "Select option: ";
//    if(!cin >> save_data)save_data = 0;cout << endl;

    if(save_data == 1){
        std::string file_time = dlp::Number::ToString(timer.Reset());

        std::cout << std::endl << "Saving depth color map..." << std::endl;
        dlp::Geometry::ConvertDistanceMapToColor(depth_map,&color_map);
        color_map.Save("D:/photo/src/output/scan_data/" + file_time + "_color_map.bmp");

        std::cout << "Saving point cloud..." << std::endl;
        point_cloud.SaveXYZ("D:/photo/src/output/scan_data/" + file_time + "_point_cloud.xyz", ' ');
    }

    if(camera->Stop().hasErrors())
        std::cout << "Camera failed to stop! Exiting scan routine..." << std::endl;


    // 关闭阅读器
    view_point_cloud.Close();

    // 清除几何模块以释放内存
    scanner_geometry.Clear();

    return;
}



//11:Video
void dlpclass::Video(dlp::Camera *camera)
{
    // 检查相机是否为空
    if(!camera) return;

    // 检查摄像头是否连接好
    if(!camera->isConnected())
    {
        std::cout << "Camera NOT connected!" << std::endl;
        return;
    }

    // 如果还没有启动，请启动相机
    if(!camera->isStarted()){
        if(camera->Start().hasErrors()){
            dlp::CmdLine::Print("Could NOT start camera! \n");
            return;
        }
        else{
            dlp::CmdLine::Print("Started camera. \n");
        }
    }

    // 给相机一些时间来填充图像缓冲区
    dlp::Time::Sleep::Milliseconds(50);

    //开始捕捉图像进行相机校准
    dlp::Image          camera_printed_board;
    dlp::Image::Window  camera_view;

    // 打开相机视图
    camera_view.Open("Camera Calibration - press SPACE to capture or ESC to exit");

    unsigned int return_key = 0;
    while(return_key != 27){
        if(VideoBool==1)
        {
            camera_printed_board.Clear();               // 清除图像对象
            camera->GetFrame(&camera_printed_board);    // 捕获最新的相框
            camera_view.Update(camera_printed_board);   // 显示图像
        }
        camera_view.WaitForKey(16,&return_key);     // 等待按键被按下或50ms超时
        if(return_key == ' ' ) VideoBool =1-VideoBool;
        if(return_key == 27 || VideoBool == 2) break;                 // ESC键被按下
    }


    // 关闭图像窗口
    camera_view.Close();

    // 停止相机捕捉
    if(camera->Stop().hasErrors()){
        std::cout << "Camera failed to stop! Exiting calibration routine..." << std::endl;
    }

    // 释放图像内存
    camera_printed_board.Clear();

    return;
}

void dlpclass::file_name_add()
{
    std::string name = "D:\\photo\\scan_image\\";
    if(file_name_num<10)name+="0";
    name+=char(file_name_num);
    name+=".jpg";
    file_name = name;
    file_name_num++;
}
//从本地存储导入图片对象
void dlpclass::image_file_io()
{
    // 将图像文件加载到图像对象中
    std::cout << "Loading image file..." << image_file << std::endl;
    file_name_add();
    ret = example_image.Load(image_file);
    if(ret.hasErrors())
        {
        std::cout << "Image load FAILED: "<< ret.ToString() << std::endl;
        std::cout << "Could not find file: " << image_file << std::endl;
        }
    else
    {
        std::cout << "Saving the loaded image as image_copy.bmp" << std::endl;
        ret = example_image.Save("D:\\photo\\image_copy.bmp");
        if(ret.hasErrors())
            std::cout << "Image save FAILED: "<< ret.ToString() << std::endl;
    }
}
void dlpclass::image_pixels()
{
    dlp::Image rgb_image;
    dlp::PixelRGB rgb_pixel;
    dlp::Image mono_image;
    unsigned char mono_pixel;
    unsigned int image_columns;
    unsigned int image_rows;
    unsigned int num[3] ; num[0] = 0; num[1] = 200; num[2] = 399;

    // 创建包含400行和300列的图像对象
    ret = rgb_image.Create( 400,300,dlp::Image::Format::RGB_UCHAR);
    if(ret.hasErrors())
        std::cout << "RGB image creation FAILED: "<< ret.ToString() << std::endl;
    ret = mono_image.Create(400,300,dlp::Image::Format::MONO_UCHAR);
    if(ret.hasErrors())
        std::cout << "Monochrome image creation FAILED: "<< ret.ToString() << std::endl;

    // 检索图像维度
    ret = rgb_image.GetColumns(&image_columns);
    ret = rgb_image.GetRows(&image_rows);
    std::cout << "Image dimensions are " << image_columns << " x " << image_rows << std::endl;
    //在图像中绘制一个颜色渐变(从红色到绿色)
    std::cout << "Drawing the images..." << std::endl;

    for(unsigned int iCols = 0; iCols < image_columns; iCols++)
    {
        //创建RGB像素值
        rgb_pixel.r=0;
        rgb_pixel.g=uchar(255*iCols/image_columns);
        rgb_pixel.b=uchar(255-(255*iCols/image_columns));
        // 创建单像素值
        mono_pixel =uchar( (255 * iCols) / image_columns); //255是最大的无符号字符值
        //设置两个图像中的像素值
        for(unsigned int iRows=0; iRows < image_rows; iRows++)
        {
            ret = rgb_image.SetPixel( iCols, iRows, rgb_pixel);
            ret = mono_image.SetPixel( iCols, iRows, mono_pixel);
        }
    }

    std::cout << "Saving the images" << std::endl;
    ret = rgb_image.Save("D:\\photo\\rgb_gradient.bmp");
    if(ret.hasErrors())
        std::cout << "RGB image save FAILED: "<< ret.ToString() << std::endl;
    ret = mono_image.Save("D:\\photo\\mono_gradient.bmp");
    if(ret.hasErrors())
        std::cout << "Monochrome image save FAILED: "<< ret.ToString() << std::endl;

    // 从这两个图像的第一行检索第一个、中间和最后一个像素
    // 注意:像素地址从0开始。最后一个地址是维数- 1

    for(int i=0;i<3;i++)
    {
    ret = rgb_image.GetPixel(num[i],0,&rgb_pixel);
    if(ret.hasErrors())
        std::cout << "Get pixel (" << num[i] << ",0) FAILED: "<< ret.ToString() << std::endl;
    else
        std::cout << "rgb_image (" << num[i] << ",0) pixel = R" << int(rgb_pixel.r) << " G" << int(rgb_pixel.g) << " B" << int(rgb_pixel.b) << std::endl;

    ret = mono_image.GetPixel(num[i],0,&mono_pixel);
    if(ret.hasErrors())
        std::cout << "Get pixel (" << num[i] << ",0) FAILED: "<< ret.ToString() << std::endl;
    else
        std::cout << "mono_image (" << num[i] << ",0) pixel = " << int(mono_pixel) << std::endl;
    }

    //GetPixel具有针对无效地址的保护
    ret = mono_image.GetPixel(1000,1000,&mono_pixel);
    if(ret.hasErrors())
        std::cout << "Get pixel (1000,1000) FAILED: "<< ret.ToString() << std::endl;
    else
        std::cout << "mono_image (1000,1000) pixel = " << int(mono_pixel) << std::endl;


}
void dlpclass::image_window()
{
    dlp::Image::Window image_window;  // 创建图像对象

    // 将图像文件加载到图像对象中
    std::cout << "Loading image..." << image_file << std::endl;
    ret = example_image.Load(image_file);
    if(ret.hasErrors())
    {
        std::cout << "Image load FAILED: "<< ret.ToString() << std::endl;
        std::cout << "Could not find file: " << image_file << std::endl;
    }
    std::cout << "Opening an OpenCV window" << std::endl;
    ret = image_window.Open("Image Window Example");
    if(ret.hasErrors())
        std::cout << "Could NOT open image window: "<< ret.ToString() << std::endl;
    std::cout << "Updating the window to display the example image" << std::endl;
    ret = image_window.Update(example_image);
    if(ret.hasErrors())
        std::cout << "Could NOT update image window: "<< ret.ToString() << std::endl;
    //等待按键被按下
    std::cout << "Waiting for any key to be pressed while the window is selected to close it..." << std::endl;
    unsigned int return_key = 0;
    ret = image_window.WaitForKey(0, &return_key); // 0表示等待，直到按下一个键
    std::cout << "ASCII Code of key pressed = " << return_key << " = " << char(return_key) << std::endl;
    // 关闭窗口
    std::cout << "Closing the window.." << std::endl;
    image_window.Close();

}
void dlpclass::point_cloud_viewer()
{

    std::cout << "Enter point cloud XYZ file name: " << filename << std::endl;

    // 加载点云文件
    dlp::Point::Cloud cloud;
    std::cout << "Loading the point cloud file..." << std::endl;

    ret = cloud.LoadXYZ(filename);
    if(ret.hasErrors())
    {
        std::cout << "Could not open point cloud file!" << std::endl << ret.ToString() << std::endl;
        return;}

    // 显示使用点云查看器的说明
    std::cout << std::endl << "Point Cloud Viewer Operation:" << std::endl;
    std::cout << "i/I = Zoom in" << std::endl;
    std::cout << "o/O = Zoom out" << std::endl;
    std::cout << "s/S = Save point cloud xyz file" << std::endl;
    std::cout << "a/A = Auto-rotate the point cloud" << std::endl;
    std::cout << "c/C = Turn point cloud color on/off" << std::endl;
    std::cout << "Press ESC key to close the viewer" << std::endl << std::endl;

    // 打开点云查看器窗口
    dlp::Point::Cloud::Window viewer;
    ret = viewer.Open("Point Cloud Viewer - " + filename);
    if(ret.hasErrors())
    {
        std::cout << "Could not open point cloud viewer!" << std::endl << ret.ToString() << std::endl;
        return;}

    // 在查看器中显示点云
    std::cout << "Updating the point cloud viewer content..." << std::endl;
    ret = viewer.Update(cloud);
    if(ret.hasErrors())
    {
        std::cout << "Could not display point cloud!" << std::endl << ret.ToString() << std::endl;
        return;}

    //等待查看器关闭
    std::cout << "Waiting for point cloud viewer to close..." << std::endl;
    while(viewer.isOpen());

    // 关闭和释放内存
    viewer.Close();
    cloud.Clear();
}
void dlpclass::returncodes_example()
{
    // ReturnCode对象在初始化时不包含错误或警告
    dlp::ReturnCode retval_a;
    dlp::ReturnCode retval_b;
    dlp::ReturnCode retval_c;
    // ReturnCode中的所有错误和警告都可以使用dlp::ReturnCode::ToString()方法以字符串的形式输出
    std::cout << "retval_a contains " << retval_a.ToString() << std::endl;
    std::cout << "retval_b contains " << retval_b.ToString() << std::endl;
    // 错误和警告可以作为字符串添加
    std::string error_msg = "This is an error message!";
    std::string warning_msg = "This is just a warning...";
    retval_a.AddError(error_msg);
    retval_a.AddWarning(warning_msg);
    // 定义宏帮助错误和警告消息保持一致
    retval_a.AddError(EXAMPLE_ERROR_2);
    retval_a.AddError(EXAMPLE_ERROR_3);
    retval_a.AddWarning(EXAMPLE_WARNING_2);
    // 整个返回码也可以添加
    retval_b.Add(retval_a);
    // 这两个ReturnCode对象具有相同的内容
    std::cout << std::endl;
    std::cout << "retval_a contains " << retval_a.ToString() << std::endl;
    std::cout << "retval_b contains " << retval_b.ToString() << std::endl;
    std::cout << "retval_c contains " << retval_c.ToString() << std::endl;
    // 有三种方法可以确定ReturnCode是否有错误
    std::cout << std::endl;
    if(retval_a)
        std::cout << "1retval_a has errors!" << std::endl;
    if(retval_a.hasErrors())
        std::cout << "2retval_a has errors!" << std::endl;
    if(retval_a.GetErrorCount() > 0)
        std::cout << "3retval_a has "<< retval_a.GetErrorCount() << " errors!" << std::endl;
    // 警告不算作错误
    std::cout << std::endl;
    retval_c.AddWarning(EXAMPLE_WARNING_1);
    if(retval_c.hasErrors())
        std::cout << "retval_c has errors!" << std::endl;
    else
        std::cout << "retval_c has NO errors!" << std::endl;

    // 返回码可以检查特定的错误和警告
    std::cout << std::endl;
    if(retval_a.ContainsError(EXAMPLE_ERROR_1)) std::cout << "retval_a contains " << EXAMPLE_ERROR_1 << std::endl;
    if(retval_a.ContainsError(EXAMPLE_ERROR_2)) std::cout << "retval_a contains " << EXAMPLE_ERROR_2 << std::endl;
    if(retval_a.ContainsError(EXAMPLE_ERROR_3)) std::cout << "retval_a contains " << EXAMPLE_ERROR_3 << std::endl;
    if(retval_a.ContainsWarning(EXAMPLE_WARNING_1)) std::cout << "retval_a contains " << EXAMPLE_WARNING_1 << std::endl;
    if(retval_a.ContainsWarning(EXAMPLE_WARNING_2)) std::cout << "retval_a contains " << EXAMPLE_WARNING_2 << std::endl;
    // 返回码可以被清除
    retval_c.Clear();
    std::cout << std::endl;
    std::cout << "After clearing retval_c contains " << retval_c.ToString() << std::endl;
    // 错误和警告消息可以作为字符串向量返回
    std::vector<std::string> error_msg_list;
    std::vector<std::string> warning_msg_list;
    error_msg_list = retval_a.GetErrors();
    warning_msg_list = retval_a.GetWarnings();
}

//目录备份
/*    if(camera_type.Get() == 0)
    {
        switch(menu_select){
        case 0:
            break;
        case 1:
            GenerateCameraCalibrationBoard(&camera_cv,
                                           config_file_calib_camera.Get(),
                                           dir_camera_calib_image_output.Get() +
                                           output_name_image_camera_calib_board.Get() +
                                           ".bmp");
            break;
        case 2:
            PrepareProjectorPatterns(&projector,
                                     config_file_calib_projector.Get(),
                                     &algo_gray_code_vert,
                                     config_file_structured_light_1.Get(),
                                     &algo_gray_code_horz,
                                     config_file_structured_light_2.Get(),
                                     false,    // Firmware will be uploaded
                                     &total_pattern_count);
            break;
        case 3:
            PrepareProjectorPatterns(&projector,
                                     config_file_calib_projector.Get(),
                                     &algo_gray_code_vert,
                                     config_file_structured_light_1.Get(),
                                     &algo_gray_code_horz,
                                     config_file_structured_light_2.Get(),
                                     true, // Firmware will NOT be uploaded
                                     &total_pattern_count);
            break;
        case 4:
            CalibrateCamera(&camera_cv,
                            config_file_calib_camera.Get(),
                            calib_data_file_camera.Get(),
                            dir_camera_calib_image_output.Get() +
                            output_name_image_camera_calib.Get(),
                            &projector);
            break;
        case 5:
            CalibrateSystem(&camera_cv,
                            config_file_calib_camera.Get(),
                            calib_data_file_camera.Get(),
                            &projector,
                            config_file_calib_projector.Get(),
                            calib_data_file_projector.Get(),
                            dir_system_calib_image_output.Get() +
                            output_name_image_system_calib.Get());
            break;
        case 6:
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_cv,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           true,
                           false,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_cv,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           true,
                           false,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
//            else ;
            break;
        case 7:
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_cv,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           false,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_cv,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           false,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
//            else ;
            break;
        case 8:
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_cv,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           true,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_cv,
                           false,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           true,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
//            else ;
            break;
        case 9:
            camera_cv.Disconnect();
            projector.Disconnect();
            dlp::DLP_Platform::ConnectSetup(projector,connect_id_projector.Get(),config_file_projector.Get(),true);
            dlp::Camera::ConnectSetup(camera_cv,connect_id_camera.Get(),config_file_camera.Get(),true);
            break;
        default:
            std::cout << "Invalid menu selection! \n" << std::endl;
        }*/
/*    }
    else if(camera_type.Get() == 1)
    {
        switch(menu_select){
        case 0:
            break;
        case 1:
            GenerateCameraCalibrationBoard(&camera_pg,
                                           config_file_calib_camera.Get(),
                                           dir_camera_calib_image_output.Get() +
                                           output_name_image_camera_calib_board.Get() +
                                           ".bmp");
            break;
        case 2:
            PrepareProjectorPatterns(&projector,
                                     config_file_calib_projector.Get(),
                                     &algo_three_phase_vert,
                                     config_file_structured_light_1.Get(),
                                     &algo_three_phase_horz,
                                     config_file_structured_light_2.Get(),
                                     false,    // Firmware will be uploaded
                                     &total_pattern_count);
            break;
        case 3:
            PrepareProjectorPatterns(&projector,
                                     config_file_calib_projector.Get(),
                                     &algo_three_phase_vert,
                                     config_file_structured_light_1.Get(),
                                     &algo_three_phase_horz,
                                     config_file_structured_light_2.Get(),
                                     true, // Firmware will NOT be uploaded
                                     &total_pattern_count);
            break;
        case 4:
            CalibrateCamera(&camera_pg,
                            config_file_calib_camera.Get(),
                            calib_data_file_camera.Get(),
                            dir_camera_calib_image_output.Get() +
                            output_name_image_camera_calib.Get(),
                            &projector);
            break;
        case 5:
            CalibrateSystem(&camera_pg,
                            config_file_calib_camera.Get(),
                            calib_data_file_camera.Get(),
                            &projector,
                            config_file_calib_projector.Get(),
                            calib_data_file_projector.Get(),
                            dir_system_calib_image_output.Get() +
                            output_name_image_system_calib.Get());
            break;
        case 6:
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_pg,
                           true,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           true,
                           false,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_pg,
                           true,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           true,
                           false,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
//            else ;
            break;
        case 7:
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_pg,
                           true,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           false,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_pg,
                           true,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           false,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
//            else ;
            break;
        case 8:
            if(algorithm_type.Get() == 0)
                ScanObject(&camera_pg,
                           true,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_gray_code_vert,
                           &algo_gray_code_horz,
                           true,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
            else if(algorithm_type.Get() == 1)
                ScanObject(&camera_pg,
                           true,
                           calib_data_file_camera.Get(),
                           &projector,
                           calib_data_file_projector.Get(),
                           &algo_three_phase_vert,
                           &algo_three_phase_horz,
                           true,
                           true,
                           config_file_geometry.Get(),
                           continuous_scanning.Get());
//            else ;
            break;
        case 9:
            camera_pg.Disconnect();
            projector.Disconnect();
            // Reconnect system objects
            dlp::DLP_Platform::ConnectSetup(projector,connect_id_projector.Get(),config_file_projector.Get(),true);
            dlp::Camera::ConnectSetup(camera_pg,connect_id_camera.Get(),config_file_camera.Get(),true);
            break;
        default:
            std::cout << "Invalid menu selection! \n" << std::endl;
        }
    }*/
