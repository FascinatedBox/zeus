#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QVBoxLayout>

#include "core/paction.h"
#include "views/streamview.h"
#include "widgets/elidinglabel.h"

QHBoxLayout *ZeusStreamView::setupTopLine(QString client, QString name) {
  QHBoxLayout *layout = new QHBoxLayout;

  m_clientNameLabel = new QLabel(QString("<b>%1</b>").arg(client));
  layout->addWidget(m_clientNameLabel);

  m_nameLabel = new ElidingLabel(this);
  m_nameLabel->setText(QString(": %1").arg(name));
  layout->addWidget(m_nameLabel);

  QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding,
                                        QSizePolicy::Policy::Minimum);
  layout->addItem(spacer);

  m_deviceCombo = new QComboBox;
  layout->addWidget(m_deviceCombo);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  return layout;
}

ZeusStreamView::ZeusStreamView(QString client, QString name, uint32_t index) {
  m_index = index;
  QHBoxLayout *layout = setupTopLine(client, name);

  setLayout(layout);
}

void ZeusStreamView::addDevice(QString desc, uint32_t index) {
  m_deviceCombo->addItem(desc, index);
}

void ZeusStreamView::removeDevice(uint32_t deviceIndex) {
  for (int i = 0; i < m_deviceCombo->count(); i++) {
    uint32_t comboDeviceIndex = m_deviceCombo->itemData(i).toUInt();

    if (deviceIndex == comboDeviceIndex) {
      m_deviceCombo->removeItem(i);
      break;
    }
  }
}
