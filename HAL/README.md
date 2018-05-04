Build the HAL_Library first and copy the generated library "karthick_hal_default.so" to /system/lib/hw in the filesystem

Then build the HAL_Application and copy the generated application "hal-application" to the device in any directory.
Make sure the executable has execute permission
If not provide execute permission with

# chmod +x hal-application

Run the application with the following command

# ./hal-application

Output should be as below for successful execution
==================================================
HAL return value = 0
It's a call to the HAL library function get_number_of_cameras
Number of cameras = 2
