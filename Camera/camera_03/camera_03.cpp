#include <iostream>
#include <hardware/hardware.h>
#include <hardware/camera_common.h>

// ID of the HAL implementation
#define HAL_LIBRARY "camera"

int main()
{
    const hw_module_t *hw_module = nullptr;
    const camera_module_t *mCameraModule = nullptr;
    vendor_tag_ops_t vTag;
    int ret;

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

    mCameraModule = reinterpret_cast<const camera_module_t*>(hw_module);

    // Get vendor tag
    mCameraModule->get_vendor_tag_ops(&vTag);

    // Call init() of Camera HAL
    if(mCameraModule->init)
    {
        std::cout << "Camera module has init" << std::endl;
        mCameraModule->init();
    }
    else
        std::cout << "Camera module does not have init" << std::endl;

    int number_of_cameras = mCameraModule->get_number_of_cameras();

    std::cout << "Number of cameras = " << number_of_cameras << std::endl;

    return 0;
}
