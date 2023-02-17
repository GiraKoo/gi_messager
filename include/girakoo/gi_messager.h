#include <string>

namespace girakoo
{
    /**
     * @brief GiMessage结构体基类
     * 
     * @details
     *      1. 本类是最基本的消息数据。不携带任何参数，使用者在处理可以通过id，sender，receiver区分的消息时。可以直接使用。
     *      2. 如果发送者需要携带复杂的参数，应当继承本类。并处理好内部的内存问题。（类内成员变量的资源创建与释放）
     *      3. 建议使用者在使用前，使用dynamic_cast<CustomGiMessage*>(message);的形式进行数据转换。保证处理的类型与发送者一致。
    */
    class GiMessage
    {
    public:
        /**
         * @brief GiMessage的构造函数，需要传递必要参数
         * 
         * @param id 消息ID，用于区分消息
         * @param sender 消息发送者ID
         * @param receiver 指定消息接收者ID，如果不指定消息处理者，将会分发给所有监听者。
        */
        GiMessage(const std::string id, const std::string& sender, const std::string& receiver = "");

        /**
         * @brief 消息销毁
         * @details
         *      1. GiMessage的创建，由消息发送者通过new操作符进行创建。
         *      2. GiMessage的析构，由GiMessageLoop在消息分发完毕后，使用delete进行析构。
         * 
         * @warning 自定义GiMessage对象，如果存在申请内存空间，应当在析构函数中进行释放。
        */
        virtual ~GiMessage() {}

        /**
         * @brief 获得消息ID，用于区分消息
         * @return 消息ID
        */
        virtual const std::string& getId() const { return m_id; }

        /**
         * @brief 获得消息发送者
         * @return 消息发送者
        */
        virtual const std::string& getSender() const { return m_sender; }

        /**
         * @brief 获得消息接收者
         * 
         * @note 当此参数为空时，将默认发送给全部监听者
         * @return 消息接受者
        */
        virtual const std::string& getReceiver() const { return m_receiver; }

        /**
         * @brief 获得消息的时间戳
         * 
         * @return size_t 
         */
        virtual size_t getTimestamp() const { return m_timestamp; }

    private:
        std::string m_id;
        std::string m_sender;
        std::string m_receiver;
        size_t m_timestamp;
    };

    class GiMessageObserver
    {
    public:
        /**
         * @brief 消息分发处理
         * 
         * @param message 消息参数。该参数可能是GiMessage
         * 
         * @return 
        */
        virtual bool onGiMessageReceived(const GiMessage* message) = 0;

        /**
         * @brief GiMessageLoop开始循环通知
        */
        virtual void onGiMessageLoopEnter() {}

        /**
         * @brief GiMessageLoop退出循环通知
        */
        virtual void onGiMessageLoopExit() {}
    };

    class GiMessageLoop
    {
    public:
        /**
         * @brief 创建GiMessageLoop对象
         * 
         * @param id 指定GiMessageLoop的ID
         * @note 创建一个GiMessageLoop对象。
         * 
         * @return 
        */
        static GiMessageLoop* create(const std::string& id);
        
        /**
         * @brief 销毁GiMessageLoop对象
         * 
         * @param id 指定GiMessageLoop的ID
         */
        static void destory(const std::string& id);
        
        /**
         * @brief 获得当前线程
         * 
         * @param id 指定GiMessageLoop的ID
         * 
         * @note 内部使用简单的for循环进行遍历。效率较低，不适合频繁调用。
         * @return GiMessageLoop句柄
         */
        static GiMessageLoop* getInstance(const std::string& id);

    public:

        /**
         * @brief 添加监听者
         * @param observer 监听者
        */
        virtual void addObserver(GiMessageObserver* observer);

        /**
         * @brief 移除监听者
         * @param observer 监听者
        */
        virtual void removeObserver(const GiMessageObserver* observer);

        /**
         * @brief 消息loop是否开始执行
         * @retval true 已经开始Loop
         * @retval false 未开始Loop
        */
        virtual bool isRunning() const;

        /**
         * @brief 消息循环的部
         * 
         * @return 返回外部调用exit函数退出循环时的入参。
        */
        virtual int loop();

        /**
         * @brief 发送退出消息，loop将会退出
         * 
         * @param exitCode 退出码，将会在loop返回时调用。
         * @param sync true:同步调用，当前线程将会被阻塞。false:异步调用
         * 
         * @return 是否成功执行
        */
        virtual bool exit(int exitCode, bool sync);

        /**
         * @brief 同步函数，强制处理消息队列中的全部消息
         * 
         * @detail 
         *      该函数用于强制同步其他消息队列状态。
         *      在某些场景下，异步处理消息可能会导致不同线程间获取状态不一致。
         *      强制子线程处理完全部消息后，可以保证线程间数据一致。
         * 
         * @note 在销毁GiMessageLoop时，会内部调用一次drain，保证所有人获得消息通知
         * 
         * @return 是否成功执行
        */
        virtual bool drain();

        /**
         * @brief 发送消息
         * @param message 发送消息。该参数指向空间应当通过new操作符进行申请。
         * 
         * @return 是否成功执行
        */
        virtual bool send(GiMessage* message);

        /**
         * @brief 消息队列中消息的数量
         * @return 消息数量
        */
        virtual size_t count() const;
    };
}