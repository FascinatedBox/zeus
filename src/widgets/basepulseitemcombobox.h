#ifndef ZEUSBASEPULSEITEMCOMBOBOX_H
#define ZEUSBASEPULSEITEMCOMBOBOX_H
#include <QComboBox>

class QWheelEvent;
class ZeusPulseDeviceInfo;

class ZeusBasePulseItemComboBox : public QComboBox {
  Q_OBJECT

public:
  ZeusBasePulseItemComboBox(void);

  // Wrap currentText to make usage more obvious.
  QString currentItemDescription(void) { return currentText(); }
  QString currentItemName(void);
  void changeDeviceTo(uint32_t);
  void useItemNameAndDesc(QString name, QString desc);

protected:
  void addDevice(ZeusPulseDeviceInfo *);
  void addRawItem(uint32_t index, QString name, QString desc);
  void removeDevice(uint32_t);

private:
  int indexUsedByPlaceholder(void);
  void mousePressEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);

  QString m_placeholderName;
};

#endif
