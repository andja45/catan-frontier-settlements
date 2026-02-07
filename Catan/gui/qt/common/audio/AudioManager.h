//
// Created by andja on 31.1.26..
//

#ifndef CATAN_AUDIOMANAGER_H
#define CATAN_AUDIOMANAGER_H

#include <QObject>

class QSoundEffect;

class AudioManager final : public QObject {
    Q_OBJECT
private:
    explicit AudioManager(QObject* parent = nullptr);
    void loadOnce();

    float m_master = 0.25f;

    QSoundEffect* m_click = nullptr;
    QSoundEffect* m_card = nullptr;
    QSoundEffect* m_build = nullptr;
    QSoundEffect* m_error = nullptr;
    QSoundEffect* m_dice = nullptr;
    QSoundEffect* m_victory = nullptr;
    QSoundEffect* m_defeat = nullptr;

    bool m_loaded = false;
public:
    enum class Sfx {
        Click,
        Card,
        Build,
        Error,
        Dice,
        Victory,
        Defeat
    };

    static AudioManager& instance();

    void setMasterVolume(float v01);
    float masterVolume() const;

    void play(Sfx sfx);

    void playClick() { play(Sfx::Click); }
    void playCard() { play(Sfx::Card); }
    void playBuild() { play(Sfx::Build); }
    void playError() { play(Sfx::Error); }
    void playDiceRll() { play(Sfx::Dice); }
    void playVictory() { play(Sfx::Victory); } // TODO play when victory/gameover overlay is shown
    void playDefeat() { play(Sfx::Defeat); }
};



#endif //CATAN_AUDIOMANAGER_H