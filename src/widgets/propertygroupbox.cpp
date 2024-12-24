#include "widgets/propertygroupbox.h"
#include "widgets/propertykventry.h"
#include <QVBoxLayout>

ZeusPropertyGroupBox::ZeusPropertyGroupBox(const QString &title,
                                           QWidget *parent)
    : QGroupBox(title, parent) {
  m_layout = new QVBoxLayout;
  onAddEntry(nullptr);
  setLayout(m_layout);
}

void ZeusPropertyGroupBox::onAddEntry(QWidget *source) {
  int index = m_layout->indexOf(source);
  ZeusPropertyKVEntry *entry = new ZeusPropertyKVEntry;

  connect(entry, &ZeusPropertyKVEntry::addClicked, this,
          &ZeusPropertyGroupBox::onAddEntry);
  connect(entry, &ZeusPropertyKVEntry::removeClicked, this,
          &ZeusPropertyGroupBox::onRemoveEntry);
  m_layout->insertWidget(index, entry);
}

void ZeusPropertyGroupBox::onRemoveEntry(QWidget *source) {
  if (m_layout->itemAt(1) == nullptr)
    // Gotta have at least one.
    return;

  m_layout->removeWidget(source);
  delete source;
}

QList<QPair<QString, QString>> ZeusPropertyGroupBox::collectProperties(void) {
  QList<QPair<QString, QString>> result;

  for (int i = 0; i < m_layout->count(); ++i) {
    QWidget *w = m_layout->itemAt(i)->widget();
    auto p = static_cast<ZeusPropertyKVEntry *>(w)->toPair();

    if (p.first.isEmpty())
      continue;

    result.append(p);
  }

  return result;
}
