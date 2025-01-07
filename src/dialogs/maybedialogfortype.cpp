#include "dialogs/maybedialogfortype.h"
#include "dialogs/createpipelinedialog.h"
#include "dialogs/createvirtualsinkdialog.h"
#include "dialogs/destroyvirtualsinkdialog.h"
#include "dialogs/movestreamdialog.h"

ZeusBaseDialog *maybeDialogForType(ZeusPulseData *pd, ZeusActionType t) {
  ZeusBaseDialog *result = nullptr;

  switch (t) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    result = new Zeus##TitleName##Dialog(pd);                                  \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
    break;
  }

  return result;
}
