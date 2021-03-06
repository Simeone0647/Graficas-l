#pragma once
class Vertex
{
public:
	Vertex();
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
	inline int GetSize() { return sizeof(m_Position) + sizeof(m_Texture); }

private:
	/*
		* @Variable Name: m_Position
		* @Type....#float
		* @brief...The x, y, and z values that represent the vertex position.
	*/
	float m_Position[3];

	/*
		* @Variable Name: m_Texture
		* @Type....#float
		* @brief...The x and y values that represent the vertex texture.
	*/
	float m_Texture[2];
};

