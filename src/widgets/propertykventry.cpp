#include "widgets/propertykventry.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

ZeusPropertyKVEntry::ZeusPropertyKVEntry(void) {
  QHBoxLayout *layout = new QHBoxLayout;
  m_keyLine = new QLineEdit;
  m_valueLine = new QLineEdit;
  QPushButton *addButton = new QPushButton("➕");
  QPushButton *removeButton = new QPushButton("❌");
  QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum);

  layout->addWidget(new QLabel("Name"));
  layout->addWidget(m_keyLine);
  layout->addItem(spacer);
  layout->addWidget(new QLabel("Value"));
  layout->addWidget(m_valueLine);
  layout->addWidget(addButton);
  layout->addWidget(removeButton);

  connect(addButton, &QAbstractButton::clicked,
          [this]() { emit addClicked(this); });
  connect(removeButton, &QAbstractButton::clicked,
          [this]() { emit removeClicked(this); });
  setLayout(layout);
}

QPair<QString, QString> ZeusPropertyKVEntry::toPair(void) {
  return qMakePair(m_keyLine->text().simplified(), m_valueLine->text());
}
