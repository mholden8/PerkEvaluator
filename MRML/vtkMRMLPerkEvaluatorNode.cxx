

#include "vtkMRMLPerkEvaluatorNode.h"

// Constants ------------------------------------------------------------------
static const char* TRANSFORM_BUFFER_REFERENCE_ROLE = "TransformBuffer";
static const char* METRICS_TABLE_REFERENCE_ROLE = "MetricsTable";
static const char* METRIC_INSTANCE_REFERENCE_ROLE = "MetricInstance";


// Standard MRML Node Methods ------------------------------------------------------------

vtkMRMLPerkEvaluatorNode* vtkMRMLPerkEvaluatorNode
::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance( "vtkMRMLPerkEvaluatorNode" );
  if( ret )
    {
      return ( vtkMRMLPerkEvaluatorNode* )ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkMRMLPerkEvaluatorNode();
}


vtkMRMLNode* vtkMRMLPerkEvaluatorNode
::CreateNodeInstance()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance( "vtkMRMLPerkEvaluatorNode" );
  if( ret )
    {
      return ( vtkMRMLPerkEvaluatorNode* )ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkMRMLPerkEvaluatorNode();
}



void vtkMRMLPerkEvaluatorNode
::PrintSelf( ostream& os, vtkIndent indent )
{
  vtkMRMLNode::PrintSelf(os,indent);
}


void vtkMRMLPerkEvaluatorNode
::WriteXML( ostream& of, int nIndent )
{
  Superclass::WriteXML(of, nIndent);

  vtkIndent indent(nIndent);
  
  of << indent << "AutoUpdateMeasurementRange=\"" << this->AutoUpdateMeasurementRange << "\"";
  of << indent << "MarkBegin=\"" << this->MarkBegin << "\"";
  of << indent << "MarkEnd=\"" << this->MarkEnd << "\"";
  of << indent << "NeedleOrientation=\"" << this->NeedleOrientation << "\"";
  of << indent << "PlaybackTime=\"" << this->PlaybackTime << "\"";
  of << indent << "RealTimeProcessing=\"" << this->RealTimeProcessing << "\"";
}


void vtkMRMLPerkEvaluatorNode
::ReadXMLAttributes( const char** atts )
{
  Superclass::ReadXMLAttributes(atts);

  // Read all MRML node attributes from two arrays of names and values
  const char* attName;
  const char* attValue;

  // Setup attributes from "old-style" scene
  this->TransformRoleMap.clear();
  this->AnatomyNodeMap.clear();

  while (*atts != NULL)
  {
    attName  = *(atts++);
    attValue = *(atts++);
 
    if ( ! strcmp( attName, "AutoUpdateMeasurementRange" ) )
    {
      this->AutoUpdateMeasurementRange = atoi( attValue );
    }
    if ( ! strcmp( attName, "MarkBegin" ) )
    {
      this->MarkBegin = atof( attValue );
    }
    if ( ! strcmp( attName, "MarkEnd" ) )
    {
      this->MarkEnd = atof( attValue );
    }
    if ( ! strcmp( attName, "NeedleOrientation" ) )
    {
      this->NeedleOrientation = ( NeedleOrientationEnum ) atoi( attValue );
    }
    if ( ! strcmp( attName, "PlaybackTime" ) )
    {
      this->PlaybackTime = atof( attValue );
    }
    if ( ! strcmp( attName, "RealTimeProcessing" ) )
    {
      this->RealTimeProcessing = atof( attValue );
    }

    // Read attributes from "old-style" scene
    if ( ! strcmp( attName, "MetricsDirectory" ) )
    {
      this->MetricsDirectory = std::string( attValue );
    }
    if ( std::string( attName ).find( "TransformRoleMap" ) != std::string::npos )
    {
      std::stringstream transformRoleStream( attValue );
      std::string transformNodeName; transformRoleStream >> transformNodeName; 
      std::string transformRole; transformRoleStream >> transformRole;
      this->TransformRoleMap[ transformNodeName ] = transformRole;
    }    
    if ( std::string( attName ).find( "AnatomyNodeMap" ) != std::string::npos )
    {
      std::stringstream anatomyNodeStream( attValue );
      std::string anatomyRole; anatomyNodeStream >> anatomyRole; 
      std::string anatomyNodeName; anatomyNodeStream >> anatomyNodeName;
      this->AnatomyNodeMap[ anatomyRole ] = anatomyNodeName;
    }
    
  }

  // Ignore the maps for now...
}


