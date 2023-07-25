/*********************************************************************
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A implement of SimplerTimer 
 * 
 * Referance : https://blog.csdn.net/u012234115/article/details/89857431
 * Referance : https://www.cnblogs.com/gtarcoder/p/4924097.html
 * 
*********************************************************************/

#pragma once

#include <functional>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace PNCUtils{
    class SimpleTimer{
    public:
        SimpleTimer(): _expired(true), _try_to_expire(false){};
        SimpleTimer(const SimpleTimer & timer){
            _expired = timer._expired.load();
            _try_to_expire = timer._try_to_expire.load();
        }

        void startSync(int interval, std::function<void()> task); // rel = interval+task
        void startASync(int interval, std::function<void()> task); // rel = interval
        void startOnce(int delay, std::function<void()> task);
        void stop();

    private:
        std::atomic<bool> _expired; // timer stopped status
        std::atomic<bool> _try_to_expire; // timer is in stop process
        std::mutex _mutex;
        std::condition_variable _expired_cond;

    };

    template<typename callable, class... arguments>
    void SyncTimerOnce(int after, callable&& f, arguments&&... args){
        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }
    template<typename callable, class... arguments>
    void ASyncTimerOnce(int after, callable&& f, arguments&&... args){
        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
 
        std::thread([after, task](){
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }).detach();
    }

} // namespace PNCUtils