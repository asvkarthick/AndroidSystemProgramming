#include <iostream>
#include <vector>
#include <hardware/hardware.h>
#include <hardware/camera_common.h>

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

int main()
{
    const hw_module_t *hw_module = nullptr;
    const camera_module_t *camera_module = nullptr;
    vendor_tag_ops_t vTag;
    camera_module_callbacks_t camera_callbacks;
    camera_info_t *camera_info;
    int camera_number;
    int number_of_cameras;
    int ret;
    std::vector<camera_info_t*> camera_info_array;

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
    camera_callbacks.camera_device_status_change = camera_device_status_change;
    camera_callbacks.torch_mode_status_change = torch_mode_status_change;
    camera_module->set_callbacks(&camera_callbacks);
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

    for(auto& x : camera_info_array)
    {
        delete x;
    }

    return 0;
}
