#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"

namespace TakeTwo
{

	//TODO
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texCoord;

		enum VertexFormat : unsigned int
		{
			POSITION,
			NORMAL,
			COLOR,
			TEXCOORD,

			Count
		};
	};

	struct PDBuffer
	{
		PDBuffer(): m_buffer(0) {};
		~PDBuffer() {	glDeleteBuffers(1, &m_buffer); }
		GLuint m_buffer;
	};


	enum BlendOption
	{
		BLEND_OPTION_INVALID = -1,

		BLEND_OPTION_ZERO = GL_ZERO,
		BLEND_OPTION_ONE = GL_ONE,

		BLEND_OPTION_SRC_COLOUR = GL_SRC_COLOR,
		BLEND_OPTION_INV_SRC_COLOUR = GL_ONE_MINUS_SRC_COLOR,

		BLEND_OPTION_SRC_ALPHA = GL_SRC_ALPHA,
		BLEND_OPTION_INV_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,

		BLEND_OPTION_DEST_ALPHA = GL_DST_ALPHA,
		BLEND_OPTION_INV_DEST_ALPHA = GL_ONE_MINUS_DST_ALPHA,

		BLEND_OPTION_DEST_COLOUR = GL_DST_COLOR,
		BLEND_OPTION_INV_DEST_COLOUR = GL_ONE_MINUS_DST_COLOR,

		BLEND_OPTION_SRC_ALPHA_SAT = GL_SRC_ALPHA_SATURATE,

		BLEND_OPTION_BLEND_FACTOR = BLEND_OPTION_INVALID - 1,
		BLEND_OPTION_INV_BLEND_FACTOR = BLEND_OPTION_INVALID - 2,

		BLEND_OPTION_SRC1_COLOR = BLEND_OPTION_INVALID - 3,
		BLEND_OPTION_INV_SRC1_COLOUR = BLEND_OPTION_INVALID - 4,

		BLEND_OPTION_SRC1_ALPHA = BLEND_OPTION_INVALID - 5,
		BLEND_OPTION_INV_SRC1_ALPHA = BLEND_OPTION_INVALID - 6,
	};

	enum PrimitiveType
	{
		PRIM_TYPE_INVALID = -1,
		PRIM_TYPE_POINTS = GL_POINTS,
		PRIM_TYPE_LINES = GL_LINES,
		PRIM_TYPE_LINE_STRIP = GL_LINE_STRIP,
		PRIM_TYPE_LINE_LOOP = GL_LINE_LOOP,
		PRIM_TYPE_TRIANGLES = GL_TRIANGLES,
		PRIM_TYPE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		PRIM_TYPE_TRIANGLE_FAN = GL_TRIANGLE_FAN,
		PRIM_TYPE_QUADS = GL_QUADS,
		PRIM_TYPE_QUAD_STRIP = GL_QUAD_STRIP,
		PRIM_TYPE_POLYGON = GL_POLYGON,
	};

	enum BlendOperation
	{
		BLEND_OPERATION_INVALID = -1,
		BLEND_OPERATION_ADD = GL_FUNC_ADD,
		BLEND_OPERATION_SUBTRACT = GL_FUNC_SUBTRACT,
		BLEND_OPERATION_REV_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
		BLEND_OPERATION_MIN = GL_MIN,
		BLEND_OPERATION_MAX = GL_MAX,
	};

	enum ComparisonMode
	{
		COMPARISON_INVALID = 0,

		COMPARISON_NEVER = GL_NEVER,
		COMPARISON_ALWAYS = GL_ALWAYS,

		COMPARISON_LESS = GL_LESS,
		COMPARISON_LESSEQUAL = GL_LEQUAL,

		COMPARISON_EQUAL = GL_EQUAL,
		COMPARISON_NOTEQUAL = GL_NOTEQUAL,

		COMPARISON_GREATER = GL_GREATER,
		COMPARISON_GREATEREQUAL = GL_GEQUAL,
	};
	enum StencilOp
	{
		STENCIL_OP_KEEP = GL_KEEP,
		STENCIL_OP_ZERO = GL_ZERO,
		STENCIL_OP_REPLACE = GL_REPLACE,
		STENCIL_OP_INCR_SAT = GL_INCR,
		STENCIL_OP_DECR_SAT = GL_DECR,
		STENCIL_OP_INCR = GL_INCR,
		STENCIL_OP_DECR = GL_DECR,
	};
	enum TextureType
	{
		TEXTURE_TYPE_INVALID = 0,
		TEXTURE_TYPE_1D = TEXTURE_TYPE_INVALID,
		TEXTURE_TYPE_2D = GL_TEXTURE_2D,
		TEXTURE_TYPE_3D = GL_TEXTURE_3D,
		TEXTURE_TYPE_CUBE = GL_TEXTURE_CUBE_MAP,
	};

	enum DepthWriteMask
	{
		DEPTH_WRITE_MASK_INVALID = -1,
		DEPTH_WRITE_MASK_ZERO = 0,
		DEPTH_WRITE_MASK_ALL = 1,
	};


	enum TextureFilteringMode
	{
		TEXTURE_FILTER_INVALID = -1,
		TEXTURE_FILTER_NONE,
		TEXTURE_FILTER_POINT = GL_NEAREST,
		TEXTURE_FILTER_LINEAR = GL_LINEAR,

		TEXTURE_FILTER_LINEAR_MIPMAP_POINT = GL_LINEAR_MIPMAP_NEAREST,
		TEXTURE_FILTER_POINT_MIPMAP_POINT = GL_NEAREST_MIPMAP_NEAREST,

		TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		TEXTURE_FILTER_POINT_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,

		TEXTURE_FILTER_ANISOTROPIC,

	};
	enum TextureMappingMode
	{
		TEXTURE_MAP_INVALID = 0,
		TEXTURE_MAP_WRAP = GL_REPEAT,
		TEXTURE_MAP_MIRROR = GL_MIRRORED_REPEAT,
		TEXTURE_MAP_CLAMP = GL_CLAMP_TO_EDGE,
		TEXTURE_MAP_BORDER = GL_CLAMP_TO_BORDER,
		TEXTURE_MAP_MIRROR_ONCE = GL_MIRROR_CLAMP_TO_BORDER_EXT,
	};

	enum CubeMapFaceID
	{
		IMAGE_CUBE_MAP_FACE_INVALID = -1,

		IMAGE_CUBE_MAP_FACE_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		IMAGE_CUBE_MAP_FACE_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		IMAGE_CUBE_MAP_FACE_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		IMAGE_CUBE_MAP_FACE_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		IMAGE_CUBE_MAP_FACE_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		IMAGE_CUBE_MAP_FACE_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,

		IMAGE_CUBE_MAP_FACE_FORCE_DWORD = 0x7fffffff,
	};

}

