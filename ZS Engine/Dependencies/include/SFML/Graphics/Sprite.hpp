////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_SPRITE_HPP
#define SFML_SPRITE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace sf
{
class Texture;

////////////////////////////////////////////////////////////
/// \brief Drawable representation of a cellTexture, with its
///        own transformations, color, etc.
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Sprite : public Drawable, public Transformable
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty spr with no source cellTexture.
    ///
    ////////////////////////////////////////////////////////////
    Sprite();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the spr from a source cellTexture
    ///
    /// \param cellTexture Source cellTexture
    ///
    /// \see setTexture
    ///
    ////////////////////////////////////////////////////////////
    explicit Sprite(const Texture& texture);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the spr from a sub-rectangle of a source cellTexture
    ///
    /// \param cellTexture   Source cellTexture
    /// \param rectangle Sub-rectangle of the cellTexture to assign to the spr
    ///
    /// \see setTexture, setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    Sprite(const Texture& texture, const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Change the source cellTexture of the spr
    ///
    /// The \a cellTexture argument refers to a cellTexture that must
    /// exist as long as the spr uses it. Indeed, the spr
    /// doesn't store its own copy of the cellTexture, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the source cellTexture is destroyed and the spr tries to
    /// use it, the behavior is undefined.
    /// If \a resetRect is true, the TextureRect property of
    /// the spr is automatically adjusted to the size of the new
    /// cellTexture. If it is false, the cellTexture rect is left unchanged.
    ///
    /// \param cellTexture   New cellTexture
    /// \param resetRect Should the cellTexture rect be reset to the size of the new cellTexture?
    ///
    /// \see getTexture, setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    void setTexture(const Texture& texture, bool resetRect = false);

    ////////////////////////////////////////////////////////////
    /// \brief Set the sub-rectangle of the cellTexture that the spr will display
    ///
    /// The cellTexture rect is useful when you don't want to display
    /// the whole cellTexture, but rather a part of it.
    /// By default, the cellTexture rect covers the entire cellTexture.
    ///
    /// \param rectangle Rectangle defining the region of the cellTexture to display
    ///
    /// \see getTextureRect, setTexture
    ///
    ////////////////////////////////////////////////////////////
    void setTextureRect(const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Set the global color of the spr
    ///
    /// This color is modulated (multiplied) with the spr's
    /// cellTexture. It can be used to colorize the spr, or change
    /// its global opacity.
    /// By default, the spr's color is opaque white.
    ///
    /// \param color New color of the spr
    ///
    /// \see getColor
    ///
    ////////////////////////////////////////////////////////////
    void setColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Get the source cellTexture of the spr
    ///
    /// If the spr has no source cellTexture, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the cellTexture when you retrieve it with this function.
    ///
    /// \return Pointer to the spr's cellTexture
    ///
    /// \see setTexture
    ///
    ////////////////////////////////////////////////////////////
    const Texture* getTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sub-rectangle of the cellTexture displayed by the spr
    ///
    /// \return Texture rectangle of the spr
    ///
    /// \see setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    const IntRect& getTextureRect() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global color of the spr
    ///
    /// \return Global color of the spr
    ///
    /// \see setColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& getColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the local bounding rectangle of the entity
    ///
    /// The returned rectangle is in local coordinates, which means
    /// that it ignores the transformations (translation, rotation,
    /// scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// entity in the entity's coordinate system.
    ///
    /// \return Local bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect getLocalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global bounding rectangle of the entity
    ///
    /// The returned rectangle is in global coordinates, which means
    /// that it takes into account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// spr in the global 2D world's coordinate system.
    ///
    /// \return Global bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect getGlobalBounds() const;

private:

    ////////////////////////////////////////////////////////////
    /// \brief Draw the spr to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the vertices' positions
    ///
    ////////////////////////////////////////////////////////////
    void updatePositions();

    ////////////////////////////////////////////////////////////
    /// \brief Update the vertices' cellTexture coordinates
    ///
    ////////////////////////////////////////////////////////////
    void updateTexCoords();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vertex         m_vertices[4]; ///< Vertices defining the spr's geometry
    const Texture* m_texture;     ///< Texture of the spr
    IntRect        m_textureRect; ///< Rectangle defining the area of the source cellTexture to display
};

} // namespace sf


#endif // SFML_SPRITE_HPP


////////////////////////////////////////////////////////////
/// \class sf::Sprite
/// \ingroup graphics
///
/// sf::Sprite is a drawable class that allows to easily display
/// a cellTexture (or a part of it) on a render target.
///
/// It inherits all the functions from sf::Transformable:
/// position, rotation, scale, origin. It also adds spr-specific
/// properties such as the cellTexture to use, the part of it to display,
/// and some convenience functions to change the overall color of the
/// spr, or to get its bounding rectangle.
///
/// sf::Sprite works in combination with the sf::Texture class, which
/// loads and provides the pixel data of a given cellTexture.
///
/// The separation of sf::Sprite and sf::Texture allows more flexibility
/// and better performances: indeed a sf::Texture is a heavy resource,
/// and any operation on it is slow (often too slow for real-time
/// applications). On the other side, a sf::Sprite is a lightweight
/// object which can use the pixel data of a sf::Texture and draw
/// it with its own transformation/color/blending attributes.
///
/// It is important to note that the sf::Sprite instance doesn't
/// copy the cellTexture that it uses, it only keeps a reference to it.
/// Thus, a sf::Texture must not be destroyed while it is
/// used by a sf::Sprite (i.e. never write a function that
/// uses a local sf::Texture instance for creating a spr).
///
/// See also the note on coordinates and undistorted rendering in sf::Transformable.
///
/// Usage example:
/// \code
/// // Declare and load a cellTexture
/// sf::Texture cellTexture;
/// cellTexture.loadFromFile("cellTexture.png");
///
/// // Create a spr
/// sf::Sprite spr;
/// spr.setTexture(cellTexture);
/// spr.setTextureRect(sf::IntRect(10, 10, 50, 30));
/// spr.setColor(sf::Color(255, 255, 255, 200));
/// spr.setPosition(100, 25);
///
/// // Draw it
/// window.draw(spr);
/// \endcode
///
/// \see sf::Texture, sf::Transformable
///
////////////////////////////////////////////////////////////
