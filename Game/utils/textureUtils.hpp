/* only meant to be included in source files */

#include <SFML/Graphics.hpp>

namespace
{

namespace TextureUtils
{

/**
 * Sets the texture to the shape keeping the texture's ratio.
 * For cases when the ratio of the shape's bounding rectangle
 * and the texture's rectangle don't match,
 * and we don't want to stretch the texture.
 * We want instead to crop an appropriate part of the texture.
 * That appropriate part will be with the same ratio
 * as the bounding rectangle of the shape.
 * The function will crop a rectangle with exactly the same size
 * as the shape's rectangle, if possible.
 * But it might not be possible, the shape's rectangle
 * might be bigger than the texture.
 * Then the function will scale down the cropped rectangle,
 * so that it stays within the texture.
 * 
 * @param[in] shape
 *  Pointer to the shape on which we want to set the texture
 * @param[in] tex
 *  Pointer to the texture that we want to set
 * @param[in] cropPosition
 *  Upper-left corner of the cropped rectangle from the texture
 */
void SetTextureKeepRatio(
    sf::Shape* shape,
    sf::Texture const* tex,
    sf::Vector2i const& cropPosition)
{
    shape->setTexture(tex);

    // Initialize crop rectangle with size same as the shape's rectangle
    sf::IntRect cropRect(
        cropPosition,
        sf::Vector2i(
            (int)shape->getLocalBounds().width,
            (int)shape->getLocalBounds().height
        )
    );

    // Scale down crop, if needed
    float scale = 1.f;
    // If the crop width goes beyond the texture
    if (cropRect.width > tex->getSize().x - cropPosition.x)
    {
        // scale down the crop, so that the crop width matches the end of the texture
        scale = (float)(tex->getSize().x - cropPosition.x) / cropRect.width;
    }
    // If after the first scale, the crop height still goes beyond the texture
    if (scale * cropRect.height > tex->getSize().y - cropPosition.y)
    {
        // scale down the crop again, so that the crop height matches the end of the texture
        scale *= (float)(tex->getSize().y - cropPosition.y) / (scale * cropRect.height);
    }

    // Perform the scale on the crop
    cropRect.width = scale * cropRect.width;
    cropRect.height = scale * cropRect.height;

    // Set the crop to the shape's texture
    shape->setTextureRect(cropRect);
}

} // namespace TextureUtils

} // namespace