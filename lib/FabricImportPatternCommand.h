//
// Copyright (c) 2010-2016, Fabric Software Inc. All rights reserved.
//

#pragma once

#include "Foundation.h"

#include <maya/MArgList.h>
#include <maya/MArgParser.h>
#include <maya/MPxCommand.h>

#include <FabricCore.h>

#include <vector>
#include <map>

struct FabricImportPatternSettings
{
  MString filePath;
  bool quiet;
  MString rootPrefix;
  MString nameSpace;
  double scale;
  bool enableMaterials;
  bool attachToExisting;
  bool createIfMissing;
  bool useLastArgValues;
  bool userAttributes;
  bool attachToSceneTime;
  bool stripNameSpaces;

  FabricImportPatternSettings()
  {
    quiet = false;
    rootPrefix = L"";
    nameSpace = L"";
    scale = 1.0;
    enableMaterials = false;
    attachToExisting = false;
    createIfMissing = true;
    useLastArgValues = true;
    userAttributes = true;
    attachToSceneTime = true;
    stripNameSpaces = false;
  }
};

class FabricImportPatternCommand: public MPxCommand
{
public:

  virtual const char * getName() { return "FabricImportPattern"; }
  static void* creator();
  static MSyntax newSyntax();
  virtual MStatus doIt(const MArgList &args);
  virtual bool isUndoable() const { return false; }
  MStatus invoke(FabricCore::Client client, FabricCore::DFGBinding, const FabricImportPatternSettings & settings);
  void cleanup(FabricCore::DFGBinding binding);

private:
  FabricCore::Client m_client;
  FabricCore::RTVal m_context;
  std::vector< FabricCore::RTVal > m_objectList;
  std::map< std::string, size_t > m_objectMap;
  std::map< std::string, MObject > m_nodes;
  std::map< std::string, MObject > m_materialSets;
  static bool s_loadedMatrixPlugin;
  MString m_prevNameSpace;

  FabricImportPatternSettings m_settings;

  MString parentPath(MString path, MString * name = NULL);
  MString simplifyPath(MString path);
  MString mayaPathFromPatternPath(MString path);
  MObject getOrCreateNodeForPath(MString path, MString type="transform", bool createIfMissing = true, bool isDagNode = true);
  MObject getOrCreateNodeForObject(FabricCore::RTVal obj);
  MObject getOrCreateShapeForObject(FabricCore::RTVal obj);
  MObject getShapeForNode(MObject node);
  bool updateTransformForObject(FabricCore::RTVal obj, MObject node = MObject::kNullObj);
  bool updateMaterialForObject(FabricCore::RTVal obj, MObject node);
  bool updateEvaluatorForObject(FabricCore::RTVal obj);
  void processUserAttributes(FabricCore::RTVal objRef);
};
