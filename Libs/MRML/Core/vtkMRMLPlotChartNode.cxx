/*==============================================================================

  Copyright (c) Kapteyn Astronomical Institute
  University of Groningen, Groningen, Netherlands. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Davide Punzo, Kapteyn Astronomical Institute,
  and was supported through the European Research Council grant nr. 291531.

==============================================================================*/

#include <sstream>
#include <map>
#include <string>

// VTK includes
#include <vtkCollection.h>
#include <vtkCommand.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPlot.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkTable.h>

// MRML includes
#include "vtkMRMLPlotChartNode.h"
#include "vtkMRMLPlotSeriesNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLTableNode.h"

const char* vtkMRMLPlotChartNode::PlotSeriesNodeReferenceRole = "plotSeries";

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLPlotChartNode);

//----------------------------------------------------------------------------
vtkMRMLPlotChartNode::vtkMRMLPlotChartNode()
: Title(NULL)
, TitleFontSize(20)
, TitleVisibility(true)
, GridVisibility(true)
, LegendVisibility(true)
, XAxisTitle(NULL)
, XAxisTitleVisibility(true)
, YAxisTitle(NULL)
, YAxisTitleVisibility(true)
, AxisTitleFontSize(16)
, AxisLabelFontSize(12)
, FontType(NULL)
, EnablePointMoveAlongX(true)
, EnablePointMoveAlongY(true)
{
  this->HideFromEditors = 0;

  this->SetFontType("Arial");

  vtkNew<vtkIntArray>  events;
  events->InsertNextValue(vtkCommand::ModifiedEvent);
  events->InsertNextValue(vtkMRMLPlotChartNode::PlotModifiedEvent);
  events->InsertNextValue(vtkMRMLPlotSeriesNode::TableModifiedEvent);
  this->AddNodeReferenceRole(this->GetPlotSeriesNodeReferenceRole(), NULL, events.GetPointer());
}


//----------------------------------------------------------------------------
vtkMRMLPlotChartNode::~vtkMRMLPlotChartNode()
{
}

