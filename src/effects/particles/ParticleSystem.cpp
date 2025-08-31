#include <random>

#include "effects/particles/ParticleSystem.hpp"


ParticleSystem::ParticleSystem(size_t count) noexcept: 
    m_pointCloud(count), 
    m_vertices(sf::PrimitiveType::Points, count) 
{

}


void ParticleSystem::setEmitter(const sf::Vector2f& position) noexcept
{
    m_emitter = position;
}


void ParticleSystem::update(const sf::Time elapsed) noexcept
{
    float dt = elapsed.asSeconds();

    for (std::size_t i = 0; i < m_pointCloud.size(); ++i)
    {
        Particle& p = m_pointCloud[i];
        p.lifetime -= elapsed;

        if (p.lifetime <= sf::Time::Zero)
            resetParticle(i);

        m_vertices[i].position += p.velocity * dt;

        float ratio           = p.lifetime.asSeconds() / m_lifetime.asSeconds();
        m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
    }
}


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices, states);
}


void ParticleSystem::resetParticle(size_t index) noexcept
{
    static std::random_device rd;
    static std::mt19937       rng(rd());

    const sf::Angle angle       = sf::degrees(std::uniform_real_distribution(0.f, 360.f)(rng));
    const float     speed       = std::uniform_real_distribution(50.f, 100.f)(rng);
    m_pointCloud[index].velocity = sf::Vector2f(speed, angle);
    m_pointCloud[index].lifetime = sf::milliseconds(std::uniform_int_distribution(1000, 3000)(rng));

    m_vertices[index].position = m_emitter;
}