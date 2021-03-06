#include "ModuleRender.h"

#include "Engine.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "Utils.h"
#include <SDL_render.h>
#include "Config.h"

ModuleRender::ModuleRender() : Module()
{
    m_Camera.x = 0; 
    m_Camera.y = SCREEN_HEIGHT;
    m_Camera.w = SCREEN_WIDTH;
    m_Camera.h = SCREEN_HEIGHT;
}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Init()
{
    LOG("Creating Renderer context");
    bool ret = true;
    Uint32 flags = 0;

    if (VSYNC == true)
    {
        flags |= SDL_RENDERER_PRESENTVSYNC;
    }

    if (RENDERER_ACCELERATED == true)
    {
        flags |= SDL_RENDERER_ACCELERATED;
    }

    m_Renderer = SDL_CreateRenderer(Engine::Instance()->m_Window->m_Window, -1, flags);

    if (m_Renderer == nullptr)
    {
        LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        ASSERT(false);
        ret = false;
    }

    return ret;
}

UpdateStatus ModuleRender::PreUpdate()
{
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_Renderer);
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleRender::PostUpdate()
{
    SDL_RenderPresent(m_Renderer);
    return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
    LOG("Destroying renderer");

    if (m_Renderer != nullptr)
    {
        SDL_DestroyRenderer(m_Renderer);
    }

    return true;
}

bool ModuleRender::Blit(size_t textureID, int x, int y, SDL_Rect* section, float scaleX, float scaleY, float speed)
{
    SDL_Texture* texture = Engine::Instance()->m_Textures->GetTexture(textureID);

    if (texture == nullptr)
    {
        LOG("Cannot blit to screen texture %i because it's not loaded", textureID);
        return false;
    }

    bool ret = true;
    SDL_Rect rect;
    rect.x = (int)(m_Camera.x * speed) + x ;
    rect.y = (int)(m_Camera.y * speed) + y ;

    if (section != NULL)
    {
        rect.w = section->w;
        rect.h = section->h;
    }
    else
    {
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    }

    rect.w *= scaleX;
    rect.h *= scaleY;

    if (SDL_RenderCopy(m_Renderer, texture, section, &rect) != 0)
    {
        LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
    bool ret = true;

    SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);

    SDL_Rect rec(rect);
    if (use_camera)
    {
        rec.x = (int)(m_Camera.x + rect.x);
        rec.y = (int)(m_Camera.y + rect.y);
    }

    if (SDL_RenderFillRect(m_Renderer, &rec) != 0)
    {
        LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}