#include "core/mainwindow.h"
#include "core/commandengine.h"
#include "tabs/actiontab.h"
#include "tabs/playbacktab.h"
#include "tabs/recordtab.h"
#include <QTabWidget>

#define ZEUS_TAB_FN(name, args, value)                                         \
  Zeus##name##Tab *ZeusMainWindow::create##name##Tab args {                    \
    Zeus##name##Tab *tab = new Zeus##name##Tab value;                          \
                                                                               \
    m_tabWidget->addTab(tab, #name);                                           \
    return tab;                                                                \
  }

ZEUS_TAB_FN(Action, (ZeusCommandEngine * ce, ZeusPulseData *pd), (ce, pd))
ZEUS_TAB_FN(Playback, (void), ())
ZEUS_TAB_FN(Record, (void), ())

ZeusMainWindow::ZeusMainWindow(void) {
  m_tabWidget = new QTabWidget;

  setCentralWidget(m_tabWidget);
  resize(600, 400);
}
