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
            android::Parcel::WritableBlob blob;
            char* buffer;

            size = data.readInt32();
            std::cout << "Size received = " << size << std::endl;
            buffer = new char[size];
            result = buf(size, buffer);
            reply->writeInt32(result);
            reply->writeBlob(size, false, &blob);
            memset(blob.data(), 0x0, size);
            memcpy((void*) blob.data(), (void*) buffer, size);

            delete [] buffer;
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
    virtual int32_t buf(int32_t size, char* buffer)
    {
        int row;
        for(row = 0; row < 480; row++)
        {
            memset((void*)(buffer + row * 640), row % 255, 640);
            std::cout << "Row = " << row << std::endl;
        }
        std::cout << "Size = " << size << std::endl;
        return size;
    }

    char* buffer_data;
};

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
