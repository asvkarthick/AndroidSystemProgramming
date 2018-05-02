#include <stdlib.h>

#include <utils/RefBase.h>
#include <utils/Log.h>
#include <binder/TextOutput.h>

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <iostream>
#include "binder_interface.h"

class BpCalculator : public BpInterface<ICalculator>
{
public:
    BpCalculator(const sp<IBinder>& impl) : BpInterface(impl) {}

    virtual int32_t add(int32_t val1, int32_t val2)
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

    virtual int32_t sub(int32_t val1, int32_t val2)
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

    virtual int32_t mul(int32_t val1, int32_t val2)
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

    virtual int32_t div(int32_t val1, int32_t val2)
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

    virtual int32_t mod(int32_t val1, int32_t val2)
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
};
