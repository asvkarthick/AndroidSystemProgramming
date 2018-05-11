#include <iostream>
#include <vector>
#include <tuple>
#include <string.h>
#include <hardware/hardware.h>
#include <hardware/camera_common.h>
#include <hardware/camera3.h>

// ID of the HAL implementation
#define HAL_LIBRARY "camera"

void camera_device_status_change(
        const struct camera_module_callbacks* callbacks,
        int camera_id,
        int new_status)
{
    (void) camera_device_status_change;
    (void) callbacks;
    (void) camera_id;
    (void) new_status;

    std::cout << __FILE__ << " " << __func__ << " " << __LINE__ << std::endl;
}

void torch_mode_status_change(
        const struct camera_module_callbacks* callbacks,
        const char* camera_id,
        int new_status)
{
    (void) torch_mode_status_change;
    (void) callbacks;
    (void) camera_id;
    (void) new_status;

    std::cout << __FILE__ << " " << __func__ << " " << __LINE__ << std::endl;
}

void process_capture_result(const struct camera3_callback_ops *ops, const camera3_capture_result *result)
{
    (void) result;
    (void) ops;

    std::cout << __FILE__ << " " << __func__ << " " << __LINE__ << std::endl;
}

void notify(const camera3_callback_ops *cb, const camera3_notify_msg *msg)
{
    (void) cb;
    (void) msg;

    std::cout << __FILE__ << " " << __func__ << " " << __LINE__ << std::endl;
}

camera3_stream_t* create_camera_configuration(camera3_stream_t *config, int camera_number)
{
    config->stream_type = CAMERA3_STREAM_OUTPUT;
    config->format = HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED;
    config->data_space = HAL_DATASPACE_ARBITRARY;
    config->rotation = CAMERA3_STREAM_ROTATION_0;
    config->usage = 0;
    config->max_buffers = 0;
    config->priv = nullptr;
    if(camera_number == 0)
    {
        config->width = 1280;
        config->height = 400;
    }
    else
    {
        config->width = 1280;
        config->height = 800;
    }
    return config;
}

int main()
{
    const hw_module_t *hw_module = nullptr;
    const camera_module_t *camera_module = nullptr;
    vendor_tag_ops_t vTag;
    camera_module_callbacks_t camera_module_callbacks;
    camera_info_t *camera_info;
    int camera_number;
    int number_of_cameras;
    int ret;
    std::vector<camera_info_t*> camera_info_array;
    hw_device_t *hw_device;
    std::vector<std::pair<camera3_device_t*, int>> camera_device_array;
    camera3_device_t *camera_device;
    camera3_callback_ops_t camera_callbacks;
    camera3_stream_configuration camera_stream_config;

    // Load the Camera HAL library
    ret = hw_get_module(HAL_LIBRARY, &hw_module);
    std::cout << "HAL return value = " << ret << std::endl;

    if(ret != 0)
    {
        std::cout << "Error loading HAL module" << std::endl;
        return -1;
    }
    else
        std::cout << "Successfully loaded Camera HAL" << std::endl;

    camera_module = reinterpret_cast<const camera_module_t*>(hw_module);

    std::cout << "Module API Version = " << camera_module->common.module_api_version << std::endl;
    std::cout << "HAL API Version = " << camera_module->common.hal_api_version << std::endl;

    // Get vendor tag
    camera_module->get_vendor_tag_ops(&vTag);

    // Call init() of Camera HAL
    if(camera_module->init)
    {
        std::cout << "Camera module has init" << std::endl;
        camera_module->init();
    }
    else
        std::cout << "Camera module does not have init" << std::endl;

    number_of_cameras = camera_module->get_number_of_cameras();

    std::cout << "Number of cameras = " << number_of_cameras << std::endl;

    // Set callbacks from HAL for any state change notification
    camera_module_callbacks.camera_device_status_change = camera_device_status_change;
    camera_module_callbacks.torch_mode_status_change = torch_mode_status_change;
    camera_module->set_callbacks(&camera_module_callbacks);
    std::cout << "Camera callbacks set to HAL" << std::endl;

    std::cout << "Getting all camera info..." << std::endl;
    if(camera_module->get_camera_info)
    {
        for(camera_number = 0; camera_number < number_of_cameras; camera_number++)
        {
            camera_info = new camera_info_t;
            camera_info_array.push_back(camera_info);
            ret = camera_module->get_camera_info(camera_number, camera_info);
            if(ret != 0)
            {
                std::cout << "Camera info not available for camera " << camera_number << std::endl;
            }
            else
            {
                std::cout << "Camera " << camera_number << " facing = " << camera_info->facing << std::endl;
                std::cout << "Camera " << camera_number << " orientation = " << camera_info->orientation << std::endl;
            }
        }
    }
    else
        std::cout << "Camera info not available" << std::endl;

    camera_callbacks.process_capture_result = process_capture_result;
    camera_callbacks.notify = notify;

    std::cout << "Opening the cameras" << std::endl;
    if(camera_module->common.methods->open)
    {
        for(camera_number = 0; camera_number < number_of_cameras; camera_number++)
        {
            // Open the camera device
            camera_module->common.methods->open(hw_module, std::to_string(camera_number).c_str(), &hw_device);
            if(hw_device == nullptr)
            {
                std::cout << "Error opening camera " << camera_number << std::endl;
                continue;
            }

            camera_device = reinterpret_cast<camera3_device_t*>(hw_device);
            camera_device_array.push_back(std::make_pair(camera_device, camera_number));
            std::cout << "Successfully opened camera " << camera_number << std::endl;
            std::cout << "Camera " << camera_number << " version = " << hw_device->version << std::endl;

            // Initialize the camera device with the callbacks
            std::cout << "Initializing camera " << camera_number << std::endl;
            camera_device->ops->initialize(camera_device, &camera_callbacks);

            // Configure the stream
            std::cout << "Configuring camera " << camera_number << std::endl;
            camera_stream_config.num_streams = 1;
            camera_stream_config.streams = new camera3_stream_t*[1];
            camera_stream_config.streams[0] = create_camera_configuration(new camera3_stream_t, camera_number);
            
            camera_device->ops->configure_streams(camera_device, &camera_stream_config);
        }
    }
    else
    {
        std::cout << "Open not implemented in Camera HAL" << std::endl;
    }

#if 0
    for(auto& x : camera_info_array)
    {
        delete x;
    }
#endif

    return 0;
}
