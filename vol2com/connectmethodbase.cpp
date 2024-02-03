#include "connectmethodbase.h"

namespace vol2com
{
  ConnectMethodBase::ConnectMethodBase(QObject *parent)
    : V2CBase { parent }
    , m_state { State::Idle }
  {
  }

  ConnectMethodBase::State ConnectMethodBase::state() const
  {
    return m_state;
  }

  void ConnectMethodBase::setState(State state)
  {
    if (m_state == state)
    {
      return;
    }

    m_state = state;
    emit stateChanged(m_state);
  }

  QString ConnectMethodBase::qmlDelegatesPath()
  {
    using namespace Qt::StringLiterals;
    return u"qrc:/qt/qml/vol2com/qml/"_s;
  }
}
