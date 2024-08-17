#pragma once

class detour {
public:
	detour() = default;

	explicit detour(void* pFunction, void* pDetour)
		: _base(pFunction), _replace(pDetour) { }

	bool create(void* pFunction, void* pDetour) {
		_base = pFunction;

		if (_base == nullptr)
			return false;

		_replace = pDetour;

		if (_replace == nullptr)
			return false;

		auto status = MH_CreateHook(_base, _replace, &_original);

		if (status == MH_OK) {
			if (!this->replace())
				return false;
		}
		else {
			throw std::runtime_error("failed to create hook");
			return false;
		}

		return true;
	}

	bool replace() {
		if (_base == nullptr)
			return false;

		if (_is_hooked)
			return false;

		auto status = MH_EnableHook(_base);

		if (status == MH_OK)
			_is_hooked = true;
		else {
			throw std::runtime_error("failed to enable hook");
			return false;
		}

		return true;
	}

	template <typename arguments> arguments original() { return (arguments)_original; }
	inline bool is_hooked() { return _is_hooked; }

private:
	bool _is_hooked = false;
	void* _base = nullptr;
	void* _replace = nullptr;
	void* _original = nullptr;
};

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace alloc_key_values {
		using fn = void* (__thiscall*)(void*, const std::int32_t);
		void* __stdcall hook(const std::int32_t size);
	}

	namespace create_move_proxy {
		using fn = void(__stdcall*)(int, float, bool);
		void __stdcall hook(int sequence_number, float input_sample_time, bool is_active);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace send_datagram {
		inline detour send_datagram_detour;
		int __fastcall hook(i_net_channel* net_channel, int edx, bf_write* datagram);
	}

	namespace do_extra_bone_processing {
		inline detour do_extra_bone_processing_detour;
		void __fastcall hook(void* ecx, void* edx, int a2, int a3, int a4, int a5, int a6, int a7);
	}

	namespace should_skip_animation_frame {
		inline detour should_skip_animation_frame_detour;
		bool __fastcall hook(void* this_pointer, void* edx);
	}

	namespace standard_blending_rules {
		inline detour standard_blending_rules_detour;
		void __fastcall hook(void* this_pointer, void* edx, void* hdr, void* pos, void* q, float current_time, int bone_mask);
	}

	namespace do_procedural_foot_plant {
		inline detour do_procedural_foot_plant_detour;
		void __fastcall hook(void* this_pointer, void* edx, void* bone_to_world, void* left_foot_chain, void* right_foot_chain, void* pos);
	}

	namespace update_animation_state {
		inline detour update_animation_state_detour;
		void __vectorcall hook(void* this_pointer, void* unknown, float z, float y, float x, void* unknown1);
	}

	namespace update_client_side_animations {
		inline detour update_client_side_animations_detour;
		void __fastcall hook(void* this_pointer, void* edx);
	}

	namespace modify_eye_position {
		inline detour modify_eye_position_detour;
		void __fastcall hook(void* this_pointer, void* edx, vec3_t& input_eye_position);
	}

	namespace calculate_view_hook {
		inline detour calculate_view_hook_detour;
		void __fastcall hook(void* this_pointer, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov);
	}

	namespace check_for_sequence_change {
		inline detour check_for_sequence_change_detour;
		void __fastcall hook(void* this_pointer, void* edx, void* hdr, int cur_sequence, bool force_new_sequence, bool interpolate);
	}

	namespace build_transformations {
		inline detour check_for_sequence_change_detour;
		void __fastcall hook(void* this_pointer, void* edx, c_studio_hdr* hdr, void* pos, void* q, matrix3x4_t* camera_transform, int bone_mak, void* bone_computed);
	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		void __stdcall hook(int frame_stage);
	}

	namespace override_view {
		using fn = void* (__fastcall*)(i_client_mode*, void* _this, view_setup_t* setup);
		void __fastcall hook(void* _this, void* _edx, view_setup_t* setup);
	}

	namespace dme {
		using fn = void(__thiscall*)(void*, void*, void*, const model_render_info_t&, matrix_t*);
		void __fastcall hook(void* thisptr, void* edx, void* ctx, void* state, const model_render_info_t& info, matrix_t* custom_bone_to_world);
	}

	namespace override_mouse_input {
		using fn = void(__fastcall*)(void*, void*, float*, float*);
		void __fastcall hook(void* ecx_, void* edx_, float* x, float* y);
	};
}
