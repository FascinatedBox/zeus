#include "tabs/tooltab.h"
#include "core/pulsedata.h"
#include "tools/propertywindow.h"
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

ZeusToolTab::ZeusToolTab(ZeusPulseData *pd) {
  m_pd = pd;
  m_propertyWindow = nullptr;

  QHBoxLayout *layout = new QHBoxLayout;
  QVBoxLayout *vLayout = new QVBoxLayout;
  QPushButton *execButton = new QPushButton("Execute");

  m_toolList = new QListWidget;
  m_toolList->addItem("Show Properties");
  m_toolList->addItem("Save Commands");
  m_toolList->setCurrentRow(0);
  vLayout->addWidget(m_toolList);
  vLayout->addWidget(execButton);
  layout->addLayout(vLayout);
  layout->addStretch(1);

  connect(execButton, &QAbstractButton::clicked, this, &ZeusToolTab::onExec);
  setLayout(layout);
}

void ZeusToolTab::closeTools(void) {
  if (m_propertyWindow)
    m_propertyWindow->close();
}

void ZeusToolTab::onExec(void) {
  int row = m_toolList->currentRow();

  if (row == 0) {
    if (m_propertyWindow == nullptr)
      m_propertyWindow = new ZeusPropertyWindow(m_pd);

    m_propertyWindow->show();
  } else if (row == 1)
    emit sendSaveCommands();
}
