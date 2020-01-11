#include "Mesh.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>
#if defined(_WIN32) | defined(_WIN64)
#include <execution>
#endif
#include <assert.h>

#include "VectorMath.h"

namespace SoftwareGL {

	bool SoftwareGL::Mesh::LoadFromObj(const std::string& path)
	{
		std::ifstream ifs;
		ifs.open(path, std::ifstream::in);
		if (!ifs.is_open())	return false;
		while (!ifs.eof()) {
			std::string line = "";
			if (!std::getline(ifs, line)) break;
			if (line == "") continue;
			std::istringstream iss(line);
			std::string dump;
			if (!(iss >> dump))	return false;
			if (dump.size() > 2) return false;
			switch (dump[0]) {
			case 'v':
			{
				if (dump.size() > 1) 
				{
					switch (dump[1])
					{
					case 'n':
					{
						assert(dump == "vn");
						VectorMath::vector4 v(0, 0, 0, 1);
						if (!(iss >> v.x)) return false;
						if (!(iss >> v.y)) return false;
						if (!(iss >> v.z)) return false;
						normals_.push_back(v);
						break;
					}
					case 't':
					{
						assert(dump == "vt");
						VectorMath::vector3 v(0, 0, 1);
						if (!(iss >> v.x)) return false;
						if (!(iss >> v.y)) return false;
						textures_.push_back(v);
						break;
					}
					default:
						return false;
					}
				}
				else
				{
					if (dump != "v") return false;
					VectorMath::vector4 v(0, 0, 0, 1);
					if (!(iss >> v.x)) return false;
					if (!(iss >> v.y)) return false;
					if (!(iss >> v.z)) return false;
					positions_.push_back(v);
				}
			}
				break;
			case 'f':
			{
				if (dump != "f") return false;
				for (int i = 0; i < 3; ++i)
				{
					std::string inner;
					if (!(iss >> inner)) return false;
					std::array<int, 3> vi;
					std::istringstream viss(inner);
					for (int& i : vi)
					{
						std::string inner_val;
						std::getline(viss, inner_val, '/');
						if (inner_val.size() > 0)
						{
							i = atoi(inner_val.c_str()) - 1;
						}
						else
						{
							i = -1;
						}
					}
					indices_.push_back(vi);
				}
			}
				break;
			default:
				break;
			}
		}
		if (indices_.size() % 3 != 0) return false;
		return true;
	}

	inline const std::vector<VectorMath::vector4>& Mesh::GetPositions() const
	{
		return positions_;
	}

	const std::vector<VectorMath::vector4>& Mesh::GetNormals() const
	{
		return normals_;
	}

	const std::vector<VectorMath::vector3>& Mesh::GetTextures() const
	{
		return textures_;
	}

	const std::vector<std::array<int, 3>>& Mesh::GetIndices() const
	{
		return indices_;
	}

	void Mesh::AllPositionMatrixMult(const VectorMath::matrix& matrix)
	{
		std::for_each(
#if defined(_WIN32) | defined(_WIN64)
			std::execution::par,
#endif
			positions_.begin(), 
			positions_.end(), 
			[&matrix](VectorMath::vector4& vec) 
		{
			vec = VectorMath::VectorMult(vec, matrix);
		});
	}

	void Mesh::AllNormalMatrixMult(const VectorMath::matrix& matrix)
	{
		std::for_each(
#if defined(_WIN32) | defined(_WIN64)
			std::execution::par,
#endif
			normals_.begin(),
			normals_.end(),
			[&matrix](VectorMath::vector4& vec)
		{
			vec = VectorMath::VectorMult(vec, matrix);
		});
	}

	void Mesh::AllPositionDivideByW()
	{
		std::for_each(
#if defined(_WIN32) | defined(_WIN64)
			std::execution::par,
#endif
			positions_.begin(),
			positions_.end(),
			[](VectorMath::vector4& vec)
		{
			vec *= 1 / vec.w;
		});
	}

