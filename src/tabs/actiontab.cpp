#include "tabs/actiontab.h"
#include "core/commandengine.h"
#include "core/pulsedata.h"
#include "dialogs/maybedialogfortype.h"
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QVBoxLayout>

ZeusActionTab::ZeusActionTab(ZeusCommandEngine *ce, ZeusPulseData *pd) {
  m_ce = ce;
  m_pd = pd;
  m_activeDialog = nullptr;
  QVBoxLayout *vLayout = new QVBoxLayout;
  QHBoxLayout *hLayout = new QHBoxLayout;
  QSpacerItem *leftItem = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum);
  QSpacerItem *rightItem =
      new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum);
  QSpacerItem *bottomItem =
      new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum);
  m_actionTree = new QTreeWidget;
  m_activeDialog = nullptr;
  m_actionType = ZeusActionType::ZANone;
  m_actionTree->setHeaderLabels(QStringList() << "Available Actions");

  setupActionTree();
  hLayout->addItem(leftItem);
  hLayout->addWidget(m_actionTree);
  hLayout->addItem(rightItem);
  vLayout->addLayout(hLayout);
  vLayout->addItem(bottomItem);
  setLayout(vLayout);

  connect(m_actionTree, &QTreeWidget::itemDoubleClicked, this,
          &ZeusActionTab::onItemDoubleClicked);
}

void ZeusActionTab::setupActionTree(void) {
  QTreeWidgetItem *item = new QTreeWidgetItem();

  item->setText(0, "Create virtual sink");
  m_actionTree->addTopLevelItem(item);

  item = new QTreeWidgetItem();
  item->setText(0, "Create pipeline");
  m_actionTree->addTopLevelItem(item);
}

void ZeusActionTab::onActionAccepted(void) {
  ZeusBaseAction *action = m_activeDialog->makeAction();

  m_ce->execAction(action);
  delete action;
  delete m_activeDialog;
}

void ZeusActionTab::onItemDoubleClicked(QTreeWidgetItem *item) {
  int row = m_actionTree->indexOfTopLevelItem(item);
  // Actions are 1-indexed (ZANone is 0).
  ZeusActionType actionType = static_cast<ZeusActionType>(row + 1);
  ZeusBaseDialog *d = ::maybeDialogForType(actionType);

  if (d == nullptr)
    return;

  d->connectToPulseData(m_pd);

  connect(d, &ZeusBaseDialog::actionAccepted, this,
          &ZeusActionTab::onActionAccepted);

  m_activeDialog = d;
  m_activeDialog->exec();
}
