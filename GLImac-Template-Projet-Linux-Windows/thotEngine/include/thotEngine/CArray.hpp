#ifndef CARRAY_HPP
#define CARRAY_HPP

#include <typeindex>
#include <typeinfo>
#include <memory>
#include <map>
#include <vector>
#include <list>
#include <algorithm>

namespace te{


//class ICArrayUser
//{
//    template<typename T>
//    virtual T* getArrayElement(int index) = 0;
//};
template<typename T>
class ICArray;

template<typename T>
class CArray; //forward

class BaseCArray{

};

//template<typename T>
typedef std::map<std::type_index, std::shared_ptr<BaseCArray> > CMap;

class IHandler
{
public:
    virtual bool istypeof(const std::type_index& type) const = 0;
};

class Handler : public IHandler
{
public:
   Handler();
   Handler(std::type_index type, int index);

   inline virtual bool istypeof(const std::type_index& type) const override
   {
       return (type == m_type);
   }

   inline int getIndex() const
   {
       return m_index;
   }

   inline std::type_index getType() const
   {
       return m_type;
   }

   operator bool() const
   {
       return m_index >=0;
   }

   bool isValid() const
   {
       return m_valid;
   }

protected :
   bool m_valid;
   int m_index;
   std::type_index m_type;
};

template<typename T>
class ExternalHandler : public Handler
{
public :
    ExternalHandler();
    ExternalHandler(std::shared_ptr<BaseCArray> user, int index);
    ExternalHandler(const Handler& handler, std::shared_ptr<BaseCArray> user);
    ExternalHandler(std::shared_ptr<BaseCArray> user, int index, std::type_index type);

    T* operator->() const;

    template<typename U>
    operator ExternalHandler<U>()
    {
        ExternalHandler<U> result( m_user.lock(), m_index);
        return result;
    }
    template<typename U>
    ExternalHandler(const ExternalHandler<U>& other)
    {
        m_valid = other.isValid();
        m_user = other.getUser();
        m_index = other.getIndex();
        m_type = other.getType();
    }
    template<typename U>
    ExternalHandler& operator=(const ExternalHandler<U>& other)
    {
        m_valid = other.isValid();
        m_user = other.getUser();
        m_index = other.getIndex();
        m_type = other.getType();

        return *this;
    }

    operator bool() const
    {
        return m_valid;
    }

    inline std::weak_ptr<BaseCArray> getUser() const
    {
        return m_user.lock();
    }

private :
    std::weak_ptr<BaseCArray> m_user;

};


template<typename T>
class BaseWorldObject
{
public:
    BaseWorldObject(){}
    inline virtual ~BaseWorldObject(){}

    virtual void setHandler(std::shared_ptr<BaseCArray> user, int index) = 0;
    virtual ExternalHandler<T> getHandler() = 0;
};

class WorldObject
{
protected:
    int m_index;
public :
    WorldObject(){}
    inline virtual ~WorldObject(){}

    void setIndex(int i){
        m_index = i;
    }

    int getIndex() const{
        return m_index;
    }
};



template<typename T>
class ICArray : public BaseCArray
{
public :
    virtual int instantiate() = 0; //active a new element in the array
    virtual int instantiate(T element) = 0; //active a new element in the array, copy element in it
    virtual void remove(const Handler& handler) = 0;
    virtual T& get(const Handler& handler) = 0;
    virtual bool isActive(int index) const = 0;
    virtual T& operator[](int index) = 0;
    virtual T& parse(int index) = 0;
    virtual void sort() = 0;
};

template<typename T>
class CArray : public ICArray<T>, public std::enable_shared_from_this<CArray<T>>
{

public :
    CArray();
    ~CArray();

    virtual int instantiate() override; //active a new element in the array
    virtual int instantiate(T element) override; //active a new element in the array, copy element in it
    virtual void remove(const Handler& handler) override;
    virtual T& get(const Handler& handler) override;
    virtual T& operator[](int index) override;
    virtual T& parse(int index) override;
    virtual void sort() override;

    inline virtual bool isActive(int index) const override
    {
        return m_status.size() > index ? m_status[index]:false;
    }

