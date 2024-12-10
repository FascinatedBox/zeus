#include "core/treesforactions.h"
#include "actions/createpipelineact.h"
#include "actions/createvirtualsinkact.h"
#include <QTreeWidgetItem>

// This must sync to actiontab.cpp's ItemGroup definiton.
enum ItemGroup {
  PredefinedTopLevel,
  PredefinedAction,
  UserTopLevel,
  UserCommand,
  UserAction,
  UserActionValue,
};

#define ITEM_GROUP_ROLE (Qt::UserRole)

#define ZEUS_ACTION_CASE(name)                                                 \
  case ZeusActionType::ZA##name:                                               \
    setup##name##Tree(parent, static_cast<Zeus##name##Act *>(a));              \
    break;

#define ADD_ACTION_TREE(text)                                                  \
  QTreeWidgetItem *actItem = new QTreeWidgetItem;                              \
  actItem->setText(0, text);                                                   \
  actItem->setData(0, ITEM_GROUP_ROLE, UserAction);

#define ADD_SIMPLE_ACTION_VALUE_TREE(format, value)                            \
  {                                                                            \
    QTreeWidgetItem *item = new QTreeWidgetItem;                               \
    item->setText(0, QString(format).arg(value));                              \
    item->setData(0, ITEM_GROUP_ROLE, UserActionValue);                        \
    actItem->addChild(item);                                                   \
  }

static void setupCreateVirtualSinkTree(QTreeWidgetItem *parent,
                                       ZeusCreateVirtualSinkAct *a) {
  ADD_ACTION_TREE("Create Virtual Sink");
  ADD_SIMPLE_ACTION_VALUE_TREE("Name: %1", a->name);
  parent->addChild(actItem);
  actItem->setExpanded(true);
}

static void setupCreatePipelineTree(QTreeWidgetItem *parent,
                                    ZeusCreatePipelineAct *a) {
  ADD_ACTION_TREE("Create Pipeline");
  ADD_SIMPLE_ACTION_VALUE_TREE("Source Device: %1", a->sourceName);
  ADD_SIMPLE_ACTION_VALUE_TREE("Playback Device: %1", a->sinkName);
  parent->addChild(actItem);
  actItem->setExpanded(true);
}

void addTreesForActions(QTreeWidgetItem *parent,
                        QList<ZeusBaseAction *> actions) {
  for (int i = 0; i < actions.size(); i++) {
    ZeusBaseAction *a = actions[i];

    switch (a->getActionType()) {
      ZEUS_ACTION_CASE(CreateVirtualSink)
      ZEUS_ACTION_CASE(CreatePipeline)
    default:
      break;
    }
  }

  parent->setExpanded(true);
}

void addOneTreeForAction(QTreeWidgetItem *parent, ZeusBaseAction *a) {
  switch (a->getActionType()) {
    ZEUS_ACTION_CASE(CreateVirtualSink)
    ZEUS_ACTION_CASE(CreatePipeline)
  default:
    break;
  }
}
