#ifndef __scene_loader_h_
#define __scene_loader_h_

#include <string>
#include <unordered_map>
#include <functional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "scene/scene.h"
#include "utils/image.h"
#include "render/render.h"
#include "scene/material.h"
#include "scene/environment.h"

namespace bpt = boost::property_tree;

/**
 * \brief Scene loader class
 */
class scene_loader
{
private:
  /** Shape for scene data */
  shape SceneShape;

  /**
   * \brief Structure for subtag's understanding
   */
  template <class type>
    struct LOAD_SUBTREE
    {
      /** Pointer to function for loading subtree */
      VOID (scene_loader::*LoadFunction)( type *StructurePointer, const LOAD_SUBTREE<type> &LoadStructure,
                                          const bpt::ptree &PropertyTree );

      /** Pointer to data */
      BYTE type::*Data;

      /** Data translator */
      std::function<VOID( const std::string &, BYTE * )> Translator;

      /**
       * \brief Structure constructor
       * \param[in, out] LoadFunction Pointer to function for loading subtree
       * \param[in, out] Data Pointer to data
       */
      LOAD_SUBTREE<type>( VOID (scene_loader::*LoadFunction)( type *StructurePointer, const LOAD_SUBTREE<type> &LoadStructure, const bpt::ptree &PropertyTree ),
                          BYTE type::*Data ) :
        LoadFunction(LoadFunction), Data(Data)
      {
      }

      /**
       * \brief Structure constructor
       * \param[in, out] LoadFunction Pointer to function for loading subtree
       * \param[in, out] Data Pointer to data
       * \param[in, out] Translator Data translator
       */
      LOAD_SUBTREE<type>( VOID (scene_loader::*LoadFunction)( type *StructurePointer, const LOAD_SUBTREE<type> &LoadStructure, const bpt::ptree &PropertyTree ),
                          BYTE type::* Data, const std::function<VOID( const std::string &, BYTE * )> &Translator ) :
        LoadFunction(LoadFunction), Data(Data), Translator(Translator)
      {
      }
    };

  /**
   * \brief Load value from property tree to member
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class type, class subtree_type>
    VOID LoadValue( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                    const bpt::ptree &PropertyTree )
    {
      StructurePointer->*reinterpret_cast<type subtree_type::*>(LoadStructure.Data) = PropertyTree.get_value<type>();
    }

  /**
   * \brief Load value from property tree to member
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class type, class subtree_type>
    VOID LoadValueWithTranslator( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                                  const bpt::ptree &PropertyTree )
    {
      std::string Value = PropertyTree.get_value<std::string>();

      LoadStructure.Translator(Value, &(StructurePointer->*LoadStructure.Data));
    }

  /**
   * \brief Load vec from property tree to member
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadVec( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                  const bpt::ptree &PropertyTree )
    {
      vec *Vector = &(StructurePointer->*reinterpret_cast<vec subtree_type::*>(LoadStructure.Data));;

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        std::unordered_map<std::string, LOAD_SUBTREE<vec>>::const_iterator Res = VecTagsMap.find(NodeName);

        if (Res == VecTagsMap.cend())
          error("unknown tag '" + NodeName + "'");

        (this->*(Res->second.LoadFunction))(Vector, Res->second, NodeSubtree);
      }
    }

  /**
   * \brief Load vec from property tree to member
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadColor( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                    const bpt::ptree &PropertyTree )
    {
      vec *Vector = &(StructurePointer->*reinterpret_cast<vec subtree_type::*>(LoadStructure.Data));;

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        std::unordered_map<std::string, LOAD_SUBTREE<vec>>::const_iterator Res = ColorTagsMap.find(NodeName);

        if (Res == ColorTagsMap.cend())
          error("unknown tag '" + NodeName + "'");

        (this->*(Res->second.LoadFunction))(Vector, Res->second, NodeSubtree);
      }
    }

  /**
   * \brief Structure for load camera
   */
  struct CAMERA_LOAD_DATA
  {
    /** Projection distance */
    FLT PrDist = 0.1;

    /** Projection size */
    FLT PrSize = 0.1;

    /** Camera location */
    vec Loc = vec(0, 5, -10);

    /** Camera at */
    vec At = vec(0, 2.5, 0);

    /** Up vector */
    vec Up1 = vec(0, 1, 0);
  };

  /** Structure for load camera */
  CAMERA_LOAD_DATA CameraLoadData;

