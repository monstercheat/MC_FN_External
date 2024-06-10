#include "m_draw.hpp"
#include "m_hex.hpp"

class ue 
{
public:
	
	auto get_player_name(uintptr_t player_state) -> std::string
	{

		int pNameLength;
		_WORD* pNameBufferPointer;
		int i;
		char v25;
		int v26;
		int v29;

		char16_t* pNameBuffer;

		uintptr_t pNameStructure = Read <uintptr_t>(player_state + offset::pname);
		if (mem::is_valid(pNameStructure)) {
			pNameLength = Read <int>(pNameStructure + 0x10);
			if (pNameLength <= 0)
				return "BOT";

			pNameBuffer = new char16_t[pNameLength + 1];
			uintptr_t pNameEncryptedBuffer = Read <uintptr_t>(pNameStructure + 0x8);
			if (mem::is_valid(pNameEncryptedBuffer)) {
				read_array(pNameEncryptedBuffer, pNameBuffer, pNameLength);

				// Manually null-terminate the buffer.
				pNameBuffer[pNameLength] = u'\0'; // Explicitly null-terminate

				v25 = pNameLength - 1;
				v26 = 0;
				pNameBufferPointer = (_WORD*)pNameBuffer;

				for (i = (v25) & 3;; *pNameBufferPointer++ += i & 7) {
					v29 = pNameLength - 1;
					if (!(_DWORD)pNameLength)
						v29 = 0;

					if (v26 >= v29)
						break;

					i += 3;
					++v26;
				}

				std::u16string temp_wstring(pNameBuffer);
				delete[] pNameBuffer;
				return std::string(temp_wstring.begin(), temp_wstring.end());
			}
		}
	}

	auto in_screen(fvector2d screen_location) -> bool {

		if (screen_location.x > 0 && screen_location.x < screen_width && screen_location.y > 0 && screen_location.y < screen_height) return true;
		else return false;
	}


	camera_position_s get_camera()
	{
		camera_position_s camera;

		auto location_pointer = Read <uintptr_t>(arrays->uworld + 0x110);
		auto rotation_pointer = Read <uintptr_t>(arrays->uworld + 0x120);

		struct FNRot
		{
			double a;
			char pad_0008[24];
			double b;
			char pad_0028[424];
			double c;
		} fnRot;

		fnRot.a = Read <double>(rotation_pointer);
		fnRot.b = Read <double>(rotation_pointer + 0x20);
		fnRot.c = Read <double>(rotation_pointer + 0x1d0);

		camera.rotation.x = asin(fnRot.c) * (180.0 / M_PI);
		camera.rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
		camera.rotation.z = 0;
		camera.location = Read<fvector>(location_pointer);
		camera.fov = Read<float>(arrays->player_controller + 0x394) * 90.f;

		return camera;
	}

	inline fvector2d w2s(fvector worldlocation)  {

		camera_postion = get_camera();

		_MATRIX tempMatrix = Matrix(camera_postion.rotation);

		fvector vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		fvector vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		fvector vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		fvector vDelta = worldlocation - camera_postion.location;
		fvector vTransformed = fvector(vDelta.dot(vAxisY), vDelta.dot(vAxisZ), vDelta.dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;
		return fvector2d((screen_width / 2.0f) + vTransformed.x * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z, (screen_height / 2.0f) - vTransformed.y * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z);

	}

	static auto Bone(uintptr_t skeletal_mesh, int bone_index) -> fvector {

		uintptr_t bone_array = Read<uintptr_t>(skeletal_mesh + offset::bone_array);

		if (bone_array == NULL) 

		bone_array = Read<uintptr_t>(skeletal_mesh + offset::bone_array + 0x10);

		FTransform bone = Read<FTransform>(bone_array + (bone_index * 0x60));

		FTransform component_to_world = Read<FTransform>(skeletal_mesh + offset::component_to_world);

		D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());

		return fvector(matrix._41, matrix._42, matrix._43);
	}
	// Bone : 0x5E0 or 0x598
		//Bone Cache : 0x598 or 0x5E0

	static auto is_visible(uintptr_t skeletal_mesh) -> bool {

		auto last_submit = Read<float>(skeletal_mesh + offset::last_sumbit); //0x2E8
		auto last_render = Read<float>(skeletal_mesh + offset::last_render); //0x2F0
		return (bool)(last_render + 0.06f >= last_submit);
	}

	__forceinline auto skeleton(uintptr_t mesh) -> void
	{
		const int color = ImColor(255, 255, 255);
		fvector bonePositions[] = {
			Bone(mesh, 109),  // HeadBone
			Bone(mesh, 2),   // Hip
			Bone(mesh, 66),  // Neck
			Bone(mesh, 9),   // UpperArmLeft
			Bone(mesh, 38),  // UpperArmRight
			Bone(mesh, 10),  // LeftHand
			Bone(mesh, 39),  // RightHand
			Bone(mesh, 11),  // LeftHand1
			Bone(mesh, 40),  // RightHand1
			Bone(mesh, 78),  // RightThigh
			Bone(mesh, 71),  // LeftThigh
			Bone(mesh, 79),  // RightCalf
			Bone(mesh, 72),  // LeftCalf
			Bone(mesh, 75),  // LeftFoot 
			Bone(mesh, 82)   // RightFoot
		};

		fvector2d bonePositionsOut[16];
		for (int i = 0; i < 16; ++i) {
			bonePositionsOut[i] = w2s(bonePositions[i]);
		}

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImVec2(bonePositionsOut[7].x, bonePositionsOut[7].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImVec2(bonePositionsOut[8].x, bonePositionsOut[8].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[9].x, bonePositionsOut[9].y), ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), ImVec2(bonePositionsOut[9].x, bonePositionsOut[9].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[13].x, bonePositionsOut[13].y), ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), color, 2.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bonePositionsOut[14].x, bonePositionsOut[14].y), ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), color, 2.f);
	}

};  static ue* ue5 = new ue();



#define debug_weapons false
