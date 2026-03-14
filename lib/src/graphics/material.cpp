#include "graphics/material.hpp"

#include <memory>
#include <string>

using namespace JaroViewer;

/**
 * Creates a material based of 2 textures and a shininess value
 * @param diffuse The diffuse texture that forms the basic color of each fragment
 * @param specular The specular texture that forms the highlight colors of the fragment
 * @param shininess The shininess of the material
 */
Material::Material(const Texture2D& diffuse, const Texture2D& specular, float shininess)
  : mDiffuse{diffuse}, mSpecular{specular} {
	mShininess = shininess;

	mVariableName = "material";
	mArrayName    = "materials";
}

Material::Material(const MaterialArgs& args)
  : Material(Texture2D(args.diffusePath), Texture2D(args.specularPath), args.shininess) {}

Material::Material(const ColorMaterialArgs& args)
  : Material(Texture2D(args.diffuse), Texture2D(args.specular), args.shininess) {}

/**
 * Loads the material in a variable set by the variable name
 * @param shader The shader you want to set the material to
 * @pre shader != nullptr
 */
void Material::load(const Shader* shader) const {
	// Setup the shader
	shader->setInt(mVariableName + ".diffuse", 0);
	shader->setInt(mVariableName + ".specular", 1);
	shader->setFloat1(mVariableName + ".shininess", mShininess);

	// Load the textures
	mDiffuse.bind(0);
	mSpecular.bind(1);
}

/**
 * Loads the material in an array set by the array name
 * @param shader The shader that contains the array
 * @param arrayIndex The index where the materials will be placed
 */
void Material::loadIntoArray(const Shader* shader, int arrayIndex) const {
	// Setup the shader
	std::string base = (mArrayName + "[") + std::to_string(arrayIndex) + "]";
	int baseIndex    = arrayIndex * 2;
	shader->setInt(base + ".diffuse", baseIndex);
	shader->setInt(base + ".specular", baseIndex + 1);
	shader->setFloat1(base + ".shininess", mShininess);

	// Load the textures
	mDiffuse.bind(baseIndex);
	mSpecular.bind(baseIndex + 1);
}
