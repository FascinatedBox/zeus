#ifndef ZEUSMAINWINDOW_H
#define ZEUSMAINWINDOW_H
#include <QMainWindow>

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
  void onActionResult(QPair<bool, QString> result);
  void onCommandResults(QPair<QString, QList<QPair<bool, QString>>> data);

private:
  void closeEvent(QCloseEvent *event);

  QTabWidget *m_tabWidget;
  ZeusActionTab *m_actionTab;
  ZeusUserCommandManager *m_cm;
};

#endif
