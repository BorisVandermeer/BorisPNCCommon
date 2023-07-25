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

#include<thread>
#include<PNCUtils/SimpleTimer.h>



namespace PNCUtils{
namespace{

} // namespace <void>


    void SimpleTimer::startSync(int interval, std::function<void()> task){
        // is started, do not start again
        if (_expired == false) return;

        _expired = false;
        std::thread([this, interval, task]()->void{
            while (!_try_to_expire)
            {
                // sleep every interval and do the task again and again until times up
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                task();
            }

            {
                // timer be stopped, update the condition variable expired and wake main thread
                std::lock_guard<std::mutex> locker(_mutex);
                _expired = true;
                _expired_cond.notify_one();
            }
        }).detach();
    }

    void SimpleTimer::startASync(int interval, std::function<void()> task){
        if (_expired == false) return;

        _expired = false;
        std::thread([this, interval, task]()->void{
            while (!_try_to_expire)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                std::thread(task).detach();
            }

            {
                // timer be stopped, update the condition variable expired and wake main thread
                std::lock_guard<std::mutex> locker(_mutex);
                _expired = true;
                _expired_cond.notify_one();
            }
        }).detach();
    }

    void SimpleTimer::startOnce(int delay, std::function<void()> task){
        std::thread([delay, task]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            task();
        }).detach();
    }

    void SimpleTimer::stop(){
        // do not stop again
        if (_expired)
            return;

        if (_try_to_expire)
            return;

        // wait until timer 
        _try_to_expire = true; // change this bool value to make timer while loop stop
        {
            std::unique_lock<std::mutex> locker(_mutex);
            _expired_cond.wait(locker, [this] {return _expired == true; });

            // reset the timer
            if (_expired == true)
                _try_to_expire = false;
        }
    }

    


}