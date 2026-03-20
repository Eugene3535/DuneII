#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <vector>
#include <random>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>


class ParticleSystem: 
    public sf::Drawable
{
public:
    ParticleSystem(size_t count, std::mt19937& rng) noexcept;

    void setEmitter(const sf::Vector2f& position) noexcept;
    void setRespawnArea(const sf::Vector2i& area) noexcept;
    void setDirection(float angle) noexcept;
    void setSpraying(float angle) noexcept;
    void setMaxLifetime(const sf::Time lifetime) noexcept;
    void setDistribution(const sf::IntRect& area) noexcept;
    void setFading(bool fading) noexcept;

    void update(const sf::Time elapsed) noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time     lifetime;
    };

    void resetParticle(size_t index) noexcept;

    std::vector<Particle> m_pointCloud;
    sf::VertexArray       m_vertices;

    std::mt19937& m_rng;
    sf::Time m_lifetime{sf::seconds(3)};

    sf::Vector2f m_emitter;
    sf::Vector2i m_respawnArea;

    float m_direction;
    float m_spraying;

    bool m_isFading;
};

#endif // !PARTICLE_SYSTEM_HPP