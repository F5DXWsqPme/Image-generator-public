#include <iostream>

#include "utils/error.h"
#include "scene_loader.h"

/** Tags map for parse frame subtree */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<scene_loader>> scene_loader::FrameTagsMap =
  {
    {
      "device_id",
      scene_loader::LOAD_SUBTREE<scene_loader>(
          &scene_loader::LoadValue<INT, scene_loader>,
          reinterpret_cast<BYTE scene_loader::*>(&scene_loader::DeviceId))
    },
    {
      "width",
        scene_loader::LOAD_SUBTREE<scene_loader>(
          &scene_loader::LoadValue<INT, scene_loader>,
          reinterpret_cast<BYTE scene_loader::*>(&scene_loader::Width))
    },
    {
      "height",
      scene_loader::LOAD_SUBTREE<scene_loader>(
        &scene_loader::LoadValue<INT, scene_loader>,
        reinterpret_cast<BYTE scene_loader::*>(&scene_loader::Height))
    },
    {
      "number_of_samples",
      scene_loader::LOAD_SUBTREE<scene_loader>(
        &scene_loader::LoadValue<UINT, scene_loader>,
        reinterpret_cast<BYTE scene_loader::*>(&scene_loader::NumberOfSamples))
    },
    {
      "output_path",
      scene_loader::LOAD_SUBTREE<scene_loader>(
        &scene_loader::LoadValue<std::string, scene_loader>,
        reinterpret_cast<BYTE scene_loader::*>(&scene_loader::OutputPath))
    },
    {
      "output_format",
      scene_loader::LOAD_SUBTREE<scene_loader>(
        &scene_loader::LoadValueWithTranslator<image::FORMAT, scene_loader>,
        reinterpret_cast<BYTE scene_loader::*>(&scene_loader::OutputFormat),
        []( const std::string &Str, BYTE *Data )
        {
          static std::unordered_map<std::string, image::FORMAT> FormatsMap =
            {
              {
                "jpg", image::FORMAT::JPG
              },
              {
                "png", image::FORMAT::PNG
              },
              {
                "tga", image::FORMAT::TGA
              },
            };

          std::unordered_map<std::string, image::FORMAT>::const_iterator Res = FormatsMap.find(Str);

          if (Res == FormatsMap.cend())
            error("unknown image type");

          *reinterpret_cast<image::FORMAT *>(Data) = Res->second;
        })
    },
    {
      "render_mode",
      scene_loader::LOAD_SUBTREE<scene_loader>(
        &scene_loader::LoadValueWithTranslator<render::MODE, scene_loader>,
        reinterpret_cast<BYTE scene_loader::*>(&scene_loader::RenderMode),
        []( const std::string &Str, BYTE *Data )
        {
          static std::unordered_map<std::string, render::MODE> TypesMap =
            {
              {
                "cpu", render::MODE::CPU
              },
              {
                "gpu", render::MODE::VULKAN
              },
              {
                "gpu_one_seed", render::MODE::VULKAN_ONE_SEED
              }
            };

          std::unordered_map<std::string, render::MODE>::const_iterator Res = TypesMap.find(Str);

          if (Res == TypesMap.cend())
            error("unknown render mode");

          *reinterpret_cast<render::MODE *>(Data) = Res->second;
        })
    },
    {
      "camera",
      scene_loader::LOAD_SUBTREE<scene_loader>(
        &scene_loader::LoadCamera,
        reinterpret_cast<BYTE scene_loader::*>(&scene_loader::CameraLoadData))
    },
    {
      "scene",
      scene_loader::LOAD_SUBTREE<scene_loader>(
        &scene_loader::LoadScene,
        reinterpret_cast<BYTE scene_loader::*>(&scene_loader::Scene))
    },
  };

