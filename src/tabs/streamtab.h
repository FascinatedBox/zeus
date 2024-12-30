#ifndef ZEUSSTREAMTAB_H
#define ZEUSSTREAMTAB_H
#include "core/pulsedata.h"
#include <QWidget>

class ZeusStreamView;

class ZeusStreamTab : public QWidget {
  Q_OBJECT

public:
  ZeusStreamTab(ZeusPulseData *, ZeusPulseInfoType type);

private slots:
  void onSinkInputAdded(ZeusPulseStreamInfo *);
  void onSinkInputRemoved(uint32_t);
  void onSourceOutputAdded(ZeusPulseStreamInfo *);
  void onSourceOutputRemoved(uint32_t);

private:
  void removeStream(uint32_t index);

  ZeusPulseData *m_pd;
  QWidget *m_streamBox;
  QList<ZeusStreamView *> m_views;
};

#endif
