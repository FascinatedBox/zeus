#ifndef ZEUSPROPERTYWINDOW_H
#define ZEUSPROPERTYWINDOW_H
#include "core/pulsedata.h"
#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

class ZeusPropertyWindow : public QWidget {
  Q_OBJECT
public:
  ZeusPropertyWindow(ZeusPulseData *pd);

private slots:
  void onClientAdded(ZeusPulseClientInfo *info);
  void onSinkAdded(ZeusPulseDeviceInfo *info);
  void onSinkInputAdded(ZeusPulseStreamInfo *info);
  void onSourceAdded(ZeusPulseDeviceInfo *info);
  void onSourceOutputAdded(ZeusPulseStreamInfo *info);
  void onClientRemoved(uint32_t id);
  void onSinkRemoved(uint32_t id);
  void onSinkInputRemoved(uint32_t id);
  void onSourceRemoved(uint32_t id);
  void onSourceOutputRemoved(uint32_t id);

private:
  void setupPropertyTree(void);
  void addChildToItem(QTreeWidgetItem *target, QString name,
                      ZeusPulseBaseInfo *info);
  void removeChildItem(QTreeWidgetItem *target, uint32_t id);

  QTreeWidget *m_tree;
  QTreeWidgetItem *m_clientItem;
  QTreeWidgetItem *m_sourceItem;
  QTreeWidgetItem *m_sourceOutputItem;
  QTreeWidgetItem *m_sinkInputItem;
  QTreeWidgetItem *m_sinkItem;
};

#endif