/** Tags map for parse camera subtree */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<scene_loader::CAMERA_LOAD_DATA>> scene_loader::CameraTagsMap =
  {
    {
      "proj_dist",
      scene_loader::LOAD_SUBTREE<CAMERA_LOAD_DATA>(
        &scene_loader::LoadValue<FLT, CAMERA_LOAD_DATA>,
        reinterpret_cast<BYTE CAMERA_LOAD_DATA::*>(&CAMERA_LOAD_DATA::PrDist))
    },
    {
      "proj_size",
      scene_loader::LOAD_SUBTREE<CAMERA_LOAD_DATA>(
        &scene_loader::LoadValue<FLT, CAMERA_LOAD_DATA>,
        reinterpret_cast<BYTE CAMERA_LOAD_DATA::*>(&CAMERA_LOAD_DATA::PrSize))
    },
    {
      "location",
      scene_loader::LOAD_SUBTREE<CAMERA_LOAD_DATA>(
        &scene_loader::LoadVec<CAMERA_LOAD_DATA>,
        reinterpret_cast<BYTE CAMERA_LOAD_DATA::*>(&CAMERA_LOAD_DATA::Loc))
    },
    {
      "at",
      scene_loader::LOAD_SUBTREE<CAMERA_LOAD_DATA>(
        &scene_loader::LoadVec<CAMERA_LOAD_DATA>,
        reinterpret_cast<BYTE CAMERA_LOAD_DATA::*>(&CAMERA_LOAD_DATA::At))
    },
    {
      "up",
      scene_loader::LOAD_SUBTREE<CAMERA_LOAD_DATA>(
        &scene_loader::LoadVec<CAMERA_LOAD_DATA>,
        reinterpret_cast<BYTE CAMERA_LOAD_DATA::*>(&CAMERA_LOAD_DATA::Up1))
    },
  };

/** Tags map for parse vector subtree */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<vec>> scene_loader::VecTagsMap =
  {
    {
      "x",
      scene_loader::LOAD_SUBTREE<vec>(
        &scene_loader::LoadValue<FLT, vec>,
        reinterpret_cast<BYTE vec::*>(&vec::X))
    },
    {
      "y",
      scene_loader::LOAD_SUBTREE<vec>(
        &scene_loader::LoadValue<FLT, vec>,
        reinterpret_cast<BYTE vec::*>(&vec::Y))
    },
    {
      "z",
      scene_loader::LOAD_SUBTREE<vec>(
        &scene_loader::LoadValue<FLT, vec>,
        reinterpret_cast<BYTE vec::*>(&vec::Z))
    },
  };

/** Tags map for parse color subtree */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<vec>> scene_loader::ColorTagsMap =
  {
    {
      "r",
      scene_loader::LOAD_SUBTREE<vec>(
        &scene_loader::LoadValue<FLT, vec>,
        reinterpret_cast<BYTE vec::*>(&vec::X))
    },
    {
      "g",
      scene_loader::LOAD_SUBTREE<vec>(
        &scene_loader::LoadValue<FLT, vec>,
        reinterpret_cast<BYTE vec::*>(&vec::Y))
    },
    {
      "b",
      scene_loader::LOAD_SUBTREE<vec>(
        &scene_loader::LoadValue<FLT, vec>,
        reinterpret_cast<BYTE vec::*>(&vec::Z))
    },
  };

/** Tags map for parse matrix subtree */
std::unordered_map<std::string, std::pair<INT, INT>> scene_loader::MatrixTagsMap =
  {
    {
      "a11", std::pair<INT, INT>(0, 0)
    },
    {
      "a12", std::pair<INT, INT>(0, 1)
    },
    {
      "a13", std::pair<INT, INT>(0, 2)
    },
    {
      "a14", std::pair<INT, INT>(0, 3)
    },
    {
      "a21", std::pair<INT, INT>(1, 0)
    },
    {
      "a22", std::pair<INT, INT>(1, 1)
    },
    {
      "a23", std::pair<INT, INT>(1, 2)
    },
    {
      "a24", std::pair<INT, INT>(1, 3)
    },
    {
      "a31", std::pair<INT, INT>(2, 0)
    },
    {
      "a32", std::pair<INT, INT>(2, 1)
    },
    {
      "a33", std::pair<INT, INT>(2, 2)
    },
    {
      "a34", std::pair<INT, INT>(2, 3)
    },
    {
      "a41", std::pair<INT, INT>(3, 0)
    },
    {
      "a42", std::pair<INT, INT>(3, 1)
    },
    {
      "a43", std::pair<INT, INT>(3, 2)
    },
    {
      "a44", std::pair<INT, INT>(3, 3)
    },
  };

