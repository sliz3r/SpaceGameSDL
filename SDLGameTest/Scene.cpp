#include "Scene.h"

#include "Engine.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include <SDL_image.h>
#include "GameConfig.h"
#include "SpawnManager.h"
#include "Player.h"

Scene::Scene(const char* backgroundPath, int cameraSpeed, bool active) 
    : m_CameraSpeed(cameraSpeed)
    , m_BackgroundPath(backgroundPath)
    , Module(active)
{
    if (m_CameraSpeed != 0)
    {
        m_MoveCamera = true;
    }
}

bool Scene::Start()
{
    m_MoveCamera = true;

    Engine::Instance()->m_Renderer->m_Camera.x = 0;
    Engine::Instance()->m_Renderer->m_Camera.y = SCREEN_HEIGHT;

    if (m_BackgroundPath != nullptr)
    {
        m_BackgroundTextureID = Engine::Instance()->m_Textures->LoadOrGet(m_BackgroundPath);
    }

    if (Engine::Instance()->m_Audio->PlayMusic("Assets/Sounds/MainSound.ogg") == false)
    {
        LOG("Couldn't Play Main Game Music");
        return false;
    }

    m_SpawnManager = new SpawnManager(this);
    if (m_SpawnManager == nullptr)
    {
        ASSERT(false);
        return false;
    }

    m_SpawnManager->Init();
    for (Entity* entity : m_Entities)
    {
        entity->Init();
    }
    m_Player = m_SpawnManager->GetPlayer();
    return true;
}

UpdateStatus Scene::PreUpdate()
{
    if (m_Active)
    {
        for (list<Entity*>::iterator it = m_Entities.begin(); it != m_Entities.end();)
        {
            if ((*it)->m_ToDelete == true)
            {
                (*it)->CleanUp();
                RELEASE(*it);
                it = m_Entities.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Scene::Update()
{
    if (m_Active)
    {
        DrawBackground();
        m_SpawnManager->Update();
        if (m_MoveCamera)
        {
            MoveCamera();
        }

        for (list<Entity*>::reverse_iterator it = m_Entities.rbegin(); it != m_Entities.rend(); ++it)
        {
            if ((*it) != nullptr && (*it)->IsActive())
            {
                (*it)->Update();
            }
        }
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
    Engine::Instance()->m_Audio->StopMusic();

    for (Entity* entity : m_Entities)
    {
        entity->CleanUp();
        RELEASE(entity);
    }
    m_Entities.clear();

    m_SpawnManager->CleanUp();
    RELEASE(m_SpawnManager);
    Engine::Instance()->m_Audio->CleanUpSoundEffects();

    return Engine::Instance()->m_Textures->CleanUp();
}

void Scene::MoveCamera()
{
#ifdef _DEBUG
    if (MOVE_MAIN_CAMERA)
#endif
    {
        Engine::Instance()->m_Renderer->m_Camera.y += m_CameraSpeed;
    }
}

void Scene::DrawBackground()
{
    if (m_BackgroundTextureID != 0)
    {
        Engine::Instance()->m_Renderer->Blit(m_BackgroundTextureID, 0, -(Engine::Instance()->m_Renderer->m_Camera.y), nullptr, 1.0f, 1.0f);
    }
}
