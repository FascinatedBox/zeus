#ifndef ZEUSSINKINPUTVIEW_H
#define ZEUSSINKINPUTVIEW_H
#include <QLabel>
#include <QWidget>

#include "views/streamview.h"

class ZeusSinkInputView : public ZeusStreamView {
  Q_OBJECT
public:
  ZeusSinkInputView(QString client, QString name, uint32_t index);
  void setCurrentDeviceIndex(uint32_t deviceIndex);

private slots:
  void updateSinkInputTargetSink(int);
};

#endif
