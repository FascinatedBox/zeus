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
  void onClientAdded(uint32_t id, ZeusPulseClientInfo *info);
  void onSinkAdded(uint32_t id, ZeusPulseDeviceInfo *info);
  void onSinkInputAdded(uint32_t id, ZeusPulseStreamInfo *info);
  void onSourceAdded(uint32_t id, ZeusPulseDeviceInfo *info);
  void onSourceOutputAdded(uint32_t id, ZeusPulseStreamInfo *info);
  void onClientRemoved(uint32_t id);
  void onSinkRemoved(uint32_t id);
  void onSinkInputRemoved(uint32_t id);
  void onSourceRemoved(uint32_t id);
  void onSourceOutputRemoved(uint32_t id);

private:
  void setupPropertyTree(void);
  void connectToPulseData(ZeusPulseData *pd);
  void addChildToItem(QTreeWidgetItem *target, QString name, uint32_t id,
                      QHash<QString, QString> props);
  void removeChildItem(QTreeWidgetItem *target, uint32_t id);

  QTreeWidget *m_tree;
  QTreeWidgetItem *m_clientItem;
  QTreeWidgetItem *m_sourceItem;
  QTreeWidgetItem *m_sourceOutputItem;
  QTreeWidgetItem *m_sinkInputItem;
  QTreeWidgetItem *m_sinkItem;
};

#endif