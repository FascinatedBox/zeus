#ifndef ZEUSMAINWINDOW_H
#define ZEUSMAINWINDOW_H
#include <QMainWindow>

class QCloseEvent;
class ZeusActionTab;
class ZeusCommandEngine;
class ZeusPlaybackTab;
class ZeusPulseData;
class ZeusRecordTab;
class ZeusUserCommandManager;

class ZeusMainWindow : public QMainWindow {
  Q_OBJECT

public:
  ZeusMainWindow(ZeusUserCommandManager *cm);
  ZeusActionTab *createActionTab(ZeusCommandEngine *ce, ZeusPulseData *pd);
  ZeusPlaybackTab *createPlaybackTab(void);
  ZeusRecordTab *createRecordTab(void);

public slots:
  void onActionResult(QPair<int, QString> result);
  void onCommandResults(QPair<QString, QList<QPair<int, QString>>> data);

private:
  void closeEvent(QCloseEvent *event);

  QTabWidget *m_tabWidget;
  ZeusUserCommandManager *m_cm;
};

#endif
