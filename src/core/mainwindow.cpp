#include "core/mainwindow.h"
#include "core/commandengine.h"
#include "core/usercommand.h"
#include "tabs/actiontab.h"
#include "tabs/playbacktab.h"
#include "tabs/recordtab.h"
#include <QCloseEvent>
#include <QStatusBar>
#include <QTabWidget>

#define ZEUS_TAB_FN(name, args, value)                                         \
  Zeus##name##Tab *ZeusMainWindow::create##name##Tab args {                    \
    Zeus##name##Tab *tab = new Zeus##name##Tab value;                          \
                                                                               \
    m_tabWidget->addTab(tab, #name);                                           \
    return tab;                                                                \
  }

ZEUS_TAB_FN(Action, (ZeusCommandEngine * ce, ZeusPulseData *pd), (ce, pd, m_cm))
ZEUS_TAB_FN(Playback, (void), ())
ZEUS_TAB_FN(Record, (void), ())

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

void ZeusMainWindow::closeEvent(QCloseEvent *event) {
  m_cm->saveCommands();
  QMainWindow::closeEvent(event);
}