    inline int size() const
    {
        return m_size;
    }

private :
    std::vector<int> m_pointers;
    std::vector<T> m_content;
    std::vector<bool> m_status;
    std::list<unsigned int> m_inactives;
    int m_size;

};

template<typename T>
ExternalHandler<T>::ExternalHandler(): Handler()
{

}

template<typename T>
ExternalHandler<T>::ExternalHandler(std::shared_ptr<BaseCArray> user, int index, std::type_index type): Handler(type, index)
{
    m_user = user;
}

template<typename T>
ExternalHandler<T>::ExternalHandler(std::shared_ptr<BaseCArray> user, int index): Handler(typeid(T), index)
{
    m_user = user;
}

template<typename T>
ExternalHandler<T>::ExternalHandler(const Handler& handler, std::shared_ptr<BaseCArray> user)
{
    m_valid = handler.isValid();
    m_index = handler.getIndex();
    m_type = handler.getType();
    m_user = user;
}

template<typename T>
T* ExternalHandler<T>::operator->() const
{
    //return m_user->getArrayElement<T>();
    //return &std::static_pointer_cast<CArray<T>>( (*m_user)[typeid(T)] )->operator[](m_index);
    return &std::static_pointer_cast<CArray<T>>( m_user.lock() )->operator[](m_index);
}

template<typename T>
CArray<T>::CArray() : m_size(0)
{ 
    m_pointers.resize(10);
    m_content.resize(10);
    m_status.resize(10);

    for(int i = 0; i < 10; i++)
    {
        //m_content[i].setIndex(i);
        m_content[i].setHandler( this->shared_from_this() , i);
    }

    for(int i = 0; i < 10; i++)
    {
        m_status[i] = false;
    }

    for(int i = 0; i < 10; i++)
    {
        m_pointers[i] = i;
    }
}

template<typename T>
CArray<T>::~CArray()
{

}

template<typename T>
int CArray<T>::instantiate()
{
    int index = 0;
    if(m_inactives.size() == 0 )
    {
        if(m_content.size() < size())
        {
            index = m_content.size();
            m_pointers.push_back(index);
            m_content.push_back(T());
            //m_content.back().setIndex(index);
            m_content.back().setHandler( this->shared_from_this() , index);
            m_status.push_back(true);
        }
        else
        {
            index = m_size;
            m_pointers[m_size] = index;
            m_status[m_size] = true;
            m_size++;
        }
    }
    else
    {
        index = m_inactives.front();
        m_status[index] = true;
        m_inactives.pop_front();
    }

    return index;
}

template<typename T>
int CArray<T>::instantiate(T element)
{
    int index = 0;

    if(m_inactives.size() == 0 )
    {
        if(m_content.size() < size())
        {
            index = m_content.size();
            m_pointers.push_back(index);
            m_content.push_back(element);
            //m_content.back().setIndex(index);
            m_content.back().setHandler( this->shared_from_this() , index);
            m_status.push_back(true);
        }
        else
        {
            index = m_size;
            m_pointers[m_size] = index;
            m_status[m_size] = true;
            m_content[m_size] = element;
            m_size++;
        }
    }
    else
    {
        index = m_inactives.front();
        m_status[index] = true;
        m_content[index] = element;
        m_inactives.pop_front();
    }

    return index;
}

template<typename T>
void CArray<T>::remove(const Handler& handler)
{
    m_status[handler.getIndex()] = false;
    m_inactives.push_back(handler.getIndex());
    if(m_size - 1 == handler.getIndex())
    {
        while(!m_status[m_size - 1] && (m_size - 1)>=0)
        {
            m_size--;
        }
    }
}

template<typename T>
T& CArray<T>::get(const Handler& handler)
{
    return m_content[ m_pointers[handler.getIndex()] ];
}

template<typename T>
T& CArray<T>::operator[](int index)
{
    return m_content[ m_pointers[index] ];
}

template<typename T>
T& CArray<T>::parse(int index)
{
    return m_content[index];
}

template<typename T>
void CArray<T>::sort()
{
    std::sort(m_content.begin(), m_content.end());
}


}



#endif // CARRAY_HPP
