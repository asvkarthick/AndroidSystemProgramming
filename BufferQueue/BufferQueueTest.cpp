#include <thread>
#include <iostream>
#include <gui/BufferItem.h>
#include <gui/BufferQueue.h>
#include <gui/IProducerListener.h>
#include <gui/IGraphicBufferConsumer.h>
#include <gui/IGraphicBufferProducer.h>
#include <utils/StrongPointer.h>

using namespace android;

sp<IGraphicBufferProducer> producer;
sp<IGraphicBufferConsumer> consumer;

struct TestConsumer : public BnConsumerListener
{
    virtual void onFrameAvailable(const BufferItem& /* item */)
    {
        uint32_t *data;
        BufferItem item;
        status_t res;
        int slot;

        std::cout << __func__ << std::endl;

        res = consumer->acquireBuffer(&item, 0);
        if(res != OK) {
            std::cout << "Error in acquiring buffer" << std::endl;
            return;
        }
        consumer->detachBuffer(item.mSlot);
        item.mGraphicBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN,
                     reinterpret_cast<void**>(&data));
        std::cout << "mSlot = " << item.mSlot << " Frame # " << item.mFrameNumber << std::endl;
        std::cout << "Output data = " << *data << std::endl;
        item.mGraphicBuffer->unlock();

        sp<GraphicBuffer> safeToClobberBuffer;
        consumer->attachBuffer(NULL, safeToClobberBuffer);
        consumer->attachBuffer(&slot, NULL);
        consumer->attachBuffer(&slot, item.mGraphicBuffer);
        consumer->releaseBuffer(slot, 0, EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE);
        // consumer->releaseBuffer(item.mSlot, item.mFrameNumber, EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE);

        std::cout << __func__ << " End" << std::endl;
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
    sp<TestConsumer> tc(new TestConsumer);
    IGraphicBufferProducer::QueueBufferOutput output;
    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    uint32_t *input_data, *output_data;

    std::cout << "BufferQueueTest Start" << std::endl;

    BufferQueue::createBufferQueue(&producer, &consumer);
    consumer->consumerConnect(tc, false);
    producer->connect(NULL, NATIVE_WINDOW_API_CPU, false, &output);
    producer->setMaxDequeuedBufferCount(10);
    consumer->setMaxAcquiredBufferCount(10);
    for(auto i = 0; i < 10000; i++) {
        producer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN);
        producer->requestBuffer(slot, &buffer);
        std::cout << "Slot = " << slot << std::endl;
        buffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN,
                     reinterpret_cast<void**>(&input_data));
        *input_data = 12345678 + i;
        buffer->unlock();
        IGraphicBufferProducer::QueueBufferInput input(0, false,
                     HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
                     NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
        producer->queueBuffer(slot, input, &output);
    }

    std::cout << "BufferQueueTest End" << std::endl;

    return 0;
}
