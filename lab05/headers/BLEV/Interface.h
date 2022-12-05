#ifndef __BLEV_INTERFACE_H__
#define __BLEV_INTERFACE_H__

#include "pch.h"
#include "Geometry.h"
#include "Enums.h"
#include "Data.h"
#include "Validator.h"

namespace BLEV {
	class Interface {
		BLEV::Data& _data;
	public:
		Interface(BLEV::Data& data) : _data(data), menu(data), obj_table(data), canvas(data) {}
	private:
		struct ready_l_system {
			std::string name;
			std::string axiom;
			float angle; //rad
			std::deque<std::pair<char, std::string>> rules;
			std::string fwd_atoms;
			ready_l_system(
				std::string _name,
				std::string _axiom,
				float _angle,
				std::deque<std::pair<char, std::string>> _rules,
				std::string _fwd_atoms) {
				name = _name;
				axiom = _axiom;
				angle = _angle;
				rules = _rules;
				fwd_atoms = _fwd_atoms;
			}
		};
		static const std::vector<ready_l_system*> ready_l_systems;

		struct ConsoleFields {
			char pseudo_console[100] = { '\0' };
			std::string feedback;
			ImVec4 feedback_color;
		};
		std::vector<ConsoleFields*> console{
			new ConsoleFields(),
			new ConsoleFields(),
			new ConsoleFields(),
			new ConsoleFields(),
			new ConsoleFields(),
			new ConsoleFields()
		};

		struct TextFilters{
			static int FilterLetters(ImGuiInputTextCallbackData* data)
			{
				if (data->EventChar < 256 && strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", (char)data->EventChar))
					return 0;
				return 1;
			}
			static int FilterLsys(ImGuiInputTextCallbackData* data)
			{
				if (data->EventChar < 256 && strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ+-[]@", (char)data->EventChar))
					return 0;
				return 1;
			}
		};

		void F_Rotate();
		void F_Translate();
		void F_Scale();
		void F_Reflect();
		// union
		void F_Edit();
		void F_Displace();
		void F_MeshGraph();
		void F_FloatingHorizon();
		void F_RotationBody();
		void F_Union();
		void F_Lsystem();
		void F_Classify();
		void F_Camera();

		void F_QuickHull();
		void F_Present();

		// union
		void F_Shells();
		

		struct B_method_open {
			//bool b_rotate_open = false;
			//bool b_translate_open = false;
			//bool b_scale_open = false;
			//bool b_reflect_open = false;
			bool b_edit_open = false;
			
			bool b_displace_open = false;
			bool b_union_open = false;
			bool b_lsys_open = false;
			bool b_classify_open = false;
			bool b_camera_open = false;
			bool b_rotation_body_open = false;
			bool b_mesh_graph_open = false;
			bool b_floating_horizon_open = false;

			bool b_shells_open = false;
		} bmo;
	public:
		void ShowExternalWindows();
	private:
		struct Global_visual_params {
			VisualParams vp;
			float color4f[4]{1.f, 1.f, 0.f, 1.f};
		} global_vp;

		struct Menu {
			BLEV::Data& _data;

			void ShowFileManagerMenu();
			void ShowModesMenu();
			void ShowMethodsMenu(B_method_open&);
			void ShowAddingMenu();

			void Show(B_method_open&, Global_visual_params&);

			Menu(BLEV::Data& data) : _data(data) {}
		} menu {_data};
		struct ObjectTable {
			BLEV::Data& _data;

			void ShowPrimTable(Primitive* prim, size_t idx);
			void ShowLsysTable(Lsystem* lsys, size_t idx);
			void ShowMeshTable(Mesh* mesh, size_t idx);
			void ShowHorizonTable(FloatingHorizon* horizon, size_t idx);
			void Show();

			ObjectTable(BLEV::Data& data) : _data(data) {}
		} obj_table {_data};
		struct Canvas {
			const float MIN_WIDTH = 50.f;
			const float MIN_HEIGHT = 50.f;

			BLEV::Data& _data;

			Camera main_camera;
			Eigen::Matrix4f vp; 
			ImVec2 prev_point = ImVec2(0.f, 0.f);
			ImVec2 deltaMouse = ImVec2(0.f, 0.f);
			float deltaTime = 0.0f;	// Time between current frame and last frame
			float lastFrame = 0.0f; // Time of last frame
			float deltaTimeSec = 0.0f; // Sigma deltaTime [0; 1)
			size_t fps = 0;
			size_t oldSize = 0;
			ZBuffer zbuf;
			LightBuffer lbuf;

			ImDrawList* draw_list;

			ImVec2 scrolling{ 0.0f, 0.0f },
				p[2],
				size,
				origin,
				mouse_pos,
				drag_delta;

			struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
			static const uint8_t hotkeysSize = 1;
			const ImGuiKey hotkeys[hotkeysSize]{ ImGuiKey_M };

			float thickness = 1.0f;
			float curr_color[4] = { 1.f, 1.f, 0.f, 1.f };

			ImVec2* point_of_transformation = nullptr;

			bool show_fps = true;

			bool is_hovered = false,
				is_active = false;

			bool b_grid_2d_enabled = false,
				b_grid_3d_enabled = true,
				b_context_menu_enabled = true;

			const float GRID_STEP = 64.0f;
			const float GRID_BORDER = 300.f;
			const VisualParams vis_p{ IM_COL32(200, 200, 200, 40), 1.f, true };

			void ProcessCamKeyboardInput(Camera& cam, const float& deltaTime);
			void ProcessCamMouseInput(Camera& cam, const ImVec2& deltaMouse);
			void SwitchModes();
			void DrawObjects();
			void PollCallbacks();
			void ShowContextMenu();
			void Draw2dGrid();
			void Draw3dGrid();
			void DrawAxis();

			//void Update();
			void Body();

			void Show();

			Canvas(BLEV::Data& data) : _data(data) {}
		} canvas {_data};
	public:
		void ShowContent();
	};
}

#endif // !__BLEV_INTERFACE_H__
