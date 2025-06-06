//===========================================================================================================================================================
// 
// ImGUI_manager [ImGUI_manager.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "ImGUI_manager.h"

// 静的メンバ変数
EditWindowMain* ImguiManager::m_pEditWindowMain = nullptr;
EditWindowBaseobj* ImguiManager::m_pEditWindowBaseobj = nullptr;
//===========================================================================================================
// コンストラクタ
//===========================================================================================================
ImguiManager::ImguiManager()
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
ImguiManager::~ImguiManager()
{
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
void ImguiManager::Init()
{
	if (m_pEditWindowMain == nullptr)
	{
		m_pEditWindowMain = new EditWindowMain;
	}
	
	if (m_pEditWindowBaseobj == nullptr)
	{
		m_pEditWindowBaseobj = new EditWindowBaseobj;
	}
}

//===========================================================================================================
// コンテキストのセットアップ
//===========================================================================================================
void ImguiManager::SetUpContext()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void ImguiManager::Uninit()
{
	if (m_pEditWindowMain != nullptr)
	{
		delete m_pEditWindowMain;
		m_pEditWindowMain = nullptr;
	}

	if (m_pEditWindowBaseobj != nullptr)
	{
		delete m_pEditWindowBaseobj;
		m_pEditWindowBaseobj = nullptr;
	}
}

//===========================================================================================================
// テストウィンドウ
//===========================================================================================================
void ImguiManager::TestWindow()
{
	//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	//{
	//	static float f = 0.0f;


	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	static char str0[128] = "Hello, world!";
	//	ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));


	//	// 選択ボックス
	//	static bool selected[4] = { false, true, false, false };
	//	ImGui::Selectable("1. こんにちは", &selected[0]);
	//	ImGui::Selectable("2. I am selectable", &selected[1]);
	//	ImGui::Text("3. I am not selectable");
	//	ImGui::Selectable("4. I am selectable", &selected[2]);
	//	if (ImGui::Selectable("5. I am double clickable", selected[3], ImGuiSelectableFlags_AllowDoubleClick))
	//		if (ImGui::IsMouseDoubleClicked(0))
	//			selected[3] = !selected[3];

	//	static int e = 0;
	//	ImGui::RadioButton(str0, &e, 0);
	//	ImGui::RadioButton("radio b", &e, 1);
	//	ImGui::RadioButton("radio c", &e, 2);

	//	static int i0 = 123;
	//	ImGui::InputInt("input int", &i0);

	//	static int i1 = 50;
	//	static int i2 = 42;
	//	ImGui::DragInt("drag int", &i1, 1);

	//	ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%.0f%%");

	//	static float angle = 0.0f;
	//	ImGui::SliderAngle("slider angle", &angle);

	//	static float begin = 10, end = 90;
	//	ImGui::DragFloatRange2("range", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%");




	//	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	//	ImGui::End();
	//}
}

