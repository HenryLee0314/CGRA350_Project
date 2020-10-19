#ifndef PARTICLES_H
#define PARTICLES_H
// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
namespace PTC {
	// CPU representation of a particle
	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight;
		float life; // Remaining life of the particle. if <0 : dead and unused.
		float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

		bool operator<(const Particle& that) const {
			// Sort in reverse order : far particles drawn first.
			return this->cameradistance > that.cameradistance;
		}
	};

	class Particles {
	private:
		GLuint m_shader = 0;
		//static Particle *particleContainer;
		//static GLfloat* g_particle_position_size_data;
		//static GLubyte* g_particle_color_data;
		Particle* particleContainer;
		GLfloat* g_particle_position_size_data;
		GLubyte* g_particle_color_data;
		int m_lastUsedParticle = 0;
		int m_particleSize = 0;
		glm::mat4 modelTransform{ 1.0 };
		glm::vec3 m_mainPosition;
		glm::vec3 m_mainDir;
		bool m_isColor = false;
		int m_amount_per_milisecond = 10000;
		GLuint VAO;
		GLuint base_vertex_buffer;
		GLuint particles_position_buffer;
		GLuint particles_color_buffer;
		GLuint Texture;
		GLuint TextureID;
		GLuint CameraRight_worldspace_ID;
		GLuint CameraUp_worldspace_ID;
		GLuint ViewProjMatrixID;

	public:
		Particles(int particleSize);
		int FindUnusedParticle();
		void SortParticles();
		void draw(const glm::mat4& view, const glm::mat4 proj, float m_distance, int particles_per_second);
		void setColor(bool useColor);
		void set_amout_per_milisecond(int amount_per_milisecond);
		Particles::~Particles();
	};


}
#endif // PARTICLES