	void Mesh::AllTextureDivideByZ()
	{
		std::for_each(
#if defined(_WIN32) | defined(_WIN64)
			std::execution::par,
#endif
			textures_.begin(),
			textures_.end(),
			[](VectorMath::vector3& vec)
		{
			vec *= 1 / vec.z;
		});
		std::for_each(
			indices_.begin(),
			indices_.end(),
			[this](const std::array<int, 3>& indices)
		{
			float w = positions_[indices[0]].w;
			textures_[indices[1]].z = 1.f / w;
		});
	}

	void Mesh::AllPositionAdd(const float f)
	{
		std::for_each(
#if defined(_WIN32) | defined(_WIN64)
			std::execution::par,
#endif
			positions_.begin(),
			positions_.end(),
			[&f](VectorMath::vector4& vec)
		{
			vec += f;
		});
	}

	void Mesh::AllPositionMult(const float f)
	{
		std::for_each(
#if defined(_WIN32) | defined(_WIN64)
			std::execution::par,
#endif
			positions_.begin(),
			positions_.end(),
			[&f](VectorMath::vector4& vec)
		{
			vec *= f;
		});
	}

	void Mesh::AllPositionMult(const VectorMath::vector4& v)
	{
		std::for_each(
#if defined(_WIN32) | defined(_WIN64)
			std::execution::par,
#endif
			positions_.begin(),
			positions_.end(),
			[&v](VectorMath::vector4& vec)
		{
			vec |= v;
		});
	}

	Mesh::ConstIterator::ConstIterator(int position, const Mesh& mesh) :
		position_(position), mesh_(mesh)
	{
		SetLocalTriangle();
	}

	Mesh::ConstIterator& Mesh::ConstIterator::operator++() {
		position_ += 3;
		if (position_ < mesh_.GetIndices().size())
		{
			SetLocalTriangle();
		}
		else
		{
			position_ = static_cast<int>(mesh_.GetIndices().size());
		}
		return *this;
	}

	bool Mesh::ConstIterator::operator!=(const ConstIterator& it) const
	{
		if (position_ != it.position_) return true;
		if (mesh_ != it.mesh_) return true;
		return false;
	}

	void Mesh::ConstIterator::SetLocalTriangle()
	{
		// Check for the end.
		if (position_ >= mesh_.GetIndices().size())
		{
			return;
		}
		auto i0 = mesh_.GetIndices()[position_];
		auto i1 = mesh_.GetIndices()[position_ + 1];
		auto i2 = mesh_.GetIndices()[position_ + 2];
		Vertex v1;
		Vertex v2;
		Vertex v3;
		v1.SetPosition(mesh_.GetPositions()[i0[0]]);
		v2.SetPosition(mesh_.GetPositions()[i1[0]]);
		v3.SetPosition(mesh_.GetPositions()[i2[0]]);
		if ((i0[1] != -1) && (i1[1] != -1) && (i2[1] != -1))
		{
			v1.SetTexture(mesh_.GetTextures()[i0[1]]);
			v2.SetTexture(mesh_.GetTextures()[i1[1]]);
			v3.SetTexture(mesh_.GetTextures()[i2[1]]);
		}
		if ((i0[2] != -1) && (i1[2] != -1) && (i2[2] != -1))
		{
			v1.SetNormal(mesh_.GetNormals()[i0[2]]);
			v2.SetNormal(mesh_.GetNormals()[i1[2]]);
			v3.SetNormal(mesh_.GetNormals()[i2[2]]);
		}
		triangle_ = std::make_unique<Triangle>(v1, v2, v3);
	}

	bool Mesh::operator!=(const Mesh& mesh) const
	{
		if (positions_ != mesh.positions_) return true;
		if (normals_ != mesh.normals_) return true;
		if (textures_ != mesh.textures_) return true;
		if (indices_ != mesh.indices_) return true;
		return false;
	}

}	// End namespace SoftwareGL.
