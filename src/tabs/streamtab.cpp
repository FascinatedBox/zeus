#include <QScrollArea>
#include <QVBoxLayout>

#include "core/pulsedata.h"
#include "tabs/streamtab.h"
#include "views/streamview.h"

ZeusStreamTab::ZeusStreamTab(ZeusPulseData *pd, ZeusPulseInfoType type)
    : m_pd(pd) {
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

  if (type == ZISinkInput)
    ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusStreamTab, sinkInput, SinkInput);
  else
    ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusStreamTab, sourceOutput,
                            SourceOutput);
}

void ZeusStreamTab::removeStream(uint32_t index) {
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

void ZeusStreamTab::onSinkInputAdded(ZeusPulseStreamInfo *info) {
  if (info->client == (uint32_t)-1)
    return; // For now, skip outputs for virtual sinks.

  auto view = new ZeusStreamView(m_pd, info);

  m_views.append(view);
  m_streamBox->layout()->addWidget(view);
}

void ZeusStreamTab::onSinkInputRemoved(uint32_t index) { removeStream(index); }

void ZeusStreamTab::onSourceOutputAdded(ZeusPulseStreamInfo *info) {
  auto view = new ZeusStreamView(m_pd, info);

  m_views.append(view);
  m_streamBox->layout()->addWidget(view);
}

void ZeusStreamTab::onSourceOutputRemoved(uint32_t index) {
  removeStream(index);
}
