#ifndef ZEUSMAINWINDOW_H
#define ZEUSMAINWINDOW_H
#include <QMainWindow>

class ZeusActionTab;
class ZeusCommandEngine;
class ZeusPlaybackTab;
class ZeusPulseData;
class ZeusRecordTab;

class ZeusMainWindow : public QMainWindow {
  Q_OBJECT

public:
  ZeusMainWindow(void);
  ZeusActionTab *createActionTab(ZeusCommandEngine *ce, ZeusPulseData *pd);
  ZeusPlaybackTab *createPlaybackTab(void);
  ZeusRecordTab *createRecordTab(void);

private:
  QTabWidget *m_tabWidget;
};

#endif
