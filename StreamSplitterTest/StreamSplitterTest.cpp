#include <thread>
#include <iostream>
#include <gui/BufferItem.h>
#include <gui/BufferQueue.h>
#include <gui/IProducerListener.h>
#include <gui/IGraphicBufferConsumer.h>
#include <gui/IGraphicBufferProducer.h>
#include <gui/StreamSplitter.h>
#include <utils/StrongPointer.h>

using namespace android;

struct TestConsumer : public BnConsumerListener
{
    virtual void onFrameAvailable(const BufferItem& /* item */)
    {
        std::cout << __func__ << std::endl;
    }

    virtual void onBuffersReleased()
    {
        std::cout << __func__ << std::endl;
    }

    virtual void onSidebandStreamChanged()
    {
        std::cout << __func__ << std::endl;
    }
};

int main(void)
{
    sp<IGraphicBufferProducer> inputProducer;
    sp<IGraphicBufferConsumer> inputConsumer;
    sp<IGraphicBufferProducer> outputProducer;
    sp<IGraphicBufferConsumer> outputConsumer;
    sp<StreamSplitter> splitter;
    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    uint32_t *inputData, *outputData;
    BufferItem item;
    IGraphicBufferProducer::QueueBufferOutput qbOutput;

    BufferQueue::createBufferQueue(&inputProducer, &inputConsumer);
    BufferQueue::createBufferQueue(&outputProducer, &outputConsumer);
    outputConsumer->consumerConnect(new TestConsumer, false);
    StreamSplitter::createSplitter(inputConsumer, &splitter);
    splitter->addOutput(outputProducer);
    outputProducer->allowAllocation(false);
    inputProducer->connect(NULL, NATIVE_WINDOW_API_CPU, false, &qbOutput);
    inputProducer->dequeueBuffer(&slot, &fence, 0, 0, 0,
                           GRALLOC_USAGE_SW_WRITE_OFTEN);
    inputProducer->requestBuffer(slot, &buffer);
    buffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN,
                 reinterpret_cast<void**>(&inputData));
    *inputData = 0x12345678;
    buffer->unlock();

    IGraphicBufferProducer::QueueBufferInput qbInput(0, false,
                HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
                NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    inputProducer->queueBuffer(slot, qbInput, &qbOutput);

    outputConsumer->acquireBuffer(&item, 0);
    item.mGraphicBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN,
                 reinterpret_cast<void**>(&outputData));
    std::cout << "Output Data = " << *outputData << std::endl;
    item.mGraphicBuffer->unlock();
    outputConsumer->releaseBuffer(item.mSlot, item.mFrameNumber, EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE);

    return 0;
}
