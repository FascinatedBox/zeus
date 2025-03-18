#include "editors/createpipelineeditor.h"
#include "core/utils.h"
#include "widgets/devicecombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

ZeusCreatePipelineEditor::ZeusCreatePipelineEditor(ZeusPulseData *pd,
                                                   QWidget *parent)
    : ZeusBaseEditor(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_sinkCombo = new ZeusDeviceComboBox(pd, ZISink);
  m_sourceCombo = new ZeusDeviceComboBox(pd, ZISource);
  m_pipeDesc = new QLineEdit;
  m_pipeRegex = new QRegularExpression("[\\d|\\w|\\-]+");
  m_skipChange = false;

  formLayout->addRow("Source Device:", m_sourceCombo);
  formLayout->addRow("Playback Device:", m_sinkCombo);
  formLayout->addRow("Description (required):", m_pipeDesc);

  connect(m_sinkCombo, &QComboBox::currentTextChanged, this,
          &ZeusCreatePipelineEditor::onDeviceTextChanged);
  connect(m_sourceCombo, &QComboBox::currentTextChanged, this,
          &ZeusCreatePipelineEditor::onDeviceTextChanged);

  // Fake an update to generate a description.
  onDeviceTextChanged("");
  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
}

bool ZeusCreatePipelineEditor::isValid(void) {
  if (m_sinkCombo->currentIndex() == -1)
    return false;

  if (m_sourceCombo->currentIndex() == -1)
    return false;

  QString pipeText = m_pipeDesc->text();

  if (pipeText.isEmpty())
    // Block this here to make intent clear.
    return false;
  else
    return ::isValidPulsePropertyValue(pipeText);
}

void ZeusCreatePipelineEditor::loadAction(ZeusBaseAction *act) {
  auto a = static_cast<ZeusCreatePipelineAct *>(act);

  m_skipChange = true;
  m_sinkCombo->useItemNameAndDesc(a->sinkName, a->sinkDesc);
  m_sourceCombo->useItemNameAndDesc(a->sourceName, a->sourceDesc);
  m_pipeDesc->setText(a->pipeDesc);
  m_skipChange = false;
}

ZeusCreatePipelineAct *ZeusCreatePipelineEditor::makeAction(void) {
  QString sinkName = m_sinkCombo->currentItemName();
  QString sinkDesc = m_sinkCombo->currentItemDescription();
  QString sourceName = m_sourceCombo->currentItemName();
  QString sourceDesc = m_sourceCombo->currentItemDescription();
  QString pipeDesc = m_pipeDesc->text();

  return ZeusCreatePipelineAct::fromSinkAndSource(
      sinkName, sinkDesc, sourceName, sourceDesc, pipeDesc);
}

void ZeusCreatePipelineEditor::onDeviceTextChanged(QString) {
  if (m_skipChange)
    return;

  if (m_sinkCombo->currentIndex() == -1)
    return;

  if (m_sourceCombo->currentIndex() == -1)
    return;

  // Autogenerate a description for the user based on what's being connected.
  // This may not be what the user wants, so don't bother trying to be perfect
  // (monitors being "Monitor" is fine).
  QString sinkDesc = m_sinkCombo->currentItemDescription();
  QString sourceDesc = m_sourceCombo->currentItemDescription();
  QString sinkText = m_pipeRegex->match(sinkDesc).captured(0);
  QString sourceText = m_pipeRegex->match(sourceDesc).captured(0);
  QString pipeText = QString("%1->%2").arg(sourceText).arg(sinkText);

  m_pipeDesc->setText(pipeText);
}

void ZeusCreatePipelineEditor::reset(void) {
  m_skipChange = true;
  m_sinkCombo->setCurrentIndex(0);
  m_sourceCombo->setCurrentIndex(0);
  m_skipChange = false;
  onDeviceTextChanged("");
}
