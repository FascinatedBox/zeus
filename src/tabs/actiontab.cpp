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

ZeusActionTab::ZeusActionTab(ZeusCommandEngine *ce, ZeusPulseData *pd,
                             ZeusUserCommandManager *cm) {
  m_ce = ce;
  m_pd = pd;
  m_cm = cm;
  m_activeDialog = nullptr;
  m_propertyWindow = nullptr;
  QVBoxLayout *innerVLayout = new QVBoxLayout;
  QVBoxLayout *vLayout = new QVBoxLayout;
  QHBoxLayout *hLayout = new QHBoxLayout;
  QSpacerItem *leftItem = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum);
  QSpacerItem *rightItem =
      new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum);
  QSpacerItem *bottomItem = new QSpacerItem(
      20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
  m_actionTree = new QTreeWidget;
  m_activeDialog = nullptr;
  m_actionType = ZeusActionType::ZANone;
  m_actionTree->setHeaderLabels(QStringList() << "Available Actions");
  m_buttonGroupStack = new QStackedWidget;

  setupActionTree();
  setupButtonGroupStack();
  loadUserCommands();
  innerVLayout->addWidget(m_actionTree);
  innerVLayout->addItem(bottomItem);
  innerVLayout->addWidget(m_buttonGroupStack);
  hLayout->addItem(leftItem);
  hLayout->addLayout(innerVLayout);
  hLayout->addItem(rightItem);
  setLayout(hLayout);

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

void ZeusActionTab::addUserCommand(ZeusUserCommand c) {
  QTreeWidgetItem *newItem = new QTreeWidgetItem;
  QString name = c.first;
  QList<ZeusBaseAction *> actions = c.second;
  newItem->setText(0, name);
  newItem->setData(0, ITEM_GROUP_ROLE, UserCommand);
  addTreesForActions(newItem, actions);

  m_userCommandItem->addChild(newItem);
}

void ZeusActionTab::loadUserCommands(void) {
  auto commands = m_cm->commands();

  for (int i = 0; i < commands.size(); i++) {
    addUserCommand(commands[i]);
  }
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

    if (ok == false)
      return;

    ZeusUserCommand c = qMakePair(text, QList<ZeusBaseAction *>());
    m_cm->addNewCommand(text);
    addUserCommand(c);
    break;
  }
  case ButtonId::BSaveCommands:
    m_cm->saveCommands();
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

    name = name.toLower().replace(" ", "");
    ZeusActionType t = m_cm->actionTypeForName(name);
    showActionDialog(t);
    break;
  }
  case ButtonId::BExecuteCommand: {
    QTreeWidgetItem *commandItem = m_actionTree->currentItem();
    int commandIndex = m_userCommandItem->indexOfChild(commandItem);

    m_cm->execCommandAtIndex(commandIndex);
    break;
  }
  case ButtonId::BDeleteCommand: {
    QTreeWidgetItem *commandItem = m_actionTree->currentItem();
    int commandIndex = m_userCommandItem->indexOfChild(commandItem);

    m_cm->removeCommandAt(commandIndex);
    delete m_userCommandItem->takeChild(commandIndex);
    break;
  }
  case ButtonId::BDeleteAction: {
    QTreeWidgetItem *actionItem = m_actionTree->currentItem();
    QTreeWidgetItem *commandItem = actionItem->parent();
    int commandIndex = m_userCommandItem->indexOfChild(commandItem);
    int actionIndex = commandItem->indexOfChild(actionItem);

    m_cm->removeCommandAction(commandIndex, actionIndex);
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
    int commandIndex = m_userCommandItem->indexOfChild(commandItem);

    m_cm->addCommandAction(commandIndex, action);
    addOneTreeForAction(currentItem, action);
    currentItem->setExpanded(true);
  }

  delete m_activeDialog;
}

void ZeusActionTab::showActionDialog(ZeusActionType actionType) {
  ZeusBaseDialog *d = ::maybeDialogForType(actionType);

  if (d == nullptr)
    return;

  d->connectToPulseData(m_pd);

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
