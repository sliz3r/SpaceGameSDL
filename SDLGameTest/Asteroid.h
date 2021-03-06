#pragma once

#include "Entity.h"

struct Collider;

class Asteroid : public Entity
{
    enum class AsteroidState
    {
        IDLE,
        ACTIVE,
        DYING,
    };

public:

    Asteroid(const iPoint& colliderOffset, const char* explosionTexturePath, int numberOfChunks, int lifePoints, int width, int height, int speed, const char* texturePath, const fPoint& textureScale, const iPoint& initialPosition, Scene* scene, bool active = true)
        : Entity(texturePath, textureScale, initialPosition, scene, active)
        , m_ExplosionTexturePath(explosionTexturePath)
        , m_NumberOfChunks(numberOfChunks)
        , m_CurrentLifePoints(lifePoints)
        , m_Width(width)
        , m_Height(height)
        , m_Speed(speed)
    {
        SetColliderOffset(colliderOffset);
    }

    void Init() override;
    void Update() override;
    void OnCollision(Collider* col1, Collider* col2) override;

private:
    void Move();
    void ReceiveDamage(bool destroy);

private:
    Animation m_DieAnimation;
    int m_CurrentLifePoints = 0;
    int m_NumberOfChunks = 0;
    int m_Width = 0;
    int m_Height = 0;
    int m_Speed = 0;
    iPoint m_Direction{ 0 , 0 };
    iPoint m_TargetPosition{ 0, 0 };
    const char* m_ExplosionTexturePath = nullptr;
    AsteroidState m_CurrentState = AsteroidState::IDLE;

    // Assets IDs
    size_t m_ExplosionTextureID = 0;
    size_t m_ExplosionSoundID = 0;
};