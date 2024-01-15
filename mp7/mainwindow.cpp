#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QListWidget>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtCore/QUrl>
#include <QtCore/QTimer>
#include <QtCore/QStandardPaths>
#include <QPainter>


class PlayButton : public QPushButton
{
public:
    PlayButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setText("Play");
        setStyleSheet("QPushButton { background-color: #0078d7; color: white; border: none; padding: 10px 20px; border-radius: 20px;font-size: 16px;}");
    }
};

class PauseButton : public QPushButton
{
public:
    PauseButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setText("Pause");
        setStyleSheet("QPushButton { background-color: #0078d7; color: white; border: none; padding: 10px 20px; border-radius: 20px; font-size: 16px; } QPushButton:hover{ background-color: #005ba1; }");
    }
};

class StopButton : public QPushButton
{
public:
    StopButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setText("Stop");
        setStyleSheet("QPushButton { background-color: #0078d7; color: white; border: none; padding: 10px 20px; border-radius: 20px; font-size: 16px; } QPushButton:hover { background-color: #005ba1; }");
    }
};

class UploadButton : public QPushButton
{
public:
    UploadButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setText("Upload");
        setStyleSheet("QPushButton { background-color: #0078d7; color: white; border: none; padding: 10px 20px; border-radius: 20px; font-size: 16px; } QPushButton:hover { background-color: #005ba1; }");
    }
};

class SkipButton : public QPushButton
{
public:
    SkipButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setText("Skip");
        setStyleSheet("QPushButton { background-color: #0078d7; color: white; border: none; padding: 10px 20px; border-radius: 20px; font-size: 16px; } QPushButton:hover { background-color: #005ba1; }");
    }
};

class BackButton : public QPushButton
{
public:
    BackButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setText("Back");
        setStyleSheet("QPushButton { background-color: #0078d7; color: white; border: none; padding: 10px 20px; border-radius: 20px; font-size: 16px; } QPushButton:hover { background-color: #005ba1; }");
    }
};

class FileNameLabel : public QLabel
{
public:
    FileNameLabel(QWidget *parent = nullptr) : QLabel(parent)
    {
        setText("No media file selected.");
        setAlignment(Qt::AlignCenter);
        setStyleSheet("font-size: 20px; font-weight: bold;");
    }
};

class ProgressBar : public QProgressBar
{
public:
    ProgressBar(QWidget *parent = nullptr) : QProgressBar(parent)
    {
        setMinimum(0);
        setTextVisible(false);
        setStyleSheet("QProgressBar { border: 1px solid #0078d7; border-radius: 10px; background-color: white; height: 30px; } QProgressBar::chunk { background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0078d7, stop:1 #00bcd4); border-radius: 10px; }");
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QProgressBar::paintEvent(event);

        // Calculate elapsed time in minutes and seconds
        int elapsedSeconds = (value() % 60);
        int elapsedMinutes = (value() / 60);

        // Calculate total duration in minutes and seconds
        int durationSeconds = (maximum() % 60);
        int durationMinutes = (maximum() / 60);

        // Format the elapsed time and total duration as strings
        QString elapsedTime = QString("%1:%2").arg(elapsedMinutes, 2, 10, QChar('0')).arg(elapsedSeconds, 2, 10, QChar('0'));
        QString totalDuration = QString("%1:%2").arg(durationMinutes, 2, 10, QChar('0')).arg(durationSeconds, 2, 10, QChar('0'));

        // Get the progress bar geometry
        QRect progressBarRect = QRect(0, 0, width(), height());

        // Draw the elapsed time and total duration text next to the progress bar
        QPainter painter(this);
        painter.setPen(Qt::black);

        // Draw elapsed time on the left
        painter.drawText(progressBarRect, Qt::AlignLeft | Qt::AlignVCenter, elapsedTime);

        // Draw total duration on the right
        painter.drawText(progressBarRect, Qt::AlignRight | Qt::AlignVCenter, totalDuration);
    }
};

class VolumeSlider : public QSlider
{
public:
    VolumeSlider(QWidget *parent = nullptr) : QSlider(Qt::Horizontal, parent)
    {
        setRange(0, 100);
        setValue(50);
        setStyleSheet("QSlider::groove:horizontal { height:8px; background-color: #dcdcdc; } QSlider::handle:horizontal { width: 20px; height: 20px; margin: -6px 0px -6px 0px; background-color: #0078d7; border-radius: 10px; } QSlider::handle:horizontal:hover { background-color: #005ba1; }");
    }
};

class PlaylistWidget : public QListWidget
{
public:
    PlaylistWidget(QWidget *parent = nullptr) : QListWidget(parent)
    {
        setSelectionMode(QAbstractItemView::SingleSelection);
        setStyleSheet("background-color:#f0f0f0; font-size: 12px; border: 1px solid #0078d7;");
    }
};

class PlaylistLabel : public QLabel
{
public:
    PlaylistLabel(QWidget *parent = nullptr) : QLabel(parent)
    {
        setText("Playlist");
        setStyleSheet("font-size: 16px; font-weight: bold;");
    }
};

class VideoWidget : public QVideoWidget
{
public:
    VideoWidget(QWidget *parent = nullptr) : QVideoWidget(parent)
    {
    }
};

