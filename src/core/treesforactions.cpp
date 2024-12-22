#include "core/treesforactions.h"
#include "actions/createpipelineact.h"
#include "actions/createvirtualsinkact.h"
#include "actions/destroyvirtualsinkact.h"
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

#define ADD_PROPERTY_TREES(title, source)                                      \
  do {                                                                         \
    if (source.size() == 0)                                                    \
      break;                                                                   \
    QTreeWidgetItem *propItem = new QTreeWidgetItem;                           \
    propItem->setText(0, title);                                               \
    propItem->setData(0, ITEM_GROUP_ROLE, UserActionValue);                    \
    for (int i = 0; i < source.size(); i++) {                                  \
      QTreeWidgetItem *item = new QTreeWidgetItem;                             \
      auto p = source[i];                                                      \
      item->setText(0, QString("%1=%2").arg(p.first).arg(p.second));           \
      item->setData(0, ITEM_GROUP_ROLE, UserActionValue);                      \
      propItem->addChild(item);                                                \
    }                                                                          \
    propItem->setExpanded(true);                                               \
    actItem->addChild(propItem);                                               \
  } while (0)

static void setupCreateVirtualSinkTree(QTreeWidgetItem *parent,
                                       ZeusCreateVirtualSinkAct *a) {
  ADD_ACTION_TREE("Create Virtual Sink");
  ADD_SIMPLE_ACTION_VALUE_TREE("Name: %1", a->name);
  ADD_PROPERTY_TREES("Sink Properties", a->props);
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

static void setupDestroyVirtualSinkTree(QTreeWidgetItem *parent,
                                        ZeusDestroyVirtualSinkAct *a) {
  ADD_ACTION_TREE("Destroy virtual sink");
  ADD_SIMPLE_ACTION_VALUE_TREE("Name: %1", a->name);
  parent->addChild(actItem);
  actItem->setExpanded(true);
}

void addTreesForActions(QTreeWidgetItem *parent,
                        QList<ZeusBaseAction *> actions) {
  for (int i = 0; i < actions.size(); i++) {
    ZeusBaseAction *a = actions[i];

    switch (a->actionType()) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    setup##TitleName##Tree(parent, static_cast<Zeus##TitleName##Act *>(a));    \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
    default:
      break;
    }
  }

  parent->setExpanded(true);
}

void addOneTreeForAction(QTreeWidgetItem *parent, ZeusBaseAction *a) {
  switch (a->actionType()) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    setup##TitleName##Tree(parent, static_cast<Zeus##TitleName##Act *>(a));    \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
    break;
  }
}
