#ifndef ZEUSSTREAMTAB_H
#define ZEUSSTREAMTAB_H
#include <QWidget>

class ZeusPulseClientInfo;
class ZeusStreamView;

class ZeusStreamTab : public QWidget {
  Q_OBJECT

public:
  ZeusStreamTab(void);

protected slots:
  void onClientAdded(uint32_t index, ZeusPulseClientInfo *info);
  void onClientRemoved(uint32_t index);
  void onDeviceRemoved(uint32_t index);
  void onStreamRemoved(uint32_t index);

protected:
  void deviceAdded(uint32_t index, QString name, QString desc);
  void streamAdded(ZeusStreamView *, uint32_t deviceIndex);

  QWidget *m_streamBox;
  QList<ZeusStreamView *> m_views;
  QMap<uint32_t, QString> m_clientNames;
  // index -> device name, device description
  QMap<uint32_t, QPair<QString, QString>> m_devices;
};

#endif
