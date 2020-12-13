#include "shape.h"
#include "utils/error.h"

/**
 * \brief Add trianngles to shape function
 * \param[in] NewTriangles New triangles
 */
VOID shape::AddTriangles( const std::vector<triangle> &NewTriangles )
{
  Triangles.insert(Triangles.end(), NewTriangles.begin(), NewTriangles.end());
}

/**
 * \brief Load *.obj file and add triangles function
 * \param[in] FileName Name of file
 * \param[in] MtlId Material identificator
 * \param[in] EnviId Environment identificator
 * \param[in] BaseTransform Transformation for model
 */
VOID shape::LoadOBJ( const std::string &FileName, const INT MtlId,
                     const INT EnviId, const matr &BaseTransform )
{
  struct OBJ_VERTEX_INDEX
  {
    INT
      VertexIndex = -1,
      TextureIndex = -1,
      NormalIndex = -1;
  };

  std::vector<vertex> V;
  std::vector<vec> Normals;
  std::vector<vec2> TexCoords;
  std::vector<OBJ_VERTEX_INDEX> I;
  FILE *F;
  CHAR Buf[1 << 8];
  matr NormalTransform(BaseTransform.GetInverse().GetTranspose());

  if ((F = fopen(FileName.c_str(), "r")) == NULL)
    error("File not load");

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      vec Pos;
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pos = BaseTransform.PointTransform(vec(x, y, z));
      V.push_back(vertex(Pos, vec(0, 0, 0), vec2(0, 0)));
    }
    else if (Buf[0] == 'v' && Buf[1] == 'n')
    {
      vec N;
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      N = NormalTransform.VectorTransform(vec(x, y, z));
      Normals.push_back(N);
    }
    else if (Buf[0] == 'v' && Buf[1] == 't')
    {
      DBL x, y;

      sscanf(Buf + 2, "%lf%lf", &x, &y);
      TexCoords.push_back(vec2(x, y));
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      OBJ_VERTEX_INDEX V1, V2, V3;

      if (!(sscanf(Buf + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                   &V1.VertexIndex, &V1.TextureIndex, &V1.NormalIndex,
                   &V2.VertexIndex, &V2.TextureIndex, &V2.NormalIndex,
                   &V3.VertexIndex, &V3.TextureIndex, &V3.NormalIndex) == 9 ||
        sscanf(Buf + 2, "%d//%d %d//%d %d//%d",
               &V1.VertexIndex, &V1.NormalIndex,
               &V2.VertexIndex, &V2.NormalIndex,
               &V3.VertexIndex, &V3.NormalIndex) == 6 ||
        sscanf(Buf + 2, "%d//%d %d//%d %d//%d",
               &V1.VertexIndex, &V1.TextureIndex,
               &V2.VertexIndex, &V2.TextureIndex,
               &V3.VertexIndex, &V3.TextureIndex) == 6 ||
        sscanf(Buf + 2, "%d %d %d",
               &V1.VertexIndex,
               &V2.VertexIndex,
               &V3.VertexIndex) == 3))
      {
        error("Wrong *.obj");
      }

      I.push_back(V1);
      I.push_back(V2);
      I.push_back(V3);
    }
  fclose(F);
  for (INT i = 0; i < I.size(); i += 3)
  {
    vertex
      P0 = V[I[i].VertexIndex - 1],
      P1 = V[I[i + 1].VertexIndex - 1],
      P2 = V[I[i + 2].VertexIndex - 1];
    const vec
      S1 = P1.P - P0.P,
      S2 = P2.P - P0.P;

    P0.N = P1.N = P2.N = (S1 % S2).GetNormalized();
    
    if (I[i].NormalIndex != -1)
      P0.N = Normals[I[i].NormalIndex - 1];
    if (I[i + 1].NormalIndex != -1)
      P1.N = Normals[I[i + 1].NormalIndex - 1];
    if (I[i + 2].NormalIndex != -1)
      P2.N = Normals[I[i + 2].NormalIndex - 1];

    if (I[i].TextureIndex != -1)
      P0.T = TexCoords[I[i].TextureIndex - 1];
    if (I[i + 1].TextureIndex != -1)
      P1.T = TexCoords[I[i + 1].TextureIndex - 1];
    if (I[i + 2].TextureIndex != -1)
      P2.T = TexCoords[I[i + 2].TextureIndex - 1];

    Triangles.push_back(triangle(P0, P1, P2, MtlId, EnviId));
  }
}

