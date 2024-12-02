#ifndef ZEUSSOURCEOUTPUTVIEW_H
#define ZEUSSOURCEOUTPUTVIEW_H
#include <QLabel>
#include <QWidget>

#include "views/streamview.h"

class ZeusSourceOutputView : public ZeusStreamView {
  Q_OBJECT
public:
  ZeusSourceOutputView(QString client, QString name, uint32_t index);
  void setCurrentDeviceIndex(uint32_t deviceIndex);

private slots:
  void updateSourceOutputTargetSource(int);
};

#endif