/** Tags maps for material loading */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<scene_loader::MATERIAL_LOAD_DATA>> scene_loader::MaterialTagsMap =
  {
    {
      "name",
      scene_loader::LOAD_SUBTREE<MATERIAL_LOAD_DATA>(
        &scene_loader::LoadValue<std::string, MATERIAL_LOAD_DATA>,
        reinterpret_cast<BYTE MATERIAL_LOAD_DATA::*>(&MATERIAL_LOAD_DATA::Name))
    },
    {
      "color",
      scene_loader::LOAD_SUBTREE<MATERIAL_LOAD_DATA>(
        &scene_loader::LoadColor<MATERIAL_LOAD_DATA>,
        reinterpret_cast<BYTE MATERIAL_LOAD_DATA::*>(&MATERIAL_LOAD_DATA::Color))
    },
    {
      "emit",
      scene_loader::LOAD_SUBTREE<MATERIAL_LOAD_DATA>(
        &scene_loader::LoadColor<MATERIAL_LOAD_DATA>,
        reinterpret_cast<BYTE MATERIAL_LOAD_DATA::*>(&MATERIAL_LOAD_DATA::Emit))
    },
    {
      "roughness",
      scene_loader::LOAD_SUBTREE<MATERIAL_LOAD_DATA>(
        &scene_loader::LoadValue<FLT, MATERIAL_LOAD_DATA>,
        reinterpret_cast<BYTE MATERIAL_LOAD_DATA::*>(&MATERIAL_LOAD_DATA::Roughness))
    },
    {
      "metal",
      scene_loader::LOAD_SUBTREE<MATERIAL_LOAD_DATA>(
        &scene_loader::LoadValue<FLT, MATERIAL_LOAD_DATA>,
        reinterpret_cast<BYTE MATERIAL_LOAD_DATA::*>(&MATERIAL_LOAD_DATA::Metal))
    },
  };

/** Tags maps for environment loading */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<scene_loader::ENVIRONMENT_LOAD_DATA>> scene_loader::EnvironmentTagsMap =
  {
    {
      "name",
      scene_loader::LOAD_SUBTREE<ENVIRONMENT_LOAD_DATA>(
        &scene_loader::LoadValue<std::string, ENVIRONMENT_LOAD_DATA>,
        reinterpret_cast<BYTE ENVIRONMENT_LOAD_DATA::*>(&ENVIRONMENT_LOAD_DATA::Name))
    },
    {
      "absorption",
      scene_loader::LOAD_SUBTREE<ENVIRONMENT_LOAD_DATA>(
        &scene_loader::LoadValue<FLT, ENVIRONMENT_LOAD_DATA>,
        reinterpret_cast<BYTE ENVIRONMENT_LOAD_DATA::*>(&ENVIRONMENT_LOAD_DATA::AbsCoef))
    },
    {
      "fog_intensity",
        scene_loader::LOAD_SUBTREE<ENVIRONMENT_LOAD_DATA>(
          &scene_loader::LoadValue<FLT, ENVIRONMENT_LOAD_DATA>,
          reinterpret_cast<BYTE ENVIRONMENT_LOAD_DATA::*>(&ENVIRONMENT_LOAD_DATA::FogCoef))
    },
    {
      "fog_color",
        scene_loader::LOAD_SUBTREE<ENVIRONMENT_LOAD_DATA>(
          &scene_loader::LoadColor<ENVIRONMENT_LOAD_DATA>,
          reinterpret_cast<BYTE ENVIRONMENT_LOAD_DATA::*>(&ENVIRONMENT_LOAD_DATA::FogColor))
    },
  };

