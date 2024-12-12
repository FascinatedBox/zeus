#include "dialogs/maybedialogfortype.h"
#include "dialogs/createpipelinedialog.h"
#include "dialogs/createvirtualsinkdialog.h"
#include "dialogs/destroyvirtualsinkdialog.h"

ZeusBaseDialog *maybeDialogForType(ZeusActionType t) {
  ZeusBaseDialog *result = nullptr;

  switch (t) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    result = new Zeus##TitleName##Dialog();                                    \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
    break;
  }

  return result;
}
