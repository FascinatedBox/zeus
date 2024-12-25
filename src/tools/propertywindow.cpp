
#include "tools/propertywindow.h"
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
  connectToPulseData(pd);
  resize(500, 500);
  setLayout(layout);
  setWindowTitle("Property Explorer");
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

#define LOAD_ITER_ITEMS(camelName, TitleName, nameField)                       \
  {                                                                            \
    auto iter = pd->camelName##Iterator();                                     \
    while (iter.hasNext()) {                                                   \
      iter.next();                                                             \
      addChildToItem(m_##camelName##Item, iter.value()->nameField, iter.key(), \
                     iter.value()->props);                                     \
    }                                                                          \
  }

void ZeusPropertyWindow::connectToPulseData(ZeusPulseData *pd) {
  connect(pd, &ZeusPulseData::clientAdded, this,
          &ZeusPropertyWindow::onClientAdded);
  connect(pd, &ZeusPulseData::sinkAdded, this,
          &ZeusPropertyWindow::onSinkAdded);
  connect(pd, &ZeusPulseData::sinkInputAdded, this,
          &ZeusPropertyWindow::onSinkInputAdded);
  connect(pd, &ZeusPulseData::sourceAdded, this,
          &ZeusPropertyWindow::onSourceAdded);
  connect(pd, &ZeusPulseData::sourceOutputAdded, this,
          &ZeusPropertyWindow::onSourceOutputAdded);
  connect(pd, &ZeusPulseData::clientRemoved, this,
          &ZeusPropertyWindow::onClientRemoved);
  connect(pd, &ZeusPulseData::sinkRemoved, this,
          &ZeusPropertyWindow::onSinkRemoved);
  connect(pd, &ZeusPulseData::sinkInputRemoved, this,
          &ZeusPropertyWindow::onSinkInputRemoved);
  connect(pd, &ZeusPulseData::sourceRemoved, this,
          &ZeusPropertyWindow::onSourceRemoved);
  connect(pd, &ZeusPulseData::sourceOutputRemoved, this,
          &ZeusPropertyWindow::onSourceOutputRemoved);

  LOAD_ITER_ITEMS(sourceOutput, SourceOutput, name);
  LOAD_ITER_ITEMS(sink, Sink, desc);
  LOAD_ITER_ITEMS(sinkInput, SinkInput, name);
  LOAD_ITER_ITEMS(source, Source, desc);
  LOAD_ITER_ITEMS(client, Client, name);
}

void ZeusPropertyWindow::addChildToItem(QTreeWidgetItem *target, QString name,
                                        uint32_t id,
                                        QHash<QString, QString> props) {
  QTreeWidgetItem *child = new QTreeWidgetItem;
  child->setText(0, name);
  child->setData(0, ITEM_SERIAL, id);

  QStringList keys = props.keys();
  std::sort(keys.begin(), keys.end());

  foreach (QString k, keys) {
    QTreeWidgetItem *p = new QTreeWidgetItem;

    p->setText(0, QString("%1 = %2").arg(k).arg(props[k]));
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

void ZeusPropertyWindow::onClientAdded(uint32_t id, ZeusPulseClientInfo *info) {
  addChildToItem(m_clientItem, info->name, id, info->props);
}

void ZeusPropertyWindow::onSinkAdded(uint32_t id, ZeusPulseDeviceInfo *info) {
  addChildToItem(m_sinkItem, info->desc, id, info->props);
}

void ZeusPropertyWindow::onSinkInputAdded(uint32_t id,
                                          ZeusPulseStreamInfo *info) {
  addChildToItem(m_sinkInputItem, info->name, id, info->props);
}

void ZeusPropertyWindow::onSourceAdded(uint32_t id, ZeusPulseDeviceInfo *info) {
  addChildToItem(m_sourceItem, info->desc, id, info->props);
}

void ZeusPropertyWindow::onSourceOutputAdded(uint32_t id,
                                             ZeusPulseStreamInfo *info) {
  addChildToItem(m_sourceOutputItem, info->name, id, info->props);
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
