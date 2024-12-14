#include <ctype.h>

#include "utils.h"

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
