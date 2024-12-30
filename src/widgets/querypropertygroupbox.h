#ifndef ZEUSQUERYPROPERTYGROUPBOX_H
#define ZEUSQUERYPROPERTYGROUPBOX_H
#include "core/pulsequery.h"
#include "widgets/basepropertygroupbox.h"
#include <tuple>

class QAbstractButton;
class QComboBox;
class QLineEdit;
class ZeusPulseData;
class ZeusQueryPreviewTree;

class ZeusQueryPropertyEntry : public ZeusBasePropertyEntry {
  Q_OBJECT

public:
  ZeusQueryPropertyEntry(void);

  std::tuple<QString, ZeusQueryMatchType, QString> toQueryLine(void);

private:
  QLineEdit *m_keyLine;
  QComboBox *m_actionCombo;
  QLineEdit *m_valueLine;
};

class ZeusQueryPropertyGroupBox : public ZeusBasePropertyGroupBox {
  Q_OBJECT

public:
  ZeusQueryPropertyGroupBox(ZeusPulseData *pd, const QString &title);

  ZeusPulseQuery *intoQuery(void);

protected:
  ZeusQueryPropertyEntry *implNewEntry(void);

private slots:
  void onApply(QAbstractButton *);

private:
  ZeusQueryPreviewTree *m_previewTree;
  ZeusPulseData *m_pd;
};

#endif
