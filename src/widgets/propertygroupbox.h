#ifndef ZEUSPROPERTYGROUPBOX_H
#define ZEUSPROPERTYGROUPBOX_H
#include <QGroupBox>

class QVBoxLayout;

class ZeusPropertyGroupBox : public QGroupBox {
  Q_OBJECT

public:
  ZeusPropertyGroupBox(const QString &title, QWidget *parent = nullptr);
  QList<QPair<QString, QString>> collectProperties(void);

private slots:
  void onAddEntry(QWidget *source);
  void onRemoveEntry(QWidget *source);

private:
  QVBoxLayout *m_layout;
};

#endif
