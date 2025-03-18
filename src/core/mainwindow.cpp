#include "core/mainwindow.h"
#include "core/commandengine.h"
#include "core/usercommand.h"
#include "tabs/actiontab.h"
#include "tabs/commandtab.h"
#include "tabs/streamtab.h"
#include "tabs/tooltab.h"
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
  m_toolTab->closeTools();
}

ZeusActionTab *ZeusMainWindow::createActionTab(ZeusPulseData *pd,
                                               ZeusCommandEngine *ce) {
  ZeusActionTab *actionTab = new ZeusActionTab(pd, ce);

  m_tabWidget->addTab(actionTab, "Actions");
  connect(actionTab, &ZeusActionTab::sendActionResult, this,
          &ZeusMainWindow::onActionResult);
  return actionTab;
}

ZeusCommandTab *ZeusMainWindow::createCommandTab(ZeusPulseData *pd,
                                                 ZeusCommandEngine *ce) {
  m_commandTab = new ZeusCommandTab(pd, ce, m_cm);

  m_tabWidget->addTab(m_commandTab, "User Commands");
  connect(m_commandTab, &ZeusCommandTab::sendMessage, this,
          &ZeusMainWindow::onSendMessage);
  return m_commandTab;
}

void ZeusMainWindow::createPlaybackTab(ZeusPulseData *pd) {
  ZeusStreamTab *tab = new ZeusStreamTab(pd, ZISinkInput);

  connect(tab, &ZeusStreamTab::sendMessage, this,
          &ZeusMainWindow::onSendMessage);
  m_tabWidget->addTab(tab, "Playback");
}

void ZeusMainWindow::createRecordTab(ZeusPulseData *pd) {
  ZeusStreamTab *tab = new ZeusStreamTab(pd, ZISourceOutput);

  connect(tab, &ZeusStreamTab::sendMessage, this,
          &ZeusMainWindow::onSendMessage);
  m_tabWidget->addTab(tab, "Record");
}

void ZeusMainWindow::createToolTab(ZeusPulseData *pd) {
  m_toolTab = new ZeusToolTab(pd);

  m_tabWidget->addTab(m_toolTab, "Tools");
  connect(m_toolTab, &ZeusToolTab::sendSaveCommands, m_commandTab,
          &ZeusCommandTab::onSendSaveCommands);
}

void ZeusMainWindow::onActionResult(QPair<bool, QString> result) {
  statusBar()->showMessage(result.second, 5000);
}

void ZeusMainWindow::onSendMessage(QString message) {
  statusBar()->showMessage(message, 5000);
}
