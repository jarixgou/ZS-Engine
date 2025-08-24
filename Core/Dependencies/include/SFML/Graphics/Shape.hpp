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

#ifndef SFML_SHAPE_HPP
#define SFML_SHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Base class for textured shapes with outline
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Shape : public Drawable, public Transformable
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Shape();

    ////////////////////////////////////////////////////////////
    /// \brief Change the source cellTexture of the shape
    ///
    /// The \a cellTexture argument refers to a cellTexture that must
    /// exist as long as the shape uses it. Indeed, the shape
    /// doesn't store its own copy of the cellTexture, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the source cellTexture is destroyed and the shape tries to
    /// use it, the behavior is undefined.
    /// \a cellTexture can be NULL to disable texturing.
    /// If \a resetRect is true, the TextureRect property of
    /// the shape is automatically adjusted to the size of the new
    /// cellTexture. If it is false, the cellTexture rect is left unchanged.
    ///
    /// \param cellTexture   New cellTexture
    /// \param resetRect Should the cellTexture rect be reset to the size of the new cellTexture?
    ///
    /// \see getTexture, setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    void setTexture(const Texture* texture, bool resetRect = false);

    ////////////////////////////////////////////////////////////
    /// \brief Set the sub-rectangle of the cellTexture that the shape will display
    ///
    /// The cellTexture rect is useful when you don't want to display
    /// the whole cellTexture, but rather a part of it.
    /// By default, the cellTexture rect covers the entire cellTexture.
    ///
    /// \param rect Rectangle defining the region of the cellTexture to display
    ///
    /// \see getTextureRect, setTexture
    ///
    ////////////////////////////////////////////////////////////
    void setTextureRect(const IntRect& rect);

    ////////////////////////////////////////////////////////////
    /// \brief Set the fill color of the shape
    ///
    /// This color is modulated (multiplied) with the shape's
    /// cellTexture if any. It can be used to colorize the shape,
    /// or change its global opacity.
    /// You can use sf::Color::Transparent to make the inside of
    /// the shape transparent, and have the outline alone.
    /// By default, the shape's fill color is opaque white.
    ///
    /// \param color New color of the shape
    ///
    /// \see getFillColor, setOutlineColor
    ///
    ////////////////////////////////////////////////////////////
    void setFillColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the outline color of the shape
    ///
    /// By default, the shape's outline color is opaque white.
    ///
    /// \param color New outline color of the shape
    ///
    /// \see getOutlineColor, setFillColor
    ///
    ////////////////////////////////////////////////////////////
    void setOutlineColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the thickness of the shape's outline
    ///
    /// Note that negative values are allowed (so that the outline
    /// expands towards the center of the shape), and using zero
    /// disables the outline.
    /// By default, the outline thickness is 0.
    ///
    /// \param thickness New outline thickness
    ///
    /// \see getOutlineThickness
    ///
    ////////////////////////////////////////////////////////////
    void setOutlineThickness(float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief Get the source cellTexture of the shape
    ///
    /// If the shape has no source cellTexture, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the cellTexture when you retrieve it with this function.
    ///
    /// \return Pointer to the shape's cellTexture
    ///
    /// \see setTexture
    ///
    ////////////////////////////////////////////////////////////
    const Texture* getTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sub-rectangle of the cellTexture displayed by the shape
    ///
    /// \return Texture rectangle of the shape
    ///
    /// \see setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    const IntRect& getTextureRect() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the fill color of the shape
    ///
    /// \return Fill color of the shape
    ///
    /// \see setFillColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& getFillColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline color of the shape
    ///
    /// \return Outline color of the shape
    ///
    /// \see setOutlineColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& getOutlineColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline thickness of the shape
    ///
    /// \return Outline thickness of the shape
    ///
    /// \see setOutlineThickness
    ///
    ////////////////////////////////////////////////////////////
    float getOutlineThickness() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the total number of points of the shape
    ///
    /// \return Number of points of the shape
    ///
    /// \see getPoint
    ///
    ////////////////////////////////////////////////////////////
    virtual std::size_t getPointCount() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get a point of the shape
    ///
    /// The returned point is in local coordinates, that is,
    /// the shape's transforms (position, rotation, scale) are
    /// not taken into account.
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the point to get, in range [0 .. getPointCount() - 1]
    ///
    /// \return index-th point of the shape
    ///
    /// \see getPointCount
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2f getPoint(std::size_t index) const = 0;

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
    /// \brief Get the global (non-minimal) bounding rectangle of the entity
    ///
    /// The returned rectangle is in global coordinates, which means
    /// that it takes into account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// shape in the global 2D world's coordinate system.
    ///
    /// This function does not necessarily return the \a minimal
    /// bounding rectangle. It merely ensures that the returned
    /// rectangle covers all the vertices (but possibly more).
    /// This allows for a fast approximation of the bounds as a
    /// first check; you may want to use more precise checks
    /// on top of that.
    ///
    /// \return Global bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect getGlobalBounds() const;

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Shape();

    ////////////////////////////////////////////////////////////
    /// \brief Recompute the internal geometry of the shape
    ///
    /// This function must be called by the derived class everytime
    /// the shape's points change (i.e. the result of either
    /// getPointCount or getPoint is different).
    ///
    ////////////////////////////////////////////////////////////
    void update();

private:

    ////////////////////////////////////////////////////////////
    /// \brief Draw the shape to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the fill vertices' color
    ///
    ////////////////////////////////////////////////////////////
    void updateFillColors();

    ////////////////////////////////////////////////////////////
    /// \brief Update the fill vertices' cellTexture coordinates
    ///
    ////////////////////////////////////////////////////////////
    void updateTexCoords();

    ////////////////////////////////////////////////////////////
    /// \brief Update the outline vertices' position
    ///
    ////////////////////////////////////////////////////////////
    void updateOutline();

    ////////////////////////////////////////////////////////////
    /// \brief Update the outline vertices' color
    ///
    ////////////////////////////////////////////////////////////
    void updateOutlineColors();

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const Texture* m_texture;          ///< Texture of the shape
    IntRect        m_textureRect;      ///< Rectangle defining the area of the source cellTexture to display
    Color          m_fillColor;        ///< Fill color
    Color          m_outlineColor;     ///< Outline color
    float          m_outlineThickness; ///< Thickness of the shape's outline
    VertexArray    m_vertices;         ///< Vertex array containing the fill geometry
    VertexArray    m_outlineVertices;  ///< Vertex array containing the outline geometry
    FloatRect      m_insideBounds;     ///< Bounding rectangle of the inside (fill)
    FloatRect      m_bounds;           ///< Bounding rectangle of the whole shape (outline + fill)
};

} // namespace sf


#endif // SFML_SHAPE_HPP


////////////////////////////////////////////////////////////
/// \class sf::Shape
/// \ingroup graphics
///
/// sf::Shape is a drawable class that allows to define and
/// display a custom convex shape on a render target.
/// It's only an abstract base, it needs to be specialized for
/// concrete types of shapes (circle, rectangle, convex polygon,
/// star, ...).
///
/// In addition to the attributes provided by the specialized
/// shape classes, a shape always has the following attributes:
/// \li a cellTexture
/// \li a cellTexture rectangle
/// \li a fill color
/// \li an outline color
/// \li an outline thickness
///
/// Each feature is optional, and can be disabled easily:
/// \li the cellTexture can be null
/// \li the fill/outline colors can be sf::Color::Transparent
/// \li the outline thickness can be zero
///
/// You can write your own derived shape class, there are only
/// two virtual functions to override:
/// \li getPointCount must return the number of points of the shape
/// \li getPoint must return the points of the shape
///
/// \see sf::RectangleShape, sf::CircleShape, sf::ConvexShape, sf::Transformable
///
////////////////////////////////////////////////////////////
