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
  ZeusMovePlaybackStreamDialog(QWidget *parent = nullptr);

  void connectToPulseData(ZeusPulseData *pd);
  ZeusMovePlaybackStreamAct *makeAction(void);

private slots:
  void onSinkAdded(uint32_t index, ZeusPulseDeviceInfo *info);
  void onSinkRemoved(uint32_t index);

private:
  bool isValid(void);
  void loadDeviceList(ZeusPulseData *pd, ZeusDeviceComboBox *combo);

  ZeusDeviceComboBox *m_sinkCombo;
  ZeusQueryPropertyGroupBox *m_queryBox;
};

#endif