/** Tags maps for box loading */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<scene_loader::BOX_LOAD_STRUCTURE>> scene_loader::BoxTagsMap =
  {
    {
      "material_name",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadValue<std::string, BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::MtlName))
    },
    {
      "environment_name",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadValue<std::string, BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::EnviName))
    },
    {
      "min",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadVec<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Min))
    },
    {
      "max",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadVec<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Max))
    },
    {
      "transform_matrix",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadMatrixAndMultiply<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Transform))
    },
    {
      "scale",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadScaleAndMultiply<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Transform))
    },
    {
      "translate",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadTranslateAndMultiply<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Transform))
    },
    {
      "rotate_x",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadRotateXAndMultiply<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Transform))
    },
    {
      "rotate_y",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadRotateYAndMultiply<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Transform))
    },
    {
      "rotate_z",
      scene_loader::LOAD_SUBTREE<BOX_LOAD_STRUCTURE>(
        &scene_loader::LoadRotateZAndMultiply<BOX_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE BOX_LOAD_STRUCTURE::*>(&BOX_LOAD_STRUCTURE::Transform))
    },
  };

/** Tags maps for obj loading */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<scene_loader::OBJ_LOAD_STRUCTURE>> scene_loader::ObjTagsMap =
  {
    {
      "file_name",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadValue<std::string, OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::FileName))
    },
    {
      "material_name",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadValue<std::string, OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::MtlName))
    },
    {
      "environment_name",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadValue<std::string, OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::EnviName))
    },
    {
      "transform_matrix",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadMatrixAndMultiply<OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::Transform))
    },
    {
      "scale",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadScaleAndMultiply<OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::Transform))
    },
    {
      "translate",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadTranslateAndMultiply<OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::Transform))
    },
    {
      "rotate_x",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadRotateXAndMultiply<OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::Transform))
    },
    {
      "rotate_y",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadRotateYAndMultiply<OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::Transform))
    },
    {
      "rotate_z",
      scene_loader::LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>(
        &scene_loader::LoadRotateZAndMultiply<OBJ_LOAD_STRUCTURE>,
        reinterpret_cast<BYTE OBJ_LOAD_STRUCTURE::*>(&OBJ_LOAD_STRUCTURE::Transform))
    },
  };

/** Tags maps for scene loading */
std::unordered_map<std::string, scene_loader::LOAD_SUBTREE<scene>> scene_loader::SceneTagsMap =
  {
    {
      "material",
      scene_loader::LOAD_SUBTREE<scene>(
        &scene_loader::LoadMaterial,
        nullptr)
    },
    {
      "environment",
      scene_loader::LOAD_SUBTREE<scene>(
        &scene_loader::LoadEnvironment,
        nullptr)
    },
    {
      "box",
      scene_loader::LOAD_SUBTREE<scene>(
        &scene_loader::LoadBox,
        nullptr)
    },
    {
      "obj_model",
      scene_loader::LOAD_SUBTREE<scene>(
        &scene_loader::LoadObj,
        nullptr)
    },
    {
      "air_environment_name",
      scene_loader::LOAD_SUBTREE<scene>(
        &scene_loader::LoadAirEnvironmentName,
        reinterpret_cast<BYTE scene::*>(&scene::AirEnvi))
    },
  };
  
/**
 * \brief Load camera structure function.
 * \param[in, out] StructurePointer Pointer to structure for fill
 * \param LoadStructure Load structure
 * \param PropertyTree Property tree for loading
 */
VOID scene_loader::LoadCamera( scene_loader *StructurePointer, const LOAD_SUBTREE<scene_loader> &LoadStructure, const bpt::ptree &PropertyTree )
{
  CAMERA_LOAD_DATA *Cam = &(StructurePointer->*reinterpret_cast<CAMERA_LOAD_DATA scene_loader::*>(LoadStructure.Data));

  for (const auto &[NodeName, NodeSubtree] : PropertyTree)
  {
    std::unordered_map<std::string, LOAD_SUBTREE<CAMERA_LOAD_DATA>>::const_iterator Res = CameraTagsMap.find(NodeName);

    if (Res == CameraTagsMap.cend())
      error("unknown tag '" + NodeName + "'");

    (this->*(Res->second.LoadFunction))(Cam, Res->second, NodeSubtree);
  }
}

/**
 * \brief Load air environment name function.
 * \param[in, out] StructurePointer Pointer to structure for fill
 * \param[in] LoadStructure Load structure
 * \param[in] PropertyTree Property tree for loading
 */
