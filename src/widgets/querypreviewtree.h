#ifndef ZEUSQUERYPREVIEWTREE_H
#define ZEUSQUERYPREVIEWTREE_H
#include "core/pulsedata.h"
#include <QTreeWidget>

class ZeusPulseData;
class ZeusPulseStreamInfo;

class ZeusQueryPreviewTree : public QTreeWidget {
  Q_OBJECT

public:
  ZeusQueryPreviewTree(void);

  void connectToPulseData(ZeusPulseData *pd);
  void markSelected(QList<QPair<uint32_t, ZeusPulseStreamInfo *>> selected);

private slots:
  void onSinkInputAdded(uint32_t id, ZeusPulseStreamInfo *info);
  void onSinkInputRemoved(uint32_t id);

private:
  void loadSinkInputList(ZeusPulseData *);
};

#endif
