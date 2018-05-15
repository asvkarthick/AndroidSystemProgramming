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
#include "binder_interface.h"
#include <ashmem.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/ion.h>

status_t BnCalculator::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    int32_t size, val1, val2, result;
    data.checkInterface(this);

    switch(code)
    {
        case ADD:
            val1 = data.readInt32();
            val2 = data.readInt32();
            result = add(val1, val2);
            reply->writeInt32(result);
            break;
        case SUB:
            val1 = data.readInt32();
            val2 = data.readInt32();
            result = sub(val1, val2);
            reply->writeInt32(result);
            break;
        case MUL:
            val1 = data.readInt32();
            val2 = data.readInt32();
            result = mul(val1, val2);
            reply->writeInt32(result);
            break;
        case DIV:
            val1 = data.readInt32();
            val2 = data.readInt32();
            result = div(val1, val2);
            reply->writeInt32(result);
            break;
        case MOD:
            val1 = data.readInt32();
            val2 = data.readInt32();
            result = mod(val1, val2);
            reply->writeInt32(result);
            break;
        case BUF:
        {
            int fd = data.readFileDescriptor();
            size = data.readInt32();
            result = buf(fd, size);
            reply->writeInt32(result);
            break;
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }

    return NO_ERROR;
}

class Calculator : public BnCalculator
{
public:

    Calculator(char* data) : buffer_data { data } {}

private:

    virtual int32_t add(int32_t val1, int32_t val2)
    {
        return val1 + val2;
    }
    virtual int32_t sub(int32_t val1, int32_t val2)
    {
        return val1 - val2;
    }
    virtual int32_t mul(int32_t val1, int32_t val2)
    {
        return val1 * val2;
    }
    virtual int32_t div(int32_t val1, int32_t val2)
    {
        return val1 / val2;
    }
    virtual int32_t mod(int32_t val1, int32_t val2)
    {
        return val1 % val2;
    }
    virtual int32_t buf(int fd, int32_t size)
    {
        int ion_fd;
        ion_fd = open("/dev/ion", O_RDWR | O_CLOEXEC);
        if(ion_fd < 0)
        {
            std::cerr << "Error opening /dev/ion" << std::endl;
            return -1;
        }
        char* buffer_ptr = reinterpret_cast<char*>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if(buffer_ptr == MAP_FAILED)
        {
            std::cerr << "Error mapping file" << std::endl;
            return -1;
        }
        else
        {
            strcpy(buffer_ptr, buffer_data);
            std::cout << "Copied " << buffer_data << std::endl;
            std::cout << "FD = " << fd << " buffer_ptr = " << buffer_ptr << " size = " << size<< std::endl;
            return 0;
        }
    }
private:
    char* buffer_data;
};

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

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <string>" << std::endl;
        return -1;
    }
    defaultServiceManager()->addService(String16("Calculator"), new Calculator(argv[1]));
    android::ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    return 0;
}
