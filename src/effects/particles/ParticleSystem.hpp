#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <vector>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>


class ParticleSystem: 
    public sf::Drawable
{
public:
    ParticleSystem(size_t count) noexcept;

    void setEmitter(const sf::Vector2f& position) noexcept;

    void update(const sf::Time elapsed) noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time     lifetime;
    };

    void resetParticle(size_t index) noexcept;

    sf::Shader* m_shader;

    std::vector<Particle> m_pointCloud;
    sf::VertexArray       m_vertices;
    sf::Time              m_lifetime{sf::seconds(3)};
    sf::Vector2f          m_emitter;
};

#endif // !PARTICLE_SYSTEM_HPP