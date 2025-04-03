#include <QRegularExpression>
#include <ctype.h>

#include "utils.h"

static QRegularExpression *keyRx = nullptr;
static QRegularExpression *valueRx = nullptr;

void initRegexes(void) {
  // Any word, or any of '_.-', can't be empty.
  keyRx = new QRegularExpression(R"RX(^[\w|_|\-|\.]{1,100}$)RX");

  // Anything that won't cause a problem in QProcess (", ', \), or nothing.
  valueRx = new QRegularExpression(R"RX(^[^"|\\|']{0,100}$)RX");
}

bool isValidPropertyKV(QString key, QString value) {
  QRegularExpressionMatch m = keyRx->match(key);

  if (m.hasMatch() == false)
    return false;

  return valueRx->match(value).hasMatch();
}

bool isValidPulseDeviceName(QString name) {
  return keyRx->match(name).hasMatch();
}

bool isValidPulsePropertyValue(QString value) {
  return valueRx->match(value).hasMatch();
}
