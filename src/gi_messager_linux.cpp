#include "girakoo/gi_messager.h"
#include <mutex>
#include <unordered_map>

#ifdef __linux__
#include "sys/time.h"
#endif

using namespace girakoo;

GiMessage::GiMessage(const std::string id, const std::string& sender, const std::string& receiver = "") :
    m_id(id), m_sender(sender), m_receiver(receiver), m_timestamp(0)
{
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv, NULL);
    m_timestamp = tv.tv_sec * 1000 + tv.tv_usec;
#else
    // Needs todo
    static_assert(0);
#endif
}

class GiMessageLoopLinux : public GiMessageLoop
{
public:
    GiMessageLoopLinux(const std::string id) :
        m_id(id)
    { }

    virtual ~GiMessageLoopLinux()
    { }

private:
    std::string m_id;
    std::mutex m_mutex;
};

static std::mutex g_giMessageLoopMutex;
static std::unordered_map<std::string, GiMessageLoop*> g_giMessageLoopArray;

GiMessageLoop* GiMessageLoop::create(const std::string& id)
{
    // 写锁
    std::lock_guard<std::mutex> lock(g_giMessageLoopMutex);
    auto instance = new GiMessageLoopLinux(id);

    g_giMessageLoopArray[id] = instance;

    return instance;
}

void GiMessageLoop::destory(const std::string& id)
{
    // 写锁
    std::lock_guard<std::mutex> lock(g_giMessageLoopMutex);

    auto iter = g_giMessageLoopArray.find(id);
    if (iter != g_giMessageLoopArray.end())
    {
        delete iter->second;
        g_giMessageLoopArray.erase(iter);
    }
}

GiMessageLoop* GiMessageLoop::getInstance(const std::string& id)
{
    // 读锁
    std::lock_guard<std::mutex> lock(g_giMessageLoopMutex);
    
    auto iter = g_giMessageLoopArray.find(id);
    if (iter != g_giMessageLoopArray.end())
    {
        return iter->second;
    }
    return nullptr;
}