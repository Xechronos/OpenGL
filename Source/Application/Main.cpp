
int main(int argc, char* argv[]) {
	neu::file::SetCurrentDirectory("Assets");
	LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

	// initialize engine
	LOG_INFO("initialize engine...");
	neu::GetEngine().Initialize();

	// initialize scene


	SDL_Event e;
	bool quit = false;

	std::vector<neu::vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 } };
	std::vector<neu::vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
	//neu:Transform transform{ {0,0,0,} };
		//neu::Transform camera{ { 0, 0, 5 } };

	
	GLuint vbo[2];
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	std::string vs_source;
	neu::file::ReadTextFile("shaders/basic.vert", vs_source);
	const char* vs_cstr = vs_source.c_str();

GLuint vs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_cstr, NULL);
	glCompileShader(vs);

	GLint success;
	glGetProgramiv(vs, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::string infoLog(512, '\0');  // pre-allocate space
		GLsizei length;
		glGetProgramInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
		infoLog.resize(length);

		LOG_WARNING("Shader compilation failed: {}", infoLog);
	}


	std::string fs_source;
	neu::file::ReadTextFile("shaders/basic.frag", fs_source);
	const char* fs_cstr = fs_source.c_str();

	GLuint fs;
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_cstr, NULL);
	glCompileShader(fs);

	glGetProgramiv(fs, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::string infoLog(512, '\0');  // pre-allocate space
		GLsizei length;
		glGetProgramInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
		infoLog.resize(length);

		LOG_WARNING("Shader compilation failed: {}", infoLog);
	}
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::string infoLog(512, '\0');  // pre-allocate space
		GLsizei length;
		glGetProgramInfoLog(program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
		infoLog.resize(length);

		LOG_WARNING("Shader compilation failed: {}", infoLog);
	}

	glUseProgram(program);


	GLuint uniform = glGetUniformLocation(program, "u_time");
	ASSERT(uniform != -1);

	// MAIN LOOP
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {


				quit = true;
			}
		}

		// update
		neu::GetEngine().Update();

		if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
		glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());
		float dt = neu::GetEngine().GetTime().GetDeltaTime();

		float speed = 10.0f;
		//if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
	   // if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;


		float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
		float scale = neu::math::Remap(-1.0f, 1.0f, 0.3f, 1.5f, neu::math::sin(neu::GetEngine().GetTime().GetTime()));

		neu::vec2 mouse = neu::GetEngine().GetInput().GetMousePosition();
		neu::vec2 position;
		position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mouse.x);
		position.y = -neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), -1.0f, 1.0f, mouse.y);
		// draw
		
		neu::GetEngine().GetRenderer().Clear();

		

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size());

		/*
		glLoadIdentity();
		glPushMatrix();

		glTranslatef(position.x, position.y, 0);
		glRotatef(angle, 1, 1, 0);
		glScalef(scale, scale, 0);

		glBegin(GL_TRIANGLES);

		for (int i = 0; i < points.size(); i++) {
			glColor3f(colors[i].r, colors[i].g, colors[i].b);
			glVertex3f(points[i].x, points[i].y, points[i].z);
		}
		glPopMatrix();
		glEnd();
		*/

		neu::GetEngine().GetRenderer().Present();
	}

	neu::GetEngine().Shutdown();

	return 0;
}
