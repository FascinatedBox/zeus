#include "core/controller.h"
#include "core/commandengine.h"
#include "core/mainwindow.h"
#include "core/pulsedata.h"
#include "core/usercommand.h"
#include "tabs/actiontab.h"
#include "tabs/commandtab.h"
#include "tabs/streamtab.h"
#include <QApplication>
#include <QMessageBox>
#include <pulse/subscribe.h>

#define ZEUS_PA_APP_ID "org.fascinatedbox.zeus"
#define ZEUS_PA_APP_NAME "Zeus Audio Manager"
#define ZEUS_PA_APP_VERSION "0.1"

pa_context *zeusPulseContext = nullptr;
static pa_mainloop_api *api = nullptr;

#define ZEUS_EVENT_CASE(event_type, op_fn, UpperName)                          \
  case PA_SUBSCRIPTION_EVENT_##event_type:                                     \
    if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) ==                               \
        PA_SUBSCRIPTION_EVENT_REMOVE) {                                        \
      zc->m_pd->remove##UpperName(index);                                      \
    } else {                                                                   \
      pa_operation *o = op_fn(c, index, on##UpperName##Info, zc);              \
      if (o != nullptr)                                                        \
        pa_operation_unref(o);                                                 \
    }                                                                          \
    break

#define ZEUS_PA_OPERATION(func, callback)                                      \
  o = func(c, callback, userdata);                                             \
  if (o == nullptr)                                                            \
    return false;

#define ZEUS_INFO_HANDLER(snake_name, UpperName, name_field)                   \
  void ZeusController::on##UpperName##Info(pa_context *,                       \
                                           const pa_##snake_name##_info *info, \
                                           int eol, void *userdata) {          \
    ZeusController *zc = static_cast<ZeusController *>(userdata);              \
                                                                               \
    if (eol > 0)                                                               \
      return;                                                                  \
                                                                               \
    zc->m_pd->add##UpperName(info);                                            \
  }

ZEUS_INFO_HANDLER(client, Client, name)
ZEUS_INFO_HANDLER(sink_input, SinkInput, name)
ZEUS_INFO_HANDLER(sink, Sink, description)
ZEUS_INFO_HANDLER(source_output, SourceOutput, name)
ZEUS_INFO_HANDLER(source, Source, description)

void ZeusController::onServerInfo(pa_context *, const pa_server_info *i,
                                  void *userdata) {
  // This is the last one, so everything is loaded.
  ZeusController *zc = static_cast<ZeusController *>(userdata);

  zc->showMainWindow();

  if (strstr(i->server_name, "PipeWire") == nullptr) {
    QMessageBox::warning(
        zc->m_mw, "Zeus",
        "You don't seem to be running the PipeWire Pulse server.\n"
        "Actions may not work.");
  }
}

void ZeusController::onContextSubscribe(pa_context *c,
                                        pa_subscription_event_type_t t,
                                        uint32_t index, void *userdata) {
  ZeusController *zc = static_cast<ZeusController *>(userdata);

  switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
    ZEUS_EVENT_CASE(CLIENT, pa_context_get_client_info, Client);
    ZEUS_EVENT_CASE(SINK, pa_context_get_sink_info_by_index, Sink);
    ZEUS_EVENT_CASE(SOURCE, pa_context_get_source_info_by_index, Source);
    ZEUS_EVENT_CASE(SINK_INPUT, pa_context_get_sink_input_info, SinkInput);
    ZEUS_EVENT_CASE(SOURCE_OUTPUT, pa_context_get_source_output_info,
                    SourceOutput);

  default:
    break;
  }
}

gboolean ZeusController::subscribeToInfoEvents(pa_context *c, void *userdata) {
  ZeusController *zc = static_cast<ZeusController *>(userdata);

  pa_context_set_subscribe_callback(c, ZeusController::onContextSubscribe, zc);

  pa_operation *o = pa_context_subscribe(
      c,
      (pa_subscription_mask_t)(PA_SUBSCRIPTION_MASK_SINK |
                               PA_SUBSCRIPTION_MASK_SOURCE |
                               PA_SUBSCRIPTION_MASK_SINK_INPUT |
                               PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT |
                               PA_SUBSCRIPTION_MASK_CLIENT |
                               PA_SUBSCRIPTION_MASK_SERVER |
                               PA_SUBSCRIPTION_MASK_CARD),
      nullptr, nullptr);

  if (o == nullptr)
    return false;

  pa_operation_unref(o);
  ZEUS_PA_OPERATION(pa_context_get_client_info_list,
                    ZeusController::onClientInfo);
  ZEUS_PA_OPERATION(pa_context_get_sink_info_list, ZeusController::onSinkInfo);
  ZEUS_PA_OPERATION(pa_context_get_source_info_list,
                    ZeusController::onSourceInfo);
  ZEUS_PA_OPERATION(pa_context_get_sink_input_info_list,
                    ZeusController::onSinkInputInfo);
  ZEUS_PA_OPERATION(pa_context_get_source_output_info_list,
                    ZeusController::onSourceOutputInfo);
  ZEUS_PA_OPERATION(pa_context_get_server_info, ZeusController::onServerInfo);
  return true;
}

