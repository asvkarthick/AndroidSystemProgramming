#include <stdlib.h>

#include <utils/RefBase.h>
#include <utils/Log.h>
#include <binder/TextOutput.h>

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryHeapBase.h>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include "binder_interface.h"
#include <linux/ion.h>

int32_t ion_allocate_and_share_buffer(int *fd, ion_user_handle_t* handle, int size)
{
    int ion_fd;
    ion_fd = open("/dev/ion", O_RDWR | O_CLOEXEC);
    if(ion_fd < 0)
    {
        std::cout << "Error opening /dev/ion" << std::endl;
        return -1;
    }

    struct ion_allocation_data data = {
        .len = static_cast<size_t>(size),
        .align = 0,
        .heap_id_mask = 1,
        .flags = 0
    };
    int ret = ioctl(ion_fd, ION_IOC_ALLOC, &data);
    if(ret < 0)
    {
        std::cerr << "Error in allocating ION buffer" << std::endl;;
        return -1;
    }
    *handle = data.handle;

    struct ion_fd_data data_fd = {
        .handle = data.handle
    };
    ret = ioctl(ion_fd, ION_IOC_SHARE, &data_fd);
    if(ret < 0)
    {
        std::cerr << "Error in sharing ION buffer" << std::endl;
        return -1;
    }
    *fd = data_fd.fd;
    *handle = data.handle;
    return 0;
}

int32_t BpCalculator::add(int32_t val1, int32_t val2)
{
    Parcel data, reply;

    data.writeInterfaceToken(ICalculator::getInterfaceDescriptor());
    data.writeInt32(val1);
    data.writeInt32(val2);
    remote()->transact(ADD, data, &reply);

    int32_t result;
    reply.readInt32(&result);
    return result;
}

int32_t BpCalculator::sub(int32_t val1, int32_t val2)
{
    Parcel data, reply;

    data.writeInterfaceToken(ICalculator::getInterfaceDescriptor());
    data.writeInt32(val1);
    data.writeInt32(val2);
    remote()->transact(SUB, data, &reply);

    int32_t result;
    reply.readInt32(&result);
    return result;
}

int32_t BpCalculator::mul(int32_t val1, int32_t val2)
{
    Parcel data, reply;

    data.writeInterfaceToken(ICalculator::getInterfaceDescriptor());
    data.writeInt32(val1);
    data.writeInt32(val2);
    remote()->transact(MUL, data, &reply);

    int32_t result;
    reply.readInt32(&result);
    return result;
}

int32_t BpCalculator::div(int32_t val1, int32_t val2)
{
    Parcel data, reply;

    data.writeInterfaceToken(ICalculator::getInterfaceDescriptor());
    data.writeInt32(val1);
    data.writeInt32(val2);
    remote()->transact(DIV, data, &reply);

    int32_t result;
    reply.readInt32(&result);
    return result;
}

int32_t BpCalculator::mod(int32_t val1, int32_t val2)
{
    Parcel data, reply;

    data.writeInterfaceToken(ICalculator::getInterfaceDescriptor());
    data.writeInt32(val1);
    data.writeInt32(val2);
    remote()->transact(MOD, data, &reply);

    int32_t result;
    reply.readInt32(&result);
    return result;
}

int32_t BpCalculator::buf(int fd, int32_t size)
{
    Parcel data, reply;

    data.writeInterfaceToken(ICalculator::getInterfaceDescriptor());
    data.writeFileDescriptor(fd);
    data.writeInt32(size);
    // remote()->transact(BUF, data, &reply, IBinder::FLAG_ONEWAY);
    remote()->transact(BUF, data, &reply);
    int32_t result;
    reply.readInt32(&result);
    return result;
}

#define BUFFER_SIZE (640 * 480 * 2)

int main(int argc, char* argv[])
{
    sp<IBinder> binder = defaultServiceManager()->getService(String16("Calculator"));
    if(binder == nullptr)
    {
        std::cout << "Binder NULL" << std::endl;
        return -1;
    }
    sp<ICalculator> calculator = interface_cast<ICalculator>(binder);
    std::cout << "Add (3, 2) = " << calculator->add(3, 2) << std::endl;
    std::cout << "Sub (3, 2) = " << calculator->sub(3, 2) << std::endl;
    std::cout << "Mul (3, 2) = " << calculator->mul(3, 2) << std::endl;
    std::cout << "Div (3, 2) = " << calculator->div(3, 2) << std::endl;
    std::cout << "Mod (3, 2) = " << calculator->mod(3, 2) << std::endl;
    
    // Getting FD of shared memory
    int fd;
    ion_user_handle_t handle;
    int ret = ion_allocate_and_share_buffer(&fd, &handle, BUFFER_SIZE);

    char* buffer_ptr = reinterpret_cast<char*>(mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    strcpy(buffer_ptr, "Kumaran");
    ret = calculator->buf(fd, BUFFER_SIZE);
    std::cout << "Ret buf = " << ret << std::endl;
    std::cout << "Buffer = " << buffer_ptr << std::endl;

    return 0;
}
