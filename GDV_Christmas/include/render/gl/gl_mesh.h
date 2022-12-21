#ifndef GL_MESH_H
#define GL_MESH_H

#include <geometry/mesh.h>

#include "gl_utils.h"

#include <vector>
#include <utility>
#include <cstdint>

struct GLMesh {
    GLMesh(const Mesh& mesh) {
        vertexBuffer.setBuffer<Point3D>("position", mesh.getVertices());
        vertexBuffer.setBuffer<Normal3D>("normal", mesh.getNormals());
        vertexBuffer.setBuffer<TriangleIndices>("triangles", mesh.getFaces());
        hasTexCoords = mesh.getTextureCoordinates().size();
        if (hasTexCoords)
            vertexBuffer.setBuffer<Point2D>("texCoords", mesh.getTextureCoordinates());

        numTriangles = static_cast<uint32_t>(mesh.getFaces().size());
        smoothGroups = mesh.getSmoothGroups();
    }

    void drawFlatParts(GLenum mode=GL_TRIANGLES) const {
        size_t pos = 0;
        for (auto [start, end] : smoothGroups) {
            if (start > pos)
                vertexBuffer.draw("triangles", mode, pos, start);
            pos = end;
        }
        vertexBuffer.draw("triangles", mode, pos, numTriangles);
    }

    void drawSmoothParts(GLenum mode=GL_TRIANGLES) const {
        for (auto [start, end] : smoothGroups)
            vertexBuffer.draw("triangles", mode, start, end);
    }

    void drawAll(GLenum mode=GL_TRIANGLES) const {
        vertexBuffer.draw("triangles", mode, 0, numTriangles);
    }

    std::vector<std::pair<uint32_t, uint32_t>> smoothGroups;
    uint32_t numTriangles{0};
    bool hasTexCoords{false};

    GLVertexBuffer vertexBuffer{};
};

#endif // GL_MESH_H
