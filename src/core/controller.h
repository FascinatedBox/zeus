#ifndef ZEUSCONTROLLER_H
#define ZEUSCONTROLLER_H
#include <QObject>
#include <pulse/glib-mainloop.h>
#include <pulse/introspect.h>

class ZeusCommandEngine;
class ZeusMainWindow;
class ZeusPulseData;
class ZeusActionTab;
class ZeusUserCommandManager;

class ZeusController : public QObject {
  Q_OBJECT

public:
  ZeusController(void);

  void start(void);

private:
  void setupMainWindow(void);
  void showMainWindow(void);

  static gboolean connectToPulse(gpointer userdata);
  static void onContextState(pa_context *c, void *userdata);
  static void onContextSubscribe(pa_context *c, pa_subscription_event_type_t t,
                                 uint32_t index, void *userdata);
  static gboolean subscribeToInfoEvents(pa_context *c, void *userdata);

  static void onClientInfo(pa_context *, const pa_client_info *i, int eol,
                           void *userdata);
  static void onSinkInputInfo(pa_context *, const pa_sink_input_info *i,
                              int eol, void *userdata);
  static void onSourceOutputInfo(pa_context *, const pa_source_output_info *i,
                                 int eol, void *userdata);
  static void onSinkInfo(pa_context *c, const pa_sink_info *i, int eol,
                         void *userdata);
  static void onSourceInfo(pa_context *, const pa_source_info *i, int eol,
                           void *userdata);
  static void onServerInfo(pa_context *, const pa_server_info *i,
                           void *userdata);

  ZeusMainWindow *m_mw;
  ZeusCommandEngine *m_ce;
  ZeusPulseData *m_pd;
  ZeusUserCommandManager *m_cm;
  ZeusActionTab *m_actionTab;
  QString m_execCommandName;
};

#endif
