#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "vector_util.h"

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    Sprite(const sf::Texture& texture) : Sprite(texture, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(texture.getSize())) {}

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint) : 
    mTexture(&texture), 
    mVertexArray(sf::PrimitiveType::Triangles, 6),
    mMinPointTex(texMinPoint), 
    mMaxPointTex(texMaxPoint ), 
    mTransform()

  {

      for (size_t i = 0; i < 6; ++i) {
        mVertexArray[i] = sf::Vertex{{0.0f, 0.0f}, sf::Color::White , DEFAULT_VERTICES[i] * sf::Vector2f(mTexture->getSize())};
      }

    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        recomputeVertexArray();
        states.texture = mTexture;
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    // It may help you to write this, which is an array of 6 vertices:
    //   - the first 3 correspond to the vertices of the top-left triangle of a "default rectangle"
    //     ranging from [0, 1] in both dimensions.
    //   - the next 3 correspond to the vertices of the bottom-right triangle. of the default
    //     rectangle.
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {sf::Vector2f(1.0f, 0.0f),sf::Vector2f(0.0f, 0.0f),sf::Vector2f(0.0f, 1.0f), 
      sf::Vector2f(0.0f, 1.0f), sf::Vector2f(1.0f, 1.0f), sf::Vector2f(1.0f, 0.0f) };  // TODO

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    void recomputeVertexArray() const {

      auto origin = mOrigin;
      auto topLeft = - origin; // location of each vertex relative to origin.
      auto bottomRight = mMaxPointTex - origin;
      auto topRight = sf::Vector2f(bottomRight.x, topLeft.y);
      auto bottomLeft = sf::Vector2f(topLeft.x, bottomRight.y);

      topLeft = mTransform.applyPoint(topLeft);
      bottomRight = mTransform.applyPoint(bottomRight);

      topRight = mTransform.applyPoint(topRight);
      bottomLeft  = mTransform.applyPoint(bottomLeft); // apply transformations to each point relative to the origin 
                                                       // must treat them as points to also capture the scaling in the trasnformation 

      mVertexArray[0].position = topRight;
      mVertexArray[1].position = topLeft;
      mVertexArray[2].position = bottomLeft;
      mVertexArray[3].position = bottomLeft;
      mVertexArray[4].position = bottomRight;
      mVertexArray[5].position = topRight;





        // TODO
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif
