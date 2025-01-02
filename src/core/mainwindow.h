#ifndef ZEUSMAINWINDOW_H
#define ZEUSMAINWINDOW_H
#include <QMainWindow>

class QCloseEvent;
class ZeusActionTab;
class ZeusCommandEngine;
class ZeusPulseData;
class ZeusUserCommandManager;

class ZeusMainWindow : public QMainWindow {
  Q_OBJECT

public:
  ZeusMainWindow(ZeusUserCommandManager *cm);
  ZeusActionTab *createActionTab(ZeusPulseData *pd, ZeusCommandEngine *ce);
  void createPlaybackTab(ZeusPulseData *pd);
  void createRecordTab(ZeusPulseData *pd);

public slots:
  void onActionResult(QPair<int, QString> result);
  void onCommandResults(QPair<QString, QList<QPair<int, QString>>> data);

private:
  void closeEvent(QCloseEvent *event);

  QTabWidget *m_tabWidget;
  ZeusUserCommandManager *m_cm;
};

#endif