void vtkMRMLPerkEvaluatorNode
::Copy( vtkMRMLNode *anode )
{
  Superclass::Copy( anode );
  vtkMRMLPerkEvaluatorNode *node = ( vtkMRMLPerkEvaluatorNode* ) anode;

  this->AutoUpdateMeasurementRange = node->AutoUpdateMeasurementRange;
  this->MarkBegin = node->MarkBegin;
  this->MarkEnd = node->MarkEnd;
  this->NeedleOrientation = node->NeedleOrientation;
  this->PlaybackTime = node->PlaybackTime;
  this->AnalysisState = node->AnalysisState;
  this->RealTimeProcessing = node->RealTimeProcessing;
}



// Constructors and Destructors --------------------------------------------------------------------

vtkMRMLPerkEvaluatorNode
::vtkMRMLPerkEvaluatorNode()
{
  this->AutoUpdateMeasurementRange = true;

  this->MarkBegin = 0.0;
  this->MarkEnd = 0.0;
  
  this->NeedleOrientation = vtkMRMLPerkEvaluatorNode::PlusZ; // Default needle orientation protocol

  this->PlaybackTime = 0.0;
  this->AnalysisState = -1;

  this->RealTimeProcessing = false;

  this->AddNodeReferenceRole( TRANSFORM_BUFFER_REFERENCE_ROLE );
  this->AddNodeReferenceRole( METRICS_TABLE_REFERENCE_ROLE );
  this->AddNodeReferenceRole( METRIC_INSTANCE_REFERENCE_ROLE );

  // Setup for "old-style" attributes
  this->MetricsDirectory = "";
}


vtkMRMLPerkEvaluatorNode
::~vtkMRMLPerkEvaluatorNode()
{
}


// Getters and setters -----------------------------------------------------------------------------


bool vtkMRMLPerkEvaluatorNode
::GetAutoUpdateMeasurementRange()
{
  return this->AutoUpdateMeasurementRange;
}


void vtkMRMLPerkEvaluatorNode
::SetAutoUpdateMeasurementRange( bool update )
{
  if ( update != this->AutoUpdateMeasurementRange )
  {
    this->AutoUpdateMeasurementRange = update;
    this->Modified();
  }
}


// Let the user set whatever values the want for MarkEnd and MarkBegin
// If they are too large/small, that is ok. Only analyze within the range.
double vtkMRMLPerkEvaluatorNode
::GetMarkBegin()
{
  return this->MarkBegin;
}


void vtkMRMLPerkEvaluatorNode
::SetMarkBegin( double begin )
{
  if ( begin != this->MarkBegin )
  {
    this->MarkBegin = begin;
    this->Modified();
  }
}


double vtkMRMLPerkEvaluatorNode
::GetMarkEnd()
{
  return this->MarkEnd;
}


void vtkMRMLPerkEvaluatorNode
::SetMarkEnd( double end )
{
  if ( end != this->MarkEnd )
  {
    this->MarkEnd = end;
    this->Modified();
  }
}


void vtkMRMLPerkEvaluatorNode
::GetNeedleOrientation( double needleOrientation[ 3 ] )
{
  // Note: The base is in the opposite direction of the pointing, so the signs will be opposite
  if ( this->NeedleOrientation == vtkMRMLPerkEvaluatorNode::PlusX )
  {
    needleOrientation[ 0 ] = 1; needleOrientation[ 1 ] = 0; needleOrientation[ 2 ] = 0;
  }
  if ( this->NeedleOrientation == vtkMRMLPerkEvaluatorNode::MinusX )
  {
    needleOrientation[ 0 ] = -1; needleOrientation[ 1 ] = 0; needleOrientation[ 2 ] = 0;
  }
  if ( this->NeedleOrientation == vtkMRMLPerkEvaluatorNode::PlusY )
  {
    needleOrientation[ 0 ] = 0; needleOrientation[ 1 ] = 1; needleOrientation[ 2 ] = 0;
  }
  if ( this->NeedleOrientation == vtkMRMLPerkEvaluatorNode::MinusY )
  {
    needleOrientation[ 0 ] = 0; needleOrientation[ 1 ] = -1; needleOrientation[ 2 ] = 0;
  }
  if ( this->NeedleOrientation == vtkMRMLPerkEvaluatorNode::PlusZ )
  {
    needleOrientation[ 0 ] = 0; needleOrientation[ 1 ] = 0; needleOrientation[ 2 ] = 1;
  }
  if ( this->NeedleOrientation == vtkMRMLPerkEvaluatorNode::MinusZ )
  {
    needleOrientation[ 0 ] = 0; needleOrientation[ 1 ] = 0; needleOrientation[ 2 ] = -1;
  }
}


