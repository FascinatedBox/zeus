#ifndef ZEUSMAINWINDOW_H
#define ZEUSMAINWINDOW_H
#include <QMainWindow>

class ZeusPlaybackTab;
class ZeusRecordTab;

class ZeusMainWindow : public QMainWindow {
  Q_OBJECT

public:
  ZeusMainWindow(void);
  ZeusPlaybackTab *createPlaybackTab(void);
  ZeusRecordTab *createRecordTab(void);

private:
  QTabWidget *m_tabWidget;
};

#endif
