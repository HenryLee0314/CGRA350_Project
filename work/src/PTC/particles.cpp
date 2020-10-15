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
#include "ptc/util.hpp"

using namespace std;
using namespace cgra;
using namespace glm;


namespace PTC {
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(-1, 1);
	std::uniform_real_distribution<double> distributionPositive(0, 1);

	Particles::Particles(int particleSize) :m_particleSize(particleSize) {
		cout << "particleSize:" << particleSize;
		particleContainer = new Particle[m_particleSize];
		m_lastUsedParticle = 0;

		//GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		shader_builder sb;
		cout << "loading shader Particle.vertexshader";
		sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//particleShader//Particle.vertexshader"));
		cout << "done";
		cout << "loading shader Particle.fragmentshader";
		sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//particleShader//Particle.fragmentshader"));
		cout << "done";
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
		cout << "loading loadDDS";
		Texture = loadDDS((CGRA_SRCDIR + std::string("//res//textures//particle.DDS")).c_str());
		cout << "done";

		// The VBO containing the 4 vertices of the particles.
		// Thanks to instancing, they will be shared by all particles.
		static const GLfloat g_vertex_buffer_data[] = {
			 -0.5f, -0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f,
			  0.5f,  0.5f, 0.0f,
		};
		//GLuint billboard_vertex_buffer;
		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
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

	void Particles::draw(const glm::mat4& view, const glm::mat4 proj, float m_distance) {
		double lastTime = glfwGetTime();
		// Clear the screen
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		delta *= 10000;
		lastTime = currentTime;

		// We will need the camera's position in order to sort the particles
		// w.r.t the camera's distance.
		// There should be a getCameraPosition() function in common/controls.cpp, 
		// but this works too.
		//glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
		glm::vec3 CameraPosition(glm::inverse(view)[3]);
		//cout << "cameraPosition(" << CameraPosition.x << "," << CameraPosition.y << "," << CameraPosition.z << ")";
		//glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glm::mat4 ViewProjectionMatrix = proj * view;

		// Generate 10 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.

		int newparticles = (int)(delta * 10000.0);
		if (newparticles > (int)(0.016f * 10000.0))
			newparticles = (int)(0.016f * 10000.0);
		//cout << "delta:" << delta << ",newparticles:" << newparticles << endl;
		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			particleContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
			//particleContainer[particleIndex].pos = glm::vec3(0.0f, 10.0f, -10.0f);

			glm::vec3 mainpos = glm::vec3(0.0f, 30.0f, -10.0f);
			glm::vec3 randompos = glm::vec3(
				distribution(generator) * 50.0f,
				distribution(generator) * 3.5f,
				distributionPositive(generator) * 10.0f
				);

			particleContainer[particleIndex].pos = mainpos + randompos;
			

			//float spreadSpeed = 3.5f;
			//speed
			//glm::vec3 maindir = glm::vec3(0.0f, 0.0f, 2.0f);
			glm::vec3 maindir = glm::vec3(0, 0, 0);
			glm::vec3 randomdir = glm::vec3(
				100.0f * distribution(generator),
				distribution(generator),
				2.5f * distribution(generator)
				);

			//particleContainer[particleIndex].speed = maindir + randomdir ;
			particleContainer[particleIndex].speed = maindir;

			// Very bad way to generate a random color
			particleContainer[particleIndex].r = rand() % 256;
			particleContainer[particleIndex].g = rand() % 256;
			particleContainer[particleIndex].b = rand() % 256;
			particleContainer[particleIndex].a = (rand() % 256) / 3;

			particleContainer[particleIndex].size = (rand() % 1000) / 8000.0f + 0.1f;

		}



		// Simulate all particles
		int ParticlesCount = 0;
		for (int i = 0; i < m_particleSize; i++) {

			Particle& p = particleContainer[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= delta;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 50.0f;
					p.pos += p.speed * (float)delta;
					p.cameradistance = glm::length(p.pos - CameraPosition);
					//cout << "p.cameradistance(" << p.cameradistance << ")" ;
					//cout << "p.pos(" << p.pos.x << "," << p.pos.y << "," << p.pos.z << ")" << endl;
					// Fill the GPU buffer
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


		cout << "ParticlesCount" << ParticlesCount << endl;


		// Update the buffers that OpenGL uses for rendering.
		// There are much more sophisticated means to stream data from the CPU to the GPU, 
		// but this is outside the scope of this tutorial.
		// http://www.opengl.org/wiki/Buffer_Object_Streaming
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

		// Same as the billboards tutorial
		glUniform3f(CameraRight_worldspace_ID, view[0][0], view[1][0], view[2][0]);
		glUniform3f(CameraUp_worldspace_ID, view[0][1], view[1][1], view[2][1]);

		glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

		//mat4 modelview = view * modelTransform;
		//glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
		//glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
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
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
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
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
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
		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);


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
	}


	//find index of an unused particle starting from last found position. 
	int Particles::FindUnusedParticle() {
		for (int i = m_lastUsedParticle; i < m_particleSize; i++) {
			if (particleContainer[i].life < 0) {
				m_lastUsedParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastUsedParticle; i++) {
			if (particleContainer[i].life < 0) {
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