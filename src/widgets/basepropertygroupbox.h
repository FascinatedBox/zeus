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
  virtual void onAddEntry(QWidget *source) = 0;
  void insertEntry(int index, ZeusBasePropertyEntry *entry);

protected:
  int entryCount(void) { return m_entryCount; }

  QVBoxLayout *m_layout;

private slots:
  void onRemoveEntry(QWidget *source);

private:
  int m_entryCount;
};

#endif
