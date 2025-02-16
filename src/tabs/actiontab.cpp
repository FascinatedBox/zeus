#include "tabs/actiontab.h"
#include "core/commandengine.h"
#include "core/pulsedata.h"
#include "editors/maybeeditorfortype.h"
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTreeWidget>

#define ITEM_ID (Qt::UserRole)

ZeusActionTab::ZeusActionTab(ZeusPulseData *pd, ZeusCommandEngine *ce) {
  m_pd = pd;
  m_ce = ce;
  m_editorStack = new QStackedWidget;
  m_actionTree = new QTreeWidget;
  m_actionType = ZeusActionType::ZANone;
  m_actionTree->setHeaderHidden(true);
  m_actionTree->setIndentation(0);
  setupActions();

  QHBoxLayout *layout = new QHBoxLayout;

  layout->addWidget(m_actionTree);

  // Editors should take extra horizontal space.
  layout->addWidget(m_editorStack, 1);
  setLayout(layout);

  connect(m_actionTree, &QTreeWidget::currentItemChanged, this,
          &ZeusActionTab::onCurrentItemChanged);

  // ...but not so much space that the tree shows horizontal scrollbars.
  m_actionTree->setMinimumWidth(m_actionTree->sizeHint().width());
  m_actionTree->setCurrentItem(m_actionTree->topLevelItem(0));
}

void ZeusActionTab::setupActions(void) {
  m_editorStack->addWidget(new QWidget);

#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  {                                                                            \
    QTreeWidgetItem *item = new QTreeWidgetItem;                               \
    item->setText(0, desc);                                                    \
    item->setData(0, ITEM_ID, ZA##TitleName);                                  \
    m_actionTree->addTopLevelItem(item);                                       \
                                                                               \
    ZeusBaseEditor *e = ::maybeEditorForType(m_pd, ZA##TitleName);             \
    m_editorStack->addWidget(e);                                               \
    connect(e, &ZeusBaseEditor::actionAccepted, this,                          \
            &ZeusActionTab::onActionAccepted);                                 \
    connect(e, &ZeusBaseEditor::actionRejected, this,                          \
            &ZeusActionTab::onActionRejected);                                 \
  }

#include "actions/actiongen.h"
#undef ZEUS_ACTION
}

void ZeusActionTab::onActionAccepted(void) {
  auto editor = static_cast<ZeusBaseEditor *>(m_editorStack->currentWidget());
  ZeusBaseAction *action = editor->makeAction();

  sendActionResult(m_ce->execAction(action));
  delete action;
  editor->reset();
}

void ZeusActionTab::onActionRejected(void) {
  auto editor = static_cast<ZeusBaseEditor *>(m_editorStack->currentWidget());

  editor->reset();
}

void ZeusActionTab::onCurrentItemChanged(QTreeWidgetItem *item,
                                         QTreeWidgetItem *) {
  if (item == nullptr)
    return;

  int id = item->data(0, ITEM_ID).toInt();

  m_editorStack->setCurrentIndex(id);
}
