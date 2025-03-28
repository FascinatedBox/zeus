#include "core/commandline.h"
#include "core/commandengine.h"
#include "core/usercommand.h"
#include <QApplication>

void cliCommandExec(QString name, ZeusPulseData *pd, ZeusCommandEngine *ce,
                    ZeusUserCommandManager *cm) {
  QHash<QString, ZeusUserCommand *> commands = cm->loadCommands();
  ZeusUserCommand *c = commands.value(name, nullptr);

  if (c == nullptr) {
    fprintf(stderr,
            "zeus: Unknown command '%s'. See zeus -l for a list of commands.\n",
            qPrintable(name));
    exit(EXIT_FAILURE);
  }

  ZeusCliCommandExecutor *executor = new ZeusCliCommandExecutor(name, pd, ce);

  // Don't exit here: Some commands need to await, and will be continued when
  // PulseData sends the appropriate signal.
  executor->exec(c);
}

void cliError(ZeusCommandLineResult parseResult) {
  QString message = parseResult.text.value_or("");

  message = QString("zeus: %1\n").arg(message);
  fputs(qPrintable(message), stderr);
  exit(EXIT_FAILURE);
}

void cliListCommands(ZeusUserCommandManager *cm) {
  QHash<QString, ZeusUserCommand *> commands = cm->loadCommands();
  QStringList keys = commands.keys();

  std::sort(keys.begin(), keys.end());
  puts("Zeus commands:");

  foreach (QString k, keys)
    puts(qPrintable("    " + k));

  exit(EXIT_SUCCESS);
}

ZeusCommandLineResult parseCommandLine(QCommandLineParser &parser) {
  using Status = ZeusCommandLineResult::Status;
  const QCommandLineOption helpOption = parser.addHelpOption();
  QCommandLineOption commandOpt(QStringList() << "c" << "command",
                                "Execute <command>.", "command");
  QCommandLineOption listCommandsOpt(QStringList() << "l" << "list",
                                     "Show available commands.");

  parser.addOption(commandOpt);
  parser.addOption(listCommandsOpt);

  if (!parser.parse(QCoreApplication::arguments()))
    return {Status::Error, parser.errorText()};

  if (parser.isSet(helpOption))
    return {Status::HelpRequested};

  if (parser.isSet(listCommandsOpt))
    return {Status::ListCommands};

  if (parser.isSet(commandOpt))
    return {Status::CommandExec, parser.value(commandOpt)};

  if (parser.positionalArguments().size())
    return {Status::HelpRequested};

  return {Status::Ok};
}

ZeusCliCommandExecutor::ZeusCliCommandExecutor(QString _name, ZeusPulseData *pd,
                                               ZeusCommandEngine *ce)
    : m_name(_name) {
  m_context = new ZeusCommandContext(pd, ce);

  connect(m_context, &ZeusCommandContext::commandComplete, this,
          &ZeusCliCommandExecutor::onCommandComplete);
}

void ZeusCliCommandExecutor::exec(ZeusUserCommand *c) {
  m_context->startCommand(m_name, c);
}

void ZeusCliCommandExecutor::onCommandComplete(void) {
  QString commandName = m_context->commandName();
  auto results = m_context->results();
  int successCount = 0;

  foreach (auto r, results) {
    if (RESULT_IS_SUCCESS(r))
      successCount++;
    else
      fputs(qPrintable(r.second + "\n"), stderr);
  }

  QString message;

  if (successCount == results.size())
    message = QString("zeus %1: All actions successful.").arg(commandName);
  else
    message = QString("zeus %1: %2 of %3 actions succeeded.")
                  .arg(commandName)
                  .arg(successCount)
                  .arg(results.size());

  puts(qPrintable(message));

  // This needs to be QApplication::exit (and not regular exit) so that the
  // event loop processes everything. Otherwise certain actions (specifically
  // MoveStream) fail when used on the command-line (but not in the gui).
  QApplication::exit(EXIT_SUCCESS);
}
