#include "widgets/basepropertygroupbox.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

ZeusBasePropertyEntry::ZeusBasePropertyEntry(void) {}

void ZeusBasePropertyEntry::addControlButtonsToLayout(QHBoxLayout *layout) {
  QPushButton *addButton = new QPushButton("➕");
  QPushButton *removeButton = new QPushButton("❌");
  connect(addButton, &QAbstractButton::clicked,
          [this]() { emit addClicked(this); });
  connect(removeButton, &QAbstractButton::clicked,
          [this]() { emit removeClicked(this); });
  layout->addWidget(addButton);
  layout->addWidget(removeButton);
}

ZeusBasePropertyGroupBox::ZeusBasePropertyGroupBox(const QString &title)
    : QGroupBox(title) {
  QVBoxLayout *areaLayout = new QVBoxLayout;
  QWidget *w = new QWidget(this);
  QScrollArea *area = new QScrollArea(this);
  m_layout = new QVBoxLayout;

  area->setVisible(true);
  area->setWidgetResizable(true);
  area->setWidget(w);
  w->setLayout(m_layout);
  areaLayout->addWidget(area);

  m_entryCount = 0;
  setLayout(areaLayout);
}

void ZeusBasePropertyGroupBox::insertEntry(int index,
                                           ZeusBasePropertyEntry *entry) {
  connect(entry, &ZeusBasePropertyEntry::addClicked, this,
          &ZeusBasePropertyGroupBox::onAddEntry);
  connect(entry, &ZeusBasePropertyEntry::removeClicked, this,
          &ZeusBasePropertyGroupBox::onRemoveEntry);
  m_entryCount++;
  m_layout->insertWidget(index, entry);
}

void ZeusBasePropertyGroupBox::onRemoveEntry(QWidget *source) {
  if (m_entryCount == 1)
    // Gotta have at least one.
    return;

  m_entryCount--;
  m_layout->removeWidget(source);
  delete source;
}

void ZeusBasePropertyGroupBox::reset(void) {
  while (m_entryCount > 1) {
    QWidget *w = m_layout->itemAt(m_entryCount - 1)->widget();

    m_layout->removeWidget(w);
    delete w;
    m_entryCount--;
  }

  QWidget *w = m_layout->itemAt(m_entryCount - 1)->widget();
  auto entry = static_cast<ZeusBasePropertyEntry *>(w);

  entry->reset();
}
