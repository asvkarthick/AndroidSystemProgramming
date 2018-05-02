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

using namespace android;

// Interface class
class ICalculator : public IInterface
{
public:
    enum
    {
        ADD = IBinder::FIRST_CALL_TRANSACTION,
        SUB,
        MUL,
        DIV,
        MOD,
        BUF   // Provide shared buffer fd
    };

    // Requests the service to perform an addition and return the result
    virtual int32_t add(int32_t val1, int32_t val2) = 0;
    // Requests the service to perform subtraction and return the result
    virtual int32_t sub(int32_t val1, int32_t val2) = 0;
    // Requests the service to perform multiplication and return the result
    virtual int32_t mul(int32_t val1, int32_t val2) = 0;
    // Requests the service to perform division and return the result
    virtual int32_t div(int32_t val1, int32_t val2) = 0;
    // Requests the service to perform modulo and return the result
    virtual int32_t mod(int32_t val1, int32_t val2) = 0;
    // Requests the service to allocate memory in ashmem and share
    virtual int32_t buf(int32_t size, char* buffer) = 0;

    DECLARE_META_INTERFACE(Calculator);
    // The above line expands as follows
    // static const android::String16 descriptor;
    // static android::sp<ICalculator> asInterface(const android::sp<android::IBinder>& obj);
    // virtual const android::String16& getInterfaceDescriptor() const;
    // ICalculator();
    // virtual ~ICalculator();
};

// Client interface
class BpCalculator : public BpInterface<ICalculator>
{
public:
    BpCalculator(const sp<IBinder>& impl) : BpInterface(impl) {}

    virtual int32_t add(int32_t val1, int32_t val2);

    virtual int32_t sub(int32_t val1, int32_t val2);

    virtual int32_t mul(int32_t val1, int32_t val2);

    virtual int32_t div(int32_t val1, int32_t val2);

    virtual int32_t mod(int32_t val1, int32_t val2);

    virtual int32_t buf(int32_t size, char* buffer);
};

// Producer interface
class BnCalculator : public BnInterface<ICalculator>
{
    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
};

IMPLEMENT_META_INTERFACE(Calculator, "Calculator_Demo");