VOID scene_loader::LoadAirEnvironmentName( scene *StructurePointer, const scene_loader::LOAD_SUBTREE<scene> &LoadStructure,
                                           const bpt::ptree &PropertyTree )
{
  std::string Name = PropertyTree.get_value<std::string>();

  std::unordered_map<std::string, INT>::const_iterator EnviIt = EnvironmentsMap.find(Name);

  if (EnviIt == EnvironmentsMap.cend())
    error("environment '" + Name + "' not found");

  StructurePointer->*reinterpret_cast<environment scene::*>(LoadStructure.Data) = environment::Table[EnviIt->second];
}

/**
 * \brief Load scene structure function.
 * \param[in, out] StructurePointer Pointer to structure for fill
 * \param[in] LoadStructure Load structure
 * \param[in] PropertyTree Property tree for loading
 */
VOID scene_loader::LoadScene( scene_loader *StructurePointer, const LOAD_SUBTREE<scene_loader> &LoadStructure, const bpt::ptree &PropertyTree )
{
  scene *ScenePtr = &(StructurePointer->*reinterpret_cast<scene scene_loader::*>(LoadStructure.Data));

  for (const auto &[NodeName, NodeSubtree] : PropertyTree)
  {
    std::unordered_map<std::string, LOAD_SUBTREE<scene>>::const_iterator Res = SceneTagsMap.find(NodeName);

    if (Res == SceneTagsMap.cend())
      error("unknown tag '" + NodeName + "'");

    (this->*(Res->second.LoadFunction))(ScenePtr, Res->second, NodeSubtree);
  }
}

/**
 * \brief Load material structure function.
 * \param[in, out] StructurePointer Pointer to structure for fill
 * \param[in] LoadStructure Load structure
 * \param[in] PropertyTree Property tree for loading
 */
VOID scene_loader::LoadMaterial( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                                 const bpt::ptree &PropertyTree )
{
  MATERIAL_LOAD_DATA MaterialData;

  for (const auto &[NodeName, NodeSubtree] : PropertyTree)
  {
    std::unordered_map<std::string, LOAD_SUBTREE<MATERIAL_LOAD_DATA>>::const_iterator Res = MaterialTagsMap.find(NodeName);

    if (Res == MaterialTagsMap.cend())
      error("unknown tag '" + NodeName + "'");

    (this->*(Res->second.LoadFunction))(&MaterialData, Res->second, NodeSubtree);
  }

  if (MaterialsMap.find(MaterialData.Name) != MaterialsMap.end())
    error("material '" + MaterialData.Name + "' redefinition");

  INT MtlId = material::AddToTable(
    material(MaterialData.Color, MaterialData.Emit, MaterialData.Roughness, MaterialData.Metal));

  MaterialsMap[MaterialData.Name] = MtlId;
}

/**
 * \brief Load environment structure function.
 * \param[in, out] StructurePointer Pointer to structure for fill
 * \param[in] LoadStructure Load structure
 * \param[in] PropertyTree Property tree for loading
 */
VOID scene_loader::LoadEnvironment( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                                    const bpt::ptree &PropertyTree )
{
  ENVIRONMENT_LOAD_DATA EnvironmentData;

  for (const auto &[NodeName, NodeSubtree] : PropertyTree)
  {
    std::unordered_map<std::string, LOAD_SUBTREE<ENVIRONMENT_LOAD_DATA>>::const_iterator Res = EnvironmentTagsMap.find(NodeName);

    if (Res == EnvironmentTagsMap.cend())
      error("unknown tag '" + NodeName + "'");

    (this->*(Res->second.LoadFunction))(&EnvironmentData, Res->second, NodeSubtree);
  }

  if (EnvironmentsMap.find(EnvironmentData.Name) != EnvironmentsMap.end())
    error("environment '" + EnvironmentData.Name + "' redefinition");

  INT EnviId = environment::AddToTable(
    environment(EnvironmentData.FogColor, EnvironmentData.FogCoef, EnvironmentData.AbsCoef));

  EnvironmentsMap[EnvironmentData.Name] = EnviId;
}

