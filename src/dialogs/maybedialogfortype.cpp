#include "dialogs/maybedialogfortype.h"
#include "dialogs/createpipelinedialog.h"
#include "dialogs/createvirtualsinkdialog.h"

#define ZEUS_ACTION_CASE(name)                                                 \
  case ZeusActionType::ZA##name:                                               \
    result = new Zeus##name##Dialog();                                         \
    break;

ZeusBaseDialog *maybeDialogForType(ZeusActionType t) {
  ZeusBaseDialog *result;

  switch (t) {
    ZEUS_ACTION_CASE(CreateVirtualSink)
    ZEUS_ACTION_CASE(CreatePipeline)
  default:
    result = nullptr;
    break;
  }

  return result;
}
