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

class WorldObject{

};

//class ICArrayUser
//{
//    template<typename T>
//    virtual T* getArrayElement(int index) = 0;
//};
template<typename T>
class ICArray;

class BaseCArray{

};

//template<typename T>
typedef std::map<std::type_index, std::shared_ptr<BaseCArray> > CMap;

class IHandler
{
public:
    virtual bool istypeof(const std::type_index& type) = 0;
};

class Handler : public IHandler
{
public:
   Handler();
   Handler(std::type_index type, int index);

   inline virtual bool istypeof(const std::type_index& type) override
   {
       return (type == *m_type);
   }

   inline int getIndex() const
   {
       return m_index;
   }

   inline std::shared_ptr<std::type_index> getType() const
   {
       return m_type;
   }

   operator bool() const
   {
       return m_index >=0;
   }

protected :
   int m_index;
   std::shared_ptr<std::type_index> m_type;
};

template<typename T>
class ExternalHandler : public Handler
{
public :
    ExternalHandler();
    ExternalHandler(CMap* user, int index);
    ExternalHandler(const Handler& handler, CMap *user);

    T* operator->();

    operator bool() const
    {
        return m_index >=0;
    }

private :
    CMap* m_user;

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
class CArray : public ICArray<T>
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


Handler::Handler(): m_index(-1)
{

}

Handler::Handler(std::type_index type, int index): m_index(index)
{
    m_type = std::make_shared<std::type_index>(type);
}

template<typename T>
ExternalHandler<T>::ExternalHandler(): Handler()
{

}

template<typename T>
ExternalHandler<T>::ExternalHandler(CMap* user, int index): Handler(typeid(T), index)
{
    m_user = user;
}

template<typename T>
ExternalHandler<T>::ExternalHandler(const Handler& handler, CMap* user)
{
    m_index = handler.getIndex();
    m_type = handler.getType();
    m_user = user;
}

template<typename T>
T* ExternalHandler<T>::operator->()
{
    //return m_user->getArrayElement<T>();
    return &std::static_pointer_cast<CArray<T>>( (*m_user)[typeid(T)] )->operator[](m_index);
}

template<typename T>
CArray<T>::CArray() : m_size(0)
{ 
    m_pointers.resize(10);
    m_content.resize(10);
    m_status.resize(10);
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
