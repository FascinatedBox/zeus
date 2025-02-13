#include <ctype.h>

#include "utils.h"

bool isValidPulseDeviceValue(QString name);

bool isValidPropertyKV(QString key, QString value) {
  if (isValidPulseDeviceName(key) == false)
    return false;

  if (value.size() == 0)
    return true;

  return isValidPulseDeviceValue(value);
}

bool isValidPulseDeviceName(QString name) {
  if (name.size() == 0 || name.size() > 100)
    return false;

  const char *raw = name.toLocal8Bit().data();
  bool result = true;

  while (*raw) {
    char ch = *raw;

    if (isalnum(ch) == false && ch != '-' && ch != '_' && ch != '.') {
      result = false;
      break;
    }

    raw++;
  }

  return result;
}

bool isValidPulseDeviceValue(QString name) {
  if (name.size() == 0 || name.size() > 100)
    return false;

  const char *raw = name.toLocal8Bit().data();
  bool result = true;

  while (*raw) {
    char ch = *raw;

    // I don't expect untrusted input to flow through here, so only check for
    // what I think would break stuff.
    if (ch == '"' || ch == '\\' || ch == '\'') {
      result = false;
      break;
    }

    raw++;
  }

  return result;
}
