#include <geometry/mesh.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>

using namespace std::string_literals;

void Mesh::loadOBJ(const std::string_view filename)
{
    clear();

    std::ifstream file{filename.data()};
    file.exceptions(std::ios::badbit);
    std::string buffer;

    if (!file)
        throw std::runtime_error("failed to open the OBJ file "s + filename.data()
                                 + "\nmake sure you run the program in the correct folder!"s);

    std::vector<Normal3D> objNormals;
    std::vector<Point2D> objTexCoords;
    std::vector<std::optional<TriangleIndices>> normalIndices, textureIndices;

    uint32_t currentSmoothGroupStart = -1U;
    uint32_t currentSmoothGroupIndex = -1U;

    while (file) {
        std::getline(file, buffer);

        if (buffer.empty() || buffer.starts_with("#"))
            continue;

        std::istringstream in{buffer};
        char type, subtype;
        in >> std::noskipws >> type >> subtype >> std::skipws;

        auto check = [&]() -> void {
            if (!in)
                throw std::runtime_error("failed to parse the OBJ file "s + filename.data()
                                         + "\ncurrent line:\n"s + buffer);
        };

        if (type == 'v') {
            if (subtype == 't') {
                Point2D vt;
                in >> vt.x >> vt.y;
                check();

                objTexCoords.push_back(vt);
            }
            else if (subtype == ' ' || subtype == '\t' || subtype == 'n') {
                Point3D v;
                in >> v.x >> v.y >> v.z;
                check();

                if (subtype == 'n') {
                    objNormals.push_back(v);
                }
                else {
                    aabb.extend(v);
                    vertices.push_back(v);
                }
            }
        }
        else if (type == 'f') {
            TriangleIndices t, tn, tt;

            bool hasNormal{true}, hasTexture{true};

            auto readIndices = [&](uint32_t& v, uint32_t& vt, uint32_t& vn) -> void {
                in >> v;
                --v;

                if (in) {
                    // texture coordinate and vertex normal id (currently unused)
                    if (in.peek() == '/') {
                        in.ignore();
                        if (in.peek() != '/') {
                            in >> vt;
                            if (in)
                                --vt;
                            else
                                hasTexture = false;
                        }
                        else
                            hasTexture = false;
                        if (in.peek() == '/') {
                            in.ignore();
                            in >> vn;
                            if (in)
                                --vn;
                            else
                                hasNormal = false;
                        }
                        else
                            hasNormal = false;
                    }
                    else
                        hasNormal = hasTexture = false;
                    in.clear();
                }
            };

            readIndices(t.v1, tt.v1, tn.v1);
            readIndices(t.v2, tt.v2, tn.v2);
            readIndices(t.v3, tt.v3, tn.v3);
            check();

            while (in) {
                if (hasNormal) {
                    if (normalIndices.size() < faces.size())
                        normalIndices.resize(faces.size());
                    normalIndices.push_back(tn);
                }
                if (hasTexture) {
                    if (textureIndices.size() < faces.size())
                        textureIndices.resize(faces.size());
                    textureIndices.push_back(tt);
                }
                faces.push_back(t);

                t.v2 = t.v3;
                tn.v2 = tn.v3;
                tt.v2 = tt.v3;
                readIndices(t.v3, tt.v3, tn.v3);
            }
        }
        else if (type == 's') {
            uint32_t smoothGroupIndex = -1U;
            in >> smoothGroupIndex;
            if (smoothGroupIndex != currentSmoothGroupIndex) {
                // end current smooth group
                if (faces.size() > currentSmoothGroupStart)
                    smoothGroups.emplace_back(currentSmoothGroupStart, faces.size());
                if (in) {
                    currentSmoothGroupStart = static_cast<uint32_t>(faces.size());
                    currentSmoothGroupIndex = smoothGroupIndex;
                }
            }
            if (!in) {
                currentSmoothGroupStart = -1U;
                currentSmoothGroupIndex = -1U;
            }
        }
    }

    if (faces.size() > currentSmoothGroupStart)
        smoothGroups.emplace_back(currentSmoothGroupStart, faces.size());

    file.close();

    std::vector<bool> usedVertices(vertices.size());

    // compute smooth normals
    {
        normals.resize(vertices.size());
        std::vector<float> weights(vertices.size());

        for (auto [start, end] : smoothGroups) {
            for (size_t i = start; i < end; ++i) {
                TriangleIndices& t = faces.at(i);

                const Vector3D v1v2 = normalize(vertices.at(t.v2) - vertices.at(t.v1));
                const Vector3D v1v3 = normalize(vertices.at(t.v3) - vertices.at(t.v1));
                const Vector3D v2v3 = normalize(vertices.at(t.v3) - vertices.at(t.v2));
                const Normal3D up = cross(v1v2, v1v3);

                // weight = angle covered by the triangle
                const float w1 = std::acos(dot(v1v2, v1v3));
                const float w2 = std::acos(dot(v1v2, v2v3));
                const float w3 = std::acos(dot(v2v3, v1v3));

                weights.at(t.v1) += w1;
                weights.at(t.v2) += w2;
                weights.at(t.v3) += w3;

                if (i < normalIndices.size() && normalIndices.at(i)) {
                    const TriangleIndices& tn = *normalIndices.at(i);
                    normals.at(t.v1) += objNormals.at(tn.v1) * w1;
                    normals.at(t.v2) += objNormals.at(tn.v2) * w2;
                    normals.at(t.v3) += objNormals.at(tn.v3) * w3;
                }
                else {
                    normals.at(t.v1) += up * w1;
                    normals.at(t.v2) += up * w2;
                    normals.at(t.v3) += up * w3;
                }

                usedVertices.at(t.v1) = true;
                usedVertices.at(t.v2) = true;
                usedVertices.at(t.v3) = true;
            }
        }
    }
    // compute flat normals and texture coordinates, also duplicate some shared vertices
    {
        float faceAreaSum = 0.0f;
        faceAreas.resize(faces.size());
        faceAreaPrefixSum.resize(faces.size());

        const bool hasTexCoords = !objTexCoords.empty();
        std::vector<std::optional<uint32_t>> texCoordPerVertex;
        if (hasTexCoords) {
            texCoords.resize(vertices.size());
            texCoordPerVertex.resize(vertices.size());
        }

        bool shadeFlat = true;
        auto currentSmoothGroup = smoothGroups.cbegin();

        auto copyVertex = [&](uint32_t index) -> uint32_t {
            const uint32_t copyIndex = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertices.at(index));
            usedVertices.push_back(false);
            normals.push_back(normals.at(index));
            if (hasTexCoords) {
                texCoords.push_back(texCoords.at(index));
                texCoordPerVertex.push_back(texCoordPerVertex.at(index));
            }
            return copyIndex;
        };

        for (size_t i = 0; i < faces.size(); ++i) {
            if (currentSmoothGroup != smoothGroups.end()) {
                if (i == currentSmoothGroup->first)
                    shadeFlat = false;
                if (i == currentSmoothGroup->second) {
                    shadeFlat = true;
                    ++currentSmoothGroup;
                }
            }

            TriangleIndices& t = faces.at(i);
            Vector3D v1v2 = vertices.at(t.v2) - vertices.at(t.v1);
            Vector3D v1v3 = vertices.at(t.v3) - vertices.at(t.v1);
            const Vector3D upTimes2Area = cross(v1v2, v1v3);
            const float area = upTimes2Area.norm() * 0.5f;
            faceAreas.at(i) = area;
            faceAreaSum += area;
            faceAreaPrefixSum.at(i) = faceAreaSum;

            if (shadeFlat) {
                // duplicate reused vertices if they are used to set the normal for flat shading
                if (usedVertices.at(t.v3))
                    t.v3 = copyVertex(t.v3);
                usedVertices.at(t.v3) = true;

                // will be normalized later
                normals.at(t.v3) = upTimes2Area;
            }
            if (i < textureIndices.size() && textureIndices.at(i)) {
                const TriangleIndices& tt = *textureIndices.at(i);
                // duplicate reused vertices if they have a different texture coordinate
                if (texCoordPerVertex.at(t.v1) && *texCoordPerVertex.at(t.v1) != tt.v1)
                    t.v1 = copyVertex(t.v1);
                else
                    texCoordPerVertex.at(t.v1) = tt.v1;

                if (texCoordPerVertex.at(t.v2) && *texCoordPerVertex.at(t.v2) != tt.v2)
                    t.v2 = copyVertex(t.v2);
                else
                    texCoordPerVertex.at(t.v2) = tt.v2;

                if (texCoordPerVertex.at(t.v3) && *texCoordPerVertex.at(t.v3) != tt.v3)
                    t.v3 = copyVertex(t.v3);
                else
                    texCoordPerVertex.at(t.v3) = tt.v3;

                texCoords.at(t.v1) = objTexCoords.at(tt.v1);
                texCoords.at(t.v2) = objTexCoords.at(tt.v2);
                texCoords.at(t.v3) = objTexCoords.at(tt.v3);
            }
        }
        // normalize all normals
        for (uint32_t i = 0; i < normals.size(); ++i)
            normals.at(i) = normalize(normals.at(i));
        // compute inverse total area for area light sampling
        invTotalArea = 1.0f / faceAreaSum;
    }

    std::cout << "Loaded OBJ file: " << filename << " containing " << vertices.size()
              << " vertices, " << objNormals.size() << " vertex normals, " << objTexCoords.size()
              << " texture coordinates, and " << faces.size() << " faces." << std::endl;

    bvh.construct(*this);
}

