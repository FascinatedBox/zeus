#ifndef ZEUSTOOLTAB_H
#define ZEUSTOOLTAB_H
#include <QWidget>

class QListWidget;
class ZeusPropertyWindow;
class ZeusPulseData;

class ZeusToolTab : public QWidget {
  Q_OBJECT

public:
  ZeusToolTab(ZeusPulseData *pd);

private slots:
  void onExec(void);

private:
  QListWidget *m_toolList;
  ZeusPulseData *m_pd;
  ZeusPropertyWindow *m_propertyWindow;
};

#endif
