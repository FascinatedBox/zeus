#include "tabs/actiontab.h"
#include "core/commandengine.h"
#include "core/pulsedata.h"
#include "core/treesforactions.h"
#include "core/usercommand.h"
#include "dialogs/maybedialogfortype.h"
#include "tools/propertywindow.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

// This maps 1-to-1 with m_buttonGroupStack widgets.
// This must sync to treesforaction.cpp's ItemGroup definiton.
enum ItemGroup {
  ToolTopLevel,
  ToolAction,
  PredefinedTopLevel,
  PredefinedAction,
  UserTopLevel,
  UserCommand,
  UserAction,
  UserActionValue,
};

enum ButtonId {
  // ToolAction buttons
  BShow,

  // PredefinedAction buttons
  BSelect,

  // UserTopLevel buttons
  BNewCommand,
  BSaveCommands,

  // UserCommand buttons
  BNewAction,
  BExecuteCommand,
  BDeleteCommand,

  // UserAction buttons
  BDeleteAction,
};

#define ITEM_GROUP_ROLE (Qt::UserRole)
#define ITEM_ACTION_ID (Qt::UserRole + 1)

ZeusActionTab::ZeusActionTab(ZeusPulseData *pd, ZeusCommandEngine *ce,
                             ZeusUserCommandManager *cm) {
  m_pd = pd;
  m_ce = ce;
  m_cm = cm;
  m_activeDialog = nullptr;
  m_propertyWindow = nullptr;
  QVBoxLayout *layout = new QVBoxLayout;
  m_actionTree = new QTreeWidget;
  m_activeDialog = nullptr;
  m_actionType = ZeusActionType::ZANone;
  m_actionTree->setHeaderHidden(true);
  m_buttonGroupStack = new QStackedWidget;

  setupActionTree();
  setupButtonGroupStack();

  // Stretch = 1 so the tree any extra vertical space available.
  layout->addWidget(m_actionTree, 1);
  layout->addWidget(m_buttonGroupStack);
  setLayout(layout);

  connect(m_actionTree, &QTreeWidget::itemDoubleClicked, this,
          &ZeusActionTab::onItemDoubleClicked);
  connect(m_actionTree, &QTreeWidget::currentItemChanged, this,
          &ZeusActionTab::onCurrentItemChanged);
}

void ZeusActionTab::setupActionTree(void) {
  QTreeWidgetItem *toolTopLevelItem = new QTreeWidgetItem;

  toolTopLevelItem->setText(0, "Tools");
  toolTopLevelItem->setData(0, ITEM_GROUP_ROLE, ToolTopLevel);

  {
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, "Show Properties");
    item->setData(0, ITEM_GROUP_ROLE, ToolAction);
    item->setData(0, ITEM_ACTION_ID, 0);
    toolTopLevelItem->addChild(item);
  }

  QTreeWidgetItem *predefinedItem = new QTreeWidgetItem;

  predefinedItem->setText(0, "Predefined Actions");
  predefinedItem->setData(0, ITEM_GROUP_ROLE, PredefinedTopLevel);

#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  {                                                                            \
    QTreeWidgetItem *item = new QTreeWidgetItem;                               \
    item->setText(0, desc);                                                    \
    item->setData(0, ITEM_GROUP_ROLE, PredefinedAction);                       \
    item->setData(0, ITEM_ACTION_ID, ZA##TitleName);                           \
    predefinedItem->addChild(item);                                            \
  }
#include "actions/actiongen.h"
#undef ZEUS_ACTION

  m_userCommandItem = new QTreeWidgetItem;
  m_userCommandItem->setText(0, "User Commands");
  m_userCommandItem->setData(0, ITEM_GROUP_ROLE, UserTopLevel);

  m_actionTree->addTopLevelItem(toolTopLevelItem);
  m_actionTree->addTopLevelItem(predefinedItem);
  m_actionTree->addTopLevelItem(m_userCommandItem);
  toolTopLevelItem->setExpanded(true);
  predefinedItem->setExpanded(true);
  m_userCommandItem->setExpanded(true);
}

void ZeusActionTab::createButtonPage(QButtonGroup *group, int startId,
                                     QStringList textList) {
  QWidget *w = new QWidget;
  QHBoxLayout *layout = new QHBoxLayout;

  for (int i = 0; i < textList.size(); i++) {
    QString text = textList[i];
    QPushButton *button = new QPushButton(text);
    group->addButton(button);
    group->setId(button, startId + i);
    layout->addWidget(button);
  }

  w->setLayout(layout);
  m_buttonGroupStack->addWidget(w);
}

void ZeusActionTab::setupButtonGroupStack(void) {
  // This maps 1-to-1 with ItemGroup entries.

  // "Tools" selected. Don't show anything.
  QButtonGroup *group = new QButtonGroup;
  m_buttonGroupStack->addWidget(new QWidget);

  // Specific tools can be shown.
  createButtonPage(group, ButtonId::BShow, QStringList() << "Show");

  // "Predefined Actions" selected. Don't show anything.
  m_buttonGroupStack->addWidget(new QWidget);

  // Specific predefined actions can be run.
  createButtonPage(group, ButtonId::BSelect, QStringList() << "Select");

  // "User Commands" selected. Offer to create and save commands.
  createButtonPage(group, ButtonId::BNewCommand,
                   QStringList() << "New Command" << "Save Commands");

  // A specific user-defined command has been selected.
  createButtonPage(group, ButtonId::BNewAction,
                   QStringList()
                       << "New Action" << "Execute" << "Delete Command");

  // A specific action has been selected.
  createButtonPage(group, ButtonId::BDeleteAction,
                   QStringList() << "Delete Action");

  // The action of a value has been selected.
  m_buttonGroupStack->addWidget(new QWidget);

  connect(group, &QButtonGroup::idClicked, this,
          &ZeusActionTab::onButtonIdClicked);

  m_buttonGroupStack->setCurrentIndex(0);
}

