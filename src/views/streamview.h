#ifndef ZEUSSTREAMVIEW_H
#define ZEUSSTREAMVIEW_H
#include <QLabel>
#include <QWidget>

class ElidingLabel;
class ZeusDeviceComboBox;
class ZeusPulseData;
class ZeusPulseStreamInfo;

class ZeusStreamView : public QWidget {
  Q_OBJECT

public:
  ZeusStreamView(ZeusPulseData *, ZeusPulseStreamInfo *);

  uint32_t index(void) { return m_index; }

private slots:
  void updateSourceOutputTargetSource(int);
  void updateSinkInputTargetSink(int);

private:
  uint32_t m_index;
  QLabel *m_clientNameLabel;
  ElidingLabel *m_nameLabel;
  ZeusDeviceComboBox *m_deviceCombo;
};

#endif