vtkMRMLPerkEvaluatorNode::NeedleOrientationEnum vtkMRMLPerkEvaluatorNode
::GetNeedleOrientation()
{
  return this->NeedleOrientation;
}


void vtkMRMLPerkEvaluatorNode
::SetNeedleOrientation( vtkMRMLPerkEvaluatorNode::NeedleOrientationEnum newNeedleOrientation )
{
  if ( newNeedleOrientation != this->NeedleOrientation )
  {
    this->NeedleOrientation = newNeedleOrientation;
    this->Modified();
  }
}


double vtkMRMLPerkEvaluatorNode
::GetPlaybackTime()
{
  return this->PlaybackTime;
}


void vtkMRMLPerkEvaluatorNode
::SetPlaybackTime( double newPlaybackTime, bool analysis )
{
  if ( newPlaybackTime != this->PlaybackTime )
  {
    this->PlaybackTime = newPlaybackTime;
    if ( ! analysis )
    {
      this->Modified();
    }
  }
}


int vtkMRMLPerkEvaluatorNode
::GetAnalysisState()
{
  return this->AnalysisState;
}


void vtkMRMLPerkEvaluatorNode
::SetAnalysisState( int newAnalysisState )
{
  if ( newAnalysisState != this->AnalysisState )
  {
    this->AnalysisState = newAnalysisState;
    this->InvokeEvent( AnalysisStateUpdatedEvent, &this->AnalysisState );
  }
}


bool vtkMRMLPerkEvaluatorNode
::GetRealTimeProcessing()
{
  return this->RealTimeProcessing;
}


void vtkMRMLPerkEvaluatorNode
::SetRealTimeProcessing( bool newRealTimeProcessing )
{
  if ( newRealTimeProcessing != this->RealTimeProcessing )
  {
    this->RealTimeProcessing = newRealTimeProcessing;
    this->Modified();
    if ( newRealTimeProcessing )
    {
      this->InvokeEvent( RealTimeProcessingStartedEvent );
    }
  }
}


// Metric scripts ------------------------------------------------------------------------------------------------


void vtkMRMLPerkEvaluatorNode
::AddMetricInstanceID( std::string metricInstanceID )
{
  if ( ! this->IsMetricInstanceID( metricInstanceID ) )
  {
    this->AddAndObserveNodeReferenceID( METRIC_INSTANCE_REFERENCE_ROLE, metricInstanceID.c_str() );
  }
}


void vtkMRMLPerkEvaluatorNode
::RemoveMetricInstanceID( std::string transformID )
{
  // Check all referenced node IDs
  for ( int i = 0; i < this->GetNumberOfNodeReferences( METRIC_INSTANCE_REFERENCE_ROLE ); i++ )
  {
    if ( transformID.compare( this->GetNthNodeReferenceID( METRIC_INSTANCE_REFERENCE_ROLE, i ) ) == 0 )
    {
      this->RemoveNthNodeReferenceID( METRIC_INSTANCE_REFERENCE_ROLE, i );
      i--;
    }
  }
}


std::vector< std::string > vtkMRMLPerkEvaluatorNode
::GetMetricInstanceIDs()
{
  std::vector< std::string > metricInstanceIDs;

  // Check all referenced node IDs
  for ( int i = 0; i < this->GetNumberOfNodeReferences( METRIC_INSTANCE_REFERENCE_ROLE ); i++ )
  {
    metricInstanceIDs.push_back( this->GetNthNodeReferenceID( METRIC_INSTANCE_REFERENCE_ROLE, i ) );
  }

  return metricInstanceIDs;
}


void vtkMRMLPerkEvaluatorNode
::SetMetricInstanceIDs( std::vector< std::string > metricInstanceIDs )
{
  // Remove all of the active transform IDs
  this->RemoveNodeReferenceIDs( METRIC_INSTANCE_REFERENCE_ROLE );

  // Add all of the specified IDs
  for ( int i = 0; i < metricInstanceIDs.size(); i++ )
  {
    this->AddAndObserveNodeReferenceID( METRIC_INSTANCE_REFERENCE_ROLE, metricInstanceIDs.at( i ).c_str() );
  }
}

bool vtkMRMLPerkEvaluatorNode
::IsMetricInstanceID( std::string metricInstanceID )
{
  // Check all referenced node IDs
  for ( int i = 0; i < this->GetNumberOfNodeReferences( METRIC_INSTANCE_REFERENCE_ROLE ); i++ )
  {
    if ( metricInstanceID.compare( this->GetNthNodeReferenceID( METRIC_INSTANCE_REFERENCE_ROLE, i ) ) == 0 )
    {
      return true;
    }
  }

  return false;
}


