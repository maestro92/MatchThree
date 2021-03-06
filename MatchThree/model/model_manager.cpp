#include "model_manager.h"

/*

when working with models in blender
the -z axis in game is the y axis in blender, 

so to be sure to orient the models in the right direction


*/

ModelManager::ModelManager()
{

}

ModelManager::~ModelManager()
{

}

void ModelManager::init()
{
	m_quad = new QuadModel(1, 1);
	m_centeredQuad = new QuadModel(-0.5, 0.5, -0.5, 0.5);
	m_xyzAxis = new XYZAxisModel();

	m_models.resize(ModelEnum::NUM_MODELS);
	m_models[ModelEnum::quad] = m_quad;
	m_models[ModelEnum::centeredQuad] = m_centeredQuad;
	m_models[ModelEnum::xyzAxis] = m_xyzAxis;
}

void ModelManager::shutDown()
{


}

Model* ModelManager::get(int modelEnum)
{
	return m_models[modelEnum];
}

// http://strike-counter.com/cs-go-stats/weapons-stats
