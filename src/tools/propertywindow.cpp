#include "tools/propertywindow.h"
#include "core/pulsedata.h"
#include <QComboBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

#define ITEM_SERIAL (Qt::UserRole)

ZeusPropertyWindow::ZeusPropertyWindow(ZeusPulseData *pd) {
  m_tree = new QTreeWidget;
  m_tree->setHeaderHidden(true);
  QVBoxLayout *layout = new QVBoxLayout;

  layout->addWidget(m_tree);
  setupPropertyTree();
  resize(500, 500);
  setLayout(layout);
  setWindowTitle("Property Explorer");

  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusPropertyWindow, client, Client);
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusPropertyWindow, sink, Sink);
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusPropertyWindow, sinkInput, SinkInput);
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusPropertyWindow, source, Source);
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusPropertyWindow, sourceOutput,
                          SourceOutput);
}

#define ADD_TOPLEVEL_ITEM(name, title)                                         \
  {                                                                            \
    name = new QTreeWidgetItem;                                                \
    name->setText(0, title);                                                   \
    m_tree->addTopLevelItem(name);                                             \
    name->setExpanded(true);                                                   \
  }

void ZeusPropertyWindow::setupPropertyTree(void) {
  ADD_TOPLEVEL_ITEM(m_sourceItem, "Input Devices");
  ADD_TOPLEVEL_ITEM(m_sourceOutputItem, "Record");
  ADD_TOPLEVEL_ITEM(m_sinkInputItem, "Playback");
  ADD_TOPLEVEL_ITEM(m_sinkItem, "Output Devices");
  ADD_TOPLEVEL_ITEM(m_clientItem, "Clients");
}

void ZeusPropertyWindow::addChildToItem(QTreeWidgetItem *target, QString name,
                                        ZeusPulseBaseInfo *info) {
  QTreeWidgetItem *child = new QTreeWidgetItem;
  child->setText(0, name);
  child->setData(0, ITEM_SERIAL, info->index);

  QStringList keys = info->props.keys();
  std::sort(keys.begin(), keys.end());

  foreach (QString k, keys) {
    QTreeWidgetItem *p = new QTreeWidgetItem;

    p->setText(0, QString("%1 = %2").arg(k).arg(info->props[k]));
    child->addChild(p);
  }

  target->addChild(child);
  child->setExpanded(false);
}

void ZeusPropertyWindow::removeChildItem(QTreeWidgetItem *target, uint32_t id) {
  for (int i = 0; i < target->childCount(); i++) {
    QTreeWidgetItem *child = target->child(i);

    if (child->data(0, ITEM_SERIAL) != id)
      continue;

    delete target->takeChild(i);
    break;
  }
}

void ZeusPropertyWindow::onClientAdded(ZeusPulseClientInfo *info) {
  addChildToItem(m_clientItem, info->name, info);
}

void ZeusPropertyWindow::onSinkAdded(ZeusPulseDeviceInfo *info) {
  addChildToItem(m_sinkItem, info->desc, info);
}

void ZeusPropertyWindow::onSinkInputAdded(ZeusPulseStreamInfo *info) {
  addChildToItem(m_sinkInputItem, info->name, info);
}

void ZeusPropertyWindow::onSourceAdded(ZeusPulseDeviceInfo *info) {
  addChildToItem(m_sourceItem, info->desc, info);
}

void ZeusPropertyWindow::onSourceOutputAdded(ZeusPulseStreamInfo *info) {
  addChildToItem(m_sourceOutputItem, info->name, info);
}

void ZeusPropertyWindow::onClientRemoved(uint32_t id) {
  removeChildItem(m_clientItem, id);
}

void ZeusPropertyWindow::onSinkRemoved(uint32_t id) {
  removeChildItem(m_sinkItem, id);
}

void ZeusPropertyWindow::onSinkInputRemoved(uint32_t id) {
  removeChildItem(m_sinkInputItem, id);
}

void ZeusPropertyWindow::onSourceRemoved(uint32_t id) {
  removeChildItem(m_sourceItem, id);
}

void ZeusPropertyWindow::onSourceOutputRemoved(uint32_t id) {
  removeChildItem(m_sourceOutputItem, id);
}
