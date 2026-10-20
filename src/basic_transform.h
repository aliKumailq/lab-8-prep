// maybe something here...
#include "vector_util.h"
#include <SFML/System/Vector2.hpp>
#include <tuple>
#include <vector>



struct BasicTransform {
    // methods...
    BasicTransform(const sf::Vector2f& scale, const sf::Vector2f& translation) : scale(scale), translation(translation) {}
    BasicTransform() : scale(sf::Vector2f(1,1)), translation(sf::Vector2f(0,0)) {}

    inline sf::Vector2f applyPoint(const sf::Vector2f& point){
      return sf::Vector2f(scale.x * point.x + translation.x, 
                          scale.y * point.y + translation.y);

    }

    inline sf::Vector2f applyVector(const sf::Vector2f& vec) {
      return sf::Vector2f(scale.x * vec.x, scale.y * vec.y);
    }

    inline bool operator==(const BasicTransform& other) {
      return (scale == other.scale) && (translation == other.translation);
    }
    sf::Vector2f scale;
    sf::Vector2f translation;

};
