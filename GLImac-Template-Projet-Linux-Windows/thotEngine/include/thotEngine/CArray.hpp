#ifndef CARRAY_HPP
#define CARRAY_HPP

#include <typeindex>
#include <typeinfo>
#include <memory>
#include <map>
#include <vector>
#include <list>

namespace te{

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

template<typename T>
class Handler : public IHandler
{
public :
    Handler(CMap* user, int index);

    T* operator->();
    inline int index() const
    {
        return m_index;
    }

    inline virtual bool istypeof(const std::type_index& type) override
    {
        return (type == m_type);
    }

private :
    int m_index;
    CMap* m_user;
    std::type_index m_type;

};


template<typename T>
class ICArray : public BaseCArray
{
public :
    virtual int instantiate() = 0; //active a new element in the array
    virtual int instantiate(T element) = 0; //active a new element in the array, copy element in it
    virtual void remove(const Handler<T>& handler) = 0;
    virtual T& get(const Handler<T>& handler) = 0;
    virtual bool isActive(int index) const = 0;

    virtual T& operator[](int index) = 0;
};

template<typename T>
class CArray : public ICArray<T>
{

public :
    CArray();
    ~CArray();

    virtual int instantiate() override; //active a new element in the array
    virtual int instantiate(T element) override; //active a new element in the array, copy element in it
    virtual void remove(const Handler<T>& handler) override;
    virtual T& get(const Handler<T>& handler) override;
    virtual T& operator[](int index) override;

    inline virtual bool isActive(int index) const override
    {
        return m_status.size() > index ? m_status[index]:false;
    }

    inline int size() const
    {
        return m_size;
    }

private :
    std::vector<T> m_content;
    std::vector<bool> m_status;
    std::list<unsigned int> m_inactives;
    int m_size;

};

template<typename T>
Handler<T>::Handler(CMap* user, int index): m_index(index)
{
    m_user = user;
}

template<typename T>
T* Handler<T>::operator->()
{
    //return m_user->getArrayElement<T>();
    return &std::static_pointer_cast<CArray<T>>( (*m_user)[typeid(T)] )->operator[](m_index);
}

template<typename T>
CArray<T>::CArray() : m_size(0)
{ 
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
            m_content.push_back(T());
            m_status.push_back(true);
        }
        else
        {
            index = m_size;
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
            m_content.push_back(element);
            m_status.push_back(true);
        }
        else
        {
            index = m_size;
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
void CArray<T>::remove(const Handler<T>& handler)
{
    m_status[handler.index()] = false;
    m_inactives.push_back(handler.index());
    if(m_size - 1 == handler.index())
    {
        while(!m_status[m_size - 1] && (m_size - 1)>=0)
        {
            m_size--;
        }
    }
}

template<typename T>
T& CArray<T>::get(const Handler<T>& handler)
{
    return m_content[handler.index()];
}

template<typename T>
T& CArray<T>::operator[](int index)
{
    return m_content[index];
}

}



#endif // CARRAY_HPP
