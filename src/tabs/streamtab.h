#ifndef ZEUSSTREAMTAB_H
#define ZEUSSTREAMTAB_H
#include "core/pulsedata.h"
#include <QWidget>

class ZeusStreamView;

class ZeusStreamTab : public QWidget {
  Q_OBJECT

public:
  ZeusStreamTab(ZeusPulseData *, ZeusPulseInfoType type);

signals:
  void sendMessage(QString);

private slots:
  void onMoveFailed(void);
  void onSinkInputAdded(ZeusPulseStreamInfo *);
  void onSinkInputRemoved(uint32_t);
  void onSinkInputUpdated(ZeusPulseStreamInfo *);
  void onSourceOutputAdded(ZeusPulseStreamInfo *);
  void onSourceOutputRemoved(uint32_t);
  void onSourceOutputUpdated(ZeusPulseStreamInfo *);

private:
  void updateStream(ZeusPulseStreamInfo *);
  void removeStream(uint32_t index);

  ZeusPulseData *m_pd;
  QWidget *m_streamBox;
  QList<ZeusStreamView *> m_views;
};

#endif
