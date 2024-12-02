#ifndef ZEUSSTREAMTAB_H
#define ZEUSSTREAMTAB_H
#include <QWidget>
#include <pulse/pulseaudio.h>

class ZeusStreamView;

class ZeusStreamTab : public QWidget {
  Q_OBJECT

public:
  ZeusStreamTab(void);

  void clientAdded(const pa_client_info *i);
  void clientRemoved(uint32_t index);

protected:
  void deviceAdded(uint32_t index, const char *desc);
  void deviceRemoved(uint32_t index);
  void streamAdded(ZeusStreamView *, uint32_t deviceIndex);
  void streamRemoved(uint32_t index);

  QWidget *m_streamBox;
  QList<ZeusStreamView *> m_views;
  QMap<uint32_t, QString> m_clientNames;
  QMap<uint32_t, QString> m_devices;
};

#endif
