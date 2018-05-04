#include "hardware/hardware.h"
#include "hardware/camera_common.h"
#include <iostream>

#define NUM_CAMERAS 2

int get_number_of_cameras()
{
    std::cout << "It's a call to the HAL library " << __func__ << std::endl;
    return NUM_CAMERAS;
}

int open_function( const struct hw_module_t *module, const char *id,
    struct hw_device_t **hw_device )
{
    (void) module;
    (void) hw_device;

    std::cout << "Open function" << std::endl;
    std::cout << id << std::endl;

    return 0;
}

struct hw_module_methods_t mModuleMethods = {
    .open = open_function,
};

static hw_module_t karthick_hal_common = {
    .tag                    = HARDWARE_MODULE_TAG,
    .module_api_version     = HARDWARE_MODULE_API_VERSION(1, 0),
    .hal_api_version        = HARDWARE_HAL_API_VERSION,
    .id                     = "karthick_hal",
    .name                   = "karthick_hal_module",
    .author                 = "Karthick Kumaran",
    .methods                = &mModuleMethods,
    .dso                    = NULL,
    .reserved               = { 0 }
};

camera_module_t HAL_MODULE_INFO_SYM = {
    .common                 = karthick_hal_common,
    .get_number_of_cameras  = get_number_of_cameras,
    .get_camera_info        = NULL,
    .set_callbacks          = NULL,
    .get_vendor_tag_ops     = NULL,
    .open_legacy            = NULL,
    .set_torch_mode         = NULL,
    .init                   = NULL,
    .reserved               = {0}
};
