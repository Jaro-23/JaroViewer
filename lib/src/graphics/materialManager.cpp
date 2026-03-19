#include "graphics/materialManager.hpp"
#include "graphics/texture2D.hpp"

using namespace JaroViewer;

MaterialManager::MaterialManager()
  : mMaterials(), mTextures(), mLastMaterial(0), mLastShader(nullptr) {}

uint MaterialManager::createNew() {
	mMaterials.push_back(std::vector<Material>());
	return mMaterials.size();
}

void MaterialManager::addMaterial(uint ident, const MaterialArgs& args) {
	if (ident == 0) return;
	if (!mTextures.contains(args.diffusePath))
		mTextures[args.diffusePath] = std::make_shared<Texture2D>(args.diffusePath);
	if (!mTextures.contains(args.specularPath))
		mTextures[args.specularPath] = std::make_shared<Texture2D>(args.specularPath);

	mMaterials.at(ident - 1).push_back(
	  Material(*mTextures[args.diffusePath], *mTextures[args.specularPath], args.shininess)
	);
}

void MaterialManager::addMaterial(uint ident, const ColorMaterialArgs& args) {
	if (ident == 0) return;
	mMaterials.at(ident - 1).push_back(Material(args));
}

void MaterialManager::loadMaterial(Shader* shader, uint material, uint offset) {
	if (material == 0 || (mLastShader == shader && mLastMaterial == material))
		return;
	mLastShader                 = shader;
	mLastMaterial               = material;
	std::vector<Material>& mats = mMaterials.at(material - 1);

	shader->setInt("numTextures", mats.size());
	for (unsigned int i = 0; i < mats.size(); i++)
		mats.at(i).loadIntoArray(shader, i, offset);
}
