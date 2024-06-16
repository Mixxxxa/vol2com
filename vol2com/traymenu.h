#pragma once

#include <QObject>
#include <memory>

class QSystemTrayIcon;
class QMenu;

namespace vol2com
{
  class TrayMenu : public QObject
  {
    Q_OBJECT

  public:
    explicit TrayMenu(QObject *parent = nullptr);
    ~TrayMenu();

    static bool Available();

  signals:
    void ShowUI();
    void Exit();

  private:
    static QIcon GetIcon();

    std::unique_ptr<QMenu> m_contextMenu;
    QSystemTrayIcon * const m_tray;
  };
}
