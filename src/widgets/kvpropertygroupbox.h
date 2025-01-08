#ifndef ZEUSKVPROPERTYGROUPBOX_H
#define ZEUSKVPROPERTYGROUPBOX_H
#include "widgets/basepropertygroupbox.h"

class QLineEdit;
class QVBoxLayout;

class ZeusKVPropertyEntry : public ZeusBasePropertyEntry {
  Q_OBJECT

public:
  ZeusKVPropertyEntry(void);
  QPair<QString, QString> toPair(void);

private:
  QLineEdit *m_keyLine;
  QLineEdit *m_valueLine;
};

class ZeusKVPropertyGroupBox : public ZeusBasePropertyGroupBox {
  Q_OBJECT

public:
  ZeusKVPropertyGroupBox(const QString &title);
  QList<QPair<QString, QString>> collectProperties(void);

private slots:
  void onAddEntry(QWidget *source);
};

#endif
