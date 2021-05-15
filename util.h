#include <chrono>
#include <utility>

#define high_res_now() std::chrono::high_resolution_clock::now()
#define time_elapsed(t) std::chrono::duration_cast<std::chrono::nanoseconds>(high_res_now()-t).count()

template<typename F, typename... Args>
double exec_time(F func, Args&&... args){
    auto time_start= high_res_now();
    func(std::forward<Args>(args)...);
    return time_elapsed(time_start);
}