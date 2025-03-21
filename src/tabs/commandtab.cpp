#include "tabs/commandtab.h"
#include "core/commandengine.h"
#include "core/pulsedata.h"
#include "core/usercommand.h"
#include "editors/maybeeditorfortype.h"
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

enum ItemGroup {
  UserTopLevel,
  UserCommand,
  UserAction,
  CommandError,
};

#define ITEM_GROUP_ROLE (Qt::UserRole + 1)
#define ITEM_ACTION_ID (Qt::UserRole + 2)

ZeusCommandTab::ZeusCommandTab(ZeusPulseData *pd, ZeusCommandEngine *ce,
                               ZeusUserCommandManager *cm) {
  m_pd = pd;
  m_ce = ce;
  m_cm = cm;
  QVBoxLayout *vLayout = new QVBoxLayout;
  QHBoxLayout *layout = new QHBoxLayout;
  m_actionToBeAdded = false;
  m_editorStack = new QStackedWidget;
  m_buttonStack = new QStackedWidget;
  m_commandTree = new QTreeWidget;
  m_commandTree->setHeaderHidden(true);
  m_commandTree->setRootIsDecorated(false);
  m_buttonStack = new QStackedWidget;
  m_commandContext = new ZeusCommandContext(pd, ce);

  setupEditors();
  setupButtonStack();

  // The command tree gets vertical space priority.
  vLayout->addWidget(m_commandTree, 1);
  vLayout->addWidget(m_buttonStack);
  layout->addLayout(vLayout);

  // However, horizontal space should go to the editors.
  layout->addWidget(m_editorStack, 1);
  setLayout(layout);

  connect(m_commandContext, &ZeusCommandContext::commandComplete, this,
          &ZeusCommandTab::onCommandComplete);
  connect(m_commandTree, &QTreeWidget::currentItemChanged, this,
          &ZeusCommandTab::onCurrentItemChanged);
  m_commandTree->setCurrentItem(m_userCommandItem);
}

ZeusBaseAction *ZeusCommandTab::actionForTreeItem(QTreeWidgetItem *item) {
  QTreeWidgetItem *parent = item->parent();
  int index = parent->indexOfChild(item);
  QString name = parent->text(0);
  ZeusUserCommand *command = m_commands[name];

  return command->actionAt(index);
}

ZeusBaseEditor *ZeusCommandTab::editorForTreeItem(QTreeWidgetItem *item) {
  int id = item->data(0, ITEM_ACTION_ID).toInt();
  QWidget *w = m_editorStack->widget(id);

  return static_cast<ZeusBaseEditor *>(w);
}

QTreeWidgetItem *ZeusCommandTab::createItemForAction(ZeusBaseAction *action) {
  QTreeWidgetItem *result = new QTreeWidgetItem;
  int type = (int)action->actionType();

  result->setText(0, action->treeItemDesc());
  result->setData(0, ITEM_GROUP_ROLE, UserAction);
  result->setData(0, ITEM_ACTION_ID, type);
  return result;
}

void ZeusCommandTab::setupEditors(void) {
  m_editorStack->addWidget(new QWidget);
  QStringList nameList = QStringList();

#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  {                                                                            \
    ZeusBaseEditor *e = ::maybeEditorForType(m_pd, ZA##TitleName);             \
    m_editorStack->addWidget(e);                                               \
    connect(e, &ZeusBaseEditor::actionAccepted, this,                          \
            &ZeusCommandTab::onActionAccepted);                                \
    connect(e, &ZeusBaseEditor::actionRejected, this,                          \
            &ZeusCommandTab::onActionRejected);                                \
                                                                               \
    nameList << #TitleName;                                                    \
  }
#include "actions/actiongen.h"
#undef ZEUS_ACTION

  m_commandErrorTree = new QTreeWidget();
  m_commandErrorTree->setHeaderHidden(true);
  m_editorStack->addWidget(m_commandErrorTree);
  m_userCommandItem = new QTreeWidgetItem;
  m_userCommandItem->setText(0, "User Commands");
  m_userCommandItem->setData(0, ITEM_GROUP_ROLE, UserTopLevel);
  m_commandTree->addTopLevelItem(m_userCommandItem);
  m_userCommandItem->setExpanded(true);
  m_errorItem = new QTreeWidgetItem;
  m_errorItem->setText(0, "Errors");
  m_errorItem->setData(0, ITEM_GROUP_ROLE, CommandError);
  m_commandTree->addTopLevelItem(m_errorItem);
  m_errorItem->setHidden(true);
  m_actionNameList = nameList;
  m_editorStack->setEnabled(false);
}

