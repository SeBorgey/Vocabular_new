#include "subtitleselectiondialog.h"

SubtitleSelectionDialog::SubtitleSelectionDialog(const QString &videoPath, QList<SubtitleTrack>& subtitleTracks,
                                                 const QStringList& audioTracks, QWidget *parent)
    : QDialog(parent), mVideoPath(videoPath), mSelectedAudioTrackIndex(-1)
{
    mTracks = subtitleTracks;
    mAudioTracks = audioTracks;
    setWindowTitle("Select Subtitles and Audio");
    setupUI();
    loadSubtitleTracks();
    loadAudioTracks();
}

void SubtitleSelectionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *languagesLayout = new QHBoxLayout();
    // Audio tracks
    mAudioGroup = new QGroupBox("Audio Tracks");
    QVBoxLayout *audioLayout = new QVBoxLayout(mAudioGroup);
    mAudioTrackRadio = new QRadioButton("Internal Track");
    mAudioTrackList = new QListWidget();
    mAudioSelectedTrack = new QLineEdit();
    mAudioSelectedTrack->setReadOnly(true);

    audioLayout->addWidget(mAudioTrackRadio);
    audioLayout->addWidget(mAudioTrackList);
    audioLayout->addWidget(new QLabel("Selected:"));
    audioLayout->addWidget(mAudioSelectedTrack);
    // Russian subtitles
    QGroupBox *russianGroup = new QGroupBox("Russian Subtitles");
    QVBoxLayout *russianLayout = new QVBoxLayout(russianGroup);
    mRussianFileRadio = new QRadioButton("External File");
    mRussianTrackRadio = new QRadioButton("Internal Track");
    mRussianFileButton = new QPushButton("Select File");
    mRussianTrackList = new QListWidget();
    mRussianSelectedSub = new QLineEdit();
    mRussianSelectedSub->setReadOnly(true);

    russianLayout->addWidget(mRussianFileRadio);
    russianLayout->addWidget(mRussianFileButton);
    russianLayout->addWidget(mRussianTrackRadio);
    russianLayout->addWidget(mRussianTrackList);
    russianLayout->addWidget(new QLabel("Selected:"));
    russianLayout->addWidget(mRussianSelectedSub);

    // English subtitles
    QGroupBox *englishGroup = new QGroupBox("English Subtitles");
    QVBoxLayout *englishLayout = new QVBoxLayout(englishGroup);
    mEnglishFileRadio = new QRadioButton("External File");
    mEnglishTrackRadio = new QRadioButton("Internal Track");
    mEnglishFileButton = new QPushButton("Select File");
    mEnglishTrackList = new QListWidget();
    mEnglishSelectedSub = new QLineEdit();
    mEnglishSelectedSub->setReadOnly(true);

    englishLayout->addWidget(mEnglishFileRadio);
    englishLayout->addWidget(mEnglishFileButton);
    englishLayout->addWidget(mEnglishTrackRadio);
    englishLayout->addWidget(mEnglishTrackList);
    englishLayout->addWidget(new QLabel("Selected:"));
    englishLayout->addWidget(mEnglishSelectedSub);

    languagesLayout->addWidget(mAudioGroup);
    languagesLayout->addWidget(englishGroup);
    languagesLayout->addWidget(russianGroup);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    mainLayout->addLayout(languagesLayout);
    mainLayout->addLayout(buttonsLayout);

    connect(mAudioTrackList, &QListWidget::currentRowChanged, this, &SubtitleSelectionDialog::onAudioTrackSelect);
    connect(mRussianFileButton, &QPushButton::clicked, this, &SubtitleSelectionDialog::onRussianFileSelect);
    connect(mEnglishFileButton, &QPushButton::clicked, this, &SubtitleSelectionDialog::onEnglishFileSelect);
    connect(mRussianTrackList, &QListWidget::currentRowChanged, this, &SubtitleSelectionDialog::onRussianTrackSelect);
    connect(mEnglishTrackList, &QListWidget::currentRowChanged, this, &SubtitleSelectionDialog::onEnglishTrackSelect);
    connect(okButton, &QPushButton::clicked, this, &SubtitleSelectionDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}
void SubtitleSelectionDialog::loadAudioTracks()
{
    for (const QString& track : mAudioTracks) {
        mAudioTrackList->addItem(track);
    }
}

void SubtitleSelectionDialog::onAudioTrackSelect(int row)
{
    if (row >= 0) {
        mSelectedAudioTrackIndex = row;
        mAudioSelectedTrack->setText(mAudioTrackList->item(row)->text());
        mAudioTrackRadio->setChecked(true);
    }
}

void SubtitleSelectionDialog::onRussianFileSelect()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Russian Subtitle File", "", "Subtitle Files (*.srt)");
    if (!filePath.isEmpty()) {
        mRussianSubtitlePath = filePath;
        mRussianSelectedSub->setText(QFileInfo(filePath).fileName());
        mRussianFileRadio->setChecked(true);
    }
}

void SubtitleSelectionDialog::onEnglishFileSelect()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select English Subtitle File", "", "Subtitle Files (*.srt)");
    if (!filePath.isEmpty()) {
        mEnglishSubtitlePath = filePath;
        mEnglishSelectedSub->setText(QFileInfo(filePath).fileName());
        mEnglishFileRadio->setChecked(true);
    }
}

void SubtitleSelectionDialog::loadSubtitleTracks()
{
    for (const SubtitleTrack& track : mTracks) {
        QString itemText = QString("%1 (%2)").arg(track.name, track.language);
        mRussianTrackList->addItem(itemText);
        mEnglishTrackList->addItem(itemText);
    }
}

void SubtitleSelectionDialog::onRussianTrackSelect(int row)
{
    if (row >= 0 && row < mTracks.size()) {
        mRussianSelectedTrack = mTracks[row];
        mRussianSelectedSub->setText(mRussianTrackList->item(row)->text());
        mRussianTrackRadio->setChecked(true);
    }
}

void SubtitleSelectionDialog::onEnglishTrackSelect(int row)
{
    if (row >= 0 && row < mTracks.size()) {
        mEnglishSelectedTrack = mTracks[row];
        mEnglishSelectedSub->setText(mEnglishTrackList->item(row)->text());
        mEnglishTrackRadio->setChecked(true);
    }
}

void SubtitleSelectionDialog::onAccept()
{
    SubtitleTrack russianTrack, englishTrack;

    if (mRussianFileRadio->isChecked()) {
        russianTrack.name = QFileInfo(mRussianSubtitlePath).fileName();
        russianTrack.language = "rus";
        russianTrack.number = -1;  // Используем -1 для внешних файлов
    } else if (mRussianTrackRadio->isChecked()) {
        russianTrack = mRussianSelectedTrack;
        mRussianSubtitlePath.clear();
    }

    if (mEnglishFileRadio->isChecked()) {
        englishTrack.name = QFileInfo(mEnglishSubtitlePath).fileName();
        englishTrack.language = "eng";
        englishTrack.number = -1;  // Используем -1 для внешних файлов
    } else if (mEnglishTrackRadio->isChecked()) {
        englishTrack = mEnglishSelectedTrack;
        mEnglishSubtitlePath.clear();
    }

    emit subtitlesAndAudioSelected(mRussianSubtitlePath, mEnglishSubtitlePath,
                                   russianTrack, englishTrack, mSelectedAudioTrackIndex);
    accept();
}
