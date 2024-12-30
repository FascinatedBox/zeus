#ifndef ZEUSQUERYPREVIEWTREE_H
#define ZEUSQUERYPREVIEWTREE_H
#include "core/pulsedata.h"
#include <QTreeWidget>

class ZeusPulseData;
class ZeusPulseStreamInfo;

class ZeusQueryPreviewTree : public QTreeWidget {
  Q_OBJECT

public:
  ZeusQueryPreviewTree(ZeusPulseData *pd);

  void markSelected(QList<ZeusPulseStreamInfo *> selected);

private slots:
  void onSinkInputAdded(ZeusPulseStreamInfo *info);
  void onSinkInputRemoved(uint32_t id);
};

#endif
