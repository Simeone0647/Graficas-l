#pragma once
class Vertex
{
public:
	Vertex();
	Vertex(float posx, float posy, float posz, float normalx, float normaly, float normalz, float texturex, float texturey);
	~Vertex();

	/*
		* @Function Name: SetPosition
		* @brief...This function will set the vertex position.
		* @param...#float: X position value.
		* @param...#float: Y position value.
		* @param...#float: Z position value.
		* @bug.....No known bugs.
	*/
	inline void SetPosition(float x, float y, float z) { m_Position[0] = x; m_Position[1] = y; m_Position[2] = z; }

	/*
		* @Function Name: SetNormal
		* @brief...This function will set the vertex normal.
		* @param...#float: X normal value.
		* @param...#float: Y normal value.
		* @param...#float: Z normal value.
		* @bug.....No known bugs.
	*/
	inline void SetNormal(float x, float y, float z) { m_Normal[0] = x; m_Normal[1] = y; m_Normal[2] = z; }

	/*
		* @Function Name: SetTexture
		* @brief...This function will set the vertex texture.
		* @param...#float:X texture value.
		* @param...#float:Y texture value.
		* @bug.....No known bugs.
	*/
	inline void SetTexture(float x, float y) { m_Texture[0] = x; m_Texture[1] = y; }

	/*
		* @Function Name: GetSize
		* @brief...This function will return the class size.
		* @bug.....No known bugs.
		* @return..#int: The class size in bytes
	*/

	inline void SetTangent(const float _x, const float _y, const float _z) {m_Tangents[0] = _x; m_Tangents[1] = _y; m_Tangents[2] = _z; }

	inline void SetBinormal(const float _x, const float _y, const float _z) { m_Binormal[0] = _x; m_Binormal[1] = _y; m_Binormal[2] = _z; }

	inline int GetSize() { return sizeof(m_Position) + sizeof(m_Texture) + sizeof(m_Normal) + sizeof(m_Tangents) + sizeof(m_Binormal); }

private:
	/*
		* @Variable Name: m_Position
		* @Type....#float
		* @brief...The x, y, and z values that represent the vertex position.
	*/
	float m_Position[4];

	/*
	* @Variable Name: m_Normals
	* @Type....#float
	* @brief...The x, y and z values that represent the vertex normals.
	*/
	float m_Normal[4];

	float m_Binormal[4];

	float m_Tangents[4];

	/*
		* @Variable Name: m_Texture
		* @Type....#float
		* @brief...The x and y values that represent the vertex texture.
	*/
	float m_Texture[2];

	//float m_BoneIndex[4];
	//
	//float m_BoneWeight[4];

	//float m_UV[3];
};

