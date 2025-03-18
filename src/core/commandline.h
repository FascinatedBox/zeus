#ifndef ZEUSCOMMANDLINE_H
#define ZEUSCOMMANDLINE_H
#include <QCommandLineParser>

class ZeusCommandContext;
class ZeusCommandEngine;
class ZeusPulseData;
class ZeusUserCommand;
class ZeusUserCommandManager;

struct ZeusCommandLineResult {
  enum class Status {
    CommandExec,
    Error,
    HelpRequested,
    ListCommands,
    Ok,
  };
  Status statusCode = Status::Ok;
  std::optional<QString> text = std::nullopt;
};

class ZeusCliCommandExecutor : public QObject {
  Q_OBJECT

public:
  ZeusCliCommandExecutor(QString name, ZeusPulseData *pd,
                         ZeusCommandEngine *ce);

  void exec(ZeusUserCommand *command);

private slots:
  void onCommandComplete(void);

private:
  QString m_name;
  ZeusCommandContext *m_context;
};

void cliCommandExec(QString name, ZeusPulseData *pd, ZeusCommandEngine *ce,
                    ZeusUserCommandManager *cm);
void cliError(ZeusCommandLineResult parseResult);
void cliListCommands(ZeusUserCommandManager *cm);
ZeusCommandLineResult parseCommandLine(QCommandLineParser &parser);

#endif
