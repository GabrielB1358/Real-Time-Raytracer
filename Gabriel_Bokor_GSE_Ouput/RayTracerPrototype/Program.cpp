#include "Program.h"

Program::Program()
{
}

Program::~Program()
{
}

void Program::Run()
{
	//Time utility variables for fps
	srand(time(0));
	double DeltaTime = 0;
	unsigned int CurrentTime = 0;
	unsigned int LastTime = 0;
	std::vector<double> FrameTimes;

	//Window & Context Creation
	glm::vec2 Resolution = glm::vec2(1000, 1000);
	glm::vec2 WindowPos = glm::vec2(500, 40);
	SDL_Window* Window = SDL_CreateWindow("Real-Time Ray Tracer", WindowPos.x, WindowPos.y, Resolution.x, Resolution.y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	SDL_GLContext Context = SDL_GL_CreateContext(Window);
	if (glewInit() != GLEW_OK) { throw std::exception(); }

	//Printing relevant hardware information
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	//IMGUI GUI system
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	bool ShowGUI = true;
	ImGui::StyleColorsDark(); // dark theme set

	const char* glslVersion = "#version 460"; // set the OpenGL GLSL version in use
	ImGui_ImplSDL2_InitForOpenGL(Window, Context);
	ImGui_ImplOpenGL3_Init(glslVersion);


	//VBOs
	std::shared_ptr<Vbo> vertexCoords = std::make_shared<Vbo>();
	vertexCoords->AddVec3(glm::vec3(-1.0f, -1.0f, 0.0f));
	vertexCoords->AddVec3(glm::vec3(1.0f, -1.0f, 0.0f));
	vertexCoords->AddVec3(glm::vec3(-1.0f, 1.0f, 0.0f));
	vertexCoords->AddVec3(glm::vec3(1.0f, -1.0f, 0.0f));
	vertexCoords->AddVec3(glm::vec3(1.0f, 1.0f, 0.0f));
	vertexCoords->AddVec3(glm::vec3(-1.0f, 1.0f, 0.0f));

	std::shared_ptr<Vbo> texCoords = std::make_shared<Vbo>();
	texCoords->AddVec2(glm::vec2(0.0f, 1.0f));
	texCoords->AddVec2(glm::vec2(1.0f, 1.0f));
	texCoords->AddVec2(glm::vec2(0.0f, 0.0f));
	texCoords->AddVec2(glm::vec2(1.0f, 1.0f));
	texCoords->AddVec2(glm::vec2(1.0f, 0.0f));
	texCoords->AddVec2(glm::vec2(0.0f, 0.0f));


	//VAO
	std::shared_ptr<Vao> VAO = std::make_shared<Vao>();
	VAO->AddVbo(vertexCoords);
	VAO->AddVbo(texCoords);


	//Texture
	Texture SceneTexture(Resolution);


	//Shader Programs
	Shader MainProgram("Shaders/FragShad.frag", GL_FRAGMENT_SHADER, false);
	MainProgram.AddShader("Shaders/VertShad.vert", GL_VERTEX_SHADER);
	MainProgram.Link();

	Shader ComputeProgram("Shaders/otherCompShad.comp", GL_COMPUTE_SHADER, true);
	ComputeProgram.Link();


	//User Input
	std::shared_ptr<Input> UserInput = std::make_shared<Input>();


	//SSBO
	ShaderData ComputeData;

	ComputeData.RandomNumber = float(rand()) / float(RAND_MAX);
	ComputeData.doReflect = 1;
	ComputeData.doSS = 1;
	ComputeData.doGI = 0;
	ComputeData.AASampleSize = 2;
	ComputeData.SSSampleSize = 40;
	ComputeData.GISampleSize = 50;
	ComputeData.lightOffset = 0;
	ComputeData.lightColour = glm::vec3(1, 1, 1);
	ComputeData.sceneChoice = 0;
	ComputeData.objectCount = 5;
	
	float lightOffsetVal = 0;
	bool moveLight = false;

	GLuint SSBO;
	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(struct ShaderData), &ComputeData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);


	//Camera
	Camera Camera(90.0f, (Resolution.x / Resolution.y), 0.1f, 50.0f);
	ComputeData.TransformMatrix = Camera.GetTransformMat();


	//Run Loop
	SDL_Event event;
	bool start = true;
	bool UseGUI = true;

	LastTime = SDL_GetTicks();

	while (start)
	{
		//FPS Average calculator
		CurrentTime = SDL_GetTicks();
		DeltaTime = (double)(CurrentTime - LastTime) / 1000.0f;
		LastTime = CurrentTime;
		FrameTimes.push_back(DeltaTime);
		if (FrameTimes.size() > 60)
		{
			FrameTimes.erase(FrameTimes.begin(), FrameTimes.begin() + 1);
		}
		double TotalTime = 0;
		for (size_t i = 0; i < FrameTimes.size(); i++)
		{
			TotalTime += FrameTimes[i];
		}
		TotalTime /= FrameTimes.size();


		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//Input Polling
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);

			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				start = false;
			}
			UserInput->ProcessInput(event, Resolution);
		}
		if (UseGUI)
		{
			SDL_WarpMouseInWindow(NULL, Resolution.x / 2, Resolution.y / 2);
			SDL_ShowCursor(false);
		}
		else
			SDL_ShowCursor(true);

		if (UserInput->IsKeyDown(tab))
		{
			UseGUI = UseGUI ? false : true;
		}

		//Light sphere movement (for inside the compute shader)
		if (moveLight)
		{
			lightOffsetVal += 0.05f;
			ComputeData.lightOffset = sin(lightOffsetVal) * 5.0f;
		}
		else
		{
			ComputeData.lightOffset = 0;
		}


		Camera.Tick(UserInput, DeltaTime, UseGUI);
		ComputeData.TransformMatrix = Camera.GetTransformMat();
		ComputeData.RandomNumber = float(rand()) / float(RAND_MAX);

		//SSBO Data binding
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		void* ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(struct ShaderData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		memcpy(ptr, &ComputeData, sizeof(struct ShaderData));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


		//Texture Writing and then rendering
		ComputeProgram.Render(Resolution, VAO.get(), &SceneTexture);

		MainProgram.Render(Resolution, VAO.get(), &SceneTexture);


		//Draw GUI using ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		if (ShowGUI)
		{
			ImGui::Begin("Feature Controls - Press Tab");

			ImGui::Text("A real-time ray tracing demonstration");
			ImGui::Text(" ");

			int GUISceneChoice = ComputeData.sceneChoice;
			ImGui::SliderInt("Scene Choice", &GUISceneChoice, 0, 1);
			ComputeData.sceneChoice = GUISceneChoice;
			if (GUISceneChoice == 0)
			{
				ComputeData.objectCount = 5;
			}
			else
			{
				ComputeData.objectCount = 8;
			}

			glm::vec3 GUILightCol = ComputeData.lightColour;
			ImGui::ColorEdit3("Light Colour", &(GUILightCol[0]));
			ComputeData.lightColour = GUILightCol;

			bool GUIReflections = ComputeData.doReflect;
			ImGui::Checkbox("Reflections", &GUIReflections);
			ComputeData.doReflect = GUIReflections;

			int GUIAASamples = ComputeData.AASampleSize;
			ImGui::SliderInt("AA Samples", &GUIAASamples, 1, 16);
			ComputeData.AASampleSize = float(GUIAASamples);

			bool GUISoftShadows = ComputeData.doSS;
			ImGui::Checkbox("Soft Shadows", &GUISoftShadows);
			ComputeData.doSS = GUISoftShadows;

			int GUISSCount = ComputeData.SSSampleSize;
			ImGui::SliderInt("SS Samples", &GUISSCount, 1, 200);
			ComputeData.SSSampleSize = GUISSCount;

			bool GUIGlobalIllum = ComputeData.doGI;
			ImGui::Checkbox("Global Illumination", &GUIGlobalIllum);
			ComputeData.doGI = GUIGlobalIllum;

			int GUIGISamples = ComputeData.GISampleSize;
			ImGui::SliderInt("GI Samples", &GUIGISamples, 1, 100);
			ComputeData.GISampleSize = GUIGISamples;

			bool GUIMoveLight = moveLight;
			ImGui::Checkbox("Move Light", &GUIMoveLight);
			moveLight = GUIMoveLight;

			ImGui::Text(" ");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		SDL_GL_SwapWindow(Window);
		UserInput->Tick();
	}

	//Handle relevant variable destruction
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(Context);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}