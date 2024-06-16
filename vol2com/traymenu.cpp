#include "traymenu.h"
#include "qsystemtrayicon.h"
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>

using namespace Qt::StringLiterals;

namespace vol2com
{
  TrayMenu::TrayMenu(QObject *parent)
    : QObject      { parent }
    , m_contextMenu{ std::make_unique<QMenu>(nullptr) }
    , m_tray       { new QSystemTrayIcon{ this } }
  {
    Q_ASSERT(TrayMenu::Available());

    auto icon = GetIcon();
    Q_ASSERT(!icon.isNull());

    m_tray->setIcon(icon);

    m_contextMenu->addAction(u"Open"_s, this, &TrayMenu::ShowUI);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(u"Exit"_s, this, &TrayMenu::Exit);
    m_tray->setContextMenu(m_contextMenu.get());

    using EReason = QSystemTrayIcon::ActivationReason;
    QObject::connect(m_tray, &QSystemTrayIcon::activated,
                     this, [this](EReason reason)
                     {
                       if(    reason != EReason::Context
                           && reason != EReason::Unknown)
                       {
                         emit this->ShowUI();
                       }
                     });

    m_tray->show();
  }

  TrayMenu::~TrayMenu() = default;

  bool TrayMenu::Available()
  {
    return QSystemTrayIcon::isSystemTrayAvailable();
  }

  QIcon TrayMenu::GetIcon()
  {
    return QIcon{ u":/qt/qml/vol2com/res/icon-active.svg"_s };
  }
}