#undef ZEUS_PA_OPERATION

void ZeusController::onContextState(pa_context *c, void *userdata) {
  ZeusController *zc = static_cast<ZeusController *>(zc);
  g_assert(c);

  switch (pa_context_get_state(c)) {
  case PA_CONTEXT_UNCONNECTED:
  case PA_CONTEXT_CONNECTING:
  case PA_CONTEXT_AUTHORIZING:
  case PA_CONTEXT_SETTING_NAME:
    break;
  case PA_CONTEXT_READY:
    if (subscribeToInfoEvents(c, userdata) == false) {
      fprintf(stderr,
              "zeus: Unable to subscribe to PulseAudio info. Stopping.");
      qApp->quit();
    }

    break;
  case PA_CONTEXT_FAILED:
    fprintf(stderr, "zeus: PulseAudio context failed. Stopping.");
  case PA_CONTEXT_TERMINATED:
  default:
    qApp->quit();
    return;
  }
}

gboolean ZeusController::connectToPulse(gpointer userdata) {
  ZeusController *zc = static_cast<ZeusController *>(userdata);

  pa_proplist *proplist = pa_proplist_new();
  pa_proplist_sets(proplist, PA_PROP_APPLICATION_NAME,
                   QObject::tr(ZEUS_PA_APP_NAME).toUtf8().constData());
  pa_proplist_sets(proplist, PA_PROP_APPLICATION_ID, ZEUS_PA_APP_ID);
  pa_proplist_sets(proplist, PA_PROP_APPLICATION_ICON_NAME, "audio-card");
  pa_proplist_sets(proplist, PA_PROP_APPLICATION_VERSION, ZEUS_PA_APP_VERSION);
  zeusPulseContext = pa_context_new_with_proplist(api, nullptr, proplist);
  g_assert(zeusPulseContext);
  pa_proplist_free(proplist);
  pa_context_set_state_callback(zeusPulseContext,
                                ZeusController::onContextState, zc);

  if (pa_context_connect(zeusPulseContext, nullptr, PA_CONTEXT_NOFAIL,
                         nullptr) < 0) {
    if (pa_context_errno(zeusPulseContext) == PA_ERR_INVALID) {
      fprintf(stderr, "zeus: Unable to connect to PulseAudio. Stopping.");
      return false;
    }
  }

  return true;
}

void ZeusController::showMainWindow(void) { m_mw->show(); }

ZeusController::ZeusController(void) {
  m_pd = new ZeusPulseData();
  m_ce = new ZeusCommandEngine(m_pd);
  m_cm = new ZeusUserCommandManager(m_ce);

  m_mw = new ZeusMainWindow(m_cm);
  m_mw->createPlaybackTab(m_pd);
  m_mw->createRecordTab(m_pd);
  m_actionTab = m_mw->createActionTab(m_pd, m_ce);

  ZeusCommandTab *commandTab = m_mw->createCommandTab(m_pd, m_ce);
  commandTab->takeCommands(m_cm->loadCommands());
  m_mw->createToolTab(m_pd);

  connect(m_actionTab, &ZeusActionTab::sendActionResult, m_mw,
          &ZeusMainWindow::onActionResult);
  connect(commandTab, &ZeusCommandTab::sendCommandResults, m_mw,
          &ZeusMainWindow::onCommandResults);
}

void ZeusController::start(void) {
  pa_glib_mainloop *m = pa_glib_mainloop_new(g_main_context_default());
  g_assert(m);
  api = pa_glib_mainloop_get_api(m);
  g_assert(api);
  if (connectToPulse(this) == false)
    qApp->quit();
}
