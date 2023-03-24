#include "text.hpp"
#include <iostream>

Text::Text()
{
    if (TTF_Init() == -1)
        std::cout << "Could not initialize SDL2 TTF, error: " << TTF_GetError() << std::endl;
    else
        std::cout << "TTF font ready to go!" << std::endl;

    this->font = TTF_OpenFont("./fonts/RobotoMono-Regular.ttf", 28);

    if (this->font == nullptr)
    {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }
}

void Text::draw(int x, int y, string text, SDL_Renderer *renderer)
{
    SDL_Surface *surfaceText = TTF_RenderText_Solid(font, text.data(), {255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_Rect textRect{x, y, surfaceText->w, surfaceText->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surfaceText);
    SDL_DestroyTexture(texture);
}

void Text::drawCentered(int startX, int startY, int endX, int endY, string text, SDL_Renderer *renderer)
{
    SDL_Surface *surfaceText = TTF_RenderText_Solid(font, text.data(), {255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceText);
    int width = (endX - startX);
    int height = (endY - startY);
    int x = startX + width / 2 - surfaceText->w / 2;
    int y = startY + height / 2 - surfaceText->h / 2;
    SDL_Rect textRect{x, y, surfaceText->w, surfaceText->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surfaceText);
    SDL_DestroyTexture(texture);
}

Text::~Text()
{
    TTF_CloseFont(font);
    TTF_Quit();
}