#ifndef ZEUSMOVESTREAMEDITOR_H
#define ZEUSMOVESTREAMEDITOR_H
#include "actions/movestreamact.h"
#include "editors/baseeditor.h"
#include <QWidget>

class QRadioButton;
class QStackedWidget;
class ZeusDeviceComboBox;
class ZeusPulseData;
class ZeusQueryPropertyGroupBox;

class ZeusMoveStreamEditor : public ZeusBaseEditor {
  Q_OBJECT

public:
  ZeusMoveStreamEditor(ZeusPulseData *pd, QWidget *parent = nullptr);

  void loadAction(ZeusBaseAction *act);
  ZeusMoveStreamAct *makeAction(void);
  void reset(void);

private slots:
  void playbackSelected(void);
  void recordSelected(void);

private:
  void loadStackWidget(ZeusPulseData *pd, int index);
  bool isValid(void);

  QRadioButton *m_playback;
  QStackedWidget *m_stack;
  ZeusDeviceComboBox *m_combos[2];
  ZeusQueryPropertyGroupBox *m_groups[2];
};

#endif
