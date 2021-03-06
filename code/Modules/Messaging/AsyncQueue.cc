//------------------------------------------------------------------------------
//  AsyncQueue.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "AsyncQueue.h"

namespace Oryol {
    
OryolClassPoolAllocImpl(AsyncQueue);

//------------------------------------------------------------------------------
AsyncQueue::AsyncQueue() {
    // empty
}
    
//------------------------------------------------------------------------------
AsyncQueue::~AsyncQueue() {
    /// @todo: should we issue a warning if the queue is not empty?
    this->queue.Clear();
    this->forwardingPort = 0;
}

//------------------------------------------------------------------------------
void
AsyncQueue::DoWork() {
    if (this->forwardingPort) {
        this->forwardingPort->DoWork();
    }
}

//------------------------------------------------------------------------------
bool
AsyncQueue::Put(const Ptr<Message>& msg) {
    this->queue.Enqueue(msg);
    return true;
}

//------------------------------------------------------------------------------
void
AsyncQueue::ForwardMessages() {
    if (this->forwardingPort) {
        while (!this->queue.Empty()) {
            this->forwardingPort->Put(this->queue.Dequeue());
        }
    }
}

//------------------------------------------------------------------------------
void
AsyncQueue::SetForwardingPort(const Ptr<Port>& port) {
    this->forwardingPort = port;
}

//------------------------------------------------------------------------------
const Ptr<Port>&
AsyncQueue::GetForwardingPort() const {
    return this->forwardingPort;
}

//------------------------------------------------------------------------------
int32
AsyncQueue::GetNumQueuedMessages() const {
    return this->queue.Size();
}

} // namespace Oryol