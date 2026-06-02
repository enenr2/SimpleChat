#ifndef SINGLETION_H
#define SINGLETION_H
#include"gloal.h"
#include <iostream>
#include <mutex>
template<typename T>
class singletion{
protected:
    singletion()=default;
    singletion(const singletion<T>&)=delete;
    singletion& operator =(const singletion<T>& st)=delete;
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag s_flag;
        std::call_once(s_flag,[&](){
            _instance=std::shared_ptr<T>(new T);
        });
        return _instance;
    }
    void Printaddress(){
        std::cout<<_instance.get()<<std::endl;

    }
    ~singletion(){
        std::cout<<"这个singletion类已被析构"<<std::endl;
    }
};

template<typename T>
std::shared_ptr<T> singletion<T>::_instance=nullptr;
#endif // SINGLETION_H