class MainWindow : public QWidget
{
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent)
    {

        // Create the media player, playlist, and video widget
        m_player = new QMediaPlayer;
        m_playlist = new QMediaPlaylist(m_player);
        m_videoWidget = new VideoWidget;
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
        m_player->setPlaylist(m_playlist);
        m_player->setVideoOutput(m_videoWidget);


        // Create the UI
        setMinimumSize(800, 600);

        m_playButton = new PlayButton;
        m_pauseButton = new PauseButton;
        m_stopButton = new StopButton;
        m_uploadButton = new UploadButton;
        m_skipButton = new SkipButton;
        m_backButton = new BackButton;
        m_fileNameLabel = new FileNameLabel;

        // Create the progress bar and volume slider
        m_progressBar = new ProgressBar;
        m_volumeSlider = new VolumeSlider;

        // Create the playlist widget and its layout
        m_playlistWidget = new PlaylistWidget;
        m_playlistLabel = new PlaylistLabel;
        QVBoxLayout *playlistLayout = new QVBoxLayout;
        playlistLayout->addWidget(m_playlistLabel);
        playlistLayout->addWidget(m_playlistWidget);
        playlistLayout->setAlignment(Qt::AlignTop);

        // Create the layout for the media player controls
        QHBoxLayout *controlLayout = new QHBoxLayout;
        controlLayout->addWidget(m_playButton);
        controlLayout->addWidget(m_pauseButton);
        controlLayout->addWidget(m_stopButton);
        controlLayout->addWidget(m_uploadButton);
        controlLayout->addWidget(m_skipButton);
        controlLayout->addWidget(m_backButton);
        controlLayout->addWidget(m_fileNameLabel);

        // Create the layout for the progress bar and volume slider
        QHBoxLayout *progressLayout = new QHBoxLayout;
        progressLayout->addWidget(m_progressBar);
        progressLayout->addWidget(m_volumeSlider);

        // Create the main layout and add the control layout, progress layout, and video widget
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(controlLayout);
        mainLayout->addLayout(progressLayout);
        mainLayout->addWidget(m_videoWidget);

        // Add the playlist layout to the main layout
        mainLayout->addLayout(playlistLayout);

        // Set the main layout for the window
        setLayout(mainLayout);

        // Connect the signals and slots for the media player controls
        connect(m_playButton, &QPushButton::clicked, m_player, &QMediaPlayer::play);
        connect(m_pauseButton, &QPushButton::clicked, m_player, &QMediaPlayer::pause);
        connect(m_stopButton, &QPushButton::clicked, m_player, &QMediaPlayer::stop);
        connect(m_uploadButton, &QPushButton::clicked, this, &MainWindow::openFile);
        connect(m_skipButton, &QPushButton::clicked, m_playlist, &QMediaPlaylist::next);
        connect(m_backButton, &QPushButton::clicked, m_playlist, &QMediaPlaylist::previous);
        connect(m_player, &QMediaPlayer::mediaChanged, this, &MainWindow::updateFileName);
        connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
        connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
        connect(m_volumeSlider, &QSlider::valueChanged, m_player, &QMediaPlayer::setVolume);
        connect(m_playlistWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::playItem);


        setWindowTitle("Media Player");


        m_defaultDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    }

private:
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
    VideoWidget *m_videoWidget;
    PlayButton *m_playButton;
    PauseButton *m_pauseButton;
    StopButton *m_stopButton;
    UploadButton *m_uploadButton;
    SkipButton *m_skipButton;
    BackButton *m_backButton;
    FileNameLabel *m_fileNameLabel;
    ProgressBar *m_progressBar;
    VolumeSlider *m_volumeSlider;
    PlaylistWidget *m_playlistWidget;
    PlaylistLabel *m_playlistLabel;
    QString m_defaultDir;
    void openFile()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), m_defaultDir, tr("Media Files (*.mp3 *.wav *.mp4 *.avi)"));
        if (!fileName.isEmpty()) {
            m_playlist->addMedia(QUrl::fromLocalFile(fileName));
            m_playlistWidget->addItem(QFileInfo(fileName).fileName());
            m_fileNameLabel->setText(QFileInfo(fileName).fileName()); // Update the FileNameLabel text
            if (m_player->state() != QMediaPlayer::PlayingState) {
                m_player->play();
            }
        }
    }

    void playItem(QListWidgetItem *item)
    {
        int index = m_playlistWidget->row(item);
        m_playlist->setCurrentIndex(index);
        m_player->play();
    }

    void updateFileName(const QMediaContent &media)
    {
        QString fileName = media.canonicalUrl().fileName();
        if (!fileName.isEmpty()) {
            m_fileNameLabel->setText(fileName);
        }
    }

    void updateDuration(qint64 duration)
    {
        m_progressBar->setMaximum(duration / 1000);
    }

    void updatePosition(qint64 position)
    {
        m_progressBar->setValue(position / 1000);

        // Calculate elapsed time in minutes and seconds
        int seconds = (position / 1000) % 60;
        int minutes = (position / 1000) / 60;

        // Format the elapsed time as a string
        QString elapsedTime = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

        // Set the elapsed time as the text for the progress bar
        m_progressBar->setFormat(elapsedTime);

    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Infinite Media");
    QIcon programLogo("C:/Users/Mostafa/Downloads/Colorful Infinite Logo Design.png");
    app.setWindowIcon(programLogo);
    MainWindow window;
    window.show();
    return app.exec();
}