void ZeusCommandTab::setupButtonStack(void) {

#define ADD_BUTTON(text, fn)                                                   \
  {                                                                            \
    QPushButton *b = new QPushButton(text);                                    \
    layout->addWidget(b);                                                      \
    connect(b, &QAbstractButton::clicked, this, fn);                           \
  }

  // "User commands"
  {
    QWidget *w = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    ADD_BUTTON("New Command", &ZeusCommandTab::onNewCommand);
    ADD_BUTTON("Expand All", &ZeusCommandTab::onExpandAll);
    ADD_BUTTON("Collapse All", &ZeusCommandTab::onCollapseAll);
    w->setLayout(layout);
    m_buttonStack->addWidget(w);
  }

  // A user command
  {
    QWidget *w = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    ADD_BUTTON("Execute", &ZeusCommandTab::onExecuteCommand);
    ADD_BUTTON("New Action", &ZeusCommandTab::onNewAction);
    ADD_BUTTON("Delete", &ZeusCommandTab::onDeleteCommand);
    w->setLayout(layout);
    m_buttonStack->addWidget(w);
  }

  // Command action
  {
    QWidget *w = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    ADD_BUTTON("Up", &ZeusCommandTab::onActionUp);
    ADD_BUTTON("Down", &ZeusCommandTab::onActionDown);
    ADD_BUTTON("Edit", &ZeusCommandTab::onEditAction);
    ADD_BUTTON("Delete", &ZeusCommandTab::onDeleteAction);
    w->setLayout(layout);
    m_buttonStack->addWidget(w);
  }

  // Errors
  {
    QWidget *w = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    ADD_BUTTON("Clear", &ZeusCommandTab::onErrorClear);
    w->setLayout(layout);
    m_buttonStack->addWidget(w);
  }

#undef ADD_BUTTON
}

void ZeusCommandTab::addCommandErrors(QString name, QList<QString> errors) {
  QTreeWidgetItem *item = new QTreeWidgetItem;

  item->setText(0, name);

  foreach (QString e, errors) {
    QTreeWidgetItem *child = new QTreeWidgetItem;

    child->setText(0, e);
    item->addChild(child);
  }

  m_commandErrorTree->addTopLevelItem(item);
  item->setExpanded(true);
  m_errorItem->setHidden(false);
}

QTreeWidgetItem *ZeusCommandTab::addUserCommand(int index, ZeusUserCommand *c) {
  QTreeWidgetItem *commandItem = new QTreeWidgetItem;
  QString name = c->name();
  auto iter = c->actionIterator();

  commandItem->setText(0, name);
  commandItem->setData(0, ITEM_GROUP_ROLE, UserCommand);

  while (iter.hasNext()) {
    ZeusBaseAction *action = iter.next();
    QTreeWidgetItem *item = createItemForAction(action);

    commandItem->addChild(item);
  }

  m_userCommandItem->insertChild(index, commandItem);
  return commandItem;
}

void ZeusCommandTab::saveCommands(void) { m_cm->saveCommands(m_commands); }

void ZeusCommandTab::takeCommands(QHash<QString, ZeusUserCommand *> commands) {
  QStringList keys = commands.keys();

  std::sort(keys.begin(), keys.end());

  for (int i = 0; i < keys.size(); i++)
    addUserCommand(i, commands[keys[i]]);

  m_commands = commands;
}

void ZeusCommandTab::onSendSaveCommands(void) {
  m_cm->saveCommands(m_commands);
  emit sendMessage("Commands have been saved.");
}

