#include "InputLayout.h"

InputLayout::InputLayout()
{
#if defined(DX11)
	m_pInputLayout = NULL;
#endif
}

InputLayout::~InputLayout()
{
}

#if defined(DX11)
void InputLayout::DefineInputLayout(D3D11_SHADER_DESC _ShaderDesc, ID3D11ShaderReflection* _ShaderReflection)
{
	//m_Layout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	//m_Layout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	//m_Layout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	for (UINT i = 0; i < _ShaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC ParamDesc;
		_ShaderReflection->GetInputParameterDesc(i, &ParamDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC ElementDesc;
		ElementDesc.SemanticName = _strdup(ParamDesc.SemanticName);
		//ElementDesc.SemanticName = ParamDesc.SemanticName;
		ElementDesc.SemanticIndex = ParamDesc.SemanticIndex;
		ElementDesc.InputSlot = 0;
		ElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		ElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ElementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (ParamDesc.Mask == 1)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32_UINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32_SINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
		}
		else if (ParamDesc.Mask <= 3)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
		}
		else if (ParamDesc.Mask <= 7)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
		}
		else if (ParamDesc.Mask <= 15)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			}
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		}
		//if (i == 0)
		//{
		//	ElementDesc.AlignedByteOffset = 0;
		//}
		//else if (i == 1)
		//{
		//	ElementDesc.AlignedByteOffset = 12;
		//}
		//else if (i == 2)
		//{
		//	ElementDesc.AlignedByteOffset = 20;
		//}
		//save element desc
		m_vInputLayoutDesc.push_back(ElementDesc);
	}
}
#endif