/**
 * \brief Load box structure function.
 * \param[in, out] StructurePointer Pointer to structure for fill
 * \param[in] LoadStructure Load structure
 * \param[in] PropertyTree Property tree for loading
 */
VOID scene_loader::LoadBox( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                            const bpt::ptree &PropertyTree )
{
  BOX_LOAD_STRUCTURE BoxData;

  for (const auto &[NodeName, NodeSubtree] : PropertyTree)
  {
    std::unordered_map<std::string, LOAD_SUBTREE<BOX_LOAD_STRUCTURE>>::const_iterator Res = BoxTagsMap.find(NodeName);

    if (Res == BoxTagsMap.cend())
      error("unknown tag '" + NodeName + "'");

    (this->*(Res->second.LoadFunction))(&BoxData, Res->second, NodeSubtree);
  }

  std::unordered_map<std::string, INT>::const_iterator MtlIt = MaterialsMap.find(BoxData.MtlName);

  if (MtlIt == MaterialsMap.cend())
    error("material '" + BoxData.MtlName + "' not found");

  std::unordered_map<std::string, INT>::const_iterator EnviIt = EnvironmentsMap.find(BoxData.EnviName);

  if (EnviIt == EnvironmentsMap.cend())
    error("environment '" + BoxData.EnviName + "' not found");

  SceneShape.MakeBox(BoxData.Min, BoxData.Max, MtlIt->second, EnviIt->second, BoxData.Transform);
}

/**
 * \brief Load obj structure function.
 * \param[in, out] StructurePointer Pointer to structure for fill
 * \param[in] LoadStructure Load structure
 * \param[in] PropertyTree Property tree for loading
 */
VOID scene_loader::LoadObj( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                            const bpt::ptree &PropertyTree )
{
  OBJ_LOAD_STRUCTURE ObjData;

  for (const auto &[NodeName, NodeSubtree] : PropertyTree)
  {
    std::unordered_map<std::string, LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>>::const_iterator Res = ObjTagsMap.find(NodeName);

    if (Res == ObjTagsMap.cend())
      error("unknown tag '" + NodeName + "'");

    (this->*(Res->second.LoadFunction))(&ObjData, Res->second, NodeSubtree);
  }

  std::unordered_map<std::string, INT>::const_iterator MtlIt = MaterialsMap.find(ObjData.MtlName);

  if (MtlIt == MaterialsMap.cend())
    error("material '" + ObjData.MtlName + "' not found");

  std::unordered_map<std::string, INT>::const_iterator EnviIt = EnvironmentsMap.find(ObjData.EnviName);

  if (EnviIt == EnvironmentsMap.cend())
    error("environment '" + ObjData.EnviName + "' not found");

  SceneShape.LoadOBJ(ObjData.FileName, MtlIt->second, EnviIt->second, ObjData.Transform);
}

/**
 * \brief Load scene description function from property tree
 * \param[in] PropertyTree Property tree
 */
VOID scene_loader::LoadFromPropertyTree( const bpt::ptree &PropertyTree )
{
  for (const auto &[NodeName, NodeSubtree] : PropertyTree)
  {
    std::unordered_map<std::string, LOAD_SUBTREE<scene_loader>>::const_iterator Res = FrameTagsMap.find(NodeName);

    if (Res == FrameTagsMap.cend())
      error("unknown tag '" + NodeName + "'");

    (this->*(Res->second.LoadFunction))(this, Res->second, NodeSubtree);
  }

  Camera.SetWH(Width, Height);
  Camera.SetProj(CameraLoadData.PrDist, CameraLoadData.PrSize);
  Camera.SetView(CameraLoadData.Loc, CameraLoadData.At, CameraLoadData.Up1);

  Scene.Objects.push_back(&SceneShape);
  Scene.IsChanged = TRUE;
}

/**
 * \brief Load XML scene description function
 * \param FileName Name of file for loading
 */
VOID scene_loader::LoadXML( const std::string &FileName )
{
  bpt::ptree PropertyTree;

  bpt::read_xml(FileName, PropertyTree);

  LoadFromPropertyTree(PropertyTree.get_child("frame"));
}