void ZeusActionTab::addUserCommand(ZeusUserCommand *c) {
  QTreeWidgetItem *newItem = new QTreeWidgetItem;
  QString name = c->name();
  auto iter = c->actionIterator();
  newItem->setText(0, name);
  newItem->setData(0, ITEM_GROUP_ROLE, UserCommand);
  addTreesForActions(newItem, iter);

  m_userCommandItem->addChild(newItem);
}

void ZeusActionTab::saveCommands(void) { m_cm->saveCommands(m_commands); }

void ZeusActionTab::takeCommands(QHash<QString, ZeusUserCommand *> commands) {
  QStringList keys = commands.keys();

  std::sort(keys.begin(), keys.end());

  foreach (QString k, keys)
    addUserCommand(commands[k]);

  m_commands = commands;
}

void ZeusActionTab::onButtonIdClicked(int id) {
  switch ((ButtonId)id) {
  case ButtonId::BShow:
    onItemDoubleClicked(m_actionTree->currentItem());
    break;
  case ButtonId::BSelect:
    onItemDoubleClicked(m_actionTree->currentItem());
    break;
  case ButtonId::BNewCommand: {
    bool ok;
    QString text = QInputDialog::getText(
        this, tr("Zeus"), tr("New command name:"), QLineEdit::Normal, "", &ok);

    if (ok == false || m_commands.contains(text))
      return;

    ZeusUserCommand *c = new ZeusUserCommand(text);

    m_commands[text] = c;
    addUserCommand(c);
    break;
  }
  case ButtonId::BSaveCommands:
    m_cm->saveCommands(m_commands);
    break;
  case ButtonId::BNewAction: {
    QStringList actionList = QStringList()
#define ZEUS_ACTION(lowername, TitleName, desc) << desc
#include "actions/actiongen.h"
#undef ZEUS_ACTION
        ;
    QString name = QInputDialog::getItem(this, tr("Zeus"), tr("Select Action"),
                                         actionList, 0, false);

    if (name.isEmpty())
      return;

    for (int i = 0; i < actionList.size(); i++) {
      if (actionList.at(i) != name)
        continue;

      showActionDialog((ZeusActionType)(i + 1));
    }

    break;
  }
  case ButtonId::BExecuteCommand: {
    QTreeWidgetItem *commandItem = m_actionTree->currentItem();
    ZeusUserCommand *c = m_commands.value(commandItem->text(0));

    sendCommandResults(qMakePair(c->name(), m_ce->execCommand(c)));
    break;
  }
  case ButtonId::BDeleteCommand: {
    QTreeWidgetItem *commandItem = m_actionTree->currentItem();
    int commandIndex = m_userCommandItem->indexOfChild(commandItem);
    ZeusUserCommand *c = m_commands.value(commandItem->text(0));

    m_commands.remove(commandItem->text(0));
    delete m_userCommandItem->takeChild(commandIndex);
    delete c;
    break;
  }
  case ButtonId::BDeleteAction: {
    QTreeWidgetItem *actionItem = m_actionTree->currentItem();
    QTreeWidgetItem *commandItem = actionItem->parent();
    ZeusUserCommand *c = m_commands.value(commandItem->text(0));
    int actionIndex = commandItem->indexOfChild(actionItem);

    c->deleteActionAt(actionIndex);
    delete commandItem->takeChild(actionIndex);
    break;
  }
  }
}

void ZeusActionTab::onActionAccepted(void) {
  ZeusBaseAction *action = m_activeDialog->makeAction();
  QTreeWidgetItem *currentItem = m_actionTree->currentItem();
  int group = currentItem->data(0, ITEM_GROUP_ROLE).toInt();

  if (group == PredefinedAction) {
    sendActionResult(m_ce->execAction(action));
    delete action;
  } else if (group == UserCommand) {
    QTreeWidgetItem *commandItem = currentItem;
    auto name = currentItem->text(0);

    m_commands[name]->append(action);
    addOneTreeForAction(currentItem, action);
    currentItem->setExpanded(true);
  }

  delete m_activeDialog;
}

void ZeusActionTab::showActionDialog(ZeusActionType actionType) {
  ZeusBaseDialog *d = ::maybeDialogForType(m_pd, actionType);

  if (d == nullptr)
    return;

  connect(d, &ZeusBaseDialog::actionAccepted, this,
          &ZeusActionTab::onActionAccepted);

  m_activeDialog = d;
  m_activeDialog->exec();
}

void ZeusActionTab::showToolWindow(int id) {
  if (m_propertyWindow == nullptr)
    m_propertyWindow = new ZeusPropertyWindow(m_pd);

  m_propertyWindow->show();
}

void ZeusActionTab::onItemDoubleClicked(QTreeWidgetItem *item) {
  int group = item->data(0, ITEM_GROUP_ROLE).toInt();

  if (group == PredefinedAction) {
    int actionId = item->data(0, ITEM_ACTION_ID).toInt();
    ZeusActionType actionType = static_cast<ZeusActionType>(actionId);

    showActionDialog(actionType);
  } else if (group == ToolAction) {
    int toolId = item->data(0, ITEM_ACTION_ID).toInt();

    showToolWindow(toolId);
  }
}

void ZeusActionTab::onCurrentItemChanged(QTreeWidgetItem *current,
                                         QTreeWidgetItem *) {
  if (current == nullptr)
    return;

  int group = current->data(0, ITEM_GROUP_ROLE).toInt();

  if (group == m_buttonGroupStack->currentIndex())
    return;

  m_buttonGroupStack->setCurrentIndex(group);
}
