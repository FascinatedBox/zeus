#ifndef ZEUSMOVEPLAYBACKSTREAMDIALOG_H
#define ZEUSMOVEPLAYBACKSTREAMDIALOG_H

#include "actions/moveplaybackstreamact.h"
#include "dialogs/basedialog.h"
#include <QLineEdit>
#include <QWidget>

class ZeusDeviceComboBox;
class ZeusPulseData;
class ZeusPulseDeviceInfo;
class ZeusQueryPropertyGroupBox;

class ZeusMovePlaybackStreamDialog : public ZeusBaseDialog {
  Q_OBJECT

public:
  ZeusMovePlaybackStreamDialog(ZeusPulseData *pd, QWidget *parent = nullptr);

  ZeusMovePlaybackStreamAct *makeAction(void);

private:
  bool isValid(void);
  void loadDeviceList(ZeusPulseData *pd, ZeusDeviceComboBox *combo);

  ZeusDeviceComboBox *m_sinkCombo;
  ZeusQueryPropertyGroupBox *m_queryBox;
};

#endif
