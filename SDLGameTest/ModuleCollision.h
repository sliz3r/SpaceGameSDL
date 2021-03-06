#pragma once

#include "Module.h"
#include "Entity.h"
#include "SDL_rect.h"

#include<list>

enum ColliderType
{
    COLLIDER_NONE = -1,
    COLLIDER_PLAYER,
    COLLIDER_ENEMY,
    COLLIDER_BULLET_PLAYER,
    COLLIDER_BULLET_ENEMY,

    COLLIDER_MAX
};

struct Collider
{
    SDL_Rect m_Rect = { 0,0,0,0 };
    bool m_Delete = false;
    bool m_Active = true;
    ColliderType m_Type = ColliderType::COLLIDER_NONE;

    Entity* m_CallbackEntity = nullptr;

    Collider(const SDL_Rect& rectangle, ColliderType type, Entity* callback = NULL) :
        m_Rect(rectangle), m_Type(type), m_CallbackEntity(callback)
    {}

    void SetPosition(int x, int y)
    {
        m_Rect.x = x;
        m_Rect.y = y;
    }
    void SetSize(int x, int y)
    {
        m_Rect.w = x;
        m_Rect.h = y;
    }

    void SetType(ColliderType type)
    {
        m_Type = type;
    }

    void ToDelete()
    {
        m_Delete = true;
    }

    void Enable()
    {
        m_Active = true;
    }

    void Disable()
    {
        m_Active = false;
    }

    bool CheckCollision(const SDL_Rect& rect) const;
};

class ModuleCollision : public Module
{
public:

    ModuleCollision();
    ~ModuleCollision();

    UpdateStatus PreUpdate() override;
    UpdateStatus Update() override;

    bool CleanUp() override;

    Collider* AddCollider(const SDL_Rect& rect, ColliderType type, Entity* callback);
#ifdef _DEBUG
    void DebugDraw();
#endif
    std::list<Collider*> m_Colliders;

private:
#ifdef _DEBUG
    bool m_ShowDebug = false;
#endif
    bool m_CollisionMatrix[COLLIDER_MAX][COLLIDER_MAX];
};