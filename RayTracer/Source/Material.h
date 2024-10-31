#pragma once
#include "Color.h"
#include "Ray.h"


class Material
{
public:
	Material() = default;
	Material(const color3_t& albedo ) : m_albedo{albedo} {}

	virtual bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) = 0;
	color3_t& GetColor() { return m_albedo; }
	virtual color3_t GetEmissive() = 0;


protected:
	color3_t m_albedo{1};

};

class Lambertian : public Material
{
public:
	Lambertian(const color3_t& albedo) : Material{ albedo } {}
	// Inherited via Material
	bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) override;
	color3_t GetEmissive() override { return m_albedo; }
};


class Dielectric : public Material
{
public:
	Dielectric(const glm::vec3& albedo, float refractiveIndex) : Material{ albedo }, m_refractiveIndex{ refractiveIndex } {}
	bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) override;
	color3_t GetEmissive() override { return m_albedo; }
protected:
	float m_refractiveIndex = 0;
};

class Metal : public Material
{
public:
	Metal(const glm::vec3& albedo, float fuzz) : Material{ albedo }, m_fuzz{ fuzz } {}
	bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) override;
	color3_t GetEmissive() override { return m_albedo; }

protected:
	float m_fuzz = 0;
};


class Emissive : public Material
{
public:
	Emissive(const color3_t& albedo, float intensity = 1) : Material{ albedo }, m_intensity{ intensity } {}

		bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& color, ray_t& scattered) override { return false; }
	color3_t GetEmissive() override { return m_albedo * m_intensity; }

private:
	float m_intensity{ 1 };
};