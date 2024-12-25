#include <QScrollArea>
#include <QVBoxLayout>

#include "core/pulsedata.h"
#include "tabs/streamtab.h"
#include "views/streamview.h"

ZeusStreamTab::ZeusStreamTab(void) {
  m_streamBox = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QScrollArea *area = new QScrollArea;
  QVBoxLayout *streamLayout = new QVBoxLayout;

  streamLayout->setContentsMargins(0, 0, 0, 0);
  streamLayout->setAlignment(Qt::AlignTop);
  m_streamBox->setLayout(streamLayout);
  area->setWidgetResizable(true);
  area->setWidget(m_streamBox);
  mainLayout->addWidget(area);
  setLayout(mainLayout);
}

void ZeusStreamTab::onClientAdded(uint32_t index, ZeusPulseClientInfo *info) {
  m_clientNames[index] = info->name;
}

void ZeusStreamTab::onClientRemoved(uint32_t index) {
  m_clientNames.remove(index);
}

void ZeusStreamTab::deviceAdded(uint32_t index, QString name, QString desc) {
  for (int i = 0; i < m_views.size(); i++) {
    ZeusStreamView *v = m_views[i];

    v->addDevice(index, name, desc);
  }

  m_devices[index] = qMakePair(name, desc);
}

void ZeusStreamTab::onDeviceRemoved(uint32_t index) {
  if (m_devices.contains(index))
    return;

  m_devices.take(index);

  for (int i = 0; i < m_views.size(); i++) {
    ZeusStreamView *v = m_views[i];

    v->removeDevice(index);
  }
}

void ZeusStreamTab::streamAdded(ZeusStreamView *view, uint32_t deviceIndex) {
  for (auto i = m_devices.begin(), end = m_devices.end(); i != end; ++i)
    view->addDevice(i.key(), i.value().first, i.value().second);

  view->setCurrentDeviceByIndex(deviceIndex);
  m_views.append(view);
  m_streamBox->layout()->addWidget(view);
}

void ZeusStreamTab::onStreamRemoved(uint32_t index) {
  ZeusStreamView *target = nullptr;

  for (int i = 0; i < m_views.size(); i++) {
    ZeusStreamView *v = m_views[i];

    if (v->index() == index) {
      target = m_views.takeAt(i);
      break;
    }
  }

  if (target == nullptr)
    return;

  m_streamBox->layout()->removeWidget(target);
  delete target;
}