  /**
   * \brief Load camera structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  VOID LoadCamera( scene_loader *StructurePointer, const LOAD_SUBTREE<scene_loader> &LoadStructure,
                   const bpt::ptree &PropertyTree );

  /**
   * \brief Load scene structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  VOID LoadScene( scene_loader *StructurePointer, const LOAD_SUBTREE<scene_loader> &LoadStructure,
                  const bpt::ptree &PropertyTree );

  /**
   * \brief Structure for load material
   */
  struct MATERIAL_LOAD_DATA
  {
    /** Material name */
    std::string Name;

    /** Fresnel coefficient for metal, albedo for dielectrics */
    vec Color = vec(1, 1, 1);

    /** Light coefficient */
    vec Emit = vec::Make();

    /** Roughness coefficient */
    FLT Roughness = 0.5;

    /** Metal ratio */
    FLT Metal = 0.5;
  };

  /**
   * \brief Load material structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  VOID LoadMaterial( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                     const bpt::ptree &PropertyTree );

  /**
   * \brief Structure for load environment
   */
  struct ENVIRONMENT_LOAD_DATA
  {
    /** Environment name */
    std::string Name;

    /** Absorption coefficient */
    FLT AbsCoef = 0;

    /** Fog coefficient */
    FLT FogCoef = 0;

    /** Fog color */
    vec FogColor = vec(0.8, 0.8, 0.8);
  };

  /**
   * \brief Load environment structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  VOID LoadEnvironment( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                        const bpt::ptree &PropertyTree );

  /**
   * \brief Load matrix structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadMatrixAndMultiply( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                                const bpt::ptree &PropertyTree )
    {
      matr *Matrix = &(StructurePointer->*reinterpret_cast<matr subtree_type::*>(LoadStructure.Data));
      matr NewMatrix;

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        std::unordered_map<std::string, std::pair<INT, INT>>::const_iterator Res = MatrixTagsMap.find(NodeName);

        if (Res == MatrixTagsMap.cend())
          error("unknown tag '" + NodeName + "'");

        NewMatrix[Res->second.first][Res->second.second] = (static_cast<bpt::ptree>(NodeSubtree)).get_value<FLT>();
      }

      *Matrix *= NewMatrix;
    }

  /**
   * \brief Load matrix structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadScaleAndMultiply( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                               const bpt::ptree &PropertyTree )
    {
      matr *Matrix = &(StructurePointer->*reinterpret_cast<matr subtree_type::*>(LoadStructure.Data));
      vec ScaleVec = vec::Make();

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        std::unordered_map<std::string, LOAD_SUBTREE<vec>>::const_iterator Res = VecTagsMap.find(NodeName);

        if (Res == VecTagsMap.cend())
          error("unknown tag '" + NodeName + "'");

        (this->*(Res->second.LoadFunction))(&ScaleVec, Res->second, NodeSubtree);
      }

      *Matrix *= matr::Scale(ScaleVec);
    }

  /**
   * \brief Load matrix structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadTranslateAndMultiply( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                               const bpt::ptree &PropertyTree )
    {
      matr *Matrix = &(StructurePointer->*reinterpret_cast<matr subtree_type::*>(LoadStructure.Data));
      vec TranslateVec = vec::Make();

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        std::unordered_map<std::string, LOAD_SUBTREE<vec>>::const_iterator Res = VecTagsMap.find(NodeName);

        if (Res == VecTagsMap.cend())
          error("unknown tag '" + NodeName + "'");

        (this->*(Res->second.LoadFunction))(&TranslateVec, Res->second, NodeSubtree);
      }

      *Matrix *= matr::Translate(TranslateVec);
    }

  /**
   * \brief Load matrix structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadRotateXAndMultiply( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                                   const bpt::ptree &PropertyTree )
    {
      matr *Matrix = &(StructurePointer->*reinterpret_cast<matr subtree_type::*>(LoadStructure.Data));
      FLT Angle = 0;

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        if (NodeName != "angle")
          error("unknown tag '" + NodeName + "'");

        Angle = (static_cast<bpt::ptree>(NodeSubtree)).get_value<FLT>();
      }

      *Matrix *= matr::RotateX(Angle);
    }

  /**
   * \brief Load matrix structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadRotateYAndMultiply( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                                 const bpt::ptree &PropertyTree )
    {
      matr *Matrix = &(StructurePointer->*reinterpret_cast<matr subtree_type::*>(LoadStructure.Data));
      FLT Angle = 0;

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        if (NodeName != "angle")
          error("unknown tag '" + NodeName + "'");

        Angle = (static_cast<bpt::ptree>(NodeSubtree)).get_value<FLT>();
      }

      *Matrix *= matr::RotateY(Angle);
    }

  /**
   * \brief Load matrix structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  template <class subtree_type>
    VOID LoadRotateZAndMultiply( subtree_type *StructurePointer, const LOAD_SUBTREE<subtree_type> &LoadStructure,
                                 const bpt::ptree &PropertyTree )
    {
      matr *Matrix = &(StructurePointer->*reinterpret_cast<matr subtree_type::*>(LoadStructure.Data));
      FLT Angle = 0;

      for (const auto &[NodeName, NodeSubtree] : PropertyTree)
      {
        if (NodeName != "angle")
          error("unknown tag '" + NodeName + "'");

        Angle = (static_cast<bpt::ptree>(NodeSubtree)).get_value<FLT>();
      }

      *Matrix *= matr::RotateZ(Angle);
    }

  /**
   * \brief Structure for load box
   */
  struct BOX_LOAD_STRUCTURE
  {
    /** Material name */
    std::string MtlName;

