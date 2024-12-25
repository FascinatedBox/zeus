#ifndef ZEUSBASEPROPERTYGROUPBOX_H
#define ZEUSBASEPROPERTYGROUPBOX_H
#include <QGroupBox>

class QHBoxLayout;
class QVBoxLayout;

class ZeusBasePropertyEntry : public QWidget {
  Q_OBJECT

public:
  ZeusBasePropertyEntry(void);

signals:
  void addClicked(QWidget *);
  void removeClicked(QWidget *);

protected:
  void addControlButtonsToLayout(QHBoxLayout *);
};

class ZeusBasePropertyGroupBox : public QGroupBox {
  Q_OBJECT

public:
  ZeusBasePropertyGroupBox(const QString &title);

protected slots:
  void onAddEntry(QWidget *source);

protected:
  virtual ZeusBasePropertyEntry *implNewEntry(void) = 0;

  QVBoxLayout *m_layout;

private slots:
  void onRemoveEntry(QWidget *source);
};

#endif
