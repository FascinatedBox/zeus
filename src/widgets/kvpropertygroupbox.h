#ifndef ZEUSKVPROPERTYGROUPBOX_H
#define ZEUSKVPROPERTYGROUPBOX_H
#include "widgets/basepropertygroupbox.h"

class QLineEdit;

class ZeusKVPropertyEntry : public ZeusBasePropertyEntry {
  Q_OBJECT

public:
  ZeusKVPropertyEntry(void);

  void load(QString key, QString value);
  QPair<QString, QString> toPair(void);
  void reset(void);

private:
  QLineEdit *m_keyLine;
  QLineEdit *m_valueLine;
};

class ZeusKVPropertyGroupBox : public ZeusBasePropertyGroupBox {
  Q_OBJECT

public:
  ZeusKVPropertyGroupBox(const QString &title);

  QList<QPair<QString, QString>> collectProperties(void);
  void loadProperties(QList<QPair<QString, QString>> props);

private slots:
  void onAddEntry(QWidget *source);

private:
  void implReset(void) {}
};

#endif
