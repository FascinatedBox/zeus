#include "core/mainwindow.h"
#include "core/commandengine.h"
#include "core/usercommand.h"
#include "tabs/actiontab.h"
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
  m_actionTab->saveCommands();
  QMainWindow::closeEvent(event);
}

ZeusActionTab *ZeusMainWindow::createActionTab(ZeusPulseData *pd,
                                               ZeusCommandEngine *ce) {
  m_actionTab = new ZeusActionTab(pd, ce, m_cm);

  m_tabWidget->addTab(m_actionTab, "Action");
  return m_actionTab;
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
