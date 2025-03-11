#ifndef ZEUSMAINWINDOW_H
#define ZEUSMAINWINDOW_H
#include <QMainWindow>

class ZeusActionTab;
class ZeusCommandTab;
class ZeusCommandEngine;
class ZeusPulseData;
class ZeusToolTab;
class ZeusUserCommandManager;

class ZeusMainWindow : public QMainWindow {
  Q_OBJECT

public:
  ZeusMainWindow(ZeusUserCommandManager *cm);
  ZeusActionTab *createActionTab(ZeusPulseData *pd, ZeusCommandEngine *ce);
  ZeusCommandTab *createCommandTab(ZeusPulseData *pd, ZeusCommandEngine *ce);
  void createPlaybackTab(ZeusPulseData *pd);
  void createRecordTab(ZeusPulseData *pd);
  void createToolTab(ZeusPulseData *pd);

public slots:
  void onActionResult(QPair<bool, QString> result);
  void onSendMessage(QString message);

private:
  void closeEvent(QCloseEvent *event);

  QTabWidget *m_tabWidget;
  ZeusCommandTab *m_commandTab;
  ZeusToolTab *m_toolTab;
  ZeusUserCommandManager *m_cm;
};

#endif
