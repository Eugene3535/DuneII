#include "effects/particles/ParticleSystem.hpp"


ParticleSystem::ParticleSystem(size_t count, std::mt19937& rng) noexcept: 
    m_rng(rng),
    m_pointCloud(count), 
    m_vertices(sf::PrimitiveType::Points, count) 
{

}


void ParticleSystem::setEmitter(const sf::Vector2f& position) noexcept
{
    m_emitter = position;
}


void ParticleSystem::setRespawnArea(const sf::Vector2i& area) noexcept
{
    m_respawnArea = {abs(area.x) , abs(area.y)};
}


void ParticleSystem::update(const sf::Time elapsed) noexcept
{
    float dt = elapsed.asSeconds();
    const float maxLifetime = m_lifetime.asSeconds();

    for (std::size_t i = 0; i < m_pointCloud.size(); ++i)
    {
        Particle& p = m_pointCloud[i];
        p.lifetime -= elapsed;

        if (p.lifetime <= sf::Time::Zero)
            resetParticle(i);

        m_vertices[i].position += p.velocity * dt;

        float ratio           = p.lifetime.asSeconds() / maxLifetime;
        m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
    }
}


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices, states);
}


void ParticleSystem::resetParticle(size_t index) noexcept
{
    const sf::Angle angle       = sf::degrees((std::uniform_real_distribution(0.f, 360.f)(m_rng)) - 90);
    const float     speed       = std::uniform_real_distribution(5.f, 10.f)(m_rng);
    m_pointCloud[index].velocity = sf::Vector2f(speed, angle);
    m_pointCloud[index].lifetime = sf::milliseconds(std::uniform_int_distribution(1000, 3000)(m_rng));

    if(m_respawnArea != sf::Vector2i())
    {
        int w = m_respawnArea.x >> 1;
        int h = m_respawnArea.y >> 1;
        int x = m_emitter.x - w;
        int y = m_emitter.y - h;
        int rndx = std::uniform_int_distribution(x, int(m_emitter.x + w))(m_rng);
        int rndy = std::uniform_int_distribution(y, int(m_emitter.y + h))(m_rng);
        m_vertices[index].position = sf::Vector2f(rndx, rndy);
    }
}