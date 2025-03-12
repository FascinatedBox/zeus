#ifndef ZEUSSTREAMVIEW_H
#define ZEUSSTREAMVIEW_H
#include <QWidget>

class QLabel;
class ZeusDeviceComboBox;
class ZeusPulseData;
class ZeusPulseStreamInfo;

class ZeusStreamView : public QWidget {
  Q_OBJECT

public:
  ZeusStreamView(ZeusPulseData *, ZeusPulseStreamInfo *);

  uint32_t index(void) { return m_index; }
  void syncToInfo(ZeusPulseStreamInfo *);

private slots:
  void updateSourceOutputTargetSource(int);
  void updateSinkInputTargetSink(int);

private:
  uint32_t m_index;
  uint32_t m_lastTarget;
  QLabel *m_clientNameLabel;
  QLabel *m_nameLabel;
  ZeusDeviceComboBox *m_deviceCombo;
};

#endif
