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

int32_t BpCalculator::buf(int32_t size, char* buffer)
{
    Parcel data, reply;
    android::Parcel::ReadableBlob blob;

    data.writeInterfaceToken(ICalculator::getInterfaceDescriptor());
    data.writeInt32(size);
    remote()->transact(BUF, data, &reply);

    int32_t size_to_read;
    reply.readInt32(&size_to_read);
    reply.readBlob(size_to_read, &blob);
    void* tmp = const_cast<void*> (blob.data());
    memcpy(buffer, tmp, size_to_read);
    blob.release();

    return size_to_read;
}

#define BUFFER_SIZE (640 * 480 * 2)

char buffer[BUFFER_SIZE];

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
    
    // Reading data from shared memory
    std::cout << "Buf size = " << calculator->buf(BUFFER_SIZE, buffer) << std::endl;
    {
        FILE* fout;
        fout = fopen("output.bin", "wb");
        fwrite(buffer, 1, BUFFER_SIZE, fout);
        fclose(fout);
    }

    return 0;
}
