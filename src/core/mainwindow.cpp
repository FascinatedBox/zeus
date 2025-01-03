#include "core/mainwindow.h"
#include "core/commandengine.h"
#include "core/usercommand.h"
#include "tabs/actiontab.h"
#include "tabs/streamtab.h"
#include <QCloseEvent>
#include <QStatusBar>
#include <QTabWidget>

#define ZEUS_TAB_FN(type, name, args, value)                                   \
  type *ZeusMainWindow::create##name##Tab args {                               \
    auto tab = new type value;                                                 \
                                                                               \
    m_tabWidget->addTab(tab, #name);                                           \
    return tab;                                                                \
  }

ZEUS_TAB_FN(ZeusActionTab, Action, (ZeusPulseData * pd, ZeusCommandEngine *ce),
            (pd, ce, m_cm))
ZEUS_TAB_FN(ZeusStreamTab, Playback, (ZeusPulseData * pd), (pd, ZISinkInput))
ZEUS_TAB_FN(ZeusStreamTab, Record, (ZeusPulseData * pd), (pd, ZISourceOutput))

ZeusMainWindow::ZeusMainWindow(ZeusUserCommandManager *cm) {
  m_tabWidget = new QTabWidget;
  m_cm = cm;

  statusBar()->showMessage("", 1000);
  setCentralWidget(m_tabWidget);
  resize(600, 400);
}

void ZeusMainWindow::onActionResult(QPair<int, QString> result) {
  statusBar()->showMessage(result.second, 5000);
}

void ZeusMainWindow::onCommandResults(
    QPair<QString, QList<QPair<int, QString>>> data) {
  QString commandName = data.first;
  auto results = data.second;
  int successCount = 0;

  foreach (auto r, results)
    successCount += RESULT_IS_SUCCESS(r);

  QString message;

  if (successCount == results.size())
    message = QString("%1: All actions successful.").arg(commandName);
  else
    message = QString("%1: %2 of %3 actions succeeded.")
                  .arg(commandName)
                  .arg(successCount)
                  .arg(results.size());

  statusBar()->showMessage(message, 5000);
}

void ZeusMainWindow::closeEvent(QCloseEvent *event) {
  m_cm->saveCommands();
  QMainWindow::closeEvent(event);
}
