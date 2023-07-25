/*********************************************************************
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A implement of Vehicle State.
 * 
*********************************************************************/

#include<PNCUtils/Timer.h>

namespace PNCUtils
{
    void Timer::Begin(){
        this->clear();
        start_ = std::chrono::system_clock::now();
    }

    void Timer::Toc(const std::string &task_name){
        end_ = std::chrono::system_clock::now();
        std::chrono::duration<double> use_time = end_ - start_;
        data_.push_back({task_name,use_time.count()});
    }

    void Timer::clear(){
        data_.clear();
    }

    double Timer::Stop() {
        end_ = std::chrono::system_clock::now();
        std::chrono::duration<double> use_time = end_ - start_;
        return use_time.count() * 1000.0;
    }


} // namespace Utils