#include <iostream>

template<typename T>
class UniquePtr {
private:
    T* ptr;

public:
    //构造
    UniquePtr() {
        ptr = nullptr;
    }

    UniquePtr(T* ptr) {
        this->ptr = ptr;
    }

    //禁用拷贝构造和拷贝赋值
    UniquePtr(const UniquePtr& uptr) = delete;
    UniquePtr& operator=(const UniquePtr& uptr) = delete;

    //移动构造和移动复制
    UniquePtr(UniquePtr&& uptr) {
        this->ptr = uptr.ptr;
        uptr.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& uptr) {
        if(this->ptr != nullptr) delete this->ptr;
        this->ptr = uptr.ptr;
        uptr.ptr = nullptr;
        return *this;
    }

    //析构
    ~UniquePtr() {
        if(this->ptr != nullptr) delete this->ptr;
    }

    //解引用
    T& operator*() {
        return *ptr;
    }
    T* operator->() {
        return ptr;
    }

    //为空指针
    bool operator!() {
        return (ptr == nullptr);
    }

    T* get() {
        return ptr;
    }
};

template<typename T>
class WeakPtr;

template<typename T>
class SharePtr {
    // friend WeakPtr<T>::WeakPtr(const SharePtr<T>&); --> Function 'SharePtr<T>::WeakPtr' must have return type
    friend class WeakPtr<T>;
private:
    T* ptr;
    int* count; // 严谨的话用原子操作（std::atomic<>）， 解决多线程问题

public:
    //构造
    SharePtr() {
        ptr = nullptr;
        count = nullptr;
    }

    SharePtr(T* ptr) {
        this->ptr = ptr;
        count = new int;
        (*count) = 1;
    }

    //拷贝构造
    SharePtr(const SharePtr& sptr) {
        if(sptr.ptr == nullptr) return;
        ptr = sptr.ptr;
        count = sptr.count;
        ++(*count);
    }

    SharePtr& operator=(const SharePtr& sptr) {
        if(sptr.ptr == nullptr) return *this;
        ptr = sptr.ptr;
        count = sptr.count;
        (*count)++;
        return *this;
    }

    //移动构造
    SharePtr(SharePtr&& sptr) {
        ptr = sptr.ptr;
        count = sptr.count;
        sptr.ptr = nullptr;
        sptr.count = nullptr;
    }

    SharePtr& operator=(SharePtr&& sptr) {
        ptr = sptr.ptr;
        count = sptr.count;
        sptr.ptr = nullptr;
        sptr.count = nullptr;
        return *this;
    }

    //析构
    ~SharePtr() {
        if(ptr == nullptr) return;
        --(*count);
        std::cout << "share break1\n";
        if(!(*count)) {
            std::cout << "share break2\n";
            delete ptr;
            delete count;
        }
    }

    // 解决weakPtr的lock问题
    SharePtr(T* ptr, int* count) {
        this->ptr = ptr;
        this->count = count;
        if(this->ptr != nullptr) ++(*count);
    }

    //解引用
    T& operator*() {
        return *ptr;
    }
    T* operator->() {
        return ptr;
    }

    //空指针
    bool operator!() {
        return (ptr == nullptr);
    }

    T* get() {
        return ptr;
    }

};

template<typename T>
class WeakPtr {
private:
    T* ptr;
    int* count;

public:
    WeakPtr() {
        ptr = nullptr;
        count = nullptr;
    }

    WeakPtr(const SharePtr<T>& sptr) {
        ptr = sptr.ptr;
        count = sptr.count;
    }

    SharePtr<T> lock() {
        return SharePtr<T>(ptr, count);
    }
};

int main() {
    // unique_ptr
    UniquePtr<int> uptr(new int);
    *uptr = 114514;
    std::cout << (*uptr) << '\n';
    UniquePtr<int> Uptr(std::move(uptr));
    std::cout << (*(Uptr.get())) << '\n';

    // share_ptr
    SharePtr<int> sptr(new int);
    *sptr = 202536;
    std::cout << (*sptr) << '\n';
    {
        SharePtr<int> Sptr(sptr);
        std::cout << (*Sptr) << '\n';
    }
    std::cout << "waiting...\n";

    //weak_ptr
    WeakPtr<int> wptr(sptr);
    std::cout << (*(wptr.lock())) << '\n';
}