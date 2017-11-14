#include "Level.h"
#include "ColorCodes.h"
#include "LevelFormat.h"
#include <fstream>
#include <algorithm>
#include <Engine/Logger.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Level::Level()
{
}


Level::~Level()
{
}


void Level::init(const std::string& path, float windowWidth, float windowHeight)
{
	std::ifstream file(path);
	std::vector<std::string> data;
	std::string line;

	setupRendering();

	// Load level's data from file
	if (file.is_open()) {
		while (std::getline(file, line)) {
			data.push_back(line);
		}
		file.close();
	}
	else {
		Logger::Error("Could not open the following level file: " + path);
	}

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	float blockWidth = windowWidth / static_cast<float>(data[0].length());
	float blockHeight = (windowHeight / 2.0f) / static_cast<float>(data.size());

	int paddleWidth = static_cast<int>(windowWidth) / 10;
	int paddleHeight = static_cast<int>(windowHeight) / 25;
	int paddleX = static_cast<int>(windowWidth) / 2 - paddleWidth / 2;
	int paddleY = static_cast<int>(windowHeight) - paddleHeight;
	m_paddle.init(true, (float)paddleX, (float)paddleY, paddleWidth, paddleHeight, 8.0f, COLOR_CODES.at(COLOR_NONE));

	int ballWidth = 26;
	int ballHeight = 26;
	int ballX = paddleX + (paddleWidth / 2) - (ballWidth / 2);
	int ballY = paddleY - ballHeight;
	m_ball.init(ballX, ballY, ballWidth, ballHeight, COLOR_CODES.at(COLOR_NONE), 999, true, false, false);
	m_ball.speed = 10.5f;

	// Initialize the objects
	//int XPos, int YPos, int Width, int Height, int Color, int Health, bool IsSolid, bool IsStatic, bool IsDestructible
	for (size_t y = 0; y < data.size(); y++) {
		for (size_t x = 0; x < data[y].length(); x++) {
			if (data[y][x] == SOLID_BLOCK) {
				m_blocks.emplace_back(Object(
					(float)x * blockWidth,
					(float)y * blockHeight,
					(int)blockWidth,
					(int)blockHeight,
					COLOR_CODES.at(COLOR_NONE),
					999,
					true,
					true,
					false
				));
			}
			// Colored blocks
			else if (data[y][x] != AIR_BLOCK) {
				m_blocks.emplace_back(Object(
					(float)x * blockWidth,
					(float)y * blockHeight,
					(int)blockWidth,
					(int)blockHeight,
					COLOR_CODES.at(data[y][x] - '0'),
					1,
					true,
					true,
					true
				));
			}
		}
	}
}


