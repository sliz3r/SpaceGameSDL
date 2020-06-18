#include "Scene.h"

#include "Engine.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include <SDL_image.h>
#include "GameConfig.h"
#include "SpawnManager.h"

Scene::Scene(const char* backgroundPath, int cameraSpeed, bool active) 
    : m_BackgroundPath(backgroundPath)
    , m_CameraSpeed(cameraSpeed)
    , Module(active)
{
    if (m_CameraSpeed != 0)
    {
        m_MoveCamera = true;
    }
}

Scene::~Scene()
{}

bool Scene::Init()
{
    m_Background = Engine::Instance()->m_Textures->LoadOrGet(m_BackgroundPath);
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
    return true;
}

UpdateStatus Scene::PreUpdate()
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
            ++it;
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Scene::Update()
{
    m_SpawnManager->Update();
    if (m_MoveCamera)
    {
        MoveCamera();
    }
    //DrawBackground();

    for (Entity* entity : m_Entities)
    {
        entity->Update();
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
    for (Entity* entity : m_Entities)
    {
        entity->CleanUp();
        RELEASE(entity);
    }
    m_Entities.clear();

    m_SpawnManager->CleanUp();
    RELEASE(m_SpawnManager);

    return Engine::Instance()->m_Textures->CleanUp();
}

void Scene::DrawBackground()
{
    if (m_Background)
    {
        Engine::Instance()->m_Renderer->Blit(m_Background, 0, -800, NULL, 1, 1);
    }
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