    /** Environment name */
    std::string EnviName;

    /** Minimal coordinate */
    vec Min = vec::Make();

    /** Maximal coordinate */
    vec Max = vec::Make();

    /** Transformation matrix */
    matr Transform = matr::Identity();
  };

  /**
   * \brief Load box structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  VOID LoadBox( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                const bpt::ptree &PropertyTree );

  /**5
   * \brief Structure for load *.obj file
   */
  struct OBJ_LOAD_STRUCTURE
  {
    /** File name */
    std::string FileName;

    /** Material name */
    std::string MtlName;

    /** Environment name */
    std::string EnviName;

    /** Transformation matrix */
    matr Transform = matr::Identity();
  };

  /**
   * \brief Load obj structure function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  VOID LoadObj( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                const bpt::ptree &PropertyTree );

  /**
   * \brief Load air environment name function.
   * \param[in, out] StructurePointer Pointer to structure for fill
   * \param[in] LoadStructure Load structure
   * \param[in] PropertyTree Property tree for loading
   */
  VOID LoadAirEnvironmentName( scene *StructurePointer, const LOAD_SUBTREE<scene> &LoadStructure,
                               const bpt::ptree &PropertyTree );

  /** Tags map for parse frame subtree */
  static std::unordered_map<std::string, LOAD_SUBTREE<scene_loader>> FrameTagsMap;

  /** Tags map for parse camera subtree */
  static std::unordered_map<std::string, LOAD_SUBTREE<CAMERA_LOAD_DATA>> CameraTagsMap;

  /** Tags map for parse vector subtree */
  static std::unordered_map<std::string, LOAD_SUBTREE<vec>> VecTagsMap;

  /** Tags map for parse color subtree */
  static std::unordered_map<std::string, LOAD_SUBTREE<vec>> ColorTagsMap;

  /** Tags maps for matrix loading */
  static std::unordered_map<std::string, std::pair<INT, INT>> MatrixTagsMap;

  /** Tags maps for material loading */
  static std::unordered_map<std::string, LOAD_SUBTREE<MATERIAL_LOAD_DATA>> MaterialTagsMap;

  /** Tags maps for environment loading */
  static std::unordered_map<std::string, LOAD_SUBTREE<ENVIRONMENT_LOAD_DATA>> EnvironmentTagsMap;

  /** Tags maps for box loading */
  static std::unordered_map<std::string, LOAD_SUBTREE<BOX_LOAD_STRUCTURE>> BoxTagsMap;

  /** Tags maps for obj loading */
  static std::unordered_map<std::string, LOAD_SUBTREE<OBJ_LOAD_STRUCTURE>> ObjTagsMap;

  /** Tags maps for scene loading */
  static std::unordered_map<std::string, LOAD_SUBTREE<scene>> SceneTagsMap;

  /** Materials map */
  std::unordered_map<std::string, INT> MaterialsMap;

  /** Environments map */
  std::unordered_map<std::string, INT> EnvironmentsMap;

  /**
   * \brief Load scene description function from property tree
   * \param[in] PropertyTree Property tree
   */
  VOID LoadFromPropertyTree( const bpt::ptree &PropertyTree );

public:
  /** Scene for load */
  scene Scene;

  /** Frame width */
  INT Width = 0;

  /** Frame height */
  INT Height = 0;

  /** Device identifier */
  UINT DeviceId = 0;

  /** Number of samples per pixel */
  INT NumberOfSamples = 1;

  /** Output image format */
  image::FORMAT OutputFormat = image::FORMAT::PNG;

  /** Path to output image */
  std::string OutputPath = "res/output_image";

  /** Render mode */
  render::MODE RenderMode = render::MODE::CPU;

  /** Camera for render */
  cam Camera;

  /**
   * \brief Load XML scene description function
   * \param[in] FileName Name of file for loading
   */
  VOID LoadXML( const std::string &FileName );
};

#endif /* __scene_loader_h_ */
