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

signals:
  void sendMoveFailed(void);

private slots:
  void updateSourceOutputTargetSource(int lastComboIndex);
  void updateSinkInputTargetSink(int lastComboIndex);

private:
  static void streamMoveCallback(void *, int success, void *data);

  uint32_t m_index;
  int m_lastComboIndex;
  QLabel *m_nameLabel;
  ZeusDeviceComboBox *m_deviceCombo;
};

#endif