void ZeusCommandTab::onActionAccepted(void) {
  QTreeWidgetItem *item = m_commandTree->currentItem();
  ZeusBaseAction *act = m_activeEditor->makeAction();

  if (m_actionToBeAdded) {
    // Focus is on the parent, not the new item.
    ZeusUserCommand *command = m_commands[item->text(0)];
    QTreeWidgetItem *actionItem = createItemForAction(act);

    command->append(act);
    item->addChild(actionItem);

    m_editorStack->setCurrentIndex(0);
    m_actionToBeAdded = false;
  } else {
    QTreeWidgetItem *parent = item->parent();
    int actionIndex = parent->indexOfChild(item);
    ZeusUserCommand *command = m_commands[parent->text(0)];

    item->setText(0, act->treeItemDesc());
    command->replaceActionAt(actionIndex, act);
  }

  m_buttonStack->setEnabled(true);
  m_commandTree->setEnabled(true);
  m_editorStack->setEnabled(false);
}

void ZeusCommandTab::onActionDown(void) {
  QTreeWidgetItem *current = m_commandTree->currentItem();
  QTreeWidgetItem *parent = current->parent();
  int actionIndex = parent->indexOfChild(current);

  if ((actionIndex + 1) == parent->childCount())
    return;

  ZeusUserCommand *command = m_commands[parent->text(0)];
  QTreeWidgetItem *otherItem = parent->takeChild(actionIndex + 1);

  parent->insertChild(actionIndex, otherItem);
  command->moveActionDown(actionIndex);
}

void ZeusCommandTab::onActionRejected(void) {
  if (m_actionToBeAdded) {
    m_actionToBeAdded = false;

    // Parent item has focus. Switch back to a blank editing page.
    m_editorStack->setCurrentIndex(0);
  } else {
    // Reload the active item's action to reflect the rejection.
    ZeusBaseAction *action = actionForTreeItem(m_commandTree->currentItem());

    m_activeEditor->loadAction(action);
  }

  m_buttonStack->setEnabled(true);
  m_commandTree->setEnabled(true);
  m_editorStack->setEnabled(false);
}

void ZeusCommandTab::onActionUp(void) {
  QTreeWidgetItem *current = m_commandTree->currentItem();
  QTreeWidgetItem *parent = current->parent();
  int actionIndex = parent->indexOfChild(current);

  if (actionIndex == 0)
    return;

  ZeusUserCommand *command = m_commands[parent->text(0)];
  QTreeWidgetItem *otherItem = parent->takeChild(actionIndex - 1);

  parent->insertChild(actionIndex, otherItem);
  command->moveActionUp(actionIndex);
}

void ZeusCommandTab::onDeleteAction(void) {
  QTreeWidgetItem *current = m_commandTree->currentItem();
  QTreeWidgetItem *parent = current->parent();
  int actionIndex = parent->indexOfChild(current);
  ZeusUserCommand *command = m_commands[parent->text(0)];
  QTreeWidgetItem *otherItem;

  if (parent->childCount() == 1)
    otherItem = parent;
  else if ((actionIndex + 1) == parent->childCount())
    otherItem = parent->child(actionIndex - 1);
  else
    otherItem = parent->child(actionIndex + 1);

  m_commandTree->setCurrentItem(otherItem);
  command->deleteActionAt(actionIndex);
  delete parent->takeChild(actionIndex);
}

void ZeusCommandTab::onCollapseAll(void) {
  for (int i = 0; i < m_userCommandItem->childCount(); i++) {
    QTreeWidgetItem *child = m_userCommandItem->child(i);

    child->setExpanded(false);
  }
}

void ZeusCommandTab::onCommandComplete(void) {
  QString commandName = m_commandContext->commandName();
  auto results = m_commandContext->results();
  QList<QString> errors = QList<QString>();
  int successCount = 0;

  foreach (auto r, results) {
    if (RESULT_IS_SUCCESS(r))
      successCount++;
    else
      errors.append(r.second);
  }

  if (errors.size())
    addCommandErrors(commandName, errors);

  QString message;

  if (successCount == results.size())
    message = QString("%1: All actions successful.").arg(commandName);
  else
    message = QString("%1: %2 of %3 actions succeeded.")
                  .arg(commandName)
                  .arg(successCount)
                  .arg(results.size());

  emit sendMessage(message);
}