void Level::setupRendering()
{
	float vertices[] = {
		// Pos              // Tex
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Level::update(PostProcessor* postprocessor, float deltaTime)
{
	float steps = m_ball.speed;
	int count = 0;

	bool collision = false;
	while (steps > 0.0f && !collision) {
		float updateSpeed = std::min(steps, 1.0f);
		m_ball.update(deltaTime, updateSpeed);
		collision = checkCollision(postprocessor);
		steps -= 1.0f;
		count++;
	}
}


void Level::breakBlock(int idx)
{
	if (m_blocks[idx].isDestructible)
		m_blocks.erase(m_blocks.begin() + idx);
}


bool breakBlockSort(int a, int b)
{
	return a > b;
}

void clamp(float &x, float minVal, float maxVal)
{
	x = std::min(std::max(x, minVal), maxVal);
}

bool Level::checkCollision(PostProcessor* postProcessor)
{
	glm::vec2 initialDir = m_ball.direction;

	int ballXRadius = m_ball.width / 2;
	int ballYRadius = m_ball.height / 2;
	int ballCenterX = m_ball.xPos + ballXRadius;
	int ballCenterY = m_ball.yPos + ballYRadius;

	{ // Collision of ball with screen
		if (ballCenterX + ballXRadius >= m_windowWidth) m_ball.direction.x *= -1;
		else if (m_ball.xPos < 0) m_ball.direction.x *= -1;

		if (ballCenterY + ballYRadius >= m_windowHeight) m_ball.direction.y *= -1;
		else if (m_ball.yPos < 0) m_ball.direction.y *= -1;
		
		clamp(m_ball.xPos, 0.0f, m_windowWidth - (float)m_ball.width);
		clamp(m_ball.yPos, 0.0f, m_windowHeight - (float)m_ball.height);
	}

	{ // Collision with paddle
		int paddleXRadius = m_paddle.width / 2;
		int paddleYRadius = m_paddle.height / 2;
		int paddleCenterX = m_paddle.xPos + paddleXRadius;
		int paddleCenterY = m_paddle.yPos + paddleYRadius;

		float xDiff = abs(ballCenterX - paddleCenterX);
		float yDiff = abs(ballCenterY - paddleCenterY);
		float minXDiff = ballXRadius + paddleXRadius;
		float minYDiff = ballYRadius + paddleYRadius;

		// X Collision
		if (xDiff < minXDiff) {
			// Y Collision
			if (yDiff < minYDiff) {
				float collisionDistFromCenter = (m_ball.xPos + m_ball.width / 2) - (m_paddle.xPos + m_paddle.width / 2);
				float farthestCollision = m_paddle.width / 2;

				m_ball.direction.y = std::max(-1.0f + (-abs(collisionDistFromCenter / farthestCollision)), -1.0f);
				Logger::Log("Set direction y to " + std::to_string(m_ball.direction.y));
				m_ball.direction.x = std::min(collisionDistFromCenter / farthestCollision, 1.0f);

				if (minYDiff - yDiff < minXDiff - xDiff) {
					m_ball.yPos = m_paddle.yPos - m_ball.height;
				}
				else {
					if (m_ball.xPos > m_paddle.xPos) {
						m_ball.xPos = m_paddle.xPos + m_paddle.width;
					}
					else {
						m_ball.xPos = m_paddle.xPos - m_ball.width;
					}
				}
			}
		}
	}

	float yChange = 0.0f;
	float xChange = 0.0f;

	bool yCollision = false;
	bool xCollision = false;
	std::vector<int> blocksToBreak;

	// Collision with blocks
	for (size_t i = 0; i < m_blocks.size(); i++) {

		int blockXRadius = m_blocks[i].width / 2;
		int blockYRadius = m_blocks[i].height / 2;
		int blockCenterX = m_blocks[i].xPos + blockXRadius;
		int blockCenterY = m_blocks[i].yPos + blockYRadius;

		float xDiff = abs(ballCenterX - blockCenterX);
		float yDiff = abs(ballCenterY - blockCenterY);
		float minXDiff = ballXRadius + blockXRadius;
		float minYDiff = ballYRadius + blockYRadius;

		// X Collision
		if (xDiff < minXDiff) {
			// Y Collision
			if (yDiff < minYDiff) {
				blocksToBreak.push_back(i);
				if (yDiff >= minYDiff - 1.0f) {
					yCollision = true;
					yChange = (ballCenterY > blockCenterY) ? 1.0f : -1.0f;
				}
				else {
					xCollision = true;
					xChange = (ballCenterX > blockCenterX) ? 1.0f : -1.0f;
				}
			}
		}
	}

	std::sort(blocksToBreak.begin(), blocksToBreak.end(), breakBlockSort);
	for (size_t i = 0; i < blocksToBreak.size(); i++) {
		if (!m_blocks[blocksToBreak[i]].isDestructible) postProcessor->startEffect(Effect::SHAKE);
		breakBlock(blocksToBreak[i]);
	}

	if (yCollision) {
		m_ball.direction.y *= -1;
		m_ball.yPos += yChange;
	}
	if (xCollision) {
		m_ball.direction.x *= -1;
		m_ball.xPos += xChange;
	}

	return (m_ball.direction != initialDir);
}


void Level::render(Engine::ShaderProgram *shader, Engine::GLTexture2D &paddle, Engine::GLTexture2D &ball, Engine::GLTexture2D &block, Engine::GLTexture2D solidBlock, Engine::GLTexture2D &background)
{
	glm::mat4 projection = glm::ortho(
		0.0f,
		static_cast<GLfloat>(m_windowWidth),
		static_cast<GLfloat>(m_windowHeight),
		0.0f,
		-1.0f,
		1.0f
	);

	shader->use();
	shader->setInt("image", 0);
	shader->setMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vao);

	/*
	{ //// Background ////
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(background.width, background.height, 1.0f));
		shader->setMat4("model", model);
		shader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));

		glBindTexture(GL_TEXTURE_2D, background.id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	*/

	{ //// Paddle ////
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(m_paddle.xPos, m_paddle.yPos, 0.0f));
		model = glm::scale(model, glm::vec3(m_paddle.width, m_paddle.height, 1.0f));
		shader->setMat4("model", model);
		shader->setVec3("color", glm::vec3(m_paddle.color.x / 255.0f, m_paddle.color.y / 255.0f, m_paddle.color.z / 255.0f));

		glBindTexture(GL_TEXTURE_2D, paddle.id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	{ //// Ball ////
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(m_ball.xPos, m_ball.yPos, 0.0f));
		model = glm::scale(model, glm::vec3(m_ball.width, m_ball.height, 1.0f));
		shader->setMat4("model", model);
		shader->setVec3("color", glm::vec3(m_ball.color.x / 255.0f, m_ball.color.y / 255.0f, m_ball.color.z / 255.0f));

		glBindTexture(GL_TEXTURE_2D, ball.id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	for (auto &b : m_blocks) {
		if (b.isDestructible) {
			glBindTexture(GL_TEXTURE_2D, block.id);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, solidBlock.id);
		}

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(b.xPos, b.yPos, 0.0f));
		model = glm::scale(model, glm::vec3(b.width, b.height, 1.0f));
		shader->setMat4("model", model);
		shader->setVec3("color", glm::vec3(b.color.x / 255.0f, b.color.y / 255.0f, b.color.z / 255.0f));

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);

	shader->unuse();
}


void Level::movePaddle(float xChange, float yChange, float deltaTime)
{
	m_paddle.move(xChange, yChange, deltaTime, m_paddle.speed);
	clamp(m_paddle.xPos, 0.0f, m_windowWidth - (float)m_paddle.width - 1.0f);
}


void Level::moveBall(float xChange, float yChange, float deltaTime)
{
	m_ball.move(xChange, yChange, deltaTime);
}


void Level::setBallMovement(const glm::vec2 &dir, float speed)
{
	m_ball.direction = dir;
	m_ball.speed = speed;
}


void Level::setBallMovement(const glm::vec2 &dir)
{
	m_ball.direction = dir;
}


void Level::clampStickyBall()
{
	clamp(m_ball.xPos, (float)m_paddle.width / 2.0f - (float)m_ball.width / 2.0f, m_windowWidth - (float)m_paddle.width / 2.0f - (float)m_ball.width / 2.0f);
}