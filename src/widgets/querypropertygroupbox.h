#ifndef ZEUSQUERYPROPERTYGROUPBOX_H
#define ZEUSQUERYPROPERTYGROUPBOX_H
#include "core/pulsequery.h"
#include "widgets/basepropertygroupbox.h"
#include <tuple>

class QAbstractButton;
class QComboBox;
class QLineEdit;
class ZeusQueryPreviewTree;

class ZeusQueryPropertyEntry : public ZeusBasePropertyEntry {
  Q_OBJECT

public:
  ZeusQueryPropertyEntry(void);

  void load(QString k, ZeusQueryMatchType t, QString v);
  void reset(void);
  std::tuple<QString, ZeusQueryMatchType, QString> toQueryLine(void);

private:
  QLineEdit *m_keyLine;
  QComboBox *m_actionCombo;
  QLineEdit *m_valueLine;
};

class ZeusQueryPropertyGroupBox : public ZeusBasePropertyGroupBox {
  Q_OBJECT

public:
  ZeusQueryPropertyGroupBox(ZeusPulseData *pd, const QString &title,
                            ZeusPulseInfoType type);

  void loadQuery(ZeusPulseQuery *);
  ZeusPulseQuery *intoQuery(void);
  void resetPreviewHighlights(void);

private slots:
  void onAddEntry(QWidget *source);
  void onApply(QAbstractButton *);

private:
  void implReset(void) { resetPreviewHighlights(); }

  ZeusQueryPreviewTree *m_previewTree;
  ZeusPulseData *m_pd;
  ZeusPulseInfoType m_type;
};

#endif
