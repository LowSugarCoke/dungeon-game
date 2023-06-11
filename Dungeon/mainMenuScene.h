#pragma once

#include <QGraphicsScene>
#include <QSlider>
#include <QGraphicsPixmapItem>
#include <QGraphicsOpacityEffect>
#include "mediaPlayer.h"

class MainMenuScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MainMenuScene(QObject* parent = nullptr);
    QSlider* getVolumeSlider() const;
    void setSceneImg(const QString& kSceneImg);
    void fadeIn(int duration);
    void fadeOut(int duration);
    void setMedia(MediaPlayer* player);


private:
    QGraphicsPixmapItem* m_backgroundItem;
    QGraphicsOpacityEffect* m_opacityEffect;
    MediaPlayer* mediaPlayer;  // �s�W�����ּ���

private slots:
    void handleStartButton();
    void handleSettingsButton();
    void handleExitButton();



};