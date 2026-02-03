//
// Created by andja on 31.1.26..
//

#include "AudioManager.h"

#include <QSoundEffect>
#include <QUrl>

static QSoundEffect* makeSfx(QObject* parent, const char* qrcPath, float volume01){
    auto* s = new QSoundEffect(parent);

    const QString path = QString::fromUtf8(qrcPath);
    s->setSource(QUrl(QStringLiteral("qrc") + path));

    s->setLoopCount(1);
    s->setVolume(volume01);
    return s;
}

AudioManager& AudioManager::instance(){
    static AudioManager g;
    return g;
}

AudioManager::AudioManager(QObject* parent)
    : QObject(parent) {
}

void AudioManager::loadOnce(){
    if (m_loaded) return;
    m_loaded = true;

    m_click =   makeSfx(this, ":/audio/click.wav", m_master * 0.80f);
    m_card =    makeSfx(this, ":/audio/card.wav",      m_master * 0.85f);
    m_build =   makeSfx(this, ":/audio/build.wav",     m_master * 0.80f);
    m_error =   makeSfx(this, ":/audio/error.wav",     m_master * 0.10f);
    m_dice =    makeSfx(this, ":/audio/dice.wav",      m_master * 1.00f);
    m_victory = makeSfx(this, ":/audio/victory.wav",   m_master * 1.10f);
    m_defeat =  makeSfx(this, ":/audio/defeat.wav",  m_master * 1.00f);
}

void AudioManager::setMasterVolume(float v01){
    m_master = std::clamp(v01, 0.0f, 1.0f);
    if (!m_loaded) return;

    if (m_click)    m_click->setVolume(m_master * 0.80f);
    if (m_card)     m_card->setVolume(m_master * 0.85f);
    if (m_build)    m_build->setVolume(m_master * 0.80f);
    if (m_error)    m_error->setVolume(m_master * 0.10f);
    if (m_dice)     m_dice->setVolume(m_master * 1.00f);
    if (m_victory)  m_victory->setVolume(m_master * 1.10f);
    if (m_defeat)   m_defeat->setVolume(m_master * 1.00f);
}

float AudioManager::masterVolume() const{
    return m_master;
}

void AudioManager::play(Sfx sfx){
    loadOnce();

    QSoundEffect* src = nullptr;
    switch (sfx) {
        case Sfx::Click:    src = m_click;    break;
        case Sfx::Card:     src = m_card;     break;
        case Sfx::Build:    src = m_build;    break;
        case Sfx::Error:    src = m_error;    break;
        case Sfx::Dice:     src = m_dice;     break;
        case Sfx::Victory:  src = m_victory;  break;
        case Sfx::Defeat:   src = m_defeat;   break;
    }

    if (!src) return;

    if (src->isPlaying()) src->stop();
    src->play();
}
