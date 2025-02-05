# Layout

## Objects structure

* AssetManager <<Singelton>>
    + AssetParameter = map<string, vector<string> | string>
    + Asset <<Interface>> {static string getKey(AssetParameter) const, void load(AssetParameter), bool isReady() const}
        * Shader
        * Material
        * Texture
        * Model
        * Cubemap
* Engine
* InputManager
* Light
    + PointLight
    + DirectionalLight
    + Spotlight
* GameObject <<Impure interface>>
    + Management of Gamobject parent child relation
    + Basic translation, rotation and scaling
    + Basic rendering

# Asset key generation
## Options for automatic generation
* HTTP as inspiration
    + Format: Type?\[key=value for each key value pair in map\]
    + Pro: Automatic so no need to think for each object
    + Con:
        * Type must be unique and maybe needs to give a warning if not satisfied
        * Asset not a pure interface anymore

