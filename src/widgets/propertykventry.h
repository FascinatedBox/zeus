#ifndef ZEUSPROPERTYKVENTRY_H
#define ZEUSPROPERTYKVENTRY_H
#include <QWidget>

class QLineEdit;

class ZeusPropertyKVEntry : public QWidget {
  Q_OBJECT

public:
  ZeusPropertyKVEntry(void);
  QPair<QString, QString> toPair(void);

signals:
  void addClicked(QWidget *);
  void removeClicked(QWidget *);

private:
  QLineEdit *m_keyLine;
  QLineEdit *m_valueLine;
};

#endif
