//
// Created by 水瀬あくあ on 2021/5/7.
//

#include "Task.h"
#include "IRun.h"

Task::Task(IRun0 *func)
    :_func0(func)
    ,_func2(nullptr) {

}

Task::Task(IRun2 *func, const std::string &str, void *param)
    :_func0(nullptr)
    ,_func2(func)
    ,_str(str)
    ,_param(param) {

}

void Task::doTask() {
    if(_func0) {
        _func0->run0();
        return;
    }
    if(_func2) {
        _func2->run2(_str, _param);
        return;
    }
}
