#ifndef ZEUSMOVESTREAMDIALOG_H
#define ZEUSMOVESTREAMDIALOG_H
#include "actions/movestreamact.h"
#include "dialogs/basedialog.h"
#include <QWidget>

class QRadioButton;
class QStackedWidget;
class ZeusDeviceComboBox;
class ZeusPulseData;
class ZeusQueryPropertyGroupBox;

class ZeusMoveStreamDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusMoveStreamDialog(ZeusPulseData *pd, QWidget *parent = nullptr);

  ZeusMoveStreamAct *makeAction(void);

private slots:
  void playbackSelected(void);
  void recordSelected(void);

private:
  void loadStackWidget(ZeusPulseData *pd, int index);
  bool isValid(void);

  QStackedWidget *m_stack;
  ZeusDeviceComboBox *m_combos[2];
  ZeusQueryPropertyGroupBox *m_groups[2];
};

#endif
