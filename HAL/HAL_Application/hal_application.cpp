#include <iostream>
#include <hardware/hardware.h>
#include <hardware/camera_common.h>

// ID of the HAL implementation
#define HAL_LIBRARY "karthick_hal"

int main()
{
    const hw_module_t *hw_module = nullptr;
    const camera_module_t *mCameraModule = nullptr;
    int ret;

    ret = hw_get_module(HAL_LIBRARY, &hw_module);
    std::cout << "HAL return value = " << ret << std::endl;

    if(ret != 0)
        std::cout << "Error loading HAL module" << std::endl;

    mCameraModule = reinterpret_cast<const camera_module_t*>(hw_module);

    int number_of_cameras = mCameraModule->get_number_of_cameras();

    std::cout << "Number of cameras = " << number_of_cameras << std::endl;

    return 0;
}
