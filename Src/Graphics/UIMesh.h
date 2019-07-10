#ifndef UIMESH_H_INCLUDED
#define UIMESH_H_INCLUDED

#include <vector>
#include <Math/Rectangle.h>

#include "../Wrap/Mesh.h"
#include "../Wrap/Material.h"

class UIMesh;

enum class Alignment {
    CenterXY = 0x0,
    Left = 0x1,
    Right = 0x2,
    Top = 0x4,
    Bottom = 0x8
};

class UIMesh {
    private:
        //TODO: store transformation matrix constants

        Shader shaderTextured;
        PGE::Shader::Constant* shaderTexturedColorConstant;

        Shader shaderTextureless;
        PGE::Shader::Constant* shaderTexturelessColorConstant;

        Mesh mesh;
        Material material;
        PGE::Color color;

        // Whether or not the texture applied to this mesh is meant to tile.
        bool tiled;
        // Whether the mesh has a texture or just a color fill.
        bool textureless;

        bool startedRender;

        std::vector<PGE::Vertex> vertices;
        std::vector<PGE::Primitive> primitives;
    public:
        UIMesh(const Graphics& gfx, const Shader& shdTextured, const Shader& shdTextureless);

        PGE::Vector2f scaleFactor;
        PGE::Rectanglef uvTilingRectangle;

        void startRender();

        void setTextureless();
        void setTextured(const Texture& texture, bool tile);

        void setColor(PGE::Color col);

        void addRect(const PGE::Rectanglef& rect);

        void endRender();
};

const Alignment operator&(const Alignment& a, const Alignment& b);
const Alignment operator|(const Alignment& a, const Alignment& b);

#endif // UIMESH_H_INCLUDED
