
project(vtkSlicer${MODULE_NAME}ModuleMRML)

#
# See CMake/SlicerMacroBuildModuleMRML.cmake for details
#

set(module_mrml_name "${PROJECT_NAME}")

# The header '${module_mrml_name}Export.h' will be automatically configured.
set(module_mrml_export_directive "VTK_SLICER_PERKEVALUATOR_MODULE_MRML_EXPORT")


# Additional directories to include
set(module_mrml_include_directories
  ${Slicer_Base_INCLUDE_DIRS}
  )

# Source files
set(module_mrml_SRCS
  vtkMRMLPerkEvaluatorNode.cxx
  vtkMRMLPerkEvaluatorNode.h
  )

# Additional Target libraries
set(module_mrml_target_libraries
  ${ITK_LIBRARIES}
  ${MRML_LIBRARIES}
  SlicerBaseLogic
  vtkSlicerTransformRecorderModuleMRML
  )

SlicerMacroBuildModuleMRML(
  NAME ${module_mrml_name}
  EXPORT_DIRECTIVE ${module_mrml_export_directive}
  INCLUDE_DIRECTORIES ${module_mrml_include_directories}
  SRCS ${module_mrml_SRCS}
  TARGET_LIBRARIES ${module_mrml_target_libraries}
  )