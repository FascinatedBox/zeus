#include "widgets/querypreviewtree.h"
#include "core/pulsedata.h"
#include <QHeaderView>

#define COLUMN_COUNT 3
#define ITEM_INDEX (Qt::UserRole)

// String typo == bug, macro typo == compile error.
#define Z_APPLICATION_PROCESS_BINARY "application.process.binary"
#define Z_MEDIA_NAME "media.name"
#define Z_OBJECT_SERIAL "object.serial"

ZeusQueryPreviewTree::ZeusQueryPreviewTree(ZeusPulseData *pd) : QTreeWidget() {
  setHeaderLabels(QStringList()
                  << Z_OBJECT_SERIAL << Z_APPLICATION_PROCESS_BINARY
                  << Z_MEDIA_NAME);

  // The middle one is a mouthful.
  header()->resizeSection(1, 200);
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusQueryPreviewTree, sinkInput, SinkInput);
}

void ZeusQueryPreviewTree::onSinkInputAdded(ZeusPulseStreamInfo *info) {
  QTreeWidgetItem *item = new QTreeWidgetItem;
  ZeusPropHash p = info->props;
  QString serial = p.value(Z_OBJECT_SERIAL);
  QString appName = p.value(Z_APPLICATION_PROCESS_BINARY);
  QString mediaName = p.value(Z_MEDIA_NAME);

  item->setData(0, ITEM_INDEX, info->index);
  item->setText(0, serial);
  item->setText(1, appName);
  item->setText(2, mediaName);
  addTopLevelItem(item);

  // The tree allows selections even if the mode is set to not allow them.
  // This prevents all selections, ensuring that only the coloring from marking
  // items is visible on the tree.
  item->setDisabled(true);
}

void ZeusQueryPreviewTree::onSinkInputRemoved(uint32_t id) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *item = topLevelItem(i);

    if (id != item->data(0, ITEM_INDEX).toInt())
      continue;

    delete takeTopLevelItem(i);
    break;
  }
}

void ZeusQueryPreviewTree::markSelected(QList<ZeusPulseStreamInfo *> selected) {
  QBrush foundBrush = QBrush(QColor(Qt::darkBlue));
  QBrush clearBrush = QBrush();

  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *item = topLevelItem(i);
    uint32_t id = item->data(0, ITEM_INDEX).toInt();
    bool found = false;

    foreach (auto s, selected) {
      if (id != s->index)
        continue;

      found = true;
      break;
    }

    QBrush itemBrush = (found == true) ? foundBrush : clearBrush;

    for (int i = 0; i < COLUMN_COUNT; i++)
      item->setBackground(i, itemBrush);
  }
}
