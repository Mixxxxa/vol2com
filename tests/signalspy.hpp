#pragma once

#include <QObject>
#include <deque>

namespace vol2com
{
  /*!
   * @brief helper class which allows you to observe the signals emitted by the object (quantity, parameters, values)
   * @details Inspired by QSignalSpy and a discussion of its improvement somewhere on Habr (find the link)
   */
  template<typename T, std::enable_if_t<std::is_base_of_v<QObject, T>, int> = 0, typename... ParamT>
  class SignalSpy
  {
  public:
    using value_type = std::tuple<std::decay_t<ParamT>...>;
    
    static SignalSpy Create(T *signalSource, void (T:: *Method)(ParamT...))
    {
      return SignalSpy<T, 0, ParamT...>(signalSource, Method);
    }
    
    SignalSpy(T *signalSource, void (T::*Method)(ParamT...))
    {
      QObject::connect(signalSource,
                       Method,
                       [this](ParamT... args)
                       {
                         m_calls.push_back(std::make_tuple(args...));
                       });
    }

    size_t size() const noexcept
    {
      return m_calls.size();
    }

    const SignalSpy::value_type &operator[](size_t i) const
    {
      return m_calls[i];
    }

    private:
      using container_type = typename std::deque<value_type>;
      container_type m_calls;
  };
}