void ZeusCommandTab::onDeleteCommand(void) {
  QTreeWidgetItem *commandItem = m_commandTree->currentItem();
  QString name = commandItem->text(0);
  int actionCount = m_commands[name]->actionCount();

  if (actionCount) {
    auto q = QMessageBox::question(
        this, "Confirm deletion",
        QString("Command '%1' has %2 action(s). Delete anyway?")
            .arg(name)
            .arg(actionCount));

    if (q == QMessageBox::No)
      return;
  }

  m_commandTree->setCurrentItem(m_userCommandItem);
  delete commandItem;
  delete m_commands.take(name);
}

void ZeusCommandTab::onEditAction(void) {
  m_editorStack->setEnabled(true);
  m_commandTree->setEnabled(false);
  m_buttonStack->setEnabled(false);
  m_editorStack->setFocus();
}

void ZeusCommandTab::onErrorClear(void) {
  m_commandErrorTree->clear();
  m_errorItem->setHidden(true);
  m_commandTree->setCurrentItem(m_userCommandItem);
}

void ZeusCommandTab::onExecuteCommand(void) {
  if (m_commandContext->executing())
    // Prevent races by only allowing one at a time.
    return;

  QTreeWidgetItem *current = m_commandTree->currentItem();
  QTreeWidgetItem *parent = current->parent();
  QString name = current->text(0);
  ZeusUserCommand *c = m_commands.value(name);

  m_commandContext->startCommand(name, c);
}

void ZeusCommandTab::onNewAction(void) {
  bool ok;
  QString name = QInputDialog::getItem(this, tr("Zeus"), tr("Select Action"),
                                       m_actionNameList, 0, false, &ok);

  if (ok == false)
    return;

  int i;

  for (i = 0; i < m_actionNameList.size(); i++)
    if (m_actionNameList.at(i) == name)
      break;

  // Offset +1 due to editor[0] being an empty widget.
  QWidget *w = m_editorStack->widget(i + 1);
  ZeusBaseEditor *e = static_cast<ZeusBaseEditor *>(w);

  e->reset();
  m_activeEditor = e;
  m_editorStack->setCurrentWidget(w);
  m_editorStack->setEnabled(true);
  m_commandTree->setEnabled(false);
  m_buttonStack->setEnabled(false);
  m_editorStack->setFocus();
  m_actionToBeAdded = true;
}

void ZeusCommandTab::onExpandAll(void) {
  for (int i = 0; i < m_userCommandItem->childCount(); i++) {
    QTreeWidgetItem *child = m_userCommandItem->child(i);

    child->setExpanded(true);
  }
}

void ZeusCommandTab::onNewCommand(void) {
  bool ok;
  QString text = QInputDialog::getText(
      this, tr("Zeus"), tr("New command name:"), QLineEdit::Normal, "", &ok);

  if (ok == false || text.isEmpty() || m_commands.contains(text))
    return;

  ZeusUserCommand *c = new ZeusUserCommand(text);
  QStringList keys = m_commands.keys();
  int i;

  std::sort(keys.begin(), keys.end());

  for (i = 0; i < keys.size(); i++)
    if (text < keys[i])
      break;

  m_commands[text] = c;

  QTreeWidgetItem *item = addUserCommand(i, c);

  item->setExpanded(true);
  m_commandTree->setCurrentItem(item);
}

void ZeusCommandTab::onCurrentItemChanged(QTreeWidgetItem *current,
                                          QTreeWidgetItem *) {
  if (current == nullptr)
    return;

  int role = current->data(0, ITEM_GROUP_ROLE).toInt();

  m_buttonStack->setCurrentIndex(role);

  if (role == UserAction) {
    ZeusBaseAction *action = actionForTreeItem(current);
    ZeusBaseEditor *editor = editorForTreeItem(current);

    m_activeEditor = editor;
    editor->loadAction(action);
    m_editorStack->setCurrentIndex(action->actionType());
  } else if (role != CommandError)
    m_editorStack->setCurrentIndex(0);
  else
    m_editorStack->setCurrentWidget(m_commandErrorTree);
}