/**
 * \brief Create box and add triangles function
 * \param[in] Min Minimum coordinate
 * \param[in] Min Maximum coordinate
 * \param[in] MtlId Material identificator
 * \param[in] EnviId Environment identificator
 * \param[in] BaseTransform Transformation for model
 */
VOID shape::MakeBox( const vec &Min, const vec &Max, const INT MtlId, const INT EnviId,
                     const matr &BaseTransform )
{
  vec V[8];
  vec N[6];
  vec2 T[4];
  vec MaxVec = vec::Max(Max, Min), MinVec = vec::Min(Max, Min);
  matr NormalTransform(BaseTransform.GetInverse().GetTranspose());

  V[0] = BaseTransform.PointTransform(MinVec);
  V[1] = BaseTransform.PointTransform(MaxVec);
  V[2] = BaseTransform.PointTransform(vec(MaxVec.X, MinVec.Y, MinVec.Z));
  V[3] = BaseTransform.PointTransform(vec(MaxVec.X, MinVec.Y, MaxVec.Z));
  V[4] = BaseTransform.PointTransform(vec(MaxVec.X, MaxVec.Y, MinVec.Z));
  V[5] = BaseTransform.PointTransform(vec(MinVec.X, MaxVec.Y, MaxVec.Z));
  V[6] = BaseTransform.PointTransform(vec(MinVec.X, MaxVec.Y, MinVec.Z));
  V[7] = BaseTransform.PointTransform(vec(MinVec.X, MinVec.Y, MaxVec.Z));
  N[0] = NormalTransform.VectorTransform(vec(1, 0, 0));
  N[1] = NormalTransform.VectorTransform(vec(0, 1, 0));
  N[2] = NormalTransform.VectorTransform(vec(0, 0, 1));
  N[3] = NormalTransform.VectorTransform(vec(-1, 0, 0));
  N[4] = NormalTransform.VectorTransform(vec(0, -1, 0));
  N[5] = NormalTransform.VectorTransform(vec(0, 0, -1));
  T[2] = vec2(0, 0);
  T[3] = vec2(0, 1);
  T[0] = vec2(1, 0);
  T[1] = vec2(1, 1);

  std::vector<triangle> Tr =
    {
      triangle(vertex(V[3], N[0], T[3]),
               vertex(V[2], N[0], T[1]),
               vertex(V[4], N[0], T[0]), MtlId, EnviId),
      triangle(vertex(V[3], N[0], T[3]),
               vertex(V[1], N[0], T[2]),
               vertex(V[4], N[0], T[0]), MtlId, EnviId),
      triangle(vertex(V[5], N[1], T[0]),
               vertex(V[1], N[1], T[2]),
               vertex(V[4], N[1], T[3]), MtlId, EnviId),
      triangle(vertex(V[5], N[1], T[0]),
               vertex(V[6], N[1], T[1]),
               vertex(V[4], N[1], T[3]), MtlId, EnviId),  
      triangle(vertex(V[1], N[2], T[0]),
               vertex(V[7], N[2], T[3]),
               vertex(V[3], N[2], T[1]), MtlId, EnviId),  
      triangle(vertex(V[1], N[2], T[0]),
               vertex(V[5], N[2], T[2]),
               vertex(V[7], N[2], T[3]), MtlId, EnviId),
      triangle(vertex(V[0], N[3], T[3]),
               vertex(V[7], N[3], T[1]),
               vertex(V[5], N[3], T[0]), MtlId, EnviId),
      triangle(vertex(V[0], N[3], T[3]),
               vertex(V[6], N[3], T[2]),
               vertex(V[5], N[3], T[0]), MtlId, EnviId),
      triangle(vertex(V[0], N[4], T[1]),
               vertex(V[7], N[4], T[0]),
               vertex(V[3], N[4], T[2]), MtlId, EnviId),
      triangle(vertex(V[0], N[4], T[1]),
               vertex(V[2], N[4], T[3]),
               vertex(V[3], N[4], T[2]), MtlId, EnviId),
      triangle(vertex(V[0], N[5], T[1]),
               vertex(V[2], N[5], T[3]),
               vertex(V[4], N[5], T[2]), MtlId, EnviId),
      triangle(vertex(V[0], N[5], T[1]),
               vertex(V[4], N[5], T[2]),
               vertex(V[6], N[5], T[0]), MtlId, EnviId)
    };
  Triangles.insert(Triangles.end(), Tr.begin(), Tr.end());
}