void Mesh::updateBounds()
{
    aabb = {};
    for (const auto& vertex : vertices) {
        aabb.extend(vertex);
    }
}

std::pair<Point3D, Normal3D> Mesh::computePointAndNormal(uint32_t faceIndex,
                                                         const BarycentricCoordinates& bary) const
{
    const TriangleIndices& face = faces.at(faceIndex);
    const Triangle& triangle = getTriangleFromFace(face);
    const TriangleNormals& triangleNormals = getTriangleNormalsFromFace(face);

    const Point3D p = bary.interpolate(triangle);
    const Normal3D n = isSmoothFace(faceIndex) ? bary.interpolate(triangleNormals)
                                               : cross(triangle.v1v2, triangle.v1v3);

    return {p, n};
}

std::pair<Point3D, Normal3D> Mesh::samplePointAndNormal(Point2D sample) const
{
    // find the bin of this sample using binary search (meshes might be huge)
    const auto it = std::lower_bound(faceAreaPrefixSum.cbegin(), faceAreaPrefixSum.cend(),
                                     sample.x * faceAreaPrefixSum.back());
    const uint32_t faceIndex = static_cast<uint32_t>(std::distance(faceAreaPrefixSum.cbegin(), it));
    // reuse the sample
    {
        if (faceIndex)
            sample.x -= *(it - 1) * invTotalArea;
        sample.x /= *it * invTotalArea;
    }

    BarycentricCoordinates bary{sample.x, sample.y};
    if (bary.lambda2 + bary.lambda3 > 1.0f) {
        bary.lambda2 = 1.0f - bary.lambda2;
        bary.lambda3 = 1.0f - bary.lambda3;
    }

    return computePointAndNormal(faceIndex, bary);
}