//----------------------------------------------------------------------------
const char* vtkMRMLPlotChartNode::GetPlotSeriesNodeReferenceRole()
{
  return vtkMRMLPlotChartNode::PlotSeriesNodeReferenceRole;
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::OnNodeReferenceAdded(vtkMRMLNodeReference *reference)
{
  this->Superclass::OnNodeReferenceAdded(reference);
  if (std::string(reference->GetReferenceRole()) == this->PlotSeriesNodeReferenceRole)
    {
    this->InvokeEvent(vtkMRMLPlotChartNode::PlotModifiedEvent, reference->GetReferencedNode());
    }
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::OnNodeReferenceModified(vtkMRMLNodeReference *reference)
{
  this->Superclass::OnNodeReferenceModified(reference);
  if (std::string(reference->GetReferenceRole()) == this->PlotSeriesNodeReferenceRole)
    {
    this->InvokeEvent(vtkMRMLPlotChartNode::PlotModifiedEvent, reference->GetReferencedNode());
    }
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::OnNodeReferenceRemoved(vtkMRMLNodeReference *reference)
{
  this->Superclass::OnNodeReferenceRemoved(reference);
  if (std::string(reference->GetReferenceRole()) == this->PlotSeriesNodeReferenceRole)
    {
    this->InvokeEvent(vtkMRMLPlotChartNode::PlotModifiedEvent, reference->GetReferencedNode());
    }
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::WriteXML(ostream& of, int nIndent)
{
  // Start by having the superclass write its information
  Superclass::WriteXML(of, nIndent);

  vtkMRMLWriteXMLBeginMacro(of);
  vtkMRMLWriteXMLStringMacro(title, Title);
  vtkMRMLWriteXMLIntMacro(titleFontSize, TitleFontSize);
  vtkMRMLWriteXMLBooleanMacro(TitleVisibility, TitleVisibility);
  vtkMRMLWriteXMLBooleanMacro(gridVisibility, GridVisibility);
  vtkMRMLWriteXMLBooleanMacro(legendVisibility, LegendVisibility);
  vtkMRMLWriteXMLStringMacro(xAxisTitle, XAxisTitle);
  vtkMRMLWriteXMLBooleanMacro(xAxisTitleVisibility, XAxisTitleVisibility);
  vtkMRMLWriteXMLStringMacro(yAxisTitle, YAxisTitle);
  vtkMRMLWriteXMLBooleanMacro(yAxisTitleVisibility, YAxisTitleVisibility);
  vtkMRMLWriteXMLIntMacro(axisTitleFontSize, AxisTitleFontSize);
  vtkMRMLWriteXMLIntMacro(axisLabelFontSize, AxisLabelFontSize);
  vtkMRMLWriteXMLStringMacro(fontType, FontType);
  vtkMRMLWriteXMLBooleanMacro(enablePointMoveAlongX, EnablePointMoveAlongX);
  vtkMRMLWriteXMLBooleanMacro(enablePointMoveAlongY, EnablePointMoveAlongY);
  vtkMRMLWriteXMLEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::ReadXMLAttributes(const char** atts)
{
  int disabledModify = this->StartModify();

  vtkMRMLNode::ReadXMLAttributes(atts);

  vtkMRMLReadXMLBeginMacro(atts);
  vtkMRMLReadXMLStringMacro(title, Title);
  vtkMRMLReadXMLIntMacro(titleFontSize, TitleFontSize);
  vtkMRMLReadXMLBooleanMacro(TitleVisibility, TitleVisibility);
  vtkMRMLReadXMLBooleanMacro(gridVisibility, GridVisibility);
  vtkMRMLReadXMLBooleanMacro(legendVisibility, LegendVisibility);
  vtkMRMLReadXMLStringMacro(xAxisTitle, XAxisTitle);
  vtkMRMLReadXMLBooleanMacro(xAxisTitleVisibility, XAxisTitleVisibility);
  vtkMRMLReadXMLStringMacro(yAxisTitle, YAxisTitle);
  vtkMRMLReadXMLBooleanMacro(yAxisTitleVisibility, YAxisTitleVisibility);
  vtkMRMLReadXMLIntMacro(axisTitleFontSize, AxisTitleFontSize);
  vtkMRMLReadXMLIntMacro(axisLabelFontSize, AxisLabelFontSize);
  vtkMRMLReadXMLStringMacro(fontType, FontType);
  vtkMRMLReadXMLBooleanMacro(enablePointMoveAlongX, EnablePointMoveAlongX);
  vtkMRMLReadXMLBooleanMacro(enablePointMoveAlongY, EnablePointMoveAlongY);
  vtkMRMLReadXMLEndMacro();

  this->EndModify(disabledModify);
}


//----------------------------------------------------------------------------
// Copy the node's attributes to this object.
void vtkMRMLPlotChartNode::Copy(vtkMRMLNode *anode)
{
  int disabledModify = this->StartModify();

  Superclass::Copy(anode);

  vtkMRMLCopyBeginMacro(anode);
  vtkMRMLCopyStringMacro(Title);
  vtkMRMLCopyIntMacro(TitleFontSize);
  vtkMRMLCopyBooleanMacro(TitleVisibility);
  vtkMRMLCopyBooleanMacro(GridVisibility);
  vtkMRMLCopyBooleanMacro(LegendVisibility);
  vtkMRMLCopyStringMacro(XAxisTitle);
  vtkMRMLCopyBooleanMacro(XAxisTitleVisibility);
  vtkMRMLCopyStringMacro(YAxisTitle);
  vtkMRMLCopyBooleanMacro(YAxisTitleVisibility);
  vtkMRMLCopyIntMacro(AxisTitleFontSize);
  vtkMRMLCopyIntMacro(AxisLabelFontSize);
  vtkMRMLCopyStringMacro(FontType);
  vtkMRMLCopyBooleanMacro(EnablePointMoveAlongX);
  vtkMRMLCopyBooleanMacro(EnablePointMoveAlongY);
  vtkMRMLCopyEndMacro();

  this->EndModify(disabledModify);
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkMRMLNode::PrintSelf(os,indent);

  vtkMRMLPrintBeginMacro(os, indent);
  vtkMRMLPrintStringMacro(Title);
  vtkMRMLPrintIntMacro(TitleFontSize);
  vtkMRMLPrintBooleanMacro(TitleVisibility);
  vtkMRMLPrintBooleanMacro(GridVisibility);
  vtkMRMLPrintBooleanMacro(LegendVisibility);
  vtkMRMLPrintStringMacro(XAxisTitle);
  vtkMRMLPrintBooleanMacro(XAxisTitleVisibility);
  vtkMRMLPrintStringMacro(YAxisTitle);
  vtkMRMLPrintBooleanMacro(YAxisTitleVisibility);
  vtkMRMLPrintIntMacro(AxisTitleFontSize);
  vtkMRMLPrintIntMacro(AxisLabelFontSize);
  vtkMRMLPrintStringMacro(FontType);
  vtkMRMLPrintBooleanMacro(EnablePointMoveAlongX);
  vtkMRMLPrintBooleanMacro(EnablePointMoveAlongY);
  vtkMRMLPrintEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::SetAndObservePlotSeriesNodeID(const char *plotSeriesNodeID)
{
  this->SetAndObserveNodeReferenceID(this->GetPlotSeriesNodeReferenceRole(), plotSeriesNodeID);
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::AddAndObservePlotSeriesNodeID(const char *plotSeriesNodeID)
{
  this->AddAndObserveNodeReferenceID(this->GetPlotSeriesNodeReferenceRole(), plotSeriesNodeID);
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::RemovePlotSeriesNodeID(const char *plotSeriesNodeID)
{
  if (!plotSeriesNodeID)
    {
    return;
    }

  this->RemoveNthPlotSeriesNodeID(this->GetPlotSeriesNodeIndexFromID(plotSeriesNodeID));
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::RemoveNthPlotSeriesNodeID(int n)
{
  this->RemoveNthNodeReferenceID(this->GetPlotSeriesNodeReferenceRole(), n);
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::RemoveAllPlotSeriesNodeIDs()
{
  this->RemoveNodeReferenceIDs(this->GetPlotSeriesNodeReferenceRole());
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::SetAndObserveNthPlotSeriesNodeID(int n, const char *plotSeriesNodeID)
{
  this->SetAndObserveNthNodeReferenceID(this->GetPlotSeriesNodeReferenceRole(), n, plotSeriesNodeID);
}

//----------------------------------------------------------------------------
bool vtkMRMLPlotChartNode::HasPlotSeriesNodeID(const char* plotSeriesNodeID)
{
  return this->HasNodeReferenceID(this->GetPlotSeriesNodeReferenceRole(), plotSeriesNodeID);
}

//----------------------------------------------------------------------------
int vtkMRMLPlotChartNode::GetNumberOfPlotSeriesNodes()
{
  return this->GetNumberOfNodeReferences(this->GetPlotSeriesNodeReferenceRole());
}

//----------------------------------------------------------------------------
const char* vtkMRMLPlotChartNode::GetNthPlotSeriesNodeID(int n)
{
    return this->GetNthNodeReferenceID(this->GetPlotSeriesNodeReferenceRole(), n);
}

//----------------------------------------------------------------------------
int vtkMRMLPlotChartNode::GetPlotSeriesNodeIndexFromID(const char *plotSeriesNodeID)
{
  if (!plotSeriesNodeID)
    {
    return -1;
    }

  int numPlotSeriesNodes = this->GetNumberOfNodeReferences(
    this->GetPlotSeriesNodeReferenceRole());

  for (int plotIndex = 0; plotIndex < numPlotSeriesNodes; plotIndex++)
    {
    const char* id = this->GetNthNodeReferenceID(
      this->GetPlotSeriesNodeReferenceRole(), plotIndex);
    if (!strcmp(plotSeriesNodeID, id))
      {
      return plotIndex;
      break;
      }
    }

  return -1;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPlotChartNode::GetPlotSeriesNodeID()
{
  return this->GetNthPlotSeriesNodeID(0);
}

//----------------------------------------------------------------------------
vtkMRMLPlotSeriesNode* vtkMRMLPlotChartNode::GetNthPlotSeriesNode(int n)
{
  return vtkMRMLPlotSeriesNode::SafeDownCast(
    this->GetNthNodeReference(this->GetPlotSeriesNodeReferenceRole(), n));
}

//----------------------------------------------------------------------------
vtkMRMLPlotSeriesNode* vtkMRMLPlotChartNode::GetPlotSeriesNode()
{
  return this->GetNthPlotSeriesNode(0);
}

//----------------------------------------------------------------------------
void vtkMRMLPlotChartNode::ProcessMRMLEvents(vtkObject *caller,
                                              unsigned long event,
                                              void *callData)
{
  Superclass::ProcessMRMLEvents(caller, event, callData);

  int numPlotSeriesNodes = this->GetNumberOfNodeReferences(this->GetPlotSeriesNodeReferenceRole());
  for (int plotIndex = 0; plotIndex < numPlotSeriesNodes; plotIndex++)
    {
    vtkMRMLPlotSeriesNode *pnode = this->GetNthPlotSeriesNode(plotIndex);
    if (pnode != NULL && pnode == vtkMRMLPlotSeriesNode::SafeDownCast(caller) &&
       (event ==  vtkCommand::ModifiedEvent || event == vtkMRMLPlotSeriesNode::TableModifiedEvent))
      {
      this->InvokeEvent(vtkMRMLPlotChartNode::PlotModifiedEvent, pnode);
      this->Modified();
      }
    }

  return;
}

//----------------------------------------------------------------------------
int vtkMRMLPlotChartNode::GetPlotSeriesNodeNames(std::vector<std::string> &plotSeriesNodeNames)
{
  plotSeriesNodeNames.clear();
  int numPlotSeriesNodes = this->GetNumberOfNodeReferences(this->GetPlotSeriesNodeReferenceRole());
  for (int plotIndex = 0; plotIndex < numPlotSeriesNodes; plotIndex++)
    {
    vtkMRMLPlotSeriesNode *pnode = this->GetNthPlotSeriesNode(plotIndex);
    if (!pnode)
      {
      continue;
      }
    plotSeriesNodeNames.push_back(pnode->GetName());
    }

  return static_cast<int>(plotSeriesNodeNames.size());
}

//----------------------------------------------------------------------------
int vtkMRMLPlotChartNode::GetPlotSeriesNodeIDs(std::vector<std::string> &plotSeriesNodeIDs)
{
  plotSeriesNodeIDs.clear();
  int numPlotSeriesNodes = this->GetNumberOfNodeReferences(this->GetPlotSeriesNodeReferenceRole());
  for (int plotIndex = 0; plotIndex < numPlotSeriesNodes; plotIndex++)
    {
    plotSeriesNodeIDs.push_back(this->GetNthPlotSeriesNodeID(plotIndex));
    }

  return static_cast<int>(plotSeriesNodeIDs.size());
}

// --------------------------------------------------------------------------
void vtkMRMLPlotChartNode::SetPropertyToAllPlotSeriesNodes(PlotSeriesNodeProperty plotProperty, const char* value)
{
  if (!this->GetScene())
    {
    vtkErrorMacro("vtkMRMLPlotChartNode::SetPropertyToAllPlotSeriesNodes failed: invalid scene");
    return;
    }

  int numPlotSeriesNodes = this->GetNumberOfNodeReferences(this->GetPlotSeriesNodeReferenceRole());

  std::vector<int> plotSeriesNodesWasModifying(numPlotSeriesNodes, 0);

  // Update all plot nodes and invoke modified events at the end

  for (int plotIndex = 0; plotIndex < numPlotSeriesNodes; plotIndex++)
    {
    vtkMRMLPlotSeriesNode *plotSeriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(this->GetNthNodeReference(this->GetPlotSeriesNodeReferenceRole(), plotIndex));
    if (!plotSeriesNode)
      {
      continue;
      }
    plotSeriesNodesWasModifying[plotIndex] = plotSeriesNode->StartModify();

    if (plotProperty == PlotType)
      {
      plotSeriesNode->SetPlotType(value);
      }
    else if (plotProperty == PlotXColumnName)
      {
      plotSeriesNode->SetXColumnName(value);
      }
    else if (plotProperty == PlotYColumnName)
      {
      plotSeriesNode->SetYColumnName(value);
      }
    else if (plotProperty == PlotMarkerStyle)
      {
      plotSeriesNode->SetMarkerStyle(plotSeriesNode->GetMarkerStyleFromString(value));
      }
    }

  for (int plotIndex = 0; plotIndex < numPlotSeriesNodes; plotIndex++)
    {
    vtkMRMLPlotSeriesNode *plotSeriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(this->GetNthNodeReference(this->GetPlotSeriesNodeReferenceRole(), plotIndex));
    if (!plotSeriesNode)
      {
      continue;
      }
    plotSeriesNode->EndModify(plotSeriesNodesWasModifying[plotIndex]);
  }
}

// --------------------------------------------------------------------------
bool vtkMRMLPlotChartNode::GetPropertyFromAllPlotSeriesNodes(PlotSeriesNodeProperty plotProperty, std::string& value)
{
  value.clear();
  if (!this->GetScene())
    {
    vtkErrorMacro("vtkMRMLPlotChartNode::GetPropertyFromAllPlotSeriesNodes failed: invalid scene");
    return false;
    }

  int numPlotSeriesNodes = this->GetNumberOfNodeReferences(this->GetPlotSeriesNodeReferenceRole());

  if (numPlotSeriesNodes < 1)
    {
    return false;
    }

  bool commonPropertyDefined = false;

  for (int plotIndex = 0; plotIndex < numPlotSeriesNodes; plotIndex++)
    {
    vtkMRMLPlotSeriesNode *plotSeriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(this->GetNthNodeReference(this->GetPlotSeriesNodeReferenceRole(), plotIndex));
    if (!plotSeriesNode)
      {
      continue;
      }

    // Get property value
    std::string propertyValue;
    if (plotProperty == PlotType)
      {
      propertyValue = plotSeriesNode->GetPlotTypeAsString(plotSeriesNode->GetPlotType());
      }
    else if (plotProperty == PlotXColumnName)
      {
      propertyValue = plotSeriesNode->GetXColumnName();
      }
    else if (plotProperty == PlotYColumnName)
      {
      propertyValue = plotSeriesNode->GetYColumnName();
      }
    else if (plotProperty == PlotMarkerStyle)
      {
      propertyValue = plotSeriesNode->GetMarkerStyleAsString(plotSeriesNode->GetMarkerStyle());
      }

    if (commonPropertyDefined)
      {
      if (propertyValue != value)
        {
        // not all plot nodes have the same property value
        return false;
        }
      }
    else
      {
      commonPropertyDefined = true;
      value = propertyValue;
      }
    }

  return true;
}
