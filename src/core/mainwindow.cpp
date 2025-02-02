#include "core/mainwindow.h"
#include "core/commandengine.h"
#include "core/usercommand.h"
#include "tabs/actiontab.h"
#include "tabs/commandtab.h"
#include "tabs/streamtab.h"
#include <QStatusBar>
#include <QTabWidget>

ZeusMainWindow::ZeusMainWindow(ZeusUserCommandManager *cm) {
  m_tabWidget = new QTabWidget;
  m_cm = cm;

  statusBar()->showMessage("", 1000);
  setCentralWidget(m_tabWidget);
  resize(600, 400);
}

void ZeusMainWindow::closeEvent(QCloseEvent *event) {
  m_commandTab->saveCommands();
  QMainWindow::closeEvent(event);
}

ZeusActionTab *ZeusMainWindow::createActionTab(ZeusPulseData *pd,
                                               ZeusCommandEngine *ce) {
  ZeusActionTab *actionTab = new ZeusActionTab(pd, ce);

  m_tabWidget->addTab(actionTab, "Action");
  return actionTab;
}

ZeusCommandTab *ZeusMainWindow::createCommandTab(ZeusPulseData *pd,
                                                 ZeusCommandEngine *ce) {
  m_commandTab = new ZeusCommandTab(pd, ce, m_cm);

  m_tabWidget->addTab(m_commandTab, "User Commands");
  return m_commandTab;
}

void ZeusMainWindow::createPlaybackTab(ZeusPulseData *pd) {
  m_tabWidget->addTab(new ZeusStreamTab(pd, ZISinkInput), "Playback");
}

void ZeusMainWindow::createRecordTab(ZeusPulseData *pd) {
  m_tabWidget->addTab(new ZeusStreamTab(pd, ZISourceOutput), "Record");
}

void ZeusMainWindow::onActionResult(QPair<bool, QString> result) {
  statusBar()->showMessage(result.second, 5000);
}

void ZeusMainWindow::onCommandResults(
    QPair<QString, QList<QPair<bool, QString>>> data) {
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
