#ifndef ZEUSQUERYPREVIEWTREE_H
#define ZEUSQUERYPREVIEWTREE_H
#include "core/pulsedata.h"
#include <QTreeWidget>

class ZeusQueryPreviewTree : public QTreeWidget {
  Q_OBJECT

public:
  ZeusQueryPreviewTree(ZeusPulseData *pd, ZeusPulseInfoType type);

  void markSelected(QList<ZeusPulseStreamInfo *> selected);

private slots:
  void onSinkInputAdded(ZeusPulseStreamInfo *info) { addStream(info); }
  void onSinkInputRemoved(uint32_t id) { removeStream(id); }
  void onSourceOutputAdded(ZeusPulseStreamInfo *info) { addStream(info); }
  void onSourceOutputRemoved(uint32_t id) { removeStream(id); }

private:
  void addStream(ZeusPulseStreamInfo *info);
  void removeStream(uint32_t id);
};

#endif
