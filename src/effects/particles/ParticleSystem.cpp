#include "effects/particles/ParticleSystem.hpp"


ParticleSystem::ParticleSystem(size_t count, std::mt19937& rng) noexcept: 
    m_pointCloud(count), 
    m_vertices(sf::PrimitiveType::Points, count),
    m_rng(rng),
    m_direction(0),
    m_spraying(0),
    m_isFading(false)
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


void ParticleSystem::setDirection(float angle) noexcept
{
    m_direction = std::fmod(angle, 360.f);

    if (m_direction < 0)
        m_direction += 360.f;
}


void ParticleSystem::setSpraying(float angle) noexcept
{
    m_spraying = std::fmod(angle, 360.f);

    if (m_spraying < 0)
        m_spraying += 360.f;
}


void ParticleSystem::setMaxLifetime(const sf::Time lifetime) noexcept
{
    m_lifetime = lifetime;
}


void ParticleSystem::setDistribution(const sf::IntRect& area) noexcept
{
    for (std::size_t i = 0; i < m_pointCloud.size(); ++i)
    {
        resetParticle(i);
        int rndx = std::uniform_int_distribution(area.position.x, area.size.x + area.size.x / 2)(m_rng);
        int rndy = std::uniform_int_distribution(area.position.y, area.size.y + area.size.y / 2)(m_rng);
        m_vertices[i].position = sf::Vector2f(rndx, rndy);
    }
}


void ParticleSystem::setFading(bool fading) noexcept
{
    m_isFading = fading;
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

        if(m_isFading)
        {
            float ratio           = p.lifetime.asSeconds() / maxLifetime;
            m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
        }
    }
}


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices, states);
}


void ParticleSystem::resetParticle(size_t index) noexcept
{
    float spraying = m_spraying > 0.f ? m_spraying * 0.5f : 0.f;

    const sf::Angle angle = sf::degrees(std::uniform_real_distribution(m_direction - spraying, m_direction + spraying)(m_rng));
    const float     speed = std::uniform_real_distribution(50.f, 100.f)(m_rng);

    m_pointCloud[index].velocity = sf::Vector2f(speed, angle);
    m_pointCloud[index].lifetime = sf::milliseconds(std::uniform_int_distribution(m_lifetime.asMilliseconds() / 10, m_lifetime.asMilliseconds())(m_rng));

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
    else
    {
        m_vertices[index].position = m_emitter;
    }

    m_vertices[index].color.a = 255;
}