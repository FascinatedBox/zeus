#include "widgets/basepropertygroupbox.h"
#include <QHBoxLayout>
#include <QPushButton>
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
  m_layout = new QVBoxLayout;
  m_entryCount = 0;
  setLayout(m_layout);
}

void ZeusBasePropertyGroupBox::onAddEntry(QWidget *source) {
  int index = m_layout->indexOf(source);
  auto entry = implNewEntry();

  connect(entry, &ZeusBasePropertyEntry::addClicked, this,
          &ZeusBasePropertyGroupBox::onAddEntry);
  connect(entry, &ZeusBasePropertyEntry::removeClicked, this,
          &ZeusBasePropertyGroupBox::onRemoveEntry);
  m_entryCount++;
  m_layout->insertWidget(index + 1, entry);
}

void ZeusBasePropertyGroupBox::onRemoveEntry(QWidget *source) {
  if (m_entryCount == 1)
    // Gotta have at least one.
    return;

  m_entryCount--;
  m_layout->removeWidget(source);
  delete source;
}
