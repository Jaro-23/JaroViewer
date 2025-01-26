# Layout

## Objects structure

* AssetManager <<Singelton>>
    + Asset <<Interface>> {string getKey(map<string, string>), void load(map<string, string>), bool isReady() const }
        * Shader
        * Material
        * Texture
        * Mesh
        * Cubemap
* Engine
* Light
    + PointLight
    + DirectionalLight
    + Spotlight