// Transform buffer/metrics table References ----------------------------------------------------------------------

std::string vtkMRMLPerkEvaluatorNode
::GetNodeReferenceIDString( std::string referenceRole )
{
  const char* refID = this->GetNodeReferenceID( referenceRole.c_str() );
  std::string refIDString;

  if ( refID == NULL )
  {
    refIDString = "";
  }
  else
  {
    refIDString = refID;
  }

  return refIDString;
}


vtkMRMLTransformBufferNode* vtkMRMLPerkEvaluatorNode
::GetTransformBufferNode()
{
  return vtkMRMLTransformBufferNode::SafeDownCast( this->GetNodeReference( TRANSFORM_BUFFER_REFERENCE_ROLE ) );
}


std::string vtkMRMLPerkEvaluatorNode
::GetTransformBufferID()
{
  return this->GetNodeReferenceIDString( TRANSFORM_BUFFER_REFERENCE_ROLE );
}


void vtkMRMLPerkEvaluatorNode
::SetTransformBufferID( std::string newTransformBufferID )
{
  vtkNew< vtkIntArray > events;
  events->InsertNextValue( vtkMRMLTransformBufferNode::TransformAddedEvent );
  events->InsertNextValue( vtkMRMLTransformBufferNode::RecordingStateChangedEvent );
  events->InsertNextValue( vtkMRMLTransformBufferNode::ActiveTransformAddedEvent );
  events->InsertNextValue( vtkMRMLTransformBufferNode::ActiveTransformRemovedEvent );
  this->SetAndObserveNodeReferenceID( TRANSFORM_BUFFER_REFERENCE_ROLE, newTransformBufferID.c_str(), events.GetPointer() );

  // Auto-update as necessary
  if ( this->GetTransformBufferNode() == NULL )
  {
    return;
  }

  // Playback time
  this->SetPlaybackTime( this->GetTransformBufferNode()->GetMinimumTime() ); // Set to minimum for convenience

  // Measurement range
  if ( this->GetAutoUpdateMeasurementRange() )
  {
    this->SetMarkBegin( 0.0 );
    this->SetMarkEnd( this->GetTransformBufferNode()->GetTotalTime() );
  }
}


vtkMRMLTableNode* vtkMRMLPerkEvaluatorNode
::GetMetricsTableNode()
{
  return vtkMRMLTableNode::SafeDownCast( this->GetNodeReference( METRICS_TABLE_REFERENCE_ROLE ) );
}


std::string vtkMRMLPerkEvaluatorNode
::GetMetricsTableID()
{
  return this->GetNodeReferenceIDString( METRICS_TABLE_REFERENCE_ROLE );
}


void vtkMRMLPerkEvaluatorNode
::SetMetricsTableID( std::string newMetricsTableID )
{
  this->SetAndObserveNodeReferenceID( METRICS_TABLE_REFERENCE_ROLE, newMetricsTableID.c_str() );
}


// MRML node event processing -----------------------------------------------------------------

void vtkMRMLPerkEvaluatorNode
::ProcessMRMLEvents( vtkObject *caller, unsigned long event, void *callData )
{
  this->vtkMRMLTransformableNode::ProcessMRMLEvents( caller, event, callData );

  // The caller will be the node that was modified
  vtkMRMLTransformBufferNode* transformBuffer = vtkMRMLTransformBufferNode::SafeDownCast( caller );
  if ( transformBuffer == NULL )
  {
    return;
  }

  // Recording state of buffer changed
  if ( event == vtkMRMLTransformBufferNode::RecordingStateChangedEvent )
  { 
    bool* eventData = reinterpret_cast< bool* >( callData );
    if ( *eventData == false )
    {
      this->SetRealTimeProcessing( false ); // Stop real-time processing if recording has stopped (note: real-time processing should not necessarily be started whenever recording is started)
    }
  }

  // Transform added to buffer
  if ( event == vtkMRMLTransformBufferNode::TransformAddedEvent && this->RealTimeProcessing )
  { 
    vtkMRMLTransformBufferNode::TransformEventDataType* eventData = reinterpret_cast< vtkMRMLTransformBufferNode::TransformEventDataType* >( callData );
    if ( transformBuffer->GetTransformRecordBuffer( eventData->first )->GetNumRecords() == eventData->second + 1 )
    {
      this->InvokeEvent( TransformRealTimeAddedEvent, &eventData->first );
    }
  }

}