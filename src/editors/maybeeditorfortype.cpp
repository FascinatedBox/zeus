#include "editors/maybeeditorfortype.h"
#include "editors/createpipelineeditor.h"
#include "editors/createvirtualsinkeditor.h"
#include "editors/destroyvirtualsinkeditor.h"
#include "editors/movestreameditor.h"

ZeusBaseEditor *maybeEditorForType(ZeusPulseData *pd, ZeusActionType t) {
  ZeusBaseEditor *result = nullptr;

  switch (t) {
#define ZEUS_ACTION(lowername, TitleName, desc)                                \
  case ZeusActionType::ZA##TitleName:                                          \
    result = new Zeus##TitleName##Editor(pd);                                  \
    break;
#include "actions/actiongen.h"
#undef ZEUS_ACTION
  default:
    break;
  }

  return result;
}
