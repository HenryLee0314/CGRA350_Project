#include "particles.hpp"

// std
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "cgra/cgra_shader.hpp"
#include "PTC/util.hpp"

#include "fluid_grid.h"

using namespace std;
using namespace cgra;
using namespace glm;

#define MIN_PARTICLES_PER_FRAME  1
#define MAX_PARTICLES_PER_FRAME  200

namespace PTC {
	//random generator
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(-1, 1);
	std::uniform_real_distribution<double> distributionPositive(0, 1);

	float clip(float n, float lower, float upper) {
		return std::max(lower, std::min(n, upper));
	}
	
	Particles::~Particles() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &base_vertex_buffer);
		glDeleteBuffers(1, &particles_position_buffer);
		glDeleteBuffers(1, &particles_color_buffer);
		delete(g_particle_position_size_data);
		delete(g_particle_color_data);
		delete(particleContainer);
	}

	Particles::Particles(int particleSize) :m_particleSize(particleSize) {
		//cout << "particleSize:" << particleSize;
		particleContainer = new Particle[m_particleSize];
		m_lastUsedParticle = 0;
		
		m_mainPosition = glm::vec3(0, 12.0f, 0);
		//m_mainDir = glm::vec3(distribution(generator), 0, distributionPositive(generator));
		m_mainDir = glm::vec3(0, 0, 0);

		//cout << "mainPosition:" << m_mainPosition.x << "," << m_mainPosition.y << "," << m_mainPosition.z << endl;
		//cout << "m_mainDir:" << m_mainDir.x << "," << m_mainDir.y << "," << m_mainDir.z << endl;

		//GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		shader_builder sb;
		// cout << "loading shader Particle.vertexshader";
		sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//particleShader//Particle.vertexshader"));
		// cout << "done";
		// cout << "loading shader Particle.fragmentshader";
		sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//particleShader//Particle.fragmentshader"));
		// cout << "done";
		GLuint shader = sb.build();
		m_shader = shader;

		CameraRight_worldspace_ID = glGetUniformLocation(m_shader, "CameraRight_worldspace");
		CameraUp_worldspace_ID = glGetUniformLocation(m_shader, "CameraUp_worldspace");
		ViewProjMatrixID = glGetUniformLocation(m_shader, "VP");

		TextureID = glGetUniformLocation(m_shader, "myTextureSampler");

		g_particle_position_size_data = new GLfloat[m_particleSize * 4];
		g_particle_color_data = new GLubyte[m_particleSize * 4];

		for (int i = 0; i < m_particleSize; i++) {
			particleContainer[i].life = -1.0f;
			particleContainer[i].cameradistance = -1.0f;
		}
		// cout << "loading loadDDS";
		Texture = loadDDS((CGRA_SRCDIR + std::string("//res//textures//particle.DDS")).c_str());
		// cout << "done";

		// The VBO containing the 4 vertices of the particles.
		// Thanks to instancing, they will be shared by all particles.
		static const GLfloat g_vertex_buffer_data[] = {
			 -0.5f, -0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f,
			  0.5f,  0.5f, 0.0f,
		};
		//GLuint base_vertex_buffer;
		glGenBuffers(1, &base_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, base_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// The VBO containing the positions and sizes of the particles
		//GLuint particles_position_buffer;
		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, m_particleSize * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles
		//GLuint particles_color_buffer;
		glGenBuffers(1, &particles_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, m_particleSize * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	}
	void Particles::setColor(bool useColor) {
		m_isColor = useColor;
	}
	void Particles::set_amout_per_milisecond(int amount_per_milisecond) {
		m_amount_per_milisecond = amount_per_milisecond;
	}

	void Particles::draw(const glm::mat4& view, const glm::mat4 proj, float m_distance, int particles_per_second, int m_ratio) {
		
		static double lastTime = glfwGetTime();
		// Clear the screen
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		double delta = (currentTime - lastTime); //seconds
		
		//cout << "currentTime:" << currentTime << "lastTime:" << lastTime << "delta:" << delta << endl;
		// We will need the camera's position in order to sort the particles
		// w.r.t the camera's distance.
		glm::vec3 CameraPosition(glm::inverse(view)[3]);
		//cout << "cameraPosition(" << CameraPosition.x << "," << CameraPosition.y << "," << CameraPosition.z << ")";
		glm::mat4 ViewProjectionMatrix = proj * view;
		
		// Generate new particles as required, but limit with min-max range
		int newparticles = (int)(delta * particles_per_second);
		newparticles = clip(newparticles, MIN_PARTICLES_PER_FRAME, MAX_PARTICLES_PER_FRAME);
		// cout << "delta:" << delta << ",newparticles:" << newparticles << endl;
		//random main speed when initialize.
		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			particleContainer[particleIndex].life = 10.0f; // This particle will live 10 seconds.
			glm::vec3 randompos = glm::vec3(
				10.0f * distribution(generator),
				1.0f * distribution(generator),
			    10.0f * distribution(generator)
				);

			particleContainer[particleIndex].pos = m_mainPosition + randompos;
			//speed
			glm::vec3 randomdir = glm::vec3(
				0.2f * distribution(generator),
				0.2f * distribution(generator),
				0.2f * distribution(generator)
				);
			//particleContainer[particleIndex].speed = m_mainDir + randomdir ;
			particleContainer[particleIndex].speed = vec3(0,0,0);
			//generate a random color
			if (m_isColor) {
				particleContainer[particleIndex].r = rand() % 256;
				particleContainer[particleIndex].g = rand() % 256;
				particleContainer[particleIndex].b = rand() % 256;
			}else {
				int colorV = rand() % 256;
				particleContainer[particleIndex].r = colorV;
				particleContainer[particleIndex].g = colorV;
				particleContainer[particleIndex].b = colorV;
			}
			particleContainer[particleIndex].a = 255 - (rand() % 256) / 3;
			particleContainer[particleIndex].size = (rand() % 100) / 8000.0f + 0.05f;
		}


		// Simulate all particles
		int ParticlesCount = 0;
		int died = 0;
		for (int i = 0; i < m_particleSize; i++) {
			Particle& p = particleContainer[i]; // shortcut
			if (p.life > 0.0f) {

				// Decrease life
				p.life -= delta;
				if (p.life > 0.0f) {
					// Simulate simple physics : gravity + acceleration from environment, no collisions
					CGRA350::FluidGrid *grid = CGRA350::FluidGrid::getInstance();
					float x = clip(p.pos.x, -9.9f, 9.9f);
					float y = clip(p.pos.y, 0.0f, 9.9f);
					float z = clip(p.pos.z, -9.9f, 9.9f);

					int idx = grid->getIndexFromPosition(x, y, z);
					CGRA350::Vec3 speedV = grid->getVelocity(idx);

					//cout << "p.pos before(" << p.pos.x << "," << p.pos.y << "," << p.pos.z << ")" << endl;
					//cout << "p.speed before(" << p.speed.x << "," << p.speed.y << "," << p.speed.z << ")" << endl;

					p.speed.x += speedV.x * m_ratio;
					p.speed.y += (-0.49 * (float)delta);
					//p.speed.y = speedV.y * 1000.0f;
					p.speed.z += speedV.z * m_ratio;
					//cout << "speedV (" << speedV.x << "," << speedV.y << "," << speedV.z << ")" << endl;
					//cout << "p.speed after(" << p.speed.x << "," << p.speed.y << "," << p.speed.z << ")" << endl;
					if (p.pos.y < 0.0f) {
						p.life = 0.0f;
						p.pos.y = 0.0f;
						p.cameradistance = -1.0f;
						died++;
					}
					p.pos += p.speed * (float)delta;
					//cout << "p.pos after(" << p.pos.x << "," << p.pos.y << "," << p.pos.z << ")" << endl;
					p.cameradistance = glm::length(p.pos - CameraPosition);
					g_particle_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particle_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particle_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

					g_particle_position_size_data[4 * ParticlesCount + 3] = p.size;

					g_particle_color_data[4 * ParticlesCount + 0] = p.r;
					g_particle_color_data[4 * ParticlesCount + 1] = p.g;
					g_particle_color_data[4 * ParticlesCount + 2] = p.b;
					g_particle_color_data[4 * ParticlesCount + 3] = p.a;
				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}
				ParticlesCount++;
			}
			
		}

		SortParticles();
		//cout << "ParticlesDied" << died << endl;
		cout << "ParticlesCount " << ParticlesCount << endl;

		// Update the buffers that OpenGL uses for rendering.
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, m_particleSize * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particle_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, m_particleSize * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particle_color_data);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Use our shader
		glUseProgram(m_shader);
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		//Set shader values
		glUniform3f(CameraRight_worldspace_ID, view[0][0], view[1][0], view[2][0]);
		//cout << "CameraRight_worldspace_ID" << view[0][0] << "," << view[1][0] << "," << view[2][0] << endl;
		glUniform3f(CameraUp_worldspace_ID, view[0][1], view[1][1], view[2][1]);
		//cout << "CameraUp_worldspace_ID" << view[0][1] << "," << view[1][1] << "," << view[2][1] << endl;
		glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, base_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute 0 for base_vertex, must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute 1 for position, must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute 2 for color, must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// These functions are specific to glDrawArrays*Instanced*.
		// The first parameter is the attribute buffer we're talking about.
		// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
		// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

		// Draw the particules !
		// This draws many times a small triangle_strip (which looks like a quad).
		// This is equivalent to :
		// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
		// but faster.
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		lastTime = currentTime;

	/*
	delete[] g_particule_position_size_data;

	// Cleanup VBO and shader
	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	*/
		glDisable(GL_BLEND);
	}


	//find index of an unused particle starting from last found position. 
	int Particles::FindUnusedParticle() {
		for (int i = m_lastUsedParticle; i < m_particleSize; i++) {
			if (particleContainer[i].life <= 0) {
				m_lastUsedParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastUsedParticle; i++) {
			if (particleContainer[i].life <= 0) {
				m_lastUsedParticle = i;
				return i;
			}
		}

		return 0; // All particles are taken, override the first one
	}

	void Particles::SortParticles() {
		std::sort(&particleContainer[0], &particleContainer[m_particleSize]);
	}

}
