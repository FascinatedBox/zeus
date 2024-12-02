#include "core/mainwindow.h"
#include "tabs/playbacktab.h"
#include "tabs/recordtab.h"
#include <QTabWidget>

#define ZEUS_TAB_FN(name)                                                      \
  Zeus##name##Tab *ZeusMainWindow::create##name##Tab(void) {                   \
    Zeus##name##Tab *tab = new Zeus##name##Tab();                              \
                                                                               \
    m_tabWidget->addTab(tab, #name);                                           \
    return tab;                                                                \
  }

ZEUS_TAB_FN(Playback)
ZEUS_TAB_FN(Record)

ZeusMainWindow::ZeusMainWindow(void) {
  m_tabWidget = new QTabWidget;

  setCentralWidget(m_tabWidget);
  resize(600, 400);
}
