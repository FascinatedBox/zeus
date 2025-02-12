#include "widgets/kvpropertygroupbox.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

ZeusKVPropertyEntry::ZeusKVPropertyEntry(void) {
  QHBoxLayout *layout = new QHBoxLayout;
  m_keyLine = new QLineEdit;
  m_valueLine = new QLineEdit;
  QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum);

  layout->addWidget(new QLabel("Name"));
  layout->addWidget(m_keyLine);
  layout->addItem(spacer);
  layout->addWidget(new QLabel("Value"));
  layout->addWidget(m_valueLine);
  addControlButtonsToLayout(layout);
  setLayout(layout);
}

void ZeusKVPropertyEntry::load(QString key, QString value) {
  m_keyLine->setText(key);
  m_valueLine->setText(value);
}

QPair<QString, QString> ZeusKVPropertyEntry::toPair(void) {
  return qMakePair(m_keyLine->text().simplified(), m_valueLine->text());
}

void ZeusKVPropertyEntry::reset(void) {
  m_keyLine->setText("");
  m_valueLine->setText("");
}

ZeusKVPropertyGroupBox::ZeusKVPropertyGroupBox(const QString &title)
    : ZeusBasePropertyGroupBox(title) {
  onAddEntry(nullptr);

  // A stretch item at the end makes elements align to the top.
  m_layout->addStretch(1);
}

QList<QPair<QString, QString>> ZeusKVPropertyGroupBox::collectProperties(void) {
  QList<QPair<QString, QString>> result;

  for (int i = 0; i < entryCount(); ++i) {
    QWidget *w = m_layout->itemAt(i)->widget();
    auto p = static_cast<ZeusKVPropertyEntry *>(w)->toPair();

    if (p.first.isEmpty())
      continue;

    result.append(p);
  }

  return result;
}

void ZeusKVPropertyGroupBox::loadProperties(
    QList<QPair<QString, QString>> props) {
  reset();

  int i = 0;

  for (auto p : props) {
    auto entry = new ZeusKVPropertyEntry;

    entry->load(p.first, p.second);
    insertEntry(i, entry);
    i++;
  }
}

void ZeusKVPropertyGroupBox::onAddEntry(QWidget *source) {
  int index = m_layout->indexOf(source) + 1;
  auto entry = new ZeusKVPropertyEntry;

  insertEntry(index, entry);
}
